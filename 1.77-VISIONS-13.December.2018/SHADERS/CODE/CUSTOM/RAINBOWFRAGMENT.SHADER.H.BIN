////////////////////////////////////////////////////////////////////////////////
///
///     @file       AtmosphereFragment.h
///     @author     User
///     @date       
///
///     @brief      SkyFragmentShader
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

#include "Custom/AtmosphereCommon.h"


#include "Common/Common.shader.h"
#include "Common/CommonDepth.shader.h"
#include "Common/CommonPlanet.shader.h"


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

INPUT( vec4, mWorldPositionVec4, TEXCOORD0 )
INPUT( vec4, mLocalPositionVec4, TEXCOORD1 )
INPUT( vec4, mScreenSpacePositionVec4, TEXCOORD3 )

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

FRAGMENT_MAIN_COLOUR_SRT
{

    vec4 lFragmentColourVec4 = vec4( 0.0, 0.0, 0.0, 1.0 );

    float lfDistance  = length( IN( mLocalPositionVec4 ).rgb ) / 15.0;
    float lfWidth     = lUniforms.mpCustomPerMaterial->gRainbowParamsVec4.r;
    float lfFadeWidth = lUniforms.mpCustomPerMaterial->gRainbowParamsVec4.b;

    // Colour from texture
    float lfRainbow = 1.0 - ((lfDistance - ( 1.0 - lfWidth) ) / lfWidth);
    vec2 lTexCoordsVec2 = vec2( 0.0, lfRainbow );
    lFragmentColourVec4 = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gRainbowMap ), lTexCoordsVec2 );

    // Fade out at edges
    lFragmentColourVec4.a  = smoothstep( 1.0 - lfWidth, (1.0 - lfWidth) + lfFadeWidth, lfDistance );
    lFragmentColourVec4.a *= 1.0 - smoothstep( 1.0 - lfFadeWidth, 1.0, lfDistance );

    lFragmentColourVec4.a *= lUniforms.mpCustomPerMaterial->gRainbowParamsVec4.g;

    // Fade out close to ground
    vec2 lWindowCoordsVec2      = ( IN( mScreenSpacePositionVec4 ).xy / IN( mScreenSpacePositionVec4 ).w ) * 0.5 + 0.5;
    float lfLinearZDepthBuffer  = FastDenormaliseDepth( lUniforms.mpPerFrame->gClipPlanesVec4, texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gBufferMap ), SCREENSPACE_AS_RENDERTARGET_UVS( lWindowCoordsVec2 ) ).r );
    float lfDepthFade           = saturate( ( lfLinearZDepthBuffer - IN( mScreenSpacePositionVec4 ).w ) * 0.005 );

    lFragmentColourVec4.a *= lfDepthFade;
    
    FRAGMENT_COLOUR = ( lFragmentColourVec4 );
}

