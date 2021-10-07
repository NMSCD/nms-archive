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
///     CustomPerMaterialUniforms
///
///     @brief      CustomPerMaterialUniforms
///
///   
//----------------------------------------------------------------------------- 
struct CustomPerMaterialUniforms
{
        // Fog
    vec4 gSkyColourVec4;

    vec4 gSkyUpperParamsVec4;
    vec4 gSkyUpperColourVec4;
    vec4 gSkySolarColourVec4;
    vec4 gSkyGradientSpeedVec4; 
    vec4 gHorizonColourVec4;
    vec4 gSunColourVec4;
    vec4 gScatteringParamsVec4;
    vec4 gSunPositionVec4;

    vec4 gFogColourVec4;
    vec4 gFogParamsVec4;
    vec4 gHeightFogParamsVec4;
    vec4 gHeightFogColourVec4;

    vec4 gSpaceHorizonColourVec4;
    vec4 gFogFadeHeightsVec4;
    vec4 gSpaceSkyColourVec4;
    vec4 gSpaceScatteringParamsVec4;

    vec4 gWaterColourNearVec4;
    vec4 gWaterColourFarVec4;
    vec4 gWaterFogVec4;

    vec4 gSpaceSkyColour1Vec4;
    vec4 gSpaceSkyColour2Vec4;
    vec4 gSpaceSkyColour3Vec4;

    vec4 gMaterialParamsVec4;   

    vec4 gaPlanetPositionsVec4[ 6 ];
    
    SAMPLER2D( gDiffuseMap );
    SAMPLER2D( gBufferMap );

#if !defined( D_RECOLOUR ) && !defined( D_COMBINE ) && !defined( D_ATMOSPHERE )
    SAMPLER2D( gDualPMapFront );
    SAMPLER2D( gDualPMapBack );
    SAMPLER2D( gCloudShadowMap );
    SAMPLER2DSHADOW_SRT(gShadowMap);
#endif
};

//
// This is the SRT buffer that everything gets uploaded to (on PS4). PC just functions as normal.
//
struct UniformBuffer
{
    DECLARE_PTR( PerFrameUniforms,          mpPerFrame )       /*: PER_MATERIAL*/
    DECLARE_PTR( CustomPerMaterialUniforms, mpCustomPerMaterial )       /*: PER_MATERIAL*/
    DECLARE_PTR( CommonPerMeshUniforms,     mpCommonPerMesh )           /*: PER_MESH*/
};