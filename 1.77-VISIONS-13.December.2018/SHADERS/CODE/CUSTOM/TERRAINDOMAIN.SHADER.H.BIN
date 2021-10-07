////////////////////////////////////////////////////////////////////////////////
///
///     @file       TerrainVertex.h
///     @author     User
///     @date       
///
///     @brief      TerrainVertexShader
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 

#if !defined(D_WATER)
#define D_TERRAIN
#endif


#define D_OCTAHEDRON_NORMALS

#if !defined( D_REFLECT_WATER_UP ) && !defined( D_REFLECT_WATER ) && !defined( D_REFLECT_DUALP ) && !defined( D_WRITE_TEX_CACHE )
    #define D_FADE
#endif
#if /*defined( D_ASTEROID ) ||*/ defined( D_REFLECT_WATER ) || defined( D_REFLECT_WATER_UP ) || defined( D_REFLECT_DUALP )
#define _F50_DISABLE_POSTPROCESS
#endif
//-----------------------------------------------------------------------------
//      Include files


#include "Common/Defines.shader.h"
#include "Common/Common.shader.h"
#include "Common/CommonUniforms.shader.h"



//
// In TerrainCommon we have our uniforms specific to terrain declared. PLUS, IMPORTANTLY we have the SRT buffer declared (gUniforms). 
// This MUST be included after CommonUniforms, but before all the other stuff that references gUniforms.
//
#ifdef D_WATER
    #include "Custom/WaterCommon.h"
#else
    #include "Custom/TerrainCommon.h"
#endif


#define D_TEXTURE_ARRAYS

//
// Have to include things that reference the global gUniforms under here. Things defined above may be parameters to functions in the following includes.
//
#include "Common/CommonVertex.shader.h"
#include "Common/CommonPlanet.shader.h"
#include "Common/CommonDepth.shader.h"
#include "Common/CommonFragment.shader.h"
#include "Common/CommonTriplanarTexturing.shader.h"

//-----------------------------------------------------------------------------
//      Global Data


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
    INPUT( vec4,   maWorldPositionVec4,              TEXCOORD0 )
#ifdef D_ASTEROID    
    INPUT( vec3,   maLocalPositionVec3,              TEXCOORD1 )
#endif
    INPUT( vec4,   maTileVec4,                       TEXCOORD2 )
    INPUT( vec4,   maTexCoordVec2_mTexBorderVec2,    TEXCOORD3 )
    INPUT( vec4,   maSmoothNormalVec3_mfDistForFade, TEXCOORD4 )
    INPUT( vec4,   maScreenSpacePositionVec4,        TEXCOORD5 )

#if  defined( D_WRITE_TEX_CACHE ) || defined( D_APPLY_DISPLACEMENT )
    INPUT( vec4,   maTessellationParamsVec4,         TEXCOORD6 )
#endif

#if defined( D_READ_TEX_CACHE ) || defined( D_WRITE_TEX_CACHE )
    INPUT( vec4,   maTexCoordsDPDUVec4,              TEXCOORD10 )
    INPUT( vec4,   maTexCoordsDPDVVec4,              TEXCOORD11 )
#endif
DECLARE_END

//-----------------------------------------------------------------------------
///
///     Output
///
///     @brief  Output
///
//-----------------------------------------------------------------------------
DECLARE_OUTPUT
    
    OUTPUT_SCREEN_POSITION
#if !defined(D_DEPTHONLY)
    OUTPUT( vec4,   mWorldPositionVec4,              TEXCOORD0 )
#ifdef D_ASTEROID    
    OUTPUT( vec3,   mLocalPositionVec3,              TEXCOORD1 )
#endif
    OUTPUT( vec4,   mTileVec4,                       TEXCOORD2 )
    OUTPUT( vec4,   mTexCoordVec2_mTexBorderVec2,    TEXCOORD3 )
    OUTPUT( vec4,   mSmoothNormalVec3_mfDistForFade, TEXCOORD4 )
#if ( defined( D_TESS_SHADERS_PRESENT ) || defined( D_PLATFORM_PC ) ) && ( defined( D_READ_TEX_CACHE ) || defined( D_WRITE_TEX_CACHE ) )
    OUTPUT( vec4,   mTexCoordsDPDUVec4,              TEXCOORD6 )
    OUTPUT( vec4,   mTexCoordsDPDVVec4,              TEXCOORD7 )
#endif
#endif
#if defined( D_OUTPUT_MOTION_VECTORS )
    OUTPUT( vec4,   mScreenSpacePositionVec4,        TEXCOORD5 )
#endif


DECLARE_END


//-----------------------------------------------------------------------------
//    Functions

//-----------------------------------------------------------------------------
///
///     Domain Main
///
///     @brief      Domain Main
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------


DECLARE_PATCH_INPUT_QUAD
    #ifdef D_TESS_ON_AMD
    PATCH_INPUT( vec4, mEdge0, TEXCOORD12 )
    PATCH_INPUT( vec4, mEdge1, TEXCOORD13 )
    PATCH_INPUT( vec4, mEdge2, TEXCOORD14 )
    PATCH_INPUT( vec4, mTexEdge, TEXCOORD15 )
    PATCH_INPUT( vec4, mTexCorner, TEXCOORD16 )
    #endif
    IN_PATCH_QUAD_TESS_CONSTANTS
DECLARE_END


DOMAIN_QUAD_MAIN_SRT
{
    float u0 = ( 1.0 - DOMAIN_COORDS.x ) * ( 1.0 - DOMAIN_COORDS.y );
    float u1 = (       DOMAIN_COORDS.x ) * ( 1.0 - DOMAIN_COORDS.y );
    float u2 = (       DOMAIN_COORDS.x ) * (       DOMAIN_COORDS.y );
    float u3 = ( 1.0 - DOMAIN_COORDS.x ) * (       DOMAIN_COORDS.y );
   
    #ifdef D_APPLY_DISPLACEMENT

    PRECISE vec4 lTessParams = INVARIANT(u0 * IN(maTessellationParamsVec4, 0) + u1 * IN(maTessellationParamsVec4, 1) + u2 * IN(maTessellationParamsVec4, 2) + u3 * IN(maTessellationParamsVec4, 3));

    // the base subdiv is a power-of-two -- this rounds the incoming coords
    // to the next lowest and next highest power-of-two
    float lfExpLow  = exp2( floor( lTessParams.x ) );
    float lfExpHigh = exp2( ceil ( lTessParams.x ) );

    vec2 lfLoDomain = round( DOMAIN_COORDS.xy * lfExpLow )  / lfExpLow; 
    vec2 lfHiDomain = round( DOMAIN_COORDS.xy * lfExpHigh ) / lfExpHigh; 

    // now recompute the low and high tex coords to get low and high tex coords
    float lfLoU0 = ( 1.0 - lfLoDomain.x ) * ( 1.0 - lfLoDomain.y );
    float lfLoU1 = (       lfLoDomain.x ) * ( 1.0 - lfLoDomain.y );
    float lfLoU2 = (       lfLoDomain.x ) * (       lfLoDomain.y );
    float lfLoU3 = ( 1.0 - lfLoDomain.x ) * (       lfLoDomain.y );

    float lfHiU0 = ( 1.0 - lfHiDomain.x ) * ( 1.0 - lfHiDomain.y );
    float lfHiU1 = (       lfHiDomain.x ) * ( 1.0 - lfHiDomain.y );
    float lfHiU2 = (       lfHiDomain.x ) * (       lfHiDomain.y );
    float lfHiU3 = ( 1.0 - lfHiDomain.x ) * (       lfHiDomain.y );

    PRECISE vec2 lCacheCoordsLo = INVARIANT( lfLoU0 * IN( maTexCoordVec2_mTexBorderVec2,   0 ) + lfLoU1 * IN( maTexCoordVec2_mTexBorderVec2,   1 ) + lfLoU2 * IN( maTexCoordVec2_mTexBorderVec2,   2 ) + lfLoU3 * IN( maTexCoordVec2_mTexBorderVec2,   3 ) ).xy;
    #ifndef D_PLATFORM_OPENGL
    lCacheCoordsLo.y = 1.0 - lCacheCoordsLo.y;
    #endif

    PRECISE vec2 lCacheCoordsHi = INVARIANT( lfHiU0 * IN( maTexCoordVec2_mTexBorderVec2,   0 ) + lfHiU1 * IN( maTexCoordVec2_mTexBorderVec2,   1 ) + lfHiU2 * IN( maTexCoordVec2_mTexBorderVec2,   2 ) + lfHiU3 * IN( maTexCoordVec2_mTexBorderVec2,   3 ) ).xy;
    #ifndef D_PLATFORM_OPENGL
    lCacheCoordsHi.y = 1.0 - lCacheCoordsHi.y;
    #endif

    // everything but the height tex reads uses a simple interpolated tex coord
    vec2 lfInterDomain = mix( lfLoDomain, lfHiDomain, fract( lTessParams.x ) );
    u0 = ( 1.0 - lfInterDomain.x ) * ( 1.0 - lfInterDomain.y );
    u1 = (       lfInterDomain.x ) * ( 1.0 - lfInterDomain.y );
    u2 = (       lfInterDomain.x ) * (       lfInterDomain.y );
    u3 = ( 1.0 - lfInterDomain.x ) * (       lfInterDomain.y );

    #endif

    // compute screenspace pos first so as not to have it expand due to border
    PRECISE vec4 lScreenSpacePositionVec4     = INVARIANT( u0 * IN( maScreenSpacePositionVec4,      0 ) + u1 * IN( maScreenSpacePositionVec4,      1 ) + u2 * IN( maScreenSpacePositionVec4,      2 ) + u3 * IN( maScreenSpacePositionVec4,      3 ) );
    PRECISE vec4 lTexCoordVec2_mTexBorderVec2 = INVARIANT( u0 * IN( maTexCoordVec2_mTexBorderVec2,   0 ) + u1 * IN( maTexCoordVec2_mTexBorderVec2,   1 ) + u2 * IN( maTexCoordVec2_mTexBorderVec2,   2 ) + u3 * IN( maTexCoordVec2_mTexBorderVec2,   3 ) );
#if defined( D_OUTPUT_MOTION_VECTORS )
    OUT( mScreenSpacePositionVec4      ) = lScreenSpacePositionVec4;
#endif

    #ifdef D_WRITE_TEX_CACHE

    #ifndef D_EXPAND_BORDER
    vec2 lfNewCoords = DOMAIN_COORDS.xy;

    if( DOMAIN_COORDS.x < 0.1 )       lTexCoordVec2_mTexBorderVec2.x = IN( maTexCoordVec2_mTexBorderVec2, 0 ).x + IN( maTessellationParamsVec4, 0 ).z * 3.0;
    else if( DOMAIN_COORDS.x < 0.49 ) lTexCoordVec2_mTexBorderVec2.x = IN( maTexCoordVec2_mTexBorderVec2, 0 ).x + IN( maTessellationParamsVec4, 0 ).z * 4.0;

    if( DOMAIN_COORDS.x > 0.9 )       lTexCoordVec2_mTexBorderVec2.x = IN( maTexCoordVec2_mTexBorderVec2, 1 ).x + IN( maTessellationParamsVec4, 1 ).z * 3.0;
    else if( DOMAIN_COORDS.x > 0.51 ) lTexCoordVec2_mTexBorderVec2.x = IN( maTexCoordVec2_mTexBorderVec2, 1 ).x + IN( maTessellationParamsVec4, 1 ).z * 4.0;

    if( DOMAIN_COORDS.y < 0.1 )       lTexCoordVec2_mTexBorderVec2.y = IN( maTexCoordVec2_mTexBorderVec2, 0 ).y + IN( maTessellationParamsVec4, 0 ).w * 3.0;
    else if( DOMAIN_COORDS.y < 0.49 ) lTexCoordVec2_mTexBorderVec2.y = IN( maTexCoordVec2_mTexBorderVec2, 0 ).y + IN( maTessellationParamsVec4, 0 ).w * 4.0;

    if( DOMAIN_COORDS.y > 0.9 )       lTexCoordVec2_mTexBorderVec2.y = IN( maTexCoordVec2_mTexBorderVec2, 2 ).y + IN( maTessellationParamsVec4, 2 ).w * 3.0;
    else if( DOMAIN_COORDS.y > 0.51 ) lTexCoordVec2_mTexBorderVec2.y = IN( maTexCoordVec2_mTexBorderVec2, 2 ).y + IN( maTessellationParamsVec4, 2 ).w * 4.0;

    if( DOMAIN_COORDS.x < 0.49 ) lfNewCoords.x = 0.0;
    if( DOMAIN_COORDS.x > 0.51 ) lfNewCoords.x = 1.0;
    if( DOMAIN_COORDS.y < 0.49 ) lfNewCoords.y = 0.0;
    if( DOMAIN_COORDS.y > 0.51 ) lfNewCoords.y = 1.0;

    #endif

    // push through the tex coords before updating UVs for border
    vec2 lTexCoords = lTexCoordVec2_mTexBorderVec2.xy * lUniforms.mpCustomPerMesh->gTextureCoordsAdjustVec4.xy + lUniforms.mpCustomPerMesh->gTextureCoordsAdjustVec4.zw;    
    lScreenSpacePositionVec4 = vec4( lTexCoords * 2.0 - 1.0, 0.5, 1.0 );

    #ifdef D_PLATFORM_OPENGL
    lScreenSpacePositionVec4.y = -lScreenSpacePositionVec4.y;
    #endif

    // apply border
    #ifdef D_EXPAND_BORDER
    PRECISE vec2 lfNewCoords = INVARIANT( u0 * IN( maTessellationParamsVec4, 0 ).zw + u1 * IN( maTessellationParamsVec4, 1 ).zw + u2 * IN( maTessellationParamsVec4, 2 ).zw + u3 * IN( maTessellationParamsVec4, 3 ).zw );
    #endif

    u0 = ( 1.0 - lfNewCoords.x ) * ( 1.0 - lfNewCoords.y );
    u1 = (       lfNewCoords.x ) * ( 1.0 - lfNewCoords.y );
    u2 = (       lfNewCoords.x ) * (       lfNewCoords.y );
    u3 = ( 1.0 - lfNewCoords.x ) * (       lfNewCoords.y );

    // now recompute the tex coords with the border applied
    lTexCoordVec2_mTexBorderVec2 = INVARIANT( u0 * IN( maTexCoordVec2_mTexBorderVec2,   0 ) + u1 * IN( maTexCoordVec2_mTexBorderVec2,   1 ) + u2 * IN( maTexCoordVec2_mTexBorderVec2,   2 ) + u3 * IN( maTexCoordVec2_mTexBorderVec2,   3 ) );
    #endif

    PRECISE vec4 lWorldPositionVec4          = INVARIANT( u0 * IN( maWorldPositionVec4,              0 ) + u1 * IN( maWorldPositionVec4,              1 ) + u2 * IN( maWorldPositionVec4,              2 ) + u3 * IN( maWorldPositionVec4,              3 ) );
    PRECISE vec4 lTileVec4                   = INVARIANT( u0 * IN( maTileVec4,                       0 ) + u1 * IN( maTileVec4,                       1 ) + u2 * IN( maTileVec4,                       2 ) + u3 * IN( maTileVec4,                       3 ) );
    PRECISE vec4 lSmoothNormalVec4           = INVARIANT( u0 * IN( maSmoothNormalVec3_mfDistForFade, 0 ) + u1 * IN( maSmoothNormalVec3_mfDistForFade, 1 ) + u2 * IN( maSmoothNormalVec3_mfDistForFade, 2 ) + u3 * IN( maSmoothNormalVec3_mfDistForFade, 3 ) );

    #ifdef D_TESS_ON_AMD
    if( distance( IN( maWorldPositionVec4, 1 ) - IN( maWorldPositionVec4, 0 ), PATCH_IN( mEdge0 ) ) > 0.05 )
    {
        // have detected a scrambled triangle -- change pos to NaN so it will be culled
        lWorldPositionVec4 /= 0.0;
        lScreenSpacePositionVec4 /= 0.0;
    }

    if( distance( IN( maWorldPositionVec4, 2 ) - IN( maWorldPositionVec4, 0 ), PATCH_IN( mEdge1 ) ) > 0.05 )
    {
        // have detected a scrambled triangle -- change pos to NaN so it will be culled
        lWorldPositionVec4 /= 0.0;
        lScreenSpacePositionVec4 /= 0.0;
    }

    if( distance( IN( maWorldPositionVec4, 3 ) - IN( maWorldPositionVec4, 0 ), PATCH_IN( mEdge2 ) ) > 0.05 )
    {
        // have detected a scrambled triangle -- change pos to NaN so it will be culled
        lWorldPositionVec4 /= 0.0;
        lScreenSpacePositionVec4 /= 0.0;
    }

    #ifdef D_READ_TEX_CACHE
    {
        // if READ is active the border will be applied so we must un-apply it
        vec2 lTexCoords0 = IN( maTexCoordVec2_mTexBorderVec2, 0 ).xy - IN( maTexCoordVec2_mTexBorderVec2, 0 ).zw;
        vec2 lTexCoords1 = IN( maTexCoordVec2_mTexBorderVec2, 1 ).xy - IN( maTexCoordVec2_mTexBorderVec2, 1 ).zw;
        vec2 lTexCoords2 = IN( maTexCoordVec2_mTexBorderVec2, 2 ).xy - IN( maTexCoordVec2_mTexBorderVec2, 2 ).zw;
        vec2 lTexCoords3 = IN( maTexCoordVec2_mTexBorderVec2, 3 ).xy - IN( maTexCoordVec2_mTexBorderVec2, 3 ).zw;

        if( distance( lTexCoords1.xy - lTexCoords0.xy, PATCH_IN( mTexEdge ).xy ) > 0.001 )
        {
            lWorldPositionVec4 /= 0.0;
            lScreenSpacePositionVec4 /= 0.0;
        }

        if( distance( lTexCoords2.xy - lTexCoords3.xy, PATCH_IN( mTexEdge ).xy ) > 0.001 )
        {
            lWorldPositionVec4 /= 0.0;
            lScreenSpacePositionVec4 /= 0.0;
        }

        if( distance( lTexCoords3.xy - lTexCoords0.xy, PATCH_IN( mTexEdge ).zw ) > 0.001 )
        {
            lWorldPositionVec4 /= 0.0;
            lScreenSpacePositionVec4 /= 0.0;
        }

        if( distance( PATCH_IN( mTexCorner ).xy, lTexCoords0 ) > 0.001 )
        {
            lWorldPositionVec4 /= 0.0;
            lScreenSpacePositionVec4 /= 0.0;
        }

        if( distance( PATCH_IN( mTexCorner ).zw, lTexCoords2 ) > 0.001 )
        {
            lWorldPositionVec4 /= 0.0;
            lScreenSpacePositionVec4 /= 0.0;
        }

    }
    #else
    {
        if( distance( IN( maTexCoordVec2_mTexBorderVec2, 1 ).xy - IN( maTexCoordVec2_mTexBorderVec2, 0 ).xy, PATCH_IN( mTexEdge ).xy ) > 0.01 )
        {
            lWorldPositionVec4 /= 0.0;
            lScreenSpacePositionVec4 /= 0.0;
        }

        if( distance( IN( maTexCoordVec2_mTexBorderVec2, 3 ).xy - IN( maTexCoordVec2_mTexBorderVec2, 0 ).xy, PATCH_IN( mTexEdge ).zw ) > 0.01 )
        {
            lWorldPositionVec4 /= 0.0;
            lScreenSpacePositionVec4 /= 0.0;
        }

        if( distance( IN( maTexCoordVec2_mTexBorderVec2, 2 ).xy - IN( maTexCoordVec2_mTexBorderVec2, 3 ).xy, PATCH_IN( mTexEdge ).xy ) > 0.01 )
        {
            lWorldPositionVec4 /= 0.0;
            lScreenSpacePositionVec4 /= 0.0;
        }
    }
    #endif


    #endif

    #if !defined(D_DEPTHONLY)
    OUT( mTileVec4                       ) = lTileVec4;
    OUT( mTexCoordVec2_mTexBorderVec2    ) = lTexCoordVec2_mTexBorderVec2;
    OUT( mWorldPositionVec4              ) = lWorldPositionVec4;
    OUT( mSmoothNormalVec3_mfDistForFade ) = lSmoothNormalVec4;
    #if ( defined( D_TESS_SHADERS_PRESENT ) || defined( D_PLATFORM_PC ) ) && ( defined( D_READ_TEX_CACHE ) || defined( D_WRITE_TEX_CACHE ) )
    OUT( mTexCoordsDPDUVec4              ) = INVARIANT( u0 * IN( maTexCoordsDPDUVec4,            0 ) + u1 * IN( maTexCoordsDPDUVec4,            1 ) + u2 * IN( maTexCoordsDPDUVec4,            2 ) + u3 * IN( maTexCoordsDPDUVec4,            3 ) );
    OUT( mTexCoordsDPDVVec4              ) = INVARIANT( u0 * IN( maTexCoordsDPDVVec4,            0 ) + u1 * IN( maTexCoordsDPDVVec4,            1 ) + u2 * IN( maTexCoordsDPDVVec4,            2 ) + u3 * IN( maTexCoordsDPDVVec4,            3 ) );
    #endif
    #endif
 

    #ifdef D_APPLY_DISPLACEMENT

    float lfTessLevel = saturate( lTessParams.x / lUniforms.mpPerFrame->gTessSettingsVec4.w );

    if( lfTessLevel > 0 )
    {
        float lfHeight     = 0.0;


        #ifdef D_READ_TEX_CACHE

        // vec2 lCacheCoordsHi = lTexCoordVec2_mfDistForFade.xy;
        // #ifndef D_PLATFORM_OPENGL
        // lCacheCoordsHi.y = 1.0 - lCacheCoordsHi.y;
        // #endif
        // vec2 lCacheCoordsLo = lCacheCoordsHi;

        float lTriSize = abs( IN( maTessellationParamsVec4, 0 ).y / exp2( lTessParams.x ) );
        ivec2 lResolution = ivec2( GetTexResolution( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gSparseNearStatusMap ) ) );
        float lTriPixelCount = lTriSize * lResolution.x * 256.0;

        float lfMinMip0 = DOMAIN_COORDS.y == 0.0? IN( maTessellationParamsVec4, 0 ).w : 0.0;
        float lfMinMip1 = DOMAIN_COORDS.x == 1.0? IN( maTessellationParamsVec4, 1 ).w : 0.0;
        float lfMinMip2 = DOMAIN_COORDS.y == 1.0? IN( maTessellationParamsVec4, 2 ).w : 0.0;
        float lfMinMip3 = DOMAIN_COORDS.x == 0.0? IN( maTessellationParamsVec4, 3 ).w : 0.0;
        float lfMinMip = max( max( lfMinMip0, lfMinMip1 ), max( lfMinMip2, lfMinMip3 ) );

        float lfDesiredMip = log2( lTriPixelCount ) - 5.0;
        lfDesiredMip = max( lfMinMip, min( 3.0, lfDesiredMip ) );

        vec2 lvBorder = lTexCoordVec2_mTexBorderVec2.zw;
            #ifndef D_PLATFORM_OPENGL
            lvBorder.y = -lvBorder.y;
            #endif

        float lfLowMip = 0.0;
        float lfHeightLo = GetSparseSample( 
            SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh, gSparseNearDiffMap ), 
            SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh, gSparseNearStatusMap ), 
            lCacheCoordsLo, 
            lvBorder, 
            vec2( 0.0, 0.0 ),
            vec2( 0.0, 0.0 ),
            lResolution, 
            lfDesiredMip, 
            lfLowMip ).a;

        float lfHiMip = 0.0;
        float lfHeightHi = GetSparseSample( 
            SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh, gSparseNearDiffMap ), 
            SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh, gSparseNearStatusMap ), 
            lCacheCoordsHi, 
            lvBorder, 
            vec2( 0.0, 0.0 ),
            vec2( 0.0, 0.0 ),
            lResolution, 
            lfDesiredMip, 
            lfHiMip ).a;

        float lfMip = max( lfLowMip, lfHiMip );

        lfHeight = mix( lfHeightLo, lfHeightHi, fract( lTessParams.x ) );
        lfHeight = 0.5 - lfHeight;
        #ifdef D_TESS_ON_AMD
        // temp fix for yet another cracking bug until I figure out what it is
        // trade pop-in for cracking since I think pop-in is a bit less visible
        if( lfMip > 1.0 ) lfHeight = 0.0;
        #else
        if( lfMip > IN( maTessellationParamsVec4, 0 ).z ) lfHeight = 0.0;
        #endif

        #else  
        float lfSpecular   = 0.0;
        float lfSubsurface = 0.0;
        float lfMetallic   = 0.0;
        float lfGlow       = 0.0;

        uvec4  lTileTextureIndicesSmall1Vec4;
        uvec4  lTileTextureIndicesSmall2Vec4;
        uvec4  lTileTextureIndicesLarge1Vec4;
        uvec4  lTileTextureIndicesLarge2Vec4;

        vec3  lTerrainSpaceNormalVec3;
        vec3  lTerrainSpaceFaceNormalVec3;
        vec3  lTerrainSpacePositionVec3;       
        vec3  lTerrainOffsetVec3;       

        float lfTextureSmallScaleFactor = 1.0 / ( lUniforms.mpCustomPerMesh->gTerrainLodParamsVec4.x );
        float lfTextureLargeScaleFactor = 1.0 / ( lUniforms.mpCustomPerMesh->gTerrainLodParamsVec4.y );

        lTerrainSpaceNormalVec3       = normalize( lSmoothNormalVec4.xyz );
        lTerrainSpaceFaceNormalVec3   = normalize( lSmoothNormalVec4.xyz );
        lTerrainSpacePositionVec3     = lWorldPositionVec4.xyz;
        lTerrainOffsetVec3 = -lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz;

        vec3 lUpDirection = normalize( lTerrainSpacePositionVec3 + lTerrainOffsetVec3 );

        lTileTextureIndicesLarge1Vec4.x = uint( round( mod( max( lTileVec4.x, 0.0 ), D_TERRAINCOLOURARRAY_SIZE ) ) );
        lTileTextureIndicesLarge2Vec4.x = uint( round( mod( max( lTileVec4.y, 0.0 ), D_TERRAINCOLOURARRAY_SIZE ) ) );

        uint luSlopeIndex1 = min( lTileTextureIndicesLarge1Vec4.x / 4, 2 );
        uint luSlopeIndex2 = min( lTileTextureIndicesLarge2Vec4.x / 4, 2 );


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
        else
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
        else
        {
            lTileTextureIndicesSmall2Vec4.x += 8;
            lTileTextureIndicesSmall2Vec4.y += 8;
            lTileTextureIndicesSmall2Vec4.z += 8;
            lTileTextureIndicesSmall2Vec4.w += 8;
        }

        // Tile blend
        float lfTile = lTileVec4.z;

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

        float lfCameraHeight = length( lUniforms.mpPerFrame->gViewPositionVec3 - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz ) - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w;
        lfCameraHeight = 1.0 - saturate( ( lfCameraHeight - 1000.0 ) / 1000.0 );

        lfPatch *= lfCameraHeight;
        lfSlope1  = mix( 1.0, lfSlope1, lfCameraHeight );
        lfSlope2  = mix( 1.0, lfSlope2, lfCameraHeight );


        vec3 lNormalVec3 = lSmoothNormalVec4.xyz;
        GetTileColourAndNormal(
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
            0.0,
            SAMPLER2DARRAYPARAM_SRT( lUniforms.mpCustomPerMaterial,gDiffuseMap ),
            SAMPLER2DARRAYPARAM_SRT( lUniforms.mpCustomPerMaterial,gNormalMap ),
            SAMPLER2DARRAYPARAM_SRT( lUniforms.mpCustomPerMaterial,gSubstanceDiffuseMap ),
            SAMPLER2DARRAYPARAM_SRT( lUniforms.mpCustomPerMaterial,gSubstanceNormalMap ),
            lfSpecular,
            lfSubsurface,
            lfMetallic,
            lfGlow,
            lfHeight );
        lfHeight = 0.5 - lfHeight;

        #endif

        lWorldPositionVec4.xyz += lfTessLevel * lSmoothNormalVec4.xyz * lfHeight * lUniforms.mpPerFrame->gTessSettingsVec4.z;
    }

    lScreenSpacePositionVec4 = CalcScreenPosFromWorld( lUniforms.mpCommonPerMesh->gViewProjectionMat4, lWorldPositionVec4 );
#if defined( D_OUTPUT_MOTION_VECTORS )
    OUT( mScreenSpacePositionVec4      ) = lScreenSpacePositionVec4;
#endif

    #endif // D_APPLY_DISPLACEMENT

    OUT_SCREEN_POSITION = lScreenSpacePositionVec4;
}
