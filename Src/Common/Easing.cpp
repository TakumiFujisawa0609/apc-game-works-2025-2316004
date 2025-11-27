#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include "Easing.h"
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

float Easing::Lerp(float start, float end, float t)
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
    //float ret;
    //float diff = end - start;
    //if (diff < -DX_PI_F)
    //{
    //    end += DX_TWO_PI_F;
    //    ret = Lerp(start, end, t);
    //    if (ret >= DX_TWO_PI_F)
    //    {
    //        ret -= DX_TWO_PI_F;
    //    }
    //}
    //else if (diff > DX_PI_F)
    //{
    //    end -= DX_TWO_PI_F;
    //    ret = Lerp(start, end, t);
    //    if (ret < 0.0)
    //    {
    //        ret += DX_TWO_PI_F;
    //    }
    //}
    //else
    //{
    //    ret = Lerp(start, end, t);
    //}

    //return ret;
    float diff = remainder(end - start, 2.0f * DX_PI_F);

    return start + diff * t;

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
