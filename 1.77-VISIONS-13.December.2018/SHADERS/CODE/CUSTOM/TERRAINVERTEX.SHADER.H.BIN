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

#ifndef D_VERTEX
#define D_VERTEX
#endif

#include "Common/Defines.shader.h"
#include "Common/Common.shader.h"
#include "Common/CommonUniforms.shader.h"




//
// In TerrainCommon we have our uniforms specific to terrain declared. PLUS, IMPORTANTLY we have the SRT buffer declared (gUniforms). 
// This MUST be included after CommonUniforms, but before all the other stuff that references gUniforms.
//
#include "Custom/TerrainCommon.h"


//
// Have to include things that reference the global gUniforms under here. Things defined above may be parameters to functions in the following includes.
//
#include "Common/CommonVertex.shader.h"
#include "Common/CommonPlanet.shader.h"
#include "Common/CommonDepth.shader.h"
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

#ifdef D_MESH

    DECLARE_INPUT

    INPUT( vec4, mkLocalPositionVec4, POSITION0 )
    INPUT( vec4, mkTexCoordsVec4,     TEXCOORD0 )
    INPUT( vec4, mkLocalNormalVec4,   TEXCOORD1 )

    #ifdef D_DECLARE_TANGENT
    INPUT( vec4, mkTangentVec4, TANGENT0 )
    #endif

DECLARE_END

#else

DECLARE_INPUT

    INPUT( vec4, mkLocalPositionVec4, POSITION0 )
    INPUT( vec4, mkCustom1Vec4,       BLENDINDICES  )
#ifdef D_OCTAHEDRON_NORMALS    
    INPUT( vec4, mkLocalNormalVec4,   TEXCOORD1       )
#else
    INPUT( vec3, mkFaceNormalVec3,    TEXCOORD1       )
    INPUT( vec3, mkSmoothNormalVec3,  TEXCOORD2       )
#endif    
#ifdef D_PLATFORM_PC
    INPUT( vec4, mkCustom2Vec4,       BLENDINDICES  )
#endif

DECLARE_END

#endif

//-----------------------------------------------------------------------------
///
///     Output
///
///     @brief  Output
///
//-----------------------------------------------------------------------------
DECLARE_OUTPUT
    
    OUTPUT_SCREEN_POSITION
#if !defined(D_DEPTHONLY) || defined( D_TESS_SHADERS_PRESENT )
    OUTPUT( vec4,   mWorldPositionVec4,              TEXCOORD0 )
#ifdef D_ASTEROID    
    OUTPUT( vec3,   mLocalPositionVec3,              TEXCOORD1 )
#endif
    OUTPUT( vec4,   mTileVec4,                       TEXCOORD2 )
    OUTPUT( vec4,   mTexCoordVec2_mTexBorderVec2,    TEXCOORD3 )
    OUTPUT( vec4,   mSmoothNormalVec3_mfDistForFade, TEXCOORD4 )

#if defined( D_PLATFORM_PC ) && !defined( D_TESS_SHADERS_PRESENT ) && ( defined( D_READ_TEX_CACHE ) || defined( D_WRITE_TEX_CACHE ) )
    OUTPUT( vec4,   mTexCoordsDPDUVec4,              TEXCOORD6 )
    OUTPUT( vec4,   mTexCoordsDPDVVec4,              TEXCOORD7 )
#endif

#endif
    OUTPUT( vec4,   mScreenSpacePositionVec4,        TEXCOORD5 )
DECLARE_END

//-----------------------------------------------------------------------------
//    Functions

//-----------------------------------------------------------------------------
///
///     Vertex Main
///
///     @brief      Vertex Main
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------

#if defined( D_PLATFORM_ORBIS ) && ( !defined( D_DEPTHONLY ) || defined( D_TESS_SHADERS_PRESENT ) )
#pragma argument(indirectdraw)
#endif

VERTEX_MAIN_SRT
{    
    vec4 lWorldPositionVec4;        
    vec4 lScreenSpacePositionVec4;
    
    vec3   lLocalPositionVec3;
    vec3   lSmoothNormalVec3;
    //vec3   lFaceNormalVec3;
    vec4   lTiles1Vec4;

    lLocalPositionVec3   = IN( mkLocalPositionVec4 ).xyz;

#ifdef D_MESH

    //lFaceNormalVec3       = IN( mkLocalNormalVec4 ).xyz;
    lSmoothNormalVec3     = IN( mkLocalNormalVec4 ).xyz;

#else

#ifdef D_OCTAHEDRON_NORMALS      
    //lFaceNormalVec3      = OctahedronNormalDecode( IN( mkLocalNormalVec4 ).xy );
    lSmoothNormalVec3    = OctahedronNormalDecode( IN( mkLocalNormalVec4 ).zw );
#else
    //lFaceNormalVec3      = IN( mkFaceNormalVec3 ).xyz;
    lSmoothNormalVec3    = IN( mkSmoothNormalVec3 ).xyz;
#endif

#endif

#ifdef D_MESH
    lTiles1Vec4          = vec4( 1.0, 1.0, 1.0, 1.0 );
#else
    lTiles1Vec4          = IN( mkCustom1Vec4 );
#endif

#if defined( D_PLATFORM_PC ) && !defined( D_MESH ) && !defined( D_TESS_SHADERS_PRESENT ) && ( defined( D_READ_TEX_CACHE ) || defined( D_WRITE_TEX_CACHE ) )
    vec3 lDpDu = OctahedronNormalDecode( IN( mkCustom2Vec4 ).zw );

    lDpDu -= lSmoothNormalVec3 * dot( lSmoothNormalVec3, lDpDu );
    lDpDu = normalize( lDpDu );

    vec3 lDpDv = normalize( cross( lSmoothNormalVec3, lDpDu ) );

    OUT( mTexCoordsDPDUVec4 ) = vec4( lDpDu, 0.0 );
    OUT( mTexCoordsDPDVVec4 ) = vec4( lDpDv, 0.0 );
#endif

    //-----------------------------------------------------------------------------
    ///
    ///     World Transform
    ///
    //-----------------------------------------------------------------------------
#if defined( D_ASTEROID ) || defined( D_MESH ) || defined( D_TESS_SHADERS_PRESENT )
    lWorldPositionVec4          = CalcWorldPos( lUniforms.mpCommonPerMesh->gWorldMat4, vec4( lLocalPositionVec3, 1.0 ) );
#else
    lWorldPositionVec4.xyz      = lLocalPositionVec3 + lUniforms.mpCommonPerMesh->gWorldMat4[3].xyz;
    lWorldPositionVec4.w        = 1.0;
#endif        

    OUT(mScreenSpacePositionVec4) = CalcScreenPosFromWorld(lUniforms.mpCommonPerMesh->gViewProjectionMat4, lWorldPositionVec4);


    vec3 lViewPositionVec3 = ( lUniforms.mpPerFrame->gViewPositionVec3 - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz ) / lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w;
    //vec3 lLightPositionVec3 = lUniforms.mpCustomPerMaterial->gSunPositionVec4.xyz;


#if !defined(D_DEPTHONLY) || defined( D_TESS_SHADERS_PRESENT )
    OUT( mWorldPositionVec4 ) = lWorldPositionVec4;
#endif

    float lfDistForFade = length( lUniforms.mpPerFrame->gViewPositionVec3 - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz );

#if !defined( D_ASTEROID ) && !defined( D_MESH )

    float lfFlattenUniform = 0.0;

    #if !defined(D_CUSTOM_TILES)
        lfFlattenUniform = lUniforms.mpCustomPerMesh->gTerrainLodParamsVec4.z;
    #endif

    // Flatten terrain down as we go to space
    float lfRadius                  = lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w + lUniforms.mpCustomPerMaterial->gWaterFogVec4.r * (1.0 - lfFlattenUniform);
    float lfWorldHeight             = length( lWorldPositionVec4.xyz - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz );
    vec3 lFlatWorldPositionNormVec3 = normalize( lWorldPositionVec4.xyz - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz );
    vec3 lFlatWorldPositionVec3;

    lFlatWorldPositionVec3 = lFlatWorldPositionNormVec3 * ( lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w - 1.0 );


    #if !defined( D_WRITE_TEX_CACHE )
        float lfFlattenFactor   = ( length( lUniforms.mpPerFrame->gViewPositionVec3 - lWorldPositionVec4.xyz ) - 6000.0 ) / ( 10000.0 - 6000.0 );
        lfFlattenFactor         = clamp( lfFlattenFactor, 0.0, 1.0 );
        lWorldPositionVec4.xyz  = mix( lWorldPositionVec4.xyz, lFlatWorldPositionVec3 + lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz, lfFlattenFactor );
        //lFaceNormalVec3       = mix( lFaceNormalVec3, lFlatWorldPositionNormVec3, lfFlattenFactor );
        lSmoothNormalVec3       = mix( lSmoothNormalVec3, lFlatWorldPositionNormVec3, lfFlattenFactor );
    #endif

    #endif

#ifdef D_ASTEROID
    OUT( mLocalPositionVec3 ) = lLocalPositionVec3;
#endif


    //-----------------------------------------------------------------------------
    ///
    ///     Screen Transform
    ///
    //-----------------------------------------------------------------------------
    #ifdef D_REFLECT_DUALP
    {
          //lScreenSpacePositionVec4 = CalcDualParaboloidScreenPosition( gCommon.gViewMat4, lWorldPositionVec4, gCommon.gClipPlanesVec4.xy );
        vec3 lReflectPosition       = GetWorldUp( lWorldPositionVec4.xyz, lUniforms.mpCommonPerMesh->gPlanetPositionVec4 ) * lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w + lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz;
        lScreenSpacePositionVec4    = CalcDualParaboloidScreenPosition( 
                                        lUniforms.mpPerFrame->gViewMat4, 
                                        vec4(lReflectPosition.x, lReflectPosition.y, lReflectPosition.z, lWorldPositionVec4.w), 
                                        lUniforms.mpPerFrame->gClipPlanesVec4.xy);
    }
    #else
    {
        lScreenSpacePositionVec4 = CalcScreenPosFromWorld( lUniforms.mpCommonPerMesh->gViewProjectionMat4, lWorldPositionVec4 );
    }    
    #endif

#if !defined(D_DEPTHONLY) || defined( D_TESS_SHADERS_PRESENT )

    //-----------------------------------------------------------------------------
    ///
    ///     Normals
    ///
    //-----------------------------------------------------------------------------

    vec2 lTexCoords = IN( mkLocalNormalVec4 ).xy;
    vec2 lvBorder = vec2( 0.0, 0.0 );

    #if defined( D_PLATFORM_PC ) && !defined( D_TESS_SHADERS_PRESENT ) && !defined( D_MESH )
    vec2 lvQuadCenter = IN( mkCustom2Vec4 ).xy;
    #ifdef D_WRITE_TEX_CACHE
    vec2 lResolution = lUniforms.mpCustomPerMesh->gTextureCoordsAdjustVec4.xy * lUniforms.mpPerFrame->gFrameBufferSizeVec4.xy;
    vec2 lInvRes = vec2( 1.0, 1.0 ) / vec2( lResolution );
    vec2 lOnePixelIn = sign( lvQuadCenter - lTexCoords ) * lInvRes;
    lvBorder = 0.45 * lOnePixelIn;
    #else
    vec2 lInvRes = vec2( 1.0 / 8192.0, 1.0 / 8192.0 );
    vec2 lOnePixelIn = sign( lvQuadCenter - lTexCoords ) * lInvRes;
    lvBorder = 0.5 * lOnePixelIn;
    #endif
    lTexCoords += lvBorder;
    #endif

    OUT( mTexCoordVec2_mTexBorderVec2 ) = vec4( lTexCoords.x, lTexCoords.y, lvBorder.x, lvBorder.y );
    OUT( mSmoothNormalVec3_mfDistForFade ) = vec4( lSmoothNormalVec3, lfDistForFade );

    
    //----------------------------------------------------------------------------- 
    ///
    ///     Terrain Values
    ///
    //-----------------------------------------------------------------------------
    // Indices passed in from dualcontouring
    float lIndex1 = lTiles1Vec4.x;
    float lIndex2 = lTiles1Vec4.y;
    
    // And this is an optimization to save a LOT of time in GetBlendedColour
#ifdef D_LOW_QUALITY
    if ( lTiles1Vec4.z > 0.5 )
    {
        float lfSwap    = lIndex1;
        lIndex1         = lIndex2;
        lIndex2         = lfSwap;
    }
#endif
   

#ifdef D_MESH
    {
        lIndex1 = lUniforms.mpCustomPerMesh->gTerrainLodParamsVec4.z;
        lIndex2 = lUniforms.mpCustomPerMesh->gTerrainLodParamsVec4.w;
        //vec3 lUpDirection = GetWorldUp( lWorldPositionVec4.xyz, lUniforms.mpCommonPerMesh->gPlanetPositionVec4 ) ;
        vec3 lUpDirection = vec3 ( 0.0, 1.0, 0.0 );
        //Skew the flat towards the top slightly, to show more slope [-1 : 1] -> [-0.8 : 1] -> Clamped to 0
        lTiles1Vec4.z = max( (0.9 * ( 1.0 + dot( lUpDirection, normalize( lSmoothNormalVec3 )))) - 0.8, 0.0 );  //Tile blend
        lTiles1Vec4.w = 1.0;  //Water fade
    }
#endif

    OUT( mTileVec4.x )        = lIndex1;
    OUT( mTileVec4.y )        = lIndex2;
    OUT( mTileVec4.z )        = lTiles1Vec4.z;
    OUT( mTileVec4.w )        = lTiles1Vec4.w;

#endif

    //-----------------------------------------------------------------------------
    ///
    ///     Output
    ///
    //-----------------------------------------------------------------------------
    #if defined( D_WRITE_TEX_CACHE )
    lTexCoords = lTexCoords * lUniforms.mpCustomPerMesh->gTextureCoordsAdjustVec4.xy + lUniforms.mpCustomPerMesh->gTextureCoordsAdjustVec4.zw;
    #ifdef D_PLATFORM_OPENGL
    lTexCoords.y = 1.0 - lTexCoords.y;
    #endif

    SCREEN_POSITION = vec4( lTexCoords * 2.0 - 1.0, 0.5, 1.0 );
    #else
    SCREEN_POSITION = lScreenSpacePositionVec4;
    #endif
}



