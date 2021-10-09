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

#ifdef D_PLATFORM_ORBIS
#pragma argument(targetoccupancy_atallcosts=5)
#pragma argument(unrollallloops)
#endif

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
#include "Common/CommonVertex.shader.h"
#include "Common/CommonGBuffer.shader.h"
#include "Common/CommonNoise.shader.h"
#include "Common/CommonPostProcess.shader.h"

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


STATIC_CONST int NUM_STEPS = 8; 

STATIC_CONST float MAX_DISTANCE = 800.0;

STATIC_CONST mat3 octave_m = mat3(  0.00,  1.60,  1.20,
                                   -1.60,  0.72, -0.96,
                                   -1.20, -0.96,  1.28 );

float 
SeaNoise( 
    vec3  lPositionVec3, 
    float lChoppyness, 
    SAMPLER3DARG( lNoiseMap ) )
{
    return  texture3DLod( lNoiseMap, lPositionVec3, 0.0 ).r;
}

float 
Map( 
    vec3  lOriginVec3, 
    vec3  lPositionVec3, 
    float lfSeaLevel, 
    float lfTime,
    float lfWaveHeight,
    float lfWaveFreq,
    float lfWaveChoppiness,
    SAMPLER3DARG( lNoiseMap ),
    int liOctaves )
{
    vec3 lCurrentPositionVec3 = lPositionVec3;

    float lfDepth = length( lOriginVec3 - lPositionVec3 );

    float lfDepthFade = 1.0 - saturate( ( lfDepth - 30.0 ) / 50.0 );
 
    float lfNoise, lfHeight = 0.0;
    for( int i = 0; i < liOctaves; i++ )
    {
        //#ifdef D_FROM_BELOW
        //if( lfDepth > (200.0*200.0) && i == 1 )
        //{
        //    // stop after 1 octave
        //    break;
        //}
        //#else
        //if ( lfDepth > (100.0*100.0) && i == 1 )
        //{
        //    // stop after 1 octave
        //    break;
        //}            
        //#endif
        lfNoise   = SeaNoise( ( lCurrentPositionVec3 + float2vec3( lfTime ) )* lfWaveFreq, lfWaveChoppiness, SAMPLER3DPARAM( lNoiseMap ) );
        lfNoise  += SeaNoise( ( lCurrentPositionVec3 - float2vec3( lfTime ) )* lfWaveFreq, lfWaveChoppiness, SAMPLER3DPARAM( lNoiseMap ) );
        lfNoise *= 0.5;
        lfHeight += lfNoise * lfWaveHeight;
        lCurrentPositionVec3 = MUL( octave_m, lCurrentPositionVec3 ); 
        lfWaveFreq   *= lfWaveChoppiness;
        lfWaveHeight *= 0.22 * lfDepthFade;
        //lfWaveChoppiness = mix( lfWaveChoppiness, 1.0, 0.2 );
    }
    return (length( lPositionVec3 ) - lfSeaLevel) - lfHeight;
}

vec3 
GetWaveNormal( 
    vec3  lOriginVec3, 
    vec3  lPositionVec3,
    float lfOffset, 
    float lfSeaLevel, 
    float lfTime, 
    float lfWaveHeight,
    float lfWaveFreq,
    float lfWaveChoppiness,
    SAMPLER3DARG( lNoiseMap ) )
{
    vec3 lNormalVec3;
    int liOctaves = 2;

    float lfHeight = Map( lOriginVec3, lPositionVec3, lfSeaLevel, lfTime, lfWaveHeight, lfWaveFreq, lfWaveChoppiness, SAMPLER3DPARAM( lNoiseMap ), liOctaves );
    lNormalVec3.x  = Map( lOriginVec3, vec3( lPositionVec3.x + lfOffset, lPositionVec3.y, lPositionVec3.z ), lfSeaLevel, lfTime, lfWaveHeight, lfWaveFreq, lfWaveChoppiness, SAMPLER3DPARAM( lNoiseMap ), liOctaves ) - lfHeight;
    lNormalVec3.y  = Map( lOriginVec3, vec3( lPositionVec3.x, lPositionVec3.y + lfOffset, lPositionVec3.z ), lfSeaLevel, lfTime, lfWaveHeight, lfWaveFreq, lfWaveChoppiness, SAMPLER3DPARAM( lNoiseMap ), liOctaves ) - lfHeight;
    lNormalVec3.z  = Map( lOriginVec3, vec3( lPositionVec3.x, lPositionVec3.y, lPositionVec3.z + lfOffset ), lfSeaLevel, lfTime, lfWaveHeight, lfWaveFreq, lfWaveChoppiness, SAMPLER3DPARAM( lNoiseMap ), liOctaves ) - lfHeight;

    return normalize( lNormalVec3 );
}

float 
GetWavePosition( 
    in vec3  lOriginVec3, 
    in vec3  lDirectionVec3, 
    in float lfSeaLevel, 
    in float lfTime, 
    float lfWaveHeight,
    float lfWaveFreq,
    float lfWaveChoppiness,
    SAMPLER3DARG( lNoiseMap ),
    out vec3 lPosition )
{
    float lfNear = 0.0;
    float lfFar  = MAX_DISTANCE;
    int liOctaves = 2;

    float hx = Map( lOriginVec3, lOriginVec3 + lDirectionVec3 * lfFar, lfSeaLevel, lfTime, lfWaveHeight, lfWaveFreq, lfWaveChoppiness, SAMPLER3DPARAM( lNoiseMap ), liOctaves );
    if( hx > 0.0 )
    {
        return lfFar;
    }

    float hm = Map( lOriginVec3, lOriginVec3 + lDirectionVec3 * lfNear, lfSeaLevel, lfTime, lfWaveHeight, lfWaveFreq, lfWaveChoppiness, SAMPLER3DPARAM( lNoiseMap ), liOctaves );
    float lfMid = 0.0;

    for( int i = 0; i < NUM_STEPS; i++ )
    {
        lfMid = mix( lfNear, lfFar, hm/( hm-hx ) );
        lPosition = lOriginVec3 + lDirectionVec3 * lfMid;
        float hmid = Map( lOriginVec3, lPosition, lfSeaLevel, lfTime, lfWaveHeight, lfWaveFreq, lfWaveChoppiness, SAMPLER3DPARAM( lNoiseMap ), liOctaves );
        if( hmid < 0.0 )
        {
            lfFar = lfMid;
            hx = hmid;
        }
        else
        {
            lfNear = lfMid;
            hm = hmid;
        }
    }
    lfMid = mix( lfNear, lfFar, hm/( hm-hx ) );
    return lfMid;
}

float
GetWavePositionFromBelow(
    vec3  lOriginVec3,
    vec3  lDirectionVec3,
    float lfSeaLevel,
    float lfTime,
    float lfWaveHeight,
    float lfWaveFreq,
    float lfWaveChoppiness,
    SAMPLER3DARG( lNoiseMap ),
    out vec3 lPosition )
{
    float lfNear = 0.0;
    float lfFar  = MAX_DISTANCE;
    int liOctaves = 3;

    float hx = Map( lOriginVec3, lOriginVec3 + lDirectionVec3 * lfFar, lfSeaLevel, lfTime, lfWaveHeight, lfWaveFreq, lfWaveChoppiness, SAMPLER3DPARAM( lNoiseMap ), liOctaves );
    if( hx < 0.0 ) // reverse - we're moving from high density to low
    {
        return lfFar;
    }

    float hm = Map( lOriginVec3, lOriginVec3 + lDirectionVec3 * lfNear, lfSeaLevel, lfTime, lfWaveHeight, lfWaveFreq, lfWaveChoppiness, SAMPLER3DPARAM( lNoiseMap ), liOctaves );
    float lfMid = 0.0;

    for( int i = 0; i < NUM_STEPS; i++ )
    {
        lfMid = mix( lfNear, lfFar, hm/( hm-hx ) );
        lPosition = lOriginVec3 + lDirectionVec3 * lfMid;
        float hmid = Map( lOriginVec3, lPosition, lfSeaLevel, lfTime, lfWaveHeight, lfWaveFreq, lfWaveChoppiness, SAMPLER3DPARAM( lNoiseMap ), liOctaves );
        if( hmid >= 0.0 ) // reverse - we're moving from high density to low
        {
            lfFar = lfMid;
            hx = hmid;
        }
        else
        {
            lfNear = lfMid;
            hm = hmid;
        }
    }
    return lfMid;
}


vec4
GetSeaColour(
    vec3  lPositionVec3,
    vec3  lNormalVec3,
    vec3  lLightVec3,
    vec3  lToCameraVec3,
    vec3  lToOriginVec3,
    vec3  lNearColourVec3,
    vec3  lFarColourVec3,
    vec3  lSkyColourVec3,
    float lfWaveHeight,
    float lfFresnelPow,
    float lfFresnelMultiply,
    float lfFresnelAlpha,
    float lfSeaLevel,
    float lfSmoothness  )
{
    float lfFresnel = clamp( 1.0 - dot( lNormalVec3, -lToCameraVec3 ), 0.0, 1.0 );
    lfFresnel = pow( lfFresnel, lfFresnelPow ) * lfFresnelMultiply;

    float lfDiffuse  = smoothstep( 0.999, 1.0, dot( lNormalVec3, lLightVec3 ) );
    float lfSpecular = pow( max( dot( reflect( lToCameraVec3, lNormalVec3 ), lLightVec3 ), 0.0 ), 120.0 ) * 10.0 * lfSmoothness;

    vec3 lReflectedVec3 = lSkyColourVec3;
    vec3 lRefractedVec3 = lFarColourVec3 + lfDiffuse * lNearColourVec3 * 0.12;

    vec4 lSeaColourVec4;
    lSeaColourVec4.rgb = mix( lRefractedVec3, lReflectedVec3, lfFresnel );
    lSeaColourVec4.a   = saturate( lfFresnel + lfSpecular + lfFresnelAlpha );

    float lfAttenuation = max( 1.0 - dot( lToOriginVec3, lToOriginVec3 ) * 0.001, 0.0 );
    lSeaColourVec4.rgb  += lNearColourVec3 * ( length( lPositionVec3 ) - ( lfWaveHeight + lfSeaLevel ) ) * 0.18 * lfAttenuation;

    lSeaColourVec4.rgb  += float2vec3( lfSpecular );
    //lSeaColourVec4 = vec4( 1.0, 0.0, 0.0, 1.0 );
    //lSeaColourVec4.rgb = lNormalVec3 * 0.5 + vec3( 0.5 );
    //lSeaColourVec4.a = 1.0;

    return lSeaColourVec4;
}


vec4
GetFoamColour(
    in CustomPerMaterialUniforms lPerMaterialUniforms,
    in vec3 lPlanetRelPositionVec3,
    in vec3 lLocalNormalVec3,
    in float lfTime,
    in float lfFoamArea,
    in float lfDepth )
{
    vec2  lWindDirection1  = lPerMaterialUniforms.gWindDirectionVec4.xy;
    vec2  lWindDirection2  = lPerMaterialUniforms.gWindDirectionVec4.zw;

    float lfFoamScale1   = lPerMaterialUniforms.gFoamParamsVec4.x;
    float lfFoamScale2   = lPerMaterialUniforms.gFoamParamsVec4.y;
    vec2  lFoamTime1Vec2 = lWindDirection1 * lfTime * lPerMaterialUniforms.gFoamParamsVec4.z;
    vec2  lFoamTime2Vec2 = lWindDirection2 * lfTime * lPerMaterialUniforms.gFoamParamsVec4.w;
    vec2  lFoamTime3Vec2 = lWindDirection2 * lfTime * lPerMaterialUniforms.gWaterSurfaceParamsVec4.z * 2.0;
    vec2  lFoamTime4Vec2 = lWindDirection1 * lfTime * lPerMaterialUniforms.gWaterSurfaceParamsVec4.w * 2.0;

    float lfFoamHeightTex  = 1.0 - GetTriPlanarColourMM( lLocalNormalVec3, lPlanetRelPositionVec3, lFoamTime2Vec2, lfFoamScale2, SAMPLER2DPARAM_LOCAL( lPerMaterialUniforms, gFoamHeightMap ) ).r;

    //lPlanetRelPositionVec3 = vec3( lPlanetRelPositionVec3.y, lPlanetRelPositionVec3.z, lPlanetRelPositionVec3.x );
    float lfFoamHeightTex2 = 1.0 - GetTriPlanarColourMM( lLocalNormalVec3, lPlanetRelPositionVec3, lFoamTime1Vec2, lfFoamScale1, SAMPLER2DPARAM_LOCAL( lPerMaterialUniforms, gFoamHeightMap ) ).r;

    float lfFoam = 0.0;

    lfFoamHeightTex = ( lfFoamHeightTex + lfFoamHeightTex2 ) / 2.0;

    lfFoam = smoothstep( clamp( lfFoamHeightTex - 0.15, 0.0, 1.0 ), lfFoamHeightTex + 0.15, lfFoamArea );
    lfFoam += pow( lfFoamArea, 10.0 );
    lfFoam *= lfFoamHeightTex2;
    lfFoam = min( lfFoam, 1.0 );

   // lfFoam *= lPerMaterialUniforms.gFoamColourVec4.a;

    vec4 lFoamColour;
    
    lFoamColour = ( lPerMaterialUniforms.gFoamColourVec4 ) * lfFoam;

    lFoamColour.a = min( pow( lfDepth, 10.0 ), 1.0 ); 

    return lFoamColour;
}

vec4
GetFogColour(
    vec4 lInputColourVec4,
    vec3 lNearColour,
    vec3 lFarColour,
    vec3 lLightColour,
    vec3 lLightDirection,
    vec3 lPlanetRelPositionVec3,
    vec3 lCameraPositionVec3,
    float lfWaterStrength,
    float lfWaterColourStrength,
    float lfWaterMultiplyStrength,
    float lfSeaLevel )
{
    vec4  lFragmentColourVec4 = lInputColourVec4;
    vec3  lNearPosition       = lCameraPositionVec3;
    vec3  lFarPosition        = lPlanetRelPositionVec3;

    float lfDarken          = clamp( dot( lLightDirection, normalize( lFarPosition ) ), 0.0, 1.0 );

    float lfFogDistance = length( lFarPosition - lNearPosition );

    float lfFogValue        = lfFogDistance * lfWaterStrength;
    lfFogValue              = 1.0 / exp( lfFogValue * lfFogValue );
    lfFogValue              = 1.0 - clamp( lfFogValue, 0.0, 1.0 );

    float lfWaterFade       = lfFogDistance * lfWaterColourStrength;
    lfWaterFade             = 1.0 / exp( lfWaterFade * lfWaterFade );
    lfWaterFade             = 1.0 - clamp( lfWaterFade, 0.0, 1.0 );

    vec3 lWaterColour        = mix( lNearColour, lFarColour, clamp( lfWaterFade, 0.0, 1.0 ) );// * /*lLightColour */ lfDarken;
    lFragmentColourVec4.rgb  = mix( lFragmentColourVec4.rgb, lWaterColour, clamp( lfFogValue, 0.0, 1.0 ) );
    lFragmentColourVec4.a    = max( lFragmentColourVec4.a, clamp( lfFogValue * 2.0, 0.0, 1.0 ) );

    return lFragmentColourVec4;
}

#ifdef D_FULLSCREEN

FRAGMENT_MAIN_COLOUR_DEPTH_SRT
{
    vec3 lViewPos = lUniforms.mpPerFrame->gViewPositionVec3;

    float lfHeightFade = length( lViewPos - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz ) - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w;
    lfHeightFade       = clamp( ( lfHeightFade - 1000.0 ) / 500.0, 0.0, 1.0 );
    if( lfHeightFade >= 1.0 )
    {
        discard;
    }

    vec2  lFragCoordsVec2          = IN( mTexCoordsVec2 );
    //lFragCoordsVec2 = GetDejitteredTexCoord( lFragCoordsVec2, lUniforms.mpPerFrame->gDeJitterVec4 );

    float lfOriginalDepth      = FastDenormaliseDepth( lUniforms.mpPerFrame->gClipPlanesVec4, DecodeDepthFromColour( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gBuffer1Map ), lFragCoordsVec2 ) ) );
    float lfSceneDepth         = lfOriginalDepth;
    if( lfOriginalDepth >= lUniforms.mpPerFrame->gClipPlanesVec4.y - 100.0 )
    {
        lfSceneDepth = 50000.0;
    }

    vec3  lPositionVec3;
    vec3  lLightDirectionVec3;
    float lfAttenuation;
    lPositionVec3  = RecreatePositionFromDepth( 
                        lfSceneDepth, 
                        lFragCoordsVec2, 
                        lViewPos,
                        lUniforms.mpCommonPerMesh->gInverseProjectionMat4, 
                        lUniforms.mpCommonPerMesh->gInverseViewMat4 );

    float lfWaterHeight = lUniforms.mpCustomPerMaterial->gWaterFogVec4.r;

    float lfAddedHeight = length( lViewPos - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz ) - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w;
    lfAddedHeight       = clamp( ( lfAddedHeight - lfWaterHeight ) / 3.0, 0.0, 1.0 );
    lfAddedHeight       = lUniforms.mpCustomPerMaterial->gFresnelParamsVec4.r;//mix( 0.0, 1.0, lfAddedHeight );

    vec3  lPlanetPosition      = lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz;
    float lfRadius             = lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w;

    float lfSeaLevel           = lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w + lfWaterHeight;
    float lfTime               = lUniforms.mpPerFrame->gfTime;

    vec3  lNearPosition        = lViewPos - lPlanetPosition;
    vec3  lFarPosition         = lPositionVec3 - lPlanetPosition;

    vec3  lLightColourVec3        = ( lUniforms.mpCommonPerMesh->gLightColourVec4.rgb );
    vec3  lWaterColourBaseVec3    = ( lUniforms.mpCustomPerMaterial->gWaterColourBaseVec4.rgb )    * lLightColourVec3;
    vec3  lWaterColourAddVec3     = ( lUniforms.mpCustomPerMaterial->gWaterColourAddVec4.rgb )     * lLightColourVec3;
    vec3  lWaterFogColourFarVec3  = ( lUniforms.mpCustomPerMaterial->gWaterFogColourFarVec4.rgb )  * lLightColourVec3;
    vec3  lWaterFogColourNearVec3 = ( lUniforms.mpCustomPerMaterial->gWaterFogColourNearVec4.rgb ) * lLightColourVec3;


    vec3 lSurfacePositionVec3, lDummyPos2;

    float lfRayTest = GetRayIntersectionPoint( lNearPosition, lFarPosition, ( lfRadius + lfWaterHeight ), lSurfacePositionVec3, lDummyPos2 );

#ifdef D_FROM_BELOW

    lSurfacePositionVec3 = lDummyPos2;

    if( lfRayTest == 0.0 )
    {
        discard;
    }
    if( lfRayTest == 1.0 )
    {
        discard;
    }
    if( lfRayTest == 3.0 )
    {
        discard;
    }
#else
    if( lfRayTest == 0.0 )
    {
        discard;
    }
    if( lfRayTest == 2.0 )
    {
        discard;
    }
#endif // D_FROM_BELOW

    float lfHeightTrace;
    vec3  lUp               = normalize( lSurfacePositionVec3 );
    lSurfacePositionVec3 -= lUp;
    lfSeaLevel           -= lfAddedHeight * 1.0;

    vec3  lRayOriginVec3    = lNearPosition;
    vec3  lRayDirectionVec3 = normalize( lFarPosition - lRayOriginVec3 );
    vec3  lWavePositionVec3 = lSurfacePositionVec3;
    vec3  lNormal = lUp;

    float lfWaveHeight      = lUniforms.mpCustomPerMaterial->gFresnelParamsVec4.r;
    float lfWaveFreq        = lUniforms.mpCustomPerMaterial->gFresnelParamsVec4.g;
    float lfWaveChoppiness  = lUniforms.mpCustomPerMaterial->gFresnelParamsVec4.b;
    float lfWaveCutoff      = lUniforms.mpCustomPerMaterial->gFresnelParamsVec4.a;
    float lfEpsilonNormal   = lUniforms.mpCustomPerMaterial->gWaterSurfaceParamsVec4.r;

#ifdef D_FROM_BELOW
    {
        lfHeightTrace = GetWavePositionFromBelow( lRayOriginVec3, lRayDirectionVec3, lfSeaLevel, lfTime, lfWaveHeight, lfWaveFreq, lfWaveChoppiness, SAMPLER3DPARAM_SRT( lUniforms.mpCustomPerMaterial,gNoiseMap ), lWavePositionVec3 );
    }
#else
    {
        lfHeightTrace = GetWavePosition( lRayOriginVec3, lRayDirectionVec3, lfSeaLevel, lfTime, lfWaveHeight, lfWaveFreq, lfWaveChoppiness, SAMPLER3DPARAM_SRT( lUniforms.mpCustomPerMaterial,gNoiseMap ), lWavePositionVec3 );
    }
#endif // D_FROM_BELOW


    vec3 lToOriginVec3 = lWavePositionVec3 - lRayOriginVec3;

    float lfBlend = saturate( ( lfHeightTrace - ( MAX_DISTANCE - 500.0 ) ) / 500.0 );

    vec3 lAt = -normalize( lToOriginVec3 );
    float lfDot = dot( lUp, lAt );
    lfDot = abs( lfDot );
    lfDot = smoothstep( 0.02, 0.12, lfDot );
    lfBlend  = 1.0 - ( ( 1.0 - lfBlend ) * lfDot );

    if( lfHeightTrace < MAX_DISTANCE && lfBlend < 1.0 )
    {
        float lfEpsilon = length( lViewPos - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz ) - ( lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w + lfWaterHeight );
        lfEpsilon = clamp( ( lfEpsilon - 20.0 ) / 100.0, 0.0, 1.0 );
        lfEpsilon = mix(  0.1, lfEpsilonNormal, lfEpsilon );
        lfEpsilon /= ( 1920.0f * 0.5f );

        lNormal = GetWaveNormal( lRayOriginVec3, lWavePositionVec3, max( dot( lToOriginVec3, lToOriginVec3 ) * lfEpsilon, lfWaveCutoff ), lfSeaLevel, lfTime, lfWaveHeight, lfWaveFreq, lfWaveChoppiness, SAMPLER3DPARAM_SRT( lUniforms.mpCustomPerMaterial, gNoiseMap ) );
    }
    else
    {
        lWavePositionVec3 = lSurfacePositionVec3;
    }

    vec3 lLight  = normalize( -lUniforms.mpPerFrame->gLightDirectionVec4.xyz );

    lNormal           = normalize( mix( lNormal, lUp, lfBlend ) );
    lWavePositionVec3 = mix( lWavePositionVec3, lSurfacePositionVec3, lfBlend );
    float lfNewDepth  = dot( ( lWavePositionVec3 + lPlanetPosition ) - lViewPos, -normalize( MAT4_GET_COLUMN( lUniforms.mpPerFrame->gCameraMat4, 2 ).xyz ) );

    // Reflections
    vec4 lFinalColour;

    #ifdef D_FROM_BELOW
    {
        float lfFresnelPow      = lUniforms.mpCustomPerMaterial->gWaveSpeedVec4.r;
        float lfFresnelMultiply = lUniforms.mpCustomPerMaterial->gWaveSpeedVec4.g;
        float lfFresnelAlpha    = lUniforms.mpCustomPerMaterial->gWaveSpeedVec4.b;

        float lfReflectionScale1      = lUniforms.mpCustomPerMaterial->gFoamParamsVec4.x * 0.01;
        vec2  lReflectionTime1Vec2    = lUniforms.mpCustomPerMaterial->gWindDirectionVec4.xy * lfTime * lUniforms.mpCustomPerMaterial->gFoamParamsVec4.z * 0.1;
        vec3  lReflectionTextureAVec3 = GetTriPlanarColourMM( lNormal, lSurfacePositionVec3, lReflectionTime1Vec2, lfReflectionScale1, SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMaterial, gFoamMap ) );

        // Offset based on normal map
        vec2 lProjectedCoordsVec2 = lFragCoordsVec2;
        lProjectedCoordsVec2 += vec2( lReflectionTextureAVec3.r, lReflectionTextureAVec3.r ) * 0.2 - float2vec2( 0.1 );
        lProjectedCoordsVec2 -= float2vec2( saturate( ( length( lWavePositionVec3 - lSurfacePositionVec3 ) - 1.0 ) * 0.05 ) ) * 0.05;
        vec3 lReflectionVec3  = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gReflectionMap ), lProjectedCoordsVec2 ).rgb;

        vec3 lWaterColourVec3 = lWaterFogColourNearVec3;

        float lfCamHeight = lfSeaLevel - length( lNearPosition );
        vec3 lCamPos      = normalize( lNearPosition ) * ( lfSeaLevel + lfCamHeight );
        lCamPos           = normalize( lCamPos - lWavePositionVec3 );

        lLight = reflect( lLight, lUp );

        lFinalColour = GetSeaColour( lWavePositionVec3, -lNormal, lLight, lRayDirectionVec3, lToOriginVec3, lWaterColourAddVec3, lWaterColourBaseVec3, lReflectionVec3, 0.6, lfFresnelPow, lfFresnelMultiply, lfFresnelAlpha, lfSeaLevel, 1.0 );

        lFinalColour = GetFogColour(
                    lFinalColour,
                    lWaterFogColourNearVec3,
                    lWaterFogColourFarVec3,
                    lUniforms.mpCommonPerMesh->gLightColourVec4.rgb,
                    lLight,
                    lWavePositionVec3,
                    lViewPos - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz,
                    lUniforms.mpCustomPerMaterial->gWaterFogVec4.g,
                    lUniforms.mpCustomPerMaterial->gWaterFogVec4.b,
                    lUniforms.mpCustomPerMaterial->gWaterFogVec4.a,
                    lfSeaLevel );

    }
    #else
    {
        if( lfNewDepth > lfSceneDepth )
        {
            discard;
        }

        vec2 lProjectedCoordsVec2 = lFragCoordsVec2;

        float lfFresnelPow      = lUniforms.mpCustomPerMaterial->gWaveScaleVec4.r;
        float lfFresnelMultiply = lUniforms.mpCustomPerMaterial->gWaveScaleVec4.g;
        float lfFresnelAlpha    = lUniforms.mpCustomPerMaterial->gWaveScaleVec4.b;

        float lfReflectionScale1      = lUniforms.mpCustomPerMaterial->gFoamParamsVec4.x * 0.01;
        vec2  lReflectionTime1Vec2    = lUniforms.mpCustomPerMaterial->gWindDirectionVec4.xy * lfTime * lUniforms.mpCustomPerMaterial->gFoamParamsVec4.z * 0.1;
        vec3  lReflectionTextureAVec3 = GetTriPlanarColourMM( lNormal, lSurfacePositionVec3, lReflectionTime1Vec2, lfReflectionScale1, SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMaterial, gFoamMap ) );

        // Offset based on normal map
        lProjectedCoordsVec2 += vec2( lReflectionTextureAVec3.r, lReflectionTextureAVec3.r ) * 0.06 - float2vec2( 0.03 );
        lProjectedCoordsVec2 -= float2vec2( saturate( ( length( lWavePositionVec3 - lSurfacePositionVec3 ) - 1.0 ) * 0.05 ) ) * 0.05;
        vec3 lReflectionVec3  = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gReflectionMap ), lProjectedCoordsVec2 ).rgb;

        float lfFoamScale1      = lUniforms.mpCustomPerMaterial->gFoamParamsVec4.x;
        vec2  lFoamTime1Vec2    = lUniforms.mpCustomPerMaterial->gWindDirectionVec4.xy * lfTime * lUniforms.mpCustomPerMaterial->gFoamParamsVec4.z;
        vec3  lFoamTextureAVec3 = GetTriPlanarColourMM( lUp, lSurfacePositionVec3, lFoamTime1Vec2, lfFoamScale1, SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMaterial, gFoamMap ) );


        vec2  lFoamCoords1  = lFragCoordsVec2 + vec2( lFoamTextureAVec3.r, lFoamTextureAVec3.r ) * 0.08 - float2vec2( 0.04 );
        float lfFoamDepth1  = FastDenormaliseDepth( lUniforms.mpPerFrame->gClipPlanesVec4, DecodeDepthFromColour( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gBuffer1Map ), lFoamCoords1 ) ) );
        vec2  lFoamCoords3  = lFragCoordsVec2 - ( vec2( lFoamTextureAVec3.r, lFoamTextureAVec3.r ) * 0.08 - float2vec2( 0.04 ) );
        float lfFoamDepth3  = FastDenormaliseDepth( lUniforms.mpPerFrame->gClipPlanesVec4, DecodeDepthFromColour( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gBuffer1Map ), lFoamCoords3 ) ) );
        //lfFoamDepth1 = min( lfFoamDepth1, min( lfSceneDepth, lfFoamDepth3 ) );
        lfFoamDepth1 = min( lfFoamDepth1, lfFoamDepth3 );

        float lfFoamHeight1 = 1.0 - saturate( ( lfFoamDepth1 - lfNewDepth ) / 8.0 );
        float lfFoamHeight2 = saturate( ( lfSceneDepth - lfNewDepth ) );
        if( lfFoamDepth1 < lfNewDepth )
        {
            lfFoamHeight1 = 1.0 - saturate( ( lfSceneDepth - lfNewDepth ) / 8.0);
        }
        vec4 lFoamColour = GetFoamColour( DEREF_PTR( lUniforms.mpCustomPerMaterial ), lSurfacePositionVec3, lUp, lfTime, lfFoamHeight1, lfFoamHeight2 );

        float lfFoamBlend   = saturate( ( max( lfFoamDepth1, lfNewDepth) - 5.0 ) / 2.0 ) * 0.8 + 0.2;
        lFoamColour.rgb *= lfDot * lfFoamBlend;

        // Calculate colour
        lFinalColour      = GetSeaColour( lWavePositionVec3, lNormal, lLight, lRayDirectionVec3, lToOriginVec3, lWaterColourAddVec3, lWaterColourBaseVec3, lReflectionVec3, 0.6, lfFresnelPow, lfFresnelMultiply, lfFresnelAlpha, lfSeaLevel, 1.0 );
        lFinalColour.rgb += lFoamColour.rgb;
        lFinalColour.a    = min( lFinalColour.a, lFoamColour.a );

        lFinalColour = saturate( lFinalColour );
    }
    #endif // D_FROM_BELOW

    if( lfSceneDepth >= 50000.0 )
    {
        lFinalColour.rgb = mix( lWaterFogColourFarVec3, lFinalColour.rgb, lFinalColour.a );
        lFinalColour.a = 1.0;
        //lfNewDepth = lfOriginalDepth;
    }

    //#ifdef D_FROM_BELOW
    //    lFinalColour = vec4( 0.0, 1.0, 0.0, 1.0 );//mix( lFinalColour.a, 0.0, lfHeightFade );
    //#else
    //    lFinalColour = vec4( 0.0, 0.0, 1.0, 1.0 );//mix( lFinalColour.a, 0.0, lfHeightFade );
    //#endif

    //if( lFragCoordsVec2.x > 0.5 )
    //{
    //    lFinalColour = GetDepthColour( min( lfNewDepth, lfOriginalDepth ) );
    //}
    //else
    //{
    //    lFinalColour = GetDepthColour( lfOriginalDepth );
    //}

    FRAGMENT_DEPTH  = LinearToReverseZDepth( lUniforms.mpPerFrame->gClipPlanesVec4, lfNewDepth );

    FRAGMENT_COLOUR = lFinalColour; 



}


#endif