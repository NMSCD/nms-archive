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
    SAMPLER2D( gBufferMap );
    SAMPLER2D( gBuffer1Map );
    SAMPLER2D( gBuffer2Map );
    SAMPLER2D( gBuffer3Map );

    SAMPLER2D( gDepthMap );    
    SAMPLER2D( gCloudDepthMap );    
	SAMPLER2D( gNoiseMap );

    SAMPLER3D( gPerlin3D );
    SAMPLER3D( gDetail3D );
    SAMPLER2D( gCoverage2D );
    SAMPLER2D( gCurl2D );
	
    vec4 gCloudColourExternalVec4;
    vec4 gCloudColourInternalVec4;
    vec4 gCloudRatioVec4;
    vec4 gCloudParamsVec4;
    vec4 gHueOverlayParamsVec4;
    vec4 gSaturationOverlayParamsVec4;
    vec4 gValueOverlayParamsVec4;

    vec4 gBaseOffset;
    vec4 gDetailOffset;
    vec4 gCoverageOffset;
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
};


//
// This is the SRT buffer that everything gets uploaded to (on PS4). PC just functions as normal.
//
struct UniformBuffer
{
     DECLARE_PTR( PerFrameUniforms,             mpPerFrame )       /*: PER_MESH*/
     DECLARE_PTR( CustomPerMeshUniforms,        mpCustomPerMesh )       /*: PER_MESH*/
     DECLARE_PTR( CommonPerMeshUniforms,        mpCommonPerMesh )       /*: PER_MESH*/
};
