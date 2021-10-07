////////////////////////////////////////////////////////////////////////////////
///
///     @file       CommonDynamicVertex.h
///     @author     User
///     @date       
///
///     @brief      CommonVertex
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 

#ifndef D_COMMONDYNAMICVERTEX_H
#define D_COMMONDYNAMICVERTEX_H

#ifdef _F14_UVSCROLL
    #define D_DECLARE_TIME
    #define D_UV_DYNAMIC
#endif

#ifdef _F13_UVANIMATION
    #define D_DECLARE_TIME
    #define D_UV_DYNAMIC
#endif

#ifdef _F17_VERTEX_ROTATION
    #define D_DECLARE_TIME
    #define D_UV_DYNAMIC
#endif

#ifdef _F18_UVTILES
    #define D_UV_DYNAMIC
#endif

#ifdef _F15_WIND
	#define D_DECLARE_TIME
#endif


//-----------------------------------------------------------------------------
//      Include files

#include "Common/CommonUniforms.shader.h"

//-----------------------------------------------------------------------------
//      Global Data



//-----------------------------------------------------------------------------
//      Functions

//-----------------------------------------------------------------------------
///
///     CalcTextureCoords
///
///     @brief      CalcTextureCoords
///
///     @param      in vec4 lTextureCoordsVec4
///     @param      in vec4 lTextureScrollStepVec4
///     @param      in vec4 lWorldPosVec3
///     @param      in vec4 lLocalPosVec3
///     @param      in float lfTime
///     @return     vec4
///
//-----------------------------------------------------------------------------
vec4 
CalcDynamicTextureCoords( 
    in CustomPerMeshUniforms lCustomUniforms,
    in vec4               lTextureCoordsVec4,
    in vec4               lWorldPosVec3,
    in vec4               lLocalPosVec3,
    in float              lfTime )
{
    
    vec2 lWorldPosVec2 = lWorldPosVec3.xz ;
    vec2 lLocalPosVec2 = lLocalPosVec3.xz;
    vec4 lOutputTextureCoordsVec4;
    
    lOutputTextureCoordsVec4.xy = lTextureCoordsVec4.xy ;

#ifdef _F16_DETAILMAP
    lOutputTextureCoordsVec4.zw = lTextureCoordsVec4.zw;
#else
    lOutputTextureCoordsVec4.zw = lOutputTextureCoordsVec4.xy;
#endif

    
#ifdef D_UV_DYNAMIC
    
    vec4 lFlippedScrollingUVVec4;

#ifdef _F13_UVANIMATION
    {
        lFlippedScrollingUVVec4 = lCustomUniforms.gUVScrollStepVec4;

        float lfFrameNumber = floor( lfTime * lFlippedScrollingUVVec4.z );

        lOutputTextureCoordsVec4.x += fract( lfFrameNumber * lFlippedScrollingUVVec4.x );
        lOutputTextureCoordsVec4.y += fract( lfFrameNumber * lFlippedScrollingUVVec4.y );

        lOutputTextureCoordsVec4.zw = lOutputTextureCoordsVec4.xy;
    }
#elif defined( _F14_UVSCROLL )
    {
        lFlippedScrollingUVVec4 = lCustomUniforms.gUVScrollStepVec4;

        lOutputTextureCoordsVec4 += lFlippedScrollingUVVec4 * lfTime;
    }
#elif defined( _F18_UVTILES )
    {

#ifdef _F12_BATCHED_BILLBOARD
        lWorldPosVec2 += lLocalPosVec2 - vec2( lOutputTextureCoordsVec4.x - 0.5,  0.0 );
#endif

        // Divide the x pos down because in batched billboards this has some slight inaccuracies
        float lfPos = lWorldPosVec2.x / 8.0 + lWorldPosVec2.y * 10.0;

        lOutputTextureCoordsVec4.x /= lCustomUniforms.gUVScrollStepVec4.z;
        lOutputTextureCoordsVec4.x += floor( mod(lfPos, lCustomUniforms.gUVScrollStepVec4.z) ) / lCustomUniforms.gUVScrollStepVec4.z;

        lOutputTextureCoordsVec4.y /= lCustomUniforms.gUVScrollStepVec4.w;
        lOutputTextureCoordsVec4.y += floor( mod(lfPos, lCustomUniforms.gUVScrollStepVec4.w) ) / lCustomUniforms.gUVScrollStepVec4.w;
    }
#endif

#endif
    
    return lOutputTextureCoordsVec4;
}

#ifdef _F15_WIND

vec4 
CalcWindVec4(
    in vec4  lWorldPositionVec4,
    in mat4  lWorldMat4,
    in mat3  lWorldUpMat3,
    in float lfTime )
{
    float lfFactor;
    float lfFactor2;
    float kfPi         = 3.1415;
    vec3   lWindDirVec3 = vec3( 0.0, 0.0, 0.0 );

	lfFactor2 = (lfTime + (lWorldPositionVec4.x + lWorldPositionVec4.y + lWorldPositionVec4.z) * 5.0) * 0.01 * kfPi;

    float lfFlatX  = dot(lWorldUpMat3[0], lWorldPositionVec4.xyz);
    float lfFlatZ  = dot(lWorldUpMat3[2], lWorldPositionVec4.xyz);
    float lfHeight = dot(lWorldUpMat3[1], lWorldPositionVec4.xyz - lWorldMat4[3].xyz);

    lfFactor  = (lfTime + (lfFlatX + lfFlatZ + 11.0 ) * 0.4 ) * 0.015 * kfPi;
    lWindDirVec3.x = cos( lfFactor ) * cos( lfFactor ) * cos( lfFactor * 3.0 ) * cos( lfFactor * 5.0 ) * 0.4 + sin( lfFactor2 * 31.0 ) * 0.025;

    lfFactor  = (lfTime + (lfFlatX + lfFlatZ + 23.0 ) * 0.5 ) * 0.015 * kfPi;
    lWindDirVec3.y = cos( lfFactor ) * cos( lfFactor ) * cos( lfFactor * 3.0 ) * cos( lfFactor * 5.0 ) * 0.2 + sin( lfFactor2 * 33.0 ) * 0.025;

    lfFactor  = (lfTime + (lfFlatX + lfFlatZ + 31.0 ) * 0.4 ) * 0.015 * kfPi;
    lWindDirVec3.z = cos( lfFactor ) * cos( lfFactor ) * cos( lfFactor * 3.0 ) * cos( lfFactor * 5.0 ) * 0.4 + sin( lfFactor2 * 29.0 ) * 0.025;

    return vec4( lWindDirVec3 * clamp( lfHeight, -1.0, 1.0 ), 0.0 );    
}

#endif

#endif