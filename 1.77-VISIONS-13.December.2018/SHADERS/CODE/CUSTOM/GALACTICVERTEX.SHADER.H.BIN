////////////////////////////////////////////////////////////////////////////////
///
///     @file       ImmediateModeVertex.h
///     @author     User
///     @date       
///
///     @brief      ImmediateModeVertex
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
    INPUT(  vec4, mkLocalPositionVec4, POSITION0 )
	INPUT( vec2, mkTexCoordsVec4,     TEXCOORD0 )
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
	OUTPUT( vec2, mTexCoordsVec2,     TEXCOORD0  )

#ifdef D_TEX_QUAD_POS
    OUTPUT( vec4, mWorldPositionVec4, TEXCOORD4 )
#endif

DECLARE_END

#include "Custom/GalaxyCommon.h"


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
    
    vec2 lTexCoordsVec2;
    vec4 lWorldPositionVec4 = vec4( 0.0, 0.0, 0.0, 0.0 );
    
    lTexCoordsVec2 = IN( mkTexCoordsVec4 );    
    lWorldPositionVec4 = MUL( kFSQuadProj, IN( mkLocalPositionVec4 ) );

    OUT( mTexCoordsVec2 ) = lTexCoordsVec2;

#ifdef D_TEX_QUAD_POS
    OUT( mWorldPositionVec4 ) = lWorldPositionVec4;
#endif

    SCREEN_POSITION = lWorldPositionVec4;
}