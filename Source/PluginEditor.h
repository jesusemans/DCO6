#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class DCO6LookAndFeel final : public juce::LookAndFeel_V4
{
public:
    DCO6LookAndFeel();

    void drawRotarySlider (juce::Graphics&, int x, int y, int w, int h,
                           float sliderPos, float startAngle, float endAngle,
                           juce::Slider&) override;

    void drawLinearSlider (juce::Graphics&, int x, int y, int w, int h,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           juce::Slider::SliderStyle, juce::Slider&) override;

    void drawToggleButton (juce::Graphics&, juce::ToggleButton&,
                           bool highlighted, bool down) override;

    juce::Font getLabelFont (juce::Label&) override;
};

class NeonLookAndFeel final : public juce::LookAndFeel_V4
{
public:
    NeonLookAndFeel();

    void drawRotarySlider (juce::Graphics&, int x, int y, int w, int h,
                           float sliderPos, float startAngle, float endAngle,
                           juce::Slider&) override;

    void drawLinearSlider (juce::Graphics&, int x, int y, int w, int h,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           juce::Slider::SliderStyle, juce::Slider&) override;

    void drawToggleButton (juce::Graphics&, juce::ToggleButton&,
                           bool highlighted, bool down) override;

    juce::Font getLabelFont (juce::Label&) override;
};

class SpectrumAnalyzerComponent final : public juce::Component
{
public:
    explicit SpectrumAnalyzerComponent (SynthProcessor& p) : proc (p) {}
    void paint (juce::Graphics&) override;
    bool isNeon = false;

private:
    SynthProcessor& proc;
};

class SynthEditor final : public juce::AudioProcessorEditor,
                          private juce::Timer
{
public:
    explicit SynthEditor (SynthProcessor&);
    ~SynthEditor() override;

    void paint   (juce::Graphics&) override;
    void resized () override;

private:
    void timerCallback() override;
    void setSkin (int skin);

    SynthProcessor& processor;
    DCO6LookAndFeel laf;
    NeonLookAndFeel neonLaf;
    int currentSkin = 0;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    using ComboAttachment  = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    juce::Label    titleLabel;
    juce::ComboBox presetBox;
    juce::TextButton skinToggleBtn;

    juce::Slider       pitchWheelSlider, modWheelSlider;
    juce::Label        pitchWheelLabel,  modWheelLabel;

    juce::ComboBox     dcoRangeBox, dcoSubBox;
    juce::Slider       dcoPwmKnob, dcoPwmLfoKnob, dcoLfoPitchKnob;
    juce::ToggleButton dcoBtnSaw, dcoBtnPulse, dcoBtnNoise;
    std::unique_ptr<SliderAttachment> sDcoPwm, sDcoPwmLfo, sDcoLfoPitch;
    std::unique_ptr<ButtonAttachment> bSaw, bPulse, bNoise;
    std::unique_ptr<ComboAttachment>  cDcoRange, cDcoSub;

    juce::Slider hpfKnob;
    std::unique_ptr<SliderAttachment> sHpf;

    juce::Slider vcfCutoffKnob, vcfResKnob, vcfEnvKnob, vcfLfoKnob, vcfKeyKnob;
    std::unique_ptr<SliderAttachment> sVcfCut, sVcfRes, sVcfEnv, sVcfLfo, sVcfKey;

    juce::Slider fAttFader, fDecFader, fSusFader, fRelFader;
    std::unique_ptr<SliderAttachment> sFAtt, sFDec, sFSus, sFRel;

    juce::Slider       aAttFader, aDecFader, aSusFader, aRelFader;
    juce::ToggleButton vcaGateBtn;
    std::unique_ptr<SliderAttachment> sAAtt, sADec, sASus, sARel;
    std::unique_ptr<ButtonAttachment> bVcaGate;

    juce::Slider   lfoRateKnob, lfoDelayKnob;
    juce::ComboBox lfoWaveBox;
    std::unique_ptr<SliderAttachment> sLfoRate, sLfoDelay;
    std::unique_ptr<ComboAttachment>  cLfoWave;

    juce::Slider delayTimeKnob, delayFbKnob, delayMixKnob;
    std::unique_ptr<SliderAttachment> sDelayTime, sDelayFb, sDelayMix;

    juce::Slider reverbRoomKnob, reverbDampKnob, reverbMixKnob;
    std::unique_ptr<SliderAttachment> sRevRoom, sRevDamp, sRevMix;

    juce::Slider   volumeKnob, portaKnob, bendRangeSlider;
    juce::ComboBox chorusBox;
    std::unique_ptr<SliderAttachment> sVol, sPorta, sBendRange;
    std::unique_ptr<ComboAttachment>  cChorus;

    juce::Slider eqLowKnob, eqMidKnob, eqHighKnob;
    std::unique_ptr<SliderAttachment> sEqLow, sEqMid, sEqHigh;

    SpectrumAnalyzerComponent spectrumComp;

    juce::MidiKeyboardComponent keyboard;

    std::vector<std::unique_ptr<juce::Label>> labels;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthEditor)
};