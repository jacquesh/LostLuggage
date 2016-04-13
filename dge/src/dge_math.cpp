#include "dge_math.h"

// ==============================
// dge::Vector2 method implementations
// ==============================
float dge::Vector2::dot(dge::Vector2& rhs)
{
    return x*rhs.x + y*rhs.y;
}

float dge::Vector2::sqrMagnitude()
{
    return x*x + y*y;
}

float dge::Vector2::magnitude()
{
    return sqrt(sqrMagnitude());
}

void dge::Vector2::normalize()
{
    float mag = magnitude();
    if(mag > 0.0f)
    {
        float inverseMagnitude = 1.0f/mag;
        x *= inverseMagnitude;
        y *= inverseMagnitude;
    }
}

dge::Vector2 dge::Vector2::normalized()
{
    dge::Vector2 result(x, y);
    result.normalize();
    return result;
}

// ==========================
// dge::Vector2 operator overloads
// ==========================
dge::Vector2 dge::operator +(dge::Vector2 lhs, dge::Vector2 rhs)
{
    dge::Vector2 result;
    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    return result;
}

dge::Vector2 dge::operator -(dge::Vector2 lhs, dge::Vector2 rhs)
{
    dge::Vector2 result;
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    return result;
}

dge::Vector2 dge::operator -(dge::Vector2 rhs)
{
    dge::Vector2 result;
    result.x = -rhs.x;
    result.y = -rhs.y;
    return result;
}

dge::Vector2 dge::operator *(dge::Vector2 lhs, float rhs)
{
    dge::Vector2 result;
    result.x = lhs.x * rhs;
    result.y = lhs.y * rhs;
    return result;
}

dge::Vector2 dge::operator *(float lhs, dge::Vector2 rhs)
{
    dge::Vector2 result;
    result.x = lhs * rhs.x;
    result.y = lhs * rhs.y;
    return result;
}

dge::Vector2 dge::operator *(dge::Vector2 lhs, dge::Vector2 rhs)
{
    dge::Vector2 result;
    result.x = lhs.x * rhs.x;
    result.y = lhs.x * rhs.y;
    return result;
}

// TODO: Shouldn't these return dge::Vector2&?
dge::Vector2 dge::operator +=(dge::Vector2& lhs, dge::Vector2 rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

dge::Vector2 dge::operator -=(dge::Vector2& lhs, dge::Vector2 rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

dge::Vector2 dge::operator *=(dge::Vector2& lhs, float rhs)
{
    lhs.x *= rhs;
    lhs.y *= rhs;
    return lhs;
}

// ===========================
// dge::Vector2I operator overloads
// ===========================
dge::Vector2I dge::operator +(dge::Vector2I lhs, dge::Vector2I rhs)
{
    dge::Vector2I result;
    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    return result;
}

dge::Vector2I dge::operator -(dge::Vector2I lhs, dge::Vector2I rhs)
{
    dge::Vector2I result;
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    return result;
}

dge::Vector2I dge::operator *(dge::Vector2I lhs, int32 rhs)
{
    dge::Vector2I result;
    result.x = lhs.x * rhs;
    result.y = lhs.y * rhs;
    return result;
}

dge::Vector2I dge::operator *(int32 lhs, dge::Vector2I rhs)
{
    dge::Vector2I result;
    result.x = lhs * rhs.x;
    result.y = lhs * rhs.y;
    return result;
}

dge::Vector2 dge::operator *(dge::Vector2I lhs, float rhs)
{
    dge::Vector2 result;
    result.x = lhs.x * rhs;
    result.y = lhs.y * rhs;
    return result;
}

dge::Vector2 dge::operator *(float lhs, dge::Vector2I rhs)
{
    dge::Vector2 result;
    result.x = lhs * rhs.x;
    result.y = lhs * rhs.y;
    return result;
}

dge::Vector2I dge::operator *(dge::Vector2I lhs, dge::Vector2I rhs)
{
    dge::Vector2I result;
    result.x = lhs.x * rhs.x;
    result.y = lhs.y * rhs.y;
    return result;
}

dge::Vector2I dge::operator +=(dge::Vector2I& lhs, dge::Vector2I rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

bool dge::operator ==(dge::Vector2I lhs, dge::Vector2I rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

// ===========================
// dge::Vector4 operator overloads
// ===========================
dge::Vector4 dge::operator *(dge::Vector4 lhs, dge::Vector4 rhs)
{
    dge::Vector4 result(lhs);
    result.x *= rhs.x;
    result.y *= rhs.y;
    result.z *= rhs.z;
    result.w *= rhs.w;
    return result;
}

dge::Vector4 dge::operator *=(dge::Vector4& lhs, dge::Vector4 rhs)
{
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    lhs.z *= rhs.z;
    lhs.w *= rhs.w;
    return lhs;
}

// ========================
// Function implementations
// ========================
int dge::round(float x)
{
    return (int)(x + 0.5f);
}
int dge::round(double x)
{
    return (int)(x + 0.5);
}

float dge::clamp(float x, float min, float max)
{
    if(x < min)
    {
        x = min;
    }
    else if(x > max)
    {
        x = max;
    }
    return x;
}

float dge::lerp(float from, float to, float t)
{
    return (1.0f-t)*from + t*to;
}

dge::Vector2 dge::lerp(dge::Vector2 from, dge::Vector2 to, float t)
{
    return (1.0f - t)*from + t*to;
}

int dge::sign(int x)
{
    int result;
    if(x > 0) result = 1;
    else if(x < 0) result = -1;
    else result = 0;
    return result;
}

float dge::signf(float x)
{
    float result;
    if(x > 0.0f) result = 1.0f;
    else if(x < 0.0f) result = -1.0f;
    else result = 0.0f;
    return result;
}

float dge::slide(float from, float to, float maxDistance)
{
    float offset = to - from;
    float direction = signf(offset);

    float result;
    if(fabs(offset) < maxDistance)
    {
        result = to;
    }
    else
    {
        result = from + direction*maxDistance;
    }

    return result;
}

dge::Vector2 dge::slide(Vector2 from, Vector2 to, float maxDistance)
{
    Vector2 offset = to - from;
    Vector2 result;
    if(offset.sqrMagnitude() < maxDistance*maxDistance)
    {
        result = to;
    }
    else
    {
        Vector2 direction = offset.normalized();
        result = from + direction*maxDistance;
    }
    return result;
}

float dge::slideAngle(float from, float to, float maxDistance)
{
    float offset = to - from;
    float topEndWrapOffset = (to + TWOPI) - from;
    float bottomEndWrapOffset = (to - TWOPI) - from;
    if(fabs(offset) > fabs(topEndWrapOffset))
    {
        offset = topEndWrapOffset;
    }
    else if(fabs(offset) > fabs(bottomEndWrapOffset))
    {
        offset = bottomEndWrapOffset;
    }
    float direction = signf(offset);

    float result;
    if(fabs(offset) < maxDistance)
    {
        result = to;
    }
    else
    {
        result = from + direction*maxDistance;
    }

    return result;
}
