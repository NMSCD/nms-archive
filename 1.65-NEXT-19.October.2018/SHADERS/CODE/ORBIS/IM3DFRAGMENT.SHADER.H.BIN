////////////////////////////////////////////////////////////////////////////////
///
///     @file       Im3dFragment.shader.h
///     @author     
///     @date       
///
///     @brief      Immediate Mode Fragment
///
///     Copyright (c) 2009 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef D_FRAGMENT
#define D_FRAGMENT
#endif
#include "Common/Defines.shader.h"

struct CommonPerMeshUniforms
{
    vec4 gImmediateRenderParams;
    mat4 gWorldViewProjectionMat4;
	
BEGIN_SAMPLERBLOCK

	SAMPLER2D( gColourMap );

END_SAMPLERBLOCK

struct UniformBuffer
{
     DECLARE_PTR( CommonPerMeshUniforms, mpCommonPerMesh )       /*: PER_MESH*/
};

#ifdef D_PLATFORM_ORBIS

struct cInput
{
    vec4 ScreenPosition : S_POSITION;
    vec2 UV             : TEXCOORD0;
    vec4 Color          : COLOR;
    vec2 mDepthVal      : TEXCOORD1;
};

struct cOutput 
{ 
    vec4  mColour : S_TARGET_OUTPUT; 
    float mDepth  : S_DEPTH_OUTPUT; 
};	

DEF_SRT(UniformBuffer, lUniforms);

void main( cInput In, out cOutput Out)
//float4 main( PS_IN In, UniformBuffer lUniforms : S_SRT_DATA  ) : S_TARGET_OUTPUT 
{
    Out.mColour = In.Color;
    Out.mDepth  = log2( In.mDepthVal.x ) * In.mDepthVal.y;
}

#else

DECLARE_INPUT
INPUT_SCREEN_POSITION
INPUT(vec2, mTexCoordsVec2, TEXCOORD0)
DECLARE_END


FRAGMENT_MAIN_COLOUR_SRT
{
    FRAGMENT_COLOUR = vec4(0.0, 0.0, 0.0, 0.0);
}

#endif