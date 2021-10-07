////////////////////////////////////////////////////////////////////////////////
///
///     @file       SkyFragment.h
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

#define D_USE_NOISETEXTURE

#ifdef D_PLATFORM_ORBIS
#pragma argument(targetoccupancy_atallcosts=80)
#pragma argument(unrollallloops)
#endif

//-----------------------------------------------------------------------------
//      Include files

#ifndef D_FRAGMENT
#define D_FRAGMENT
#endif
#include "Common/Defines.shader.h"
#include "Common/CommonUniforms.shader.h"
#include "UberCommon.h"
#include "Common/CommonGBuffer.shader.h"
#include "Common/Common.shader.h"
#include "Common/CommonPlanet.shader.h"
#include "Common/CommonScattering.shader.h"
#include "Common/CommonNoise.shader.h"

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
    
    INPUT( vec4, mWorldPositionVec4,        TEXCOORD0 )
    INPUT( vec4, mLocalPositionVec4,        TEXCOORD1 )
#if !defined( D_LOWQUALITY )
    INPUT( vec4, mScreenSpacePositionVec4,  TEXCOORD2 )
#endif

DECLARE_END


//-----------------------------------------------------------------------------
//    Functions


#if defined( D_SKY_CLEAR )

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
    vec3  lLocalPositionVec3 = normalize(IN(mLocalPositionVec4).xyz);

    float lfCloudScale = lUniforms.mpCustomPerMaterial->gSpaceNebulaColour3Vec4.a;
    float lfCloudOffset = lUniforms.mpCustomPerMaterial->gSpaceSkyColour1Vec4.a;
    float lfCloudBrightness = lUniforms.mpCustomPerMaterial->gSpaceNebulaParamsVec4.a;

    float lfNebulaCoverageScale = lUniforms.mpCustomPerMaterial->gSpaceNebulaColour4Vec4.a;
    float lfNebulaCoverageStrength = lUniforms.mpCustomPerMaterial->gSpaceNormalParamsVec4.a;
    float lfNebulaOffset = lUniforms.mpCustomPerMaterial->gSpaceNebulaColour2Vec4.a;

    float lfRandom1 = lUniforms.mpCustomPerMaterial->gSpaceCloudColourVec4.a * 100.0f;
    float lfCloudNoise = FractBrownianMotion4(lLocalPositionVec3 * lfCloudScale + float2vec3(lfRandom1), SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMaterial,gNoiseMap));
    float lfEncodedCloudNoise = lfCloudNoise * 0.5 + 0.5;

    lfCloudNoise = smoothstep(0.0, 1.1, lfCloudNoise * lfCloudBrightness + lfCloudOffset);
    float lfNebulaCoverage = 0.0;
    if ((lfCloudNoise > (1.0 / 1024.0)) && (lfNebulaOffset > -1.0))
    {
        vec3 lRandom3 = float2vec3(lfRandom1 + 1000.0f) + lLocalPositionVec3 * sin(lUniforms.mpPerFrame->gfTime * 0.001);
        lfNebulaCoverage = clamp(FractBrownianMotion4(lLocalPositionVec3 * lfNebulaCoverageScale + lRandom3, SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMaterial,gNoiseMap)) + lfNebulaCoverageStrength, 0.0, 1.0);
    }

    FRAGMENT_COLOUR0 = vec4(0.0, 0.0, 0.0, 0.0);
    FRAGMENT_COLOUR1 = vec4(1.0, 1.0, 1.0, 1.0);
    FRAGMENT_COLOUR2 = vec4(lfEncodedCloudNoise, lfNebulaCoverage, 0.0, 0.0);
    FRAGMENT_COLOUR3 = vec4(float(D_UNLIT) / 255.0, 0.0, 0.0, 0.0);
}

#elif defined( D_SCRATCHPAD )

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

#include "Common/CommonLighting.shader.h"

FRAGMENT_MAIN_COLOUR_SRT
{

    mat3 lUpMatrix = GetInverseWorldUpTransform( lUniforms.mpPerFrame->gViewPositionVec3, lUniforms.mpCommonPerMesh->gLightOriginVec4 );	

    vec3 lTransformedNormalVec3    = MUL( lUpMatrix, -normalize( IN( mWorldPositionVec4 ).xyz ) );

    vec3 SpecularIBL = GetImageBasedReflectionLighting( SAMPLER2DPARAM_SRT( 
        lUniforms.mpCustomPerMaterial,gDualPMapBack ),
        SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMaterial,gDualPMapFront ),
        lTransformedNormalVec3,
        0.0 );

    FRAGMENT_COLOUR = vec4( SpecularIBL, 1.0 );
}

#else

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
    vec3  lLightPositionVec3 = lUniforms.mpCustomPerMaterial->gSunPositionVec4.xyz;

    vec3  lViewPositionVec3 = float2vec3( 0.0 );
    vec3  lWorldPositionVec3 = normalize( IN( mWorldPositionVec4 ).xyz - lUniforms.mpPerFrame->gViewPositionVec3 );
    vec3  lLocalPositionVec3 = normalize( IN( mLocalPositionVec4 ).xyz );

    // Get params from uniforms
    float lfAtmosphereSize = lUniforms.mpCustomPerMaterial->gScatteringParamsVec4.b;


    vec3  lSunWavelengthVec3 = ( lUniforms.mpCommonPerMesh->gLightColourVec4.rgb );


    vec3  lSpaceSkyColour1Vec3     = lUniforms.mpCustomPerMaterial->gSpaceSkyColour1Vec4.rgb;
    vec3  lSpaceSkyColour2Vec3     = lUniforms.mpCustomPerMaterial->gSpaceSkyColour2Vec4.rgb;
    vec3  lSpaceSkyColour3Vec3     = lUniforms.mpCustomPerMaterial->gSpaceSkyColour3Vec4.rgb;

    float lfSpaceSunSize           = lUniforms.mpCustomPerMaterial->gSpaceSkyColour3Vec4.a;
    float lfSpaceCenterPowerVec3   = lUniforms.mpCustomPerMaterial->gSpaceSkyColour2Vec4.a;
                                   
    vec3  lSunColour               = lUniforms.mpCustomPerMaterial->gSpaceSunColourVec4.rgb;

    vec4 lFragmentColourVec4 = float2vec4( 1.0 );

    lFragmentColourVec4.rgb = saturate( GetSpaceColour(
        lWorldPositionVec3,
        lViewPositionVec3,
        lLightPositionVec3,
        lSpaceSkyColour1Vec3,
        lSpaceSkyColour2Vec3,
        lSpaceSkyColour3Vec3,
        lfSpaceCenterPowerVec3 ) );


    vec3 lSunScatteringVec3 = InScatteringPhase(
        float2vec3( 1.0 ),
        lSunColour,
        1.0,
        1.0,
        MiePhasePN( lLightPositionVec3, lWorldPositionVec3, lfSpaceSunSize ) );
    

    {
        vec3  lSpaceCloudColourVec3    = lUniforms.mpCustomPerMaterial->gSpaceCloudColourVec4.rgb;
        vec3  lSpaceNebulaColour1Vec3  = lUniforms.mpCustomPerMaterial->gSpaceNebulaColour1Vec4.rgb;
        vec3  lSpaceNebulaColour2Vec3  = lUniforms.mpCustomPerMaterial->gSpaceNebulaColour2Vec4.rgb;
        vec3  lSpaceNebulaColour3Vec3  = lUniforms.mpCustomPerMaterial->gSpaceNebulaColour3Vec4.rgb;
        vec3  lSpaceNebulaColour4Vec3  = lUniforms.mpCustomPerMaterial->gSpaceNebulaColour4Vec4.rgb;

        float lfRandom1                = lUniforms.mpCustomPerMaterial->gSpaceCloudColourVec4.a * 100.0f;
        float lfRandom2                = lUniforms.mpCustomPerMaterial->gSpaceCloudColourVec4.a * 100.0f;
                                       
        float lfScale1                 = lUniforms.mpCustomPerMaterial->gSpaceNebulaColour1Vec4.a;
        float lfScale2                 = lfScale1;

        float lfNebulaOffset           = lUniforms.mpCustomPerMaterial->gSpaceNebulaColour2Vec4.a;
        float lfCloudScale             = lUniforms.mpCustomPerMaterial->gSpaceNebulaColour3Vec4.a;
        float lfCloudOffset            = lUniforms.mpCustomPerMaterial->gSpaceSkyColour1Vec4.a;

        float lfNebulaCoverageScale    = lUniforms.mpCustomPerMaterial->gSpaceNebulaColour4Vec4.a;
        float lfNebulaCoverageStrength = lUniforms.mpCustomPerMaterial->gSpaceNormalParamsVec4.a;

        float lfShadowOffset           = lUniforms.mpCustomPerMaterial->gSpaceNebulaParamsVec4.r;
        float lfNebulaBrightness       = lUniforms.mpCustomPerMaterial->gSpaceNebulaParamsVec4.g;
        float lfAnimationSpeed         = lUniforms.mpCustomPerMaterial->gSpaceNebulaParamsVec4.b;
        float lfCloudBrightness        = lUniforms.mpCustomPerMaterial->gSpaceNebulaParamsVec4.a;


        vec3 lRandom2   = float2vec3( lfRandom2 )            + lLocalPositionVec3 * sin( lUniforms.mpPerFrame->gfTime * lfAnimationSpeed );
        vec3 lRandom3   = float2vec3( lfRandom1 + 1000.0f  ) + lLocalPositionVec3 * sin( lUniforms.mpPerFrame->gfTime * 0.001 );

        float lfNebula          = 0.0;
        vec3  lNebulaColourVec3 = float2vec3(0.0);

#if !defined( D_LOWQUALITY )

        vec2 lFragCoordsVec2 = SCREENSPACE_AS_RENDERTARGET_UVS((IN(mScreenSpacePositionVec4).xy / IN(mScreenSpacePositionVec4).w) * 0.5 + 0.5);
        vec2 lfCloudNoiseAndCoverage = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial,gBufferMap ), lFragCoordsVec2.xy).xy;
        float lfCloudNoise = lfCloudNoiseAndCoverage.x * 2.0 - 1.0;

        lfCloudNoise = smoothstep(0.0, 1.1, lfCloudNoise * lfCloudBrightness + lfCloudOffset);
        if ((lfCloudNoise > (1.0 / 1024.0)) && (lfNebulaOffset > -1.0))
        {
            float lfNebulaCoverage = lfCloudNoiseAndCoverage.y;

#else

        float lfCloudNoise = FractBrownianMotion4(lLocalPositionVec3 * lfCloudScale + float2vec3(lfRandom1), SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMaterial,gNoiseMap));
        lfCloudNoise = smoothstep(0.0, 1.1, lfCloudNoise * lfCloudBrightness + lfCloudOffset);

        if ((lfCloudNoise > (1.0 / 1024.0)) && (lfNebulaOffset > -1.0))
        {
            float lfNebulaCoverage = clamp(FractBrownianMotion4(lLocalPositionVec3 * lfNebulaCoverageScale + lRandom3, SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMaterial,gNoiseMap)) + lfNebulaCoverageStrength, 0.0, 1.0);

#endif
            float lfNebulaNoise    = max( (lfCloudNoise + lfNebulaOffset ) / (1.0 + lfNebulaOffset) * lfNebulaCoverage, 0.0 ) * lfNebulaBrightness;
            if( lfNebulaNoise > (1.0 / 1024.0) )
            {
                lfNebula        = DistortedNoise( lLocalPositionVec3 + lRandom2, lfScale1, lfScale2, SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMaterial,gNoiseMap ) );
                float lfShadow  = DistortedNoise( lLocalPositionVec3 * lfShadowOffset + lRandom2, lfScale1, lfScale2, SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMaterial,gNoiseMap ) );
                lfNebula        = lfNebula * lfNebula * lfNebula * lfNebula * lfNebula;
                lfShadow        = lfShadow * lfShadow * lfShadow;
                       
                lfNebula        = saturate( lfNebula );
                lfShadow        = saturate( lfShadow );

                lfNebula        *= lfNebulaNoise;
                lfShadow        *= lfNebulaNoise;

                float lfBlend1 = lUniforms.mpCustomPerMaterial->gSpaceNormalParamsVec4.r;
                float lfBlend2 = lUniforms.mpCustomPerMaterial->gSpaceNormalParamsVec4.g;
                float lfBlend3 = lUniforms.mpCustomPerMaterial->gSpaceNormalParamsVec4.b;
                float lfBlend4 = 1.0;

                float lfNoiseBlend1    = clamp( ( lfNebula - lfBlend1 ) / ( lfBlend2 - lfBlend1 ), 0.0, 1.0 );
                float lfNoiseBlend2    = clamp( ( lfNebula - lfBlend3 ) / ( lfBlend4 - lfBlend3 ), 0.0, 1.0 );
                lNebulaColourVec3      = mix( lSpaceNebulaColour2Vec3, lSpaceNebulaColour3Vec3 * 3.0, lfNoiseBlend2 );
                lNebulaColourVec3      = mix( lSpaceNebulaColour1Vec3, lNebulaColourVec3, lfNoiseBlend1 );

                lSpaceCloudColourVec3 -= lfShadow * lSpaceNebulaColour4Vec3;
                lSpaceCloudColourVec3 += lfNebula * lNebulaColourVec3;
            }
            lFragmentColourVec4.rgb = mix(lFragmentColourVec4.rgb, lSpaceCloudColourVec3, lfCloudNoise);
            vec3 lSunScattering2Vec3 = float2vec3(MiePhasePN(lLightPositionVec3, lWorldPositionVec3, 0.9) * 0.2);
            lFragmentColourVec4.rgb += (lSpaceCloudColourVec3 * lfCloudNoise) * lSunScattering2Vec3 * 0.3;
        }   
    }

    lFragmentColourVec4.rgb += clamp( lSunScatteringVec3, 0.0, 64.0);
    lFragmentColourVec4.a = 0.0; // cause sky to replace what's under it in HDRBUF

    FRAGMENT_COLOUR = lFragmentColourVec4;
}

#endif


