#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include "Easing.h"
float Easing::EaseFunc(const float start, const float end, const float per)
{
    float dis = end - start;
    return start + dis * per;
}
Vector2F Easing::EaseFunc(const Vector2F& start, const Vector2F& end, const float per)
{
    Vector2F dis = end - start;
    return start + dis * per;
}
VECTOR Easing::EaseFunc(const VECTOR& start, const VECTOR& end, const float per)
{
    VECTOR dis = VSub(end, start);
    VECTOR power = VScale(dis, per);
    return VAdd(start, power);
} 
int Easing::Lerp(int start, int end, float t)
{
    // 線形補間
    if (t >= 1.0f)
    {
        return end;
    }

    int ret = start;
    ret += UtilityCommon::Round(t * static_cast<float>(end - start));
    return ret;
}

float Easing::Lerp(const float start, const float end, const float t)
{
        // 線形補間
    if (t >= 1.0f)
    {
        return end;
    }

    float ret = start;
    ret += t * (end - start);
    return ret;
}

double Easing::Lerp(double start, double end, double t)
{
    // 線形補間
    if (t >= 1.0)
    {
        return end;
    }

    double ret = start;
    ret += t * (end - start);
    return ret;
}

Vector2 Easing::Lerp(const Vector2& start, const Vector2& end, float t)
{
    // 線形補間
    if (t >= 1.0f)
    {
        return end;
    }

    Vector2 ret = start;
    ret.x += UtilityCommon::Round(t * static_cast<float>((end.x - start.x)));
    ret.y += UtilityCommon::Round(t * static_cast<float>((end.y - start.y)));
    return ret;
}

Vector2F Easing::Lerp(const Vector2F& start, const Vector2F& end, float t)
{
    // 線形補間
    if (t >= 1.0f)
    {
        return end;
    }

    Vector2F ret = start;
    ret.x += t * (end.x - start.x);
    ret.y += t * (end.y - start.y);
    return ret;
}

VECTOR Easing::Lerp(const VECTOR& start, const VECTOR& end, float t)
{
    // 線形補間
    if (t >= 1.0f)
    {
        return end;
    }

    VECTOR ret = start;
    ret.x += t * (end.x - start.x);
    ret.y += t * (end.y - start.y);
    ret.z += t * (end.z - start.z);

    return ret;
}

double Easing::LerpDeg(double& start, double& end, double t)
{

    double ret;

    double diff = end - start;
    if (diff < -180.0)
    {
        end += 360.0;
        ret = Lerp(start, end, t);
        if (ret >= 360.0)
        {
            ret -= 360.0;
        }
    }
    else if (diff > 180.0)
    {
        end -= 360.0;
        ret = Lerp(start, end, t);
        if (ret < 0.0)
        {
            ret += 360.0;
        }
    }
    else
    {
        ret = Lerp(start, end, t);
    }

    return ret;

}

float Easing::LerpDeg(float& start, float& end, float t)
{

    float ret;

    float diff = end - start;
    if (diff < -180.0f)
    {
        end += 360.0;
        ret = Lerp(start, end, t);
        if (ret >= 360.0f)
        {
            ret -= 360.0f;
        }
    }
    else if (diff > 180.0f)
    {
        end -= 360.0;
        ret = Lerp(start, end, t);
        if (ret < 0.0)
        {
            ret += 360.0f;
        }
    }
    else
    {
        ret = Lerp(start, end, t);
    }

    return ret;

}

float Easing::LerpRad(float& start, float& end, float t)
{
    float diff = remainder(end - start, 2.0f * DX_PI_F);
    return start + diff * t;
}

float Easing::EaseInOut(const float start, const float end, const float t)
{
    return 0.0f;
}

float Easing::EaseOutElasticPer(const float t)
{
    if (t > 1.0f)return 1.0f;
    float ret = 0.0f;
    const float c4 = (2 * DX_PI_F) / 3;
    ret = (pow(2, -10.0f * t) * sinf((t * 10.0f - 0.75f) * c4) + 1.0f);
    return ret;
}

float Easing::EaseOutElastic(const float start, const float end, const float t)
{
    if (t > 1.0f)return end;
    float dis = end - start;
    float ret = 0.0f;
    const float c4 = (2 * DX_PI_F) / 3;
    ret = start+dis*(pow(2, -10.0f * t) * sinf((t * 10.0f - 0.75f) * c4) + 1.0f);
    return ret;
}

Vector2F Easing::EaseOutElastic(const Vector2F& start, const Vector2F& end, float t)
{
    if (t > 1.0f)return end;
    Vector2F ret = {};
    ret.x = EaseOutElastic(start.x, end.x, t);
    ret.y = EaseOutElastic(start.y, end.y, t);

    return ret;
}

VECTOR Easing::EaseOutElastic(const VECTOR& start, const VECTOR& end, const float& t)
{
    VECTOR ret = {};
    ret.x = EaseOutElastic(start.x, end.x, t);
    ret.y = EaseOutElastic(start.y, end.y, t);
    ret.z = EaseOutElastic(start.z, end.z, t);
    return ret;
}

float Easing::EaseBounce(const float start, const float end, const float t)
{
    if (t > 1.0f)return end;
    float dis = end - start;
    const float BOUNCE_POW = 2.75f;
    const float BOUNCE_ACCEL = 7.5625;
    MATH_FUNC quad;
    float ret = 0.0f;
    quad.accel = BOUNCE_ACCEL;
    if (t < 1.0f / BOUNCE_POW)
    {
        return EaseQuadIn(start, end, t / (1.0f / BOUNCE_POW));
    }
    else if (1.0f / BOUNCE_POW <= t && t < 2.0f / BOUNCE_POW)
    {
        quad.graph_vertex = { 1.5f / BOUNCE_POW,0.75f };
        ret = start + dis * quad.QuadFunc(t);

    }
    else if (2.0f / BOUNCE_POW <= t && t < 2.5f / BOUNCE_POW)
    {
        quad.graph_vertex = { 2.25f / BOUNCE_POW,0.9375f };
        ret = start + dis * quad.QuadFunc(t);
    }
    else
    {
        quad.graph_vertex = { 2.625f / BOUNCE_POW,0.984375f };
        ret = start + dis * quad.QuadFunc(t);
    }
    return ret;
}

Vector2F Easing::EaseBounce(const Vector2F& start, const Vector2F& end, const float& t)
{
    if (t > 1.0f)return end;
    Vector2F ret = {};
    ret.x = EaseBounce(start.x, end.x, t);
    ret.y = EaseBounce(start.y, end.y, t);
    return ret;
}

VECTOR Easing::EaseBounce(const VECTOR& start, const VECTOR& end, const float& t)
{
    if (t > 1.0f)return end;
    VECTOR ret = {};
    ret.x = EaseBounce(start.x, end.x, t);
    ret.y = EaseBounce(start.y, end.y, t);
    ret.z = EaseBounce(start.z, end.z, t);
    return ret;
}

float Easing::EaseQuadIn(const float start, const float end, const float t)
{
    if (t >= 1.0f)return end;
    MATH_FUNC quad;
    float dis = end - start;

    float ret = start + dis * quad.QuadFunc(t);
    return ret;
}

Vector2F Easing::EaseQuadIn(const Vector2F& start, const Vector2F& end, const float t)
{
    if (t >= 1.0f)return end;
    Vector2F ret = {};
    ret.x = EaseQuadIn(start.x, end.x, t);
    ret.y = EaseQuadIn(start.y, end.y, t);
    return ret;
}

VECTOR Easing::EaseQuadIn(const VECTOR& start, const VECTOR& end, const float t)
{
    if (t >= 1.0f)return end;
    VECTOR ret = {};
    ret.x = EaseQuadIn(start.x, end.x, t);
    ret.y = EaseQuadIn(start.y, end.y, t);
    ret.z = EaseQuadIn(start.z, end.z, t);
    return ret;
}

float Easing::EaseQuadOut(const float start, const float end, const float t)
{
    if (t >= 1.0f)return end;
    float dis = end - start;
    MATH_FUNC quad;
    quad.accel = -1.0f;
    quad.graph_vertex = { 1.0f,1.0f };
    float ret = start + dis * quad.QuadFunc(t);
    return ret;
}

Vector2F Easing::EaseQuadOut(const Vector2F& start, const Vector2F& end, const float t)
{
    if (t >= 1.0f)return end;
    Vector2F ret = {};
    ret.x = EaseQuadOut(start.x, end.x, t);
    ret.y = EaseQuadOut(start.y, end.y, t);
    return ret;
}

VECTOR Easing::EaseQuadOut(const VECTOR& start, const VECTOR& end, const float t)
{
    if (t >= 1.0f)return end;
    VECTOR ret = {};
    ret.x = EaseQuadOut(start.x, end.x, t);
    ret.y = EaseQuadOut(start.y, end.y, t);
    ret.z = EaseQuadOut(start.z, end.z, t);
    return ret;
}

float Easing::EaseQuadInOut(const float start, const float end, const float t)
{
    if (t >= 1.0f)return end;
    float ret = 0.0f;
    float dis = end - start;
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
    ret = start + dis * quad.QuadFunc(t);
    return ret;
}

Vector2F Easing::EaseQuadInOut(const Vector2F& start, const Vector2F& end, const float t)
{
    if (t >= 1.0f)return end;
    Vector2F ret = {};
    ret.x = EaseQuadInOut(start.x, end.x, t);
    ret.y = EaseQuadInOut(start.y, end.y, t);
    return ret;
}

VECTOR Easing::EaseQuadInOut(const VECTOR& start, const VECTOR& end, const float t)
{
    if (t >= 1.0f)return end;
    VECTOR ret = {};
    ret.x = EaseQuadInOut(start.x, end.x, t);
    ret.y = EaseQuadInOut(start.y, end.y, t);
    ret.z = EaseQuadInOut(start.z, end.z, t);
    return ret;
}

float Easing::EaseQuadOutIn(const float start, const float end, const float t)
{
    if (t >= 1.0f)return end;
    float ret = 0.0f;
    float dis = end - start;
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
    ret = start + dis * quad.QuadFunc(t);
    return ret;
}

Vector2F Easing::EaseQuadOutIn(const Vector2F& start, const Vector2F& end, const float t)
{
    if (t >= 1.0f)return end;
    Vector2F ret = {};
    ret.x = EaseQuadOutIn(start.x, end.x, t);
    ret.y = EaseQuadOutIn(start.y, end.y, t);
    return ret;
}

VECTOR Easing::EaseQuadOutIn(const VECTOR& start, const VECTOR& end, const float t)
{
    if (t >= 1.0f)return end;
    VECTOR ret = {};
    ret.x = EaseQuadOutIn(start.x, end.x, t);
    ret.y = EaseQuadOutIn(start.y, end.y, t);
    ret.z = EaseQuadOutIn(start.z, end.z, t);
    return ret;
}

float Easing::EaseCubicIn(const float start, const float end, const float t)
{
    if (t >= 1.0f)return end;
    MATH_FUNC qubic;
    float dis = end - start;
    float ret = start + dis * qubic.CubicFunc(t);
    return ret;
}

Vector2F Easing::EaseCubicIn(const Vector2F& start, const Vector2F& end, const float t)
{
    if (t >= 1.0f)return end;
    Vector2F ret = {};
    ret.x = EaseCubicIn(start.x, end.x, t);
    ret.y = EaseCubicIn(start.y, end.y, t);
    return ret;
}

VECTOR Easing::EaseCubicIn(const VECTOR& start, const VECTOR& end, const float t)
{
    if (t >= 1.0f)return end;
    VECTOR ret = {};
    ret.x = EaseCubicIn(start.x, end.x, t);
    ret.y = EaseCubicIn(start.y, end.y, t);
    ret.z = EaseCubicIn(start.z, end.z, t);
    return ret;
}

float Easing::EaseCubicOut(const float start, const float end, const float t)
{
    if (t >= 1.0f)return end;
    MATH_FUNC qubic;
    float dis = (end - start);
    qubic.graph_vertex = { -1.0f,1.0f };
    float ret = start + dis * qubic.CubicFunc(t);
    return ret;
}

Vector2F Easing::EaseCubicOut(const Vector2F start, const Vector2F end, const float t)
{
    if (t >= 1.0f)return end;
    Vector2F ret = {};
    ret.x = EaseCubicOut(start.x, end.x, t);
    ret.y = EaseCubicOut(start.y, end.y, t);
    return ret;
}

VECTOR Easing::EaseCubicOut(const VECTOR start, const VECTOR end, const float t)
{
    if (t >= 1.0f)return end;
    VECTOR ret = {};
    ret.x = EaseCubicOut(start.x, end.x, t);
    ret.y = EaseCubicOut(start.y, end.y, t);
    ret.z = EaseCubicOut(start.z, end.z, t);
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

float Easing::EaseExpo(const float start, const float end, const float t, const float expo)
{
    if (t >= 1.0f)return end;
    float ret = 0.0f;
    float dis = end - start;
    float scaled = powf(1.0f - t, expo);
    float base = 1.0f - scaled;
    float inv_t = 1.0f / t;
    float expofunc = powf(base, inv_t);
    ret = start + dis * expofunc;
    
    return ret;
}

Vector2F Easing::EaseExpo(const Vector2F start, const Vector2F end, const float t, const float expo)
{
    if (t >= 1.0f)return end;
    Vector2F ret = {};
    ret.x = EaseExpo(start.x, end.x, t, expo);
    ret.y = EaseExpo(start.x, end.x, t, expo);
    return ret;
}

VECTOR Easing::EaseExpo(const VECTOR start, const VECTOR end, const float t, const float expo)
{
    if (t >= 1.0f)return end;
    VECTOR ret = {};
    ret.x = EaseExpo(start.x, end.x, t,expo);
    ret.y = EaseExpo(start.x, end.x, t, expo);
    ret.z = EaseExpo(start.x, end.x, t, expo);
    return ret;
}

float Easing::EaseExpoInOut(const float start, const float end, const float t, float expo)
{
    return 0.0f;
}

Vector2F Easing::EaseExpoInOut(const Vector2F& start, const Vector2F& end, const float t, const float expo)
{
    return Vector2F();
}

float Easing::EaseExpoOutIn(const float start, const float end, const float t, float expo)
{
    return 0.0f;
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

COLOR_F Easing::Lerp(const COLOR_F& start, const COLOR_F& end, float t)
{
    // 線形補間
    if (t >= 1.0f)
    {
        return end;
    }

    COLOR_F ret = start;
    ret.r += t * (end.r - start.r);
    ret.g += t * (end.g - start.g);
    ret.b += t * (end.b - start.b);
    ret.a += t * (end.a - start.a);
    return ret;
}
