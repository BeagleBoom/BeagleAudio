#include "maximilian/maximilian.h"
#include "maximilian/libs/maxiGrains.h"
#include <cmath>
#include <EventQueue/MessageQueue.h>
#include <EventQueue/EventType.h>
#include <memory>

maxiOsc mySine, myPhasor; // This is the oscillator we will use to generate the test tone
float data[256];
int _index = 0;
maxiEnv myEnvelope;
maxiSample beats; //We give our sample a name. It's called beats this time. We could have loads of them, but they have to have different names. shift;
std::shared_ptr<maxiTimePitchStretch<hannWinFunctor, maxiSample>> shift;
int i = 0;
volatile int rate = -20;
volatile int trigger = 0;
volatile int rate2 = -20;
volatile bool refresh = false;


volatile bool refreshADSR = false;
volatile int adsr_attack = 0;
volatile int adsr_decay = 1;
volatile float adsr_sustain = 1;
volatile int adsr_release = 1000;
int receipent = 1;

static void *userInput_thread(void *) {
    MessageQueue queue = MessageQueue(receipent);
    bool doBreak = false;
    while (true) {
        std::cout << "LOOP" << std::endl;
        if (doBreak) {
            break;
        }
        std::cout << "WAIT" << std::endl;
        Event e = queue.receive();
        //std::cout << "EVENT " << e.getPayloadCount() << std::endl;
        switch (e.getType()) {
            case EventType::ADC_VALUES:
                for (unsigned int i = 0; i < e.getPayloadCount(); i += 2) {
                    int channel = e.getInt(i);
                    int value = e.getInt(i + 1);
                    switch (channel) {
                        case 2://CV 1
                            rate = value;
                            break;
                        case 6://Gate 1
                            trigger = value;
                            break;
                        case 3://CV 2
                            //rate2 = value;
                            break;
                        case 7://Gate 2

                            break;
                    }
                }
                refresh = true;
                break;
            case EventType::ADSR:
                adsr_attack = e.getInt(0);
                adsr_decay = e.getInt(1);
                adsr_sustain = e.getFloat(2);
                adsr_release = e.getInt(3);
                refreshADSR = true;
                break;
        }
    }
}

std::string fileName = "";

void setup(std::vector<std::string> arguments) {//some inits
    if (arguments.size() != 2) {
        std::cout << "Please start the Process with: receipentID waveLocation" << std::endl;
        exit(1);
    }


    receipent = std::stoi(arguments[0]);
    fileName = arguments[1];

    beats.load(fileName);

    float max = 1 << 16;

    std::cout << "##WAVE_START##" << std::endl;
    for (int i = 0; i < beats.myDataSize / 8; i++) {
        std::cout << (beats.temp[i] / max) << " ";
    }

    std::cout << "##WAVE_END##" << std::endl;
    shift = std::make_shared<maxiTimePitchStretch<hannWinFunctor, maxiSample>>(&beats);
    std::cout << std::endl << beats.myBitsPerSample;

    myEnvelope.setAttack(0);
    myEnvelope.setDecay(1);  // Needs to be at least 1
    myEnvelope.setSustain(1);
    myEnvelope.setRelease(1000);

    pthread_t tId;
    (void) pthread_create(&tId, 0, userInput_thread, 0);
}

float base = 1000;
int baseNote = 12;//A5
int n = std::floor(49 + 12 * std::log2(base / 440));
float b = 440 * std::pow(2.0, (n + rate - 49) / 12.0);
float pitch = b / base;


void play(double *output) {//this is where the magic happens. Very slow magic.
    //i++;
    if (refresh) {
        std::cout << "REFRESH" << std::endl;
        refresh = false;
        //rate += 1;
        b = 440 * std::pow(2.0, (rate - 49) / 12.0);
        pitch = b / base;
        myEnvelope.trigger = trigger;
    }
    if (refreshADSR) {
        myEnvelope.setAttack(adsr_attack);
        myEnvelope.setDecay(adsr_decay);  // Needs to be at least 1
        myEnvelope.setSustain(adsr_sustain);
        myEnvelope.setRelease(adsr_release);
        refreshADSR = false;
    }

    double myCurrentVolume = myEnvelope.adsr(1., myEnvelope.trigger);

    if (myCurrentVolume > 0) {
        output[0] = shift->play(pitch, 1, 0.4, 2, 0.0) * myCurrentVolume;
    } else {
        output[0] = 0;
    }

    output[1] = output[0];
}