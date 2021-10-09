

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
    vec4 gMaterialParamsVec4;
    vec4 gMaterialSFXVec4;
    vec4 gMaterialSFXColVec4;

    vec4 gaPlanetPositionsVec4[ 6 ];
    vec4 gaPlanetColoursVec4[ 6 ];

    vec4 gSkyColourVec4;
    vec4 gHorizonColourVec4;
    vec4 gSunColourVec4;
    vec4 gScatteringParamsVec4;
    vec4 gSunPositionVec4;

    vec4 gSkyUpperParamsVec4;
    vec4 gSkyUpperColourVec4;
    vec4 gSkySolarColourVec4;
    vec4 gSkyGradientSpeedVec4;

    vec4 gFogParamsVec4;
    vec4 gFogColourVec4;
    vec4 gHeightFogParamsVec4;
    vec4 gHeightFogColourVec4;

    vec4 gWaterFogVec4;
    vec4 gWaterFogColourFarVec4;
    vec4 gWaterFogColourNearVec4;

    vec4 gSpaceHorizonColourVec4;
    vec4 gFogFadeHeightsVec4;
    vec4 gFogFadeHeights2Vec4;
    vec4 gFogFadeHeights3Vec4;
    vec4 gSpaceSkyColourVec4;
    vec4 gSpaceScatteringParamsVec4;

    vec4 gSpaceSkyColour1Vec4;
    vec4 gSpaceSkyColour2Vec4;
    vec4 gSpaceSkyColour3Vec4;
    vec4 gSpaceFogColourVec4;
    vec4 gSpaceFogColour2Vec4;
    vec4 gSpaceFogParamsVec4;

    vec4 gLightShaftParamsVec4;
    vec4 gLightTopColourVec4;

    vec4 gHueOverlayParamsVec4;
    vec4 gSaturationOverlayParamsVec4;
    vec4 gValueOverlayParamsVec4;

BEGIN_SAMPLERBLOCK
    
	SAMPLER2D( gBufferMap );
    SAMPLER2D( gBuffer1Map );
    SAMPLER2D( gBuffer2Map );
    SAMPLER2D( gBuffer3Map );
    SAMPLER2D( gBuffer4Map );
    
#if !defined( D_RECOLOUR ) && !defined(D_COMBINE)
    SAMPLER2D(gCausticMap);
    SAMPLER2D(gCausticOffsetMap);
    SAMPLER2DSHADOW_SRT(gShadowMap);
    SAMPLER2D(gCloudShadowMap);
    SAMPLER2D(gDualPMapFront);
    SAMPLER2D(gDualPMapBack);
    SAMPLER2D(gHSVOverlay);
    SAMPLER2D(gHSVNormalOverlay);
#endif

END_SAMPLERBLOCK

#ifndef D_COMPUTE
#define TEX_COORDS IN(mTexCoordsVec2)
#define READ_GBUFFER( structure, buf, coords ) texture2D( SAMPLER_GETMAP( structure, buf ), coords )

#elif defined D_PLATFORM_ORBIS

//-----------------------------------------------------------------------------
///
///     ComputeOutputUniforms
///
///     @brief      ComputeOutputUniforms
///
///     Refs to output textures for compute quad shaders
//-----------------------------------------------------------------------------

struct ComputeOutputUniforms
{
    RW_Texture2D<float4> gOutTexture0; 
    RW_Texture2D<float4> gOutTexture1; 
    RW_Texture2D<float4> gOutTexture2; 
    RW_Texture2D<float4> gOutTexture3; 
    RW_Texture2D<float4> gOutTexture4; 
};

#define TEX_COORDS ( ( vec2( dispatchThreadID.xy ) + vec2(0.5,0.5) ) / vec2( GetImgResolution( lUniforms.mpCmpOutPerMesh->gOutTexture0 ) ) )
#define READ_GBUFFER( structure, buf, coords ) (structure->buf[dispatchThreadID.xy])

#else

RW_Texture2D<float4> gOutTexture0 : register(u0);
RW_Texture2D<float4> gOutTexture1 : register(u1);
RW_Texture2D<float4> gOutTexture2 : register(u2);
RW_Texture2D<float4> gOutTexture3 : register(u3);
RW_Texture2D<float4> gOutTexture4 : register(u4);

#define TEX_COORDS ( ( vec2( dispatchThreadID.xy ) + vec2(0.5,0.5) ) / vec2( GetImgResolution( gOutTexture0 ) ) )
#define READ_GBUFFER( structure, buf, coords ) (buf[dispatchThreadID.xy])

#endif


//
// This is the SRT buffer that everything gets uploaded to (on PS4). PC just functions as normal.
//
struct UniformBuffer
{
    DECLARE_PTR( PerFrameUniforms, mpPerFrame )
    DECLARE_PTR( CustomPerMaterialUniforms, mpCustomPerMaterial )
    DECLARE_PTR( CommonPerMeshUniforms, mpCommonPerMesh )

#if defined D_PLATFORM_ORBIS && defined D_COMPUTE
     DECLARE_PTR( ComputeOutputUniforms,        mpCmpOutPerMesh )   /* hack - marked 'per mesh' so it'll be alloced in cmd buf */
#endif
};
