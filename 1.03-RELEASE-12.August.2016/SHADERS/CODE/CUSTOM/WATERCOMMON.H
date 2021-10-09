////////////////////////////////////////////////////////////////////////////////
///
///     @file       DoFFragment.h
///     @author     User
///     @date       
///
///     @brief      DepthOfFieldFragmentShader
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 


//-----------------------------------------------------------------------------
//      Include files

#include "Common/Defines.shader.h"

struct CustomPerMaterialUniforms
{
    vec4 gMaterialParamsVec4;

    vec4 gWaveScaleVec4;
    vec4 gWaveSpeedVec4;
    vec4 gWindDirectionVec4;
    vec4 gWaterSurfaceParamsVec4;
    vec4 gFresnelParamsVec4;
    vec4 gFoamParamsVec4;
    vec4 gFoamColourVec4;

    // Fog
    vec4 gSkyColourVec4;
    vec4 gHorizonColourVec4;
    vec4 gSunColourVec4;
    vec4 gWaterColourNearVec4;
    vec4 gWaterColourFarVec4;
    vec4 gWaterFogVec4;
    vec4 gHeightFogParamsVec4;
    vec4 gHeightFogColourVec4;
    vec4 gSpaceHorizonColourVec4;
    vec4 gFogColourVec4;
    vec4 gFogParamsVec4;
    vec4 gScatteringParamsVec4;
    vec4 gSpaceSkyColour1Vec4;
    vec4 gSpaceSkyColour2Vec4;
    vec4 gSpaceSkyColour3Vec4;
    vec4 gSpaceSkyColourVec4;
    vec4 gFogFadeHeightsVec4;
    vec4 gSunPositionVec4;
    vec4 gSpaceScatteringParamsVec4;
    SAMPLER2DSHADOW_SRT( gShadowMap );

#if !defined( D_RECOLOUR ) && !defined(D_COMBINE)
    SAMPLER2D( gDualPMapFront );
    SAMPLER2D( gDualPMapBack );
    SAMPLER2D( gCloudShadowMap );
#endif

    SAMPLER2D( gNormalMap );
    SAMPLER2D( gLargeNormalMap );
    SAMPLER2D( gFoamNormalMap );

    SAMPLER2D( gBufferMap );
    SAMPLER2D( gReflectionMap );
    SAMPLER2D( gFresnelMap );
    SAMPLER2D( gFoamMap );
    SAMPLER2D( gDisplacementMap );
#ifdef D_FADE //ined( D_INSTANCED ) || defined( D_TERRAIN )
    SAMPLER2D( gFadeNoiseMap );
#endif
    
};


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
    SAMPLER2D( gBuffer1Map );
    SAMPLER2D( gBuffer2Map );

};



struct UniformBuffer
{
    DECLARE_PTR( PerFrameUniforms,          mpPerFrame )   /*: PER_MATERIAL*/ // sematics currently crash the compiler so the parser is hardcoded to look for names.
    DECLARE_PTR( CommonPerMeshUniforms,     mpCommonPerMesh )       /*: PER_MESH*/

    DECLARE_PTR( CustomPerMaterialUniforms, mpCustomPerMaterial )   /*: PER_MATERIAL*/
    DECLARE_PTR( CustomPerMeshUniforms,     mpCustomPerMesh )
};
