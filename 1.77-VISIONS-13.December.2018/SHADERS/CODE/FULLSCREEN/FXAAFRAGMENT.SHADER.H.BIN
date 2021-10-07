////////////////////////////////////////////////////////////////////////////////
///
///     @file       FXAAFragment.h
///     @author     User
///     @date       
///
///     @brief      FXAAFragmentShader
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 


//-----------------------------------------------------------------------------
//      Include files

#ifndef D_FRAGMENT
#define D_FRAGMENT
#endif
#include "Common/Defines.shader.h"
#include "Common/CommonUniforms.shader.h"

// must include this after CommonUniforms
#include "Fullscreen/PostCommon.h"


#define FXAA_PC              1
#define FXAA_GLSL_130        1

#if defined( D_PLATFORM_PC_LOWEND )
#define FXAA_QUALITY__PRESET 12
#define FXAA_GATHER4_ALPHA   0
#elif defined( D_PLATFORM_ORBIS ) 
#define FXAA_QUALITY__PRESET 39
#define FXAA_GATHER4_ALPHA   1
#elif defined( D_PLATFORM_DX12 )
#define FXAA_QUALITY__PRESET 12
#define FXAA_GATHER4_ALPHA   0  // XB1:FIXME
#else
#define FXAA_QUALITY__PRESET 39
#define FXAA_GATHER4_ALPHA   1
#endif

#include "Fullscreen/Fxaa.h"

//-----------------------------------------------------------------------------
//      Global Data





//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END


//-----------------------------------------------------------------------------
//    Functions

//-----------------------------------------------------------------------------
///
///     Main
///
///     @brief      Fragment Main
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
FRAGMENT_MAIN_COLOUR_DEPTH_SRT
{
	/*
		half4 FxaaPixelShader
		// {xy} = center of pixel
		float2 pos,
		// {xy__} = upper left of pixel
		// {__zw} = lower right of pixel
		float4 posPos,
		// {rgb_} = color in linear or perceptual color space
		// {___a} = alpha output is junk value
		FxaaTex tex,
		// This must be from a constant/UNIFORM.
		// {xy} = rcpFrame not used on PC version of FXAA Console
		float2 rcpFrame,
		// This must be from a constant/UNIFORM.
		// {x___} = 2.0/screenWidthInPixels
		// {_y__} = 2.0/screenHeightInPixels
		// {__z_} = 0.5/screenWidthInPixels
		// {___w} = 0.5/screenHeightInPixels
		float4 rcpFrameOpt
	*/

    //defines for constants for optimised FXAA
    float rcpFrameX    = 1.0 * lUniforms.mpPerFrame->gFrameBufferSizeVec4.z;
    float rcpFrameY    = 1.0 * lUniforms.mpPerFrame->gFrameBufferSizeVec4.w;
    float rcpFrameOptX = 2.0 * lUniforms.mpPerFrame->gFrameBufferSizeVec4.z;
    float rcpFrameOptY = 2.0 * lUniforms.mpPerFrame->gFrameBufferSizeVec4.w;
    float rcpFrameOptZ = 0.5 * lUniforms.mpPerFrame->gFrameBufferSizeVec4.z;
    float rcpFrameOptW = 0.5 * lUniforms.mpPerFrame->gFrameBufferSizeVec4.w;


	vec2 lPositionVec2     = IN( mTexCoordsVec2 ).xy;
	vec2 lCentreOffsetVec2 = vec2( rcpFrameOptZ, rcpFrameOptW );
	
	
	vec4 lFxaaCol = FxaaPixelShader( 
		lPositionVec2, 
        SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBufferMap ),
        SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBuffer1Map ),
        vec2(rcpFrameX, rcpFrameY) );
		
	float lfAlpha = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), IN( mTexCoordsVec2 ).xy ).a;

	FRAGMENT_COLOUR = vec4( lFxaaCol.rgb, lfAlpha );

    float lfDepth = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer2Map ), IN( mTexCoordsVec2 ).xy ).x;
    
    FRAGMENT_DEPTH  = lfDepth;
}

