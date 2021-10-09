////////////////////////////////////////////////////////////////////////////////
///
///     @file       CompressFragment.h
///     @author     User
///     @date       
///
///     @brief      Dxt5 Compression Shader
///                 compress a 4x4 block to DXT5 format
///                 integer version, renders to 4 x int32 buffer
///                 code based on 
///                 https://github.com/OpenGLInsights/OpenGLInsightsCode
///                 ( Chapter 31 
///                   In-Game Video Capture with Real-Time Texture Compression )
///
///     Copyright (c) 2015 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////




#ifndef D_FRAGMENT
#define D_FRAGMENT
#endif

#include "Common/Defines.shader.h"

#include "Common/CommonUniforms.shader.h"
#include "Common/CommonFragment.shader.h"

#if !defined( D_COMPUTECOMPRESS )
#include "Fullscreen/PostCommon.h"
#endif

#if !defined( D_PLATFORM_ORBIS ) || defined( D_COMPUTECOMPRESS )

#ifdef D_COMPUTECOMPRESS
#else
DECLARE_INPUT

    INPUT_SCREEN_POSITION

    INPUT(  vec2, mkTexCoordsVec4,     TEXCOORD0 )
DECLARE_END
#endif

#if defined( D_PLATFORM_ORBIS ) && defined( D_BC5 )
// it seems like this shader does not compile correctly under -O3 and higher
#pragma argument(optimizationlevel=2)
#endif

//-----------------------------------------------------------------------------
//      Functions 

float 
mySaturate( float lfVal ) 
{ 
    return clamp( lfVal, -1.0, 1.0 ); 
}

vec2 
mySaturate( vec2 lValVec2 ) 
{ 
    return clamp( lValVec2, vec2( -1.0, -1.0 ), vec2( 1.0, 1.0 ) ); 
}

vec3 
mySaturate( vec3 lValVec3 ) 
{ 
    return clamp( lValVec3, vec3( -1.0, -1.0, -1.0), vec3( 1.0, 1.0, 1.0) );
}

vec4 
mySaturate( vec4 lValVec4 ) 
{ 
    return clamp( lValVec4, vec4( -1.0, -1.0, -1.0, -1.0), vec4( 1.0, 1.0, 1.0, 1.0) );
}



#if defined( D_COMPUTECOMPRESS ) 
void 
ExtractColorBlockRGBA(
    out vec4 lColorVec4[ 16 ],
    Texture2D<float4> lImage,
    ivec2 lCoordIVec2,
    int liMip )
#else
void 
ExtractColorBlockRGBA(
    out vec4 lColorVec4[ 16 ],
    SAMPLER2DARG( lImage ),
    ivec2 lCoordIVec2,
    int liMip )
#endif
{
    for ( int liY = 0; liY < 4; liY++ ) 
    {
        for ( int liX = 0; liX < 4; liX++ ) 
        {
#if defined(D_PLATFORM_DX12)
            lColorVec4[ liY * 4 + liX ] = lImage.Load( int3( lCoordIVec2 + ivec2(liX, liY), liMip ) );
#else
            lColorVec4[ liY * 4 + liX ] = texelFetch( lImage, lCoordIVec2 + ivec2( liX, liY ), liMip );
#endif
        }
    }
}


//
// find minimum and maximum colors based on bounding box in color space
//
void FindMinMaxColorsBox(
    vec4 lBlockVec4[ 16 ],
    out vec4 lMinColorVec4, 
    out vec4 lMaxColorVec4,
    out bool lbIsAlphaAllSame )
{
    lMinColorVec4 = lBlockVec4[ 0 ];
    lMaxColorVec4 = lBlockVec4[ 0 ];

    lbIsAlphaAllSame = true;

    for ( int liIdx = 1; liIdx < 16; liIdx++ ) 
    {
        lMinColorVec4.rgb = min( lMinColorVec4.rgb, lBlockVec4[ liIdx ].rgb );
        lMaxColorVec4.rgb = max( lMaxColorVec4.rgb, lBlockVec4[ liIdx ].rgb );

        #ifdef D_HEIGHTFIELD_MODE
        if( lMinColorVec4.a == 0.0 )
        {
            lMinColorVec4.a = lBlockVec4[ liIdx ].a;
            lMaxColorVec4.a = lBlockVec4[ liIdx ].a;
        }
        else if( lBlockVec4[ liIdx ].a != 0.0 )
        {
            lMinColorVec4.a = min( lMinColorVec4.a, lBlockVec4[ liIdx ].a );
            lMaxColorVec4.a = max( lMaxColorVec4.a, lBlockVec4[ liIdx ].a );
        }
        #else
        lMinColorVec4.a = min( lMinColorVec4.a, lBlockVec4[ liIdx ].a );
        lMaxColorVec4.a = max( lMaxColorVec4.a, lBlockVec4[ liIdx ].a );
        #endif

        lbIsAlphaAllSame = lbIsAlphaAllSame && ( lBlockVec4[ liIdx ].a == lBlockVec4[ 0 ].a );
    }
}

void SelectRGBDiagonal(
    vec4 lBlockVec4[ 16 ], 
    inout vec3 lMinColorVec3, 
    inout vec3 lMaxColorVec3 )
{
    vec3 lCenterVec3 = ( lMinColorVec3 + lMaxColorVec3 ) * 0.5;

    vec2 lCovVec2 = vec2( 0.0, 0.0 );
    for ( int liIdx = 0; liIdx < 16; liIdx++ )
    {
        vec3 lTVec3 = lBlockVec4[ liIdx ].rgb - lCenterVec3;
        lCovVec2.x += lTVec3.x * lTVec3.z;
        lCovVec2.y += lTVec3.y * lTVec3.z;
    }

    if ( lCovVec2.x < 0 ) 
    {
        float lfTemp    = lMaxColorVec3.x;
        lMaxColorVec3.x = lMinColorVec3.x;
        lMinColorVec3.x = lfTemp;
    }
    if ( lCovVec2.y < 0 ) 
    {
        float lfTemp    = lMaxColorVec3.y;
        lMaxColorVec3.y = lMinColorVec3.y;
        lMinColorVec3.y = lfTemp;
    }
}


uint EmitEndPointsAlpha(
    inout float lfMinColor, 
    inout float lfMaxColor )
{
    float lfInset = ( lfMaxColor - lfMinColor ) / 32.0 - ( 16.0 / 255.0 ) / 32.0;
    lfMinColor    = mySaturate( lfMinColor + lfInset );
    lfMaxColor    = mySaturate( lfMaxColor - lfInset );

    uint luC0     = uint( round( lfMinColor * 255.0 ) );
    uint luC1     = uint( round( lfMaxColor * 255.0 ) );

    return ( ( luC0 << 8u ) | luC1 );
}


uvec2 EmitIndicesAlpha(
    vec4 lBlockVec4[ 16 ], 
    float lfMinAlpha, 
    float lfMaxAlpha )
{
    const int ALPHA_RANGE = 7;

    float lfBias  = lfMaxAlpha + ( lfMaxAlpha - lfMinAlpha ) / ( 2.0 * ALPHA_RANGE );
    float lfScale = 1.0 / ( lfMaxAlpha - lfMinAlpha );

    uvec2 lIndicesUVec2 = uvec2( 0u, 0u );
    int liIdx;
    for ( liIdx = 0; liIdx < 6; liIdx++ )
    {
        uint luIndex = uint( mySaturate( ( lfBias - lBlockVec4[ liIdx ].a ) * lfScale ) * ALPHA_RANGE );
        lIndicesUVec2.x |= luIndex << uint( 3 * liIdx );
    }

    for ( liIdx = 6; liIdx < 16; liIdx++ )
    {
        uint luIndex = uint( mySaturate( ( lfBias - lBlockVec4[ liIdx ].a ) * lfScale ) * ALPHA_RANGE );
        lIndicesUVec2.y |= luIndex << uint( 3 * liIdx - 18 );
    }

    uvec2 lIdx0UVec2 = ( lIndicesUVec2 >> 0u ) & 0x09249249u;
    uvec2 lIdx1UVec2 = ( lIndicesUVec2 >> 1u ) & 0x09249249u;
    uvec2 lIdx2UVec2 = ( lIndicesUVec2 >> 2u ) & 0x09249249u;

    lIdx2UVec2 ^= lIdx0UVec2 & lIdx1UVec2;
    lIdx1UVec2 ^= lIdx0UVec2;
    lIdx0UVec2 ^= ( lIdx1UVec2 | lIdx2UVec2 );

    lIndicesUVec2.x = ( lIdx2UVec2.x << 2u ) | ( lIdx1UVec2.x << 1u ) | lIdx0UVec2.x;
    lIndicesUVec2.y = ( ( ( lIdx2UVec2.y << 2u ) | ( lIdx1UVec2.y << 1u ) | lIdx0UVec2.y ) << 2u ) | ( lIndicesUVec2.x >> 16u );
    lIndicesUVec2.x <<= 16u;

    return lIndicesUVec2;
}


uint EmitIndicesBC75Alpha(
    vec4 lBlockVec4[ 16 ], 
    inout float lfMinAlpha, 
    inout float lfMaxAlpha )
{
    const int ALPHA_RANGE = 3;

    float lfScale = 1.0 / ( lfMaxAlpha - lfMinAlpha );

    uint lIndices = ( 0u );

    for ( int liIdx = 0; liIdx < 16; liIdx++ )
    {
        uint luIndex = uint( saturate( ( lBlockVec4[ liIdx ].a - lfMinAlpha ) * lfScale ) * (ALPHA_RANGE + 0.999) );

        if( liIdx == 0 )
        {
            if( luIndex >= 2 )
            {
                float tmp = lfMinAlpha;
                lfMinAlpha = lfMaxAlpha;
                lfMaxAlpha = tmp;
                lfScale = -lfScale;
                luIndex = ALPHA_RANGE - luIndex;
            }

            lIndices = ( luIndex & 1 );
        }
        else
        {
            lIndices |= luIndex << ( uint( 2 * liIdx ) - 1 );
        }
    }

    return lIndices;
}

void EmitIndicesBC5_r(
    in vec4 lBlockVec4[ 16 ], 
    in float lfMinValue, 
    in float lfMaxValue,
    inout uvec4 lCompressedValueUVec4 )
{
    const int ALPHA_RANGE = 7;

    float lfScale = 1.0 / ( lfMaxValue - lfMinValue );
    uint laInds[16];

    for ( int liIdx = 0; liIdx < 16; liIdx++ )
    {
        uint luIndex = uint( saturate( ( lBlockVec4[ liIdx ].r - lfMinValue ) * lfScale ) * (ALPHA_RANGE + 0.999) );

        if( luIndex >= 7 )
            luIndex = 1;
        else if( luIndex > 0 )
            luIndex++;

        laInds[liIdx] = luIndex;
    }

    lCompressedValueUVec4.x |= laInds[0] << 16;
    lCompressedValueUVec4.x |= laInds[1] << 19;
    lCompressedValueUVec4.x |= laInds[2] << 22;
    lCompressedValueUVec4.x |= laInds[3] << 25;
    lCompressedValueUVec4.x |= laInds[4] << 28;

    lCompressedValueUVec4.x |= (laInds[5] & 1) << 31;
    lCompressedValueUVec4.y = (laInds[5] & 6) >> 1;

    lCompressedValueUVec4.y |= laInds[6 ] << 2;
    lCompressedValueUVec4.y |= laInds[7 ] << 5;
    lCompressedValueUVec4.y |= laInds[8 ] << 8;
    lCompressedValueUVec4.y |= laInds[9 ] << 11;
    lCompressedValueUVec4.y |= laInds[10] << 14;
    lCompressedValueUVec4.y |= laInds[11] << 17;
    lCompressedValueUVec4.y |= laInds[12] << 20;
    lCompressedValueUVec4.y |= laInds[13] << 23;
    lCompressedValueUVec4.y |= laInds[14] << 26;
    lCompressedValueUVec4.y |= laInds[15] << 29;
}

void EmitIndicesBC5_g(
    in vec4 lBlockVec4[ 16 ], 
    in float lfMinValue, 
    in float lfMaxValue,
    inout uvec4 lCompressedValueUVec4 )
{
    const int ALPHA_RANGE = 7;

    float lfScale = 1.0 / ( lfMaxValue - lfMinValue );
    uint laInds[16];

    for ( int liIdx = 0; liIdx < 16; liIdx++ )
    {
        uint luIndex = uint( saturate( ( lBlockVec4[ liIdx ].g - lfMinValue ) * lfScale ) * (ALPHA_RANGE + 0.999) );

        if( luIndex >= 7 )
            luIndex = 1;
        else if( luIndex > 0 )
            luIndex++;

        laInds[liIdx] = luIndex;
    }

    lCompressedValueUVec4.z |= laInds[0] << 16;
    lCompressedValueUVec4.z |= laInds[1] << 19;
    lCompressedValueUVec4.z |= laInds[2] << 22;
    lCompressedValueUVec4.z |= laInds[3] << 25;
    lCompressedValueUVec4.z |= laInds[4] << 28;

    lCompressedValueUVec4.z |= (laInds[5] & 1) << 31;
    lCompressedValueUVec4.w = (laInds[5] & 6) >> 1;

    lCompressedValueUVec4.w |= laInds[6 ] << 2;
    lCompressedValueUVec4.w |= laInds[7 ] << 5;
    lCompressedValueUVec4.w |= laInds[8 ] << 8;
    lCompressedValueUVec4.w |= laInds[9 ] << 11;
    lCompressedValueUVec4.w |= laInds[10] << 14;
    lCompressedValueUVec4.w |= laInds[11] << 17;
    lCompressedValueUVec4.w |= laInds[12] << 20;
    lCompressedValueUVec4.w |= laInds[13] << 23;
    lCompressedValueUVec4.w |= laInds[14] << 26;
    lCompressedValueUVec4.w |= laInds[15] << 29;
}


vec3 RoundAndExpand(
    vec3 lValVec3, 
    out uint luW )
{
    ivec3 lCVec3 = ivec3( round( lValVec3 * vec3( 31, 63, 31 ) ) );
    luW = uint( ( lCVec3.r << 11 ) | ( lCVec3.g << 5 ) | lCVec3.b );

    lCVec3.rb = ( lCVec3.rb << 3 ) | ( lCVec3.rb >> 2 );
    lCVec3.g  = ( lCVec3.g  << 2 ) | ( lCVec3.g  >> 4 );

    return vec3( lCVec3 * ( 1.0 / 255.0 ) );
}

float RoundAndExpand7(
    float lVal, 
    out uint luW )
{
    luW = uint( round( lVal * 127 ) );

    return ( float( luW ) * ( 1.0 / 127.0 ) );
}

float RoundAndExpand8(
    float lVal, 
    out uint luW )
{
    luW = uint( round( lVal * 255 ) );

    return ( float( luW ) * ( 1.0 / 255.0 ) );
}

uint EmitEndPointsDXT1(
    inout vec4 lMinColorVec4, 
    inout vec4 lMaxColorVec4 )
{
    vec4 lInsetVec4 = ( lMaxColorVec4 - lMinColorVec4 ) / 16.0 - ( 8.0 / 255.0 ) / 16.0;
    lMinColorVec4   = mySaturate( lMinColorVec4 + lInsetVec4 );
    lMaxColorVec4   = mySaturate( lMaxColorVec4 - lInsetVec4 );

    uvec2 lOutValUVec2;
    lMaxColorVec4.rgb = RoundAndExpand( lMaxColorVec4.rgb, lOutValUVec2.x );
    lMinColorVec4.rgb = RoundAndExpand( lMinColorVec4.rgb, lOutValUVec2.y );

    // We have to do this in case we select an alternate diagonal.
    if ( lOutValUVec2.x < lOutValUVec2.y )
    {
        vec3 lTmpVec3     = lMinColorVec4.rgb;
        lMinColorVec4.rgb = lMaxColorVec4.rgb;
        lMaxColorVec4.rgb = lTmpVec3;
        return lOutValUVec2.y | ( lOutValUVec2.x << 16u );
    }

    return lOutValUVec2.x | ( lOutValUVec2.y << 16u );
}

uvec4 EmitEndPointsBC5(
    inout vec4 lMinColorVec4, 
    inout vec4 lMaxColorVec4 )
{
    uint r0, r1, g0, g1;
    lMinColorVec4.g = RoundAndExpand8(lMinColorVec4.g, g0);
    lMinColorVec4.r = RoundAndExpand8(lMinColorVec4.r, r0);

    lMaxColorVec4.g = RoundAndExpand8(lMaxColorVec4.g, g1);
    lMaxColorVec4.r = RoundAndExpand8(lMaxColorVec4.r, r1);

    uvec4 lOutVal;
    lOutVal.x = r1 | (r0 << 8);
    lOutVal.y = 0;
    lOutVal.z = g1 | (g0 << 8);
    lOutVal.w = 0;

    return lOutVal;
}

uvec4 EmitEndPointsBC75(
    inout vec4 lMinColorVec4, 
    inout vec4 lMaxColorVec4 )
{
    uint r0, r1, g0, g1, b0, b1, a0, a1;
    lMinColorVec4.r = RoundAndExpand7(lMinColorVec4.r, r0);
    lMinColorVec4.g = RoundAndExpand7(lMinColorVec4.g, g0);
    lMinColorVec4.b = RoundAndExpand7(lMinColorVec4.b, b0);
    lMinColorVec4.a = RoundAndExpand8(lMinColorVec4.a, a0);

    lMaxColorVec4.r = RoundAndExpand7(lMaxColorVec4.r, r1);
    lMaxColorVec4.g = RoundAndExpand7(lMaxColorVec4.g, g1);
    lMaxColorVec4.b = RoundAndExpand7(lMaxColorVec4.b, b1);
    lMaxColorVec4.a = RoundAndExpand8(lMaxColorVec4.a, a1);

    uvec4 lOutVal;
    lOutVal.x = 32 | (r0 << 8) | (r1 << 15) | (g0 << 22) | (g1 << 29);
    lOutVal.y = (g1 >> 3) | (b0 << 4) | (b1 << 11) | (a0 << 18) | (a1 << 26);
    lOutVal.z = a1 >> 6;

    return lOutVal;
}

void EmitEndPointsBC76(
    inout vec4 lMinColorVec4, 
    inout vec4 lMaxColorVec4,
    inout uvec4 lCompressedValueUVec4 )
{
    uint r0, r1, g0, g1, b0, b1, a0, a1;
    lMinColorVec4.r = RoundAndExpand7(lMinColorVec4.r, r0);
    lMinColorVec4.g = RoundAndExpand7(lMinColorVec4.g, g0);
    lMinColorVec4.b = RoundAndExpand7(lMinColorVec4.b, b0);
    lMinColorVec4.a = RoundAndExpand8(lMinColorVec4.a, a0);

    lMaxColorVec4.r = RoundAndExpand7(lMaxColorVec4.r, r1);
    lMaxColorVec4.g = RoundAndExpand7(lMaxColorVec4.g, g1);
    lMaxColorVec4.b = RoundAndExpand7(lMaxColorVec4.b, b1);
    lMinColorVec4.a = RoundAndExpand8(lMinColorVec4.a, a1);

    uint p0 = a0 & 1;
    uint p1 = a1 & 1;

    a0 = a0 >> 1;
    a1 = a1 >> 1;

    lCompressedValueUVec4.x = 64 | (r0 << 7) | (r1 << 14) | (g0 << 21) | (g1 << 28);
    lCompressedValueUVec4.y = (g1 >> 4) | (b0 << 3) | (b1 << 10) | (a0 << 17) | (a1 << 24) | (p0 << 31);
    lCompressedValueUVec4.z |= p1;
}

uint EmitIndicesDXT1_noalpha(
    vec4 lBlockVec4[ 16 ], 
    vec4 lMinColorVec4, 
    vec4 lMaxColorVec4 )
{
    const float RGB_RANGE = 3;  //divisions of min-max range

    vec3 lDirVec3 = ( lMaxColorVec4.rgb - lMinColorVec4.rgb );
    vec3 lOriginVec3 = lMaxColorVec4.rgb + lDirVec3.rgb / ( 2.0 * RGB_RANGE );   //half the division above max
    lDirVec3 /= dot( lDirVec3, lDirVec3 );

    // Compute indices
    uint luIndices = 0u;
    for ( int liIdx = 0; liIdx < 16; liIdx++ )
    {
        uint luIndex = uint( mySaturate( dot( lOriginVec3 - lBlockVec4[ liIdx ].rgb, lDirVec3 ) ) * RGB_RANGE );
        luIndices |= luIndex << uint( liIdx * 2 );
    }

    uint luI0 = ( luIndices & 0x55555555u );
    uint luI1 = ( luIndices & 0xAAAAAAAAu ) >> 1u;
    luIndices = ( ( luI0 ^ luI1 ) << 1u ) | luI1;

    // Output indices
    return luIndices;
}


uint EmitIndicesBC75_noalpha(
    vec4 lBlockVec4[ 16 ], 
    inout vec4 lMinColorVec4, 
    inout vec4 lMaxColorVec4 )
{
    const int RGB_RANGE = 3;  //divisions of min-max range

    vec3 lDirVec3 = ( lMaxColorVec4.rgb - lMinColorVec4.rgb );
    lDirVec3 /= dot( lDirVec3, lDirVec3 );

    // Compute indices
    uint luIndices = 0u;
    for ( int liIdx = 0; liIdx < 16; liIdx++ )
    {
        uint luIndex = uint( saturate( dot( lBlockVec4[ liIdx ].rgb - lMinColorVec4.xyz, lDirVec3 ) ) * (RGB_RANGE + 0.999) );
        if( liIdx == 0 )
        {
            if( luIndex >= 2 )
            {
                vec3 tmp = lMinColorVec4.xyz;
                lMinColorVec4.xyz = lMaxColorVec4.xyz;
                lMaxColorVec4.xyz = tmp;
                lDirVec3 = -lDirVec3;
                luIndex = RGB_RANGE - luIndex;
            }
            luIndices = ( luIndex & 1 );
        }
        else
        {
            luIndices |= luIndex << ( uint( 2 * liIdx ) - 1 );
        }
    }

    // Output indices
    return luIndices;
}


uvec4 EmitIndicesBC76(
    vec4 lBlockVec4[ 16 ], 
    inout vec4 lMinColorVec4, 
    inout vec4 lMaxColorVec4 )
{
    const int RGB_RANGE = 15;  //divisions of min-max range

    vec3 lDirVec3 = ( lMaxColorVec4.rgb - lMinColorVec4.rgb );
    lDirVec3 /= dot( lDirVec3, lDirVec3 );

    // Compute indices
    uvec4 luIndices = uvec4( 0u, 0u, 0u, 0u);
    for ( int liIdx = 0; liIdx < 16; liIdx++ )
    {
        uint luIndex = uint( saturate( dot( lBlockVec4[ liIdx ].rgb - lMinColorVec4.xyz, lDirVec3 ) ) * (RGB_RANGE + 0.999) );
        if( liIdx == 0 )
        {
            if( luIndex >= 8 )
            {
                vec3 tmp = lMinColorVec4.xyz;
                lMinColorVec4.xyz = lMaxColorVec4.xyz;
                lMaxColorVec4.xyz = tmp;
                lDirVec3 = -lDirVec3;
                luIndex = RGB_RANGE - luIndex;
            }
            luIndices.z = ( luIndex & 7 ) << 1;
        }
        else
        {
            if( liIdx < 8 )
                luIndices.z |= luIndex << ( uint( 4 * liIdx ) );
            else
                luIndices.w |= luIndex << ( uint( 4 * ( liIdx - 8 ) ) );
        }
    }

    // Output indices
    return luIndices;
}

//
// DXT1 with alpha
//
uint EmitIndicesDXT1_alpha(
    vec4 lBlockVec4[ 16 ], 
    vec4 lMinColorVec4, 
    vec4 lMaxColorVec4 )
{
    const float RGB_RANGE = 2;  //divisions of min-max range

    vec3 lDirVec3    = ( lMaxColorVec4.rgb - lMinColorVec4.rgb );
    vec3 lOriginVec3 = lMaxColorVec4.rgb + lDirVec3.rgb / ( 2.0 * RGB_RANGE );   //half the division above max
    lDirVec3 /= dot( lDirVec3, lDirVec3 );

    // Compute indices
    uint luIndices = 0u;
    for ( int i = 0; i < 16; i++ )
    {
        uint luIndex = ( lBlockVec4[ i ].a < 0.5 )
            ? 3u
            : min( 2u, uint( mySaturate( dot( lOriginVec3 - lBlockVec4[ i ].rgb, lDirVec3 ) ) * RGB_RANGE ) );
        luIndices |= luIndex << uint( i * 2 );
    }

    uint luI0 = ( ( luIndices & 0x55555555u ) << 1u );
    uint luI1 = ( ( luIndices & 0xAAAAAAAAu ) >> 1u );
    luIndices = luI0 | luI1;

    // Output indices
    return luIndices;
}

#ifdef D_COMPUTECOMPRESS

struct GlobalUniforms
{
    uvec4 mMipParams;
    uvec4 mLocParams;

BEGIN_SAMPLERBLOCK

    Texture2D<float4> mInTexture;

    #ifdef D_DXT1
    RWIMAGE2D(uvec2, mOutTexture);
    #else
    RWIMAGE2D(uvec4, mOutTexture);
    #endif
    //float4 mTextureSizeMipLevelVec4;
END_SAMPLERBLOCK

struct UniformBuffer
{
    DECLARE_PTR( GlobalUniforms, mpGlobalUniforms )
};

#else

#if defined( D_DXT1 ) && defined(D_PLATFORM_DX12)
#undef FRAGMENT_MAIN_UICOLOUR_SRT
#define FRAGMENT_MAIN_UICOLOUR_SRT			UNIFORMS_CB struct cOutput { uint2 mColour : S_TARGET_OUTPUT; };	                              ROOT_SIG(PS)              void main(cInput In, out cOutput Out UNIFORMS_SRT)
#endif


#endif


FRAGMENT_MAIN_UICOLOUR_SRT
{
    // read block
    vec4 lBlockVec4[ 16 ] ;

    #ifdef D_COMPUTECOMPRESS

    uvec2 lSrcDispatchOffset = dispatchThreadID.xy * 4 + lUniforms.mpGlobalUniforms->mLocParams.xy;
    uvec2 lDstDispatchOffset = dispatchThreadID.xy + lUniforms.mpGlobalUniforms->mLocParams.zw;

    //int liMipLevel = int( lUniforms.mpGlobalUniforms->mTextureSizeMipLevelVec4.z + 0.5 );
    int liMipLevel = lUniforms.mpGlobalUniforms->mMipParams.x;
    ivec2 lTexCoordIVec2 = ivec2( lSrcDispatchOffset ) & ivec2( ~3, ~3 );
    ExtractColorBlockRGBA( lBlockVec4, SAMPLER_GETMAP( lUniforms.mpGlobalUniforms, mInTexture ), lTexCoordIVec2, liMipLevel );

    #else

    int liMipLevel = int( lUniforms.mpCustomPerMesh->gTextureSizeMipLevelVec4.z + 0.5 );
#if defined( D_PLATFORM_DX12 )
    #ifdef D_COMPUTE
    uvec2 lSrcDispatchOffset = dispatchThreadID.xy + lUniforms.mpGlobalUniforms->mLocParams.zw;
    uvec2 lDstDispatchOffset = dispatchThreadID.xy + lUniforms.mpGlobalUniforms->mLocParams.zw;
    ivec2 lTexCoordIVec2 = ivec2( lSrcDispatchOffset ) & ivec2(~3, ~3);
    #else
    ivec2 lTexCoordIVec2 = ivec2(4 * ivec2( IN(mScreenPositionVec4).xy )) & ivec2(~3, ~3);
    lTexCoordIVec2 += ivec2( lUniforms.mpCustomPerMesh->gTextureSizeMipLevelVec4.xy );
    #endif
#else
    ivec2 lTexCoordIVec2 = ivec2( 4 * gl_FragCoord.xy ) & ivec2( ~3 );
    lTexCoordIVec2 += ivec2( lUniforms.mpCustomPerMesh->gTextureSizeMipLevelVec4.xy );
#endif
    ExtractColorBlockRGBA( lBlockVec4, SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh, gBufferMap ), lTexCoordIVec2, liMipLevel );

    #endif


    // find min and max colors
    vec4 lMinColorVec4, lMaxColorVec4;
    bool lbIsAlphaAllSame;
    FindMinMaxColorsBox( lBlockVec4, lMinColorVec4, lMaxColorVec4, lbIsAlphaAllSame );

    uvec4 lCompressedValueUVec4;

    #ifndef D_BC5
    SelectRGBDiagonal( lBlockVec4, lMinColorVec4.rgb, lMaxColorVec4.rgb );
    #endif

    #ifdef D_BC5

    lCompressedValueUVec4 = EmitEndPointsBC5( lMinColorVec4, lMaxColorVec4 );

    EmitIndicesBC5_r( lBlockVec4, lMaxColorVec4.r, lMinColorVec4.r, lCompressedValueUVec4 );
    EmitIndicesBC5_g( lBlockVec4, lMaxColorVec4.g, lMinColorVec4.g, lCompressedValueUVec4 );


    #elif defined( D_BC7 )

    if( lbIsAlphaAllSame )
    {
        lCompressedValueUVec4 = EmitIndicesBC76( lBlockVec4, lMinColorVec4, lMaxColorVec4 );
        EmitEndPointsBC76( lMinColorVec4, lMaxColorVec4, lCompressedValueUVec4 );
    }
    else
    {
        uint colorInds = EmitIndicesBC75_noalpha( lBlockVec4, lMinColorVec4, lMaxColorVec4 );
        uint alphaInds = EmitIndicesBC75Alpha( lBlockVec4, lMinColorVec4.a, lMaxColorVec4.a );

        lCompressedValueUVec4 = EmitEndPointsBC75( lMinColorVec4, lMaxColorVec4 );

        lCompressedValueUVec4.z = lCompressedValueUVec4.z | (colorInds << 2);
        lCompressedValueUVec4.w = (colorInds >> 30) | (alphaInds << 1);
    }

    #else

    // Output RGB in DXT1 block.
    lCompressedValueUVec4.z = EmitEndPointsDXT1( lMinColorVec4, lMaxColorVec4 );
    lCompressedValueUVec4.w = EmitIndicesDXT1_noalpha( lBlockVec4, lMinColorVec4, lMaxColorVec4 );


    #ifndef D_DXT1
    // Output Y in DXT5 alpha block.
    lCompressedValueUVec4.x = EmitEndPointsAlpha( lMinColorVec4.a, lMaxColorVec4.a );

    uvec2 lIndicesUVec2 = EmitIndicesAlpha( lBlockVec4, lMinColorVec4.a, lMaxColorVec4.a );
    lCompressedValueUVec4.x |= lIndicesUVec2.x;
    lCompressedValueUVec4.y = lIndicesUVec2.y;
    #endif

    #endif

    #ifdef D_DXT1
    lCompressedValueUVec4 = lCompressedValueUVec4.zwzw;
    #endif

    #ifdef D_PLATFORM_ORBIS
    lUniforms.mpGlobalUniforms->mOutTexture.MipMaps(lUniforms.mpGlobalUniforms->mMipParams.y, lDstDispatchOffset) = lCompressedValueUVec4;
    #elif defined( D_COMPUTECOMPRESS )
    mOutTexture[lDstDispatchOffset] = lCompressedValueUVec4;
    #else
    FRAGMENT_COLOUR_UVEC4 = lCompressedValueUVec4;
    #endif
}
#else


DECLARE_INPUT
    INPUT_SCREEN_POSITION
    INPUT(  vec2, mTexCoordsVec2,      TEXCOORD0 )
DECLARE_END



FRAGMENT_MAIN_COLOUR_SRT
{
    FRAGMENT_COLOUR = vec4( 0.0, 1.0, 0.0, 0.0 );
}

#endif