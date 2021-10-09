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

    vec4 gMaterialParamsVec4;
#if defined(D_PLATFORM_ORBIS)
    vec4 maAverageColoursVec4[ 13 ];
    vec4 maTerrainColoursVec4[ 13 ];
#endif

    vec4 gAverageColour1Vec4; 
    vec4 gAverageColour2Vec4; 

    vec4 gDebugColourVec4;

    SAMPLER2D( gCausticMap );
    SAMPLER2D( gCausticOffsetMap );

    SAMPLER2DARRAY( gNormalMap );
    SAMPLER2DARRAY( gDiffuseMap );
    SAMPLER2DARRAY( gSubstanceDiffuseMap );
    SAMPLER2DARRAY( gSubstanceNormalMap );
    SAMPLER2DSHADOW_SRT( gShadowMap );

#if !defined( D_RECOLOUR ) && !defined(D_COMBINE)
    SAMPLER2D( gDualPMapFront );
    SAMPLER2D( gDualPMapBack );
    SAMPLER2D( gCloudShadowMap );
#endif
#ifdef D_FADE //ined( D_INSTANCED ) || defined( D_TERRAIN )
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
