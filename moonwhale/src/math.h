#pragma once
#include "common.h"
#include <math.h>

constexpr float pi32 = 3.14159265359f;

union Vec2
{
    struct
    {
        float x, y;
    };
    struct
    {
        float width, height;
    };

    inline bool operator==(Vec2 other) { return (x == other.x && y == other.y); }
    inline float dot(Vec2 other) { return (x * other.x + y * other.y); }
    inline float lengthSq() { return (x * x + y * y); }
    inline float length() { return sqrtf(lengthSq()); }
};

inline Vec2 operator-(Vec2 v) { return Vec2{ -v.x, -v.y }; }
inline Vec2 operator+(Vec2 a, Vec2 b) { return Vec2{ a.x + b.x, a.y + b.y }; }
inline Vec2 operator-(Vec2 a, Vec2 b) { return a + (-b); }
inline Vec2 operator*(Vec2 a, Vec2 b) { return Vec2{ a.x * b.x, a.y * b.y }; }
inline Vec2 operator*(Vec2 a, float b) { return Vec2{ a.x * b, a.y * b }; }
inline Vec2 operator/(Vec2 a, Vec2 b) { return Vec2{ a.x / b.x, a.y / b.y }; }
inline Vec2 operator/(Vec2 a, float b) { return a * (1.f / b); }


struct Vec3
{
    float x, y, z;

    inline bool operator==(Vec3 other) { return (x == other.x && y == other.y && z == other.z); }
    inline float dot(Vec3 other) { return (x * other.x + y * other.y + z * other.z); }
    inline Vec3 cross(Vec3 other) { return Vec3{ y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x }; }
    inline float lengthSq() { return (x * x + y * y + z * z); }
    inline float length() { return sqrtf(lengthSq()); }
};

inline Vec3 operator-(Vec3 v) { return Vec3{ -v.x, -v.y, -v.z }; }
inline Vec3 operator+(Vec3 a, Vec3 b) { return Vec3{ a.x + b.x, a.y + b.y, a.z + b.z }; }
inline Vec3 operator-(Vec3 a, Vec3 b) { return a + (-b); }
inline Vec3 operator*(Vec3 a, Vec3 b) { return Vec3{ a.x * b.x, a.y * b.y, a.z * b.z }; }
inline Vec3 operator*(Vec3 a, float b) { return Vec3{ a.x * b, a.y * b, a.z * b }; }
inline Vec3 operator/(Vec3 a, Vec3 b) { return Vec3{ a.x / b.x, a.y / b.y, a.z / b.z }; }
inline Vec3 operator/(Vec3 a, float b) { return a * (1.f / b); }


struct Vec4
{
    float x, y, z, w;
};

union Vec2i
{
    struct
    {
        int x, y;
    };
    struct
    {
        int width, height;
    };

};

inline Vec2i operator-(Vec2i a, Vec2i b) { return Vec2i{ a.x - b.x, a.y - b.y }; }


// Row major
struct Mat4
{
    float e[4][4];

    Mat4() = default;
    Mat4(
        float e00, float e01, float e02, float e03,
        float e10, float e11, float e12, float e13,
        float e20, float e21, float e22, float e23,
        float e30, float e31, float e32, float e33)
        : e{ { e00, e10, e20, e30 },
             { e01, e11, e21, e31 },
             { e02, e12, e22, e32 },
             { e03, e13, e23, e33 } }
    {
    }

    inline static Mat4 diagonal(float v)
    {
        return Mat4
        (
            v, 0, 0, 0,
            0, v, 0, 0,
            0, 0, v, 0,
            0, 0, 0, v
        );
    }
    inline static Mat4 identity() { return diagonal(1.f); }
    inline static Mat4 transpose(Mat4 m)
    {
        for (int column = 0; column < 4; ++column)
        {
            for (int row = 0; row < 4; ++row)
            {
                float tmp = m.e[column][row];
                m.e[column][row] = m.e[row][column];
                m.e[row][column] = tmp;
            }
        }
        return m;
    }

    inline static Mat4 ortho(float left, float right, float bottom, float top, float nearZ, float farZ)
    {
        Mat4 result = {};
        result.e[0][0] = 2.f / (right - left);
        result.e[1][1] = 2.f / (top - bottom);
        result.e[2][2] = 2.f / (nearZ - farZ);
        result.e[3][3] = 1.f;
        result.e[3][0] = (left + right) / (left - right);
        result.e[3][1] = (bottom + top) / (bottom - top);
        result.e[3][2] = (nearZ + farZ) / (nearZ - farZ);
        return result;
    }

    inline static Mat4 persp(float fov, float aspectRatio, float nearZ, float farZ)
    {
        Mat4 result = {};
        float tanThetaOver2 = tanf(fov * (pi32 / 360.f));
        result.e[0][0] = 1.f / tanThetaOver2;
        result.e[1][1] = aspectRatio / tanThetaOver2;
        result.e[2][3] = -1.f;
        result.e[2][2] = (nearZ + farZ) / (nearZ - farZ);
        result.e[3][2] = (2.f * nearZ * farZ) / (nearZ - farZ);
        return result;
    }

    inline static Mat4 translate(float x, float y, float z)
    {
        Mat4 result = identity();
        result.e[3][0] = x;
        result.e[3][1] = y;
        result.e[3][2] = z;
        return result;
    }

    inline static Mat4 scale(float x, float y, float z)
    {
        Mat4 result = {};
        result.e[0][0] = x;
        result.e[1][1] = y;
        result.e[2][2] = z;
        result.e[3][3] = 1.f;
        return result;
    }

    inline static common::Result<Mat4> inverse(Mat4 in)
    {
        float inv[16];
        float det;
        int i;
        float* m = (float*)in.e;

        Mat4 result = {};
        auto out = &result;

        inv[0] = m[5] * m[10] * m[15] -
            m[5] * m[11] * m[14] -
            m[9] * m[6] * m[15] +
            m[9] * m[7] * m[14] +
            m[13] * m[6] * m[11] -
            m[13] * m[7] * m[10];

        inv[4] = -m[4] * m[10] * m[15] +
            m[4] * m[11] * m[14] +
            m[8] * m[6] * m[15] -
            m[8] * m[7] * m[14] -
            m[12] * m[6] * m[11] +
            m[12] * m[7] * m[10];

        inv[8] = m[4] * m[9] * m[15] -
            m[4] * m[11] * m[13] -
            m[8] * m[5] * m[15] +
            m[8] * m[7] * m[13] +
            m[12] * m[5] * m[11] -
            m[12] * m[7] * m[9];

        inv[12] = -m[4] * m[9] * m[14] +
            m[4] * m[10] * m[13] +
            m[8] * m[5] * m[14] -
            m[8] * m[6] * m[13] -
            m[12] * m[5] * m[10] +
            m[12] * m[6] * m[9];

        inv[1] = -m[1] * m[10] * m[15] +
            m[1] * m[11] * m[14] +
            m[9] * m[2] * m[15] -
            m[9] * m[3] * m[14] -
            m[13] * m[2] * m[11] +
            m[13] * m[3] * m[10];

        inv[5] = m[0] * m[10] * m[15] -
            m[0] * m[11] * m[14] -
            m[8] * m[2] * m[15] +
            m[8] * m[3] * m[14] +
            m[12] * m[2] * m[11] -
            m[12] * m[3] * m[10];

        inv[9] = -m[0] * m[9] * m[15] +
            m[0] * m[11] * m[13] +
            m[8] * m[1] * m[15] -
            m[8] * m[3] * m[13] -
            m[12] * m[1] * m[11] +
            m[12] * m[3] * m[9];

        inv[13] = m[0] * m[9] * m[14] -
            m[0] * m[10] * m[13] -
            m[8] * m[1] * m[14] +
            m[8] * m[2] * m[13] +
            m[12] * m[1] * m[10] -
            m[12] * m[2] * m[9];

        inv[2] = m[1] * m[6] * m[15] -
            m[1] * m[7] * m[14] -
            m[5] * m[2] * m[15] +
            m[5] * m[3] * m[14] +
            m[13] * m[2] * m[7] -
            m[13] * m[3] * m[6];

        inv[6] = -m[0] * m[6] * m[15] +
            m[0] * m[7] * m[14] +
            m[4] * m[2] * m[15] -
            m[4] * m[3] * m[14] -
            m[12] * m[2] * m[7] +
            m[12] * m[3] * m[6];

        inv[10] = m[0] * m[5] * m[15] -
            m[0] * m[7] * m[13] -
            m[4] * m[1] * m[15] +
            m[4] * m[3] * m[13] +
            m[12] * m[1] * m[7] -
            m[12] * m[3] * m[5];

        inv[14] = -m[0] * m[5] * m[14] +
            m[0] * m[6] * m[13] +
            m[4] * m[1] * m[14] -
            m[4] * m[2] * m[13] -
            m[12] * m[1] * m[6] +
            m[12] * m[2] * m[5];

        inv[3] = -m[1] * m[6] * m[11] +
            m[1] * m[7] * m[10] +
            m[5] * m[2] * m[11] -
            m[5] * m[3] * m[10] -
            m[9] * m[2] * m[7] +
            m[9] * m[3] * m[6];

        inv[7] = m[0] * m[6] * m[11] -
            m[0] * m[7] * m[10] -
            m[4] * m[2] * m[11] +
            m[4] * m[3] * m[10] +
            m[8] * m[2] * m[7] -
            m[8] * m[3] * m[6];

        inv[11] = -m[0] * m[5] * m[11] +
            m[0] * m[7] * m[9] +
            m[4] * m[1] * m[11] -
            m[4] * m[3] * m[9] -
            m[8] * m[1] * m[7] +
            m[8] * m[3] * m[5];

        inv[15] = m[0] * m[5] * m[10] -
            m[0] * m[6] * m[9] -
            m[4] * m[1] * m[10] +
            m[4] * m[2] * m[9] +
            m[8] * m[1] * m[6] -
            m[8] * m[2] * m[5];

        det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

        if (det == 0)
            return { false, Mat4{} };

        det = 1.0f / det;

        float* invOut = (float*)out->e;
        for (i = 0; i < 16; i++)
            invOut[i] = inv[i] * det;

        return { true, *out };
    }

};

inline Mat4 operator-(Mat4 m)
{
    Mat4 result = {};
    for (int column = 0; column < 4; ++column)
    {
        for (int row = 0; row < 4; ++row)
        {
            result.e[column][row] = -m.e[column][row];
        }
    }
    return result;
}

inline Mat4 operator+(Mat4 a, Mat4 b)
{
    Mat4 result = {};
    for (int column = 0; column < 4; ++column)
    {
        for (int row = 0; row < 4; ++row)
        {
            result.e[column][row] = a.e[column][row] + b.e[column][row];
        }
    }
    return result;
}

inline Mat4 operator-(Mat4 a, Mat4 b)
{
    return a + (-b);
}

inline Mat4 operator*(Mat4 a, Mat4 b)
{
    Mat4 result = {};
    for (int column = 0; column < 4; ++column)
    {
        for (int row = 0; row < 4; ++row)
        {
            float sum = 0;
            for (int i = 0; i < 4; ++i)
            {
                sum += a.e[i][row] * b.e[column][i];
            }
            result.e[column][row] = sum;
        }
    }
    return result;
}

inline Mat4 operator*(Mat4 a, float b)
{
    Mat4 result = {};
    for (int column = 0; column < 4; ++column)
    {
        for (int row = 0; row < 4; ++row)
        {
            result.e[column][row] = a.e[column][row] * b;
        }
    }
    return result;
}

inline Vec4 operator*(Mat4 a, Vec4 b)
{
    Vec4 result = {
        a.e[0][0] * b.x + a.e[1][0] * b.y + a.e[2][0] * b.z + a.e[3][0] * b.w,
        a.e[0][1] * b.x + a.e[1][1] * b.y + a.e[2][1] * b.z + a.e[3][1] * b.w,
        a.e[0][2] * b.x + a.e[1][2] * b.y + a.e[2][2] * b.z + a.e[3][2] * b.w,
        a.e[0][3] * b.x + a.e[1][3] * b.y + a.e[2][3] * b.z + a.e[3][3] * b.w
    };
    return result;
}