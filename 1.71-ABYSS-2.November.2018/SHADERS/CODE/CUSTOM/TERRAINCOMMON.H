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
#if !defined( D_DEPTHONLY ) || defined( D_TESS_SHADERS_PRESENT )

    vec4 gSkyColourVec4;
    vec4 gHorizonColourVec4;
    vec4 gSunColourVec4;
    vec4 gWaterFogColourNearVec4;
    vec4 gWaterFogColourFarVec4;

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
    vec4 gTextureScalesVec4;
    vec4 gTextureScales2Vec4;
    vec4 gTextureDistancesVec4;
    vec4 gTextureDistances2Vec4;

    vec4 gSkyUpperColourVec4;
    vec4 gMaterialParamsVec4;
#if !defined(D_PLATFORM_OPENGL)
    vec4 gaAverageColoursVec4[ D_TERRAINCOLOURARRAY_SIZE ];
    vec4 gaTerrainColoursVec4[ D_TERRAINCOLOURARRAY_SIZE ];
#endif

    vec4 gAverageColour1Vec4; 
    vec4 gAverageColour2Vec4; 

    vec4 gDebugColourVec4;

    vec4 gHueOverlayParamsVec4;
    vec4 gSaturationOverlayParamsVec4;
    vec4 gValueOverlayParamsVec4;

    vec4 gTileBlendScalesVec4;
    vec4 gTerrainSlopeParamsVec4;

    vec4 gLightTopColourVec4;

BEGIN_SAMPLERBLOCK

    SAMPLER3DREG(gPerlin3D, 5);
    SAMPLER2DARRAYREG(gNormalMap, 7);
    SAMPLER2DARRAYREG(gSubstanceNormalMap, 9);
    SAMPLER2DARRAYREG(gDiffuseMap, 6);
    SAMPLER2DARRAYREG(gSubstanceDiffuseMap, 8);
    SAMPLER2DREG(gNoiseMap, 4);

#if !defined( D_RECOLOUR ) && !defined( D_COMBINE ) && !defined( D_DEFER )
    SAMPLER2D(gCausticMap);
    SAMPLER2D(gCausticOffsetMap);
    SAMPLER2DSHADOW_SRT(gShadowMap);
    SAMPLER2D(gCloudShadowMap);
    SAMPLER2D(gDualPMapFront);
    SAMPLER2D(gDualPMapBack);
    SAMPLER2D(gHSVOverlay);
    SAMPLER2D(gHSVNormalOverlay);
#endif

#else

BEGIN_SAMPLERBLOCK

#endif

#if defined( D_FADE )
    SAMPLER2DREG( gFadeNoiseMap, 14 );
#endif

END_SAMPLERBLOCK

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
    vec4 gSparseTextureStatusVec4;
    
#ifdef D_WRITE_TEX_CACHE
    vec4 gTextureCoordsAdjustVec4;
    vec4 gTextureCoordsBorderVec4;
#endif

BEGIN_SAMPLERBLOCK

#ifdef D_READ_TEX_CACHE

    SAMPLER2DREG( gSparseNearDiffMap, 15 );
    SAMPLER2DREG( gSparseNearNormMap, 16 );

    SAMPLER2DREG( gSparseNearStatusMap, 17 );

    SAMPLER2DREG( gSparseFarStatusMap, 19 );

    SAMPLER2DREG( gSparseFarDiffMap, 21 );
    SAMPLER2DREG( gSparseFarNormMap, 22 );

#endif

BEGIN_IMAGEBLOCK

#ifdef D_WRITE_CACHE_FEEDBACK

    RWINTIMAGE2D( r32i, gFeedbackMap );

#endif
    
END_IMAGEBLOCK

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
