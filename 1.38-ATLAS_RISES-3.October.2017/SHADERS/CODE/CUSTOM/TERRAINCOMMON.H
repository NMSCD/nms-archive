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
    vec4 gWaterFogVec4;
#if !defined( D_DEPTHONLY )

    vec4 gSkyColourVec4;
    vec4 gHorizonColourVec4;
    vec4 gSunColourVec4;
    vec4 gWaterColourNearVec4;
    vec4 gWaterColourFarVec4;

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
    vec4 gTextureParamsVec4;
    vec4 gTextureScalesVec4;
    vec4 gTextureScales2Vec4;

    vec4 gSkyUpperColourVec4;
    vec4 gMaterialParamsVec4;
#if defined(D_PLATFORM_ORBIS)
    vec4 gaAverageColoursVec4[ D_TERRAINCOLOURARRAY_SIZE ];
    vec4 gaTerrainColoursVec4[ D_TERRAINCOLOURARRAY_SIZE ];
#endif

    vec4 gAverageColour1Vec4; 
    vec4 gAverageColour2Vec4; 

    vec4 gDebugColourVec4;

#if !defined( D_DEFER )
    SAMPLER2D( gCausticMap );
    SAMPLER2D( gCausticOffsetMap );
#endif

    SAMPLER2DARRAY( gNormalMap );
    SAMPLER2DARRAY( gDiffuseMap );
    SAMPLER2DARRAY( gSubstanceDiffuseMap );
    SAMPLER2DARRAY( gSubstanceNormalMap );
    SAMPLER2DARRAY( gLargeDiffuseMap );
    SAMPLER2DARRAY( gLargeNormalMap );

    SAMPLER2D( gTileOverlay );
    SAMPLER2D( gHSVOverlay );
    SAMPLER2D( gHSVNormalOverlay );
    SAMPLER2D( gNoiseMap );
    SAMPLER3D( gPerlin3D );

    vec4 gHueOverlayParamsVec4;
    vec4 gSaturationOverlayParamsVec4;
    vec4 gValueOverlayParamsVec4;

    vec4 gTileBlendScalesVec4;

    vec4 gLightTopColourVec4;


#if !defined( D_RECOLOUR ) && !defined( D_COMBINE ) && !defined( D_DEFER )
    SAMPLER2D( gDualPMapFront );
    SAMPLER2D( gDualPMapBack );
#endif

#endif

#if defined( D_FADE )
    SAMPLER2DSHADOW_SRT(gShadowMap);
    SAMPLER2D(gCloudShadowMap);
    SAMPLER2D( gFadeNoiseMap );
#endif

};

//-----------------------------------------------------------------------------
///
///     CustomPerMaterialUniforms
///
///     @brief      CustomPerMaterialUniforms
///
///   
//-----------------------------------------------------------------------------
struct CustomPerMeshUniforms
{
    vec4 gTerrainLodParamsVec4;
};

//
// This is the SRT buffer that everything gets uploaded to (on PS4). PC just functions as normal.
//
struct UniformBuffer
{
     DECLARE_PTR( PerFrameUniforms,             mpPerFrame )   /*: PER_MATERIAL*/ // sematics currently crash the compiler so the parser is hardcoded to look for names.
     DECLARE_PTR( CommonPerMeshUniforms,        mpCommonPerMesh )       /*: PER_MESH*/

     DECLARE_PTR( CustomPerMaterialUniforms,    mpCustomPerMaterial )   /*: PER_MATERIAL*/
     DECLARE_PTR( CustomPerMeshUniforms,        mpCustomPerMesh )       /*: PER_MESH*/
};

//
