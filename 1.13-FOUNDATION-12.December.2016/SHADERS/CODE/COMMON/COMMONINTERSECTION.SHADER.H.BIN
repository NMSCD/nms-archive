////////////////////////////////////////////////////////////////////////////////
///
///     @file       CommonIntersection.h
///     @author     hdenholm
///     @date       
///
///     @brief      Intersection with primitives
///
///     Copyright (c) 2014 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 

#ifndef D_COMMONISECT_H
#define D_COMMONISECT_H

//-----------------------------------------------------------------------------
//      Include files

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Functions



#define kIsectEpsilon 0.001

float
DistanceToSegment( vec2 a, vec2 b, vec2 p )
{
    vec2 pa = p - a;
    vec2 ba = b - a;
    float h = clamp( dot( pa, ba ) / dot( ba, ba ), 0.0, 1.0 );

    return length( pa - ba * h );
}

float
IsectSphere( vec3 rayOrigin, vec3 rayDir, vec4 sphere )
{
    float sphereRadius = sphere.w;

    vec3 oc = rayOrigin - sphere.xyz;
    float b = dot( oc, rayDir );
    float c = dot( oc, oc ) - sphereRadius * sphereRadius;
    float h = ( b * b ) - c;
    if ( h < 0.0 )
        return -1.0;

    h = sqrt( h );

    float t1 = -b - h;
    float t2 = -b + h;

    if ( t1 < kIsectEpsilon && t2 < kIsectEpsilon )
        return -1.0;

    if ( t1 < kIsectEpsilon )
        return t2;

    return t1;
}

float
sSphere( vec3 rayOrigin, vec3 rayDir, vec4 sphere )
{
    float res = 1.0;

    vec3 oc = sphere.xyz - rayOrigin;
    float b = dot( oc, rayDir );

    if ( b < 0.0 )
    {
        res = 1.0;
    }
    else
    {
        float h = sqrt( dot( oc, oc ) - b * b ) - sphere.w;
        res = clamp( 16.0 * h / b, 0.0, 1.0 );
    }
    return res;
}

vec3
IsectNormalSphere( vec3 isectPos, vec4 sphere )
{
    float sphereRadius = sphere.w;
    return ( isectPos - sphere.xyz ) / sphereRadius;
}

float
IsectPlane( vec3 rayOrigin, vec3 rayDir, vec4 planeObj )
{
    return ( -planeObj.w - dot( planeObj.xyz, rayOrigin ) ) / dot( planeObj.xyz, rayDir );
}

vec3
IsectNormalPlane( vec3 isectPos, vec4 planeObj )
{
    return planeObj.xyz;
}

#endif