#pragma once
#include <vector>
#include <cmath>

class Chorus
{
public:
    Chorus();

    void setSampleRate (double sr);
    void process (float* left, float* right, int numSamples, int mode);
    void reset();

private:
    float readAt (float delaySamples) const;

    static constexpr int   kBufSize      = 8192;
    static constexpr float kBaseDelayMs  = 6.0f;
    static constexpr float kModDepthMs   = 2.8f;
    static constexpr float kRate1Hz      = 0.513f;
    static constexpr float kRate2Hz      = 0.863f;

    double             sampleRate = 44100.0;
    std::vector<float> buf;
    int                writePos   = 0;

    float lfoPhase1 = 0.0f;
    float lfoPhase2 = 0.25f;
};
