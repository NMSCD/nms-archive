////////////////////////////////////////////////////////////////////////////////
///
///     @file       SkyVertex.h
///     @author     User
///     @date       
///
///     @brief      SkyVertexShader
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 

#ifndef D_VERTEX
#define D_VERTEX
#endif

//-----------------------------------------------------------------------------
//      Include files

#include "Common/Defines.shader.h"
#include "Common/Common.shader.h"
#include "Common/CommonUniforms.shader.h"


#include "Custom/AtmosphereCommon.h"

#include "Common/CommonVertex.shader.h"
#include "Common/CommonDepth.shader.h"

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

    INPUT( vec4, mkLocalPositionVec4, POSITION0 )
    INPUT( vec4, mkTexCoordsVec4,     TEXCOORD0     )
    INPUT( vec4, mkLocalNormalVec4,   TEXCOORD1     )

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

    OUTPUT( vec4, mWorldPositionVec4,        TEXCOORD0 )
#if defined( D_RINGS ) || defined( D_RAINBOW )
    OUTPUT( vec4, mLocalPositionVec4, TEXCOORD1 )
#else
    OUTPUT( vec4, mTexCoordsVec4,            TEXCOORD1 )
    OUTPUT( vec4, mWorldNormalVec3_mfDistanceFromPlanet,  TEXCOORD2 )
#endif
#ifndef D_REFLECT    
    OUTPUT( vec4, mScreenSpacePositionVec4,  TEXCOORD3 )
#endif    

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
VERTEX_MAIN_SRT
{
    vec4 lWorldPositionVec4;

    lWorldPositionVec4 = CalcWorldPos( lUniforms.mpCommonPerMesh->gWorldMat4, vec4( IN( mkLocalPositionVec4 ).xyz, 1.0 ) );

    // Resize sphere to correct radius
#if defined( D_CLOUD ) || defined( D_SHADOW ) || defined( D_PLANET )
    {
        float lfSize = 1.0;

        #if defined( D_SHADOW ) 
            lfSize = 1.005;
        #elif defined( D_CLOUD ) 
            lfSize = 1.01;
#endif

    lWorldPositionVec4.xyz  = normalize( lWorldPositionVec4.xyz - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz ) * ( lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w * lfSize );
    lWorldPositionVec4.xyz += lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz;
    }
#endif

    OUT( mWorldPositionVec4 )       = lWorldPositionVec4;

#if defined( D_RINGS ) 
    OUT( mLocalPositionVec4 ).xyz                    = lWorldPositionVec4.xyz - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz;
    OUT( mLocalPositionVec4 ).w                      = length( IN( mkLocalPositionVec4 ).xyz );
#elif defined( D_RAINBOW )
    OUT( mLocalPositionVec4 )                        = vec4( IN( mkLocalPositionVec4 ).xyz, 1.0 );
#else
    OUT( mWorldNormalVec3_mfDistanceFromPlanet ).xyz = normalize( lWorldPositionVec4.xyz - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz );
    OUT( mWorldNormalVec3_mfDistanceFromPlanet ).w   =  length( lUniforms.mpPerFrame->gViewPositionVec3 - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz ) - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w;

    OUT( mTexCoordsVec4 )           = IN( mkTexCoordsVec4 );
#endif

    vec4 lScreenSpacePositionVec4 = CalcScreenPosFromWorld( lUniforms.mpCommonPerMesh->gViewProjectionMat4, lWorldPositionVec4 );

#ifdef D_REFLECT
    {
        //SCREEN_POSITION = CalcDualParaboloidScreenPosition( gCommon.gViewMat4, vec4(lWorldPositionVec4.x, lWorldPositionVec4.y - 128.0, lWorldPositionVec4.z, lWorldPositionVec4.w), gCommon.gClipPlanesVec4.xy );
        SCREEN_POSITION = CalcDualParaboloidScreenPosition( lUniforms.mpPerFrame->gViewMat4, lWorldPositionVec4, lUniforms.mpPerFrame->gClipPlanesVec4.xy );
    }
#else
    {
        OUT( mScreenSpacePositionVec4 ) = lScreenSpacePositionVec4;
        SCREEN_POSITION = lScreenSpacePositionVec4;
    }
#endif
}
