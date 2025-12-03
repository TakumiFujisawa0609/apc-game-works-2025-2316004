#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include "Easing.h"


void Easing::SetEasing(const float t, const EASING_TYPE type)
{
    switch (type)
    {
    case Easing::EASING_TYPE::LERP:
        easingUpdate_ = [this, t](float) {return Lerp(t); };
        break;
    case Easing::EASING_TYPE::QUAD_IN:
        easingUpdate_ = [this, t](float) {return EaseQuadIn(t); };
        break;
    case Easing::EASING_TYPE::QUAD_OUT:
        easingUpdate_ = [this, t](float) {return EaseQuadOut(t); };
        break;
    case Easing::EASING_TYPE::QUAD_IN_OUT:
        easingUpdate_ = [this, t](float) {return EaseQuadInOut(t); };
        break;
    case Easing::EASING_TYPE::QUAD_OUT_IN:
        easingUpdate_ = [this, t](float) {return EaseQuadOutIn(t); };
        break;
    case Easing::EASING_TYPE::CUBIC_IN:
        easingUpdate_ = [this, t](float) {return EaseCubicIn(t); };
        break;
    case Easing::EASING_TYPE::CUBIC_OUT:
        easingUpdate_ = [this, t](float) {return EaseCubicOut(t); };
        break;
    case Easing::EASING_TYPE::EXPO:
        easingUpdate_ = [this, t](float) {return EaseExpo(t); };
        break;
    case Easing::EASING_TYPE::ELASTIC_OUT:
        easingUpdate_ = [this, t](float) {return EaseOutElastic(t); };
        break;
    case Easing::EASING_TYPE::BOUNCE:
        easingUpdate_ = [this, t](float) {return EaseBounce(t); };
        break;
    default:
        break;
    }
}

int Easing::EaseFunc(const int start, const int end, const float t, const EASING_TYPE type)
{
    SetEasing(t, type);
    int dis = end - start;
    int ret = UtilityCommon::Round(start + dis * easingUpdate_(t));
    return ret;
}

float Easing::EaseFunc(const float start, const float end, const float t,const EASING_TYPE type)
{
    SetEasing(t, type);
    float dis = end - start;
    return start + dis * easingUpdate_(t);
}
double Easing::EaseFunc(const double start, const double end, const float t, const EASING_TYPE type)
{
    SetEasing(t, type);
    double dis = end - start;
    return start + dis * easingUpdate_(t);
}
Vector2F Easing::EaseFunc(const Vector2F& start, const Vector2F& end, const float t, const EASING_TYPE type)
{
    SetEasing(t, type);
    Vector2F dis = end - start;
    return start + dis * easingUpdate_(t);
}
Vector2 Easing::EaseFunc(const Vector2& start, const Vector2& end, const float t, const EASING_TYPE type)
{
    SetEasing(t, type);
    Vector2 dis = end - start;
    Vector2 ret = {};
    ret.x = UtilityCommon::Round(start.x + dis.x * easingUpdate_(t));
    ret.y = UtilityCommon::Round(start.y + dis.y * easingUpdate_(t));
    return ret;
}
VECTOR Easing::EaseFunc(const VECTOR& start, const VECTOR& end, const float t, const EASING_TYPE type)
{
    SetEasing(t, type);
    VECTOR dis = VSub(end, start);
    VECTOR power = VScale(dis, easingUpdate_(t));
    return VAdd(start, power);
}

COLOR_F Easing::EaseFunc(const COLOR_F& start, const COLOR_F& end, const float t, const EASING_TYPE type)
{
    // üŒ`•âŠÔ
    if (t >= 1.0f)
    {
        return end;
    }
    SetEasing(t, type);
    COLOR_F ret = start;
    ret.r += (end.r - start.r) * easingUpdate_(t);
    ret.g += (end.g - start.g) * easingUpdate_(t);
    ret.b += (end.b - start.b) * easingUpdate_(t);
    ret.a += (end.a - start.a) * easingUpdate_(t);
    return ret;
}

float Easing::EaseFuncDeg(float& start, float& end, const float t, const EASING_TYPE type)
{
    float ret;

    float diff = end - start;
    if (diff < -180.0f)
    {
        end += 360.0;
        ret = EaseFunc(start, end, t, type);
        if (ret >= 360.0f)
        {
            ret -= 360.0f;
        }
    }
    else if (diff > 180.0f)
    {
        end -= 360.0f;
        ret = EaseFunc(start, end, t, type);
        if (ret < 0.0)
        {
            ret += 360.0f;
        }
    }
    else
    {
        ret = EaseFunc(start, end, t, type);
    }

    return ret;
}

double Easing::EaseFuncDeg(double& start, double& end, float t, const EASING_TYPE type)
{
    double ret;

    double diff = end - start;
    if (diff < -180.0)
    {
        end += 360.0;
        ret = EaseFunc(start, end, t, type);
        if (ret >= 360.0f)
        {
            ret -= 360.0f;
        }
    }
    else if (diff > 180.0)
    {
        end -= 360.0;
        ret = EaseFunc(start, end, t, type);
        if (ret < 0.0)
        {
            ret += 360.0f;
        }
    }
    else
    {
        ret = EaseFunc(start, end, t, type);
    }

    return ret;
}

float Easing::EaseFuncRad(float& start, float& end, const float t, const EASING_TYPE type)
{
    SetEasing(t, type);
    float diff = remainder(end - start, 2.0f * DX_PI_F);
    return start + diff * easingUpdate_(t);
}

float Easing::Lerp(const float t)
{
    if (t > 1.0f)return 1.0f;
    //Š„‡‚¾‚¯‚ð•Ô‚µ‚ÄAFunc‚ÅŒvŽZ
    return t;
}



float Easing::EaseOutElastic(const float t)
{
    if (t > 1.0f)return 1.0f;
    float ret = 0.0f;
    const float c4 = (2.0f * DX_PI_F) / 3.0f;
    ret = (powf(2, -10.0f * t) * sinf((t * 10.0f - 0.75f) * c4) + 1.0f);
    return ret;
}


float Easing::EaseBounce(const float t)
{
    if (t > 1.0f)return 1.0f;
    const float BOUNCE_POW = 2.75f;
    const float BOUNCE_ACCEL = 7.5625;
    MATH_FUNC quad;
    float ret = 0.0f;
    quad.accel = BOUNCE_ACCEL;
    if (t < 1.0f / BOUNCE_POW)
    {
        return EaseQuadIn(t / (1.0f / BOUNCE_POW));
    }
    else if (1.0f / BOUNCE_POW <= t && t < 2.0f / BOUNCE_POW)
    {
        quad.graph_vertex = { 1.5f / BOUNCE_POW,0.75f };
        ret = quad.QuadFunc(t);

    }
    else if (2.0f / BOUNCE_POW <= t && t < 2.5f / BOUNCE_POW)
    {
        quad.graph_vertex = { 2.25f / BOUNCE_POW,0.9375f };
        ret = quad.QuadFunc(t);
    }
    else
    {
        quad.graph_vertex = { 2.625f / BOUNCE_POW,0.984375f };
        ret = quad.QuadFunc(t);
    }
    return ret;
}

float Easing::EaseQuadIn(const float t)
{
    if (t >= 1.0f)return 1.0f;
    MATH_FUNC quad;
    float ret = quad.QuadFunc(t);
    return ret;
}



float Easing::EaseQuadOut(const float t)
{
    if (t >= 1.0f)return 1.0f;
    MATH_FUNC quad;
    quad.accel = -1.0f;
    quad.graph_vertex = { 1.0f,1.0f };
    float ret = quad.QuadFunc(t);
    return ret;
}


float Easing::EaseQuadInOut(const float t)
{
    if (t >= 1.0f)return 1.0F;
    float ret = 0.0f;
    MATH_FUNC quad;
    if (t <= HALF)
    {
        quad.accel = 2.0f;
    }
    else
    {
        quad.graph_vertex = { 1.0f,1.0f };
        quad.accel = -2.0f;
    }
    ret = quad.QuadFunc(t);
    return ret;
}


float Easing::EaseQuadOutIn(const float t)
{
    if (t >= 1.0f)return 1.0f;
    float ret = 0.0f;
    MATH_FUNC quad;
    if (t < 0.5f)
    {
        quad.accel = -2.0f;
        quad.graph_vertex = { 0.5f,0.5f };
    }
    else
    {
        quad.accel = 2.0f;
        quad.graph_vertex = { 0.5f,0.5f };
    }
    ret = quad.QuadFunc(t);
    return ret;
}


float Easing::EaseCubicIn(const float t)
{
    if (t >= 1.0f)return 1.0f;
    MATH_FUNC qubic;
    float ret = qubic.CubicFunc(t);
    return ret;
}


float Easing::EaseCubicOut(const float t)
{
    if (t >= 1.0f)return 1.0f;
    MATH_FUNC qubic;
    qubic.graph_vertex = { -1.0f,1.0f };
    float ret = qubic.CubicFunc(t);
    return ret;
}



float Easing::EaseCubicInOut(const float start, const float end, const float t)
{
    return 0.0f;
}

float Easing::EaseCubicOutIn(const float start, const float end, const float t)
{
    return 0.0f;
}

float Easing::EaseExpo(const float t, const int expo)
{
    if (t >= 1.0f)return 1.0f;
    float ret = 0.0f;
    float scaled = powf(1.0f - t, static_cast<float>(expo));
    float base = 1.0f - scaled;
    float inv_t = 1.0f / t;
    float expoFunc = powf(base, inv_t);
    ret = expoFunc;

    return ret;
}


Vector2F Easing::EaseEpiCycloid(const Vector2F& start, const float t, const float halfRadiusNum, const float smallRadius)
{
    if (t > 1.0f)return start;

    float rad = DX_TWO_PI_F * t;
    float baseRadius = smallRadius * halfRadiusNum;
    Vector2F ret = {};
    ret.x = (baseRadius + smallRadius) * cos(rad) - smallRadius * cos(((baseRadius + smallRadius) / smallRadius) * rad);
    ret.y = (baseRadius + smallRadius) * sin(rad) - smallRadius * sin(((baseRadius + smallRadius) / smallRadius) * rad);
    return start + ret;
}

Vector2F Easing::EaseHypoCycloid(const Vector2F& start, const float t, const float halfRadiusNum, const float smallRadius)
{
    if (t > 1.0f)return start;

    float rad = DX_TWO_PI_F * t;
    float baseRadius = smallRadius * halfRadiusNum;
    Vector2F ret = {};
    ret.x = (baseRadius - smallRadius) * cos(rad) + smallRadius * cos(((baseRadius - smallRadius) / smallRadius) * rad);
    ret.y = (baseRadius - smallRadius) * sin(rad) - smallRadius * sin(((baseRadius - smallRadius) / smallRadius) * rad);
    return start + ret;
}


