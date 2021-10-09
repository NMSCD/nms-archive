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
    // not sure where this should go - from CommonVertexUniforms
    vec4 gCustomParams01Vec4;
    vec4 gCustomParams02Vec4;
    vec4 gUVScrollStepVec4;
       
   
//#ifdef D_HEAVYAIR
    vec4 gHeavyAirFadeOutVec4;
//#endif

    vec4 gMultiTextureVec4;

    // temp from mapobjectfrag
    vec4    gObjectColourVec4;
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

    SAMPLER2D( gCausticMap );
    SAMPLER2D( gCausticOffsetMap );

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

    vec4 gImposterDataVec4;
    
    SAMPLER2D( gDiffuseMap );
    SAMPLER2D( gNormalMap );
    
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
    SAMPLER2D( gMasksMap );
//#endif
#endif

    // move to particle common
    SAMPLER2D( gDepthBuffer );

#if !defined( D_PARTICLE_UNIFORMS )
    
    SAMPLER2DSHADOW_SRT( gShadowMap );

#if !defined( D_RECOLOUR ) && !defined(D_COMBINE)
    SAMPLER2D( gDualPMapFront );
    SAMPLER2D( gDualPMapBack );
    SAMPLER2D( gCloudShadowMap );
#endif

#endif

#ifdef D_FADE //ined( D_INSTANCED ) || defined( D_TERRAIN )
    SAMPLER2D( gFadeNoiseMap );
#endif

#ifdef D_DEFERRED_DECAL
    SAMPLER2D( gBufferMap );
#endif

#ifdef D_SKY
    vec4 gSpaceSkyColour1Vec4;
    vec4 gSpaceSkyColour2Vec4;
    vec4 gSpaceSkyColour3Vec4;
    vec4 gSpaceCloudColourVec4;

    vec4 gSpaceNebulaColour1Vec4;
    vec4 gSpaceNebulaColour2Vec4;
    vec4 gSpaceNebulaColour3Vec4;
    vec4 gSpaceNebulaColour4Vec4;

    vec4 gSpaceNormalParamsVec4;
    vec4 gSpaceNebulaParamsVec4;

    SAMPLER2D(  gNoiseMap );
#endif

#ifdef _F31_DISPLACEMENT
    vec4 gWaveOneAmplitudeVec4;
    vec4 gWaveOneFrequencyVec4;
    vec4 gWaveOneFallOffAndSpeedVec4;

    vec4 gWaveTwoAmplitudeVec4;
    vec4 gWaveTwoFrequencyVec4;
    vec4 gWaveTwoFallOffAndSpeedVec4;
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






