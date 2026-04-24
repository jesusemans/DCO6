#pragma once
#include <vector>
#include <cmath>
#include <algorithm>

class StereoDelay
{
public:
    StereoDelay();

    void setSampleRate (double sr);
    void setTime       (float seconds);
    void setFeedback   (float fb);
    void setMix        (float mix);
    void process       (float* left, float* right, int numSamples);
    void reset();

private:
    static constexpr int kMaxSeconds = 3;

    double sampleRate     = 44100.0;
    float  delayTime      = 0.3f;
    float  feedback       = 0.35f;
    float  mix            = 0.0f;
    int    delayInSamples = 13230;

    std::vector<float> bufL, bufR;
    int writePos = 0;
};
