#include "DCO.h"
#include <cmath>

DCO::DCO()
    : rng (std::random_device{}())
{}

void DCO::setSampleRate (double sr) { sampleRate = sr; }

void DCO::setFrequency (float hz)   { frequency  = hz; }

void DCO::setRange (int range)
{
    switch (range)
    {
        case 0: rangeOctave = -1; break;
        case 1: rangeOctave =  0; break;
        case 2: rangeOctave =  1; break;
        case 3: rangeOctave =  2; break;
        default: rangeOctave = 0; break;
    }
}

void DCO::setPulseWidth (float pw)
{
    pulseWidth = std::max (0.05f, std::min (pw, 0.95f));
}

float DCO::process()
{
    const float freq = frequency * std::pow (2.0f, (float)rangeOctave);
    const float dt   = freq / (float)sampleRate;
    float out  = 0.0f;
    float norm = 0.0f;

    if (sawEnabled)
    {
        float saw = 2.0f * phase - 1.0f;
        saw -= polyBlep (phase, dt);
        out  += saw;
        norm += 1.0f;
    }

    if (pulseEnabled)
    {
        float p = (phase < pulseWidth) ? 1.0f : -1.0f;
        p += polyBlep (phase, dt);
        p -= polyBlep (std::fmod (phase + 1.0f - pulseWidth, 1.0f), dt);
        out  += p;
        norm += 1.0f;
    }

    if (subType > 0)
    {
        float sub = (subPhase < 0.5f) ? 1.0f : -1.0f;
        out  += sub * 0.70f;
        norm += 0.70f;
    }

    if (noiseEnabled)
    {
        out  += noiseDist (rng);
        norm += 1.0f;
    }

    phase += dt;
    if (phase >= 1.0f) phase -= 1.0f;

    const float subDt = (subType == 2) ? dt * 0.25f : dt * 0.5f;
    subPhase += subDt;
    if (subPhase >= 1.0f) subPhase -= 1.0f;

    return (norm > 0.0f) ? (out / std::sqrt (norm)) : 0.0f;
}

void DCO::reset()
{
    phase    = 0.0f;
    subPhase = 0.0f;
}

void DCO::sync()
{
    phase    = 0.0f;
    subPhase = 0.0f;
}


float DCO::polyBlep (float t, float dt) const
{
    if (dt <= 0.0f) return 0.0f;

    if (t < dt)
    {
        const float x = t / dt;
        return x + x - x * x - 1.0f;
    }
    if (t > 1.0f - dt)
    {
        const float x = (t - 1.0f) / dt;
        return x * x + x + x + 1.0f;
    }
    return 0.0f;
}
