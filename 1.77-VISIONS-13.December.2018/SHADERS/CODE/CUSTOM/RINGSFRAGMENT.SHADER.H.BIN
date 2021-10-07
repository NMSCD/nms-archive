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
#include "Common/CommonScattering.shader.h"
#include "Common/CommonLighting.shader.h"

#ifdef D_PLATFORM_ORBIS
#pragma argument(targetoccupancy_atallcosts=8)
#pragma argument(unrollallloops)
#endif

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
float
Bayer(
    uvec2 lPos )
{
    const mat4 bayer = mat4(
        vec4( 1, 9, 3, 11 ),
        vec4( 13, 5, 15, 7 ),
        vec4( 4, 12, 2, 10 ),
        vec4( 16, 8, 14, 6 )
        ) / 17.0;

    //vec2 positionMod = vec2( uvec2( tc * lUniforms.mpPerFrame->gFrameBufferSizeVec4.xy ) & 3 );

    uvec2 positionMod = uvec2( lPos & 3 );

    float rndoffset = bayer[ positionMod.x ][ positionMod.y ];
    //float rndoffset = float(positionMod.x) * 0.25;

    return rndoffset;
}

float
RayIntersection( 
    vec3      lvRayStart,
    vec3      lvRayDir,
    vec3      lSpherePos,
    float     lfSphereRadius,
    out float lResultT1,
    out float lResultT2 )
{
    vec3  m = lvRayStart - lSpherePos;
    float b = dot( m, lvRayDir );
    float c = dot( m, m ) - ( lfSphereRadius * lfSphereRadius );

    if( c > 0.0 && b > 0.0 )
    {
        return 0.0;
    }

    float discr = b * b - c;
    if( discr < 0.0 )
    {
        return 0.0;
    }

    float lfSqrt = sqrt( discr );
    lResultT1 = -b - lfSqrt;
    lResultT2 = -b + lfSqrt;

    lResultT1 = max( 0.0, lResultT1 );
    lResultT2 = max( 0.0, lResultT2 );

    return 1.0;
}


vec4 
GetRingColour(
    CustomPerMaterialUniforms lPerMaterialUniforms,
    float lfCurrentRadius,
    float lfWorldDistance,
    vec3  lUp,
    vec3  lToCamera )
{
    vec4 lRingColour         = vec4( 1.0, 1.0, 1.0, 1.0 );

    float lfDistance         = lfCurrentRadius * 0.000001 + lPerMaterialUniforms.gRingParamsVec4.a + 100.0;
    float lfRingStrength     = 0.0;

    // large rings 1
    float lfFarDistance      = lfDistance * lPerMaterialUniforms.gRingParamsVec4.r;
    lfRingStrength           = sin( lfFarDistance * 11.0 + cos( lfFarDistance * 13.0 ) + sin( lfFarDistance * 31.0 ) ) * 0.5 + 0.5;
    lRingColour.rgb          = lPerMaterialUniforms.gRingColour1Vec4.rgb * lfRingStrength;
    lRingColour.a            = 0.5 * smoothstep( 0.1, 0.2, lfRingStrength );

    // large rings 2
    float lfFarDistance2     = lfDistance * lPerMaterialUniforms.gRingParamsVec4.g + 20.0;
    lfRingStrength           = sin( lfFarDistance2 * 17.0 + cos( lfFarDistance2 * 7.0 ) + sin( lfFarDistance2 * 43.0 ) ) * 0.5 + 0.5;
    lRingColour.rgb         += lPerMaterialUniforms.gRingColour2Vec4.rgb * lfRingStrength;
    lRingColour.a           += 0.6 * smoothstep( 0.1, 0.3, lfRingStrength );

    // mid rings
    float lfMidDistance      = lfDistance * lPerMaterialUniforms.gRingColour1Vec4.a;
    float lfMidScaleNoise    = sin( lfMidDistance * 19.0 + cos( lfMidDistance * 17.0 ) ) * sin( lfMidDistance * 37.0 + sin( lfMidDistance * 23.0 ) ) * 0.5 + 0.5;
    float lfMidScaleFade     = saturate( ( lfWorldDistance - 200000.0 ) / 300000.0 );

    float lfDot              = abs( dot( normalize( lUp ), normalize( lToCamera ) ) );
    lfDot                    = saturate( ( lfDot - 0.3 ) / 0.7 );
    float lfAngleFade        = saturate( ( lfWorldDistance - 5000.0 ) / 5000.0 );
    lfAngleFade              = (1.0 - lfDot) * lfAngleFade;
    lfMidScaleNoise          = mix( lfMidScaleNoise, lfMidScaleNoise * 0.25 + 0.25, max( lfAngleFade, lfMidScaleFade ) );

    lRingColour.rgb         *= lfMidScaleNoise + 0.5;
    lRingColour.a           *= lfMidScaleNoise + lPerMaterialUniforms.gRingParamsVec4.b;

    // small rings
    float lfSmallDistance    = lfDistance * lPerMaterialUniforms.gRingColour2Vec4.a;
    float lfSmallScaleNoise  = sin( lfSmallDistance * 23.0 + cos( lfSmallDistance * 13.0 ) ) * sin( lfSmallDistance * 31.0 + cos( lfSmallDistance * 19.0 ) ) * 0.3;
    float lfSmallScaleFade   = saturate( ( lfWorldDistance - 50000.0 ) / 100000.0 );
    lfSmallScaleNoise        = mix( lfSmallScaleNoise, 0.0, max( lfSmallScaleFade, lfAngleFade ) );

    lRingColour.rgb         +=  lfSmallScaleNoise;
    lRingColour.a            = mix( 0.75, lRingColour.a, lfSmallScaleFade );

    return lRingColour;
}

float
GetRingShadow(
    vec3  lPlanetRelative,
    vec3  lSunPosition,
    float lfPlanetRadius )
{
    float lfShadow       = 0.95;

    float b = dot( lPlanetRelative, lSunPosition );
    float c = dot( lPlanetRelative, lPlanetRelative ) - ( lfPlanetRadius * lfPlanetRadius );

    if( c > 0.0f && b > 0.0f )
    {
        lfShadow *= 0.0;
    }

    float lfDiscr = b * b - c;
    if( lfDiscr < 0.0f )
    {
        lfShadow *= 0.0;
    }

    return 1.0 - lfShadow;
}

float 
GetRingDensity(
    float lfOuterRadius,
    float lfInnerRadius,
    float lfRingDistance,
    float lfHeight,
    float lfThickness)
{
    float lfFadeDistance =  ( lfOuterRadius - lfInnerRadius ) * 0.1;
    float lfRingBlend    = 1.0 - saturate( lfRingDistance / ( ( lfOuterRadius - lfInnerRadius ) * 0.5 ) );
    float lfDensity      = 1.0 - saturate( abs( lfHeight ) / lfThickness );

    lfDensity           *= lfDensity;
    lfDensity           *= lfRingBlend; 

    return lfDensity;
}

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

    vec4 lFragmentColourVec4 = vec4( 1.0, 1.0, 1.0, 1.0 );

    float lfInnerRadius     = lUniforms.mpCustomPerMaterial->gRingParams2Vec4.x;
    float lfOuterRadius     = lUniforms.mpCustomPerMaterial->gRingParams2Vec4.y;
    float lfThickness       = lUniforms.mpCustomPerMaterial->gRingParams2Vec4.z;
    float lfHeightFade      = lUniforms.mpCustomPerMaterial->gRingParams2Vec4.w;
    vec3  lPlanetPos        = lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz;
    float lfPlanetRadius    = lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w;
    vec3  lViewPos          = lUniforms.mpPerFrame->gViewPositionVec3;
    vec3  lSunPosition      = normalize( lUniforms.mpCustomPerMaterial->gSunPositionVec4.xyz );

    vec3  lUp             = normalize( MAT4_GET_COLUMN( lUniforms.mpCommonPerMesh->gWorldMat4, 1 ).xyz );
    vec3  lWorldPosition  = IN( mWorldPositionVec4 ).xyz;
    float lfWorldRadius   = 0.0;

    vec3  lRayStart   = lViewPos;
    vec3  lRayDir    = normalize( lWorldPosition - lViewPos );
    float lfDenom    = dot( lUp, lRayDir );

    // Mirror so the ring plane is double sided
    if( lfDenom < 0.0 )
    {
        lUp     = -lUp;
        lfDenom = -lfDenom;
    }

    if( lfDenom == 0.0 )
    {
        discard;
    }
    
    // Height of the camera above the plane
    float lfHeight      = dot( lRayStart - lPlanetPos, lUp );
    float lfDiskRayDist = 0.0;
    {
        // Calculate ntersection of ray with the highest point on the disk
        vec3  lToPoint = lRayStart - ( lPlanetPos - lUp * lfThickness );
        float lfDist1   = -dot( lUp, lToPoint ) / lfDenom;

        if( lfDist1 > 0.0 )
        {
            vec3 lIntersectionPoint = lToPoint + lfDist1 * lRayDir + ( lPlanetPos - lUp * lfThickness );
            lfDiskRayDist               = length( lIntersectionPoint - lRayStart );
        }

        if( abs( lfHeight ) < lfThickness )
        {
            // If we're within the disk, start at the camera
            lfDiskRayDist = 0.0;

            float lfCamDist = length( lViewPos - lPlanetPos );
            lfDiskRayDist   = max( 0.0, lfInnerRadius - lfCamDist );
        }
        else 
        {
            // If we're not inside and the intersection would be behind us
            if( lfDist1 <= 0.0 )
            {
                discard;
            }

            // Calculate intersection with the disk center plane
            lToPoint      = lRayStart - ( lPlanetPos + lUp * lfThickness );
            float lfDist2 = -dot( lUp, lToPoint ) / lfDenom;

            if( lfDist2 > 0.0 )
            {
                // If both intersection points are outside the ring we can discard
                float lfLength1      = length( lToPoint + lfDist1 * lRayDir );
                float lfLength2      = length( lToPoint + lfDist2 * lRayDir );

                if( ( lfLength1 > lfOuterRadius && lfLength2 > lfOuterRadius ) ||
                    ( lfLength1 < lfInnerRadius && lfLength2 < lfInnerRadius ) )
                {
                    discard;
                }
            }
            else
            {
                // Plane intersection is behind us, this should never actually happen
                discard;
            }
        }

        // Move the ray start forward to the top of the disk
        lRayStart = lRayStart + lRayDir * lfDiskRayDist;
    }

    // Read the scene depth
    vec2  lCoords     = ( IN( mScreenSpacePositionVec4 ).xy / IN( mScreenSpacePositionVec4 ).w ) * 0.5 + vec2( 0.5, 0.5 );
    lCoords           = SCREENSPACE_AS_RENDERTARGET_UVS( lCoords );
    float lfSceneDist = FastDenormaliseDepth( lUniforms.mpPerFrame->gClipPlanesVec4, DecodeDepthFromColour( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gBufferMap ), lCoords ) ) );
    float lfCos       = dot( -normalize( MAT4_GET_COLUMN( lUniforms.mpPerFrame->gCameraMat4, 2 ) ), lRayDir );
    lfCos             = max( lfCos, 0.00001 );
    lfSceneDist       = lfSceneDist / lfCos;

    int   liSteps             = 20;
    float lfStep              = lfThickness * 0.75;
    float lfT                 = 0.0;

    float lfColourFade        = mix( 1.8,  1.0, lfHeightFade );
    float lfAlphaFade         = mix( 1.0,  5.0, lfHeightFade );
    float lfColourMin         = mix( 0.25, 0.0, lfHeightFade );

    float lfPlanetDistance    = length( lViewPos - lPlanetPos );
    float lfMinFade           = lfPlanetRadius + lfOuterRadius * 1.2;
    float lfMaxFade           = lfPlanetRadius + lfOuterRadius * 1.4;
    float lfBlend             = saturate( ( lfPlanetDistance - lfMinFade ) / ( lfMaxFade - lfMinFade ) );
    lfBlend                   = max( lfBlend,  1.0 - lfHeightFade );

    float lfNearTotalRingDensity  = 0.0;
    vec4  lNearTotalRingColour    = vec4( 0.0, 0.0, 0.0, 0.0 );
    float lfNearTotalAlpha        = 0.0;
    float lfFarTotalRingDensity   = 0.0;
    vec4  lFarTotalRingColour     = vec4( 0.0, 0.0, 0.0, 0.0 );
    float lfFarTotalAlpha         = 0.0;

    // Just do one step when far from the rings
    if( lfBlend > 0.001 )
    {
        vec3  lToPoint    = lViewPos - lPlanetPos;
        float lfDist      = -dot( lUp, lToPoint ) / lfDenom;
        vec3  lCurrentPos = lRayStart + lRayDir * lfT;

        if( lfDist > 0.0 )
        {
            lCurrentPos = lToPoint + lfDist * lRayDir + lPlanetPos;
        }

        vec3  lPlanetRelative = lCurrentPos - lPlanetPos;
        lfHeight              = dot( lCurrentPos - lPlanetPos, lUp );

        vec3  lProjectedPos   = lCurrentPos - lUp * lfHeight;
        float lfCurrentRadius = length( lProjectedPos - lPlanetPos );

        float lfMidRing       = ( lfOuterRadius - lfInnerRadius ) * 0.5 + lfInnerRadius;
        vec3  lRingPos        = normalize( lProjectedPos - lPlanetPos ) * lfMidRing + lPlanetPos;
        float lfRingDistance  = length( lCurrentPos - lRingPos );

        vec3  lToCamera       = lViewPos - lCurrentPos;
        float lfWorldDistance = length( lToCamera );

        vec4 lRingColour = vec4( 0.0, 0.0, 0.0, 0.0 );

        lRingColour = GetRingColour(
            DEREF_PTR( lUniforms.mpCustomPerMaterial ),
            lfCurrentRadius,
            lfWorldDistance,
            lUp,
            lToCamera );

        float lfShadow = GetRingShadow(
            lPlanetRelative,
            lSunPosition,
            lfPlanetRadius );
        lfShadow = mix( max( lfShadow, 0.9 ), lfShadow,  lfHeightFade );

        lRingColour.rgb *= lfShadow;

        float lfDensity = GetRingDensity(
            lfOuterRadius,
            lfInnerRadius,
            lfRingDistance,
            lfHeight,
            lfThickness );
        lfDensity *= lRingColour.a;

        // Don't add density once we get behind another object in the scene
        if( lfT + lfDiskRayDist <= lfSceneDist )
        {
            lfFarTotalAlpha    += lfDensity;
        }

        lFarTotalRingColour    = saturate( lRingColour ) * lfDensity;
        lfFarTotalRingDensity  = lfDensity;
        lfFarTotalAlpha        *= 2.0;
    }

    if( lfBlend < 0.999 )
    {
        for( int i = 0; i < liSteps; i++ )
        {
            vec3  lCurrentPos     = lRayStart + lRayDir * lfT;
            vec3  lPlanetRelative = lCurrentPos - lPlanetPos;
            lfHeight              = dot( lCurrentPos - lPlanetPos, lUp );

            vec3  lProjectedPos   = lCurrentPos - lUp * lfHeight;

            float lfMidRing       = ( lfOuterRadius - lfInnerRadius ) * 0.5 + lfInnerRadius;
            vec3  lRingPos        = normalize( lProjectedPos - lPlanetPos ) * lfMidRing + lPlanetPos;
            float lfRingDistance  = length( lCurrentPos - lRingPos );

            float lfDensity = GetRingDensity(
                lfOuterRadius,
                lfInnerRadius,
                lfRingDistance,
                lfHeight,
                lfThickness );

            if (lfDensity > 0.0)
            {
                float lfCurrentRadius = length(lProjectedPos - lPlanetPos);

                vec3  lToCamera       = lViewPos - lCurrentPos;
                float lfWorldDistance = length( lToCamera );

                vec4 lRingColour = vec4( 0.0, 0.0, 0.0, 0.0 );

                lRingColour = GetRingColour(
                    DEREF_PTR( lUniforms.mpCustomPerMaterial ),
                    lfCurrentRadius,
                    lfWorldDistance,
                    lUp,
                    lToCamera );
                lfDensity *= lRingColour.a;

                float lfShadow = GetRingShadow(
                    lPlanetRelative,
                    lSunPosition,
                    lfPlanetRadius );
                lfShadow = mix( lfShadow, max( lfShadow, 0.9 ), lfHeightFade );

                lRingColour.rgb *= vec3( lfShadow, lfShadow, lfShadow);

                // Don't add density once we get behind another object in the scene
                if( lfT + lfDiskRayDist <= lfSceneDist )
                {
                    lfNearTotalAlpha += lfDensity;
                }

                lNearTotalRingColour += saturate(lRingColour) * lfDensity;
                lfNearTotalRingDensity += lfDensity;
            }

            lfT += lfStep;
        }
    }

    if( lfNearTotalRingDensity > 0.0 || lfFarTotalRingDensity > 0.0 )
    {
        vec4  lTotalRingColour   = mix( lNearTotalRingColour,   lFarTotalRingColour,   lfBlend );
        float lfTotalRingDensity = mix( lfNearTotalRingDensity, lfFarTotalRingDensity, lfBlend );
        float lfTotalAlpha       = mix( lfNearTotalAlpha,       lfFarTotalAlpha,       lfBlend );

        lFragmentColourVec4.rgb  = max( vec3( lfColourMin, lfColourMin, lfColourMin), lfColourFade * lTotalRingColour.rgb / lfTotalRingDensity );
        lFragmentColourVec4.a    = saturate( lfTotalAlpha * lfAlphaFade );
        lFragmentColourVec4.a    = min( lFragmentColourVec4.a, 0.90 );
    }
    else
    {
        discard;
    }

    vec3  lToPlanet  = ( lPlanetPos - lViewPos );
    vec3  lToPoint   = normalize( lWorldPosition - lViewPos );
    vec3  lToPointu   = ( lRayStart - lViewPos );
    float lfToPlanet = length( lToPlanet );
    lToPlanet  = normalize( lToPlanet );

    float lfDot      = dot( lToPoint, -lToPlanet );

    vec3  lToEdge    = normalize( cross( lToPlanet, lUp ) );
    vec3  lInnerEdge = normalize( lPlanetPos + ( lToEdge * lfPlanetRadius ) - lViewPos );
    vec3  lOuterEdge = normalize( lPlanetPos + ( lToEdge * lfPlanetRadius * 1.08 ) - lViewPos );
    

    float lfInnerDot = dot( lInnerEdge, -lToPlanet );
    float lfOuterDot = dot( lOuterEdge, -lToPlanet );

    float lfValue = ( lfDot - lfInnerDot ) / ( lfOuterDot - lfInnerDot );
    lfValue = saturate( lfValue );

    float lfTestLength = dot( lToPointu, lToPlanet );
    if( lfTestLength > lfToPlanet )
    {
        lFragmentColourVec4 *= lfValue;
    }

    FRAGMENT_COLOUR = ( lFragmentColourVec4 );

    //vec4 lScreenSpacePos = IN( mScreenSpacePositionVec4 );
    //lScreenSpacePos.w = lfDiskRayDist;
}

