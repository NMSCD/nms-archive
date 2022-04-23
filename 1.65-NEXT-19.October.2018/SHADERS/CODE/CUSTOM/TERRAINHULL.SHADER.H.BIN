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

//-----------------------------------------------------------------------------
//      Include files


#include "Common/Defines.shader.h"
#include "Common/Common.shader.h"
#include "Common/CommonUniforms.shader.h"




//
// In TerrainCommon we have our uniforms specific to terrain declared. PLUS, IMPORTANTLY we have the SRT buffer declared (gUniforms). 
// This MUST be included after CommonUniforms, but before all the other stuff that references gUniforms.
//

#include "Custom/TerrainCommon.h"

#define D_TEXTURE_ARRAYS
//
// Have to include things that reference the global gUniforms under here. Things defined above may be parameters to functions in the following includes.
//
#include "Common/CommonVertex.shader.h"
#include "Common/CommonPlanet.shader.h"
#include "Common/CommonDepth.shader.h"
#include "Common/CommonFragment.shader.h"

#ifndef D_DEPTHONLY
#include "Common/CommonTriplanarTexturing.shader.h"
#endif

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
    INPUT( vec4,   mWorldPositionVec4,              TEXCOORD0 )
#ifdef D_ASTEROID    
    INPUT( vec3,   mLocalPositionVec3,              TEXCOORD1 )
#endif
    INPUT( vec4,   mTileVec4,                       TEXCOORD2 )
    INPUT( vec4,   mTexCoordVec2_mTexBorderVec2,    TEXCOORD3 )
    INPUT( vec4,   mSmoothNormalVec3_mfDistForFade, TEXCOORD4 )
    INPUT( vec4,   mScreenSpacePositionVec4,        TEXCOORD5 )
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
    OUTPUT( vec4,   maWorldPositionVec4,              TEXCOORD0 )
#ifdef D_ASTEROID    
    OUTPUT( vec3,   maLocalPositionVec3,              TEXCOORD1 )
#endif
    OUTPUT( vec4,   maTileVec4,                       TEXCOORD2 )
    OUTPUT( vec4,   maTexCoordVec2_mTexBorderVec2,    TEXCOORD3 )
    OUTPUT( vec4,   maSmoothNormalVec3_mfDistForFade, TEXCOORD4 )
    OUTPUT( vec4,   maScreenSpacePositionVec4,        TEXCOORD5 )
#if  defined( D_WRITE_TEX_CACHE ) ||  defined( D_APPLY_DISPLACEMENT )
    OUTPUT( vec4,   maTessellationParamsVec4,         TEXCOORD6 )
#endif

#if defined( D_READ_TEX_CACHE ) || defined( D_WRITE_TEX_CACHE )
    OUTPUT( vec4,   maTexCoordsDPDUVec4,              TEXCOORD10 )
    OUTPUT( vec4,   maTexCoordsDPDVVec4,              TEXCOORD11 )
#endif

DECLARE_END

//-----------------------------------------------------------------------------
//    Functions


//-----------------------------------------------------------------------------
///
///     Hull Main
///
///     @brief      Hull Main
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------

DECLARE_PATCH_OUTPUT_QUAD
#ifdef D_TESS_ON_AMD
    PATCH_OUTPUT( vec4, mEdge0, TEXCOORD12 )
    PATCH_OUTPUT( vec4, mEdge1, TEXCOORD13 )
    PATCH_OUTPUT( vec4, mEdge2, TEXCOORD14 )
    PATCH_OUTPUT( vec4, mTexEdge, TEXCOORD15 )
    PATCH_OUTPUT( vec4, mTexCorner, TEXCOORD16 )
#endif
    OUT_PATCH_QUAD_TESS_CONSTANTS
DECLARE_END


//-----------------------------------------------------------------------------
///
///     ComputeTessellationFactor
///
///     @brief      ComputeTessellationFactor
///
///     @param      vec3 lPointPositionVec3
///     @param      vec3 lViewPositionVec3
///     @param      vec4 lGlobalTessSettingsVec4
///     @return     float
///
//-----------------------------------------------------------------------------
float
ComputeTessellationFactor(
    vec3 lPointPositionVec3,
    vec3 lViewPositionVec3,
    vec4 lGlobalTessSettingsVec4 )
{
    #ifdef D_APPLY_DISPLACEMENT

    float lfDistScale = 2.0;
    float lfMaxTess   = lGlobalTessSettingsVec4.y - 1.0;
    float lfDistStart = lGlobalTessSettingsVec4.x;
    float lfDistEnd   = 1.5;

    vec3 lvDiff = lPointPositionVec3 - lViewPositionVec3;
    float lfDist = max( 0.0, length( lvDiff ) - lfDistEnd ); 

    float lfTessAmount = saturate( ( lfDistScale / lfDist ) - ( lfDistScale / lfDistStart ) );
    lfTessAmount *= lfMaxTess;

    return ( log2( 1.0 + lfTessAmount ) );

    #else

    return 0.0;

    #endif
}

HULL_QUAD_CONSTANTS_SRT( 6 )
{
    bool cull = false;

    // note -- hull shader takes two triangles (6 points) as input and outputs a quad (4 points)
    // this is so the vertex buffer can be submitted as either patches or triangles depending on need
    // the hull shader uses patch verts 0, 5, 1, 2 in that order and (mostly) ignores verts 3 and 4

    #ifdef D_TESS_ON_AMD
    PATCH_OUT( mEdge0 ) = PATCH_IN( mWorldPositionVec4, 5 ) - PATCH_IN( mWorldPositionVec4, 0 );
    PATCH_OUT( mEdge1 ) = PATCH_IN( mWorldPositionVec4, 1 ) - PATCH_IN( mWorldPositionVec4, 0 );
    PATCH_OUT( mEdge2 ) = PATCH_IN( mWorldPositionVec4, 2 ) - PATCH_IN( mWorldPositionVec4, 0 );

    PATCH_OUT( mTexEdge ).xy = vec2( PATCH_IN( mTexCoordVec2_mTexBorderVec2, 3 ).x * 2.0, 0.0 );
    PATCH_OUT( mTexEdge ).zw = vec2( 0.0, PATCH_IN( mTexCoordVec2_mTexBorderVec2, 3 ).y * 2.0 );

    PATCH_OUT( mTexCorner ).xy = PATCH_IN( mTexCoordVec2_mTexBorderVec2, 0 ).xy - PATCH_IN( mTexCoordVec2_mTexBorderVec2, 3 ).xy;
    PATCH_OUT( mTexCorner ).zw = PATCH_IN( mTexCoordVec2_mTexBorderVec2, 0 ).xy + PATCH_IN( mTexCoordVec2_mTexBorderVec2, 3 ).xy;
    #endif

    #ifdef D_WRITE_TEX_CACHE
    {
        vec2 lfMinCorner = PATCH_IN( mTexCoordVec2_mTexBorderVec2, 0 ).xy - PATCH_IN( mTexCoordVec2_mTexBorderVec2, 3 ).xy;
        vec2 lfMaxCorner = PATCH_IN( mTexCoordVec2_mTexBorderVec2, 0 ).xy + PATCH_IN( mTexCoordVec2_mTexBorderVec2, 3 ).xy;

        lfMinCorner = lfMinCorner * lUniforms.mpCustomPerMesh->gTextureCoordsAdjustVec4.xy + lUniforms.mpCustomPerMesh->gTextureCoordsAdjustVec4.zw;
        lfMaxCorner = lfMaxCorner * lUniforms.mpCustomPerMesh->gTextureCoordsAdjustVec4.xy + lUniforms.mpCustomPerMesh->gTextureCoordsAdjustVec4.zw;

        if( lfMinCorner.x < lUniforms.mpCustomPerMesh->gTextureCoordsBorderVec4.x ) cull = true;
        if( lfMinCorner.y < lUniforms.mpCustomPerMesh->gTextureCoordsBorderVec4.y ) cull = true;
        if( lfMaxCorner.x > lUniforms.mpCustomPerMesh->gTextureCoordsBorderVec4.z ) cull = true;
        if( lfMaxCorner.y > lUniforms.mpCustomPerMesh->gTextureCoordsBorderVec4.w ) cull = true;
    }
    #else
    {
        vec4 lScreenSpace[4] = { PATCH_IN( mScreenSpacePositionVec4, 0 ),
                                 PATCH_IN( mScreenSpacePositionVec4, 1 ),
                                 PATCH_IN( mScreenSpacePositionVec4, 2 ),
                                 PATCH_IN( mScreenSpacePositionVec4, 5 ) };

        vec4 lNormals[4] = { PATCH_IN( mSmoothNormalVec3_mfDistForFade, 0 ),
                             PATCH_IN( mSmoothNormalVec3_mfDistForFade, 1 ),
                             PATCH_IN( mSmoothNormalVec3_mfDistForFade, 2 ),
                             PATCH_IN( mSmoothNormalVec3_mfDistForFade, 5 ) };

        #ifdef D_APPLY_DISPLACEMENT
        float lfMaxHeight = lUniforms.mpPerFrame->gTessSettingsVec4.z * 0.5;
        #else
        float lfMaxHeight = 0.0;
        #endif

        int lPlanes[6] = { 0,0,0,0,0,0 };

        for( int ii=0; ii<4; ++ii )
        {
            lNormals[ii] = MUL( lUniforms.mpCommonPerMesh->gViewProjectionMat4, vec4( lNormals[ii].xyz, 0.0 ) ) * lfMaxHeight;

            vec4 lCorners0 = lScreenSpace[ii] + lNormals[ii];

            lPlanes[0] += lCorners0.x < -lCorners0.w? 1 : 0;
            lPlanes[1] += lCorners0.x >  lCorners0.w? 1 : 0;

            lPlanes[2] += lCorners0.y < -lCorners0.w? 1 : 0;
            lPlanes[3] += lCorners0.y >  lCorners0.w? 1 : 0;

            lPlanes[4] += lCorners0.z <  0.0? 1 : 0;
            lPlanes[5] += lCorners0.z >  lCorners0.w? 1 : 0;

            vec4 lCorners1 = lScreenSpace[ii] - lNormals[ii];

            lPlanes[0] += lCorners1.x < -lCorners1.w? 1 : 0;
            lPlanes[1] += lCorners1.x >  lCorners1.w? 1 : 0;

            lPlanes[2] += lCorners1.y < -lCorners1.w? 1 : 0;
            lPlanes[3] += lCorners1.y >  lCorners1.w? 1 : 0;

            lPlanes[4] += lCorners1.z <  0.0? 1 : 0;
            lPlanes[5] += lCorners1.z >  lCorners1.w? 1 : 0;

        }

        cull = lPlanes[0] == 8 || lPlanes[1] == 8 || lPlanes[2] == 8 || lPlanes[3] == 8 || lPlanes[4] == 8 || lPlanes[5] == 8;

    }
    #endif


    if( cull )
    {
        TESS_LEVEL_EDGE( 0 ) = -1;
        TESS_LEVEL_EDGE( 1 ) = -1;
        TESS_LEVEL_EDGE( 2 ) = -1;
        TESS_LEVEL_EDGE( 3 ) = -1;

        TESS_LEVEL_INNER( 0 ) = -1;
        TESS_LEVEL_INNER( 1 ) = -1;
    }
    else
    {

        #ifdef D_WRITE_TEX_CACHE


        #ifdef D_EXPAND_BORDER

        // it's good to subdivide the quads in cache-writing a little bit so and tri distortion
        // is less visible

        // first compute the pixels on the side of a quad
         float lfMinU = PATCH_IN( mTexCoordVec2_mTexBorderVec2, 0 ).x - PATCH_IN( mTexCoordVec2_mTexBorderVec2, 3 ).x;

         float lfMaxU = PATCH_IN( mTexCoordVec2_mTexBorderVec2, 0 ).x + PATCH_IN( mTexCoordVec2_mTexBorderVec2, 3 ).x;

        vec2 lResolution = lUniforms.mpCustomPerMesh->gTextureCoordsAdjustVec4.xy * lUniforms.mpPerFrame->gFrameBufferSizeVec4.xy;
        vec2 lInvRes = vec2( 1.0, 1.0 ) / vec2( lResolution );
        float lfPixelsPerQuad = lResolution.x * ( lfMaxU - lfMinU );

        // then do either 16x16 quads or 4 subdivs, whichever is less
        float lfSubdiv = max( 1.0, min( 4.0, lfPixelsPerQuad / 16.0 ) );

        TESS_LEVEL_EDGE( 0 ) = lfSubdiv;
        TESS_LEVEL_EDGE( 1 ) = lfSubdiv;
        TESS_LEVEL_EDGE( 2 ) = lfSubdiv;
        TESS_LEVEL_EDGE( 3 ) = lfSubdiv;

        TESS_LEVEL_INNER( 0 ) = lfSubdiv;
        TESS_LEVEL_INNER( 1 ) = lfSubdiv;

        #else

        TESS_LEVEL_EDGE( 0 ) = 4;
        TESS_LEVEL_EDGE( 1 ) = 4;
        TESS_LEVEL_EDGE( 2 ) = 4;
        TESS_LEVEL_EDGE( 3 ) = 4;

        TESS_LEVEL_INNER( 0 ) = 4;
        TESS_LEVEL_INNER( 1 ) = 4;

        #endif

        #else

        float z0 = ComputeTessellationFactor( PATCH_IN( mWorldPositionVec4, 0 ).xyz,
                                              lUniforms.mpPerFrame->gViewPositionVec3,
                                              lUniforms.mpPerFrame->gTessSettingsVec4 );
        float z1 = ComputeTessellationFactor( PATCH_IN( mWorldPositionVec4, 1 ).xyz,
                                              lUniforms.mpPerFrame->gViewPositionVec3,
                                              lUniforms.mpPerFrame->gTessSettingsVec4 );
        float z2 = ComputeTessellationFactor( PATCH_IN( mWorldPositionVec4, 2 ).xyz,
                                              lUniforms.mpPerFrame->gViewPositionVec3,
                                              lUniforms.mpPerFrame->gTessSettingsVec4 );
        float z3 = ComputeTessellationFactor( PATCH_IN( mWorldPositionVec4, 5 ).xyz,
                                              lUniforms.mpPerFrame->gViewPositionVec3,
                                              lUniforms.mpPerFrame->gTessSettingsVec4 );

        z0 = ceil( z0 );    
        z1 = ceil( z1 );
        z2 = ceil( z2 );
        z3 = ceil( z3 );

        float lfTessLevel = exp2( max( max( z0, z1 ), max( z2, z3 ) ) );

        TESS_LEVEL_EDGE( 0 ) = lfTessLevel;
        TESS_LEVEL_EDGE( 1 ) = lfTessLevel;
        TESS_LEVEL_EDGE( 2 ) = lfTessLevel;
        TESS_LEVEL_EDGE( 3 ) = lfTessLevel;

        TESS_LEVEL_INNER( 0 ) = lfTessLevel;
        TESS_LEVEL_INNER( 1 ) = lfTessLevel;

        #endif
    }
}

HULL_QUAD_MAIN_SRT( 6 )
{
     CALL_HULL_CONSTANTS;
     DECLARE_HULL_OUTPUT;

     OUT_SCREEN_POSITION           =  IN_SCREEN_POSITION;

    // note -- hull shader takes two triangles (6 points) as input and outputs a quad (4 points)
    // this is so the vertex buffer can be submitted as either patches or triangles depending on need
    // the hull shader uses patch verts 0, 5, 1, 2 in that order and (mostly) ignores verts 3 and 4
     int lVertOrder[4] = { 0, 5, 1, 2 };
     int vertId = lVertOrder[ gl_InvocationID ];
    
     OUT( maWorldPositionVec4            ) =  PATCH_IN( mWorldPositionVec4,           vertId );
#ifdef D_ASTEROID    
     OUT( maLocalPositionVec3            ) =  PATCH_IN( mLocalPositionVec3,           vertId );
#endif
     OUT( maTileVec4                     ) =  PATCH_IN( mTileVec4,                    vertId );
     OUT( maTexCoordVec2_mTexBorderVec2  ) =  PATCH_IN( mTexCoordVec2_mTexBorderVec2,  vertId );

     vec4 lSmoothNormalVec4                =  PATCH_IN( mSmoothNormalVec3_mfDistForFade, vertId );
     OUT( maSmoothNormalVec3_mfDistForFade ) =  lSmoothNormalVec4;

     OUT( maScreenSpacePositionVec4      ) =  PATCH_IN( mScreenSpacePositionVec4,     vertId );
 
     vec4 lDistVec = vec4( 0.0, 0.0, 0.0, 0.0 );
     lDistVec.x = ComputeTessellationFactor( PATCH_IN( mWorldPositionVec4,  vertId ).xyz,
                                             lUniforms.mpPerFrame->gViewPositionVec3,
                                             lUniforms.mpPerFrame->gTessSettingsVec4 );

     float lfMinU = PATCH_IN( mTexCoordVec2_mTexBorderVec2, 0 ).x - PATCH_IN( mTexCoordVec2_mTexBorderVec2, 3 ).x;
     float lfMinV = PATCH_IN( mTexCoordVec2_mTexBorderVec2, 0 ).y - PATCH_IN( mTexCoordVec2_mTexBorderVec2, 3 ).y;
     float lfMaxU = PATCH_IN( mTexCoordVec2_mTexBorderVec2, 0 ).x + PATCH_IN( mTexCoordVec2_mTexBorderVec2, 3 ).x;
     float lfMaxV = PATCH_IN( mTexCoordVec2_mTexBorderVec2, 0 ).y + PATCH_IN( mTexCoordVec2_mTexBorderVec2, 3 ).y;

     lDistVec.y = lfMaxU - lfMinU;

     vec2 lvPatchCenter = ( vec2(lfMinU, lfMinV) + vec2(lfMaxU, lfMaxV) ) * 0.5;

     float lfTexU[4] = {-1, 1, 1,-1};
     float lfTexV[4] = {-1,-1, 1, 1};

     vec2 lTexCoords;
     lTexCoords.x = PATCH_IN( mTexCoordVec2_mTexBorderVec2, 0 ).x + PATCH_IN( mTexCoordVec2_mTexBorderVec2, 3 ).x * lfTexU[ gl_InvocationID ];
     lTexCoords.y = PATCH_IN( mTexCoordVec2_mTexBorderVec2, 0 ).y + PATCH_IN( mTexCoordVec2_mTexBorderVec2, 3 ).y * lfTexV[ gl_InvocationID ];

     OUT( maTexCoordVec2_mTexBorderVec2 ).xy = lTexCoords;

    #if defined( D_READ_TEX_CACHE ) || defined( D_WRITE_TEX_CACHE )
     vec3 dPdU;

        dPdU = PATCH_IN( mWorldPositionVec4, 5 ).xyz - PATCH_IN( mWorldPositionVec4, 0 ).xyz;

        if( length( dPdU ) < 0.001 )
        {
            dPdU = PATCH_IN( mWorldPositionVec4, 1 ).xyz - PATCH_IN( mWorldPositionVec4, 2 ).xyz;
        }

     dPdU -= lSmoothNormalVec4.xyz * dot( lSmoothNormalVec4.xyz, dPdU );
     dPdU = normalize( dPdU );
     OUT( maTexCoordsDPDUVec4 ) = vec4( dPdU, 0.0 );

     vec3 dPdV = normalize( cross( lSmoothNormalVec4.xyz, dPdU ) );
     OUT( maTexCoordsDPDVVec4 ) = vec4( dPdV, 0.0 );
     #endif


     #ifdef D_READ_TEX_CACHE

     ivec2 lResolution = ivec2( GetTexResolution( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gSparseNearStatusMap ) ) );
     vec2 lInvRes = vec2( 1.0 / 256.0, 1.0 / 256.0 ) / vec2( lResolution );
     vec2 lOnePixelIn = sign( lvPatchCenter - lTexCoords ) * lInvRes;

     float kfBorderSize = 3.5;

     vec2 lvBorder = lOnePixelIn * kfBorderSize;
     OUT( maTexCoordVec2_mTexBorderVec2 ).xy = lTexCoords + lvBorder;
     OUT( maTexCoordVec2_mTexBorderVec2 ).zw = lvBorder;

     int lNeighborOrder[4] = { 4, 5, 1, 2 };
     vec2 lNeighborCoords = PATCH_IN( mTexCoordVec2_mTexBorderVec2, lNeighborOrder[ gl_InvocationID ] ).xy;
     #ifndef D_PLATFORM_OPENGL
     lNeighborCoords.y = 1.0 - lNeighborCoords.y;
     #endif
     float lfClosestMip = fract( texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gSparseNearStatusMap ), lNeighborCoords, 0.0 ).w ) * 8.0;

     lDistVec.w = lfClosestMip;

     float lfMaxDisplaceMip = log2( 1024 * lDistVec.y ); // 1024 == 8192 / 8, this mean we allow use of 8-pixel or larger mips
     lDistVec.z = lfMaxDisplaceMip;

     #elif defined( D_WRITE_TEX_CACHE )

     vec2 lResolution = lUniforms.mpCustomPerMesh->gTextureCoordsAdjustVec4.xy * lUniforms.mpPerFrame->gFrameBufferSizeVec4.xy;
     vec2 lInvRes = vec2( 1.0, 1.0 ) / vec2( lResolution );
     vec2 lOnePixelIn = sign( lvPatchCenter - lTexCoords ) * lInvRes;


     #ifdef D_EXPAND_BORDER

     float kfBorderSize = 3.5;
     float lfQuadPixels = lResolution.x * ( lfMaxU - lfMinU );
     float lfInterp = min( 0.25, kfBorderSize / lfQuadPixels );

     float lfWidth = -0.5 / ( 2 * lfInterp - 1 );

     float lfU[4] = {-1, 1,1,-1};
     float lfV[4] = {-1,-1,1, 1};

     lDistVec.z = 0.5 + lfU[ gl_InvocationID ] * lfWidth;
     lDistVec.w = 0.5 + lfV[ gl_InvocationID ] * lfWidth;

     #else

     float kfBorderSize = 3.49609375;

     OUT( maTexCoordVec2_mTexBorderVec2 ).xy = lTexCoords;

     lDistVec.z = lOnePixelIn.x;
     lDistVec.w = lOnePixelIn.y;

     #endif  // D_EXPAND_BORDER
     
     #endif  // D_WRITE_TEX_CACHE

#if  defined( D_WRITE_TEX_CACHE ) || defined( D_APPLY_DISPLACEMENT )
     OUT( maTessellationParamsVec4 ) = lDistVec;
#endif
     RETURN_HULL_OUTPUT;
}

