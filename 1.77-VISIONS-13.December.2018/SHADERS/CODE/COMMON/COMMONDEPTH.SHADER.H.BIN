////////////////////////////////////////////////////////////////////////////////
///
///     @file       CommonDepth.h
///     @author     User
///     @date       
///
///     @brief      CommonDepth
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 

#ifndef D_COMMONDEPTH_H
#define D_COMMONDEPTH_H

//-----------------------------------------------------------------------------
//      Include files

//#include "Common/CommonUniforms.shader.h"

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Functions

float
ReverseZToLinearDepth(
in vec4  lClipPlanes,
float    lfDepth)
{
    float zNear = lClipPlanes.x;
    float zFar =  lClipPlanes.y;
    return zNear * zFar / ( zNear + lfDepth * (zFar - zNear));
}

float
ReverseZToLinearDepthNorm(
    in vec4  lClipPlanes,
    float    lfDepth)
{
    float zNear = lClipPlanes.x;
    float zFar = lClipPlanes.y;
    return zNear / (zNear + lfDepth * (zFar - zNear));
}

float
LinearToReverseZDepth(
in vec4  lClipPlanes,
float    lfDepth)
{
    float zNear = lClipPlanes.x;
    float zFar  = lClipPlanes.y;
    return ( zNear * zFar / lfDepth - zNear ) / ( zFar - zNear );
}

//-----------------------------------------------------------------------------
///
///     FastNormaliseDepth
///
///     @brief      FastNormaliseDepth
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
float
FastNormaliseDepth(
    in vec4  lClipPlanes,
	float    lfDepth )
{
	float kfRecip_Far = lClipPlanes.w;
	return (lfDepth * kfRecip_Far);
}


//-----------------------------------------------------------------------------
///
///     FastDenormaliseDepth
///
///     @brief      FastDenormaliseDepth
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
float
FastDenormaliseDepth(
in vec4  lClipPlanes,
float    lfDepth )
{
    float kfFar      = lClipPlanes.y;
    return lfDepth * kfFar;
}


//-----------------------------------------------------------------------------
///
///     EncodeDepthToColour
///
///     @brief      EncodeDepthToColour
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------

vec4
EncodeDepthToColour( 
    float lDepth )
{
	return vec4(lDepth,0.0,0.0,0.0);
}


//-----------------------------------------------------------------------------
///
///     DecodeDepthFromColour
///
///     @brief      DecodeDepthFromColour
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------

float 
DecodeDepthFromColour( 
    vec4 lColour )
{
	return lColour.x;
}


vec3
RecreatePositionFromDepthWithIVP(
    in float lfDepth,
    in vec2  lFragCoordsVec2,
    in vec3  lViewPosition,
    in mat4  lInverseViewProjectionMatrix,
    in vec4  lClipPlanes)
{
    vec4 lPositionVec4;
    lPositionVec4.xy = SCREENSPACE_AS_RENDERTARGET_UVS(lFragCoordsVec2) * 2.0 - 1.0;
    lPositionVec4.z = LinearToReverseZDepth( lClipPlanes, lfDepth );
    lPositionVec4.w = 1.0;

    lPositionVec4 = MUL(lInverseViewProjectionMatrix, lPositionVec4);
    lPositionVec4.xyz = lPositionVec4.xyz / lPositionVec4.w;
    lPositionVec4.xyz += lViewPosition;

    return lPositionVec4.xyz;
}


vec3
RecreatePositionFromDepth(
    in float lfDepth,
    in vec2  lFragCoordsVec2,
    in vec3  lViewPosition,
    in mat4  lInverseProjectionMatrix,
    in mat4  lInverseViewMatrix )
{
    vec4 lPositionVec4;
    lPositionVec4.xy = SCREENSPACE_AS_RENDERTARGET_UVS(lFragCoordsVec2) * 2.0 - 1.0;
    lPositionVec4.z = 0.0;
    lPositionVec4.w = 1.0;

    // Inverse projection
    lPositionVec4        = MUL( lInverseProjectionMatrix, lPositionVec4 );
    //lPositionVec4        = lPositionVec4 / lPositionVec4.w;
    lPositionVec4        = lPositionVec4 / abs(lPositionVec4.z);
    lPositionVec4       *= lfDepth;

    // Inverse view
    mat4 lViewMat   = lInverseViewMatrix;
    MAT4_SET_POS( lViewMat, vec4( 0.0, 0.0, 0.0, 1.0 ) );
    lPositionVec4   = MUL( lViewMat, lPositionVec4 );
    //lPositionVec4   = lPositionVec4 / lPositionVec4.w;

    //lPositionVec4.xyz   -= lViewPosition;

    lPositionVec4.xyz    = lPositionVec4.xyz + lViewPosition;

    return lPositionVec4.xyz;
}


vec4 
GetDepthColour(
    in float lfDepth )
{
    vec4 lColourVec4;
    if( lfDepth < 1.0 )
    {
        lColourVec4 = vec4( 0.0, 0.0, 1.0, 1.0 );
    }
    else if( lfDepth < 10.0 )
    {
        lColourVec4 = vec4( 1.0, 0.0, 0.0, 1.0 );
    }
    else if( lfDepth < 20.0 )
    {
        lColourVec4 = vec4( 1.0, 1.0, 0.0, 1.0 );
    }
    else if( lfDepth < 40.0 )
    {
        lColourVec4 = vec4( 0.0, 1.0, 0.0, 1.0 );
    }
    else if( lfDepth < 80.0 )
    {
        lColourVec4 = vec4( 0.0, 1.0, 1.0, 1.0 );
    }
    else if( lfDepth < 160.0 )
    {
        lColourVec4 = vec4( 1.0, 0.0, 1.0, 1.0 );
    }
    else if( lfDepth < 320.0 )
    {
        lColourVec4 = vec4( 1.0, 0.5, 0.0, 1.0 );
    }
    else if( lfDepth < 640.0 )
    {
        lColourVec4 = vec4( 0.5, 1.0, 0.0, 1.0 );
    }
    else if( lfDepth < 1280.0 )
    {
        lColourVec4 = vec4( 0.0, 1.0, 0.5, 1.0 );
    }
    else if( lfDepth < 2560.0 )
    {
        lColourVec4 = vec4( 0.5, 0.0, 1.0, 1.0 );
    }
    else
    {
        lColourVec4 = vec4( 1.0, 0.0, 0.5, 1.0 );
    }
    return lColourVec4;
}

#endif