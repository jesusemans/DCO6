#pragma once
#include <cmath>

class EnvGen
{
public:
    EnvGen();

    void setSampleRate (double sr);
    void setAttack     (float seconds);
    void setDecay      (float seconds);
    void setSustain    (float level);
    void setRelease    (float seconds);

    void noteOn();
    void noteOff();
    void reset();

    float process();
    bool  isActive() const;

private:
    enum class Stage { Idle, Attack, Decay, Sustain, Release };

    void  recalcAttack();
    void  recalcDecay();
    void  recalcRelease();
    float coeff (float nSamples, float targetRatio) const;

    double sampleRate   = 44100.0;
    Stage  stage        = Stage::Idle;
    float  output       = 0.0f;

    float  attackTime   = 0.01f;
    float  decayTime    = 0.30f;
    float  sustainLevel = 0.80f;
    float  releaseTime  = 0.30f;

    float  attackCoeff  = 0.0f;
    float  attackBase   = 0.0f;
    float  decayCoeff   = 0.0f;
    float  decayBase    = 0.0f;
    float  releaseCoeff = 0.0f;
    float  releaseBase  = 0.0f;

    static constexpr float kAttackRatio  = 0.3f;
    static constexpr float kDRRatio      = 0.0001f;
};
