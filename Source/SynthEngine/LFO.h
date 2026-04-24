#pragma once
#include <cmath>

class LFO
{
public:
    enum class Waveform { Triangle, Square, Sawtooth };

    LFO();

    void setSampleRate (double sr);
    void setRate       (float hz);
    void setDelay      (float seconds);
    void setWaveform   (Waveform w) { waveform = w; }

    void  noteOn();
    float process();

    void reset();

private:
    double   sampleRate   = 44100.0;
    float    rateHz       = 1.0f;
    float    phase        = 0.0f;
    Waveform waveform     = Waveform::Triangle;

    float delayTotal   = 0.0f;
    float delayCounter = 0.0f;
    float envelope     = 1.0f;
};
