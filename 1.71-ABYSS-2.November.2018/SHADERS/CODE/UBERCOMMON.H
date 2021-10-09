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

#include "Common/Defines.shader.h"

// Griff - working around SDK5 shader compiler bug with billboard/displacement vertex positions
#if defined( D_PLATFORM_ORBIS ) && defined( D_VERTEX )
#if defined( _F12_BATCHED_BILLBOARD ) || defined(_F31_DISPLACEMENT)
#pragma argument (O2; nofastmath; scheduler=minpressure)
#endif
#endif

#ifdef _F20_PARALLAXMAP
    #define _F03_NORMALMAP
#endif

#if defined( _F03_NORMALMAP ) || defined( _F42_DETAIL_NORMAL )
    #define D_NORMALMAPPED
#endif

#if defined( D_NORMALMAPPED ) || defined( _F27_VBTANGENT ) || defined( _F31_DISPLACEMENT )
    #define D_DECLARE_TANGENT
#endif

#if defined( _F25_ROUGHNESS_MASK ) || defined( _F41_DETAIL_DIFFUSE ) || defined( _F42_DETAIL_NORMAL ) || defined( _F24_AOMAP ) || defined( _F40_SUBSURFACE_MASK ) || defined( _F54_COLOURMASK ) || defined( _F44_IMPOSTER )
#define D_MASKS
#endif

#if ( defined( _F15_WIND ) || defined( _F19_BILLBOARD ) ) && !defined( D_DEPTHONLY ) && !defined( D_DEPTH_CLEAR ) && !defined( _F12_BATCHED_BILLBOARD ) && !defined( _F44_IMPOSTER )
    #define D_OUTPUT_MOTION_VECTORS
#endif

#if ( defined( _F31_DISPLACEMENT ) && !defined( D_DEPTHONLY ) && !defined( D_DEPTH_CLEAR ) )
    #define D_OUTPUT_MOTION_VECTORS
#endif

#if defined( _F01_DIFFUSEMAP ) || defined( _F03_NORMALMAP ) || defined( D_IMPOSTERMASKS ) || defined(D_MASKS)
    #define D_TEXCOORDS
#endif

#if !defined( D_DEPTHONLY ) || ( defined( D_DEPTHONLY ) && defined( _F60_ACUTE_ANGLE_FADE ) && !defined( D_IMPOSTER ) ) || defined( _F63_DISSOLVE )
#define D_USES_WORLD_POSITION
#endif

#if !defined _F03_NORMALMAP && (defined( _F04_ENVMAP ) || !defined( _F07_UNLIT ))
    #define D_USES_VERTEX_NORMAL
#endif

#if (defined( _F58_USE_CENTRAL_NORMAL ) && !defined( D_DEPTHONLY )) || defined( _F60_ACUTE_ANGLE_FADE ) || ( defined( _F36_DOUBLESIDED ) && !defined( D_DEPTHONLY ) )
    #define D_USES_VERTEX_NORMAL
#endif

#if defined( _F42_DETAIL_NORMAL ) || defined( _F41_DETAIL_DIFFUSE )
    #define D_DETAIL
#endif

#if defined( _F02_SKINNED )
    #define D_SKINNING_UNIFORMS
#endif

#if !defined( D_DEFER ) || defined( D_DEFERRED_DECAL ) || defined( D_OUTPUT_MOTION_VECTORS ) 
    #define D_USE_SCREEN_POSITION
#endif

#if defined( D_IMPOSTER_COLOUR ) || defined( D_IMPOSTER_NORMAL ) || defined( D_IMPOSTER_MASK ) || defined( D_IMPOSTER_VERTEX )
	#define D_IMPOSTER
#endif

#if (!defined( D_IMPOSTER ) && !defined( D_ZEQUALS )) || defined( _F60_ACUTE_ANGLE_FADE )
	#define D_FADE
#endif

#ifdef _F31_DISPLACEMENT
    #define D_DECLARE_TIME
#endif

//-----------------------------------------------------------------------------
//      Include files

#include "Common/CommonUniforms.shader.h"

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
    vec4 gTileBlendScalesVec4;
    vec4 gTerrainSlopeParamsVec4;

    vec4 gHueOverlayParamsVec4;
    vec4 gSaturationOverlayParamsVec4;
    vec4 gValueOverlayParamsVec4;
    vec4 gTerrainColour1Vec4;
    vec4 gTerrainColour2Vec4;

#ifdef _F31_DISPLACEMENT
    vec4 gWaveOneAmplitudeVec4;
    vec4 gWaveOneFrequencyVec4;
    vec4 gWaveOneFallOffAndSpeedVec4;

    vec4 gWaveTwoAmplitudeVec4;
    vec4 gWaveTwoFrequencyVec4;
    vec4 gWaveTwoFallOffAndSpeedVec4;
#endif

#if defined( D_SHADOW )
    
BEGIN_SAMPLERBLOCK  
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
    
#if !defined(D_DEFER) && !defined( D_PARTICLE_UNIFORMS )
    vec4 gAverageColour1Vec4;            
    vec4 gAverageColour2Vec4;   

    vec4 gRecolour1Vec4;
    vec4 gRecolour2Vec4; 

    vec4 gSkyColourVec4;
    vec4 gHorizonColourVec4;
    vec4 gSunColourVec4;
    vec4 gWaterFogColourNearVec4;
    vec4 gWaterFogColourFarVec4;
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
#endif

#if defined(D_PLANE_SPOT)
    vec4 gPlaneSpotPositionVec4;
#endif

#if defined( _F63_DISSOLVE )
    vec4 gDissolveDataVec4;
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

BEGIN_SAMPLERBLOCK  

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

    // move to particle common
    SAMPLER2D(gDepthBuffer);

#endif

#endif

    SAMPLER2D(gNoiseMap);
    SAMPLER3D(gPerlin3D);

#ifdef D_FADE
    SAMPLER2D(gFadeNoiseMap);
#endif

#if defined( D_DEFERRED_DECAL ) || defined( D_SKY )
    SAMPLER2D(gBufferMap);
#endif

END_SAMPLERBLOCK
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






