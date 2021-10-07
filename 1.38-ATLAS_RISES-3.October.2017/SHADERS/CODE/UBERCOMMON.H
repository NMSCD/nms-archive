////////////////////////////////////////////////////////////////////////////////
///
///     @file       UberCommon.h
///     @author     User
///     @date       
///
///     @brief      UberCommon
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////



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
    vec4 gUVScrollStepVec4;
    vec4 gCustomParams01Vec4;

#if defined( D_SHADOW )
    
    vec4    gBboxDepthAndClips;

#else

#if defined( D_PARTICLE_UNIFORMS )
    vec4 gHeavyAirFadeOutVec4;
    vec4 gMultiTextureVec4;
#else
    vec4 gCustomParams02Vec4;
    vec4 gObjectColourVec4;
#endif

#endif   
};
 
//-----------------------------------------------------------------------------
///
///     CustomPerMaterialUniforms
///
///     @brief      CustomPerMaterialUniforms
///
///     Stuff that is only used for these materials.
//-----------------------------------------------------------------------------
struct CustomPerMaterialUniforms
{
    vec4 gImposterDataVec4;

#if defined( D_SHADOW )
    
    #ifdef _F55_MULTITEXTURE
        SAMPLER2DARRAY(gDiffuseMap);
    #else
        SAMPLER2D(gDiffuseMap);
    #endif

#else
    // Lighting
    // [ Roughness | NonMetalSpecularScale | Metallic | 0 ]
    vec4 gMaterialParamsVec4;
    vec4 gMaterialColourVec4;                  
    vec4 gMaterialSFXVec4;
    vec4 gMaterialSFXColVec4;
    
    vec4 gAverageColour1Vec4;            
    vec4 gAverageColour2Vec4;   

    vec4 gRecolour1Vec4;
    vec4 gRecolour2Vec4; 

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

    vec4 gFogFadeHeightsVec4;
    vec4 gSunPositionVec4;
    vec4 gSpaceScatteringParamsVec4;
        
    vec4 gSkyUpperColourVec4;
    vec4 gLightTopColourVec4;

    vec4 gPlaneSpotPositionVec4;

#ifdef _F55_MULTITEXTURE
    SAMPLER2DARRAY( gDiffuseMap );
    SAMPLER2DARRAY( gNormalMap );
#else
    SAMPLER2D( gDiffuseMap );
    SAMPLER2D( gNormalMap );
#endif
    
    // TODO add these ifdefs. Move them from uberfrag etc? SHouldn't need to....

#if !defined( D_PARTICLE_UNIFORMS )

//#ifdef _F41_DETAIL_DIFFUSE
    SAMPLER2DARRAY( gDetailDiffuseMap );
//#endif

//#ifdef D_DETAIL
    SAMPLER2DARRAY( gDetailNormalMap );
//#endif

//#ifdef _F16_DIFFUSE2MAP
    SAMPLER2D( gDiffuse2Map );
//#endif

//#ifdef D_MASKS
#ifdef _F55_MULTITEXTURE
    SAMPLER2DARRAY( gMasksMap );
#else
    SAMPLER2D( gMasksMap );
#endif
//#endif
    

#if !defined( D_RECOLOUR ) && !defined( D_COMBINE ) && !defined( D_DEFER )
    SAMPLER2D( gCausticMap );
    SAMPLER2D( gCausticOffsetMap );
    SAMPLER2D( gDualPMapFront );
    SAMPLER2D( gDualPMapBack );
#endif

#if !defined( D_DEFER ) || (defined( D_TERRAIN ) && defined( D_DEFER ))
    SAMPLER2D(gCloudShadowMap);
    SAMPLER2DSHADOW_SRT(gShadowMap);
#endif

#else

    // move to particle common
    SAMPLER2D(gDepthBuffer);

#endif

#if defined( D_DEFERRED_DECAL ) || defined( D_SKY )
    SAMPLER2D( gBufferMap );
#endif

#ifdef D_SKY
    vec4 gSpaceSkyColour1Vec4;
    vec4 gSpaceSkyColour2Vec4;
    vec4 gSpaceSkyColour3Vec4;
    vec4 gSpaceCloudColourVec4;
    vec4 gSpaceSunColourVec4;

    vec4 gSpaceNebulaColour1Vec4;
    vec4 gSpaceNebulaColour2Vec4;
    vec4 gSpaceNebulaColour3Vec4;
    vec4 gSpaceNebulaColour4Vec4;

    vec4 gSpaceNormalParamsVec4;
    vec4 gSpaceNebulaParamsVec4;
#endif

    SAMPLER3D(gPerlin3D);
    SAMPLER2D(gNoiseMap);

    vec4 gTileBlendScalesVec4;

    SAMPLER2D(gHSVOverlay);
    SAMPLER2D(gHSVNormalOverlay);

    vec4 gHueOverlayParamsVec4;
    vec4 gSaturationOverlayParamsVec4;
    vec4 gValueOverlayParamsVec4;

    vec4 gTerrainColour1Vec4;
    vec4 gTerrainColour2Vec4;

#endif

#ifdef _F31_DISPLACEMENT
    vec4 gWaveOneAmplitudeVec4;
    vec4 gWaveOneFrequencyVec4;
    vec4 gWaveOneFallOffAndSpeedVec4;

    vec4 gWaveTwoAmplitudeVec4;
    vec4 gWaveTwoFrequencyVec4;
    vec4 gWaveTwoFallOffAndSpeedVec4;
#endif

#ifdef D_FADE
    SAMPLER2D(gFadeNoiseMap);
#endif


};

//
// This is the SRT buffer that everything gets uploaded to (on PS4). PC just functions as normal.
//
struct UniformBuffer
{
 //    DECLARE_PTR( CommonPerMaterialUniforms,    mpCommonPerMaterial )   /*: PER_MATERIAL*/ // sematics currently crash the compiler so the parser is hardcoded to look for names.
     DECLARE_PTR( PerFrameUniforms,             mpPerFrame )   /*: PER_MATERIAL*/ // sematics currently crash the compiler so the parser is hardcoded to look for names.
     DECLARE_PTR( CommonPerMeshUniforms,        mpCommonPerMesh )       /*: PER_MESH*/

     DECLARE_PTR( CustomPerMaterialUniforms,    mpCustomPerMaterial )   /*: PER_MATERIAL*/
     DECLARE_PTR( CustomPerMeshUniforms,        mpCustomPerMesh )       /*: PER_MESH*/
};






