#pragma once
#include <JuceHeader.h>
#include "SynthEngine/PolyEngine.h"
#include "SynthEngine/SynthParams.h"

class SynthProcessor final : public juce::AudioProcessor,
                              public juce::AsyncUpdater
{
public:
    SynthProcessor();
    ~SynthProcessor() override = default;

    void prepareToPlay  (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock   (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "DCO6"; }
    bool acceptsMidi()  const override { return true;  }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }

    double getTailLengthSeconds() const override { return 2.0; }

    int  getNumPrograms()    override { return (int)presetNames.size(); }
    int  getCurrentProgram() override { return currentPreset; }
    void setCurrentProgram  (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& dest) override;
    void setStateInformation (const void* data, int size) override;
    void handleAsyncUpdate() override;

    juce::AudioProcessorValueTreeState apvts;
    juce::MidiKeyboardState            keyboardState;

    std::atomic<float> displayPitchBend { 0.5f };
    std::atomic<float> displayModWheel  { 0.0f };

    const float* getSpectrumData()    const noexcept { return engine.specBins.data(); }
    static constexpr int kSpectrumBins = PolyEngine::kSpecBins;

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    SynthParams buildSynthParams() const;
    void        handleMidiCC (int cc, int value);
    void        applyPreset  (int index);
    void        buildPresets();

    PolyEngine engine;

    int currentPreset = 0;

    juce::StringArray presetNames;

    struct PresetParam { const char* id; float value; };
    std::vector<std::vector<PresetParam>> presets;

    std::array<std::atomic<float>, 128> ccPending;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthProcessor)
};
