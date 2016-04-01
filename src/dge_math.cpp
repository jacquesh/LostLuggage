#include "dge_math.h"

// ==============================
// Vector2 method implementations
// ==============================
float Vector2::dot(Vector2& rhs)
{
    return x*rhs.x + y*rhs.y;
}

float Vector2::sqrMagnitude()
{
    return x*x + y*y;
}

float Vector2::magnitude()
{
    return sqrt(sqrMagnitude());
}

void Vector2::normalize()
{
    float mag = magnitude();
    if(mag > 0.0f)
    {
        float inverseMagnitude = 1.0f/mag;
        x *= inverseMagnitude;
        y *= inverseMagnitude;
    }
}

Vector2 Vector2::normalized()
{
    Vector2 result(x, y);
    result.normalize();
    return result;
}

// ==========================
// Vector2 operator overloads
// ==========================
Vector2 operator +(Vector2 lhs, Vector2 rhs)
{
    Vector2 result;
    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    return result;
}

Vector2 operator -(Vector2 lhs, Vector2 rhs)
{
    Vector2 result;
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    return result;
}

Vector2 operator -(Vector2 rhs)
{
    Vector2 result;
    result.x = -rhs.x;
    result.y = -rhs.y;
    return result;
}

Vector2 operator *(Vector2 lhs, float rhs)
{
    Vector2 result;
    result.x = lhs.x * rhs;
    result.y = lhs.y * rhs;
    return result;
}

Vector2 operator *(float lhs, Vector2 rhs)
{
    Vector2 result;
    result.x = lhs * rhs.x;
    result.y = lhs * rhs.y;
    return result;
}

// TODO: Shouldn't these return Vector2&?
Vector2 operator +=(Vector2& lhs, Vector2 rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

Vector2 operator -=(Vector2& lhs, Vector2 rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

Vector2 operator *=(Vector2& lhs, float rhs)
{
    lhs.x *= rhs;
    lhs.y *= rhs;
    return lhs;
}

// ===========================
// Vector2I operator overloads
// ===========================
Vector2I operator +(Vector2I lhs, Vector2I rhs)
{
    Vector2I result;
    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    return result;
}

Vector2I operator -(Vector2I lhs, Vector2I rhs)
{
    Vector2I result;
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    return result;
}

Vector2I operator *(Vector2I lhs, int32 rhs)
{
    Vector2I result;
    result.x = lhs.x * rhs;
    result.y = lhs.y * rhs;
    return result;
}

Vector2I operator *(int32 lhs, Vector2I rhs)
{
    Vector2I result;
    result.x = lhs * rhs.x;
    result.y = lhs * rhs.y;
    return result;
}

Vector2 operator *(Vector2I lhs, float rhs)
{
    Vector2 result;
    result.x = lhs.x * rhs;
    result.y = lhs.y * rhs;
    return result;
}

Vector2 operator *(float lhs, Vector2I rhs)
{
    Vector2 result;
    result.x = lhs * rhs.x;
    result.y = lhs * rhs.y;
    return result;
}

Vector2I operator +=(Vector2I& lhs, Vector2I rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

bool operator ==(Vector2I lhs, Vector2I rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

// ===========================
// Vector4 operator overloads
// ===========================
Vector4 operator *(Vector4 lhs, Vector4 rhs)
{
    Vector4 result(lhs);
    result.x *= rhs.x;
    result.y *= rhs.y;
    result.z *= rhs.z;
    result.w *= rhs.w;
    return result;
}

Vector4 operator *=(Vector4& lhs, Vector4 rhs)
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
float clamp(float x, float min, float max)
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

float lerp(float from, float to, float t)
{
    return (1.0f-t)*from + t*to;
}

Vector2 lerp(Vector2 from, Vector2 to, float t)
{
    return (1.0f - t)*from + t*to;
}

int sign(int x)
{
    int result;
    if(x > 0) result = 1;
    else if(x < 0) result = -1;
    else result = 0;
    return result;
}

float signf(float x)
{
    float result;
    if(x > 0.0f) result = 1.0f;
    else if(x < 0.0f) result = -1.0f;
    else result = 0.0f;
    return result;
}

float slide(float from, float to, float maxDistance)
{
    float offset = to - from;
    float direction = signf(offset);

    float result;
    if(abs(offset) < maxDistance)
    {
        result = to;
    }
    else
    {
        result = from + direction*maxDistance;
    }

    return result;
}

float slideAngle(float from, float to, float maxDistance)
{
    float offset = to - from;
    float topEndWrapOffset = (to + TWOPI) - from;
    float bottomEndWrapOffset = (to - TWOPI) - from;
    if(abs(offset) > abs(topEndWrapOffset))
    {
        offset = topEndWrapOffset;
    }
    else if(abs(offset) > abs(bottomEndWrapOffset))
    {
        offset = bottomEndWrapOffset;
    }
    float direction = signf(offset);

    float result;
    if(abs(offset) < maxDistance)
    {
        result = to;
    }
    else
    {
        result = from + direction*maxDistance;
    }

    return result;
}
