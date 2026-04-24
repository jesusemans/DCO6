#include "Delay.h"

StereoDelay::StereoDelay()
{
    bufL.assign (kMaxSeconds * 192001, 0.0f);
    bufR.assign (kMaxSeconds * 192001, 0.0f);
}

void StereoDelay::setSampleRate (double sr)
{
    sampleRate = sr;
    const int bufSize = (int)(sr * kMaxSeconds) + 1;
    bufL.assign (bufSize, 0.0f);
    bufR.assign (bufSize, 0.0f);
    writePos = 0;
    setTime (delayTime);
}

void StereoDelay::setTime (float seconds)
{
    delayTime = std::clamp (seconds, 0.01f, (float)kMaxSeconds - 0.01f);
    delayInSamples = std::clamp ((int)(delayTime * (float)sampleRate),
                                  1, (int)bufL.size() - 1);
}

void StereoDelay::setFeedback (float fb)
{
    feedback = std::clamp (fb, 0.0f, 0.95f);
}

void StereoDelay::setMix (float m)
{
    mix = std::clamp (m, 0.0f, 1.0f);
}

void StereoDelay::process (float* left, float* right, int numSamples)
{
    if (mix < 0.001f) return;
    const int bufSize = (int)bufL.size();
    for (int i = 0; i < numSamples; ++i)
    {
        const int readPos = (writePos - delayInSamples + bufSize) % bufSize;
        const float dL = bufL[readPos];
        const float dR = bufR[readPos];
        bufL[writePos] = left[i]  + dL * feedback;
        bufR[writePos] = right[i] + dR * feedback;
        left[i]  = left[i]  + dL * mix;
        right[i] = right[i] + dR * mix;
        writePos = (writePos + 1) % bufSize;
    }
}

void StereoDelay::reset()
{
    std::fill (bufL.begin(), bufL.end(), 0.0f);
    std::fill (bufR.begin(), bufR.end(), 0.0f);
    writePos = 0;
}
