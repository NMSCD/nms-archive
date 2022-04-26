////////////////////////////////////////////////////////////////////////////////
///
///     @file       CommonFade.h
///     @author     User
///     @date       
///
///     @brief      CommonFogShader
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 

#ifndef D_COMMONFADE_H
#define D_COMMONFADE_H

//-----------------------------------------------------------------------------
//      Include files

#include "Common/CommonUniforms.shader.h"

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Functions


//-----------------------------------------------------------------------------
///
///     Hash
///
///     @brief      Hash
///
///     @param      vec2 lPositionVec2 
///     @return     float
///
//-----------------------------------------------------------------------------
float 
Hash( 
    vec3 lPositionVec3 )
{
    return fract(sin(dot(lPositionVec3,vec3(1.0,113.0,337.0)))*43758.5453123);
}

//-----------------------------------------------------------------------------
///
///     CalculateFadeValue
///
///     @brief      Hash
///
///     @param      vec2 lPositionVec2 
///     @return     float
///
//-----------------------------------------------------------------------------
void
CheckFade(
    SAMPLER2DARG( lFadeNoiseMap ),
    vec4          lScreenSpacePositionVec4,
    float         lfCommonFadeValue,
    vec2          lScreenSizeVec2 )
{
	if (lfCommonFadeValue < 1.0)
	{
        vec2  lScreenVal  = ( ( lScreenSpacePositionVec4.xy / lScreenSpacePositionVec4.w ) + 1.0 ) * float2vec2( 0.5 );

        lScreenVal *= vec2( lScreenSizeVec2.x / 512.0, lScreenSizeVec2.y / 512.0 );

	    float lfScreenVal = texture2DLod( lFadeNoiseMap, lScreenVal, 0.0 ).x;
		
	    float lfFadeValue = lfCommonFadeValue;
	
	    if( lfFadeValue < 0.0 )
	    {
	        // Fade out
	        if( ( 1.0-lfScreenVal ) > ( lfFadeValue+2.0 ) )
	        {
	            discard;
	        }
	    }
	    else
	    {
	        // Fade in
	        if( lfScreenVal > lfFadeValue )
	        {
	            discard;
	        }
	    }
	}
}

//-----------------------------------------------------------------------------
///
///     CheckFadeFragCoord
///
///     @brief      Runs fade with gl_fragCoord or SV_POSITION as input
///
///     @param      vec2 lPositionVec2 
///     @return     float
///
//-----------------------------------------------------------------------------
void
CheckFadeFragCoord(
    SAMPLER2DARG( lFadeNoiseMap ),
    vec4          lFragCoordVec4,
    float         lfCommonFadeValue )
{
    if (lfCommonFadeValue < 1.0)
    {
        vec2  lScreenVal  = lFragCoordVec4.xy;

        lScreenVal *= vec2( 1.0 / 512.0, 1.0 / 512.0 );

        float lfScreenVal = texture2DLod( lFadeNoiseMap, lScreenVal, 0.0 ).x;
        
        float lfFadeValue = lfCommonFadeValue;
    
        if( lfFadeValue < 0.0 )
        {
            // Fade out
            if( ( 1.0-lfScreenVal ) > ( lfFadeValue+2.0 ) )
            {
                discard;
            }
        }
        else
        {
            // Fade in
            if( lfScreenVal > lfFadeValue )
            {
                discard;
            }
        }
    }
}


//-----------------------------------------------------------------------------
///
///     CalculateFadeValue
///
///     @brief      Hash
///
///     @param      vec2 lPositionVec2 
///     @return     float
///
//-----------------------------------------------------------------------------
void 
CheckHeightFade( 
    SAMPLER2DARG( lNoiseMap ),
    vec4    lScreenSpacePositionVec4,
    float   lfHeight,
    float   lfCommonFadeValue )
{
    vec2  lScreenVal  = ((lScreenSpacePositionVec4.xy / lScreenSpacePositionVec4.w) * 0.5) + 0.5;
    float lfScreenVal = texture2D( lNoiseMap, lScreenVal*vec2(5.0,3.0) ).x;

    float lfFadeValue = lfCommonFadeValue * lfHeight;

    if( lfFadeValue < 0.0 )
    {
        // Fade out
        if( (1.0-lfScreenVal) > (lfFadeValue+2.0) )
        {
            discard;
        }
    }
    else
    {
        // Fade in
        if( lfScreenVal > lfFadeValue )
        {
            discard;
        }
    }
}

//-----------------------------------------------------------------------------
///
///     CheckFixedFade
///
///     @brief      Hash
///
///     @param      vec2 lPositionVec2 
///     @return     float
///
//-----------------------------------------------------------------------------
void 
CheckFixedFade(
    vec4 lScreenSpacePositionVec4,
    float lfFadeValue )
{
    //vec4 lDitherCoordsVec4 = lScreenSpacePositionVec4;
    //lDitherCoordsVec4.xy /= lDitherCoordsVec4.w;
    //lDitherCoordsVec4.x *= 1280.0 * 0.5;
    //lDitherCoordsVec4.y *= 750.0 * 0.5;

    //float lfDither = Hash( lDitherCoordsVec4.xy );
    //if( lfDither > lfFadeValue )
    //{
    //    discard;
    //}
}

#endif