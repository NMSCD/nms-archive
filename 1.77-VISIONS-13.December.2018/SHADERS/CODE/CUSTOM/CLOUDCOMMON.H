////////////////////////////////////////////////////////////////////////////////
///
///     @file       CloudCommon.h
///     @author     User
///     @date       
///
///     @brief      CloudCommon
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 


//-----------------------------------------------------------------------------
//      Include files

#include "Common/Defines.shader.h"

//-----------------------------------------------------------------------------
///
///     CustomPerMeshUniforms
///
///     @brief      CustomPerMeshUniforms
///
///     Stuff that is only used for these types of meshes.
//-----------------------------------------------------------------------------
struct CustomPerMeshUniforms
{
    vec4 gCloudColourExternalVec4;
    vec4 gCloudColourInternalVec4;
    vec4 gCloudRatioVec4;
    vec4 gCloudParamsVec4;
    vec4 gHueOverlayParamsVec4;
    vec4 gSaturationOverlayParamsVec4;
    vec4 gValueOverlayParamsVec4;


    vec4 gSunRayParams;
    vec4 gSunPositionVec4;

	vec4 gWindOffset;
	/*
    vec4 gBaseOffset;
    vec4 gDetailOffset;
    vec4 gCoverageRot0;
    vec4 gCoverageRot1;
    vec4 gCoverageRot2;
	*/
    vec4 gCloudBaseColour;
    vec4 gCloudTopColour;
    vec4 gCloudHeightGradient1;
    vec4 gCloudHeightGradient2;
    vec4 gCloudHeightGradient3;

    vec4 gCoverageParamsVec4;
    vec4 gLightingParamsVec4;
    vec4 gLightScatteringParamsVec4;
    vec4 gAnimationParamsVec4;
    vec4 gModelingBaseParamsVec4;
    vec4 gModelingDetailParamsVec4;
    vec4 gOptimisationParamsVec4;
    vec4 gAtmosphereParamsVec4;
    vec4 gCloudSubFrameParamsVec4;
BEGIN_SAMPLERBLOCK    
    SAMPLER2D( gBufferMap );
    SAMPLER2D( gBuffer1Map );
    SAMPLER2D( gBuffer2Map );
    SAMPLER2D( gBuffer3Map );
    SAMPLER2D( gBuffer4Map );
    SAMPLER2D( gBuffer5Map );

    SAMPLER2D( gDepthMap );    
    SAMPLER2D( gCloudDepthMap );    
    SAMPLER2D( gNoiseMap );
    SAMPLER2D( gBlueNoiseMap );

    SAMPLER2D(gCoverage2D);
    SAMPLER2D(gCloudsHigh2D);
    SAMPLER2D(gCurl2D);

    SAMPLER3D( gPerlin3D );
    SAMPLER3D( gDetail3D );
    
END_SAMPLERBLOCK

#ifndef D_COMPUTE

#define TEX_COORDS IN(mTexCoordsVec2)

#elif defined D_PLATFORM_ORBIS
//-----------------------------------------------------------------------------
///
///     ComputeOutputUniforms
///
///     @brief      ComputeOutputUniforms
///
///     Refs to output textures for compute quad shaders
//-----------------------------------------------------------------------------

struct ComputeOutputUniforms
{
    RW_Texture2D<float4> gOutTexture0; 
    RW_Texture2D<float4> gOutTexture1;
    RW_Texture2D<float4> gOutTexture2; 
    RW_Texture2D<float4> gOutTexture3; 
    RW_Texture2D<float4> gOutTexture4; 
    RW_Texture2D<float>  gOutTextureDepth;
};

#define TEX_COORDS ( ( vec2( dispatchThreadID.xy ) + vec2(0.5,0.5) ) / vec2( GetImgResolution( lUniforms.mpCmpOutPerMesh->gOutTexture0 ) ) )

#else

RW_Texture2D<float4> gOutTexture0 : register(u0);
#ifdef D_CLOUD_RENDER
RW_Texture2D<float>  gOutTexture1 : register(u1);
#else
RW_Texture2D<float4> gOutTexture1 : register(u1);
#endif

#define TEX_COORDS ( ( vec2( dispatchThreadID.xy ) + vec2(0.5,0.5) ) / vec2( GetImgResolution( gOutTexture0 ) ) )

#endif

//
// This is the SRT buffer that everything gets uploaded to (on PS4). PC just functions as normal.
//
struct UniformBuffer
{
     DECLARE_PTR( PerFrameUniforms,             mpPerFrame )       /*: PER_MESH*/
     DECLARE_PTR( CustomPerMeshUniforms,        mpCustomPerMesh )       /*: PER_MESH*/
     DECLARE_PTR( CommonPerMeshUniforms,        mpCommonPerMesh )       /*: PER_MESH*/

#if defined D_PLATFORM_ORBIS && defined D_COMPUTE
     DECLARE_PTR( ComputeOutputUniforms,        mpCmpOutPerMesh )   /* hack - marked 'per mesh' so it'll be alloced in cmd buf */
#endif
};
