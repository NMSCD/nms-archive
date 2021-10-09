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

    vec4 gThresholdParamsVec4;
    vec4 gBlurParamsVec4;

    SAMPLER2D( gBufferMap );
    SAMPLER2D( gBuffer1Map );
    SAMPLER2D( gBuffer2Map );
    SAMPLER2D( gBuffer3Map );
    SAMPLER2D( gBuffer4Map );
    SAMPLER2D( gBuffer5Map );
    
    SAMPLER2D( gBlurMask );
    SAMPLER3D( gColourLUT );    

    SAMPLER2D( gLensDirt );


    
    // water - separate?
    vec4 gWaterParamsVec4;
    vec4 gWaterParams2Vec4;
    vec4 gFoamColourVec4;               

//#if defined( _F03_NORMALMAP ) || defined(D_WATER)
    // SAMPLER2D( gNormalMap );
//#endif


//#ifdef D_UI_MASK
    // UI shader stuff
    SAMPLER2D( gUIMap );
    SAMPLER2D( gUIMask );
    SAMPLER2D( gUIFullscreenEffect );
    SAMPLER2D( gUIFullscreenNormal );
    SAMPLER2D( gUIZoomEffect );
    SAMPLER2D( gUINoiseEffect );
    //SAMPLER2D( gUINoShieldOverlay );
    SAMPLER2D( gUIFullScreenRefraction );
//#endif

    vec4   gUIColourVec4;
    vec4   gUIDeformVec4;
    vec4   gVignetteVec4;
    vec4   gUILowHealthVignetteVec4;
    vec4   gWashParamsVec4;
    vec4   gCriticalHitPointsVec4;

    // move into a IBLCOmmon.h?
    vec4 gIBLParamsVec4;
	vec4 gTextureSizeMipLevelVec4;
	
	vec4 gEffectAmountVec4;

#if !defined( D_RECOLOUR ) && !defined( D_COMBINE ) && !defined( D_PLATFORM_PC )
    SAMPLER2D( gDualPMapFront );
    SAMPLER2D( gDualPMapBack );
    //SAMPLER2D( gCloudShadowMap );
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
