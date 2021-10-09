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

//-----------------------------------------------------------------------------
//      Include files

#ifndef D_FRAGMENT
#define D_FRAGMENT
#endif

#include "Common/Defines.shader.h"
#include "Common/CommonUniforms.shader.h"
#include "Common/CommonScattering.shader.h"
#include "Common/CommonDepth.shader.h"
#include "Common/CommonGBuffer.shader.h"
#include "LightCommon.shader.h"
#include "Common/CommonLighting.shader.h"

#ifdef D_PLATFORM_ORBIS
#pragma argument(unrollallloops)
#elif defined( D_PLATFORM_OPENGL )
#pragma optionNV(unroll all)
#endif

// =================================================================================================
//
// RAYMARCH
//
// =================================================================================================


#define NB_STEPS       16
#define PI  3.1415926535897932384626433832795 

#define TAU    0.0001
#define PHI    10000000.0
// 1.0 / PI
#define PI_RCP 0.31830988618379067153776752674503

/*
float
ComputeFallOff(
in float lfDistance,
in float lfDistanceRecip )
{
float lfFallOff;

lfFallOff = TAU * ( ( PHI * 0.25 * PI_RCP ) * lfDistanceRecip * lfDistanceRecip ) * exp( -lfDistance * TAU ) * exp( -1.0 * TAU );

return lfFallOff;
}
*/

#if defined(D_PLATFORM_DX12)
float
PCF(
in CustomPerMaterialUniforms lPerMaterialUniforms,
in CommonPerMeshUniforms   lUniforms,
in vec3                    lTexCoordVec3,
in vec4                 lShadowMapSize,
in float                lfFilterRadius,
in bool                 lbHQ)
{
#ifdef D_ENABLE_REVERSEZ_PROJECTION
    lTexCoordVec3.z = 1.0 - lTexCoordVec3.z;
#endif
    
    float shadow = shadow2D(gShadowMap, lTexCoordVec3).x;
    if (!lbHQ)
    {
        return shadow;
    }

    vec3 lTexOffsetCoordVec3 = lTexCoordVec3;

    const float offsetx = lfFilterRadius * (lShadowMapSize.z / D_NUMBER_OF_CASCADES);
    const float offsety = lfFilterRadius * (lShadowMapSize.w);

    lTexOffsetCoordVec3.xy = lTexCoordVec3.xy + vec2(-1.0 * offsetx, 0.0);
    shadow += shadow2D(gShadowMap, lTexOffsetCoordVec3).x;

    lTexOffsetCoordVec3.xy = lTexCoordVec3.xy + vec2(1.0 * offsetx, 0.0);
    shadow += shadow2D(gShadowMap, lTexOffsetCoordVec3).x;

    lTexOffsetCoordVec3.xy = lTexCoordVec3.xy + vec2(0.0, -1.0 * offsety);
    shadow += shadow2D(gShadowMap, lTexOffsetCoordVec3).x;

    lTexOffsetCoordVec3.xy = lTexCoordVec3.xy + vec2(0.0, 1.0 * offsety);
    shadow += shadow2D(gShadowMap, lTexOffsetCoordVec3).x;

    return shadow * (1.0 / 5.0);
}

#else

float
PCF(
in CustomPerMaterialUniforms lPerMaterialUniforms,
in CommonPerMeshUniforms   lUniforms,
in vec3                    lTexCoordVec3,
in vec4                 lShadowMapSize,
in float                lfFilterRadius,
in bool                 lbHQ)
{
    #ifdef D_ENABLE_REVERSEZ_PROJECTION
        lTexCoordVec3.z = 1.0 - lTexCoordVec3.z;
    #endif
    
    float shadow = shadow2D(lPerMaterialUniforms.gShadowMap, lTexCoordVec3).x;
    if (!lbHQ)
    {
        return shadow;
    }
    vec3 lTexOffsetCoordVec3 = lTexCoordVec3;

    const float offsetx = lfFilterRadius * (lShadowMapSize.z / D_NUMBER_OF_CASCADES);
    const float offsety = lfFilterRadius * (lShadowMapSize.w);

    lTexOffsetCoordVec3.xy = lTexCoordVec3.xy + vec2(-1.0 * offsetx, 0.0);
    shadow += shadow2D(lPerMaterialUniforms.gShadowMap, lTexOffsetCoordVec3).x;

    lTexOffsetCoordVec3.xy = lTexCoordVec3.xy + vec2(1.0 * offsetx, 0.0);
    shadow += shadow2D(lPerMaterialUniforms.gShadowMap, lTexOffsetCoordVec3).x;

    lTexOffsetCoordVec3.xy = lTexCoordVec3.xy + vec2(0.0, -1.0 * offsety);
    shadow += shadow2D(lPerMaterialUniforms.gShadowMap, lTexOffsetCoordVec3).x;

    lTexOffsetCoordVec3.xy = lTexCoordVec3.xy + vec2(0.0, 1.0 * offsety);
    shadow += shadow2D(lPerMaterialUniforms.gShadowMap, lTexOffsetCoordVec3).x;

    return shadow * (1.0 / 5.0);
}

#endif


float
ComputeShadowIntensityPCF(
in CustomPerMaterialUniforms   lPerMaterialUniforms,
in PerFrameUniforms            lRenderTargetUniforms,
in CommonPerMeshUniforms       lUniforms,
in vec3                        lPositionVec3)
{
    vec3   lProjectedPos;
    float lfTexelSize = lRenderTargetUniforms.gShadowSizeVec4.w; 
    lProjectedPos = MUL(lUniforms.gaShadowMat4[0], vec4(lPositionVec3, 1.0)).xyz;

    // Check if you are outside the high detail shadow cascade
    if (!D_INSIDE_SM_BOUNDS1((lProjectedPos.xyz), lfTexelSize))
    {
        lProjectedPos = MUL(lUniforms.gaShadowMat4[1], vec4(lPositionVec3, 1.0)).xyz;

        if (!D_INSIDE_SM_BOUNDS2((lProjectedPos.xyz), lfTexelSize))
        {
            lProjectedPos = MUL(lUniforms.gaShadowMat4[2], vec4(lPositionVec3, 1.0)).xyz;

            if (!D_INSIDE_SM_BOUNDS3((lProjectedPos.xyz), lfTexelSize))
            {
                // outside all cascades
                return 1.0;
            }
            else
            {   // inside cascade 2
                lProjectedPos.x += 2.0;
                lProjectedPos.x = (lProjectedPos.x * D_CASCADE_SIZE);
                return PCF(lPerMaterialUniforms, lUniforms, lProjectedPos, lRenderTargetUniforms.gShadowSizeVec4, 0.5, false);
            }
        }
        else
        {
            // inside cascade 1
            lProjectedPos.x += 1.0;
        }
    }

    lProjectedPos.x = (lProjectedPos.x * D_CASCADE_SIZE);

    return PCF(lPerMaterialUniforms, lUniforms, lProjectedPos, lRenderTargetUniforms.gShadowSizeVec4, 0.5, true);

}


// Mie scaterring approximated with Henyey-Greenstein phase function.
float
ComputeScattering(
float lfLightDotView,
float lfScattering)
{
    float result = 1.0f - lfScattering;
    result *= result;
    result /= (4.0f * PI * pow(1.0f + lfScattering * lfScattering - (2.0f * lfScattering) * lfLightDotView, 1.5f));
    return result;
}


//STATIC_CONST float kfMaxRayMarchDistance = 100.0;

float
Bayer(
uvec2 lPos)
{
    const mat4 bayer = mat4(
        vec4(1, 9, 3, 11),
        vec4(13, 5, 15, 7),
        vec4(4, 12, 2, 10),
        vec4(16, 8, 14, 6)
        ) / 17.0;

    //vec2 positionMod = vec2( uvec2( tc * lUniforms.mpPerFrame->gFrameBufferSizeVec4.xy ) & 3 );

    uvec2 positionMod = uvec2(lPos & 3);

    float rndoffset = bayer[positionMod.x][positionMod.y];
    //float rndoffset = float(positionMod.x) * 0.25;

    return rndoffset;
}

vec4 RayMarch(  in vec2 lFragCoordsVec2,
                in float lfDepth,
                in CustomPerMaterialUniforms   lPerMaterialUniforms,
                in PerFrameUniforms            lRenderTargetUniforms,
                in CommonPerMeshUniforms       lPerMeshUniforms
    )

{
    vec4 lFragCol;

#ifdef D_PLATFORM_PC_LOWEND

    // Intel Driver crashes compiling this shader.
    lFragCol = vec4(0.0);

#else

    vec3  lStartPositionVec3;
    vec3  lRayDirectionVec3;
    float lfRayLength;
   
#ifdef D_SPHERE
    {
        /*
        vec3 lSphereCenter = lRenderTargetUniforms.gLightPositionVec4.xyz;
        vec3 lRayEnd       = IN( mPositionVec4 ).xyz; // We're using front face culling

        lRayDirectionVec3       = normalize( lRayEnd - lRenderTargetUniforms.gViewPositionVec3 );

        // Chord to a sphere function
        lfRayLength          = dot( lSphereCenter - lRayEnd, -lRayDirectionVec3 ) * 2.0;
        lStartPositionVec3   = lRayEnd - ( lRayDirectionVec3 * lfRayLength );

        // Clamp to world pos
        {
        vec3 lRayEndViewSpace = lRayEnd - lRenderTargetUniforms.gViewPositionVec3;


        float lfDepth          = FastDenormaliseDepth( lRenderTargetUniforms.gClipPlanesVec4, DecodeDepthFromColour( texture2D( lPerMaterialUniforms.gBufferMap, lFragCoordsVec2 ) ) );
        vec3 lWorldPosition    = RecreatePositionFromDepth( lfDepth, lFragCoordsVec2, lRenderTargetUniforms.gViewPositionVec3, lPerMeshUniforms.gInverseProjectionMat4, lPerMeshUniforms.gInverseViewMat4 );

        // Deferred Pos is in view space
        //vec3 lWorldPosition = texture2D( lPerMaterialUniforms.gBuffer2Map, lFragCoordsVec2 ).xyz;

        if ( length( lWorldPosition ) < length( lRayEndViewSpace ) )
        {
        lfRayLength = length( ( lWorldPosition + lRenderTargetUniforms.gViewPositionVec3 ) - lStartPositionVec3 );
        }
        }
        */
    }
#else
    {
        vec3 lWorldPosition = RecreatePositionFromDepth(lfDepth, lFragCoordsVec2, vec3(0.0,0.0,0.0), lPerMeshUniforms.gInverseProjectionMat4, lPerMeshUniforms.gInverseViewMat4);
        vec3 rayVector = lWorldPosition;

        lStartPositionVec3 = lRenderTargetUniforms.gViewPositionVec3;

        lfRayLength = length(rayVector);

        // Image Stabilisation
        //rayLength      = trunc( min( rayLength, kfMaxRayMarchDistance ) * 10.0 ) * 0.1;
        //rayLength      = min( rayLength, kfMaxRayMarchDistance );

        lRayDirectionVec3 = rayVector / lfRayLength;
    }
#endif

    float lfLightAccumulation = 0.0;
    
    if (lPerMaterialUniforms.gLightShaftParamsVec4.y != 0.0)
    {
        float lfStepLength = lfRayLength / float(NB_STEPS);
        vec3 lRayStepVec3 = lRayDirectionVec3 * lfStepLength;
        vec3 lCurrentPositionVec3 = lStartPositionVec3;

        float ditherValue = Bayer(uvec2(lFragCoordsVec2 * lRenderTargetUniforms.gFrameBufferSizeVec4.xy));

        lCurrentPositionVec3 += (lRayStepVec3)* ditherValue;
        //lCurrentPositionVec3 += ( lRayStepVec3 ) * lPerMaterialUniforms.gLightShaftParamsVec4.y;

#ifndef D_SPHERE
        float lfSunScatter = ComputeScattering(dot(lRayDirectionVec3, /*normalize*/(lPerMaterialUniforms.gSunPositionVec4.xyz)), lPerMaterialUniforms.gLightShaftParamsVec4.x);
        float lfCount = 0.0;
        if (lfSunScatter > 0.003)
#else
        float lfCount = float(NB_STEPS - 2);
#endif
        {
            lCurrentPositionVec3 += lRayStepVec3;

            for (int i = 1; i < NB_STEPS - 1; i++)
            {
#ifdef D_SPHERE
                {
                    /*
                    vec3  lPosToLight       = lRenderTargetUniforms.gLightPositionVec4.xyz - lCurrentPositionVec3;
                    float lfDistanceSquared = dot( lPosToLight, lPosToLight );

                    // Quadratic Distance attenuation
                    float lfAttenuation = 1.0 / max( 1.0, lfDistanceSquared );

                    if ( lfAttenuation > 0.1 )
                    {
                    lfLightAccumulation += lfAttenuation;
                    }
                    */
                }
#else
                {
                    float shadowMapValue = ComputeShadowIntensityPCF(lPerMaterialUniforms, lRenderTargetUniforms, lPerMeshUniforms, lCurrentPositionVec3);

                    lfLightAccumulation += lfSunScatter * shadowMapValue;
                    lfCount = lfCount + 1.0;
                    //lfLightAccumulation += lfSunScatter;
                }
#endif

                //lfFog *= Map( currentPosition );

                lCurrentPositionVec3 += lRayStepVec3;
                if (lengthSquared(lRenderTargetUniforms.gViewPositionVec3 - lCurrentPositionVec3) > lRenderTargetUniforms.gShadowFadeParamVec4.w)
                {
                    // We are stepping from the view position to the reconstructed pixel's view position
                    // once we reach the cascade's bounds, it can't possible shadow any further 
                    break;
                }

            }

            lfLightAccumulation /= max(0.0001,lfCount);

            lfLightAccumulation = clamp(lfLightAccumulation, 0.0, 1.0);
            lfLightAccumulation *= lPerMaterialUniforms.gLightShaftParamsVec4.y;
            lfLightAccumulation = smoothstep(lPerMaterialUniforms.gLightShaftParamsVec4.z, lPerMaterialUniforms.gLightShaftParamsVec4.w, lfLightAccumulation);
        }
    }
    //lfLightAccumulation  = smoothstep( 0.0, lPerMaterialUniforms.gLightShaftParamsVec4.z, lfLightAccumulation );
    /*vec4 lFinalColourVec4 = vec4( step( lPerMaterialUniforms.gLightShaftParamsVec4.z, lfLightAccumulation ), 0.0, 0.0, step( lPerMaterialUniforms.gLightShaftParamsVec4.z, lfLightAccumulation ) );
    lFinalColourVec4.rgb *= lPerMeshUniforms.gLightColourVec4.rgb;
    lFragCol = lFinalColourVec4;*/

    lFragCol = float2vec4( lfLightAccumulation );

    //lFragCol = vec4( 1.0 );
    /*
    if ( ditherValue <= 0.0 )
    {
    lFragCol = vec4( 1.0, 0.0, 0.0, 1.0 );
    }
    else
    if ( ditherValue <= 0.125 )
    {
    lFragCol = vec4( 1.0, 0.0, 0.0, 1.0 );
    }
    else
    if ( ditherValue <= 0.25 )
    {
    lFragCol = vec4( 1.0, 1.0, 0.0, 1.0 );
    }
    else
    if ( ditherValue <= 0.375 )
    {
    lFragCol = vec4( 0.0, 1.0, 0.0, 1.0 );
    }
    else
    if ( ditherValue <= 0.5 )
    {
    lFragCol = vec4( 0.0, 1.0, 1.0, 1.0 );
    }
    else
    if ( ditherValue <= 0.625 )
    {
    lFragCol = vec4( 0.0, 0.0, 1.0, 1.0 );
    }
    else
    if ( ditherValue <= 0.75 )
    {
    lFragCol = vec4( 1.0, 0.0, 1.0, 1.0 );
    }
    else
    if ( ditherValue <= 0.875 )
    {
    lFragCol = vec4( 1.0, 0.0, 1.0, 1.0 );
    }
    else
    if ( ditherValue <= 1.0 )
    {
    lFragCol = vec4( 1.0, 0.0, 1.0, 1.0 );
    }
    else
    {
    lFragCol = vec4( 0.5, 0.0, 0.0, 1.0 );
    }*/

    //lFragCol = vec4(lfLightAccumulation);
    //lFragCol *= vec4( vec3( lfLightAccumulation ), 1.0 );
    //lFragCol *= vec4( lfLightAccumulation );
    //lFragCol = vec4( 1.0, 0.0, 0.0, 1.0 );
    //lFragCol = vec4( vec3(1.0), lfLightAccumulation );
    //lFragCol = vec4( lfLightAccumulation );
#endif

    return lFragCol;
}




#ifdef D_SCATTERING

#ifdef D_PLATFORM_ORBIS
//#pragma argument(targetoccupancy_atallcosts=40)
#endif

//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION
    INPUT( vec2, mTexCoordsVec2,  TEXCOORD0 )
    //INPUT( vec4, mCornerRayVec4, TEXCOORD5 )
DECLARE_END

FRAGMENT_MAIN_COLOUR01_SRT
{
    
    vec4  lFragmentColourVec4 = vec4( 0.0, 0.0, 0.0, 0.0 );
    vec2  lFragCoordsVec2     = TEX_COORDS;

    // Get fragment position
    vec3  lWorldPositionVec3           = vec3( 0.0, 0.0, 0.0 );
    vec3  lCameraRelativePositionVec3  = vec3( 0.0, 0.0, 0.0 );
    float lfDepth = FastDenormaliseDepth(lUniforms.mpPerFrame->gClipPlanesVec4, DecodeDepthFromColour(texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gBufferMap ), lFragCoordsVec2, 0.0)));

#ifndef D_PLATFORM_OPENGL
    // Godrays output to MRT1

    vec4 lRayStrength = RayMarch(
#ifdef D_SPHERE
        (IN(mScreenSpacePositionVec4).xy / IN(mScreenSpacePositionVec4).w) * 0.5 + 0.5,
#else
        TEX_COORDS,
#endif
        lfDepth,
        DEREF_PTR(lUniforms.mpCustomPerMaterial),
        DEREF_PTR(lUniforms.mpPerFrame),
        DEREF_PTR(lUniforms.mpCommonPerMesh)
        );
    lRayStrength.a = 1.0;
    FRAGMENT_COLOUR1 = lRayStrength;

#else
    FRAGMENT_COLOUR1 = vec4( 0.0, 0.0, 0.0, 0.0 );
#endif

    {
        // Read depth from g-buffer

        float lfModifiedDepth = lfDepth;
        if( lfDepth >= lUniforms.mpPerFrame->gClipPlanesVec4.y - 100.0 )
        {
            lfModifiedDepth = 50000.0;
        }

        // Recreate position
        lWorldPositionVec3          = RecreatePositionFromDepth( lfModifiedDepth, lFragCoordsVec2, lUniforms.mpPerFrame->gViewPositionVec3, lUniforms.mpCommonPerMesh->gInverseProjectionMat4, lUniforms.mpCommonPerMesh->gInverseViewMat4 );
        lCameraRelativePositionVec3 = lWorldPositionVec3 - lUniforms.mpPerFrame->gViewPositionVec3;
    }

    vec3  lPrimaryPlanetPositionVec3       = lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz;
    float lfPrimaryPlanetRadius            = lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w;
    vec3  lPrimaryRelativeViewPositionVec3 = ( lUniforms.mpPerFrame->gViewPositionVec3 - lPrimaryPlanetPositionVec3 );
    vec3  lPrimaryRelativePositionVec3     = ( lWorldPositionVec3 - lPrimaryPlanetPositionVec3 );
    vec3  lWaterHitPositionVec3            = vec3( 0.0, 0.0, 0.0 );
    float lfUseWaterHitPos                 = 0.0;

    // Create water plane, as water is not rendered to the g-buffer
    float lfWaterHeight = lUniforms.mpCustomPerMaterial->gWaterFogVec4.r;
    if( lfWaterHeight > 0.0 )
    {
        // Calculate positions relative to planet
        vec3  lDummyPositionVec3;
        float lfRayTest         = GetClampedRayIntersectionPoint( lPrimaryRelativeViewPositionVec3, lPrimaryRelativePositionVec3, ( lfPrimaryPlanetRadius + lfWaterHeight ), lWaterHitPositionVec3, lDummyPositionVec3 );

        // If we hit the sphere and the water plane is closer than the original point
        if( lfRayTest > 0.0 &&
            length( lPrimaryRelativeViewPositionVec3 - lPrimaryRelativePositionVec3 ) > length( lPrimaryRelativeViewPositionVec3 - lWaterHitPositionVec3 ) &&
            length( lPrimaryRelativeViewPositionVec3 - lWaterHitPositionVec3 ) > 0.0 )
        {
            lWorldPositionVec3          = lWaterHitPositionVec3 + lPrimaryPlanetPositionVec3;
            lCameraRelativePositionVec3 = lWaterHitPositionVec3 + lPrimaryPlanetPositionVec3 -  lUniforms.mpPerFrame->gViewPositionVec3;
            lfUseWaterHitPos            = 1.0;
        }
        else if( lfDepth >= lUniforms.mpPerFrame->gClipPlanesVec4.y - 100.0 )
        {
            #ifndef D_COMPUTE
            FRAGMENT_COLOUR0 = vec4( 0.0, 0.0, 0.0, 0.0 );
            #endif
            return;
        }
    }
    else if( lfDepth >= lUniforms.mpPerFrame->gClipPlanesVec4.y - 100.0 )
    {
        #ifndef D_COMPUTE
        FRAGMENT_COLOUR0 = vec4( 0.0, 0.0, 0.0, 0.0 );
        #endif
        return;
    }


    // Find nearest planet
    vec3  lNearestPlanetPosition     = lPrimaryPlanetPositionVec3;
    vec3  lPlanetHorizonColour       = lUniforms.mpCustomPerMaterial->gHorizonColourVec4.xyz;
    float lfNearestPlanetRadius      = lfPrimaryPlanetRadius;
    {
        float lfDistance                 = length( lNearestPlanetPosition - lWorldPositionVec3 );

        for( int i = 0; i < 6; i++ )
        {
            float lfNewDistance  = length( ( lUniforms.mpCustomPerMaterial->gaPlanetPositionsVec4[ i ].xyz - lWorldPositionVec3 ) );
            lfNewDistance       -= lUniforms.mpCustomPerMaterial->gaPlanetPositionsVec4[ i ].w;
            if( lfNewDistance <= lfDistance && lUniforms.mpCustomPerMaterial->gaPlanetPositionsVec4[ i ].w > 0.0 )
            {
                lPlanetHorizonColour   = lUniforms.mpCustomPerMaterial->gaPlanetColoursVec4[ i ].xyz;
                lNearestPlanetPosition = lUniforms.mpCustomPerMaterial->gaPlanetPositionsVec4[ i ].xyz;
                lfNearestPlanetRadius  = lUniforms.mpCustomPerMaterial->gaPlanetPositionsVec4[ i ].w;
                lfDistance             = lfNewDistance;
            }
        }
    }

    // Calculate various height fade values to interpolate between fog settings
    float lfIsInSpace            = 0.0;
    float lfColourFade           = 0.0;
    float lfHorizonPlanetFade    = 0.0;
    float lfParamFade            = 0.0;
    float lfFogStrengthFade      = 0.0;
    float lfThisIsPrimaryPlanet  = 0.0;
    float lfParamFadeMix         = 0.0;
    float lfHorizonBlend         = 0.0;
    float lfFarBlend             = 0.0;
    float lfFarBlendPrimary      = 0.0;
    {
        float lfBlendStartHeight         = lUniforms.mpCustomPerMaterial->gFogFadeHeightsVec4.r;
        float lfBlendDistance            = lUniforms.mpCustomPerMaterial->gFogFadeHeightsVec4.g;
        float lfFogStrengthStartHeight   = lUniforms.mpCustomPerMaterial->gFogFadeHeightsVec4.b;
        float lfFogStrengthDistance      = lUniforms.mpCustomPerMaterial->gFogFadeHeightsVec4.a;
        float lfPositionBlendStartHeight = lUniforms.mpCustomPerMaterial->gFogFadeHeights2Vec4.r;
        float lfPositionBlendDistance    = lUniforms.mpCustomPerMaterial->gFogFadeHeights2Vec4.g;
        float lfColourBlendStartHeight   = lUniforms.mpCustomPerMaterial->gFogFadeHeights2Vec4.b;
        float lfColourBlendDistance      = lUniforms.mpCustomPerMaterial->gFogFadeHeights2Vec4.a;
        float lfHorizonBlendStartHeight  = lUniforms.mpCustomPerMaterial->gFogFadeHeights3Vec4.r;
        float lfHorizonBlendDistance     = lUniforms.mpCustomPerMaterial->gFogFadeHeights3Vec4.g;
        float lfFarBlendStartHeight      = lUniforms.mpCustomPerMaterial->gFogFadeHeights3Vec4.b;
        float lfFarBlendDistance         = lUniforms.mpCustomPerMaterial->gFogFadeHeights3Vec4.a;
        float lfFogStrengthPower         = lUniforms.mpCustomPerMaterial->gSpaceFogParamsVec4.a;

        float lfDistanceFromPlanet  = length( lUniforms.mpPerFrame->gViewPositionVec3 - lNearestPlanetPosition ) - lfNearestPlanetRadius;
        float lfDistanceFromPrimary = length( lUniforms.mpPerFrame->gViewPositionVec3 - lPrimaryPlanetPositionVec3 ) - lfPrimaryPlanetRadius;

        float lfIsOffPrimaryPlanet  = clamp( ( lfDistanceFromPrimary - lfColourBlendStartHeight ) / lfColourBlendDistance, 0.0, 1.0 );
        lfIsInSpace                 = clamp( ( lfDistanceFromPlanet - lfColourBlendStartHeight ) / lfColourBlendDistance, 0.0, 1.0 );
        lfColourFade                = mix( lfIsOffPrimaryPlanet, 0.0, ( 1.0 - lfIsOffPrimaryPlanet ) * lfIsInSpace );

        lfIsOffPrimaryPlanet        = clamp( ( lfDistanceFromPrimary - lfBlendStartHeight ) / lfBlendDistance, 0.0, 1.0 );
        lfIsInSpace                 = clamp( ( lfDistanceFromPlanet - lfBlendStartHeight ) / lfBlendDistance, 0.0, 1.0 );
        lfParamFade                 = mix( lfIsOffPrimaryPlanet, 0.0, ( 1.0 - lfIsOffPrimaryPlanet ) * lfIsInSpace );

        lfIsOffPrimaryPlanet        = clamp( ( lfDistanceFromPrimary - lfFogStrengthStartHeight ) / lfFogStrengthDistance, 0.0, 1.0 );
        lfIsInSpace                 = clamp( ( lfDistanceFromPlanet -  lfFogStrengthStartHeight ) / lfFogStrengthDistance, 0.0, 1.0 );
        lfFogStrengthFade           = mix( lfIsOffPrimaryPlanet, 0.0, ( 1.0 - lfIsOffPrimaryPlanet ) * lfIsInSpace );
        lfFogStrengthFade           = 1.0 - clamp( pow( 1.0 - lfFogStrengthFade, lfFogStrengthPower ), 0.0, 1.0 );

        lfThisIsPrimaryPlanet       = ( abs( lfDistanceFromPlanet - lfDistanceFromPrimary ) < 100.0 ) ? 1.0 : 0.0;
        lfIsInSpace                 = clamp( ( lfDistanceFromPrimary - lfPositionBlendStartHeight ) / lfPositionBlendDistance, 0.0, 1.0 );
        
        lfParamFadeMix              = max( lfParamFade, lfThisIsPrimaryPlanet );

        lfHorizonBlend              = clamp( ( lfDistanceFromPlanet - lfHorizonBlendStartHeight ) / lfHorizonBlendDistance, 0.0, 1.0 );
        lfFarBlend                  = clamp( ( lfDistanceFromPlanet - lfFarBlendStartHeight ) / lfFarBlendDistance, 0.0, 1.0 );
    }

    //lPlanetHorizonColour = mix( lUniforms.mpCustomPerMaterial->gHorizonColourVec4.xyz, lPlanetHorizonColour, lfIsInSpace );


    // Shared values
    float lfAtmosphereSize                 = 1.080000;
    float lfSunStrength                    = lUniforms.mpCustomPerMaterial->gHorizonColourVec4.a;
    vec3  lLightDirectionVec3              =  lUniforms.mpCustomPerMaterial->gSunPositionVec4.xyz;///*normalize*/( -lUniforms.mpPerFrame->gLightDirectionVec4.xyz );

    vec3  lPlanetRelativeViewPositionVec3  = ( lUniforms.mpPerFrame->gViewPositionVec3 - lNearestPlanetPosition );
    vec3  lPlanetRelativePositionVec3      = ( lWorldPositionVec3 - lNearestPlanetPosition );
    if( lfUseWaterHitPos > 0.0 )
    {
        lPlanetRelativePositionVec3        = lWaterHitPositionVec3;
    }

    // Atmosphere scattering
    float lfHorizon = 1.0;
    float lfAtmosHorizon = 0.0;
    float lfSpaceHorizon = 0.0;
    vec3 lAtmosphereScattering = vec3( 1.0, 0.0, 0.0 );
    vec3 lSolarScatteringVec3  = vec3( 1.0, 0.0, 0.0 );
    if ( ( lfColourFade < 1.0) || (lfParamFadeMix < 1.0) )
    {
        vec3  lHorizonColour        = /*GammaCorrectInput*/ ( lUniforms.mpCustomPerMaterial->gHorizonColourVec4.rgb ) * lUniforms.mpCustomPerMaterial->gSunPositionVec4.a;

        lPrimaryRelativeViewPositionVec3 = ( lUniforms.mpPerFrame->gViewPositionVec3 - lPrimaryPlanetPositionVec3 ) / lfPrimaryPlanetRadius;
        vec3  lDirectionToAtmosphereVec3       = normalize( lCameraRelativePositionVec3 );
        vec3  lUpDirectionVec3                 = normalize( lUniforms.mpPerFrame->gViewPositionVec3 - lPrimaryPlanetPositionVec3 );
        
        float lfAngleTest                      = dot( lDirectionToAtmosphereVec3, lUpDirectionVec3 );
        lfAngleTest                            = smoothstep( 0.0, 0.2, -lfAngleTest );

        //if( lfAngleTest < 1.0 )
        {
            vec3  lAtmospherePositionVec3  = normalize( GetFarRayIntersectionPoint( ( lUniforms.mpPerFrame->gViewPositionVec3 - lPrimaryPlanetPositionVec3 ), lDirectionToAtmosphereVec3, lfAtmosphereSize * lfPrimaryPlanetRadius ) ) * lfAtmosphereSize;

            vec3  lSkyColour            = /*GammaCorrectInput*/ ( lUniforms.mpCustomPerMaterial->gSkyColourVec4.rgb ) * lUniforms.mpCustomPerMaterial->gSkyColourVec4.a;
            vec3  lSunColour            = /*GammaCorrectInput*/ ( lUniforms.mpCustomPerMaterial->gSunColourVec4.rgb );

            float lfAtmosphereThickness = lUniforms.mpCustomPerMaterial->gScatteringParamsVec4.g;
            float lfHorizonMultiplier   = lUniforms.mpCustomPerMaterial->gScatteringParamsVec4.b;
            float lfHorizonFadeSpeed    = lUniforms.mpCustomPerMaterial->gScatteringParamsVec4.a;
            float lfBlendTightness      = min( lUniforms.mpCustomPerMaterial->gScatteringParamsVec4.r, ( 0.8 - 0.1 ) * 0.5 );
                
            vec3  lGradientSpeed             = /*GammaCorrectInput*/( lUniforms.mpCustomPerMaterial->gSkyGradientSpeedVec4.rgb );
            vec3  lSkyUpperColour            = /*GammaCorrectInput*/( lUniforms.mpCustomPerMaterial->gSkyUpperColourVec4.rgb ) * lUniforms.mpCustomPerMaterial->gSkyColourVec4.a;
            vec3  lSkySolarColour            = /*GammaCorrectInput*/( lUniforms.mpCustomPerMaterial->gSkySolarColourVec4.rgb ) * lUniforms.mpCustomPerMaterial->gSkyColourVec4.a;
            float lfSolarFadeSize            = lUniforms.mpCustomPerMaterial->gSkyUpperColourVec4.a;
            float lfSolarFadeStrength        = lUniforms.mpCustomPerMaterial->gSkySolarColourVec4.a;
            float lfUpperSkyFadeSpeed        = lUniforms.mpCustomPerMaterial->gSkyGradientSpeedVec4.a;
            float lfUpperSkyFadeOffset       = lUniforms.mpCustomPerMaterial->gSkyUpperParamsVec4.r;

            // Calculate scattering
            vec3 lInScatteringVec3 = InScattering(
                lAtmospherePositionVec3,
                lLightDirectionVec3,
                lPrimaryRelativeViewPositionVec3,
                lfAtmosphereSize,
                lfHorizonFadeSpeed,
                lfAtmosphereThickness,
#ifdef D_PLATFORM_PC_LOWEND
                true,
#else
                false,
#endif
                false );

            vec3 lAtmosphereInScattering = InScatteringPhase(
                lInScatteringVec3,
                GammaCorrectInput( vec3( 1.0, 1.0, 1.0 ) ),
                lfSunStrength,
                lfHorizonFadeSpeed,
                RayleighPhase( lLightDirectionVec3, lAtmospherePositionVec3 - lPrimaryRelativeViewPositionVec3 ) );


            // Sky colouring
            lSolarScatteringVec3 = InScatteringPhase(
                lInScatteringVec3,
                lSunColour,
                lfSolarFadeStrength,
                1.0,
                MiePhase( lLightDirectionVec3, lAtmospherePositionVec3 - lPrimaryRelativeViewPositionVec3, lfSolarFadeSize ) );

            // Shift scattering to cover full range
            vec3 lAdjustedScatteringVec3;
            lAdjustedScatteringVec3.x    = smoothstep( min( 0.1 + lfBlendTightness * lGradientSpeed.x, 0.45 ), max( 0.8 - lfBlendTightness * lGradientSpeed.x, 0.451 ), lAtmosphereInScattering.x * lfHorizonMultiplier );
            lAdjustedScatteringVec3.y    = smoothstep( min( 0.1 + lfBlendTightness * lGradientSpeed.y, 0.45 ), max( 0.8 - lfBlendTightness * lGradientSpeed.y, 0.451 ), lAtmosphereInScattering.x * lfHorizonMultiplier );
            lAdjustedScatteringVec3.z    = smoothstep( min( 0.1 + lfBlendTightness * lGradientSpeed.z, 0.45 ), max( 0.8 - lfBlendTightness * lGradientSpeed.z, 0.451 ), lAtmosphereInScattering.x * lfHorizonMultiplier );
            vec3 lScatteringColourVec3   = lSkyColour;//mix( lSkyColour, lSkyColour /* max( lAtmosphereInScattering.x, 1.0 )*/, lfIsInSpace );

            lScatteringColourVec3        = mix( lSkyUpperColour,       lSkyColour,      saturate( max( lAtmosphereInScattering.x - lfUpperSkyFadeOffset, 0.0 ) * lfUpperSkyFadeSpeed * lGradientSpeed ) );
            lScatteringColourVec3        = mix( lScatteringColourVec3, lSkySolarColour, saturate( lSolarScatteringVec3 * lGradientSpeed ) );
            lAtmosphereScattering        = mix( lScatteringColourVec3, lHorizonColour,  saturate( lAdjustedScatteringVec3 * lGradientSpeed ) );

            lfAtmosHorizon = saturate( length( lAdjustedScatteringVec3 * lGradientSpeed.x + lAdjustedScatteringVec3.y * lGradientSpeed.y + lAdjustedScatteringVec3.z * lGradientSpeed.z ) / 3.0 );

            // Fade on distant planets at the horizon
            lfHorizonPlanetFade = 1.0 - smoothstep( 0.1, 0.8, lAtmosphereInScattering.x * 2.2 );
        }

        lAtmosphereScattering = mix( lAtmosphereScattering, lHorizonColour, lfAngleTest );
    }



    // Space scattering
    vec3 lPositionScattering = vec3(0.0, 0.0, 0.0);

    if ( ( lfColourFade > 0.0) || (lfParamFadeMix < 1.0) )
    {        
        vec3 lTerrainPositionVec3        = normalize( lPlanetRelativePositionVec3 );
        vec3 lViewPositionVec3           = lTerrainPositionVec3 - normalize( lCameraRelativePositionVec3 ) * ( lfAtmosphereSize - 1.0 );

        vec3  lSpaceSkyColour            = /*GammaCorrectInput*/( lUniforms.mpCustomPerMaterial->gSpaceSkyColourVec4.rgb );
        vec3  lSpaceHorizonColour        = lPlanetHorizonColour;
        float lfSpaceAtmosphereThickness = lUniforms.mpCustomPerMaterial->gSpaceScatteringParamsVec4.g;
        float lfSpaceHorizonMultiplier   = lUniforms.mpCustomPerMaterial->gSpaceScatteringParamsVec4.b;
        float lfSpaceHorizonFadeSpeed    = lUniforms.mpCustomPerMaterial->gSpaceScatteringParamsVec4.a;
        float lfSpaceSunStrength         = lUniforms.mpCustomPerMaterial->gSpaceSkyColourVec4.a;

        // Calculate scattering
        vec3 lInScatteringVec3 = InScattering(
            lTerrainPositionVec3,
            lLightDirectionVec3,
            lViewPositionVec3, 
            lfAtmosphereSize,
            lfSpaceHorizonFadeSpeed,
            lfSpaceAtmosphereThickness,
#ifdef D_PLATFORM_PC_LOWEND
            true,
#else
            false,
#endif
            true );

        lPositionScattering = InScatteringPhase(
            lInScatteringVec3,
            GammaCorrectInput( vec3( 1.0, 1.0, 1.0 ) ),
            lfSpaceSunStrength,
            lfSpaceHorizonFadeSpeed,
            RayleighPhase( lLightDirectionVec3, lTerrainPositionVec3 - lViewPositionVec3 ) );

        vec3 lAdjustedScatteringVec3 = float2vec3( smoothstep( 0.1, 0.8, lPositionScattering.x * lfSpaceHorizonMultiplier ) );
        //lPositionScattering = lSpaceHorizonColour;///* mix( 1.0, lPositionScattering.x, lfColourFade ) */ mix( lSpaceSkyColour, lSpaceHorizonColour, saturate( lAdjustedScatteringVec3 ) );
        lPositionScattering = mix( 1.0, lAdjustedScatteringVec3.x, lfFarBlend ) * lSpaceHorizonColour;//mix( lSpaceSkyColour, lSpaceHorizonColour, saturate( lAdjustedScatteringVec3 ) );

        // Fade on the dark side of distant planets
        //lfHorizonPlanetFade = min( lfHorizonPlanetFade, max( smoothstep( 0.1, 0.8, lPositionScattering.x ), 0.25 ) );
        lfHorizonPlanetFade = min( lfHorizonPlanetFade, lPositionScattering.x * 0.2 + 0.01 );

        lfSpaceHorizon = saturate( saturate( lAdjustedScatteringVec3 ).x );
    }

    // Blending as you leave atmosphere
    lfParamFade  = mix( lfHorizonPlanetFade, lfParamFade, lfParamFadeMix );
    lfColourFade = mix( lfHorizonPlanetFade, lfColourFade, lfParamFadeMix );
    lFragmentColourVec4.rgb = mix(saturate(lAtmosphereScattering), saturate(lPositionScattering), lfColourFade);

    // Fog values        
    vec3  lLightColour           = /*GammaCorrectInput*/( lUniforms.mpCommonPerMesh->gLightColourVec4.rgb );
    vec3  lFogColour             = /*GammaCorrectInput*/( lUniforms.mpCustomPerMaterial->gFogColourVec4.rgb );
    vec3  lHeightFogColour       = /*GammaCorrectInput*/( lUniforms.mpCustomPerMaterial->gHeightFogColourVec4.rgb ) * lLightColour;
    float lfFogStrength          = lUniforms.mpCustomPerMaterial->gFogParamsVec4.r;
    float lfFogMax               = mix( lUniforms.mpCustomPerMaterial->gFogParamsVec4.g, 1.0, lfColourFade );
    float lfFogColourStrength    = lUniforms.mpCustomPerMaterial->gFogParamsVec4.b;
    float lfFogColourMax         = lUniforms.mpCustomPerMaterial->gFogParamsVec4.a;
    float lfSpaceFogStrength     = mix( lUniforms.mpCustomPerMaterial->gSpaceScatteringParamsVec4.r, 4.0, lfFarBlend ) / lfNearestPlanetRadius;
    
    // Distance
#ifndef D_PLATFORM_OPENGL
    [isolate]
#endif
    {
        vec4 lAtmosphereFogResultVec4  = vec4( 0.0, 0.0, 0.0, 0.0 );
        vec3  lNearPosition  = lUniforms.mpPerFrame->gViewPositionVec3 - lPrimaryPlanetPositionVec3;
        vec3  lFarPosition   = lWorldPositionVec3 - lPrimaryPlanetPositionVec3;
        float lfRayTest      = GetClampedRayIntersectionPoint( lNearPosition, lFarPosition, lfAtmosphereSize * lfPrimaryPlanetRadius, lNearPosition, lFarPosition );
        if( lfRayTest > 0.0 )
        {
            float lfFogDistance          = length( lFarPosition - lNearPosition ); 

            float lfFogValue             = lfFogDistance * mix( lfFogStrength,  lfSpaceFogStrength, lfFogStrengthFade );
            lfFogValue                   = 1.0 / exp( lfFogValue * lfFogValue );
            lfFogValue                   = clamp( 1.0 - lfFogValue, 0.0, lfFogMax );

            float lfFogColour            = lfFogDistance * lfFogColourStrength;
            lfFogColour                  = 1.0 / exp( lfFogColour * lfFogColour );
            lfFogColour                  = clamp( 1.0 - lfFogColour, 0.0, lfFogColourMax );

            lAtmosphereFogResultVec4.rgb = mix( lFogColour, lFragmentColourVec4.rgb, lfFogColour );
            lAtmosphereFogResultVec4.a   = max( lFragmentColourVec4.a, lfFogValue /* lfHorizon*/ );
            //lAtmosphereFogResultVec4.rgb = vec3( 0.0, 0.0, 1.0 );
        }
        
        vec4 lSpaceFogResultVec4  = vec4( 0.0, 0.0, 0.0, 0.0 );
        if (lfParamFade > 0.0)
        {
            lfRayTest         = GetClampedRayIntersectionPoint( lPlanetRelativeViewPositionVec3, lPlanetRelativePositionVec3, lfNearestPlanetRadius * lfAtmosphereSize, lNearPosition, lFarPosition ); 
            if( lfRayTest > 0.0 )
            {
                if( length( lWorldPositionVec3 - lNearestPlanetPosition ) <= lfNearestPlanetRadius * lfAtmosphereSize )
                {
                    float lfFogDistance     = length( lNearPosition - lFarPosition );
 
                    float lfFogValue        = lfFogDistance * lfSpaceFogStrength;
                    lfFogValue              = 1.0 / exp( 0.0 + lfFogValue * lfFogValue );
                    lfFogValue              = 1.0 - clamp( lfFogValue, 0.0, 1.0 );
 
                    lSpaceFogResultVec4.rgb = lFragmentColourVec4.rgb;
                    //lSpaceFogResultVec4.rgb = vec3( 1.0, 0.0, 0.0 );
                    lSpaceFogResultVec4.a   = max( lFragmentColourVec4.a, lfFogValue /* lfHorizon*/ );
                }
            } 
        }            
        lFragmentColourVec4 = mix( lAtmosphereFogResultVec4, lSpaceFogResultVec4, lfParamFade );
    }

    // Height Fog
    float lfHeightFogStrength    = lUniforms.mpCustomPerMaterial->gHeightFogParamsVec4.r;
    float lfHeightFogOffset      = lUniforms.mpCustomPerMaterial->gHeightFogParamsVec4.g;
    float lfHeightFogHeight      = lUniforms.mpCustomPerMaterial->gHeightFogParamsVec4.b;
    float lfHeightFogMax         = lUniforms.mpCustomPerMaterial->gHeightFogParamsVec4.a;
    float lfHeightFogOutStrength = lUniforms.mpCustomPerMaterial->gHeightFogColourVec4.a;
    {
        vec3  lNearPosition = lUniforms.mpPerFrame->gViewPositionVec3 - lNearestPlanetPosition;
        vec3  lFarPosition  = lWorldPositionVec3 - lNearestPlanetPosition;

        float lfFogDistance = length( lFarPosition - lNearPosition );

        float lfFogValue        = lfFogDistance * lfHeightFogStrength;
        lfFogValue              = 1.0 / exp( lfHeightFogOffset + lfFogValue * lfFogValue );
        lfFogValue              = 1.0 - clamp( lfFogValue, 0.0, 1.0 );

        float lfHeightFogValue  = ( length( lFarPosition ) - ( lfNearestPlanetRadius + lfWaterHeight ) ) / ( lfHeightFogHeight + 0.0001 );
        //lfHeightFogValue        = 1.0 / exp( lfHeightFogValue * lfHeightFogValue );
        lfHeightFogValue        = 1.0 - clamp( lfHeightFogValue, 0.0, 1.0 );
        lfHeightFogValue       *= lfHeightFogMax;

        float lfHeightFogOut    = lfFogDistance * lfHeightFogOutStrength;
        lfHeightFogOut          = 1.0 / exp( lfHeightFogOffset + lfHeightFogOut * lfHeightFogOut );
        lfHeightFogOut          = 1.0 - clamp( lfHeightFogOut, 0.0, 1.0 );

        lfFogValue              = clamp( lfHeightFogValue * lfFogValue, 0.0, ( 1.0 - lfFogStrengthFade ) * lfThisIsPrimaryPlanet );

        lFragmentColourVec4.rgb = mix( lFragmentColourVec4.rgb, lHeightFogColour, clamp( lfFogValue - lfHeightFogOut, 0.0, 1.0 ) );
        lFragmentColourVec4.a   = max( lFragmentColourVec4.a, lfFogValue );

    }

    //////
    //Water Fog
    {
        vec3  lNearPosition = lUniforms.mpPerFrame->gViewPositionVec3 - lNearestPlanetPosition;
        if( length( lNearPosition ) <  (lfNearestPlanetRadius + lfWaterHeight) )
        {
            float lfWaterFade = length( lUniforms.mpPerFrame->gViewPositionVec3 - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz ) - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w;
            lfWaterFade       = 1.0 - clamp( ( lfWaterFade - 2000.0 ) / 1000.0, 0.0, 1.0 );
            lFragmentColourVec4.a = 1.0 - ( lfThisIsPrimaryPlanet * lfWaterFade );
        }
    }

    // Space fog
    if( lfIsInSpace > 0 )
    {
        float lfSpaceFogStrength2       = lUniforms.mpCustomPerMaterial->gSpaceFogParamsVec4.r;
        float lfSpaceFogClamp           = lUniforms.mpCustomPerMaterial->gSpaceFogParamsVec4.g;
        //float lfSpaceSunSize            = lUniforms.mpCustomPerMaterial->gSpaceSkyColour3Vec4.a;

        if( length( lPlanetRelativePositionVec3 ) < lfNearestPlanetRadius * lfAtmosphereSize )
        {
            lfSpaceFogClamp   = lUniforms.mpCustomPerMaterial->gSpaceFogParamsVec4.b;
            lfFogStrengthFade = saturate( 1.0 - length( lFragmentColourVec4.rgb ) );
        }

        float lfDistance    = length( lCameraRelativePositionVec3 );

        float lfFogValue  = lfDistance *  mix( lfFogStrength, lfSpaceFogStrength2, lfFogStrengthFade );
        lfFogValue        = 1.0 / exp(  lfFogValue * lfFogValue );
        lfFogValue        = clamp( 1.0 - lfFogValue, 0.0, 1.0 );
        lfFogValue       *= lfParamFade * lfFarBlend;
        lfFogValue        = clamp( lfFogValue, 0.0, lfSpaceFogClamp );

        if (lfFogValue > 0.0)
        {
            vec3  lSpaceFogColourVec3       = lUniforms.mpCustomPerMaterial->gSpaceFogColourVec4.rgb;
            vec3  lSpaceFogColour2Vec3      = lUniforms.mpCustomPerMaterial->gSpaceFogColour2Vec4.rgb;
            vec3  lSpaceWavelength1Vec3     = lUniforms.mpCustomPerMaterial->gSpaceSkyColour1Vec4.rgb;
            vec3  lSpaceWavelength2Vec3     = lUniforms.mpCustomPerMaterial->gSpaceSkyColour2Vec4.rgb;
            vec3  lSpaceWavelength3Vec3     = lUniforms.mpCustomPerMaterial->gSpaceSkyColour3Vec4.rgb;
            float lfSpaceFogColourDistance  = lUniforms.mpCustomPerMaterial->gSpaceFogColourVec4.a;
            float lfSpaceFogColour2Distance = lUniforms.mpCustomPerMaterial->gSpaceFogColour2Vec4.a;
            float lfSpaceCenterPowerVec3    = lUniforms.mpCustomPerMaterial->gSpaceSkyColour2Vec4.a;

            vec3 lSpaceFogColour = saturate(GetSpaceColour(
                normalize(lCameraRelativePositionVec3),
                vec3(0.0, 0.0, 0.0),
                lLightDirectionVec3,
                lSpaceWavelength1Vec3,
                lSpaceWavelength2Vec3,
                lSpaceWavelength3Vec3,
                lfSpaceCenterPowerVec3));

            float lfFogColour            = lfDistance * lfSpaceFogColourDistance;
            lfFogColour                  = 1.0 / exp( lfFogColour * lfFogColour );
            lfFogColour                  = 1.0 - clamp( lfFogColour, 0.0, 1.0 );
            lSpaceFogColour              = mix( lSpaceFogColourVec3, lSpaceFogColour, lfFogColour );

            lfFogColour                  = lfDistance * lfSpaceFogColour2Distance;
            lfFogColour                  = 1.0 / exp( lfFogColour * lfFogColour );
            lfFogColour                  = 1.0 - clamp( lfFogColour, 0.0, 1.0 );
            lSpaceFogColour              = mix( lSpaceFogColour2Vec3, lSpaceFogColour, lfFogColour );

            //lFragmentColourVec4.rgb      = mix( lFragmentColourVec4.rgb, lSpaceFogColour, saturate( lfFogValue - lFragmentColourVec4.a ) * lfParamFade );
            lFragmentColourVec4.rgb      = mix( lFragmentColourVec4.rgb, lSpaceFogColour, saturate( lfFogValue) );
        }

        lFragmentColourVec4.a        = max( lFragmentColourVec4.a,   lfFogValue );
    }

    #ifdef D_COMPUTE
    FRAGMENT_COLOUR0 += lFragmentColourVec4;
    #else
    if( lFragmentColourVec4.a == 0.0 )
    {
        discard;
    }
    lFragmentColourVec4.rgb /= lFragmentColourVec4.a;
    FRAGMENT_COLOUR0 = lFragmentColourVec4;
    #endif

}


#endif

// =================================================================================================
//
// RAYMARCH
//
// =================================================================================================

#ifdef D_RAYMARCH

//-----------------------------------------------------------------------------
//      Global Data

//SAMPLER2DSHADOW( gShadowMap, 15 );


//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT


    #ifdef D_SPHERE
        INPUT_SCREEN_POSITION
        INPUT( vec4, mPositionVec4, TEXCOORD0 )
        INPUT( vec4, mScreenSpacePositionVec4, TEXCOORD4 )
    #else
        INPUT_SCREEN_POSITION
        INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
    #endif

DECLARE_END


FRAGMENT_MAIN_COLOUR_SRT
{
#ifdef D_COMPUTE
    return;
#elif !defined D_PLATFORM_OPENGL
    discard;
#else

#ifdef D_SPHERE
    vec2 lFragCoordsVec2 = (IN(mScreenSpacePositionVec4).xy / IN(mScreenSpacePositionVec4).w) * 0.5 + 0.5;
#else
    vec2 lFragCoordsVec2 = TEX_COORDS;
#endif
    float lfDepth = FastDenormaliseDepth(lUniforms.mpPerFrame->gClipPlanesVec4, DecodeDepthFromColour(texture2DLod(SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gBufferMap ), lFragCoordsVec2, 0.0)));

    FRAGMENT_COLOUR = RayMarch(
                                lFragCoordsVec2,
                                lfDepth,
                                DEREF_PTR(lUniforms.mpCustomPerMaterial),
                                DEREF_PTR(lUniforms.mpPerFrame),
                                DEREF_PTR(lUniforms.mpCommonPerMesh)
                              );   
#endif
}

#endif
