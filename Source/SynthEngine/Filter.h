#pragma once
#include <cmath>

class Filter
{
public:
    Filter();

    void setSampleRate (double sr);
    void setCutoffHz   (float hz);
    void setResonance  (float res);

    float process (float input);
    void  reset();

private:
    void  updateCoefficients();
    static float tanhApprox (float x) noexcept;

    double sampleRate  = 44100.0;
    float  cutoffHz    = 8000.0f;
    float  resonance   = 0.0f;

    float y1 = 0.0f, y2 = 0.0f, y3 = 0.0f, y4 = 0.0f;
    float t1 = 0.0f, t2 = 0.0f, t3 = 0.0f;

    float f = 0.0f;
    float r = 0.0f;
};

class OnePoleHPF
{
public:
    void setSampleRate (double sr) { sampleRate = sr; }

    void setCutoffNorm (float c)
    {
        if (c <= 0.001f) { active = false; return; }
        active = true;
        const float wc = 2.0f * 3.14159265f * (c * (float)sampleRate * 0.05f) / (float)sampleRate;
        a = (2.0f - wc) / (2.0f + wc);
    }

    float process (float x)
    {
        if (!active) return x;
        const float out = a * (prev_out + x - prev_in);
        prev_in  = x;
        prev_out = out;
        return out;
    }

    void reset() { prev_in = prev_out = 0.0f; }

private:
    double sampleRate = 44100.0;
    float  a          = 0.0f;
    float  prev_in    = 0.0f;
    float  prev_out   = 0.0f;
    bool   active     = false;
};
