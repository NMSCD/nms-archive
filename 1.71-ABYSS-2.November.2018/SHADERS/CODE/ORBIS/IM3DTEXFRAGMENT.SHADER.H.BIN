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

struct PS_IN                                     
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

void main( PS_IN In,  out cOutput Out) 
{                                                  
    vec4 lAlbedo;

    if ( lUniforms.mpCommonPerMesh->gImmediateRenderParams.x >= 0.0f )
    {
        //lAlbedo = lUniforms.mpCommonPerMesh->gColourMap.SampleLOD( lUniforms.mpCommonPerMesh->gColourMapSS, float2( In.UV.x, 1.0 - In.UV.y ), 0 );
        lAlbedo = texture2DLod( lUniforms.mpCommonPerMesh->gColourMap, vec2( In.UV.x, 1.0 - In.UV.y ), lUniforms.mpCommonPerMesh->gImmediateRenderParams.x );
        //return vec4( 0.0, 0.5, 1.0, 1.0 );
    }
    else
    {
        //lAlbedo = lUniforms.mpCommonPerMesh->gColourMap.Sample( lUniforms.mpCommonPerMesh->gColourMapSS, float2( In.UV.x, 1.0 - In.UV.y ) );
        lAlbedo = texture2D( lUniforms.mpCommonPerMesh->gColourMap, vec2( In.UV.x, 1.0 - In.UV.y ) );
        //return vec4( 0.0, 1.0, 0.5, 1.0 );
    }

    lAlbedo *= In.Color; 

    Out.mColour = lAlbedo;
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
