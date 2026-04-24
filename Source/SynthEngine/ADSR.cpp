#include "ADSR.h"
#include <cmath>

EnvGen::EnvGen()
{
    recalcAttack();
    recalcDecay();
    recalcRelease();
}

void EnvGen::setSampleRate (double sr)
{
    sampleRate = sr;
    recalcAttack();
    recalcDecay();
    recalcRelease();
}

void EnvGen::setAttack (float seconds)
{
    attackTime = seconds;
    recalcAttack();
}

void EnvGen::setDecay (float seconds)
{
    decayTime = seconds;
    recalcDecay();
}

void EnvGen::setSustain (float level)
{
    sustainLevel = level;
    recalcDecay();
}

void EnvGen::setRelease (float seconds)
{
    releaseTime = seconds;
    recalcRelease();
}

void EnvGen::noteOn()
{
    stage = Stage::Attack;
}

void EnvGen::noteOff()
{
    if (stage != Stage::Idle)
        stage = Stage::Release;
}

void EnvGen::reset()
{
    stage  = Stage::Idle;
    output = 0.0f;
}

float EnvGen::process()
{
    switch (stage)
    {
        case Stage::Idle:
            break;

        case Stage::Attack:
            output = attackBase + output * attackCoeff;
            if (output >= 1.0f)
            {
                output = 1.0f;
                stage  = Stage::Decay;
            }
            break;

        case Stage::Decay:
            output = decayBase + output * decayCoeff;
            if (output <= sustainLevel + kDRRatio * 0.5f)
            {
                output = sustainLevel;
                stage  = Stage::Sustain;
            }
            break;

        case Stage::Sustain:
            output = sustainLevel;
            break;

        case Stage::Release:
            output = releaseBase + output * releaseCoeff;
            if (output <= 0.0001f)
            {
                output = 0.0f;
                stage  = Stage::Idle;
            }
            break;
    }
    return output;
}

bool EnvGen::isActive() const
{
    return stage != Stage::Idle;
}

void EnvGen::recalcAttack()
{
    float n    = attackTime * (float)sampleRate;
    attackCoeff = coeff (n, kAttackRatio);
    attackBase  = (1.0f + kAttackRatio) * (1.0f - attackCoeff);
}

void EnvGen::recalcDecay()
{
    float n   = decayTime * (float)sampleRate;
    decayCoeff = coeff (n, kDRRatio);
    decayBase  = (sustainLevel - kDRRatio) * (1.0f - decayCoeff);
}

void EnvGen::recalcRelease()
{
    float n     = releaseTime * (float)sampleRate;
    releaseCoeff = coeff (n, kDRRatio);
    releaseBase  = -kDRRatio * (1.0f - releaseCoeff);
}

float EnvGen::coeff (float nSamples, float targetRatio) const
{
    if (nSamples <= 0.0f) return 0.0f;
    return std::exp (-std::log ((1.0f + targetRatio) / targetRatio) / nSamples);
}
