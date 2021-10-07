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
#include "Common/CommonUniforms.shader.h"

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
    INPUT(  vec2, mkTexCoordsVec4,     TEXCOORD0 )
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
    OUTPUT( vec2, mTexCoordsVec2,     TEXCOORD0 )

#ifdef D_TEX_QUAD_POS
    OUTPUT( vec4, mWorldPositionVec4, TEXCOORD4 )
#endif

DECLARE_END
      

// Uses postcommon as most of the post process stuff uses im3d and the uniform buffers have to 
// match between vertex and pixel shader. We coudl change this if we wanted but it wouldn't
// actually save memory or anything, but maybe it would be useful...?
    
// should pobaly do something less shit here!
#if defined( D_RECOLOUR ) || defined( D_COMBINE )
// use different buffer for recolouring stuff
#include "Fullscreen/RecolourCommon.shader.h"
#elif defined( D_CLOUD_RENDER )
#include "Custom/CloudCommon.h"
#elif defined( D_LIGHT )
#include "LightCommon.shader.h"
#elif defined( D_WATER )
#include "Custom/WaterCommon.h"
#else 
#include "Fullscreen/PostCommon.h"
#endif
    

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

#if defined( D_SCREEN_POS_HALF_PIXEL ) && defined( D_PLATFORM_PC )
    lWorldPositionVec4.xy -= (vec2(0.5, 0.5) * lUniforms.mpPerFrame->gFrameBufferSizeVec4.zw);
#endif
    SCREEN_POSITION = lWorldPositionVec4;
}
