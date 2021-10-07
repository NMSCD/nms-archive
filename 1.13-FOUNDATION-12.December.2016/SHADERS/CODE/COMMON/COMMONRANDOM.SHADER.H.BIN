////////////////////////////////////////////////////////////////////////////////
///
///     @file       CommonRandom.h
///     @author     hdenholm
///     @date       
///
///     @brief      Various hashing functions to generate random values 
///
///     Copyright (c) 2014 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 

#ifndef D_COMMONRANDOM_H
#define D_COMMONRANDOM_H

//-----------------------------------------------------------------------------
//      Include files

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Functions

float Hash1to1( float lfVal )
{
    return fract(sin(lfVal)*43758.5453123);
}

//-----------------------------------------------------------------------------
vec2 Hash1to2( float lfVal )
{
    return fract(sin(vec2(lfVal,lfVal+1.0))*vec2(43758.5453123,22578.1459123));
}

//-----------------------------------------------------------------------------
vec3 Hash1to3( float lfVal )
{
    return fract(sin(vec3(lfVal,lfVal+1.0,lfVal+2.0))*vec3(43758.5453123,22578.1459123,19642.3490423));
}

//-----------------------------------------------------------------------------
float Hash2to1( vec2 lVec2 )
{
    return fract(sin(dot(lVec2,vec2(1.0,113.0)))*43758.5453123);
}

//-----------------------------------------------------------------------------
vec3 Hash2to3( vec2 lVec2 )
{
    vec3 q = vec3( dot(lVec2,vec2(127.1,311.7)), 
                   dot(lVec2,vec2(269.5,183.3)), 
                   dot(lVec2,vec2(419.2,371.9)));
    return fract(sin(q)*43758.5453);
}

//-----------------------------------------------------------------------------
float Random1( float lfVal )
{
    return fract(sin(lfVal * 123.456) * 23.4567 + sin(lfVal * 345.678) * 45.6789 + sin(lfVal * 456.789) * 56.789);
}

//-----------------------------------------------------------------------------
float Dither( float c, vec2 seed ) 
{
  return c + Hash2to3( seed ).r / 255.0;
}


//-----------------------------------------------------------------------------
float iqNoise3( vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);

    f = f*f*(3.0-2.0*f);

    float n = p.x + p.y*57.0 + 113.0*p.z;

    float res = mix( mix( mix( Hash1to1(n+  0.0), Hash1to1(n+  1.0),f.x),
                          mix( Hash1to1(n+ 57.0), Hash1to1(n+ 58.0),f.x),f.y),
                     mix( mix( Hash1to1(n+113.0), Hash1to1(n+114.0),f.x),
                          mix( Hash1to1(n+170.0), Hash1to1(n+171.0),f.x),f.y),f.z);
    return res;
}

//-----------------------------------------------------------------------------
// http://www.iquilezles.org/www/articles/voronoise/voronoise.htm
//
//  u=0, v=0: Cell Noise
//  u=0, v=1: Noise
//  u=1, v=0: Voronoi
//  u=1, v=1: Voronoise
//
float iqVoronoise( vec2 x, float u, float v )
{
    vec2 p = floor( x );
    vec2 f = fract( x );

    float k = 1.0 + 63.0 * pow( 1.0 - v, 4.0 );
    float va = 0.0;
    float wt = 0.0;

    for ( int j = -2; j <= 2; j++ )
    {
        for ( int i = -2; i <= 2; i++ )
        {
            vec2 g = vec2( float( i ), float( j ) );
            vec3 o = Hash2to3( p + g ) * vec3( u, u, 1.0 );
            vec2 r = g - f + o.xy;
            float d = dot( r, r );
            float w = pow( 1.0 - smoothstep( 0.0, 1.414, sqrt( d ) ), k );
            va += w * o.z;
            wt += w;
        }
    }

    return va / wt;
}

float iqVoronoiseFbm( vec2 x, float jitter, float interp )
{
  float f;

  f  = 0.5000  * iqVoronoise( x, jitter, interp ); x = x * 2.01;
  f += 0.2500  * iqVoronoise( x, jitter, interp ); x = x * 2.03;
  f += 0.1250  * iqVoronoise( x, jitter, interp ); x = x * 2.02;
  f += 0.0625  * iqVoronoise( x, jitter, interp ); 

  return f;
}


#endif