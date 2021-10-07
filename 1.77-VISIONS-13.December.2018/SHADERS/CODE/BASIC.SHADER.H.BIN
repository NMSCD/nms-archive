////////////////////////////////////////////////////////////////////////////////
///
///     @file       
///     @author     User
///     @date       
///
///     @brief      
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 


//-----------------------------------------------------------------------------
//      Include files

#include "Common/Defines.shader.h"
#include "Common/CommonUniforms.shader.h"

struct UniformBuffer
{
    DECLARE_PTR( PerFrameUniforms,          mpPerFrame )       /*: PER_MATERIAL*/
    DECLARE_PTR( CommonPerMeshUniforms,     mpCommonPerMesh )           /*: PER_MESH*/
};


#ifdef D_VERTEX

//-----------------------------------------------------------------------------
//      Compilation defines 


//-----------------------------------------------------------------------------
//      Include files

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
    INPUT(  vec4, mkLocalPositionVec4, POSITION0 )
DECLARE_END

DECLARE_OUTPUT
    OUTPUT_SCREEN_POSITION

    OUTPUT( vec4, mSceenPosVec4, TEXCOORD0 )
DECLARE_END

//-----------------------------------------------------------------------------
///
///     Output
///
///     @brief  Output
///
//-----------------------------------------------------------------------------


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
    vec4 lWorldPositionVec4       = MUL( lUniforms.mpCommonPerMesh->gWorldMat4, vec4(IN(mkLocalPositionVec4).xyz, 1.0) );
    vec4 lScreenSpacePositionVec4 = MUL( lUniforms.mpCommonPerMesh->gViewProjectionMat4, lWorldPositionVec4 );

    OUT(mSceenPosVec4)       = lScreenSpacePositionVec4;

    SCREEN_POSITION   = lScreenSpacePositionVec4;
}


#endif

#ifdef D_FRAGMENT

//-----------------------------------------------------------------------------
//      Global Data

//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec4, mSceenPosVec4, TEXCOORD0 )
DECLARE_END

//-----------------------------------------------------------------------------
//    Functions

//-----------------------------------------------------------------------------
///
///     Main
///
///     @brief      Fragment Main
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
           
FRAGMENT_MAIN_COLOUR_SRT
{
    FRAGMENT_COLOUR = vec4( 1.0, 0.0, 0.0, 1.0 );
}

#endif


