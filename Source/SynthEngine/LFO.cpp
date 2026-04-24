#include "LFO.h"
#include <cmath>
#include <algorithm>

LFO::LFO() {}

void LFO::setSampleRate (double sr) { sampleRate = sr; }

void LFO::setRate (float hz)
{
    rateHz = std::max (0.01f, hz);
}

void LFO::setDelay (float seconds)
{
    delayTotal = std::max (0.0f, seconds) * (float)sampleRate;
}

void LFO::noteOn()
{
    delayCounter = 0.0f;
    envelope     = (delayTotal <= 0.0f) ? 1.0f : 0.0f;
}

float LFO::process()
{
    if (delayCounter < delayTotal)
    {
        delayCounter += 1.0f;
        envelope      = delayCounter / delayTotal;
    }
    else
    {
        envelope = 1.0f;
    }

    float value = 0.0f;
    if (waveform == Waveform::Triangle)
    {
        value = (phase < 0.5f) ? (4.0f * phase - 1.0f)
                               : (3.0f - 4.0f * phase);
    }
    else if (waveform == Waveform::Square)
    {
        value = (phase < 0.5f) ? 1.0f : -1.0f;
    }
    else
    {
        value = 1.0f - 2.0f * phase;
    }

    phase += rateHz / (float)sampleRate;
    if (phase >= 1.0f) phase -= 1.0f;

    return value * envelope;
}

void LFO::reset()
{
    phase        = 0.0f;
    delayCounter = 0.0f;
    envelope     = 1.0f;
}
