#pragma once

#include <cmath>
#include <format>
#include <string>

struct vec3
{
  float e[3];

  vec3()
    : e{ 0.0f, 0.0f, 0.0f }
  {
  }
  vec3(float e0, float e1, float e2)
    : e{ e0, e1, e2 }
  {
  }

  float x() const { return e[0]; }
  float y() const { return e[1]; }
  float z() const { return e[2]; }

  vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
  float operator[](int i) const { return e[i]; }
  float& operator[](int i) { return e[i]; }

  vec3& operator+=(const vec3& v)
  {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
  }

  vec3& operator*=(float t)
  {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }

  vec3& operator/=(float t) { return *this *= 1.0f / t; }

  float length() const { return std::sqrt(length_squared()); }

  float length_squared() const
  {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
  }

  std::string to_string() const
  {
    return std::format("{} {} {}", e[0], e[1], e[2]);
  }
};

/* Aliases */

using point3 = vec3;
using color = vec3;

/*  Utils */

vec3
operator+(const vec3& u, const vec3& v)
{
  return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

vec3
operator-(const vec3& u, const vec3& v)
{
  return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

vec3
operator*(const vec3& u, const vec3& v)
{
  return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

vec3
operator*(float t, const vec3& v)
{
  return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

vec3
operator*(const vec3& v, float t)
{
  return t * v;
}

vec3
operator/(const vec3& v, float t)
{
  return (1.0f / t) * v;
}

float
dot(const vec3& u, const vec3& v)
{
  return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

vec3
cross(const vec3& u, const vec3& v)
{
  return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
              u.e[2] * v.e[0] - u.e[0] * v.e[2],
              u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

vec3
unit_vector(const vec3& v)
{
  return v / v.length();
}