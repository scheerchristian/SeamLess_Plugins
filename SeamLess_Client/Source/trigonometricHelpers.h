/*
  ==============================================================================

    trigonometricHelpers.h
    Created: 31 Aug 2022 1:22:48pm
    Author:  schee

  ==============================================================================
*/
#ifndef TRIGONOMETRIC_HELPERS
#define TRIGONOMETRIC_HELPERS


#pragma once
#include <math.h>
#define _USE_MATH_DEFINES


inline float radius_from_cartesian(float x, float y, float z)
{
    return sqrt((x * x) + (y * y) + (z * z));
}

inline float azimuth_from_cartesian(float x, float y)
{
    // avoid division by 0
    if (x == 0 && y == 0)  
        return 0;
    else if (x == 0 && y > 0)
        return -M_PI/2;
    else if (x == 0 && y < 0)
        return M_PI/2;
    // right hemisphere
    else if (x > 0)
        return -atan(y / x);
    // top left
    else if (x < 0 && y >= 0)
        return -(atan(y / x)+M_PI);
    // bottom left
    else if (x < 0 && y < 0)
        return (M_PI - atan(y / x));
}

inline float elevation_from_cartesian(float x, float y, float z)
{
    if (x == 0 && y == 0 && z == 0)
        return 0.0f;
    else if (z > 0)
        return atan(z / sqrt((x * x) + (y * y)));
    else
    {
        auto eins = (-atan(-z / sqrt((x * x) + (y * y))));
        return (-atan(-z / sqrt((x * x) + (y * y))));
    }    
}

inline float x_from_spherical(float radius, float elevation, float azimuth)
{
    return radius * cos(elevation) * cos(azimuth);
}

inline float y_from_spherical(float radius, float elevation, float azimuth)
{
    return radius * cos(elevation) * sin(azimuth);
}

inline float z_from_spherical(float radius, float elevation)
{
    return radius * sin(elevation);
}

inline float radian_to_degree(float radian)
{
    return radian * 180 / M_PI;
}

inline float degree_to_radian(float degree)
{
    return degree * M_PI / 180;
}
#endif TRIGONOMETRIC_HELPERS
