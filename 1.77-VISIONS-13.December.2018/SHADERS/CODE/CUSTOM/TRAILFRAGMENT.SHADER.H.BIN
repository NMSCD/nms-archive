#define D_PARTICLE_UNIFORMS

#ifndef D_FRAGMENT
#define D_FRAGMENT
#endif
#include "Common/Defines.shader.h"
#include "Common/CommonUniforms.shader.h"

#include "Custom/TrailCommon.h"


//-----------------------------------------------------------------------------
///
///     Input
///
///     @brief  Input
///
//-----------------------------------------------------------------------------
DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2,  mfTexCoord, TEXCOORD1 )
    INPUT( float, mfFade,     TEXCOORD2 )

DECLARE_END

FRAGMENT_MAIN_COLOUR_SRT
{
    vec4 lCol;
    lCol = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gDiffuseMap ), IN( mfTexCoord ) );
    lCol.w *= IN( mfFade );

    FRAGMENT_COLOUR = lCol;
}