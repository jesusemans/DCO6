#include "Filter.h"
#include <cmath>
#include <algorithm>

Filter::Filter()
{
    updateCoefficients();
}

void Filter::setSampleRate (double sr)
{
    sampleRate = sr;
    updateCoefficients();
}

void Filter::setCutoffHz (float hz)
{
    cutoffHz = std::clamp (hz, 20.0f, 20000.0f);
    updateCoefficients();
}

void Filter::setResonance (float res)
{
    resonance = std::clamp (res, 0.0f, 1.0f);
    updateCoefficients();
}

float Filter::process (float input)
{
    float x = input - r * y4;

    const float y1n = y1 + f * (tanhApprox (x)  - t1);
    const float y2n = y2 + f * (t1              - t2);
    const float y3n = y3 + f * (t2              - t3);
    const float y4n = y4 + f * (t3 - tanhApprox (y4));

    t1 = tanhApprox (y1n);
    t2 = tanhApprox (y2n);
    t3 = tanhApprox (y3n);

    y1 = std::clamp (y1n, -4.0f, 4.0f);
    y2 = std::clamp (y2n, -4.0f, 4.0f);
    y3 = std::clamp (y3n, -4.0f, 4.0f);
    y4 = std::clamp (y4n, -4.0f, 4.0f);

    return y4;
}

void Filter::reset()
{
    y1 = y2 = y3 = y4 = 0.0f;
    t1 = t2 = t3 = 0.0f;
}

void Filter::updateCoefficients()
{
    const float fc = std::clamp (cutoffHz / (float)sampleRate, 0.0001f, 0.49f);
    f = 2.0f * std::sin (3.14159265f * fc);
    r = resonance * 3.80f;
}

float Filter::tanhApprox (float x) noexcept
{
    if (x >  3.0f) return  1.0f;
    if (x < -3.0f) return -1.0f;
    const float x2 = x * x;
    return x * (27.0f + x2) / (27.0f + 9.0f * x2);
}
