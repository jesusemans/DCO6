#include "Voice.h"
#include <cmath>
#include <algorithm>

SynthVoice::SynthVoice() {}

void SynthVoice::setParams (const SynthParams& p)
{
    params = p;

    dco.setRange        (p.dcoRange);
    dco.setSawEnabled   (p.dcoSaw);
    dco.setPulseEnabled (p.dcoPulse);
    dco.setSubType      (p.dcoSub);
    dco.setNoiseEnabled (p.dcoNoise);

    hpf.setCutoffNorm (p.hpfCutoff);

    filterEnv.setAttack   (p.envFAttack);
    filterEnv.setDecay    (p.envFDecay);
    filterEnv.setSustain  (p.envFSustain);
    filterEnv.setRelease  (p.envFRelease);

    ampEnv.setAttack  (p.vcaGateMode ? 0.001f : p.envAAttack);
    ampEnv.setDecay   (p.vcaGateMode ? 0.001f : p.envADecay);
    ampEnv.setSustain (p.vcaGateMode ? 1.0f   : p.envASustain);
    ampEnv.setRelease (p.vcaGateMode ? 0.01f  : p.envARelease);


    lfo.setRate     (p.lfoRate);
    lfo.setDelay    (p.lfoDelay);
    LFO::Waveform lfoWf = LFO::Waveform::Triangle;
    if (p.lfoWaveform == 1) lfoWf = LFO::Waveform::Square;
    if (p.lfoWaveform == 2) lfoWf = LFO::Waveform::Sawtooth;
    lfo.setWaveform (lfoWf);

    if (p.portamento < 0.001f)
    {
        portaCoeff = 0.0f;
    }
    else
    {
        const float glideTimeSec = std::pow (p.portamento, 2.0f) * 5.0f;
        const float sr           = (float)getSampleRate();
        portaCoeff = (sr > 0.0f)
            ? std::exp (-1.0f / (glideTimeSec * sr))
            : 0.0f;
    }


}

void SynthVoice::startNote (int midiNote,
                            float vel,
                            juce::SynthesiserSound*,
                            int pitchWheelPos)
{
    noteNumber  = midiNote;
    velocity    = vel;
    noteFreqHz  = midiNoteToHz (midiNote);
    targetFreqHz = noteFreqHz;

    if (portaCoeff < 0.001f)
        currentFreqHz = targetFreqHz;

    pitchBendSemi = ((float)(pitchWheelPos - 8192) / 8192.0f)
                  * (float)params.bendRange;

    filterEnv.noteOn();
    ampEnv.noteOn();
    lfo.noteOn();
}

void SynthVoice::stopNote (float, bool allowTailOff)
{
    if (allowTailOff)
    {
        filterEnv.noteOff();
        ampEnv.noteOff();
    }
    else
    {
        filterEnv.reset();
        ampEnv.reset();
        clearCurrentNote();
    }
}

void SynthVoice::pitchWheelMoved (int newValue)
{
    pitchBendSemi = ((float)(newValue - 8192) / 8192.0f)
                  * (float)params.bendRange;
}

void SynthVoice::controllerMoved (int controller, int value)
{
    if (controller == 1)
        modWheelValue = (float)value / 127.0f;
}

void SynthVoice::renderNextBlock (juce::AudioBuffer<float>& buffer,
                                  int startSample,
                                  int numSamples)
{
    if (!ampEnv.isActive()) return;

    for (int s = 0; s < numSamples; ++s)
    {
        const float lfoOut = lfo.process();

        targetFreqHz  = midiNoteToHz (noteNumber, pitchBendSemi);
        currentFreqHz = targetFreqHz + portaCoeff * (currentFreqHz - targetFreqHz);

        const float effectiveLfoPitch = params.dcoLfoPitch + modWheelValue * 0.5f;
        const float lfoSemis    = lfoOut * effectiveLfoPitch * 2.0f;
        const float pitchedFreq = currentFreqHz * std::pow (2.0f, lfoSemis / 12.0f);

        const float pwm = std::clamp (params.dcoPwmManual + lfoOut * params.dcoPwmLfo,
                                       0.05f, 0.95f);

        dco.setFrequency  (pitchedFreq);
        dco.setPulseWidth (pwm);

        const float dcoOut = dco.process();
        const float hpfOut = hpf.process (dcoOut);

        const float filterEnvVal  = filterEnv.process();
        const float keyTrackSemis = (float)(noteNumber - 69) * params.vcfKeyFollow;
        const float keyTrackMult  = std::pow (2.0f, keyTrackSemis / 12.0f);
        const float effectiveLfoFilter = params.vcfLfoAmount + modWheelValue * 0.3f;
        const float lfoFilterAmt  = lfoOut * effectiveLfoFilter;

        const float modCutoff = std::clamp (
            params.vcfCutoff
            + filterEnvVal * params.vcfEnvAmount * 0.5f
            + lfoFilterAmt * 0.3f,
            0.0f, 1.0f);

        const float cutoffHz = 20.0f * std::pow (900.0f, modCutoff) * keyTrackMult;
        vcf.setCutoffHz  (cutoffHz);
        vcf.setResonance (params.vcfResonance);

        const float vcfOut = vcf.process (hpfOut);
        const float ampVal = ampEnv.process() * velocity;
        const float sample = vcfOut * ampVal * params.masterVolume;

        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            buffer.addSample (ch, startSample + s, sample);

        if (!ampEnv.isActive())
        {
            filterEnv.reset();
            clearCurrentNote();
            break;
        }
    }
}

float SynthVoice::midiNoteToHz (int note, float bendSemitones) const
{
    return 440.0f * std::pow (2.0f, ((float)(note - 69) + bendSemitones) / 12.0f);
}
