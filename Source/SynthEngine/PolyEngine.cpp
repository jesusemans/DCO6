#include "PolyEngine.h"
#include <cmath>

void PolyEngine::Biquad::setLowShelf (double sr, float fc, float gainDB)
{
    const float A     = std::pow (10.0f, gainDB / 40.0f);
    const float w0    = juce::MathConstants<float>::twoPi * fc / (float)sr;
    const float cosW  = std::cos (w0);
    const float sinW  = std::sin (w0);
    const float alpha = sinW / juce::MathConstants<float>::sqrt2;
    const float sqA   = std::sqrt (A);
    const float a0    = (A+1) + (A-1)*cosW + 2*sqA*alpha;
    b0 =  A * ((A+1) - (A-1)*cosW + 2*sqA*alpha) / a0;
    b1 =  2*A * ((A-1) - (A+1)*cosW)             / a0;
    b2 =  A * ((A+1) - (A-1)*cosW - 2*sqA*alpha) / a0;
    a1 = -2 * ((A-1) + (A+1)*cosW)               / a0;
    a2 =      ((A+1) + (A-1)*cosW - 2*sqA*alpha) / a0;
}

void PolyEngine::Biquad::setHighShelf (double sr, float fc, float gainDB)
{
    const float A     = std::pow (10.0f, gainDB / 40.0f);
    const float w0    = juce::MathConstants<float>::twoPi * fc / (float)sr;
    const float cosW  = std::cos (w0);
    const float sinW  = std::sin (w0);
    const float alpha = sinW / juce::MathConstants<float>::sqrt2;
    const float sqA   = std::sqrt (A);
    const float a0    = (A+1) - (A-1)*cosW + 2*sqA*alpha;
    b0 =   A * ((A+1) + (A-1)*cosW + 2*sqA*alpha) / a0;
    b1 = -2*A * ((A-1) + (A+1)*cosW)              / a0;
    b2 =   A * ((A+1) + (A-1)*cosW - 2*sqA*alpha) / a0;
    a1 =   2 * ((A-1) - (A+1)*cosW)               / a0;
    a2 =       ((A+1) - (A-1)*cosW - 2*sqA*alpha) / a0;
}

void PolyEngine::Biquad::setPeakEQ (double sr, float fc, float Q, float gainDB)
{
    const float A     = std::pow (10.0f, gainDB / 40.0f);
    const float w0    = juce::MathConstants<float>::twoPi * fc / (float)sr;
    const float cosW  = std::cos (w0);
    const float alpha = std::sin (w0) / (2.0f * Q);
    const float a0    = 1.0f + alpha / A;
    b0 = (1.0f + alpha * A) / a0;
    b1 = (-2.0f * cosW)     / a0;
    b2 = (1.0f - alpha * A) / a0;
    a1 = (-2.0f * cosW)     / a0;
    a2 = (1.0f - alpha / A) / a0;
}

PolyEngine::PolyEngine()
{
    synth.addSound (new SynthSound());

    for (int i = 0; i < kNumVoices; ++i)
    {
        auto* v = new SynthVoice();
        voices[i] = v;
        synth.addVoice (v);
    }
}

void PolyEngine::prepare (double sampleRate, int samplesPerBlock)
{
    engineSampleRate = sampleRate;
    synth.setCurrentPlaybackSampleRate (sampleRate);
    chorus.setSampleRate (sampleRate);
    delay.setSampleRate  (sampleRate);
    reverb.reset();

    for (auto& bq : bqLow)  bq.reset();
    for (auto& bq : bqMid)  bq.reset();
    for (auto& bq : bqHigh) bq.reset();
    eqCachedLow = eqCachedMid = eqCachedHigh = -999.f;

    fifoPos = 0;
    fftData.fill (0.0f);
    specBins.fill (0.0f);

    SynthParams defaultParams;
    for (auto* v : voices)
        v->setParams (defaultParams);

    juce::ignoreUnused (samplesPerBlock);
}

void PolyEngine::process (juce::AudioBuffer<float>& audio,
                          juce::MidiBuffer&         midi,
                          const SynthParams&        params)
{
    for (auto* v : voices)
        v->setParams (params);

    audio.clear();
    synth.renderNextBlock (audio, midi, 0, audio.getNumSamples());

    const int numSamples = audio.getNumSamples();
    const int numCh      = audio.getNumChannels();

    if (numCh >= 2)
    {
        float* L = audio.getWritePointer (0);
        float* R = audio.getWritePointer (1);

        chorus.process (L, R, numSamples, params.chorusMode);

        delay.setTime     (params.delayTime);
        delay.setFeedback (params.delayFeedback);
        delay.setMix      (params.delayMix);
        delay.process (L, R, numSamples);

        juce::Reverb::Parameters rp;
        rp.roomSize   = params.reverbRoom;
        rp.damping    = params.reverbDamp;
        rp.wetLevel   = params.reverbMix;
        rp.dryLevel   = 1.0f - params.reverbMix * 0.5f;
        rp.width      = 1.0f;
        rp.freezeMode = 0.0f;
        reverb.setParameters (rp);
        reverb.processStereo (L, R, numSamples);

        // 3-band EQ (update coefficients only when parameters change)
        if (params.eqLowGain  != eqCachedLow)  { eqCachedLow  = params.eqLowGain;  for (auto& bq : bqLow)  bq.setLowShelf  (engineSampleRate, 200.0f,  params.eqLowGain); }
        if (params.eqMidGain  != eqCachedMid)  { eqCachedMid  = params.eqMidGain;  for (auto& bq : bqMid)  bq.setPeakEQ    (engineSampleRate, 1000.0f, 0.8f, params.eqMidGain); }
        if (params.eqHighGain != eqCachedHigh) { eqCachedHigh = params.eqHighGain; for (auto& bq : bqHigh) bq.setHighShelf (engineSampleRate, 5000.0f, params.eqHighGain); }

        for (int i = 0; i < numSamples; ++i)
        {
            L[i] = bqHigh[0].tick (bqMid[0].tick (bqLow[0].tick (L[i])));
            R[i] = bqHigh[1].tick (bqMid[1].tick (bqLow[1].tick (R[i])));
        }

        // Tanh soft-clip limiter – keeps signal in (-1, +1) with musical saturation
        for (int i = 0; i < numSamples; ++i)
        {
            L[i] = std::tanh (L[i]);
            R[i] = std::tanh (R[i]);
        }

        // Spectrum FFT FIFO (mono mix)
        for (int i = 0; i < numSamples; ++i)
        {
            fftData[fifoPos] = (L[i] + R[i]) * 0.5f;
            if (++fifoPos >= kFFTSize)
            {
                for (int j = 0; j < kFFTSize; ++j)
                    fftData[j] *= 0.5f - 0.5f * std::cos (juce::MathConstants<float>::twoPi * j / (float)(kFFTSize - 1));
                std::fill (fftData.begin() + kFFTSize, fftData.end(), 0.0f);
                fft.performFrequencyOnlyForwardTransform (fftData.data());
                const float norm   = 2.0f / (float)kFFTSize;
                const float smooth = 0.75f;
                for (int j = 0; j < kSpecBins; ++j)
                    specBins[j] = smooth * specBins[j] + (1.0f - smooth) * fftData[j] * norm;
                fifoPos = 0;
            }
        }
    }
    else if (numCh == 1)
    {
        if (params.chorusMode == 0)
            return;
        std::vector<float> right (numSamples, 0.0f);
        std::copy (audio.getReadPointer (0),
                   audio.getReadPointer (0) + numSamples,
                   right.data());
        chorus.process (audio.getWritePointer (0), right.data(),
                        numSamples, params.chorusMode);
    }
}

void PolyEngine::reset()
{
    synth.allNotesOff (0, false);
    chorus.reset();
    delay.reset();
    reverb.reset();
}
