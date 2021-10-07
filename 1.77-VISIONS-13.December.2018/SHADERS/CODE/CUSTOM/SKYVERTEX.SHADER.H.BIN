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


//-----------------------------------------------------------------------------
//      Include files


#ifndef D_VERTEX
#define D_VERTEX
#endif

#include "Common/Defines.shader.h"
#include "Common/CommonUniforms.shader.h"
#include "UberCommon.h"

#include "Common/CommonVertex.shader.h"
#include "Common/CommonPlanet.shader.h"
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
    OUTPUT( vec4, mLocalPositionVec4,        TEXCOORD1 )
#if !defined( D_LOWQUALITY )
    OUTPUT( vec4, mScreenSpacePositionVec4,  TEXCOORD2 )
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
    vec3 lDirectionVec3;
    vec4 lWorldPositionVec4;
    vec4 lFlatWorldPositionVec4;
    vec3 lWorldNormalVec3;

    lWorldPositionVec4        = CalcWorldPos( lUniforms.mpCommonPerMesh->gWorldMat4, vec4( IN( mkLocalPositionVec4 ).xyz, 1.0 ) );

    OUT( mLocalPositionVec4 ) = IN( mkLocalPositionVec4 );
    OUT( mWorldPositionVec4 ) = lWorldPositionVec4;
    //OUT( mSunDirectionVec3 )  = normalize( lWorldPositionVec4.xyz - lUniforms.mpPerFrame->gViewPositionVec3 );
    //OUT( mTexCoordsVec2 )     = IN( mkTexCoordsVec4 ).xy;

    #ifdef D_REFLECT
    {
        //SCREEN_POSITION = CalcDualParaboloidScreenPosition( gCommon.gViewMat4, vec4(lWorldPositionVec4.x, lWorldPositionVec4.y - 128.0, lWorldPositionVec4.z, lWorldPositionVec4.w), gCommon.gClipPlanesVec4.xy );
        SCREEN_POSITION = CalcDualParaboloidScreenPosition( lUniforms.mpPerFrame->gViewMat4, lWorldPositionVec4, lUniforms.mpPerFrame->gClipPlanesVec4.xy );
    }
    #else
    {   
        vec4 lScreenSpacePositionVec4 = MUL(lUniforms.mpCommonPerMesh->gViewProjectionMat4, lWorldPositionVec4);
#if !defined( D_LOWQUALITY )
        OUT( mScreenSpacePositionVec4 ) = lScreenSpacePositionVec4;
#endif
        SCREEN_POSITION = lScreenSpacePositionVec4;
    }
    #endif
}