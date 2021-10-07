////////////////////////////////////////////////////////////////////////////////
///
///     @file       CommonGBuffer.h
///     @author     User
///     @date       
///
///     @brief      CommonLighting
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 

#ifndef D_COMMONGBUFFER_H
#define D_COMMONGBUFFER_H

//-----------------------------------------------------------------------------
//      Include files

#define D_DETAILOVERLAY       ( 1 << 0 ) 
#define D_NORECEIVESHADOW     ( 1 << 1 ) 
#define D_DISABLE_POSTPROCESS ( 1 << 2 ) 
#define D_UNLIT               ( 1 << 3 ) 
#define D_NOWATER             ( 1 << 4 )
#define D_DISABLE_AMBIENT     ( 1 << 5 ) 
#define D_EMBEDDED_SHADOW     ( 1 << 6 ) 
#define D_CLAMP_AA            ( 1 << 7 ) 
#define D_CLAMPAMBIENT        ( 1 << 7 )

#include "Common/Common.shader.h"
#include "Common/CommonDepth.shader.h"

float
EncodeUnitScalars(
    in float lfUnitScalarA,
    in float lfUnitScalarB )
{
    int liUpper = int( saturate( lfUnitScalarB ) * 255.0 ) << 8;
    int liLower = int( saturate( lfUnitScalarA ) * 255.0 );

    return float( liUpper | liLower );

    //return lfUnitScalarA;
}

void
DecodeUnitScalars(
    in  float lfEncodedFloat,
    out float lfUnitScalarA,
    out float lfUnitScalarB )
{
    lfUnitScalarA = float(   int( lfEncodedFloat )        & 0xFF ) / 255.0;
    lfUnitScalarB = float( ( int( lfEncodedFloat ) >> 8 ) & 0xFF ) / 255.0;
}

float
EncodeUnitScalars(
    in int   liUnitScalarA,
    in float lfUnitScalarB )
{
    int liUpper = int( saturate( lfUnitScalarB ) * 255.0 ) << 8;
    int liLower = liUnitScalarA & 0xFF;

    return float( liUpper | liLower );

    //return lfUnitScalarA;
}

void
DecodeUnitScalars(
    in  float lfEncodedFloat,
    out int   liUnitScalarA,
    out float lfUnitScalarB )
{
    liUnitScalarA = int( lfEncodedFloat ) & 0xFF;
    lfUnitScalarB = float( ( int( lfEncodedFloat ) >> 8 ) & 0xFF ) / 255.0;
}

// 4bit


float
Encode4BitUnitScalars(
    in float lfUnitScalarA,
    in float lfUnitScalarB )
{
    int liUpper = int( saturate( lfUnitScalarB ) * 15.0 ) << 4;
    int liLower = int( saturate( lfUnitScalarA ) * 15.0 );

    return float( liUpper | liLower ) / 255.0;

    //return lfUnitScalarA;
}

void
Decode4BitUnitScalars(
    in  float lfEncodedFloat,
    out float lfUnitScalarA,
    out float lfUnitScalarB )
{
    lfEncodedFloat *= 255.0;
    lfUnitScalarA = float( int( lfEncodedFloat )        & 0xF ) / 15.0;
    lfUnitScalarB = float( ( int( lfEncodedFloat ) >> 4 ) & 0xF ) / 15.0;
}

float
Encode4BitUnitScalars(
in int   liUnitScalarA,
in float lfUnitScalarB )
{
    int liUpper = int( saturate( lfUnitScalarB ) * 15.0 ) << 4;
    int liLower = liUnitScalarA & 0xF;

    return float( liUpper | liLower ) / 255.0;

    //return lfUnitScalarA;
}

void
Decode4BitUnitScalars(
    in  float lfEncodedFloat,
    out int   liUnitScalarA,
    out float lfUnitScalarB )
{
    lfEncodedFloat *= 255.0;
    liUnitScalarA = int( lfEncodedFloat ) & 0xF;
    lfUnitScalarB = float( ( int( lfEncodedFloat ) >> 4 ) & 0xF ) / 15.0;
}

//-----------------------------------------------------------------------------
///
///     EncodeScalarAsVector
///
//-----------------------------------------------------------------------------
vec4
EncodeScalarAsVector(
    in float lfScalar )
{
    vec4 lEncoded;

    float lfScaled = lfScalar * 2048.0;

    lEncoded = vec4( floor( lfScaled ), fract( lfScaled ), 0.0, 0.0 );
    return lEncoded;
}

//-----------------------------------------------------------------------------
///
///     EncodeScalarAsVector
///
//-----------------------------------------------------------------------------
float
DecodeScalarAsVector(
    in vec4  lEncoded )
{
    return dot( lEncoded, float2vec4( 1.0 / 2048.0 ) );
}

//-----------------------------------------------------------------------------
///
///     EncodeNormal
///
//-----------------------------------------------------------------------------
void
EncodeNormal(
    in  vec3 lNormal,
    out vec3 lBuffer )
{
    lBuffer = lNormal * 0.5 +  0.5;
}

//-----------------------------------------------------------------------------
///
///     DecodeNormal
///
//-----------------------------------------------------------------------------
vec3
DecodeNormal(
    in vec3 lBuffer )
{
    return lBuffer * 2.0 - 1.0;
}

//-----------------------------------------------------------------------------
///
///     EncodeMotion
///
//-----------------------------------------------------------------------------
vec2
EncodeMotion(
    in  vec2 lMotion )
{
    // gamma 2.0 encoding to pack a reasonable value into RG8 targets
    // corresponding decode is in CommonPostProcess to save rebuilds

    vec2 lMotionScaled = DSCREENSPACE_AS_RENDERTARGET_UVS(lMotion * 16.0);

    //float lLength = length( lMotionScaled * vec2( 9.0 / 16.0, 1.0 ) );
    float lLength = length( lMotionScaled );
    lMotionScaled /= max( 1.0, lLength );
    if( lLength > 0.0 )
    {
        // set length to sqrt( the clamped length )
        lMotionScaled /= sqrt( length( lMotionScaled ) );
    }

    // reserve 1.0 for signaling values`
    vec2 lMaxRealValue = float2vec2( 254.0 / 255.0 );

    return min( lMaxRealValue, lMotionScaled * ( 127.0 / 255.0 ) + 0.5 );
}

//-----------------------------------------------------------------------------
///
///     DecodeGBufferPosition
///
//-----------------------------------------------------------------------------
vec3
DecodeGBufferPosition(
in  vec2  lScreenPosVec2,
in  vec4  lClipPlanes,
in  mat4  lInverseProjectionMat4,
in  mat4  lInverseViewMat4,
in  vec3  lViewPositionVec3,
in  vec4  lBuffer1_Vec4,
in  bool  lbWithIVP
)
{
    float lfDepth = FastDenormaliseDepth(lClipPlanes, DecodeDepthFromColour(lBuffer1_Vec4));
    if (lbWithIVP)
    {
        // fast path with lInverseProjectionMat4 as invView(zero position) * invProj
        return RecreatePositionFromDepthWithIVP(lfDepth, lScreenPosVec2, lViewPositionVec3, lInverseProjectionMat4, lClipPlanes);
    }
    else
    {
        return RecreatePositionFromDepth(lfDepth, lScreenPosVec2, lViewPositionVec3, lInverseProjectionMat4, lInverseViewMat4);
    }
}

//-----------------------------------------------------------------------------
///
///     DecodeGBuffer
///
//-----------------------------------------------------------------------------
void
DecodeGBuffer(
    in  vec2  lScreenPosVec2,
    in  vec4  lClipPlanes,
    in  mat4  lInverseProjectionMat4,
    in  mat4  lInverseViewMat4,
    in  vec3  lViewPositionVec3,
    in  vec4  lBuffer0_Vec4,
    in  vec4  lBuffer1_Vec4,
    in  vec4  lBuffer2_Vec4,
    in  vec4  lBuffer3_Vec4,
    in  bool  lbDecodePosition,
    in  bool  lbWithIVP,
    out vec3  lColourVec3,
    inout vec3  lPositionVec3,
    out vec3  lNormalVec3,
    out int   liMaterialID,
    out float lfRoughness,
    out float lfMetallic,
    out float lfSubsurface,
    out float lfGlow )
{
    if (lbDecodePosition)
    {
        lPositionVec3 = DecodeGBufferPosition(lScreenPosVec2, lClipPlanes, lInverseProjectionMat4, lInverseViewMat4, lViewPositionVec3, lBuffer1_Vec4, lbWithIVP);
    }

    lColourVec3 = lBuffer0_Vec4.rgb;
    lfGlow = lBuffer0_Vec4.a;

    lNormalVec3       = DecodeNormal( lBuffer2_Vec4.xyz );

    liMaterialID      = int(lBuffer3_Vec4.r * 255.0 );
    lfRoughness       = ( lBuffer3_Vec4.g * lBuffer3_Vec4.g * 2.0 );
    lfMetallic        = lBuffer3_Vec4.b;
    lfSubsurface      = lBuffer3_Vec4.a;
}

//-----------------------------------------------------------------------------
///
///     EncodeGBuffer
///
//-----------------------------------------------------------------------------
void
EncodeGBuffer(
    in  vec4  lClipPlanes,
    in  vec3  lViewPositionVec3,
    in  mat4  lViewMat4,
    out vec4  lBuffer0_Vec4,
    out vec4  lBuffer1_Vec4,
    out vec4  lBuffer2_Vec4,
    out vec4  lBuffer3_Vec4,
    out vec4  lBuffer4_Vec4,
    in  vec3  lColourVec3,
    in  vec3  lPositionVec3,
    in  vec3  lNormalVec3,
    in  int   liMaterialID,
    in  float lfRoughness,
    in  float lfMetallic,
    in  float lfSubsurface,
    in  float lfGlow,
    in  vec2  lScreenSpaceMotionVec2 )
{
    lBuffer0_Vec4.xyz = lColourVec3;
    lBuffer0_Vec4.a    = saturate(lfGlow);

    EncodeNormal( lNormalVec3, lBuffer2_Vec4.xyz );
    lBuffer2_Vec4.w    = 0.0;

    float lfRoughnessEncoded = sqrt( lfRoughness * ( 1.0 / 2.0 ) );

    lBuffer3_Vec4.r    = float(liMaterialID) / 255.0;
    lBuffer3_Vec4.g    = lfRoughnessEncoded;
    lBuffer3_Vec4.b    = saturate(lfMetallic);
    lBuffer3_Vec4.a    = saturate(lfSubsurface);

    #ifdef D_OUTPUT_MOTION_VECTORS
    lBuffer1_Vec4      = vec4( EncodeMotion( lScreenSpaceMotionVec2 * 0.5 ), 0.0, 0.0 );
    #else
    lBuffer1_Vec4      = vec4( lScreenSpaceMotionVec2, 0.0, 0.0 );
    #endif
    
    lBuffer4_Vec4			 = vec4(0.0, 0.0, 0.0, 0.0);
}

#endif
































