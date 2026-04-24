#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

namespace
{
    auto timeRange()
    {
        return juce::NormalisableRange<float> (0.001f, 10.0f,
            [](float, float, float v) { return std::pow (v, 3.0f) * 10.0f; },
            [](float, float, float v) { return std::pow (v / 10.0f, 1.0f / 3.0f); });
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout SynthProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    auto addFloat = [&](const juce::String& id, const juce::String& name,
                        float lo, float hi, float def)
    {
        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID {id, 1}, name,
            juce::NormalisableRange<float> (lo, hi), def));
    };

    auto addInt = [&](const juce::String& id, const juce::String& name,
                      int lo, int hi, int def)
    {
        params.push_back (std::make_unique<juce::AudioParameterInt> (
            juce::ParameterID {id, 1}, name, lo, hi, def));
    };

    auto addBool = [&](const juce::String& id, const juce::String& name, bool def)
    {
        params.push_back (std::make_unique<juce::AudioParameterBool> (
            juce::ParameterID {id, 1}, name, def));
    };

    auto addChoice = [&](const juce::String& id, const juce::String& name,
                         juce::StringArray choices, int def)
    {
        params.push_back (std::make_unique<juce::AudioParameterChoice> (
            juce::ParameterID {id, 1}, name, choices, def));
    };

    addChoice ("dco_range",    "DCO Range",      {"32'","16'","8'","4'"}, 1);
    addBool   ("dco_saw",      "DCO Saw",        true);
    addBool   ("dco_pulse",    "DCO Pulse",      false);
    addChoice ("dco_sub",      "DCO Sub",        {"Off","Square","Sub2"}, 0);
    addBool   ("dco_noise",    "DCO Noise",      false);
    addFloat  ("dco_pwm",      "DCO PWM",        0.05f, 0.95f, 0.5f);
    addFloat  ("dco_pwm_lfo",  "DCO PWM LFO",   0.0f,  1.0f,  0.0f);
    addFloat  ("dco_lfo_pitch","DCO LFO Pitch",  0.0f,  1.0f,  0.0f);

    addFloat  ("hpf_cutoff",   "HPF Cutoff",     0.0f,  1.0f,  0.0f);

    addFloat  ("vcf_cutoff",   "VCF Cutoff",     0.0f,  1.0f,  0.85f);
    addFloat  ("vcf_res",      "VCF Resonance",  0.0f,  1.0f,  0.0f);
    addFloat  ("vcf_env",      "VCF Env Amt",    0.0f,  1.0f,  0.5f);
    addFloat  ("vcf_lfo",      "VCF LFO Amt",   0.0f,  1.0f,  0.0f);
    addFloat  ("vcf_key",      "VCF Key Follow", 0.0f,  1.0f,  0.0f);

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID {"env_f_a", 1}, "Filter Attack",  timeRange(), 0.01f));
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID {"env_f_d", 1}, "Filter Decay",   timeRange(), 0.4f));
    addFloat  ("env_f_s",      "Filter Sustain", 0.0f, 1.0f, 0.5f);
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID {"env_f_r", 1}, "Filter Release", timeRange(), 0.3f));

    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID {"env_a_a", 1}, "Amp Attack",  timeRange(), 0.01f));
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID {"env_a_d", 1}, "Amp Decay",   timeRange(), 0.3f));
    addFloat  ("env_a_s",      "Amp Sustain",   0.0f, 1.0f, 0.8f);
    params.push_back (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID {"env_a_r", 1}, "Amp Release", timeRange(), 0.3f));
    addBool   ("vca_gate",     "VCA Gate Mode", false);

    addFloat  ("lfo_rate",     "LFO Rate",   0.1f, 20.0f, 1.0f);
    addFloat  ("lfo_delay",    "LFO Delay",  0.0f,  5.0f, 0.0f);
    addChoice ("lfo_wave",     "LFO Wave",   {"Triangle","Square","Sawtooth"}, 0);

    addChoice ("chorus",       "Chorus",     {"Off","I","II"}, 0);

    addFloat  ("delay_time",   "Delay Time",     0.01f, 2.0f, 0.3f);
    addFloat  ("delay_fb",     "Delay Feedback", 0.0f,  0.95f, 0.35f);
    addFloat  ("delay_mix",    "Delay Mix",      0.0f,  1.0f,  0.0f);

    addFloat  ("reverb_room",  "Reverb Room",    0.0f, 1.0f, 0.5f);
    addFloat  ("reverb_damp",  "Reverb Damp",    0.0f, 1.0f, 0.5f);
    addFloat  ("reverb_mix",   "Reverb Mix",     0.0f, 1.0f, 0.0f);

    addFloat  ("volume",       "Volume",         0.0f, 1.0f, 0.75f);
    addFloat  ("portamento",   "Portamento",     0.0f, 1.0f, 0.0f);
    addInt    ("bend_range",   "Bend Range",     1,   12,    2);

    addFloat  ("eq_low",  "EQ Low",  -12.0f, 12.0f, 0.0f);
    addFloat  ("eq_mid",  "EQ Mid",  -12.0f, 12.0f, 0.0f);
    addFloat  ("eq_high", "EQ High", -12.0f, 12.0f, 0.0f);

    return { params.begin(), params.end() };
}

SynthProcessor::SynthProcessor()
    : AudioProcessor (BusesProperties()
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "STATE", createParameterLayout())
{
    for (auto& c : ccPending) c.store (-1.0f);
    buildPresets();
    applyPreset (0);
}

void SynthProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    engine.prepare (sampleRate, samplesPerBlock);
    keyboardState.reset();
}

void SynthProcessor::releaseResources()
{
    engine.reset();
    keyboardState.reset();
}

void SynthProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                   juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;

    for (const auto meta : midi)
    {
        const auto msg = meta.getMessage();
        if (msg.isController())
        {
            handleMidiCC (msg.getControllerNumber(), msg.getControllerValue());
            if (msg.getControllerNumber() == 1)
                displayModWheel.store ((float)msg.getControllerValue() / 127.0f);
        }
        if (msg.isPitchWheel())
        {
            const float pb = (float)(msg.getPitchWheelValue() - 8192) / 8192.0f;
            displayPitchBend.store (pb * 0.5f + 0.5f);
        }
    }

    keyboardState.processNextMidiBuffer (midi, 0, buffer.getNumSamples(), true);

    const SynthParams p = buildSynthParams();
    engine.process (buffer, midi, p);
}

void SynthProcessor::handleMidiCC (int cc, int value)
{
    const float v = (float)value / 127.0f;

    auto writeRaw = [&](const char* id)
    {
        if (auto* raw = apvts.getRawParameterValue (id))
            if (auto* param = dynamic_cast<juce::RangedAudioParameter*>(apvts.getParameter (id)))
                raw->store (param->getNormalisableRange().convertFrom0to1 (v));
    };

    switch (cc)
    {
        case 74: writeRaw ("vcf_cutoff");    break;
        case 71: writeRaw ("vcf_res");       break;
        case 76: writeRaw ("vcf_env");       break;
        case 77: writeRaw ("dco_pwm");       break;
        case 78: writeRaw ("lfo_rate");      break;
        case 84: writeRaw ("lfo_delay");     break;
        case 86: writeRaw ("delay_mix");     break;
        case 87: writeRaw ("reverb_mix");    break;
        case 88: writeRaw ("dco_lfo_pitch"); break;
        case 73: writeRaw ("env_f_a");       break;
        case 75: writeRaw ("env_f_d");       break;
        case 79: writeRaw ("env_f_s");       break;
        case 72: writeRaw ("env_f_r");       break;
        case 80: writeRaw ("env_a_a");       break;
        case 81: writeRaw ("env_a_d");       break;
        case 82: writeRaw ("env_a_s");       break;
        case 83: writeRaw ("env_a_r");       break;
        case 85: writeRaw ("volume");        break;
        default: break;
    }

    if (cc >= 0 && cc < 128)
        ccPending[cc].store (v);
    triggerAsyncUpdate();
}

void SynthProcessor::handleAsyncUpdate()
{
    auto apply = [&](int cc, const char* id)
    {
        const float v = ccPending[cc].exchange (-1.0f);
        if (v < 0.0f) return;
        if (auto* param = dynamic_cast<juce::RangedAudioParameter*>(apvts.getParameter (id)))
            param->setValueNotifyingHost (
                param->convertTo0to1 (param->getNormalisableRange().convertFrom0to1 (v)));
    };

    apply (74, "vcf_cutoff");
    apply (71, "vcf_res");
    apply (76, "vcf_env");
    apply (77, "dco_pwm");
    apply (78, "lfo_rate");
    apply (84, "lfo_delay");
    apply (86, "delay_mix");
    apply (87, "reverb_mix");
    apply (88, "dco_lfo_pitch");
    apply (73, "env_f_a");
    apply (75, "env_f_d");
    apply (79, "env_f_s");
    apply (72, "env_f_r");
    apply (80, "env_a_a");
    apply (81, "env_a_d");
    apply (82, "env_a_s");
    apply (83, "env_a_r");
    apply (85, "volume");
}

SynthParams SynthProcessor::buildSynthParams() const
{
    SynthParams p;

    auto getF = [&](const char* id) -> float
    {
        return apvts.getRawParameterValue (id)->load();
    };
    auto getI = [&](const char* id) -> int
    {
        return (int)apvts.getRawParameterValue (id)->load();
    };
    auto getB = [&](const char* id) -> bool
    {
        return apvts.getRawParameterValue (id)->load() > 0.5f;
    };

    p.dcoRange     = getI ("dco_range");
    p.dcoSaw       = getB ("dco_saw");
    p.dcoPulse     = getB ("dco_pulse");
    p.dcoSub       = getI ("dco_sub");
    p.dcoNoise     = getB ("dco_noise");
    p.dcoPwmManual = getF ("dco_pwm");
    p.dcoPwmLfo    = getF ("dco_pwm_lfo");
    p.dcoLfoPitch  = getF ("dco_lfo_pitch");

    p.hpfCutoff    = getF ("hpf_cutoff");

    p.vcfCutoff    = getF ("vcf_cutoff");
    p.vcfResonance = getF ("vcf_res");
    p.vcfEnvAmount = getF ("vcf_env");
    p.vcfLfoAmount = getF ("vcf_lfo");
    p.vcfKeyFollow = getF ("vcf_key");

    p.envFAttack   = getF ("env_f_a");
    p.envFDecay    = getF ("env_f_d");
    p.envFSustain  = getF ("env_f_s");
    p.envFRelease  = getF ("env_f_r");

    p.envAAttack   = getF ("env_a_a");
    p.envADecay    = getF ("env_a_d");
    p.envASustain  = getF ("env_a_s");
    p.envARelease  = getF ("env_a_r");
    p.vcaGateMode  = getB ("vca_gate");

    p.lfoRate      = getF ("lfo_rate");
    p.lfoDelay     = getF ("lfo_delay");
    p.lfoWaveform  = getI ("lfo_wave");

    p.chorusMode   = getI ("chorus");

    p.delayTime     = getF ("delay_time");
    p.delayFeedback = getF ("delay_fb");
    p.delayMix      = getF ("delay_mix");

    p.reverbRoom    = getF ("reverb_room");
    p.reverbDamp    = getF ("reverb_damp");
    p.reverbMix     = getF ("reverb_mix");

    p.masterVolume = getF ("volume");
    p.portamento   = getF ("portamento");
    p.bendRange    = getI ("bend_range");

    p.eqLowGain  = getF ("eq_low");
    p.eqMidGain  = getF ("eq_mid");
    p.eqHighGain = getF ("eq_high");

    return p;
}

void SynthProcessor::setCurrentProgram (int index)
{
    if (index >= 0 && index < (int)presets.size())
    {
        currentPreset = index;
        applyPreset (index);
    }
}

const juce::String SynthProcessor::getProgramName (int index)
{
    if (index >= 0 && index < presetNames.size())
        return presetNames[index];
    return "Init";
}

void SynthProcessor::applyPreset (int index)
{
    if (index < 0 || index >= (int)presets.size()) return;
    for (auto& pp : presets[index])
    {
        if (auto* param = apvts.getParameter (pp.id))
        {
            const float norm = param->convertTo0to1 (pp.value);
            param->setValueNotifyingHost (norm);
        }
    }
}

void SynthProcessor::buildPresets()
{
    presetNames.clear();
    presets.clear();

    presetNames.add ("Classic Pad");
    presets.push_back ({
        {"dco_saw", 1.f}, {"dco_pulse", 0.f}, {"dco_noise", 0.f}, {"dco_sub", 0.f},
        {"dco_range", 1.f}, {"dco_pwm", 0.5f}, {"dco_pwm_lfo", 0.f}, {"dco_lfo_pitch", 0.f},
        {"hpf_cutoff", 0.f},
        {"vcf_cutoff", 0.65f}, {"vcf_res", 0.1f}, {"vcf_env", 0.4f}, {"vcf_lfo", 0.f}, {"vcf_key", 0.3f},
        {"env_f_a", 1.2f}, {"env_f_d", 0.8f}, {"env_f_s", 0.6f}, {"env_f_r", 1.5f},
        {"env_a_a", 1.5f}, {"env_a_d", 0.5f}, {"env_a_s", 0.9f}, {"env_a_r", 2.0f}, {"vca_gate", 0.f},
        {"lfo_rate", 0.8f}, {"lfo_delay", 0.5f}, {"lfo_wave", 0.f},
        {"chorus", 2.f},
        {"delay_time", 0.4f}, {"delay_fb", 0.4f}, {"delay_mix", 0.2f},
        {"reverb_room", 0.75f}, {"reverb_damp", 0.4f}, {"reverb_mix", 0.35f},
        {"volume", 0.75f}, {"portamento", 0.f}, {"bend_range", 2.f}
    });

    presetNames.add ("PWM Strings");
    presets.push_back ({
        {"dco_saw", 0.f}, {"dco_pulse", 1.f}, {"dco_noise", 0.f}, {"dco_sub", 0.f},
        {"dco_range", 1.f}, {"dco_pwm", 0.5f}, {"dco_pwm_lfo", 0.7f}, {"dco_lfo_pitch", 0.f},
        {"hpf_cutoff", 0.05f},
        {"vcf_cutoff", 0.75f}, {"vcf_res", 0.05f}, {"vcf_env", 0.2f}, {"vcf_lfo", 0.f}, {"vcf_key", 0.4f},
        {"env_f_a", 0.8f}, {"env_f_d", 0.5f}, {"env_f_s", 0.7f}, {"env_f_r", 1.2f},
        {"env_a_a", 1.0f}, {"env_a_d", 0.3f}, {"env_a_s", 0.95f}, {"env_a_r", 1.5f}, {"vca_gate", 0.f},
        {"lfo_rate", 0.6f}, {"lfo_delay", 0.3f}, {"lfo_wave", 0.f},
        {"chorus", 2.f},
        {"delay_time", 0.5f}, {"delay_fb", 0.35f}, {"delay_mix", 0.15f},
        {"reverb_room", 0.6f}, {"reverb_damp", 0.5f}, {"reverb_mix", 0.3f},
        {"volume", 0.75f}, {"portamento", 0.f}, {"bend_range", 2.f}
    });

    presetNames.add ("Sweep Pad");
    presets.push_back ({
        {"dco_saw", 1.f}, {"dco_pulse", 0.f}, {"dco_noise", 0.f}, {"dco_sub", 0.f},
        {"dco_range", 1.f}, {"dco_pwm", 0.5f}, {"dco_pwm_lfo", 0.f}, {"dco_lfo_pitch", 0.04f},
        {"hpf_cutoff", 0.f},
        {"vcf_cutoff", 0.3f}, {"vcf_res", 0.5f}, {"vcf_env", 0.65f}, {"vcf_lfo", 0.f}, {"vcf_key", 0.5f},
        {"env_f_a", 2.0f}, {"env_f_d", 1.0f}, {"env_f_s", 0.5f}, {"env_f_r", 2.5f},
        {"env_a_a", 2.5f}, {"env_a_d", 0.5f}, {"env_a_s", 0.9f}, {"env_a_r", 3.0f}, {"vca_gate", 0.f},
        {"lfo_rate", 0.5f}, {"lfo_delay", 1.0f}, {"lfo_wave", 0.f},
        {"chorus", 2.f},
        {"delay_time", 0.6f}, {"delay_fb", 0.5f}, {"delay_mix", 0.25f},
        {"reverb_room", 0.85f}, {"reverb_damp", 0.3f}, {"reverb_mix", 0.45f},
        {"volume", 0.7f}, {"portamento", 0.f}, {"bend_range", 2.f}
    });

    presetNames.add ("Dream Pad");
    presets.push_back ({
        {"dco_saw", 1.f}, {"dco_pulse", 1.f}, {"dco_noise", 0.f}, {"dco_sub", 0.f},
        {"dco_range", 1.f}, {"dco_pwm", 0.45f}, {"dco_pwm_lfo", 0.25f}, {"dco_lfo_pitch", 0.02f},
        {"hpf_cutoff", 0.f},
        {"vcf_cutoff", 0.55f}, {"vcf_res", 0.08f}, {"vcf_env", 0.3f}, {"vcf_lfo", 0.f}, {"vcf_key", 0.2f},
        {"env_f_a", 2.5f}, {"env_f_d", 1.5f}, {"env_f_s", 0.55f}, {"env_f_r", 4.0f},
        {"env_a_a", 3.0f}, {"env_a_d", 0.5f}, {"env_a_s", 0.9f}, {"env_a_r", 5.0f}, {"vca_gate", 0.f},
        {"lfo_rate", 0.35f}, {"lfo_delay", 1.5f}, {"lfo_wave", 0.f},
        {"chorus", 2.f},
        {"delay_time", 0.75f}, {"delay_fb", 0.55f}, {"delay_mix", 0.3f},
        {"reverb_room", 0.95f}, {"reverb_damp", 0.2f}, {"reverb_mix", 0.55f},
        {"volume", 0.7f}, {"portamento", 0.f}, {"bend_range", 2.f}
    });

    presetNames.add ("Dark Pad");
    presets.push_back ({
        {"dco_saw", 1.f}, {"dco_pulse", 0.f}, {"dco_noise", 0.f}, {"dco_sub", 1.f},
        {"dco_range", 1.f}, {"dco_pwm", 0.5f}, {"dco_pwm_lfo", 0.f}, {"dco_lfo_pitch", 0.f},
        {"hpf_cutoff", 0.f},
        {"vcf_cutoff", 0.35f}, {"vcf_res", 0.15f}, {"vcf_env", 0.45f}, {"vcf_lfo", 0.f}, {"vcf_key", 0.2f},
        {"env_f_a", 1.8f}, {"env_f_d", 1.2f}, {"env_f_s", 0.4f}, {"env_f_r", 2.5f},
        {"env_a_a", 2.0f}, {"env_a_d", 0.5f}, {"env_a_s", 0.85f}, {"env_a_r", 3.5f}, {"vca_gate", 0.f},
        {"lfo_rate", 0.4f}, {"lfo_delay", 0.8f}, {"lfo_wave", 0.f},
        {"chorus", 1.f},
        {"delay_time", 0.55f}, {"delay_fb", 0.4f}, {"delay_mix", 0.2f},
        {"reverb_room", 0.8f}, {"reverb_damp", 0.6f}, {"reverb_mix", 0.4f},
        {"volume", 0.72f}, {"portamento", 0.f}, {"bend_range", 2.f}
    });

    presetNames.add ("Shimmer Pad");
    presets.push_back ({
        {"dco_saw", 1.f}, {"dco_pulse", 0.f}, {"dco_noise", 0.f}, {"dco_sub", 0.f},
        {"dco_range", 2.f}, {"dco_pwm", 0.5f}, {"dco_pwm_lfo", 0.f}, {"dco_lfo_pitch", 0.06f},
        {"hpf_cutoff", 0.15f},
        {"vcf_cutoff", 0.88f}, {"vcf_res", 0.12f}, {"vcf_env", 0.15f}, {"vcf_lfo", 0.f}, {"vcf_key", 0.6f},
        {"env_f_a", 1.0f}, {"env_f_d", 0.8f}, {"env_f_s", 0.75f}, {"env_f_r", 1.5f},
        {"env_a_a", 1.2f}, {"env_a_d", 0.3f}, {"env_a_s", 0.9f}, {"env_a_r", 2.0f}, {"vca_gate", 0.f},
        {"lfo_rate", 1.2f}, {"lfo_delay", 0.6f}, {"lfo_wave", 0.f},
        {"chorus", 2.f},
        {"delay_time", 0.33f}, {"delay_fb", 0.45f}, {"delay_mix", 0.25f},
        {"reverb_room", 0.9f}, {"reverb_damp", 0.25f}, {"reverb_mix", 0.5f},
        {"volume", 0.72f}, {"portamento", 0.f}, {"bend_range", 2.f}
    });

    presetNames.add ("Evolving Pad");
    presets.push_back ({
        {"dco_saw", 1.f}, {"dco_pulse", 1.f}, {"dco_noise", 0.f}, {"dco_sub", 0.f},
        {"dco_range", 1.f}, {"dco_pwm", 0.5f}, {"dco_pwm_lfo", 0.4f}, {"dco_lfo_pitch", 0.05f},
        {"hpf_cutoff", 0.f},
        {"vcf_cutoff", 0.5f}, {"vcf_res", 0.2f}, {"vcf_env", 0.35f}, {"vcf_lfo", 0.5f}, {"vcf_key", 0.3f},
        {"env_f_a", 1.5f}, {"env_f_d", 1.0f}, {"env_f_s", 0.5f}, {"env_f_r", 2.0f},
        {"env_a_a", 1.8f}, {"env_a_d", 0.5f}, {"env_a_s", 0.9f}, {"env_a_r", 2.5f}, {"vca_gate", 0.f},
        {"lfo_rate", 0.45f}, {"lfo_delay", 0.5f}, {"lfo_wave", 0.f},
        {"chorus", 2.f},
        {"delay_time", 0.62f}, {"delay_fb", 0.48f}, {"delay_mix", 0.22f},
        {"reverb_room", 0.78f}, {"reverb_damp", 0.35f}, {"reverb_mix", 0.42f},
        {"volume", 0.73f}, {"portamento", 0.f}, {"bend_range", 2.f}
    });

    presetNames.add ("Ambient Pad");
    presets.push_back ({
        {"dco_saw", 1.f}, {"dco_pulse", 0.f}, {"dco_noise", 1.f}, {"dco_sub", 0.f},
        {"dco_range", 1.f}, {"dco_pwm", 0.5f}, {"dco_pwm_lfo", 0.f}, {"dco_lfo_pitch", 0.01f},
        {"hpf_cutoff", 0.05f},
        {"vcf_cutoff", 0.6f}, {"vcf_res", 0.05f}, {"vcf_env", 0.2f}, {"vcf_lfo", 0.f}, {"vcf_key", 0.15f},
        {"env_f_a", 3.5f}, {"env_f_d", 2.0f}, {"env_f_s", 0.45f}, {"env_f_r", 6.0f},
        {"env_a_a", 4.0f}, {"env_a_d", 0.5f}, {"env_a_s", 0.85f}, {"env_a_r", 7.0f}, {"vca_gate", 0.f},
        {"lfo_rate", 0.25f}, {"lfo_delay", 2.0f}, {"lfo_wave", 0.f},
        {"chorus", 2.f},
        {"delay_time", 0.9f}, {"delay_fb", 0.6f}, {"delay_mix", 0.35f},
        {"reverb_room", 0.98f}, {"reverb_damp", 0.15f}, {"reverb_mix", 0.65f},
        {"volume", 0.68f}, {"portamento", 0.f}, {"bend_range", 2.f}
    });
}

void SynthProcessor::getStateInformation (juce::MemoryBlock& dest)
{
    if (auto state = apvts.copyState(); state.isValid())
    {
        state.setProperty ("currentPreset", currentPreset, nullptr);
        if (auto xml = state.createXml())
            copyXmlToBinary (*xml, dest);
    }
}

void SynthProcessor::setStateInformation (const void* data, int size)
{
    if (auto xml = getXmlFromBinary (data, size))
    {
        auto state = juce::ValueTree::fromXml (*xml);
        if (state.isValid())
        {
            currentPreset = (int)state.getProperty ("currentPreset", 0);
            apvts.replaceState (state);
        }
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthProcessor();
}

juce::AudioProcessorEditor* SynthProcessor::createEditor()
{
    return new SynthEditor (*this);
}