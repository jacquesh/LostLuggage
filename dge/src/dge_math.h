#ifndef DGE_MATH_H
#define DGE_MATH_H

#include "dge.h"
#include <math.h>
#include <stdlib.h>


#define PI 3.1415926f
#define TWOPI 6.2831853f
#define PIOVERTWO 1.5707963f
#define DEG2RAD(x) (x*(PI/180.0f))
#define RAD2DEG(x) (x*(180.0f/PI))

namespace dge
{
  //NOTE: Apparently the spec does not define behaviour if you access/modify multiple entries in
  //      a union, although as far as I can tell it works correctly? It might be that it depends
  //      on the struct's packing and this has just always been sufficient in the cases I've seen

  struct Vector2I
  {
      int32 x;
      int32 y;

      Vector2I() : x(0), y(0) {}
      Vector2I(int32 xVal, int32 yVal) : x(xVal), y(yVal) {}
  };

  struct Vector2
  {
      float x;
      float y;

      Vector2() : x(0.0f), y(0.0f) {}
      Vector2(float xVal, float yVal) : x(xVal), y(yVal) {}
      Vector2(Vector2I ivec) : x((float)ivec.x), y((float)ivec.y) {}

      float dot(Vector2& rhs);
      float sqrMagnitude();
      float magnitude();
      void normalize();
      Vector2 normalized();
  };

  Vector2I operator +(Vector2I lhs, Vector2I rhs);
  Vector2I operator -(Vector2I lhs, Vector2I rhs);
  Vector2I operator *(Vector2I lhs, int32 rhs);
  Vector2I operator *(int32 lhs, Vector2I rhs);
  Vector2I operator *(Vector2I lhs, Vector2I rhs);
  Vector2 operator *(Vector2I lhs, float rhs);
  Vector2 operator *(float lhs, Vector2I rhs);
  Vector2I operator +=(Vector2I& lhs, Vector2I rhs);
  bool operator ==(Vector2I lhs, Vector2I rhs);

  Vector2 operator +(Vector2 lhs, Vector2 rhs);
  Vector2 operator -(Vector2 lhs, Vector2 rhs);
  Vector2 operator -(Vector2 rhs);
  Vector2 operator *(Vector2 lhs, float rhs);
  Vector2 operator *(float lhs, Vector2 rhs);
  Vector2 operator *(Vector2 lhs, Vector2 rhs);

  // TODO: Shouldn't these return Vector2&?
  Vector2 operator +=(Vector2& lhs, Vector2 rhs);
  Vector2 operator -=(Vector2& lhs, Vector2 rhs);
  Vector2 operator *=(Vector2& lhs, float rhs);

  struct Vector4
  {
      float x;
      float y;
      float z;
      float w;

      Vector4(float xVal, float yVal, float zVal, float wVal) :
          x(xVal), y(yVal), z(zVal), w(wVal) {}
  };

  Vector4 operator *(Vector4 lhs, Vector4 rhs);
  Vector4 operator *=(Vector4& lhs, Vector4 rhs);


  int sign(int x);
  int round(float x);
  int round(double x);

  float clamp(float x, float min, float max);
  float lerp(float from, float to, float t);
  float signf(float x);
  float slide(float from, float to, float maxDistance);
  float slideAngle(float from, float to, float maxDistance);

  Vector2 lerp(Vector2 from, Vector2 to, float t);
}

#endif
