#include "PluginEditor.h"
#include <cmath>

static constexpr int BASE_W = 1150;
static constexpr int BASE_H = 610;

DCO6LookAndFeel::DCO6LookAndFeel()
{
    setColour (juce::Slider::thumbColourId,              juce::Colours::black);
    setColour (juce::Slider::rotarySliderFillColourId,   juce::Colour (0xFFDDDDDD));
    setColour (juce::Slider::rotarySliderOutlineColourId,juce::Colours::black);
    setColour (juce::Slider::trackColourId,              juce::Colour (0xFFAAAAAA));
    setColour (juce::Slider::backgroundColourId,         juce::Colour (0xFFCCCCCC));
    setColour (juce::Label::textColourId,                juce::Colours::black);
    setColour (juce::ComboBox::backgroundColourId,       juce::Colours::white);
    setColour (juce::ComboBox::textColourId,             juce::Colours::black);
    setColour (juce::ComboBox::outlineColourId,          juce::Colours::black);
    setColour (juce::PopupMenu::backgroundColourId,      juce::Colours::white);
    setColour (juce::PopupMenu::textColourId,            juce::Colours::black);
}

void DCO6LookAndFeel::drawRotarySlider (juce::Graphics& g,
                                         int x, int y, int w, int h,
                                         float sliderPos,
                                         float startAngle, float endAngle,
                                         juce::Slider&)
{
    const float radius  = (float)std::min (w, h) * 0.5f - 3.0f;
    const float cx      = (float)x + (float)w * 0.5f;
    const float cy      = (float)y + (float)h * 0.5f;
    const float angle   = startAngle + sliderPos * (endAngle - startAngle);

    g.setColour (juce::Colours::white);
    g.fillEllipse (cx - radius, cy - radius, radius * 2.0f, radius * 2.0f);
    g.setColour (juce::Colours::black);
    g.drawEllipse (cx - radius, cy - radius, radius * 2.0f, radius * 2.0f, 2.0f);

    const float lineLen = radius * 0.65f;
    const float lx      = cx + lineLen * std::sin (angle);
    const float ly      = cy - lineLen * std::cos (angle);
    g.drawLine (cx, cy, lx, ly, 2.5f);
}

void DCO6LookAndFeel::drawLinearSlider (juce::Graphics& g,
                                         int x, int y, int w, int h,
                                         float sliderPos, float, float,
                                         juce::Slider::SliderStyle style,
                                         juce::Slider&)
{
    if (style == juce::Slider::LinearVertical)
    {
        const float cx = (float)x + (float)w * 0.5f;
        g.setColour (juce::Colour (0xFF999999));
        g.fillRect (cx - 1.0f, (float)y, 2.0f, (float)h);
        g.setColour (juce::Colours::black);
        g.drawRect ((int)(cx - 1.0f), y, 2, h, 1);

        const float thumbH = 12.0f;
        const float thumbW = (float)w - 4.0f;
        const float tx = (float)x + 2.0f;
        const float ty = sliderPos - thumbH * 0.5f;
        g.setColour (juce::Colours::black);
        g.fillRect (tx, ty, thumbW, thumbH);
        g.setColour (juce::Colours::white);
        g.drawLine (tx + 2.0f, ty + thumbH * 0.5f,
                    tx + thumbW - 2.0f, ty + thumbH * 0.5f, 1.5f);
    }
    else
    {
        const float cy = (float)y + (float)h * 0.5f;
        g.setColour (juce::Colour (0xFF999999));
        g.fillRect ((float)x, cy - 1.0f, (float)w, 2.0f);
        g.setColour (juce::Colours::black);
        g.drawRect (x, (int)(cy - 1.0f), w, 2, 1);

        const float thumbW = 12.0f;
        const float thumbH = (float)h - 4.0f;
        const float tx = sliderPos - thumbW * 0.5f;
        const float ty = (float)y + 2.0f;
        g.setColour (juce::Colours::black);
        g.fillRect (tx, ty, thumbW, thumbH);
    }
}

void DCO6LookAndFeel::drawToggleButton (juce::Graphics& g,
                                         juce::ToggleButton& btn,
                                         bool, bool)
{
    const bool on = btn.getToggleState();
    const auto b  = btn.getLocalBounds().toFloat().reduced (1.0f);

    g.setColour (on ? juce::Colours::black : juce::Colour (0xFFE0E0E0));
    g.fillRoundedRectangle (b, 3.0f);
    g.setColour (juce::Colours::black);
    g.drawRoundedRectangle (b, 3.0f, 1.0f);

    g.setColour (on ? juce::Colours::white : juce::Colours::black);
    g.setFont (juce::Font (10.0f, juce::Font::bold));
    g.drawText (btn.getButtonText(), btn.getLocalBounds(), juce::Justification::centred);
}

juce::Font DCO6LookAndFeel::getLabelFont (juce::Label&)
{
    return juce::Font (10.0f, juce::Font::bold);
}

//==============================================================================
// Neon skin
//==============================================================================

static const juce::Colour kNeonBg    { 0xFF0A0A1F };
static const juce::Colour kNeonCyan  { 0xFF00E5FF };
static const juce::Colour kNeonPink  { 0xFFFF00C8 };
static const juce::Colour kNeonPanel { 0xFF0F0F28 };

NeonLookAndFeel::NeonLookAndFeel()
{
    setColour (juce::Slider::thumbColourId,                        kNeonPink);
    setColour (juce::Slider::rotarySliderFillColourId,             kNeonBg);
    setColour (juce::Slider::rotarySliderOutlineColourId,          kNeonCyan);
    setColour (juce::Slider::trackColourId,                        kNeonCyan);
    setColour (juce::Slider::backgroundColourId,                   kNeonBg);
    setColour (juce::Label::textColourId,                          kNeonCyan);
    setColour (juce::ComboBox::backgroundColourId,                 kNeonBg);
    setColour (juce::ComboBox::textColourId,                       kNeonCyan);
    setColour (juce::ComboBox::outlineColourId,                    kNeonCyan);
    setColour (juce::ComboBox::arrowColourId,                      kNeonCyan);
    setColour (juce::PopupMenu::backgroundColourId,                kNeonPanel);
    setColour (juce::PopupMenu::textColourId,                      kNeonCyan);
    setColour (juce::PopupMenu::highlightedBackgroundColourId,     juce::Colour (0xFF1E1E42));
    setColour (juce::PopupMenu::highlightedTextColourId,           kNeonPink);
    setColour (juce::TextButton::buttonColourId,                   kNeonBg);
    setColour (juce::TextButton::textColourOffId,                  kNeonCyan);
    setColour (juce::TextButton::textColourOnId,                   kNeonPink);
}

void NeonLookAndFeel::drawRotarySlider (juce::Graphics& g,
                                         int x, int y, int w, int h,
                                         float sliderPos,
                                         float startAngle, float endAngle,
                                         juce::Slider&)
{
    const float radius = (float)std::min (w, h) * 0.5f - 3.0f;
    const float cx     = (float)x + (float)w * 0.5f;
    const float cy     = (float)y + (float)h * 0.5f;
    const float angle  = startAngle + sliderPos * (endAngle - startAngle);

    g.setColour (kNeonCyan.withAlpha (0.18f));
    g.fillEllipse (cx-radius-3, cy-radius-3, (radius+3)*2.0f, (radius+3)*2.0f);

    g.setColour (kNeonBg);
    g.fillEllipse (cx-radius, cy-radius, radius*2.0f, radius*2.0f);
    g.setColour (kNeonCyan);
    g.drawEllipse (cx-radius, cy-radius, radius*2.0f, radius*2.0f, 1.5f);

    const float lineLen = radius * 0.65f;
    const float lx      = cx + lineLen * std::sin (angle);
    const float ly      = cy - lineLen * std::cos (angle);
    g.setColour (kNeonPink.withAlpha (0.45f));
    g.drawLine (cx, cy, lx, ly, 4.0f);
    g.setColour (kNeonPink);
    g.drawLine (cx, cy, lx, ly, 1.8f);
}

void NeonLookAndFeel::drawLinearSlider (juce::Graphics& g,
                                         int x, int y, int w, int h,
                                         float sliderPos, float, float,
                                         juce::Slider::SliderStyle style,
                                         juce::Slider&)
{
    if (style == juce::Slider::LinearVertical)
    {
        const float cx = (float)x + (float)w * 0.5f;
        g.setColour (kNeonCyan.withAlpha (0.12f));
        g.fillRect (cx-3.0f, (float)y, 6.0f, (float)h);
        g.setColour (kNeonCyan.withAlpha (0.55f));
        g.fillRect (cx-1.0f, (float)y, 2.0f, (float)h);

        const float thumbH = 12.0f;
        const float thumbW = (float)w - 4.0f;
        const float tx     = (float)x + 2.0f;
        const float ty     = sliderPos - thumbH * 0.5f;
        g.setColour (kNeonPink.withAlpha (0.35f));
        g.fillRect (tx-2.0f, ty-2.0f, thumbW+4.0f, thumbH+4.0f);
        g.setColour (kNeonPink);
        g.fillRect (tx, ty, thumbW, thumbH);
        g.setColour (kNeonBg);
        g.drawLine (tx+2.0f, ty+thumbH*0.5f, tx+thumbW-2.0f, ty+thumbH*0.5f, 1.5f);
    }
    else
    {
        const float cy = (float)y + (float)h * 0.5f;
        g.setColour (kNeonCyan.withAlpha (0.55f));
        g.fillRect ((float)x, cy-1.0f, (float)w, 2.0f);
        const float thumbW = 12.0f;
        const float thumbH = (float)h - 4.0f;
        const float tx = sliderPos - thumbW * 0.5f;
        const float ty = (float)y + 2.0f;
        g.setColour (kNeonPink);
        g.fillRect (tx, ty, thumbW, thumbH);
    }
}

void NeonLookAndFeel::drawToggleButton (juce::Graphics& g,
                                         juce::ToggleButton& btn,
                                         bool, bool)
{
    const bool on = btn.getToggleState();
    const auto b  = btn.getLocalBounds().toFloat().reduced (1.0f);

    if (on)
    {
        g.setColour (kNeonCyan.withAlpha (0.18f));
        g.fillRoundedRectangle (b.expanded (1.0f), 3.0f);
    }
    g.setColour (on ? juce::Colour (0xFF131330) : kNeonBg);
    g.fillRoundedRectangle (b, 3.0f);
    g.setColour (kNeonCyan);
    g.drawRoundedRectangle (b, 3.0f, 1.5f);
    g.setColour (on ? kNeonCyan : kNeonCyan.withAlpha (0.6f));
    g.setFont (juce::Font (10.0f, juce::Font::bold));
    g.drawText (btn.getButtonText(), btn.getLocalBounds(), juce::Justification::centred);
}

juce::Font NeonLookAndFeel::getLabelFont (juce::Label&)
{
    return juce::Font (10.0f, juce::Font::bold);
}

//==============================================================================
// Spectrum analyzer component
//==============================================================================

void SpectrumAnalyzerComponent::paint (juce::Graphics& g)
{
    const float w = (float)getWidth();
    const float h = (float)getHeight();

    g.fillAll (isNeon ? juce::Colour (0xFF0D0D24) : juce::Colour (0xFFF0F0F0));

    const float* bins     = proc.getSpectrumData();
    const int    numBins  = SynthProcessor::kSpectrumBins;
    const float  srHalf   = 44100.0f / 2.0f;
    const float  logMin   = std::log10 (20.0f);
    const float  logMax   = std::log10 (22000.0f);
    const float  logRange = logMax - logMin;

    juce::Path path;
    bool started = false;

    for (int i = 1; i < numBins; ++i)
    {
        const float freq = (float)i * srHalf / (float)numBins;
        if (freq < 20.0f) continue;
        const float xNorm = (std::log10 (freq) - logMin) / logRange;
        const float px    = xNorm * w;
        const float dB    = 20.0f * std::log10 (bins[i] + 1e-9f);
        const float py    = juce::jmap (dB, -80.0f, 0.0f, h - 2.0f, 2.0f);

        if (!started) { path.startNewSubPath (px, h); path.lineTo (px, py); started = true; }
        else           path.lineTo (px, py);
    }

    if (started)
    {
        path.lineTo (w, h);
        path.closeSubPath();

        const juce::Colour fillTop = isNeon ? kNeonCyan                    : juce::Colour (0xFF888888);
        const juce::Colour fillBot = isNeon ? kNeonCyan.withAlpha (0.25f)  : juce::Colour (0x33888888);
        g.setGradientFill (juce::ColourGradient (fillTop, 0, 0, fillBot, 0, h, false));
        g.fillPath (path);
        g.setColour (isNeon ? kNeonCyan : juce::Colour (0xFF444444));
        g.strokePath (path, juce::PathStrokeType (1.5f));
    }

    // Frequency grid
    const float gridHz[]     = { 50,100,200,500,1000,2000,5000,10000,20000 };
    const char* gridLabel[]  = { "50","100","200","500","1k","2k","5k","10k","20k" };
    g.setFont (8.0f);
    g.setColour (isNeon ? kNeonCyan.withAlpha (0.28f) : juce::Colour (0xFFBBBBBB));
    for (int i = 0; i < 9; ++i)
    {
        const float x = (std::log10 (gridHz[i]) - logMin) / logRange * w;
        g.drawVerticalLine ((int)x, 0.0f, h);
        g.setColour (isNeon ? kNeonCyan.withAlpha (0.55f) : juce::Colour (0xFF888888));
        g.drawText (gridLabel[i], (int)x + 2, (int)h - 11, 28, 10, juce::Justification::left);
        g.setColour (isNeon ? kNeonCyan.withAlpha (0.28f) : juce::Colour (0xFFBBBBBB));
    }

    g.setColour (isNeon ? kNeonCyan : juce::Colours::black);
    g.drawRect (getLocalBounds(), 1);
}

static void makeRotary (juce::Slider& s)
{
    s.setSliderStyle (juce::Slider::Rotary);
    s.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
}

static void makeFader (juce::Slider& s)
{
    s.setSliderStyle (juce::Slider::LinearVertical);
    s.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
}

static void makeHSlider (juce::Slider& s)
{
    s.setSliderStyle (juce::Slider::LinearHorizontal);
    s.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
}

static juce::Label* makeLabel (juce::Component& parent,
                                std::vector<std::unique_ptr<juce::Label>>& pool,
                                const juce::String& text)
{
    auto l = std::make_unique<juce::Label>();
    l->setText (text, juce::dontSendNotification);
    l->setJustificationType (juce::Justification::centred);
    l->setFont (juce::Font (9.5f, juce::Font::bold));
    l->setColour (juce::Label::textColourId, juce::Colours::black);
    auto* ptr = l.get();
    parent.addAndMakeVisible (*l);
    pool.push_back (std::move (l));
    return ptr;
}

SynthEditor::SynthEditor (SynthProcessor& p)
    : AudioProcessorEditor (p),
      processor (p),
      spectrumComp (p),
      keyboard (p.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    setLookAndFeel (&laf);

    auto& apvts = p.apvts;

    auto initRotary = [&](juce::Slider& s, const char* id,
                           std::unique_ptr<SliderAttachment>& att)
    {
        makeRotary (s);
        addAndMakeVisible (s);
        att = std::make_unique<SliderAttachment> (apvts, id, s);
    };

    auto initFader = [&](juce::Slider& s, const char* id,
                          std::unique_ptr<SliderAttachment>& att)
    {
        makeFader (s);
        addAndMakeVisible (s);
        att = std::make_unique<SliderAttachment> (apvts, id, s);
    };

    titleLabel.setText ("DCO6", juce::dontSendNotification);
    titleLabel.setFont (juce::Font (16.0f, juce::Font::bold));
    titleLabel.setColour (juce::Label::textColourId, juce::Colours::black);
    titleLabel.setJustificationType (juce::Justification::centredLeft);
    addAndMakeVisible (titleLabel);

    for (int i = 0; i < p.getNumPrograms(); ++i)
        presetBox.addItem (p.getProgramName (i), i + 1);
    presetBox.setSelectedId (p.getCurrentProgram() + 1, juce::dontSendNotification);
    presetBox.onChange = [this]
    {
        processor.setCurrentProgram (presetBox.getSelectedId() - 1);
    };
    addAndMakeVisible (presetBox);

    pitchWheelSlider.setSliderStyle (juce::Slider::LinearVertical);
    pitchWheelSlider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    pitchWheelSlider.setRange (-1.0, 1.0);
    pitchWheelSlider.setValue (0.0);
    pitchWheelSlider.setInterceptsMouseClicks (false, false);
    addAndMakeVisible (pitchWheelSlider);

    modWheelSlider.setSliderStyle (juce::Slider::LinearVertical);
    modWheelSlider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    modWheelSlider.setRange (0.0, 1.0);
    modWheelSlider.setValue (0.0);
    modWheelSlider.setInterceptsMouseClicks (false, false);
    addAndMakeVisible (modWheelSlider);

    initRotary (dcoPwmKnob,      "dco_pwm",       sDcoPwm);
    initRotary (dcoPwmLfoKnob,   "dco_pwm_lfo",   sDcoPwmLfo);
    initRotary (dcoLfoPitchKnob, "dco_lfo_pitch", sDcoLfoPitch);

    dcoBtnSaw.setButtonText ("SAW");
    dcoBtnPulse.setButtonText ("PULSE");
    dcoBtnNoise.setButtonText ("NOISE");
    addAndMakeVisible (dcoBtnSaw);
    addAndMakeVisible (dcoBtnPulse);
    addAndMakeVisible (dcoBtnNoise);
    bSaw   = std::make_unique<ButtonAttachment> (apvts, "dco_saw",   dcoBtnSaw);
    bPulse = std::make_unique<ButtonAttachment> (apvts, "dco_pulse", dcoBtnPulse);
    bNoise = std::make_unique<ButtonAttachment> (apvts, "dco_noise", dcoBtnNoise);

    dcoRangeBox.addItem ("32'", 1);
    dcoRangeBox.addItem ("16'", 2);
    dcoRangeBox.addItem ("8'",  3);
    dcoRangeBox.addItem ("4'",  4);
    addAndMakeVisible (dcoRangeBox);
    cDcoRange = std::make_unique<ComboAttachment> (apvts, "dco_range", dcoRangeBox);

    dcoSubBox.addItem ("Off",    1);
    dcoSubBox.addItem ("Square", 2);
    dcoSubBox.addItem ("Sub2",   3);
    addAndMakeVisible (dcoSubBox);
    cDcoSub = std::make_unique<ComboAttachment> (apvts, "dco_sub", dcoSubBox);

    initRotary (hpfKnob, "hpf_cutoff", sHpf);

    initRotary (vcfCutoffKnob, "vcf_cutoff", sVcfCut);
    initRotary (vcfResKnob,    "vcf_res",    sVcfRes);
    initRotary (vcfEnvKnob,    "vcf_env",    sVcfEnv);
    initRotary (vcfLfoKnob,    "vcf_lfo",    sVcfLfo);
    initRotary (vcfKeyKnob,    "vcf_key",    sVcfKey);

    initFader (fAttFader, "env_f_a", sFAtt);
    initFader (fDecFader, "env_f_d", sFDec);
    initFader (fSusFader, "env_f_s", sFSus);
    initFader (fRelFader, "env_f_r", sFRel);

    initFader (aAttFader, "env_a_a", sAAtt);
    initFader (aDecFader, "env_a_d", sADec);
    initFader (aSusFader, "env_a_s", sASus);
    initFader (aRelFader, "env_a_r", sARel);

    vcaGateBtn.setButtonText ("GATE");
    addAndMakeVisible (vcaGateBtn);
    bVcaGate = std::make_unique<ButtonAttachment> (apvts, "vca_gate", vcaGateBtn);

    initRotary (lfoRateKnob,  "lfo_rate",  sLfoRate);
    initRotary (lfoDelayKnob, "lfo_delay", sLfoDelay);

    lfoWaveBox.addItem ("Triangle", 1);
    lfoWaveBox.addItem ("Square",   2);
    lfoWaveBox.addItem ("Sawtooth", 3);
    addAndMakeVisible (lfoWaveBox);
    cLfoWave = std::make_unique<ComboAttachment> (apvts, "lfo_wave", lfoWaveBox);

    initRotary (delayTimeKnob, "delay_time", sDelayTime);
    initRotary (delayFbKnob,   "delay_fb",   sDelayFb);
    initRotary (delayMixKnob,  "delay_mix",  sDelayMix);

    initRotary (reverbRoomKnob, "reverb_room", sRevRoom);
    initRotary (reverbDampKnob, "reverb_damp", sRevDamp);
    initRotary (reverbMixKnob,  "reverb_mix",  sRevMix);

    initRotary (volumeKnob, "volume",     sVol);
    initRotary (portaKnob,  "portamento", sPorta);

    chorusBox.addItem ("Off", 1);
    chorusBox.addItem ("I",   2);
    chorusBox.addItem ("II",  3);
    addAndMakeVisible (chorusBox);
    cChorus = std::make_unique<ComboAttachment> (apvts, "chorus", chorusBox);

    bendRangeSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    bendRangeSlider.setTextBoxStyle (juce::Slider::TextBoxLeft, false, 28, 18);
    bendRangeSlider.setNumDecimalPlacesToDisplay (0);
    addAndMakeVisible (bendRangeSlider);
    sBendRange = std::make_unique<SliderAttachment> (apvts, "bend_range", bendRangeSlider);

    initRotary (eqLowKnob,  "eq_low",  sEqLow);
    initRotary (eqMidKnob,  "eq_mid",  sEqMid);
    initRotary (eqHighKnob, "eq_high", sEqHigh);

    addAndMakeVisible (spectrumComp);

    skinToggleBtn.setButtonText ("NEON");
    skinToggleBtn.onClick = [this]
    {
        setSkin (currentSkin == 0 ? 1 : 0);
        skinToggleBtn.setButtonText (currentSkin == 0 ? "NEON" : "LIGHT");
    };
    addAndMakeVisible (skinToggleBtn);

    keyboard.setColour (juce::MidiKeyboardComponent::whiteNoteColourId,     juce::Colours::white);
    keyboard.setColour (juce::MidiKeyboardComponent::blackNoteColourId,     juce::Colours::black);
    keyboard.setColour (juce::MidiKeyboardComponent::keySeparatorLineColourId, juce::Colours::black);
    keyboard.setColour (juce::MidiKeyboardComponent::mouseOverKeyOverlayColourId, juce::Colour (0xAADDDDDD));
    keyboard.setColour (juce::MidiKeyboardComponent::keyDownOverlayColourId,  juce::Colour (0xAA888888));
    keyboard.setScrollButtonsVisible (false);
    keyboard.setAvailableRange (36, 96);
    keyboard.setLowestVisibleKey (36);
    addAndMakeVisible (keyboard);

    auto& lv = labels;
    auto& self = *this;
    pitchWheelLabel.setText ("PB", juce::dontSendNotification);
    pitchWheelLabel.setJustificationType (juce::Justification::centred);
    pitchWheelLabel.setFont (juce::Font (9.0f, juce::Font::bold));
    pitchWheelLabel.setColour (juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible (pitchWheelLabel);

    modWheelLabel.setText ("MOD", juce::dontSendNotification);
    modWheelLabel.setJustificationType (juce::Justification::centred);
    modWheelLabel.setFont (juce::Font (9.0f, juce::Font::bold));
    modWheelLabel.setColour (juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible (modWheelLabel);

    makeLabel (self, lv, "PWM");
    makeLabel (self, lv, "PWM-LFO");
    makeLabel (self, lv, "LFO-P");
    makeLabel (self, lv, "RANGE");
    makeLabel (self, lv, "SUB");
    makeLabel (self, lv, "HPF");
    makeLabel (self, lv, "CUTOFF");
    makeLabel (self, lv, "RES");
    makeLabel (self, lv, "ENV");
    makeLabel (self, lv, "LFO");
    makeLabel (self, lv, "KEY");
    makeLabel (self, lv, "A");
    makeLabel (self, lv, "D");
    makeLabel (self, lv, "S");
    makeLabel (self, lv, "R");
    makeLabel (self, lv, "A");
    makeLabel (self, lv, "D");
    makeLabel (self, lv, "S");
    makeLabel (self, lv, "R");
    makeLabel (self, lv, "RATE");
    makeLabel (self, lv, "DELAY");
    makeLabel (self, lv, "TIME");
    makeLabel (self, lv, "FB");
    makeLabel (self, lv, "MIX");
    makeLabel (self, lv, "ROOM");
    makeLabel (self, lv, "DAMP");
    makeLabel (self, lv, "MIX");
    makeLabel (self, lv, "VOL");
    makeLabel (self, lv, "PORTA");
    makeLabel (self, lv, "CHORUS");
    makeLabel (self, lv, "BEND");
    makeLabel (self, lv, "LOW");
    makeLabel (self, lv, "MID");
    makeLabel (self, lv, "HIGH");

    setResizable (true, true);
    setResizeLimits (760, 414, 2300, 1220);
    setSize (BASE_W, BASE_H);

    startTimerHz (20);
}

SynthEditor::~SynthEditor()
{
    stopTimer();
    setLookAndFeel (nullptr);
}

void SynthEditor::setSkin (int skin)
{
    currentSkin = skin;
    spectrumComp.isNeon = (skin == 1);
    if (skin == 1)
    {
        setLookAndFeel (&neonLaf);
        keyboard.setColour (juce::MidiKeyboardComponent::whiteNoteColourId,            juce::Colour (0xFF131330));
        keyboard.setColour (juce::MidiKeyboardComponent::blackNoteColourId,            juce::Colour (0xFF05050F));
        keyboard.setColour (juce::MidiKeyboardComponent::keySeparatorLineColourId,     kNeonCyan);
        keyboard.setColour (juce::MidiKeyboardComponent::mouseOverKeyOverlayColourId,  kNeonCyan.withAlpha (0.25f));
        keyboard.setColour (juce::MidiKeyboardComponent::keyDownOverlayColourId,       kNeonPink.withAlpha (0.7f));
    }
    else
    {
        setLookAndFeel (&laf);
        keyboard.setColour (juce::MidiKeyboardComponent::whiteNoteColourId,            juce::Colours::white);
        keyboard.setColour (juce::MidiKeyboardComponent::blackNoteColourId,            juce::Colours::black);
        keyboard.setColour (juce::MidiKeyboardComponent::keySeparatorLineColourId,     juce::Colours::black);
        keyboard.setColour (juce::MidiKeyboardComponent::mouseOverKeyOverlayColourId,  juce::Colour (0xAADDDDDD));
        keyboard.setColour (juce::MidiKeyboardComponent::keyDownOverlayColourId,       juce::Colour (0xAA888888));
    }
    repaint();
}

void SynthEditor::timerCallback()
{
    const float pb  = processor.displayPitchBend.load() * 2.0f - 1.0f;
    const float mod = processor.displayModWheel.load();
    if (std::abs ((float)pitchWheelSlider.getValue() - pb)  > 0.001f)
        pitchWheelSlider.setValue (pb,  juce::dontSendNotification);
    if (std::abs ((float)modWheelSlider.getValue() - mod) > 0.001f)
        modWheelSlider.setValue (mod, juce::dontSendNotification);

    if (presetBox.getSelectedId() != processor.getCurrentProgram() + 1)
        presetBox.setSelectedId (processor.getCurrentProgram() + 1, juce::dontSendNotification);

    spectrumComp.repaint();
}
void SynthEditor::paint (juce::Graphics& g)
{
    const bool neon = (currentSkin == 1);
    g.fillAll (neon ? kNeonBg : juce::Colour (0xFFF2F2F2));

    const float scale = std::min (
        (float)getWidth()  / (float)BASE_W,
        (float)getHeight() / (float)BASE_H);
    const float offX = ((float)getWidth()  - (float)BASE_W * scale) * 0.5f;
    const float offY = ((float)getHeight() - (float)BASE_H * scale) * 0.5f;

    const juce::Colour panelFill    = neon ? kNeonPanel : juce::Colours::white;
    const juce::Colour panelBorder  = neon ? kNeonCyan  : juce::Colours::black;
    const juce::Colour textCol      = neon ? kNeonCyan  : juce::Colours::black;

    auto sect = [&](int x, int y, int w, int h, const char* name)
    {
        const int rx = (int)(offX + x * scale);
        const int ry = (int)(offY + y * scale);
        const int rw = (int)(w * scale + 0.5f);
        const int rh = (int)(h * scale + 0.5f);
        g.setColour (panelFill);
        g.fillRect (rx, ry, rw, rh);
        g.setColour (panelBorder);
        g.drawRect (rx, ry, rw, rh, 1);
        g.setFont (juce::Font (10.0f * scale, juce::Font::bold));
        g.setColour (textCol);
        g.drawText (name, rx + 3, ry + 2, rw - 6, (int)(14 * scale),
                    juce::Justification::centredLeft);
    };

    sect (4,   26, 56,  306, "WHEELS");
    sect (64,  26, 182, 306, "DCO");
    sect (250, 26, 58,  306, "HPF");
    sect (312, 26, 180, 306, "VCF");
    sect (496, 26, 110, 306, "ENV F");
    sect (610, 26, 128, 306, "ENV A");
    sect (742, 26, 96,  306, "LFO");
    sect (842, 26, 96,  306, "DELAY");
    sect (942, 26, 96,  306, "REVERB");
    sect (1042,26, 104, 306, "OUT");

    sect (4,   338, 191, 144, "EQ");
    sect (199, 338, 947, 144, "ANALYZER");

    g.setColour (textCol);
    g.setFont (juce::Font (14.0f * scale, juce::Font::bold));
    g.drawText ("DCO6",
                (int)(offX + 8 * scale), (int)(offY),
                (int)(80 * scale),       (int)(24 * scale),
                juce::Justification::centredLeft);
}

void SynthEditor::resized()
{
    const float scale = std::min (
        (float)getWidth()  / (float)BASE_W,
        (float)getHeight() / (float)BASE_H);
    const float offX = ((float)getWidth()  - (float)BASE_W * scale) * 0.5f;
    const float offY = ((float)getHeight() - (float)BASE_H * scale) * 0.5f;

    auto sc = [&](int x, int y, int w, int h) -> juce::Rectangle<int>
    {
        return { (int)(offX + x * scale), (int)(offY + y * scale),
                 (int)(w * scale + 0.5f), (int)(h * scale + 0.5f) };
    };

    titleLabel.setBounds (sc (8, 0, 80, 24));
    presetBox.setBounds  (sc (92, 2, 230, 20));
    skinToggleBtn.setBounds (sc (340, 2, 80, 20));

    pitchWheelSlider.setBounds (sc (6,   42, 24, 120));
    pitchWheelLabel.setBounds  (sc (6,  164, 24,  12));
    modWheelSlider.setBounds   (sc (34,  42, 24, 120));
    modWheelLabel.setBounds    (sc (34, 164, 24,  12));

        dcoBtnSaw.setBounds   (sc (66,  44, 54, 18));
    dcoBtnPulse.setBounds (sc (124, 44, 54, 18));
    dcoBtnNoise.setBounds (sc (182, 44, 54, 18));
    labels[3]->setBounds  (sc (66,  65, 84, 14));
    dcoRangeBox.setBounds (sc (66,  79, 84, 20));
    labels[4]->setBounds  (sc (156, 65, 84, 14));
    dcoSubBox.setBounds   (sc (156, 79, 84, 20));
    labels[0]->setBounds  (sc (66, 103, 52, 12));
    dcoPwmKnob.setBounds      (sc (66, 116, 52, 52));
    labels[1]->setBounds  (sc (122,103, 52, 12));
    dcoPwmLfoKnob.setBounds   (sc (122,116, 52, 52));
    labels[2]->setBounds  (sc (178,103, 52, 12));
    dcoLfoPitchKnob.setBounds (sc (178,116, 52, 52));

    labels[5]->setBounds  (sc (253, 60, 52, 12));
    hpfKnob.setBounds     (sc (253, 72, 52, 52));

    labels[6]->setBounds  (sc (314, 46, 52, 12));
    vcfCutoffKnob.setBounds (sc (314, 58, 52, 52));
    labels[7]->setBounds  (sc (370, 46, 52, 12));
    vcfResKnob.setBounds  (sc (370, 58, 52, 52));
    labels[8]->setBounds  (sc (314,126, 52, 12));
    vcfEnvKnob.setBounds  (sc (314,138, 52, 52));
    labels[9]->setBounds  (sc (370,126, 52, 12));
    vcfLfoKnob.setBounds  (sc (370,138, 52, 52));
    labels[10]->setBounds (sc (426,126, 52, 12));
    vcfKeyKnob.setBounds  (sc (426,138, 52, 52));

    fAttFader.setBounds (sc (500, 44, 22, 216));
    fDecFader.setBounds (sc (526, 44, 22, 216));
    fSusFader.setBounds (sc (552, 44, 22, 216));
    fRelFader.setBounds (sc (578, 44, 22, 216));
    labels[11]->setBounds (sc (498,262, 24, 12));
    labels[12]->setBounds (sc (524,262, 24, 12));
    labels[13]->setBounds (sc (550,262, 24, 12));
    labels[14]->setBounds (sc (576,262, 24, 12));

    aAttFader.setBounds (sc (614, 44, 22, 206));
    aDecFader.setBounds (sc (640, 44, 22, 206));
    aSusFader.setBounds (sc (666, 44, 22, 206));
    aRelFader.setBounds (sc (692, 44, 22, 206));
    labels[15]->setBounds (sc (612,252, 24, 12));
    labels[16]->setBounds (sc (638,252, 24, 12));
    labels[17]->setBounds (sc (664,252, 24, 12));
    labels[18]->setBounds (sc (690,252, 24, 12));
    vcaGateBtn.setBounds  (sc (614,268,114, 18));

    labels[19]->setBounds (sc (744, 44, 44, 12));
    lfoRateKnob.setBounds (sc (744, 56, 44, 44));
    labels[20]->setBounds (sc (792, 44, 44, 12));
    lfoDelayKnob.setBounds(sc (792, 56, 44, 44));
    lfoWaveBox.setBounds  (sc (744,106, 90, 20));

    labels[21]->setBounds (sc (844, 44, 44, 12));
    delayTimeKnob.setBounds (sc (844, 56, 44, 44));
    labels[22]->setBounds (sc (844,110, 44, 12));
    delayFbKnob.setBounds   (sc (844,122, 44, 44));
    labels[23]->setBounds (sc (844,176, 44, 12));
    delayMixKnob.setBounds  (sc (844,188, 44, 44));

    labels[24]->setBounds (sc (944, 44, 44, 12));
    reverbRoomKnob.setBounds (sc (944, 56, 44, 44));
    labels[25]->setBounds (sc (944,110, 44, 12));
    reverbDampKnob.setBounds (sc (944,122, 44, 44));
    labels[26]->setBounds (sc (944,176, 44, 12));
    reverbMixKnob.setBounds  (sc (944,188, 44, 44));

    labels[27]->setBounds (sc (1044, 44, 44, 12));
    volumeKnob.setBounds  (sc (1044, 56, 44, 44));
    labels[28]->setBounds (sc (1092, 44, 44, 12));
    portaKnob.setBounds   (sc (1092, 56, 44, 44));
    labels[29]->setBounds (sc (1044,108, 96, 12));
    chorusBox.setBounds   (sc (1044,120, 96, 20));
    labels[30]->setBounds (sc (1044,148, 96, 12));
    bendRangeSlider.setBounds (sc (1044,160, 96, 20));

    // EQ section (inside "EQ" panel: x=4..195, y=338..482)
    labels[31]->setBounds (sc (8,  408, 52, 12));
    eqLowKnob.setBounds   (sc (8,  354, 52, 52));
    labels[32]->setBounds (sc (68, 408, 52, 12));
    eqMidKnob.setBounds   (sc (68, 354, 52, 52));
    labels[33]->setBounds (sc (128,408, 52, 12));
    eqHighKnob.setBounds  (sc (128,354, 52, 52));

    // Spectrum analyzer (inside "ANALYZER" panel: x=199..1146, y=338..482)
    spectrumComp.setBounds (sc (201, 355, 941, 119));

    keyboard.setScrollButtonsVisible (false);
    keyboard.setBounds (0, (int)(offY + 488.0f * scale), getWidth(), std::max (60, (int)(118.0f * scale + 0.5f)));
    keyboard.setKeyWidth ((float)getWidth() / 36.0f);
}