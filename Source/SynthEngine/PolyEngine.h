#pragma once
#include <JuceHeader.h>
#include "SynthParams.h"
#include "Voice.h"
#include "Chorus.h"
#include "Delay.h"

class PolyEngine
{
public:
    PolyEngine();

    void prepare (double sampleRate, int samplesPerBlock);
    void process (juce::AudioBuffer<float>& audio,
                  juce::MidiBuffer&         midi,
                  const SynthParams&        params);
    void reset();

    static constexpr int kNumVoices  = 6;
    static constexpr int kFFTOrder   = 10;
    static constexpr int kFFTSize    = 1 << kFFTOrder;
    static constexpr int kSpecBins   = kFFTSize / 2;

    std::array<float, kSpecBins> specBins {};

    struct Biquad
    {
        float b0=1,b1=0,b2=0,a1=0,a2=0,z1=0,z2=0;
        void setLowShelf  (double sr, float fc, float gainDB);
        void setHighShelf (double sr, float fc, float gainDB);
        void setPeakEQ    (double sr, float fc, float Q, float gainDB);
        float tick (float x) noexcept
        {
            const float y = b0*x + z1;
            z1 = b1*x - a1*y + z2;
            z2 = b2*x - a2*y;
            return y;
        }
        void reset() noexcept { z1 = z2 = 0.0f; }
    };

private:
    juce::Synthesiser synth;
    Chorus            chorus;
    StereoDelay       delay;
    juce::Reverb      reverb;

    std::array<SynthVoice*, kNumVoices> voices {};

    std::array<Biquad, 2> bqLow, bqMid, bqHigh;
    float eqCachedLow  = -999.f;
    float eqCachedMid  = -999.f;
    float eqCachedHigh = -999.f;

    juce::dsp::FFT fft { kFFTOrder };
    std::array<float, kFFTSize * 2> fftData {};
    int    fifoPos          = 0;
    double engineSampleRate = 44100.0;
};
