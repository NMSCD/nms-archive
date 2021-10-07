////////////////////////////////////////////////////////////////////////////////
///
///     @file       LineVertex.h
///     @author     User
///     @date       
///
///     @brief      LineVertex
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////


#ifndef D_VERTEX
#define D_VERTEX
#endif

#include "Common/Defines.shader.h"
#include "Common/CommonUniforms.shader.h"
#include "Common/CommonDepth.shader.h"
#include "Common/CommonVertex.shader.h"

// just here to make the SRT structure match he fragments shdaer version. TODO - move this into a shared file.
//-----------------------------------------------------------------------------
///
///     CustomPerMeshUniforms
///
///     @brief      CustomPerMeshUniforms
///
///     Stuff that is only used for these types of meshes.
//-----------------------------------------------------------------------------
struct CustomPerMaterialUniforms
{
BEGIN_SAMPLERBLOCK
    SAMPLER2D( gDiffuseMap );
    SAMPLER2D( gNormalMap );
    SAMPLER2D( gBufferMap );
END_SAMPLERBLOCK    

//
// This is the SRT buffer that everything gets uploaded to (on PS4). PC just functions as normal.
//
struct UniformBuffer
{
     DECLARE_PTR( PerFrameUniforms,             mpPerFrame )   /*: PER_MATERIAL*/ // sematics currently crash the compiler so the parser is hardcoded to look for names.
     DECLARE_PTR( CustomPerMaterialUniforms,    mpCustomPerMaterial )       /*: PER_MESH*/
     DECLARE_PTR( CommonPerMeshUniforms,        mpCommonPerMesh )       /*: PER_MESH*/
};


STATIC_CONST float radius      = 0.2;
STATIC_CONST float invScrRatio = 1280.0 / 720.0;

DECLARE_INPUT

	INPUT( vec4, mkLocalPositionVec4, POSITION0 )
    INPUT( vec4, mkLocalNormalVec4,   TEXCOORD0 )
	INPUT( vec4, mkCustom1Vec4,       TEXCOORD1 )

DECLARE_END

DECLARE_OUTPUT

    OUTPUT_SCREEN_POSITION
    OUTPUT( vec2,   mTexCoordsVec2,     TEXCOORD0 )
    OUTPUT( vec4,   mWorldPositionVec4, TEXCOORD1 )
    OUTPUT( float,  mfVertAlpha,        TEXCOORD3 )
    OUTPUT( vec4,   mColourVec4,        TEXCOORD4 )
    
DECLARE_END

vec4
GetLinePosition(
    vec4 lScreenPositionA,
    vec4 lScreenPositionB,
    vec2 lOffset )
{
    vec2 lineDirProj;

    //  line direction in screen space (perspective division required)
    lineDirProj = radius * normalize( lScreenPositionA.xy/lScreenPositionA.ww - lScreenPositionB.xy/lScreenPositionB.ww );

    // small trick to avoid inversed line condition when points are not on the same side of Z plane
    if( sign(lScreenPositionA.w) != sign(lScreenPositionB.w) )
        lineDirProj = -lineDirProj;
        
    vec4 vMVP = lScreenPositionA;

    // offset position in screen space along line direction and orthogonal direction
    vMVP.xy += lineDirProj.xy						* lOffset.xx * vec2( 1.0, invScrRatio );
    vMVP.xy += lineDirProj.yx * vec2( 1.0, -1.0 )	* lOffset.yy * vec2( 1.0, invScrRatio );

    return vMVP;
}

VERTEX_MAIN_SRT
{    
    vec2 lUVs;
    vec2 lOffsets;
    
    vec4 laPosition      [2];
    vec4 laScreenPosition[2];

	vec4 laOffsetsUVs[8];

	laOffsetsUVs[0] = vec4(1.0,  1.0, 1.0, 0.0);
	laOffsetsUVs[1] = vec4(1.0, -1.0, 1.0, 1.0);
	laOffsetsUVs[2] = vec4(1.0, -1.0, 0.0, 0.0);
	laOffsetsUVs[3] = vec4(1.0,  1.0, 0.0, 1.0);
	laOffsetsUVs[4] = vec4(0.0,  1.0, 0.5, 0.0);
	laOffsetsUVs[5] = vec4(0.0, -1.0, 0.5, 1.0);
	laOffsetsUVs[6] = vec4(0.0, -1.0, 0.5, 0.0);
	laOffsetsUVs[7] = vec4(0.0,  1.0, 0.5, 1.0);

	vec3 lNormalVec3;

    laPosition[0] = MUL( lUniforms.mpCommonPerMesh->gWorldMat4, vec4( IN( mkLocalPositionVec4 ).xyz, 1.0 ) );
    laPosition[1] = MUL( lUniforms.mpCommonPerMesh->gWorldMat4, vec4( IN( mkLocalNormalVec4 ).xyz,   1.0 ) );

	mat4 lViewProj = lUniforms.mpCommonPerMesh->gViewProjectionMat4;

    laScreenPosition[0] = MUL( lViewProj, laPosition[0] );
    laScreenPosition[1] = MUL( lViewProj, laPosition[1] );

	float lfWidth = IN( mkLocalNormalVec4 ).w;
	vec4  lColour = IN( mkCustom1Vec4 );

	int liIdx = 2;
	if ( lfWidth > 0.0f )
	{
		// get the sign bits and use them to calculate the index
		liIdx = int( (-(sign(lColour.r) - 1.0f) * 2.0f - (sign(lColour.g) - 1.0f) - (sign(lColour.b) - 1.0f) / 2.0f) );
	}	

	lOffsets = laOffsetsUVs[liIdx].xy * lfWidth;
	lUVs     = laOffsetsUVs[liIdx].zw;	
	

    vec4 vMVP  = GetLinePosition( laScreenPosition[0], laScreenPosition[1], lOffsets );

    OUT( mfVertAlpha )          = IN( mkLocalPositionVec4 ).w;
    OUT( mTexCoordsVec2 )       = lUVs;
    OUT( mWorldPositionVec4 )   = MUL( lUniforms.mpCommonPerMesh->gWorldMat4, vec4( IN( mkLocalPositionVec4 ).xyz, 1.0) );

    OUT( mColourVec4 )          = abs( lColour ) - vec4( 1, 1, 1, 1);
    
    SCREEN_POSITION = vMVP;
}
