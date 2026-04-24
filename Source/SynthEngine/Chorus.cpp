#include "Chorus.h"
#include <cmath>
#include <algorithm>

static constexpr float kTwoPi = 6.28318530f;

Chorus::Chorus()
{
    buf.assign (kBufSize, 0.0f);
}

void Chorus::setSampleRate (double sr) { sampleRate = sr; }

void Chorus::process (float* left, float* right, int numSamples, int mode)
{
    if (mode == 0)
    {
        for (int i = 0; i < numSamples; ++i)
            right[i] = left[i];
        return;
    }

    const float baseDelay = (kBaseDelayMs / 1000.0f) * (float)sampleRate;
    const float modDepth  = (kModDepthMs  / 1000.0f) * (float)sampleRate;
    const float inc1      = kRate1Hz / (float)sampleRate;
    const float inc2      = kRate2Hz / (float)sampleRate;

    for (int i = 0; i < numSamples; ++i)
    {
        const float dry = left[i];

        buf[writePos & (kBufSize - 1)] = dry;

        const float mod1 = std::sin (lfoPhase1 * kTwoPi);
        const float wet1 = readAt (baseDelay + mod1 * modDepth);

        if (mode == 1)
        {
            left[i]  = (dry + wet1) * 0.5f;
            right[i] = (dry - wet1) * 0.5f;
        }
        else
        {
            const float mod2 = std::sin (lfoPhase2 * kTwoPi);
            const float wet2 = readAt (baseDelay + mod2 * modDepth);

            left[i]  = (dry + wet1) * 0.5f;
            right[i] = (dry + wet2) * 0.5f;
        }

        writePos = (writePos + 1) & (kBufSize - 1);

        lfoPhase1 += inc1;
        if (lfoPhase1 >= 1.0f) lfoPhase1 -= 1.0f;

        if (mode == 2)
        {
            lfoPhase2 += inc2;
            if (lfoPhase2 >= 1.0f) lfoPhase2 -= 1.0f;
        }
    }
}

void Chorus::reset()
{
    std::fill (buf.begin(), buf.end(), 0.0f);
    writePos   = 0;
    lfoPhase1  = 0.0f;
    lfoPhase2  = 0.25f;
}

float Chorus::readAt (float delaySamples) const
{
    delaySamples = std::clamp (delaySamples, 1.0f, (float)(kBufSize - 2));

    const float readF = (float)writePos - delaySamples;
    const float rPos  = readF - std::floor (readF / (float)kBufSize) * (float)kBufSize;

    const int   ri0  = (int)rPos & (kBufSize - 1);
    const int   ri1  = (ri0 + 1) & (kBufSize - 1);
    const float frac = rPos - std::floor (rPos);

    return buf[ri0] + frac * (buf[ri1] - buf[ri0]);
}
