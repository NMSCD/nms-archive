////////////////////////////////////////////////////////////////////////////////
///
///     @file       WaterFragment.h
///     @author     User
///     @date       
///
///     @brief      WaterFragmentShader
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 

// define this before including commonuniforms so we have the correct samplers declared.
#ifndef D_FULLSCREEN
#define D_FADE
#endif

//-----------------------------------------------------------------------------
//      Include files

#ifndef D_FRAGMENT
#define D_FRAGMENT
#endif
#include "Common/Defines.shader.h"
#include "Common/Common.shader.h"
#include "Common/CommonUniforms.shader.h"
#include "Common/CommonFragment.shader.h"
#include "Common/CommonDepth.shader.h"

//#ifndef D_SCATTERING
//#pragma argument(targetoccupancy_atallcosts=50)
//#endif

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Typedefs and Classes 

#include "Custom/WaterCommon.h"
#include "Common/CommonLighting.shader.h"
#include "Common/CommonFade.shader.h"
#include "Common/CommonPlanet.shader.h"
#include "Common/CommonScattering.shader.h"
#include "Common/CommonFog.shader.h"
#include "Common/CommonDepth.shader.h"
#include "Common/CommonTriplanarTexturing.shader.h"

vec3
ComputeWaterLighting(
    in vec3                   lPositionVec3,
    in vec3                   lNormalVec3,
    in vec3                   lViewPositionVec3,
    in vec3                   lLightDirectionVec3,
    in vec3                   lBaseColourVec3,
    in vec3                   lReflectionColourVec3,
    in vec4                   lLightColourVec4,
    in vec4                   lPlanetPositionVec4,
    in float                  lfMetallic, // 1.0
    in float                  lfNonMetalSpecularScale, //0.5
    in float                  lfRoughness, // 0.0
    in float                  lfShadow )
{
    vec3   lFinalColour = vec3( 0.0, 0.0, 0.0 );
    vec3   lDiffuseColourVec3;
    vec3   lSpecularColourVec3;
    vec3   lViewDirVec3;
    float  lfAttenuation = 1.0;
    float  lfNoV;
    float  lfNoL;

    lViewDirVec3 = normalize( lViewPositionVec3 - lPositionVec3.xyz );

    lfNoV = max( 0.0, dot( lNormalVec3, lViewDirVec3 ) );
    lfNoL = max( 0.0, dot( lNormalVec3, lLightDirectionVec3 ) );

    {
        float DielectricSpecular = 0.08 * lfNonMetalSpecularScale;

        lDiffuseColourVec3 = lBaseColourVec3 - ( lBaseColourVec3 * lfMetallic );	// 1 mad
        lSpecularColourVec3 = ( DielectricSpecular - ( DielectricSpecular * lfMetallic ) ) + ( lBaseColourVec3 * lfMetallic );	// 2 mad
    }

    {
        lSpecularColourVec3 = EnvBRDFApprox( lSpecularColourVec3, lfRoughness, lfNoV );
    }

    {
        vec3   lLightColourVec3 = /*GammaCorrectInput*/ ( lLightColourVec4.xyz ) * lLightColourVec4.w;
        vec3   lReflectionVec3 = reflect( -lViewDirVec3, lNormalVec3 );

        float  lfRoL = max( 0.0, dot( lReflectionVec3, lLightDirectionVec3 ) );
        lFinalColour += ( ( lfShadow * lfNoL ) ) * lLightColourVec3 * ( lDiffuseColourVec3 + lSpecularColourVec3 * PhongApprox( lfRoughness, lfRoL ) );

        {
            vec3 SpecularIBL = lReflectionColourVec3;
            lFinalColour += lfShadow * SpecularIBL * lSpecularColourVec3;
        }
    }

    return lFinalColour;
}

//-----------------------------------------------------------------------------
///
///     Input
///
///     @brief  Input
///
//-----------------------------------------------------------------------------

DECLARE_INPUT
INPUT_SCREEN_POSITION
INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END


//FRAGMENT_MAIN_COLOUR_DEPTH_SRT
FRAGMENT_MAIN_COLOUR_SRT
{
    vec4 lFragmentColourVec4 = vec4( 0.0, 0.0, 1.0, 1.0 );

    vec2  lFragCoordsVec2 = IN( mTexCoordsVec2 );
    float lfDepth         = FastDenormaliseDepth( lUniforms.mpPerFrame->gClipPlanesVec4, DecodeDepthFromColour( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gBuffer1Map ), lFragCoordsVec2 ) ) );
    if( lfDepth >= lUniforms.mpPerFrame->gClipPlanesVec4.y - 100.0 )
    {
        discard;
        //lfDepth = 50000.0;
    }

    vec3  lTerrainPosition  = RecreatePositionFromDepth( lfDepth, lFragCoordsVec2, lUniforms.mpPerFrame->gViewPositionVec3, lUniforms.mpCommonPerMesh->gInverseProjectionMat4, lUniforms.mpCommonPerMesh->gInverseViewMat4 );

    vec3  lPlanetPosition = lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz;
    float lfRadius        = lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w;
    float lfWaterHeight   = lUniforms.mpCustomPerMaterial->gWaterFogVec4.r;

    vec3  lNearPosition          = lUniforms.mpPerFrame->gViewPositionVec3 - lPlanetPosition;
    vec3  lFarPosition           = lTerrainPosition - lPlanetPosition;
    //if( length( lNearPosition ) < lfRadius + lfWaterHeight || length( lFarPosition ) < lfRadius + lfWaterHeight )

        float lfIntersection = GetClampedRayIntersectionPoint( lNearPosition, lFarPosition, lfRadius + lfWaterHeight, lNearPosition, lFarPosition );

        float lfCameraHeight = length( lUniforms.mpPerFrame->gViewPositionVec3 - lPlanetPosition );
        if( lfCameraHeight > lfRadius + lfWaterHeight )
        {
            if( abs( lfIntersection - 1.0 ) > 0.01 && abs( lfIntersection - 3.0 ) > 0.01 )
            {
                discard;
            }
        }
        else
        {
            lNearPosition = lFarPosition;
            if( abs( lfIntersection - 2.0 ) > 0.01 )
            {
                discard;
            }
        }

        vec3  lViewDirVec3           = normalize( lUniforms.mpPerFrame->gViewPositionVec3 - lTerrainPosition );
        vec3  lPlanetRelPositionVec3 = lNearPosition;
        vec3  lWorldPosition         = lPlanetRelPositionVec3 + lPlanetPosition;

        //-----------------------------------------------------------------------------
        ///
        ///     Normal Mapping
        ///
        //-----------------------------------------------------------------------------

        // Add waves to the local normal
        vec3   lLocalNormalVec3 = normalize( lPlanetRelPositionVec3 );

        vec2  lWindDirection1  = lUniforms.mpCustomPerMaterial->gWindDirectionVec4.xy;
        vec2  lWindDirection2  = lUniforms.mpCustomPerMaterial->gWindDirectionVec4.zw;

        float lfWaveScale1     = lUniforms.mpCustomPerMaterial->gWaveScaleVec4.x;
        float lfWaveScale2     = lUniforms.mpCustomPerMaterial->gWaveScaleVec4.y;

        vec2  lWaveAnimation1  = lWindDirection1 * lUniforms.mpPerFrame->gfTime * lUniforms.mpCustomPerMaterial->gWaveSpeedVec4.x;
        vec2  lWaveAnimation2  = lWindDirection2 * lUniforms.mpPerFrame->gfTime * lUniforms.mpCustomPerMaterial->gWaveSpeedVec4.y;

        vec3  lWaveNormal1Vec3 = GetTriPlanarNormal( lLocalNormalVec3, lPlanetRelPositionVec3, lWaveAnimation1, lfWaveScale1, SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMaterial, gLargeNormalMap ) );
        vec3  lWaveNormal2Vec3 = GetTriPlanarNormal( lLocalNormalVec3, lPlanetRelPositionVec3, lWaveAnimation2, lfWaveScale2, SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMaterial, gLargeNormalMap ) );

        lLocalNormalVec3 += lWaveNormal1Vec3 * 0.1 * lUniforms.mpCustomPerMaterial->gWaveScaleVec4.z;
        lLocalNormalVec3 += lWaveNormal2Vec3 * 0.1 * lUniforms.mpCustomPerMaterial->gWaveScaleVec4.w;
        lLocalNormalVec3 = normalize( lLocalNormalVec3 );

        // Detailed normal mapping
        vec3   lMappedNormalVec3;

        float lfNormalMapScale1    = lUniforms.mpCustomPerMaterial->gWaterSurfaceParamsVec4.x;
        float lfNormalMapScale2    = lUniforms.mpCustomPerMaterial->gWaterSurfaceParamsVec4.y;

        vec2  lNormalMapAnimation1 = lWindDirection1 * lUniforms.mpPerFrame->gfTime * lUniforms.mpCustomPerMaterial->gWaterSurfaceParamsVec4.z;
        vec2  lNormalMapAnimation2 = lWindDirection2 * lUniforms.mpPerFrame->gfTime * lUniforms.mpCustomPerMaterial->gWaterSurfaceParamsVec4.w;

        vec3  lMappedNormal1Vec3 = GetTriPlanarNormal( lLocalNormalVec3, lPlanetRelPositionVec3, lNormalMapAnimation1, lfNormalMapScale1, SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMaterial, gNormalMap ) );
        vec3  lMappedNormal2Vec3 = GetTriPlanarNormal( lLocalNormalVec3, lPlanetRelPositionVec3, lNormalMapAnimation2, lfNormalMapScale2, SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMaterial, gNormalMap ) );

        lMappedNormalVec3 = lMappedNormal1Vec3 + lMappedNormal2Vec3;
        lMappedNormalVec3 = normalize( lLocalNormalVec3 + lMappedNormalVec3 );

        float lfDistanceFade = lfCameraHeight - lfRadius;
        lfDistanceFade       = clamp( ( lfDistanceFade - 1000.0 ) / 1000.0, 0.0, 1.0 );

        lMappedNormalVec3    = mix( lMappedNormalVec3, lLocalNormalVec3, lfDistanceFade );


        //-----------------------------------------------------------------------------
        ///
        ///     Reflections
        ///
        //-----------------------------------------------------------------------------
        float  lfFresnel;
        vec3   lReflectionVec3;

        // Get reflection from previously rendered scene
        float lfSpaceHeight   = lfCameraHeight;
        lfSpaceHeight        -= lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w;
        lfSpaceHeight         = clamp( ( lfSpaceHeight - 400.0 ) / 100.0, 0.0, 1.0 );

        if( lfCameraHeight > lfRadius + lUniforms.mpCustomPerMaterial->gWaterFogVec4.r )
        {
            vec3   lProjectedCoordsVec3;

            // Calculate coordinates on rendered texture
            lProjectedCoordsVec3 = vec3( lFragCoordsVec2, lfDepth );

            // Offset based on normal map
            lProjectedCoordsVec3 += vec3( dot( lMappedNormal1Vec3, lLocalNormalVec3 ) * 0.3, dot( lMappedNormal2Vec3, lLocalNormalVec3 ) * 0.3, 0.0 );
            vec3 lWaterColourVec3 = /*GammaCorrectInput*/( lUniforms.mpCustomPerMaterial->gWaterFogColourFarVec4.rgb ) * /*GammaCorrectInput*/( lUniforms.mpCommonPerMesh->gLightColourVec4.xyz );
            lReflectionVec3 = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gReflectionMap ), lProjectedCoordsVec3.xy ).rgb;
            lReflectionVec3 = mix( lReflectionVec3*lWaterColourVec3, lReflectionVec3, lUniforms.mpCustomPerMaterial->gWaveSpeedVec4.b );
            lReflectionVec3 = mix( lReflectionVec3, lWaterColourVec3, lfSpaceHeight );
        }
        else
        {
            lReflectionVec3 = lUniforms.mpCustomPerMaterial->gWaterFogColourFarVec4.rgb * lUniforms.mpCommonPerMesh->gLightColourVec4.xyz;
        }

        // Calculate fresnel
        float lfAngle   = 0.0;

        if( lfCameraHeight > lfRadius + lUniforms.mpCustomPerMaterial->gWaterFogVec4.r )
        {
            lfAngle = dot( lViewDirVec3, lMappedNormalVec3 );
        }
        else
        {
            lfAngle = 1.0 - dot( lViewDirVec3, lMappedNormalVec3 );
        }

        float lfPower   = lUniforms.mpCustomPerMaterial->gFresnelParamsVec4.x;
        float lfBiasMin = lUniforms.mpCustomPerMaterial->gFresnelParamsVec4.y;
        float lfBiasMax = lUniforms.mpCustomPerMaterial->gFresnelParamsVec4.z - lfBiasMin;

        lfFresnel = clamp( pow( lfAngle, lfPower ), 0.0, 1.0 );
        lfFresnel = lfFresnel * lfBiasMax + lfBiasMin;

        lFragmentColourVec4.a = lfFresnel;

        //-----------------------------------------------------------------------------
        ///
        ///     Foam
        ///
        //-----------------------------------------------------------------------------
        float lfFoamArea = 1.0 - clamp( length( lWorldPosition - lTerrainPosition ) / 80.0, 0.0, 1.0 );
        float lfFoam     = 0.0;

        if( lfFoamArea > ( 1.0 / 1024 ) )
        {
            float lfFoamScale1   = lUniforms.mpCustomPerMaterial->gFoamParamsVec4.x;
            float lfFoamScale2   = lUniforms.mpCustomPerMaterial->gFoamParamsVec4.y;
            vec2  lFoamTime1Vec2 = lWindDirection1 * lUniforms.mpPerFrame->gfTime * lUniforms.mpCustomPerMaterial->gFoamParamsVec4.z;
            vec2  lFoamTime2Vec2 = lWindDirection2 * lUniforms.mpPerFrame->gfTime * lUniforms.mpCustomPerMaterial->gFoamParamsVec4.w;
            vec2  lFoamTime3Vec2 = lWindDirection2 * lUniforms.mpPerFrame->gfTime * lUniforms.mpCustomPerMaterial->gWaterSurfaceParamsVec4.z * 2.0;
            vec2  lFoamTime4Vec2 = lWindDirection1 * lUniforms.mpPerFrame->gfTime * lUniforms.mpCustomPerMaterial->gWaterSurfaceParamsVec4.w * 2.0;

            //vec3 lFoamTextureAVec3 = GetTriPlanarColourMM( lLocalNormalVec3, lPlanetRelPositionVec3, lFoamTime1Vec2, lfFoamScale1, SAMPLER2DPARAM( lUniforms.mpCustomPerMaterial->gFoamMap ) );
            //vec3 lFoamTextureBVec3 = GetTriPlanarColourMM( lLocalNormalVec3, lPlanetRelPositionVec3, lFoamTime2Vec2, lfFoamScale2, SAMPLER2DPARAM( lUniforms.mpCustomPerMaterial->gFoamMap ) );
            vec3 lFoamNormalVec3   = -GetTriPlanarNormal( lLocalNormalVec3, lPlanetRelPositionVec3, lFoamTime1Vec2, lfFoamScale1, SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMaterial, gFoamMap ) );
            vec3 lFoamNormal2Vec3  = -GetTriPlanarNormal( lLocalNormalVec3, lPlanetRelPositionVec3, lFoamTime2Vec2, lfFoamScale2, SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMaterial, gFoamMap ) );

            lFoamNormalVec3 = max( lFoamNormalVec3, lFoamNormal2Vec3 );

            lfFoam += ( saturate( lFoamNormalVec3.y ) + 0.1 ) * pow( lfFoamArea, 1.0 );
            //lfFoam += ( saturate( lFoamNormal2Vec3.y ) + 0.1 ) * pow( lfFoamArea, 1.0 );

            lfFoam += ( saturate( lFoamNormalVec3.y ) + 0.2 ) * pow( lfFoamArea, 2.0 );
            // lfFoam += ( saturate( lFoamNormal2Vec3.y ) + 0.2 ) * pow( lfFoamArea, 2.0 );

            lfFoam += ( saturate( lFoamNormalVec3.y ) + 0.8 ) * pow( lfFoamArea, 10.0 );
            // lfFoam += ( saturate( lFoamNormal2Vec3.y ) + 0.8 ) * pow( lfFoamArea, 10.0 );
            lfFoam = min( lfFoam, 1.0 );

            lFoamNormalVec3          = normalize( lFoamNormalVec3 * lfFoam + lMappedNormalVec3 );
            //lFoamNormalVec3          = normalize( ( -lFoamNormalVec3 ) * lfFoam + lMappedNormalVec3 );

            lfFoam *= lUniforms.mpCustomPerMaterial->gFoamColourVec4.a;

            lFragmentColourVec4.rgb = lFragmentColourVec4.rgb + ( lUniforms.mpCustomPerMaterial->gFoamColourVec4.rgb ) * lfFoam;
            lFragmentColourVec4.a    = mix( lFragmentColourVec4.a, 1.0, lfFoam * lfFoam );
            // lFragmentColourVec4.a    = clamp( lFragmentColourVec4.a + lfFoam, 0.0, 1.0 );

            lMappedNormalVec3        = lFoamNormalVec3;//mix( lMappedNormalVec3, lFoamNormalVec3, lfFoam );

            //lFragmentColourVec4.rgb = lFoamNormalVec3;
        }

        //-----------------------------------------------------------------------------
        ///
        ///     Lighting
        ///
        //-----------------------------------------------------------------------------
        float lfHeight = 0.0;
        vec3 lWorldUpVec3 = GetWorldUp( lWorldPosition, lUniforms.mpCommonPerMesh->gPlanetPositionVec4, lfHeight );

        float lfShadow = 1.0;

        vec3 lPosForShadowVec3 = lWorldPosition + lMappedNormalVec3;
        float lfShadowFade = clamp( ( length( lUniforms.mpPerFrame->gViewPositionVec3 - lPosForShadowVec3 ) - lUniforms.mpPerFrame->gShadowFadeParamVec4.x ) * lUniforms.mpPerFrame->gShadowFadeParamVec4.y, 0.0, 1.0 );
        if( lfShadowFade != 1.0 )
        {
            lfShadow = ComputeShadowIntensity( DEREF_PTR( lUniforms.mpCustomPerMaterial ), DEREF_PTR( lUniforms.mpPerFrame ), DEREF_PTR( lUniforms.mpCommonPerMesh ), lPosForShadowVec3, lLocalNormalVec3, vec2( 0.0, 0.0 ), false );
        }
        // fake some ambient.
        lfShadow = 0.12 + ( lfShadow * 0.38 );

        vec3 lUp = GetWorldUp( lUniforms.mpPerFrame->gViewPositionVec3, lUniforms.mpCommonPerMesh->gPlanetPositionVec4 );
        vec3 lCross1 = normalize( cross( lUp, vec3( 0.0, 0.0, 1.0 ) ) );
        vec3 lCross2 = normalize( cross( lUp, lCross1 ) );
        lCross1 = normalize( cross( lUp, lCross2 ) );
        vec3 lViewPos = lUp * lPlanetPosition;
        vec3 lGroundPos = GetWorldUp( lWorldPosition, lUniforms.mpCommonPerMesh->gPlanetPositionVec4 ) * lfRadius;
        vec3 lDir = ( lGroundPos - lViewPos ) * 0.001;

        vec2 lCloudTexCoords = vec2( dot( lDir, lCross1 ), dot( lDir, lCross2 ) ) * 0.5;
        lCloudTexCoords += vec2( 0.5, 0.5 );

        lfShadow *= 1.0 - texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gCloudShadowMap ), lCloudTexCoords ).r * 0.5;

        lFragmentColourVec4.rgb = ComputeWaterLighting(
            lWorldPosition,
            lMappedNormalVec3,
            lUniforms.mpPerFrame->gViewPositionVec3,
            -lUniforms.mpPerFrame->gLightDirectionVec4.xyz,
            lFragmentColourVec4.xyz,
            lReflectionVec3,
            lUniforms.mpCommonPerMesh->gLightColourVec4,
            lUniforms.mpCommonPerMesh->gLightOriginVec4,
            mix( lUniforms.mpCustomPerMaterial->gMaterialParamsVec4.z, 0.0, lfFoam ),
            lUniforms.mpCustomPerMaterial->gMaterialParamsVec4.y,
            mix( lUniforms.mpCustomPerMaterial->gFresnelParamsVec4.w, 0.4, lfFoam ),
            lfShadow );

        //-----------------------------------------------------------------------------
        ///
        ///     Fog
        ///
        //-----------------------------------------------------------------------------
        {
            vec3  lWaterColourNearVec3     = /*GammaCorrectInput*/( lUniforms.mpCustomPerMaterial->gWaterFogColourNearVec4.rgb );
            vec3  lWaterColourFarVec3      = /*GammaCorrectInput*/( lUniforms.mpCustomPerMaterial->gWaterFogColourFarVec4.rgb );
            vec3  lLightColourVec3         = /*GammaCorrectInput*/( lUniforms.mpCommonPerMesh->gLightColourVec4.rgb );
            float lfWaterStrength          = lUniforms.mpCustomPerMaterial->gWaterFogVec4.g;
            float lfWaterColourStrength    = lUniforms.mpCustomPerMaterial->gWaterFogVec4.b;
            float lfWaterMultiplyStrength  = lUniforms.mpCustomPerMaterial->gWaterFogVec4.a;

            vec3  lNearPosition = lUniforms.mpPerFrame->gViewPositionVec3 - lPlanetPosition;
            vec3  lFarPosition  = lPlanetRelPositionVec3;
            if( length( lNearPosition ) < lfRadius + lfWaterHeight )
            {
                float lfDarken          = clamp( dot( -lUniforms.mpPerFrame->gLightDirectionVec4.xyz, normalize( lFarPosition ) ), 0.0, 1.0 );

                float lfFogDistance = length( lFarPosition - lNearPosition );

                float lfFogValue        = lfFogDistance * lfWaterStrength;
                lfFogValue              = 1.0 / exp( lfFogValue * lfFogValue );
                lfFogValue              = 1.0 - clamp( lfFogValue, 0.0, 1.0 );

                float lfWaterFade       = lfFogDistance * lfWaterColourStrength;
                lfWaterFade             = 1.0 / exp( lfWaterFade * lfWaterFade );
                lfWaterFade             = 1.0 - clamp( lfWaterFade, 0.0, 1.0 );

                vec3 lWaterColour        = mix( lWaterColourNearVec3, lWaterColourFarVec3, clamp( lfWaterFade, 0.0, 1.0 ) ) * lLightColourVec3 * lfDarken;
                lFragmentColourVec4.rgb  = mix( lFragmentColourVec4.rgb, lWaterColour, clamp( lfFogValue, 0.0, 1.0 ) );
                lFragmentColourVec4.a    = max( lFragmentColourVec4.a, clamp( lfFogValue * 2.0, 0.0, 1.0 ) );
            }
        }

        if( lfCameraHeight < lfRadius + lfWaterHeight )
        {
            float lfAlphaFade     = saturate( ( lfRadius + lUniforms.mpCustomPerMaterial->gWaterFogVec4.r - lfCameraHeight ) / 15.0 );
            lFragmentColourVec4.a = max( lFragmentColourVec4.a, 0.85 + lfAlphaFade * 0.15 );
        }

        {
            // Read depth from g-buffer
            float lfDepthFade = saturate( ( lfDepth - 400.0 ) / 400.0 );

            vec4 lOpaqueColour = vec4( mix( lUniforms.mpCustomPerMaterial->gWaterFogColourFarVec4.rgb * lUniforms.mpCommonPerMesh->gLightColourVec4.xyz, lFragmentColourVec4.rgb, lFragmentColourVec4.a ), 1.0 );
            lFragmentColourVec4 = mix( lFragmentColourVec4, lOpaqueColour, lfDepthFade );

        }

        lFragmentColourVec4.a = mix( lFragmentColourVec4.a, 0.0, lfDistanceFade );


    FRAGMENT_COLOUR = lFragmentColourVec4;

    float lfNewDepth  = dot( lWorldPosition - lUniforms.mpPerFrame->gViewPositionVec3, -normalize( MAT4_GET_COLUMN( lUniforms.mpPerFrame->gCameraMat4, 2 ) ) );
    // FRAGMENT_DEPTH  = LinearToReverseZDepth( lUniforms.mpPerFrame->gClipPlanesVec4, lfNewDepth );

}
