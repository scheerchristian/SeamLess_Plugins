/**
#include <iostream>
#include <cmath>

using namespace std;

// ----------------------------------------------------------------------------
// cartesian to spherical

// calculate radius from cartesian coordinates
float radius_from_cartesian(float x, float y, float z)
{
    return sqrt((x * x) + (y * y) + (z * z));
}

// calculate azimuth from cartesian coordinates
float azimuth_from_cartesian(float x, float y)
{
    return atan(y / x);
}

// calculate elevation from cartesian coordinates
float elevation_from_cartesian(float x, float y, float z)
{
    return atan(z / sqrt((x * x) + (y * y)));
}

// ----------------------------------------------------------------------------
// spherical to cartesian

float x_from_spherical(float radius, float elevation, float azimuth)
{
    return radius * cos(elevation) * cos(azimuth);
}

float y_from_spherical(float radius, float elevation, float azimuth)
{
    return radius * cos(elevation) * sin(azimuth);
}

float z_from_spherical(float radius, float elevation)
{
    return radius * sin(elevation);
}

// ----------------------------------------------------------------------------
// radian to degree

float radian_to_degree(float radian)
{
    return radian * 180 / M_PI;
}

// degree to radian

float degree_to_radian(float degree)
{
    return radian_to_degree * M_PI / 180;
}
// ----------------------------------------------------------------------------

// test
int main()
{
    float x, y, z;                    // cartesian coordinates
    float radius, elevation, azimuth; // spherical coordinates

    // set cartesian coordinates in gui
    x = 4.5;
    y = 6.7;
    z = -3.3;

    r = radius_from_cartesian(x, y, z);
    a = azimuth_from_cartesian(x, y);
    e = elevation_from_cartesian(x, y, z);

    cout << "r: " << radius << ", e: " << radian_to_degree(elevation) << "° , a: " << radian_to_degree(azimuth) << "°" << endl;

    x = x_from_spherical(radius, elevation, azimuth);
    y = y_from_spherical(radius, elevation, azimuth);
    z = z_from_spherical(radius, elevation);

    cout << "x: " << x << ", y: " << y << ", z: " << z << endl;

    // set spherical coordinates in degree in gui
    radius = 4.5;
    elevation = 45 % 360; //<-- always modulo 360 for 2-pi periodicity
    azimuth = 23 % 360;

    x = x_from_spherical(radius, degree_to_radian(elevation), degree_to_radian(azimuth));
    y = y_from_spherical(radius, degree_to_radian(elevation), degree_to_radian(azimuth));
    z = z_from_spherical(radius, degree_to_radian(elevation));

    cout << "x: " << x << ", y: " << y << ", z: " << z << endl;

    radius = radius_from_cartesian(x, y, z);
    azimuth = azimuth_from_cartesian(x, y);
    elevation = elevation_from_cartesian(x, y, z);

    cout << "r: " << radius << ", e: " << radian_to_degree(elevation) << "° , a: " << radian_to_degree(azimuth) << "°" << endl;
}
**/