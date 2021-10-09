////////////////////////////////////////////////////////////////////////////////
///
///     @file       PostProcessFragment.h
///     @author     User
///     @date       
///
///     @brief      DepthOfFieldFragmentShader
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 

#ifndef D_FRAGMENT
#define D_FRAGMENT
#endif

//-----------------------------------------------------------------------------
//      Include files
#include "Common/Defines.shader.h"
#include "Common/CommonUniforms.shader.h"
#include "Common/Common.shader.h"


//-----------------------------------------------------------------------------
///
///     CustomPerMaterialUniforms
///
///     @brief      CustomPerMaterialUniforms
///
///     Stuff that is only used for these types of meshes.
//-----------------------------------------------------------------------------

#include "Fullscreen/PostCommon.h"

#include "Common/CommonPostProcess.shader.h"
#include "Common/CommonDepth.shader.h"

// must include this after CommonUniforms
//#include "Fullscreen/PostCommon.h"
#include "Common/CommonHBAO.shader.h"
#include "Common/CommonGBuffer.shader.h"

//-----------------------------------------------------------------------------
//      Const Data


// =================================================================================================
//
// COPY
//
// =================================================================================================

//#ifdef D_SSAO

//-----------------------------------------------------------------------------
//      Global Data



//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
INPUT_SCREEN_POSITION

INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END

#if defined( D_PLATFORM_ORBIS )
#pragma argument(targetoccupancy_atallcosts=90)
#pragma argument(unrollallloops)
#elif defined( D_PLATFORM_DX12 )
//#define __XBOX_REGALLOC_FORCE_VGPR_LIMIT 64
#else
#pragma optionNV(unroll all)
#endif





//-----------------------------------------------------------------------------
//      Functions 


FRAGMENT_MAIN_COLOUR_SRT
{
    vec2  lFragCoordsVec2 = TEX_COORDS;
    //vec2  lFragCoordsVec2 = IN(mScreenPositionVec4).xy;
    //float lfDepth = lUniforms.mpCustomPerMesh->gBuffer1Map.Load(int3(lFragCoordsVec2,0.0));
    //lFragCoordsVec2*=lUniforms.mpPerFrame->gFrameBufferSizeVec4.zw;
    
    vec4 lFragCol = HBAO(   SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMesh,gBuffer1Map),
                            SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMesh,gBuffer2Map),
                            lFragCoordsVec2,
                            lUniforms.mpPerFrame->gClipPlanesVec4,
                            lUniforms.mpPerFrame->gFoVValuesVec4,
                            lUniforms.mpPerFrame->gFrameBufferSizeVec4,
                            lUniforms.mpPerFrame->gViewPositionVec3,
                            lUniforms.mpPerFrame->gCameraMat4,
                            lUniforms.mpCommonPerMesh->gInverseProjectionSCMat4,
                            lUniforms.mpCustomPerMesh->gHBAOParamsVec4
                            );


    FRAGMENT_COLOUR = lFragCol;

    //FRAGMENT_COLOUR = vec4( ViewNormal, 1.0 );
    /*
    if ( RadiusPixels <= 0.0 )
    {
        FRAGMENT_COLOUR = ( vec4( 0.0, 0.0, 0.0, 1.0 ) );
    }
    else
    if ( RadiusPixels < 25.0 )
    {
        FRAGMENT_COLOUR = ( vec4( 1.0, 1.0, 0.0, 1.0 ) );
    }
    else
    if ( RadiusPixels < 50.0 )
    {
        FRAGMENT_COLOUR = ( vec4( 1.0, 0.0, 1.0, 1.0 ) );
    }
    else
    if ( RadiusPixels < 75.0 )
    {
        FRAGMENT_COLOUR = (vec4(1.0,0.0,0.0, 1.0));
    }
    else
    if ( RadiusPixels < 100.0 )
    {
        FRAGMENT_COLOUR = (vec4(0.0,0.0,1.0, 1.0));
    }
    else
    if ( RadiusPixels < 125.0 )
    {
        FRAGMENT_COLOUR = (vec4(1.0,0.0,1.0, 1.0));
    }
    else
    if ( RadiusPixels < 150.0 )
    {
        FRAGMENT_COLOUR = (vec4(1.0,1.0,0.0, 1.0));
    }
    else
    if ( RadiusPixels < 175.0 )
    {
        FRAGMENT_COLOUR = (vec4(0.0,1.0,1.0, 1.0));
    }
    else
    {
        FRAGMENT_COLOUR = (vec4(0.0,1.0,0.0, 1.0));
    }
    */
    
    /*
    if ( lFragCoordsVec2.x < 0.5 )
    {
        if ( lFragCoordsVec2.y < 0.5 )
        {
            FRAGMENT_COLOUR = Rand;
        }
        else
        {
            FRAGMENT_COLOUR = vec4( lUniforms.mpPerFrame->gViewPositionVec3, 0 );
        }
    }
    else
    {
        if ( lFragCoordsVec2.y < 0.5 )
        {
            FRAGMENT_COLOUR = vec4( vec3( RadiusPixels ), 0.0 );
        }
        else
        {
            FRAGMENT_COLOUR = vec4( vec3( pow( AO, PowExponent ) ), 0.0 );
        }
    }
    */

}

//#endif

	

