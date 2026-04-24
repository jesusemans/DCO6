#pragma once
#include <JuceHeader.h>
#include "SynthParams.h"
#include "ADSR.h"
#include "DCO.h"
#include "Filter.h"
#include "LFO.h"

class SynthVoice final : public juce::SynthesiserVoice
{
public:
    SynthVoice();

    bool canPlaySound (juce::SynthesiserSound*) override { return true; }

    void startNote (int midiNote,
                    float velocity,
                    juce::SynthesiserSound*,
                    int pitchWheelPosition) override;

    void stopNote   (float velocity, bool allowTailOff) override;
    void pitchWheelMoved (int newValue) override;
    void controllerMoved (int controller, int value) override;

    void renderNextBlock (juce::AudioBuffer<float>& outputBuffer,
                          int startSample,
                          int numSamples) override;

    void setParams (const SynthParams& p);

private:
    float midiNoteToHz (int note, float pitchBendSemitones = 0.0f) const;

    SynthParams params;

    EnvGen    ampEnv;
    EnvGen    filterEnv;
    DCO       dco;
    Filter    vcf;
    OnePoleHPF hpf;
    LFO       lfo;

    int   noteNumber    = 60;
    float velocity      = 0.0f;
    float pitchBendSemi = 0.0f;
    float modWheelValue = 0.0f;

    float currentFreqHz = 440.0f;
    float targetFreqHz  = 440.0f;
    float portaCoeff    = 0.0f;

    float noteFreqHz    = 440.0f;
};

struct SynthSound final : public juce::SynthesiserSound
{
    bool appliesToNote    (int) override { return true; }
    bool appliesToChannel (int) override { return true; }
};
