////////////////////////////////////////////////////////////////////////////////
///
///     @file       TerrainFragment.h
///     @author     User
///     @date       
///
///     @brief      TerrainFragmentShader
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 

#define D_TERRAIN

#if !defined( D_REFLECT_WATER_UP ) && !defined( D_REFLECT_WATER ) && !defined( D_REFLECT_DUALP ) && !defined( D_WRITE_TEX_CACHE )
    #define D_FADE
#endif
#if /*defined( D_ASTEROID ) ||*/ defined( D_REFLECT_WATER ) || defined( D_REFLECT_WATER_UP ) || defined( D_REFLECT_DUALP )
#define _F50_DISABLE_POSTPROCESS
#endif

#ifdef GL_AMD_shader_explicit_vertex_parameter
#extension GL_AMD_shader_explicit_vertex_parameter : enable
#endif

//#if defined( D_REFLECT_WATER ) || defined( D_REFLECT_DUALP ) 
//#define D_LOW_QUALITY
//#endif

//-----------------------------------------------------------------------------
//      Include files

#include "Common/Defines.shader.h"

#include "Common/Common.shader.h"
#include "Common/CommonUniforms.shader.h"





//-----------------------------------------------------------------------------
//      Global Data



//
// In TerrainCommon we have our uniforms specific to terrain declared. PLUS, IMPORTANTLY we have the SRT buffer declared (gUniforms). 
// This MUST be included after CommonUniforms, but before all the other stuff that references gUniforms.
//

#include "Custom/TerrainCommon.h"

//
// Must be included after gUniforms is declared as they reference it.
//
#define D_TEXTURE_ARRAYS
#if defined( D_DEPTHONLY )
#include "Common/CommonGBuffer.shader.h"
#else
#include "Common/CommonTriPlanarTexturing.shader.h"
#ifdef D_DEFER
#include "Common/CommonDepth.shader.h"
#include "Common/CommonPlanet.shader.h"
#include "Common/CommonFragment.shader.h"
#include "Common/CommonGBuffer.shader.h"
#include "OutputDeferred.shader.h"

#else
#include "Common/CommonLighting.shader.h"
#include "OutputForward.shader.h"
#endif
#endif
#include "Common/CommonFade.shader.h" 
#include "Common/CommonNoise.shader.h" 
#include "Common/CommonFragment.shader.h" 

//-----------------------------------------------------------------------------
//      Constants 

//-----------------------------------------------------------------------------
//      Typedefs and Classes 

//-----------------------------------------------------------------------------
///
///     Input
///
///     @brief  Input
///
//-----------------------------------------------------------------------------

DECLARE_INPUT
    INPUT_SCREEN_POSITION

#if !defined( D_DEPTHONLY )
    INPUT( vec4,   mWorldPositionVec4,              TEXCOORD0 )
#ifdef D_ASTEROID    
    INPUT( vec3,   mLocalPositionVec3,              TEXCOORD1 )
#endif
    INPUT( vec4,   mTileVec4,                       TEXCOORD2 )
    INPUT( vec4,   mTexCoordVec2_mTexBorderVec2,    TEXCOORD3 )
    INPUT( vec4,   mSmoothNormalVec3_mfDistForFade, TEXCOORD4 )
#if ( defined( D_TESS_SHADERS_PRESENT ) || defined( D_PLATFORM_PC ) ) && ( defined( D_READ_TEX_CACHE ) || defined( D_WRITE_TEX_CACHE ) )
    INPUT( vec4,   mTexCoordsDPDUVec4,              TEXCOORD6 )
    INPUT( vec4,   mTexCoordsDPDVVec4,              TEXCOORD7 )
#endif
#endif
#if defined( D_OUTPUT_MOTION_VECTORS )
    INPUT( vec4,   mScreenSpacePositionVec4,        TEXCOORD5 )
#endif
    

DECLARE_END


//-----------------------------------------------------------------------------
//    Functions


//-----------------------------------------------------------------------------
///
///     Main
///
///     @brief      Main Fragment
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
#if defined( D_DEPTHONLY )
VOID_MAIN_SRT
#else
FRAGMENT_MAIN_COLOUR_SRT
#endif
{  
    #if defined( D_REFLECT_WATER )
    {
        vec3  lWorldUpVec3;
        float lfHeight;
        lWorldUpVec3 = GetWorldUp(IN(mWorldPositionVec4).xyz, lUniforms.mpCommonPerMesh->gPlanetPositionVec4, lfHeight);

        if( lfHeight < lUniforms.mpCustomPerMaterial->gWaterFogVec4.r - 0.5 )
        {
            discard;
        }
    }
    #endif

    #if defined( D_FADE ) && ( !(defined( D_TERRAIN_X_FACING ) || defined( D_TERRAIN_Y_FACING ) || defined( D_TERRAIN_Z_FACING )  || defined( D_TERRAIN_N_FACING ) ) )
    {
        CheckFadeFragCoord( 
            SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMaterial, gFadeNoiseMap ),
            IN_SCREEN_POSITION,
            lUniforms.mpCommonPerMesh->gfFadeValue );

    }
    #endif


#if !defined( D_DEPTHONLY )
    vec3  lFragmentColourVec3;
    vec3  lNormalVec3;

    float lfTextureSmallScaleFactor = 1.0;
    float lfTextureLargeScaleFactor = 1.0;

    float lfSpecular = 0.0;

    //-----------------------------------------------------------------------------
    ///
    ///     Triplanar Texturing
    ///
    //-----------------------------------------------------------------------------
    float lfSubsurface = 0.0;
    float lfMetallic   = 0.0;
    float lfHeight     = 0.0;
    float lfGlow       = 0.0;
    float lfScaleFade  = 0.0;
    vec3  lTerrainSpacePositionVec3;      
    vec3  lTerrainSpaceNormalVec3; 

    {
        vec3  lTerrainSpaceFaceNormalVec3;
        vec3  lTerrainOffsetVec3;       

        lTerrainSpaceNormalVec3       = normalize( IN( mSmoothNormalVec3_mfDistForFade ).xyz );
        lTerrainSpaceFaceNormalVec3   = normalize( IN( mSmoothNormalVec3_mfDistForFade ).xyz );
        lTerrainSpacePositionVec3     = IN( mWorldPositionVec4 ).xyz;
        lTerrainOffsetVec3            = -lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz;

        #ifdef D_ASTEROID
        {
            lTerrainOffsetVec3 = vec3( 0.0, 0.0, 0.0 ); 
            lTerrainSpacePositionVec3 = IN( mLocalPositionVec3 ).xyz;
        }
        #endif

        #if !defined( D_CACHE_COLOUR ) && !defined( D_CACHE_NORMAL ) && !defined( D_CACHE_MASK )
        float lfDistance     = length( lTerrainSpacePositionVec3 - lUniforms.mpPerFrame->gViewPositionVec3 );
        #else 
        float lfDistance     = 0.0;
        #endif

		float lfTileDistance = 0.0;
        float lfTileGap      = 0;
        bool  lbIsDetail     = false;

#ifndef D_ASTEROID

#if defined( D_READ_TEX_CACHE ) || defined( D_TESS_SHADERS_PRESENT )
        if( true ) // force fading between the first two stages 'cause that's all we cache
#else
        if( lfDistance < lUniforms.mpCustomPerMaterial->gTextureDistancesVec4[ 0 ] ) 
#endif
        {
            lfTextureSmallScaleFactor = lUniforms.mpCustomPerMaterial->gTextureScalesVec4[ 0 ];
            lfTextureLargeScaleFactor = lUniforms.mpCustomPerMaterial->gTextureScalesVec4[ 1 ];
            lfScaleFade               = lfDistance / lUniforms.mpCustomPerMaterial->gTextureDistancesVec4[ 0 ];
            lbIsDetail                = true;

            lfScaleFade = saturate( ( ( lfScaleFade - 0.5 ) * 2.0 ) + 0.5 ); // this saves 0.5 ms (ps4 base) by reducing bandwidth
        }
        else if( lfDistance < lUniforms.mpCustomPerMaterial->gTextureDistancesVec4[ 1 ] )
        {
            lfTextureSmallScaleFactor = lUniforms.mpCustomPerMaterial->gTextureScalesVec4[ 1 ];
            lfTextureLargeScaleFactor = lUniforms.mpCustomPerMaterial->gTextureScalesVec4[ 2 ];
            lfScaleFade               = lfDistance - lUniforms.mpCustomPerMaterial->gTextureDistancesVec4[ 0 ];
            lfScaleFade              /= lUniforms.mpCustomPerMaterial->gTextureDistancesVec4[ 1 ] - lUniforms.mpCustomPerMaterial->gTextureDistancesVec4[ 0 ];
        }
        else if( lfDistance < lUniforms.mpCustomPerMaterial->gTextureDistancesVec4[ 2 ] )
        {
            lfTextureSmallScaleFactor = lUniforms.mpCustomPerMaterial->gTextureScalesVec4[ 2 ];
            lfTextureLargeScaleFactor = lUniforms.mpCustomPerMaterial->gTextureScalesVec4[ 3 ];
            lfScaleFade               = lfDistance - lUniforms.mpCustomPerMaterial->gTextureDistancesVec4[ 1 ];
            lfScaleFade              /= lUniforms.mpCustomPerMaterial->gTextureDistancesVec4[ 2 ] - lUniforms.mpCustomPerMaterial->gTextureDistancesVec4[ 1 ];
        }
        else if( lfDistance < lUniforms.mpCustomPerMaterial->gTextureDistancesVec4[ 3 ] )
        {
            lfTextureSmallScaleFactor = lUniforms.mpCustomPerMaterial->gTextureScalesVec4[ 3 ];
            lfTextureLargeScaleFactor = lUniforms.mpCustomPerMaterial->gTextureScales2Vec4[ 0 ];
            lfScaleFade               = lfDistance - lUniforms.mpCustomPerMaterial->gTextureDistancesVec4[ 2 ];
            lfScaleFade              /= lUniforms.mpCustomPerMaterial->gTextureDistancesVec4[ 3 ] - lUniforms.mpCustomPerMaterial->gTextureDistancesVec4[ 2 ];
        }
        else if( lfDistance < lUniforms.mpCustomPerMaterial->gTextureDistances2Vec4[ 0 ] )
        {
            lfTextureSmallScaleFactor = lUniforms.mpCustomPerMaterial->gTextureScales2Vec4[ 0 ];
            lfTextureLargeScaleFactor = lUniforms.mpCustomPerMaterial->gTextureScales2Vec4[ 1 ];
            lfScaleFade               = lfDistance - lUniforms.mpCustomPerMaterial->gTextureDistancesVec4[ 3 ];
            lfScaleFade              /= lUniforms.mpCustomPerMaterial->gTextureDistances2Vec4[ 0 ] - lUniforms.mpCustomPerMaterial->gTextureDistancesVec4[ 3 ];
        }
        else if( lfDistance < lUniforms.mpCustomPerMaterial->gTextureDistances2Vec4[ 1 ] )
        {
            lfTextureSmallScaleFactor = lUniforms.mpCustomPerMaterial->gTextureScales2Vec4[ 1 ];
            lfTextureLargeScaleFactor = lUniforms.mpCustomPerMaterial->gTextureScales2Vec4[ 2 ];
            lfScaleFade               = lfDistance - lUniforms.mpCustomPerMaterial->gTextureDistances2Vec4[ 0 ];
            lfScaleFade              /= lUniforms.mpCustomPerMaterial->gTextureDistances2Vec4[ 1 ] - lUniforms.mpCustomPerMaterial->gTextureDistances2Vec4[ 0 ];
        }
        else if( lfDistance < lUniforms.mpCustomPerMaterial->gTextureDistances2Vec4[ 2 ] )
        {
            lfTextureSmallScaleFactor = lUniforms.mpCustomPerMaterial->gTextureScales2Vec4[ 2 ];
            lfTextureLargeScaleFactor = lUniforms.mpCustomPerMaterial->gTextureScales2Vec4[ 3 ];
            lfScaleFade               = lfDistance - lUniforms.mpCustomPerMaterial->gTextureDistances2Vec4[ 1 ];
            lfScaleFade              /= lUniforms.mpCustomPerMaterial->gTextureDistances2Vec4[ 2 ] - lUniforms.mpCustomPerMaterial->gTextureDistances2Vec4[ 1 ];
        }
        else
        {
            lfTextureSmallScaleFactor = lUniforms.mpCustomPerMaterial->gTextureScales2Vec4[ 3 ];
            lfTextureLargeScaleFactor = lUniforms.mpCustomPerMaterial->gTextureScales2Vec4[ 3 ] * 2.0;
            lfScaleFade               = lfDistance - lUniforms.mpCustomPerMaterial->gTextureDistances2Vec4[ 2 ];
            lfScaleFade              /= lUniforms.mpCustomPerMaterial->gTextureDistances2Vec4[ 3 ] - lUniforms.mpCustomPerMaterial->gTextureDistances2Vec4[ 2 ];
            lfScaleFade               = min( lfScaleFade, 0.5 );
        }

        lfScaleFade = saturate( lfScaleFade );

        lfTextureSmallScaleFactor = 1.0 / lfTextureSmallScaleFactor;
        lfTextureLargeScaleFactor = 1.0 / lfTextureLargeScaleFactor;

#else

    lfTextureSmallScaleFactor = 1.0 / 256.0;
    lfTextureLargeScaleFactor = 1.0 / 256.0;
    lfScaleFade               = 0.0;

#endif
    
#ifndef D_READ_TEX_CACHE

        uvec4  lTileTextureIndicesSmall1Vec4;
        uvec4  lTileTextureIndicesSmall2Vec4;
        uvec4  lTileTextureIndicesLarge1Vec4;
        uvec4  lTileTextureIndicesLarge2Vec4;

        vec3 lUpDirection = normalize( lTerrainSpacePositionVec3 + lTerrainOffsetVec3 );



        // lTileVec2 sometimes comes in negative which crashes PS4. Should be fixed
        // on CPU wherever it gets passed in from.
        lTileTextureIndicesLarge1Vec4.x = uint( round( mod( max( IN( mTileVec4 ).x, 0.0 ), D_TERRAINCOLOURARRAY_SIZE ) ) );
        lTileTextureIndicesLarge2Vec4.x = uint( round( mod( max( IN( mTileVec4 ).y, 0.0 ), D_TERRAINCOLOURARRAY_SIZE ) ) );

        uint luSlopeIndex1 = min( lTileTextureIndicesLarge1Vec4.x / 4, 1 );
        uint luSlopeIndex2 = min( lTileTextureIndicesLarge2Vec4.x / 4, 1 );


        lTileTextureIndicesLarge1Vec4.y = lTileTextureIndicesLarge1Vec4.x + 2;
        lTileTextureIndicesLarge1Vec4.z = lTileTextureIndicesLarge1Vec4.x + 1;
        lTileTextureIndicesLarge1Vec4.w = lTileTextureIndicesLarge1Vec4.y + 1;
        lTileTextureIndicesSmall1Vec4   = lTileTextureIndicesLarge1Vec4;

        if( lTileTextureIndicesLarge1Vec4.x >= 20 )
        {
            // Substances read from a second atlas, don't have slope textures
            lTileTextureIndicesLarge1Vec4.y = lTileTextureIndicesLarge1Vec4.x;
            lTileTextureIndicesLarge1Vec4.z = lTileTextureIndicesLarge1Vec4.x;
            lTileTextureIndicesLarge1Vec4.w = lTileTextureIndicesLarge1Vec4.x;
            lTileTextureIndicesSmall1Vec4.y = lTileTextureIndicesLarge1Vec4.x;
            lTileTextureIndicesSmall1Vec4.z = lTileTextureIndicesLarge1Vec4.x;
            lTileTextureIndicesSmall1Vec4.w = lTileTextureIndicesLarge1Vec4.x;
        }
        else if( lTileTextureIndicesLarge1Vec4.x >= 16 )
        {
            lTileTextureIndicesLarge1Vec4.y = lTileTextureIndicesLarge1Vec4.x;
            lTileTextureIndicesLarge1Vec4.w = lTileTextureIndicesLarge1Vec4.z;
            lTileTextureIndicesSmall1Vec4.y = lTileTextureIndicesLarge1Vec4.x;
            lTileTextureIndicesSmall1Vec4.w = lTileTextureIndicesLarge1Vec4.z;
        }
        else if( lbIsDetail )
        {
            // The nearest textures read from detail textures in the second half of the atlas
            lTileTextureIndicesSmall1Vec4.x += 8;
            lTileTextureIndicesSmall1Vec4.y += 8;
            lTileTextureIndicesSmall1Vec4.z += 8;
            lTileTextureIndicesSmall1Vec4.w += 8;
        }


        lTileTextureIndicesLarge2Vec4.y = lTileTextureIndicesLarge2Vec4.x + 2;
        lTileTextureIndicesLarge2Vec4.z = lTileTextureIndicesLarge2Vec4.x + 1;
        lTileTextureIndicesLarge2Vec4.w = lTileTextureIndicesLarge2Vec4.y + 1;
        lTileTextureIndicesSmall2Vec4   = lTileTextureIndicesLarge2Vec4;

        if( lTileTextureIndicesLarge2Vec4.x >= 20 )
        {
            lTileTextureIndicesLarge2Vec4.y = lTileTextureIndicesLarge2Vec4.x;
            lTileTextureIndicesLarge2Vec4.z = lTileTextureIndicesLarge2Vec4.x;
            lTileTextureIndicesLarge2Vec4.w = lTileTextureIndicesLarge2Vec4.x;
            lTileTextureIndicesSmall2Vec4.y = lTileTextureIndicesLarge2Vec4.x;
            lTileTextureIndicesSmall2Vec4.z = lTileTextureIndicesLarge2Vec4.x;
            lTileTextureIndicesSmall2Vec4.w = lTileTextureIndicesLarge2Vec4.x;
        }
        else if( lTileTextureIndicesLarge2Vec4.x >= 16 )
        {
            lTileTextureIndicesLarge2Vec4.y = lTileTextureIndicesLarge2Vec4.x;
            lTileTextureIndicesLarge2Vec4.w = lTileTextureIndicesLarge2Vec4.z;
            lTileTextureIndicesSmall2Vec4.y = lTileTextureIndicesLarge2Vec4.x;
            lTileTextureIndicesSmall2Vec4.w = lTileTextureIndicesLarge2Vec4.z;
        }
        else if( lbIsDetail )
        {
            lTileTextureIndicesSmall2Vec4.x += 8;
            lTileTextureIndicesSmall2Vec4.y += 8;
            lTileTextureIndicesSmall2Vec4.z += 8;
            lTileTextureIndicesSmall2Vec4.w += 8;
        }

        // Tile blend
        float lfTile = IN( mTileVec4 ).z;

        // Patch noise
        vec4  lfNoiseSample  = texture3DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gPerlin3D ), ( lTerrainSpacePositionVec3 + lTerrainOffsetVec3 ) * lUniforms.mpCustomPerMaterial->gTileBlendScalesVec4.x, 0.0 );
        float lfPatch1       = saturate( ( ( lfNoiseSample.r + lfNoiseSample.g + lfNoiseSample.b + lfNoiseSample.a ) / 4.0 ) ) * 2.0 - 1.0;

        lfNoiseSample        = texture3DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gPerlin3D ), ( lTerrainSpacePositionVec3 + lTerrainOffsetVec3 ) * lUniforms.mpCustomPerMaterial->gTileBlendScalesVec4.y, 0.0 );
        float lfPatch2       = saturate( ( ( lfNoiseSample.r + lfNoiseSample.g + lfNoiseSample.b + lfNoiseSample.a ) / 4.0 ) ) * 2.0 - 1.0;

        float lfPatch        = lfPatch1 + lfPatch2 * lUniforms.mpCustomPerMaterial->gTileBlendScalesVec4.z;
        lfPatch              = 1.0 - saturate( lfPatch * 0.5 + 0.5 + lUniforms.mpCustomPerMaterial->gTileBlendScalesVec4.w );

        // Blend to different texture on the slope
        //Skew the flat towards the top slightly, to show more slope [-1 : 1] -> [-0.8 : 1] -> Clamped to 0
        float lfSlope  = 0.0;
        float lfSlope1 = 0.0;
        float lfSlope2 = 0.0;
        lfSlope        = max( dot( lUpDirection, lTerrainSpaceNormalVec3 ), 0.0 );
        lfSlope        = asin( lfSlope ) / ( 3.1415 * 0.5 );

        lfSlope1       = pow( lfSlope, lUniforms.mpCustomPerMaterial->gTerrainSlopeParamsVec4[ luSlopeIndex1 ] );
        lfSlope1      += lUniforms.mpCustomPerMaterial->gTerrainSlopeParamsVec4[ luSlopeIndex1 + 2 ];
        lfSlope1       = saturate( lfSlope1 );

        lfSlope2       = pow( lfSlope, lUniforms.mpCustomPerMaterial->gTerrainSlopeParamsVec4[ luSlopeIndex2 ] );
        lfSlope2      += lUniforms.mpCustomPerMaterial->gTerrainSlopeParamsVec4[ luSlopeIndex2 + 2 ];
        lfSlope2       = saturate( lfSlope2 );

        if( lTileTextureIndicesLarge1Vec4.x >= 16 )
        {
            lfSlope1 = lfSlope * lfSlope;
        }
        if( lTileTextureIndicesLarge2Vec4.x >= 16 )
        {
            lfSlope2 = lfSlope * lfSlope;
        }

        #if !defined( D_CACHE_COLOUR ) && !defined( D_CACHE_NORMAL ) && !defined( D_CACHE_MASK )
        float lfCameraHeight = length( lUniforms.mpPerFrame->gViewPositionVec3 - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz ) - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w;
        lfCameraHeight = 1.0 - saturate( ( lfCameraHeight - 1000.0 ) / 1000.0 );

        lfPatch *= lfCameraHeight;
        lfSlope1  = mix( 1.0, lfSlope1, lfCameraHeight );
        lfSlope2  = mix( 1.0, lfSlope2, lfCameraHeight );
        #endif



#ifdef D_ASTEROID
        lfPatch = 0.0;
        lfSlope = 1.0;
        lTileTextureIndicesSmall1Vec4 = uvec4( 0, 0, 0, 0 );
        lTileTextureIndicesSmall2Vec4 = uvec4( 0, 0, 0, 0 );
        lTileTextureIndicesLarge1Vec4 = uvec4( 0, 0, 0, 0 );
        lTileTextureIndicesLarge2Vec4 = uvec4( 0, 0, 0, 0 );
#endif

#ifdef D_CACHE_FAR
        lfScaleFade = 1.0f;
#endif

        lFragmentColourVec3 = GetTileColourAndNormal( 
            DEREF_PTR( lUniforms.mpCustomPerMaterial ),
            lTerrainSpaceNormalVec3,
            lTerrainSpaceFaceNormalVec3,
            lTerrainOffsetVec3,
            lTerrainSpacePositionVec3,
            lTileTextureIndicesSmall1Vec4,
            lTileTextureIndicesSmall2Vec4,
            lTileTextureIndicesLarge1Vec4,
            lTileTextureIndicesLarge2Vec4,
            lfPatch,
            lfSlope1,
            lfSlope2,
            lfTile,
            lNormalVec3,
            lfTextureSmallScaleFactor, 
            lfTextureLargeScaleFactor,
            lfScaleFade,
            SAMPLER2DARRAYPARAM_SRT( lUniforms.mpCustomPerMaterial,gDiffuseMap ),
            SAMPLER2DARRAYPARAM_SRT( lUniforms.mpCustomPerMaterial,gNormalMap ),
            SAMPLER2DARRAYPARAM_SRT( lUniforms.mpCustomPerMaterial,gSubstanceDiffuseMap ),
            SAMPLER2DARRAYPARAM_SRT( lUniforms.mpCustomPerMaterial,gSubstanceNormalMap ),
            lfSpecular,
            lfSubsurface,
            lfMetallic,
            lfGlow,
            lfHeight );
#endif  // D_READ_TEX_CACHE

#ifdef D_READ_TEX_CACHE
        lNormalVec3 = lTerrainSpaceNormalVec3;
#endif

#if defined( D_ASTEROID )	
        mat3 lNormalisedMat3;
        MAT3_SET_COLUMN( lNormalisedMat3, 0, normalize( lUniforms.mpCommonPerMesh->gWorldMat4[0].xyz ) );
        MAT3_SET_COLUMN( lNormalisedMat3, 1, normalize( lUniforms.mpCommonPerMesh->gWorldMat4[1].xyz ) );
        MAT3_SET_COLUMN( lNormalisedMat3, 2, normalize( lUniforms.mpCommonPerMesh->gWorldMat4[2].xyz ) );
        
        lNormalVec3 = normalize( MUL( lNormalisedMat3 , lNormalVec3 ) );
#endif

    }
    
        float lfRoughness  = lfSpecular * lUniforms.mpCustomPerMaterial->gMaterialParamsVec4.x;



#if defined( D_READ_TEX_CACHE ) || defined( D_WRITE_CACHE_FEEDBACK )
    {
        ivec2 lResolution = ivec2( 32, 32 );

        vec2 lCacheCoords01 = IN( mTexCoordVec2_mTexBorderVec2 ).xy;

        #ifndef D_PLATFORM_OPENGL
        lCacheCoords01.y = 1.0 - lCacheCoords01.y;
        #endif

        vec2 lfDerivX = dFdx( lCacheCoords01.xy );
        vec2 lfDerivY = dFdy( lCacheCoords01.xy );

        vec4 lSparseStatus = lUniforms.mpCustomPerMesh->gSparseTextureStatusVec4;
        float lfMipBias = lSparseStatus[2];
        float lfAniso   = lSparseStatus[3];

        float Px = sqrt( dot( lfDerivX, lfDerivX ) ) * lResolution.x * 256.0;
        float Py = sqrt( dot( lfDerivY, lfDerivY ) ) * lResolution.y * 256.0;

        float Pmax = max(Px,Py);
        float Pmin = min(Px,Py);

        float N = min( ceil(Pmax/Pmin), lfAniso );

        float lfBaseMip = log2( Pmax/N );
        lfBaseMip -= min( 1.0, lfBaseMip * 0.25 ); // compensate for lost resolution in high mips due to gutters

        float lfDesiredMip = max( 0.0, min( 5.0, lfBaseMip + lfMipBias ) );

        uint liWriteSparse0 = uint( lSparseStatus[0] );
        float lfFadeThreshold        = 1.0 / 100.0;

        if( lfScaleFade < 1.0 - lfFadeThreshold )
        {
            #ifdef D_WRITE_CACHE_FEEDBACK
            uint liDiffIndex = liWriteSparse0 & 0xff;

            if( liDiffIndex != 255 )
            {
                WriteTextureCacheFeedback( 
                    lCacheCoords01.xy,
                    lResolution,
                    max( 0.0, lfDesiredMip - 0.25 ),
                    IMAGE_GETMAP( lUniforms.mpCustomPerMesh, gFeedbackMap ),
                    liDiffIndex );
            }
            #endif
        }
        else
        {
            lfScaleFade = 1.0;
        }

        if( lfScaleFade > lfFadeThreshold )
        {
            #ifdef D_WRITE_CACHE_FEEDBACK
            uint liDiffIndex = ( liWriteSparse0 >> 8 ) & 0xff;

            if( liDiffIndex != 255 )
            {
                WriteTextureCacheFeedback( 
                    lCacheCoords01.xy,
                    lResolution,
                    max( 0.0, lfDesiredMip - 0.25 ),
                    IMAGE_GETMAP( lUniforms.mpCustomPerMesh, gFeedbackMap ),
                    liDiffIndex );
            }
            #endif
        }
        else
        {
            lfScaleFade = 0.0;
        }


        vec3 lFragmentColourNearVec3 = vec3( 0.0, 0.0, 0.0 );
        vec4 lMasksNearVec4 = vec4( 0.0, 0.0, 0.0, 0.0 );
        vec3 lNormalNearVec3 = vec3( 0.0, 0.0, 0.0 );

        vec3 lFragmentColourFarVec3 = vec3( 0.0, 0.0, 0.0 );
        vec4 lMasksFarVec4 = vec4( 0.0, 0.0, 0.0, 0.0 );
        vec3 lNormalFarVec3  = vec3( 0.0, 0.0, 0.0 );

        float lfNearHeight;
        float lfFarHeight;

    #ifdef D_READ_TEX_CACHE
        #if defined( D_TESS_SHADERS_PRESENT ) || defined( D_PLATFORM_PC )
            vec2 lvBorder = vec2( IN ( mTexCoordVec2_mTexBorderVec2 ).zw );
            #ifndef D_PLATFORM_OPENGL
            lvBorder.y = -lvBorder.y;
            #endif
        #else
            vec2 lvBorder = vec2( 0,0 );
        #endif

        if( lfScaleFade < 1.0 - lfFadeThreshold )
        {
            ReadTextureCache( 
                lCacheCoords01,
                lvBorder,
                lfDerivX,
                lfDerivY,
                lResolution,
                lfDesiredMip,
                SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh, gSparseNearDiffMap ),
                SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh, gSparseNearNormMap ),
                SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh, gSparseNearStatusMap ),
                SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh, gSparseNearStatusMap ),
                lFragmentColourNearVec3,
                lNormalNearVec3,
                lMasksNearVec4,
                lfNearHeight );
        }

        if( lfScaleFade > lfFadeThreshold )
        {
            ReadTextureCache( 
                lCacheCoords01,
                lvBorder,
                lfDerivX,
                lfDerivY,
                lResolution,
                lfDesiredMip,
                SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh, gSparseFarDiffMap ),
                SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh, gSparseFarNormMap ),
                SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh, gSparseFarStatusMap ),
                SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh, gSparseFarStatusMap ),
                lFragmentColourFarVec3,
                lNormalFarVec3,
                lMasksFarVec4,
                lfFarHeight );
        }

        lFragmentColourVec3 = mix( lFragmentColourNearVec3, lFragmentColourFarVec3, lfScaleFade );
        lFragmentColourVec3 = GammaCorrectInput( lFragmentColourVec3 );
        
        vec4 lMasksVec4 = mix( lMasksNearVec4, lMasksFarVec4, lfScaleFade );

        #if !defined( D_TESS_SHADERS_PRESENT ) && !defined( D_PLATFORM_PC )
        // can't get the texcoord derivative from the hull shader, must compute it ourselves

        #if 0 // def GL_AMD_shader_explicit_vertex_parameter

        // on AMD can use the simple subtract from the hull shader
        vec3 dPdU;

        if( (interpolateAtVertexAMD( mTexCoordVec2_mfDistForFade, 2 ).x - interpolateAtVertexAMD( mTexCoordVec2_mfDistForFade, 1 ).x) == 0)
        {
           dPdU = interpolateAtVertexAMD( mWorldPositionVec4, 2 ).xyz - interpolateAtVertexAMD( mWorldPositionVec4, 0 ).xyz;
           dPdU *= sign( interpolateAtVertexAMD( mTexCoordVec2_mfDistForFade, 2 ).x - interpolateAtVertexAMD( mTexCoordVec2_mfDistForFade, 0 ).x );
        }
        else
        {
           dPdU = interpolateAtVertexAMD( mWorldPositionVec4, 2 ).xyz - interpolateAtVertexAMD( mWorldPositionVec4, 1 ).xyz;
           dPdU *= sign( interpolateAtVertexAMD( mTexCoordVec2_mfDistForFade, 2 ).x - interpolateAtVertexAMD( mTexCoordVec2_mfDistForFade, 1 ).x );
        }
        dPdU = normalize( dPdU );

        #else

        // don't have the AMD feature, will nead to do some chain-rule acrobatics

        vec3 dPdX = dFdxFine( lTerrainSpacePositionVec3 );
        vec3 dPdY = dFdyFine( lTerrainSpacePositionVec3 );

        float dUdX = dFdxFine( lCacheCoords01.x );
        float dUdY = dFdyFine( lCacheCoords01.x );
        float dVdX = dFdxFine( lCacheCoords01.y );
        float dVdY = dFdyFine( lCacheCoords01.y );

        mat2 derivs;
        derivs[0][0] = dUdX;
        derivs[0][1] = dUdY;
        derivs[1][0] = dVdX;
        derivs[1][1] = dVdY;

        derivs = Inverse( derivs );

        float dXdU = derivs[0][0];
        float dYdU = derivs[1][0];

        float dXdV = derivs[0][1];
        float dYdV = derivs[1][1];

        vec3 dPdU = dPdX * dXdU + dPdY * dYdU;
        dPdU = normalize( dPdU );

        #endif
        vec3 dPdV = normalize( cross( lNormalVec3, dPdU ) );

        #else
        vec3 dPdU = IN( mTexCoordsDPDUVec4 ).xyz;
        vec3 dPdV = IN( mTexCoordsDPDVVec4 ).xyz;

        #endif

        lNormalNearVec3 *= (1.0 - lfScaleFade);
        lNormalFarVec3  *= lfScaleFade;
            
        lNormalNearVec3 = vec3( lNormalNearVec3.x + lNormalFarVec3.x, lNormalNearVec3.y + lNormalFarVec3.y, lNormalNearVec3.z + lNormalFarVec3.z );

        lNormalNearVec3 = lNormalNearVec3.x * dPdU + lNormalNearVec3.y * dPdV;
        lNormalVec3 = normalize( lNormalVec3 + lNormalNearVec3 );

        lfMetallic   = lMasksVec4.x;
        lfRoughness  = lMasksVec4.y;
        lfSubsurface = 0.0; // lMasksVec4.z;
        lfGlow       = 0.0; // lMasksVec4.w;
    #endif // D_READ_TEX_CACHE
    }
#endif

    // vec2 lCenter = GetViVjPerspCenter();
    // float lfW = 1.0 - lCenter.x - lCenter.y;
    // if( lCenter.x < (lUniforms.mpPerFrame->gTessSettingsVec4.w) || lCenter.y < (lUniforms.mpPerFrame->gTessSettingsVec4.w) || lfW < (lUniforms.mpPerFrame->gTessSettingsVec4.w) )
    // {
    //     lFragmentColourVec3 = vec3( 1 );
    // }

    //-----------------------------------------------------------------------------
    ///
    ///     Distant Water Colouring
    ///
    //-----------------------------------------------------------------------------
    #if !defined( D_CACHE_COLOUR ) && !defined( D_CACHE_NORMAL ) && !defined( D_CACHE_MASK )

    #ifndef D_ASTEROID
    float lfWaterFade = IN( mSmoothNormalVec3_mfDistForFade ).w - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w;
    if ( lfWaterFade > 500.0 )
    {
        vec3 lFaceNormalVec3 = normalize( IN( mWorldPositionVec4 ).xyz - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz );

        lfWaterFade       = clamp( ( lfWaterFade - 700.0 ) / 200.0, 0.0, 1.0 );
        lfWaterFade      *= ( 1.0 - IN( mTileVec4 ).w );

        //lfWaterFade      *= smoothstep( 0.9, 1.0, dot( lNormalVec3, lFaceNormalVec3 ) );// * 0.3 + 0.7;

        lNormalVec3             = normalize( mix( lNormalVec3, lFaceNormalVec3, max( lfWaterFade - 0.1, 0.0 ) ) );


        vec3 lLightColourVec3  = mix( /*GammaCorrectInput*/( lUniforms.mpCommonPerMesh->gLightColourVec4.xyz ), vec3( 1.0,1.0,1.0 ), lfWaterFade );

        lFragmentColourVec3 = mix( lFragmentColourVec3, /*GammaCorrectInput*/( lUniforms.mpCustomPerMaterial->gWaterFogColourFarVec4.xyz ), lfWaterFade );
        lfRoughness         = mix( lfRoughness, 0.35 * lUniforms.mpCustomPerMaterial->gMaterialParamsVec4.x, lfWaterFade );
    }
    #endif

    #if defined( D_REFLECT_DUALP ) || defined( D_REFLECT_WATER ) || defined( D_REFLECT_WATER_UP )
    {
        lFragmentColourVec3 *= ( lUniforms.mpCommonPerMesh->gLightColourVec4.xyz );
    }
    #endif

    #endif

    //-----------------------------------------------------------------------------
    ///
    ///		Output
    ///
    //-----------------------------------------------------------------------------

  
    {
        int liMaterialID = D_DETAILOVERLAY;

#ifdef _F10_NORECEIVESHADOW
        liMaterialID |= D_NORECEIVESHADOW;
#else       

#ifdef D_FADE
        //liMaterialID |= D_EMBEDDED_SHADOW;
        //vec2 lFragCoordsVec2 = ( IN( mScreenSpacePositionVec4 ).xy / IN( mScreenSpacePositionVec4 ).w ) * 0.5 + 0.5;
        //lfSubsurface = ComputeShadowIntensity( DEREF_PTR( lUniforms.mpCustomPerMaterial ), DEREF_PTR( lUniforms.mpPerFrame ),DEREF_PTR( lUniforms.mpCommonPerMesh ), IN(mWorldPositionVec4).xyz, lNormalVec3, lFragCoordsVec2, true );
#endif

#endif


#ifdef D_NO_WATER
        liMaterialID |= D_NOWATER;
#endif
        
/*
        if( lUniforms.mpCustomPerMesh->gTerrainLodParamsVec4.w == 0.0 )
        {
            lFragmentColourVec3 = vec3( 1.0, 0.0, 0.0 );
        }
        else if( lUniforms.mpCustomPerMesh->gTerrainLodParamsVec4.w == 1.0 )
        {
            lFragmentColourVec3 = vec3( 1.0, 1.0, 0.0 );
        }
        else if( lUniforms.mpCustomPerMesh->gTerrainLodParamsVec4.w == 2.0 )
        {
            lFragmentColourVec3 = vec3( 0.0, 1.0, 0.0 );
        }
        else if( lUniforms.mpCustomPerMesh->gTerrainLodParamsVec4.w == 3.0 )
        {
            lFragmentColourVec3 = vec3( 0.0, 1.0, 1.0 );
        }
        else if( lUniforms.mpCustomPerMesh->gTerrainLodParamsVec4.w == 4.0 )
        {
            lFragmentColourVec3 = vec3( 0.0, 0.0, 1.0 );
        }
        else if( lUniforms.mpCustomPerMesh->gTerrainLodParamsVec4.w == 5.0 )
        {
            lFragmentColourVec3 = vec3( 1.0, 0.0, 1.0 );
        }
        else
        {
            lFragmentColourVec3 = vec3( 1.0, 1.0, 1.0 );
        }
*/

        vec4 lOutColours0Vec4;
        vec4 lOutColours1Vec4;
        vec4 lOutColours2Vec4;
        vec4 lOutColours3Vec4;
        vec4 lOutColours4Vec4;



#if defined( D_CACHE_COLOUR )

        #ifdef D_CACHE_HEIGHT
        lOutColours0Vec4 = vec4( 0.0, 0.0, 0.0, lfHeight );
        #else
        lOutColours0Vec4 = vec4( lFragmentColourVec3, 0.0 );
        #endif

#elif  defined( D_CACHE_NORMAL )

        #if !defined( D_TESS_SHADERS_PRESENT ) && !defined( D_PLATFORM_PC )
        float dPdUInvLen = 1.0;
        float dPdVInvLen = 1.0;
        vec3 dPdU = normalize( dFdx( lTerrainSpacePositionVec3 ) );
        vec3 dPdV = normalize( cross( lTerrainSpaceNormalVec3, dPdU ) );
        #else
        vec3 dPdU = IN( mTexCoordsDPDUVec4 ).xyz;
        vec3 dPdV = IN( mTexCoordsDPDVVec4 ).xyz;
        float dPdUInvLen = invsqrt( dot( dPdU, dPdU ) );
        float dPdVInvLen = invsqrt( dot( dPdV, dPdV ) );
        dPdU *= dPdUInvLen;
        dPdV *= dPdVInvLen;
        #endif

        vec2 lEncDerivVec2 = vec2( dot( lNormalVec3, dPdU ), dot( lNormalVec3, dPdV ) );
        lEncDerivVec2.x *= dPdUInvLen;
        lEncDerivVec2.y *= dPdVInvLen;
        //lEncDerivVec2 *= length( IN( mSmoothNormalVec3 ).xyz );  // this removes a normalise() from the fragment side
        lEncDerivVec2 = sign( lEncDerivVec2 ) * sqrt( abs( lEncDerivVec2 ) * 0.25 );
        lEncDerivVec2 = lEncDerivVec2 * 0.5 + vec2( 0.5, 0.5 );

        float lfRoughnessEncoded = sqrt( lfRoughness * ( 1.0 / 2.0 ) );

        lOutColours0Vec4.r = lEncDerivVec2.x;
        lOutColours0Vec4.g = lfMetallic;
        lOutColours0Vec4.b = lfRoughnessEncoded;
        lOutColours0Vec4.a = lEncDerivVec2.y;

#elif  defined( D_CACHE_MASK )
        lOutColours0Vec4 = vec4( lfMetallic, lfRoughness, lfSubsurface, lfGlow );
#else

       // lFragmentColourVec3 = vec3( 1.0, 0.0, 0.0 );

        WriteOutput(
            lOutColours0Vec4,
            lOutColours1Vec4,
            lOutColours2Vec4,
            lOutColours3Vec4,
            lOutColours4Vec4,
            DEREF_PTR( lUniforms.mpPerFrame ),
            DEREF_PTR( lUniforms.mpCommonPerMesh ),
            DEREF_PTR( lUniforms.mpCustomPerMaterial ),
            vec4( lFragmentColourVec3, 1.0 ),
            IN( mWorldPositionVec4 ).xyz,
            lNormalVec3,
            liMaterialID,
            lfMetallic,
            lfRoughness,
            lfSubsurface,
            lfGlow,
#if defined( D_OUTPUT_MOTION_VECTORS )
            IN( mScreenSpacePositionVec4 ).xyzw,
            IN( mScreenSpacePositionVec4 ).xyzw,
#else
            float2vec4(0.0),
            float2vec4(0.0),
#endif
#if !defined( D_ATTRIBUTES ) && !defined( _F07_UNLIT )
            GetInverseWorldUpTransform(lUniforms.mpPerFrame->gViewPositionVec3, lUniforms.mpCommonPerMesh->gLightOriginVec4),
            SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMaterial, gDualPMapBack),
            SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMaterial, gDualPMapFront),
#endif
            0.0,
            false
            );

#endif

        /*
        if ( lUniforms.mpCustomPerMesh->gTerrainLodParamsVec4.w == 0.25 )
        {
            lOutColours0Vec4 = vec4( 1.0, 0.0, 0.0, 0.0 );
        }
        else
        if ( lUniforms.mpCustomPerMesh->gTerrainLodParamsVec4.w == 0.5 )
        {
            lOutColours0Vec4 = vec4( 0.0, 1.0, 0.0, 0.0 );
        }
        */
        //lOutColours0Vec4 = vec4( lUniforms.mpCustomPerMesh->gTerrainLodParamsVec4.xyz, 0.0 );

#if !defined( D_ATTRIBUTES )
        FRAGMENT_COLOUR  = lOutColours0Vec4;
#else
        FRAGMENT_COLOUR0 = lOutColours0Vec4;
        FRAGMENT_COLOUR1 = lOutColours1Vec4;
        FRAGMENT_COLOUR2 = lOutColours2Vec4;
        FRAGMENT_COLOUR3 = lOutColours3Vec4;


#endif

    }
#endif
    
}

