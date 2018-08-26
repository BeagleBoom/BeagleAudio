# ![alt text](docs/beagleAudio_logo.png "BeagleAudio")
BeagleAudio plays a soundfile in different pitches based on incomming events. It currently only supports wave files as input. The maxim library is used for pitching and to apply an envelope to the sound.

# Parameters
The program has to be started with two parameters:
- recipient id: the id on which the message queue should be listening for events.
- wave location: the absolute path to the wave file which should be played.

# Waveform
At startup the software will echo out the waveform information of the loaded file to stdout. This information is used by the menu to render the waveform in the UI.

# Used events
| Event      | Direction | Parameters                                                                | Purpose                                                                                                                                                      |
|------------|-----------|---------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------|
| ADC_VALUES | incoming  | alternating: `channel_number, value`. e.g. `CH1=1,CH2=5.4 => [1,1,2,5.4]` | Used to trigger the playback of the sound and change the pitch.                                                                                              |
| ADSR       | incoming | `[A,D,S,R]`                                                               | Used to set the envelope values for Attack, Decay, Sustain, Release. A,D and R need to be milliseconds as whole numbers. S is a float value between 0 and 1. |

# About this repository
This repository is part of the [BeagleBoom](https://github.com/beagleboom)-Sampler project developed by the academic working group "[Creative Technologies AG](http://www.creative-technologies.de/)" (ctag) at the [University of Applied Sciences Kiel](https://www.fh-kiel.de/).

# License
The content and source code of this project itself is licensed under the [Creative Commons Attribution 4.0 International license](https://creativecommons.org/licenses/by/4.0/).