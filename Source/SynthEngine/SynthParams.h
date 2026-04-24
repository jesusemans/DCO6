#pragma once

struct SynthParams
{
    int   dcoRange      = 1;
    bool  dcoSaw        = true;
    bool  dcoPulse      = false;
    int   dcoSub        = 0;
    bool  dcoNoise      = false;
    float dcoPwmManual  = 0.5f;
    float dcoPwmLfo     = 0.0f;
    float dcoLfoPitch   = 0.0f;

    float hpfCutoff     = 0.0f;

    float vcfCutoff     = 0.85f;
    float vcfResonance  = 0.0f;
    float vcfEnvAmount  = 0.5f;
    float vcfLfoAmount  = 0.0f;
    float vcfKeyFollow  = 0.0f;

    float envFAttack    = 0.01f;
    float envFDecay     = 0.40f;
    float envFSustain   = 0.50f;
    float envFRelease   = 0.30f;

    float envAAttack    = 0.01f;
    float envADecay     = 0.30f;
    float envASustain   = 0.80f;
    float envARelease   = 0.30f;
    bool  vcaGateMode   = false;

    float lfoRate       = 1.0f;
    float lfoDelay      = 0.0f;
    int   lfoWaveform   = 0;

    int   chorusMode    = 0;

    float delayTime     = 0.30f;
    float delayFeedback = 0.35f;
    float delayMix      = 0.0f;

    float reverbRoom    = 0.5f;
    float reverbDamp    = 0.5f;
    float reverbMix     = 0.0f;

    float portamento    = 0.0f;
    float masterVolume  = 0.75f;
    float pitchBend     = 0.0f;
    float modWheel      = 0.0f;
    int   bendRange     = 2;

    float eqLowGain  = 0.0f;
    float eqMidGain  = 0.0f;
    float eqHighGain = 0.0f;
};
