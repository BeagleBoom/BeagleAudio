#include "maximilian/maximilian.h"
#include "maximilian/libs/maxiGrains.h"
#include <cmath>
#include <EventQueue/MessageQueue.h>
#include <EventQueue/EventType.h>

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

static void *userInput_thread(void *) {
    MessageQueue queue = MessageQueue(10);
    while (true) {
        Event e = queue.receive();
        switch (e.getType()) {
            case EventType::ADC_VALUES:
                for (unsigned int i = 1; i < e.getPayloadCount(); i += 2) {
                    int channel = e.getInt(i);
                    int value = e.getInt(i + 1);
                    switch (channel) {
                        case 2://CV 1
                            rate = value;
                            break;
                        case 6://Gain 1
                            trigger = value;
                            break;
                        case 3://CV 2
                            //rate2 = value;
                            break;
                        case 7://Gain 2

                            break;
                    }
                }
                refresh = true;
                break;
        }
    }
}

void setup() {//some inits

    beats.load(
            "/Users/torbenhartmann/Downloads/audiocheck.net_sin_1000Hz_-3dBFS_3s (1).wav");//load in your samples. Provide the full path to a wav file.
    //printf("Summary:\n%s", beats.getSummary());//get info on samples if you like.

    float max = 1 << 16;

    std::cout << max << std::endl;

    for (int i = 0; i < beats.myDataSize / 8; i++) {
        std::cout << (beats.temp[i] / max) << " ";
    }

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
int baseNote = 81;//A5
int n = std::floor(49 + 12 * std::log2(base / 440));
float b = 440 * std::pow(2.0, (n + rate - 49) / 12.0);
float pitch = b / base;


void play(double *output) {//this is where the magic happens. Very slow magic.
    double myCurrentVolume = myEnvelope.adsr(1., myEnvelope.trigger);
    //i++;
    if (refresh) {
        refresh = false;
        rate += 1;
        b = 440 * std::pow(2.0, (n + rate - 49) / 12.0);
        pitch = b / base;
        myEnvelope.trigger = trigger;
        //std::cout << b << std::endl;
    }
    output[0] = shift->play(pitch, 1, 0.4, 2, 0.0);
    output[1] = output[0];
}