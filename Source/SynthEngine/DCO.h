#pragma once
#include <cmath>
#include <random>

class DCO
{
public:
    DCO();

    void setSampleRate (double sr);
    void setFrequency  (float hz);
    void setRange      (int range);
    void setPulseWidth (float pw);
    void setSawEnabled   (bool on)  { sawEnabled   = on;   }
    void setPulseEnabled (bool on)  { pulseEnabled = on;   }
    void setSubType      (int type) { subType      = type; }
    void setNoiseEnabled (bool on)  { noiseEnabled = on;   }

    float process();
    void  reset();
    void  sync();

private:
    float polyBlep (float t, float dt) const;

    double sampleRate  = 44100.0;
    float  frequency   = 440.0f;
    float  phase       = 0.0f;
    float  subPhase    = 0.0f;
    float  pulseWidth  = 0.5f;
    int    rangeOctave = 0;

    bool   sawEnabled   = true;
    bool   pulseEnabled = false;
    int    subType      = 0;
    bool   noiseEnabled = false;

    std::mt19937                          rng;
    std::uniform_real_distribution<float> noiseDist { -1.0f, 1.0f };
};
