////////////////////////////////////////////////////////////////////////////////
///
///     @file       PostCommon.h
///     @author     User
///     @date       
///
///     @brief      PostCommon
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
    vec4 gDoFParamsVec4;
    vec4 gHDRParamsVec4;
    vec4 gHBAOParamsVec4;
    vec4 gThresholdParamsVec4;
    vec4 gBlurParamsVec4;

    SAMPLER2D( gBufferMap );
    SAMPLER2D( gBuffer1Map );
    SAMPLER2D( gBuffer2Map );
    SAMPLER2D( gBuffer3Map );
    SAMPLER2D( gBuffer4Map );
    SAMPLER2D( gBuffer5Map );
#if defined( D_DEBUG_QUAD )
    SAMPLER2D( gBuffer6Map );
    SAMPLER2D( gBuffer7Map );
    SAMPLER2D( gBuffer8Map );
    SAMPLER2D( gBuffer9Map );
    SAMPLER2D( gBuffer10Map );
    SAMPLER2D( gBuffer11Map );
    SAMPLER2D( gBuffer12Map );
    SAMPLER2D( gBuffer13Map );
    SAMPLER2D( gBuffer14Map );
#endif
#if defined ( D_IBL )
    SAMPLER2D (gDualPMapFront );
    SAMPLER2D (gDualPMapBack );
    vec4 gIBLParamsVec4;
#else
    SAMPLER2D( gBlurMask );
    SAMPLER3D( gColourLUT );    
    vec4 gTextureSizeMipLevelVec4;
#endif
    SAMPLER2D( gLensDirt );
    
#if defined ( D_UI )
    // UI shader stuff
    SAMPLER2D( gUIMap );
    SAMPLER2D( gUIMask );
    SAMPLER2D( gUIFullscreenEffect );
    SAMPLER2D( gUIFullscreenNormal );
    SAMPLER2D( gUIZoomEffect );
    SAMPLER2D( gUINoiseEffect );
    SAMPLER2D( gUIFullScreenRefraction );

    vec4   gUIColourVec4;
    vec4   gUIDeformVec4;
    vec4   gVignetteVec4;
    vec4   gUILowHealthVignetteVec4;
    vec4   gWashParamsVec4;
    vec4   gCriticalHitPointsVec4;
#else
    vec4 gSkyUpperParamsVec4;
    vec4 gLightShaftColourBottomVec4;
    vec4 gLightShaftColourTopVec4;
#endif
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
