////////////////////////////////////////////////////////////////////////////////
///
///     @file       CloudFragment.h
///     @author     User
///     @date       
///
///     @brief      CloudFragment
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
//      Compilation defines 

#define D_USE_NOISETEXTURE
#define D_NORMALISED_NOISE

//-----------------------------------------------------------------------------
//      Include files

#include "Common/Defines.shader.h"
#include "Common/CommonUniforms.shader.h"
#include "Common/Common.shader.h"
#include "Common/CommonDepth.shader.h"

#include "Custom/CloudCommon.h"


#include "Common/CommonPlanet.shader.h"

#ifdef D_PLATFORM_ORBIS
#pragma argument(unrollallloops)
#endif

//-----------------------------------------------------------------------------
//      Global Data




//-----------------------------------------------------------------------------
//      Typedefs and Classes 

#define PI 3.141592653589793

//-----------------------------------------------------------------------------
//      Functions


//float kfHackDebugScreenSide = 0.0;


vec4
DebugColour(
    float lfValue,
    float lfMin,
    float lfMax )
{
    float lfUnitValue;
    //float lfAlpha;
    //vec4  lColour;

    lfUnitValue = 1.0 - saturate( (lfValue - lfMin) / (lfMax - lfMin) );
    //lfAlpha     = 1.0;


    vec3 r = lfUnitValue * 2.1 - vec3(1.8, 1.14, 0.3);
    return vec4( 1.0 - r * r, 1.0 );

    /*
    if ( lfUnitValue < 0.0 )
    {
        lColour = vec4( 0.0, 0.0, 0.0, 1.0 );
    }
    else
    if ( lfUnitValue < 0.125 )
    {
        lfAlpha = (lfUnitValue - 0.0) / 0.125;
        lColour = vec4( 1.0, 0.0, 0.0, 1.0 );
    }
    else
    if ( lfUnitValue < 0.25 )
    {
        lfAlpha = (lfUnitValue - 0.125) / 0.125;
        lColour = vec4( 1.0, 0.5, 0.0, 1.0 );
    }
    else
    if ( lfUnitValue < 0.375 )
    {
        lfAlpha = (lfUnitValue - 0.25) / 0.125;
        lColour = vec4( 1.0, 1.0, 0.0, 1.0 );
    }
    else
    if ( lfUnitValue < 0.5 )
    {
        lfAlpha = (lfUnitValue - 0.375) / 0.125;
        lColour = vec4( 0.0, 0.5, 0.0, 1.0);
    }
    else
    if ( lfUnitValue < 0.625 )
    {
        lfAlpha = (lfUnitValue - 0.5) / 0.125;
        lColour = vec4( 0.5, 1.0, 0.5, 1.0);
    }
    else
    if ( lfUnitValue < 0.75 )
    {
        lfAlpha = (lfUnitValue - 0.625) / 0.125;
        lColour = vec4( 0.0, 1.0, 1.0, 1.0);
    }
    else
    if ( lfUnitValue < 0.875 )
    {
        lfAlpha = (lfUnitValue - 0.75) / 0.125;
        lColour = vec4( 0.0, 0.0, 1.0, 1.0);
    }
    else
    if ( lfUnitValue <= 1.0 )
    {
        lfAlpha = (lfUnitValue - 0.875) / 0.125;
        lColour = vec4( 1.0, 0.0, 1.0, 1.0);
    }
    else
    {
        lColour = vec4( 1.0, 1.0, 1.0, 1.0);
    }

    lColour.xyz *= (lfAlpha * 0.5) + 0.5;
    return lColour;
    */
}



vec4
GetTriPlanarMap(
    in vec3  lBlendWeights,
    in vec3  lWorldPositionVec3,
    SAMPLER2DARG( lTexture ) )
{
    vec2  lCoord1Vec2 = lWorldPositionVec3.yz;
    vec2  lCoord2Vec2 = lWorldPositionVec3.zx;
    vec2  lCoord3Vec2 = lWorldPositionVec3.xy;

    vec4 lColour1Vec4 = texture2DLod( lTexture, lCoord1Vec2, 0.0 );
    vec4 lColour2Vec4 = texture2DLod( lTexture, lCoord2Vec2, 0.0 );
    vec4 lColour3Vec4 = texture2DLod( lTexture, lCoord3Vec2, 0.0 );

	return (lColour1Vec4 * lBlendWeights.x + lColour2Vec4 * lBlendWeights.y + lColour3Vec4 * lBlendWeights.z);
}


/*
vec4
GetTriPlanarMapCheap(
    in vec3  lBlendWeights,
    in vec3  lWorldPositionVec3,
    SAMPLER2DARG( lTexture ) )
{
    vec2  lCoordVec2 = vec2( 0.0, 0.0 );


    if ( lBlendWeights.x > 0.1 )
    {
        lCoordVec2 = lWorldPositionVec3.yz;
    }
    else
    if ( lBlendWeights.y > 0.1 )
    {
        lCoordVec2 = lWorldPositionVec3.zx;
    }
    else
    if ( lBlendWeights.z > 0.1 )
    {
        lCoordVec2 = lWorldPositionVec3.xy;
    }
    else
    {
        vec2  lCoord1Vec2 = lWorldPositionVec3.yz;
        vec2  lCoord2Vec2 = lWorldPositionVec3.zx;
        vec2  lCoord3Vec2 = lWorldPositionVec3.xy;

        vec4 lColour1Vec4 = texture2DLod( lTexture, lCoord1Vec2, 0.0 );
        vec4 lColour2Vec4 = texture2DLod( lTexture, lCoord2Vec2, 0.0 );
        vec4 lColour3Vec4 = texture2DLod( lTexture, lCoord3Vec2, 0.0 );

        return (lColour1Vec4 * lBlendWeights.x + lColour2Vec4 * lBlendWeights.y + lColour3Vec4 * lBlendWeights.z);
    }

    return texture2DLod(lTexture, lCoordVec2, 0.0);
    //return vec4(lWorldPositionVec3,0.0);
    //return vec4( 0.0, 0.0, 0.0, 0.0);
}
*/


vec3
GetWindDirection(
    in vec3  lBlendWeights,
    in vec2  lWindVec2 )
{	
    vec2  lWindDirectionVec3;

	vec3  lWindDirectionXVec3 = vec3( 0.5, lWindVec2.y, lWindVec2.x );
	vec3  lWindDirectionYVec3 = vec3( lWindVec2.y, 0.5, lWindVec2.x );
	vec3  lWindDirectionZVec3 = vec3( lWindVec2.x, lWindVec2.y, 0.5 );

	return (lWindDirectionXVec3 * lBlendWeights.x + lWindDirectionYVec3 * lBlendWeights.y + lWindDirectionZVec3 * lBlendWeights.z);
}


vec3 
InternalRaySphereIntersect( 
    in float sphereRadius, 
    in vec3  origin, 
    in vec3  rayDirection )
{
    float a0 = sphereRadius * sphereRadius - dot( origin, origin );
    float a1 = dot( origin, rayDirection );
    float result = sqrt( a1 * a1 + a0 ) - a1;

    return origin + rayDirection * result;
}

STATIC_CONST uvec2 inverseBayerArray[16] = {
    uvec2( 0, 0 ),    // 1
    uvec2( 2, 2 ),    // 2
    uvec2( 0, 2 ),    // 3
    uvec2( 2, 0 ),    // 4
    uvec2( 1, 1 ),    // 5
    uvec2( 3, 3 ),    // 6
    uvec2( 1, 3 ),    // 7
    uvec2( 3, 1 ),    // 8
    uvec2( 0, 1 ),    // 9
    uvec2( 2, 3 ),    // 10
    uvec2( 0, 3 ),    // 11
    uvec2( 2, 1 ),    // 12
    uvec2( 1, 0 ),    // 13
    uvec2( 3, 2 ),    // 14
    uvec2( 1, 2 ),    // 15
    uvec2( 3, 0 ) };  // 16

uvec2
InverseBayer(
    uint luFrameIndex )
{
    //vec2 lOffset = inverseBayerArray[ uint( lfFrameIndex ) ] * 0.25;
    uvec2 lOffset = inverseBayerArray[ luFrameIndex ];

    return lOffset;
}


STATIC_CONST mat4 bayer = mat4(
    vec4(1, 9, 3, 11),
    vec4(13, 5, 15, 7),
    vec4(4, 12, 2, 10),
    vec4(16, 8, 14, 6));

float
Bayer(
    uvec2 lPos )
{
    uvec2 positionMod = uvec2( lPos & 3 );

    float rndoffset = bayer[positionMod.x][positionMod.y];

    return rndoffset;
}

#define M_CALCULATE_HORIZON_DISTANCE( innerRadius, outerRadius )        sqrt( ( outerRadius * outerRadius ) - ( innerRadius * innerRadius ) )
#define M_CALCULATE_PLANET_RADIUS( atmosphereHeight, horizonDistance )  ((atmosphereHeight * atmosphereHeight + horizonDistance * horizonDistance) / (2.0 * atmosphereHeight)) - atmosphereHeight;

#define vec4_COVERAGE( f)	f.r
#define vec4_RAIN( f)		f.g
#define vec4_TYPE( f)		f.b

float
GetRayIntersectionPoint(
    in vec3  lStartPointVec3,
    in vec3  lEndPointVec3,
    in float lfRadius,
    out vec3 lOutNearPointVec3,
    out vec3 lOutFarPointVec3 )
{
    lOutNearPointVec3 = lStartPointVec3;
    lOutFarPointVec3  = lEndPointVec3;

    float lfLength           = length( lStartPointVec3 - lEndPointVec3 );
    vec3  lNormalisedRayVec3 = normalize( lStartPointVec3 - lEndPointVec3 );
    float lfB                = 2.0 * dot( lStartPointVec3, lNormalisedRayVec3 );
    float lfC                = dot( lStartPointVec3, lStartPointVec3 ) - ( lfRadius * lfRadius );
    float lfDet              = lfB * lfB - 4.0 * lfC;

    if ( lfDet >= 0.0 )
    {
        float lfSqrt = sqrt( lfDet );
        float lfNear = 0.5 * ( -lfB + lfSqrt );
        float lfFar  = 0.5 * ( -lfB - lfSqrt );

        lOutFarPointVec3 = lStartPointVec3 + lNormalisedRayVec3 * lfFar;
        lOutNearPointVec3 = lStartPointVec3 + lNormalisedRayVec3 * lfNear;

        lfDet = 1.0;

        return lfDet;
    }

    return 0.0;
}

//#define D_CLOUD_RENDER



vec3
GetScreenSpaceViewDir(
    in vec2  lFragCoordsVec2,
    in mat4  lInverseProjectionMatrix,
    in mat4  lInverseViewMatrix )
{
    vec4 lPositionVec4;
    lPositionVec4.x = lFragCoordsVec2.x * 2.0 - 1.0;

#ifndef D_PLATFORM_OPENGL   
    lPositionVec4.y = ( 1.0f-lFragCoordsVec2.y ) * 2.0 - 1.0;
#else
    lPositionVec4.y = lFragCoordsVec2.y * 2.0 - 1.0;
#endif

    lPositionVec4.z = 0.0;
    lPositionVec4.w = 1.0;

    // Inverse projection
    lPositionVec4        = MUL( lInverseProjectionMatrix, lPositionVec4 );
    lPositionVec4        = lPositionVec4 / lPositionVec4.w;

    // Inverse view
    mat4 lViewMat   = lInverseViewMatrix;
    MAT4_SET_POS( lViewMat, vec4( 0.0, 0.0, 0.0, 1.0 ) );
    lPositionVec4   = MUL( lViewMat, lPositionVec4 );

    vec3 lViewVectorVec3 = normalize( lPositionVec4.xyz );

    return lViewVectorVec3;
}




struct cCloudProperties
{
    // Data
    int   miMaxIterations;
    float mfHorizonCoverageStart;
    float mfHorizonCoverageEnd;

    float mfLightScalar;
    float mfAmbientScalar;
    float mfSunRayLength;
    float mfConeRadius;

    float mfDensity;
    float mfAmbientDensity;
    float mfForwardScatteringG;
    float mfBackwardScatteringG;
    float mfDarkOutlineScalar;

    float mfAnimationScale;

    float mfBaseScaleScalar;
    float mfSampleScalar;
    float mfSampleThreshold;
    float mfCloudBottomFade;

    float mfDetailScale;
    float mfErosionEdgeSize;
    float mfCloudDistortion;
    float mfCloudDistortionScale;

    float mfRayMinimumY;
    float mfLODDistance;
    float mfHorizonFadeStartAlpha;
    float mfHorizonFadeScalar;	// Fades clouds on horizon, 1.0 -> 10.0 (1.0 = smooth fade, 10 = no fade)

    float mfHorizonDistance;
    float mfAtmosphereStartHeight;
	float mfAtmosphereEndHeight;
	float mfStratosphereHeight;

    // Calculated
    vec3  mLightDirectionVec3;
    vec3  mLightColourVec3;
    vec3  mCameraPositionVec3;
    vec3  mTriplanarBlendWeights;
    vec3  mWindDirection;
    //mat3  mCoverageRotMat3;
    float mfRayAboveHorizon;

    float mfCloudRatio;
    float mfRatioAbovePlanet;
    float mfAtmosphereThickness;
    float mfEarthRadius;
    float mfMaxDistance;
    float mfRayStepLengthMin;
    float mfRayStepLengthMax;
    float mfBaseScale;
    float mfCoverageScale;
};


float 
SmoothThreshold( 
    in float value, 
    in float threshold, 
    in float edgeSize )
{
    return smoothstep( threshold, threshold + edgeSize, value );
}

vec3 
SmoothThreshold( 
    in vec3  value, 
    in float threshold, 
    in float edgeSize )
{
    value.r = smoothstep( threshold, threshold + edgeSize, value.r );
    value.g = smoothstep( threshold, threshold + edgeSize, value.g );
    value.b = smoothstep( threshold, threshold + edgeSize, value.b );

    return value;
}

float 
MixNoise( 
    in float value, 
    in float noise, 
    in float a, 
    in float b, 
    in float height )
{
    float s = smoothstep( a, b, height );
    value += noise * s;
    //value *= lerp( 1.0, 0.5, s);

    return value;
}

float 
Lerp3( 
    in float v0, 
    in float v1, 
    in float v2, 
    in float a )
{
    return a < 0.5 ? mix( v0, v1, a * 2.0 ) : mix( v1, v2, ( a - 0.5 ) * 2.0 );
}

vec4 
Lerp3( 
    in vec4 v0, 
    in vec4 v1, 
    in vec4 v2, 
    in float a )
{
    return vec4( Lerp3( v0.x, v1.x, v2.x, a ),
        Lerp3( v0.y, v1.y, v2.y, a ),
        Lerp3( v0.z, v1.z, v2.z, a ),
        Lerp3( v0.w, v1.w, v2.w, a ) );
}

float 
NormalizedAtmosphereY( 
    in vec3  lRayVec3,
    in float lfEarthRadius,
    in float lfStartHeight,
    in float lfAtmosphereThickness )
{
    float y = length( lRayVec3 ) - (lfEarthRadius + lfStartHeight);
    return saturate(y / lfAtmosphereThickness);
}

float
LinearStep(
    float edge0,
    float edge1,
    float x )
{
    // Scale, and clamp x to 0..1 range
    x = saturate( ( x - edge0 ) / ( edge1 - edge0 ) );
    return x;
}

STATIC_CONST float kfBlahHackPower = 0.5;

float
PowStep(
    float edge0,
    float edge1,
    float x )
{
    // Scale, and clamp x to 0..1 range
    x = saturate( ( x - edge0 ) / ( edge1 - edge0 ) );
    return pow( x, kfBlahHackPower );
}

float
InversePowStep(
    float edge0,
    float edge1,
    float x )
{
    // Scale, and clamp x to 0..1 range
    x = saturate( ( x - edge0 ) / ( edge1 - edge0 ) );
    return 1.0 - pow( 1.0 - x, kfBlahHackPower );
}

float 
GradientStep( 
    in float a, 
    in vec4 gradient )
{
    return PowStep( gradient.x, gradient.y, a ) - InversePowStep( gradient.z, gradient.w, a );
}

#ifdef D_CLOUD_RENDER

#ifdef D_PLATFORM_ORBIS
#pragma argument(targetoccupancy_atallcosts=4)
#endif

//-----------------------------------------------------------------------------
///
///     Input
///
///     @brief  Input
///
//-----------------------------------------------------------------------------
DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2,                  TEXCOORD0 )

DECLARE_END


// Random on Unit Sphere
STATIC_CONST vec3   _Random0 = vec3( 0.4469, 0.5511, 0.7047 );
STATIC_CONST vec3   _Random1 = vec3( -0.0791, -0.0898, -0.9928 );
STATIC_CONST vec3   _Random2 = vec3( -0.0205, 0.8772, -0.4797 );
STATIC_CONST vec3   _Random3 = vec3( 0.4335, -0.0052, -0.9012 );
STATIC_CONST vec3   _Random4 = vec3( -0.8569, 0.0677, -0.5111 );
STATIC_CONST vec3   _Random5 = vec3( 0.8033, -0.5266, 0.2782 );

STATIC_CONST vec3 RandomUnitSphere[6] = { _Random0, _Random1, _Random2, _Random3, _Random4, _Random5 };


float
HenyeyGreensteinPhase(
	in float costh,
	in float g)
{
	return (1.0 - g * g) / (4.0 * PI * pow(1.0 + g * g - 2.0 * g * costh, 3.0 / 2.0));
}


float 
BeerTerm( 
    in float lfPrecipitation,
    in float lfDensity )
{
    return exp( -(lfDensity * lfPrecipitation) );
}

float 
PowderTerm( 
    in float lfDensityAtSample, 
    in float lfCosTheta,
    in float lfDarkOutlineScalar,
    in float lfDensityScalar )
{
    float lfPowder = 1.0 - exp( -lfDensityScalar * lfDensityAtSample * 2.0 );
    //lfPowder = saturate( lfPowder * lfDarkOutlineScalar * 2.0 );
    //return mix( 1.0, lfPowder, smoothstep( 0.5, -0.5, lfCosTheta ) );
    return lfPowder;
}

vec3
SampleAmbientLight( 
    in vec3  lBaseColour,
    in vec3  lTopColour,
    in float atmosphereY )
{
    return mix( lBaseColour, lTopColour, atmosphereY );
}

vec4 
SampleCoverage( 
    in CustomPerMeshUniforms lCustomPerMeshUniforms,
    in cCloudProperties      lCloudProperties, 
    in vec3                  lRayVec3,
    in bool                  lbHasHorizon )
{
    vec3 lNewRayVec3 = lRayVec3 * lCloudProperties.mfCoverageScale;

	vec4 lCoverageVec4 = GetTriPlanarMap( lCloudProperties.mTriplanarBlendWeights, lNewRayVec3, SAMPLER2DPARAM_LOCAL(lCustomPerMeshUniforms, gCoverage2D) );

    return lCoverageVec4;

    /*
    coverageB.b = saturate( smoothstep( _HorizonCoverageEnd, _HorizonCoverageStart, depth) * 2.0);
    if ( lbHasHorizon )
    {
        vec4 lCoverageBVec4 = vec4( 1.0, 0.0, 0.0, 0.0 );

        float lfAlpha = smoothstep( lCloudProperties.mfHorizonCoverageStart, lCloudProperties.mfHorizonCoverageEnd, lfDepth );

        lCoverageBVec4 = vec4(  smoothstep( lCloudProperties.mfHorizonCoverageStart, lCloudProperties.mfHorizonCoverageEnd, lfDepth ),
                                0.0,
                                smoothstep( lCloudProperties.mfHorizonCoverageEnd, lCloudProperties.mfHorizonCoverageStart + ( lCloudProperties.mfHorizonCoverageEnd - lCloudProperties.mfHorizonCoverageStart ) * 0.5, lfDepth ),
                                0.0 );
                            
        return mix( lCoverageVec4, lCoverageBVec4, lfAlpha );
    }
    else
    {
        return lCoverageVec4;
    }*/
}


// Utility function that maps a value from one range to another.
float 
Remap(
    float original_value, 
    float original_min, 
    float original_max, 
    float new_min, 
    float new_max )
{
    return new_min + (saturate((original_value - original_min) / (original_max - original_min)) * (new_max - new_min));
}

float
GetDensityHeightGradientForPoint(
    in CustomPerMeshUniforms lCustomPerMeshUniforms,
    float lfAtmosphereY,
    float  lfCloudType )
{
    vec4 lGradientVec4;

    lGradientVec4 = Lerp3(  lCustomPerMeshUniforms.gCloudHeightGradient1,
                            lCustomPerMeshUniforms.gCloudHeightGradient2,
                            lCustomPerMeshUniforms.gCloudHeightGradient3,
                            lfCloudType );

    return GradientStep( lfAtmosphereY, lGradientVec4 );
}

float
SampleCloud(
    in CustomPerMeshUniforms lCustomPerMeshUniforms,
    in cCloudProperties      lCloudProperties,
    in vec3                  lRayVec3,
    in float                 lfMipLevel,
    in int                   liQuality)
{
    // get height fraction  (be sure to create a cloud_min_max variable)
    float lfAtmosphereY = NormalizedAtmosphereY(lRayVec3, lCloudProperties.mfEarthRadius, lCloudProperties.mfAtmosphereStartHeight, lCloudProperties.mfAtmosphereThickness);

    vec3 wind_direction = GetWindDirection(lCloudProperties.mTriplanarBlendWeights, lCustomPerMeshUniforms.gWindOffset.xy);

    float cloud_speed = lCloudProperties.mfAnimationScale;

    // cloud_top offset - push the tops of the clouds along this wind direction by this many units.
    float cloud_top_offset = 500.0;
    // skew in wind direction
    lRayVec3 += lfAtmosphereY * wind_direction * cloud_top_offset;

    //animate clouds in wind direction and add a small upward bias to the wind direction
    lRayVec3 += wind_direction * cloud_speed;

    vec4 lWeatherDataVec4 = SampleCoverage( lCustomPerMeshUniforms,
                                            lCloudProperties,
                                            lRayVec3,
                                            false );

    // cloud coverage is stored in the weather_data’s red channel.
    float cloud_coverage = lWeatherDataVec4.r * lCloudProperties.mfCloudRatio;

    // apply anvil deformations
    cloud_coverage = pow(cloud_coverage, Remap(lfAtmosphereY, 0.7, 0.8, 1.0, mix(1.0, 0.5, lCloudProperties.mfCloudBottomFade)));

    cloud_coverage = smoothstep(0.25, 1.0, cloud_coverage);

    if ( cloud_coverage <= 0.0 || liQuality == 0)
    {
        return max(0.0, cloud_coverage);
    }


    //vec3 lLowFreqNoiseCoordsVec3 = vec3( lRayVec3 * lCloudProperties.mfBaseScale + (lCustomPerMeshUniforms.gBaseOffset.xyz * lCloudProperties.mfAnimationScale )); // * gfTime
    vec3 lLowFreqNoiseCoordsVec3 = vec3(lRayVec3 * lCloudProperties.mfBaseScale); // * gfTime

                                                                                  // wind settings

                                                                                  // read the low frequency Perlin-Worley and Worley noises
    vec4 low_frequency_noises = texture3DLod(SAMPLER_GETLOCAL(lCustomPerMeshUniforms, gPerlin3D), lLowFreqNoiseCoordsVec3, lfMipLevel);

    // build an fBm out of  the low frequency Worley noises that can be used to add detail to the Low frequency Perlin-Worley noise
    float low_freq_fBm = (low_frequency_noises.g * 0.625) + (low_frequency_noises.b * 0.25) + (low_frequency_noises.a * 0.125);

    // define the base cloud shape by dilating it with the low frequency fBm made of Worley noise.
    float base_cloud = Remap(low_frequency_noises.r, -(1.0 - low_freq_fBm), 1.0, 0.0, 1.0);

    // Get the density-height gradient using the density-height function (not included)
    //float density_height_gradient = GetDensityHeightGradientForPoint( lCustomPerMeshUniforms, lfAtmosphereY, kfHackDebugScreenSide > 0.5 ? lCloudProperties.mfLODDistance : lWeatherDataVec4.b );
    float density_height_gradient = GetDensityHeightGradientForPoint(lCustomPerMeshUniforms, lfAtmosphereY, lWeatherDataVec4.b);

    // apply the height function to the base cloud shape
    base_cloud *= density_height_gradient;

    if (base_cloud <= 0.0)
    {
        return 0.0;
    }

    //Use remapper to apply cloud coverage attribute
    float base_cloud_with_coverage = Remap(base_cloud, 1.0 - cloud_coverage, 1.0, 0.0, 1.0);

    //Multiply result by cloud coverage so that smaller clouds are lighter and more aesthetically pleasing.
    //base_cloud_with_coverage *= cloud_coverage;
    //base_cloud_with_coverage *= 1.0 - cloud_coverage;

    //define final cloud value
    //float final_cloud = mix( base_cloud_with_coverage, smoothstep( 0.4, 0.5, cloud_coverage ), lCloudProperties.mfSampleThreshold );
    float final_cloud = base_cloud_with_coverage;

    // only do detail work if we are taking expensive samples!
    if (liQuality == 2)
    {
        vec3 lDistortionCoordsVec3 = lRayVec3 * lCloudProperties.mfBaseScale * lCloudProperties.mfCloudDistortionScale;
        // add some turbulence to bottoms of clouds using curl noise.  Ramp the effect down over height and scale it by some value (200 in this example)
        //vec2 curl_noise = tex2Dlod(Cloud2DNoiseTexture,  Cloud2DNoiseSampler,  vec4 ( vec2(lRayVec3.x, lRayVec3.y), 0.0, 1.0 ).rg;
        vec3 curl = GetTriPlanarMap(lCloudProperties.mTriplanarBlendWeights, lDistortionCoordsVec3, SAMPLER2DPARAM_LOCAL(lCustomPerMeshUniforms, gCurl2D)).xyz;
        //lRayVec3.xy += curl_noise.rg * (1.0 - lfAtmosphereY) * 200.0;
        lRayVec3 += curl * (1.0 - lfAtmosphereY) * lCloudProperties.mfCloudDistortion;
        //lRayVec3 += curl * (1.0 - lfAtmosphereY) * 200.0;

        // sample high-frequency noises
        vec3 lHighFreqNoiseCoordsVec3 = (lRayVec3 * lCloudProperties.mfBaseScale * lCloudProperties.mfDetailScale); //+(lCustomPerMeshUniforms.gDetailOffset.xyz * lCloudProperties.mfAnimationScale); // * gfTime
        vec3 high_frequency_noises = texture3DLod(SAMPLER_GETLOCAL(lCustomPerMeshUniforms, gDetail3D), lHighFreqNoiseCoordsVec3, lfMipLevel).xyz;

        // build High frequency Worley noise fBm
        float high_freq_fBm = (high_frequency_noises.r * 0.625) + (high_frequency_noises.g * 0.25) + (high_frequency_noises.b * 0.125);

        // get the lfAtmosphereY for use with blending noise types over height
        lfAtmosphereY = NormalizedAtmosphereY(lRayVec3, lCloudProperties.mfEarthRadius, lCloudProperties.mfAtmosphereStartHeight, lCloudProperties.mfAtmosphereThickness);

        // transition from wispy shapes to billowy shapes over height
        float high_freq_noise_modifier = mix(high_freq_fBm, 1.0 - high_freq_fBm, saturate(lfAtmosphereY * 10.0));
        //float high_freq_noise_modifier = 1.0 - high_freq_fBm;
        //float high_freq_noise_modifier = high_freq_fBm;

        // erode the base cloud shape with the distorted high frequency Worley noises.
        final_cloud = Remap(final_cloud, high_freq_noise_modifier * lCloudProperties.mfErosionEdgeSize, 1.0, 0.0, 1.0);
    }

    //final_cloud = step(0.1, final_cloud);
    return final_cloud * lCloudProperties.mfSampleScalar;
    //return step( 0.5, cloud_coverage );
}


float
SampleCloudLighting(
    in CustomPerMeshUniforms lCustomPerMeshUniforms,
    in cCloudProperties      lCloudProperties,
    in vec3                  lRayVec3,
    //in vec4                  lWeatherDataVec4,
    in float                 lfMipLevel,
    in int                   liQuality )
{
    // get height fraction  (be sure to create a cloud_min_max variable)
    float lfAtmosphereY = NormalizedAtmosphereY(lRayVec3, lCloudProperties.mfEarthRadius, lCloudProperties.mfAtmosphereStartHeight, lCloudProperties.mfAtmosphereThickness);

    float cloud_speed = lCloudProperties.mfAnimationScale;

    // cloud_top offset - push the tops of the clouds along this wind direction by this many units.
    float cloud_top_offset = 500.0;
    // skew in wind direction
    lRayVec3 += lfAtmosphereY * lCloudProperties.mWindDirection * cloud_top_offset;

    //animate clouds in wind direction and add a small upward bias to the wind direction
    lRayVec3 += lCloudProperties.mWindDirection * cloud_speed;

    
    vec4 lWeatherDataVec4 = SampleCoverage(lCustomPerMeshUniforms,
        lCloudProperties,
        lRayVec3,
        false);

    // cloud coverage is stored in the weather_data’s red channel.
    float cloud_coverage = lWeatherDataVec4.r * lCloudProperties.mfCloudRatio;

    // apply anvil deformations
    cloud_coverage = pow(cloud_coverage, Remap(lfAtmosphereY, 0.7, 0.8, 1.0, mix(1.0, 0.5, lCloudProperties.mfCloudBottomFade)));

    cloud_coverage = smoothstep(0.25, 1.0, cloud_coverage);

    vec3 lLowFreqNoiseCoordsVec3 = vec3(lRayVec3 * lCloudProperties.mfBaseScale); // * gfTime

    // read the low frequency Perlin-Worley and Worley noises
    vec4 low_frequency_noises = texture3DLod(SAMPLER_GETLOCAL(lCustomPerMeshUniforms, gPerlin3D), lLowFreqNoiseCoordsVec3, lfMipLevel);

    // build an fBm out of  the low frequency Worley noises that can be used to add detail to the Low frequency Perlin-Worley noise
    float low_freq_fBm = (low_frequency_noises.g * 0.625) + (low_frequency_noises.b * 0.25) + (low_frequency_noises.a * 0.125);

    // define the base cloud shape by dilating it with the low frequency fBm made of Worley noise.
    float base_cloud = Remap(low_frequency_noises.r, -(1.0 - low_freq_fBm), 1.0, 0.0, 1.0);

    // Get the density-height gradient using the density-height function (not included)
    //float density_height_gradient = GetDensityHeightGradientForPoint( lCustomPerMeshUniforms, lfAtmosphereY, kfHackDebugScreenSide > 0.5 ? lCloudProperties.mfLODDistance : lWeatherDataVec4.b );
    float density_height_gradient = GetDensityHeightGradientForPoint(lCustomPerMeshUniforms, lfAtmosphereY, lWeatherDataVec4.b);

    // apply the height function to the base cloud shape
    base_cloud *= density_height_gradient;

    if (base_cloud <= 0.0)
    {
        return 0.0;
    }

    //Use remapper to apply cloud coverage attribute
    float base_cloud_with_coverage = Remap(base_cloud, 1.0 - cloud_coverage, 1.0, 0.0, 1.0);

    //Multiply result by cloud coverage so that smaller clouds are lighter and more aesthetically pleasing.
    //define final cloud value
    return base_cloud_with_coverage * lCloudProperties.mfSampleScalar;
}



float 
SampleCloudMedium(
    in CustomPerMeshUniforms lCustomPerMeshUniforms,
    in cCloudProperties      lCloudProperties, 
    in vec3                  lRayVec3, 
    in float                 lfMipLevel,
    in int                   liQuality )
{
	// get height fraction  (be sure to create a cloud_min_max variable)
	float lfAtmosphereY = NormalizedAtmosphereY(lRayVec3, lCloudProperties.mfEarthRadius, lCloudProperties.mfAtmosphereStartHeight, lCloudProperties.mfAtmosphereThickness);

	vec3 wind_direction = GetWindDirection(lCloudProperties.mTriplanarBlendWeights, lCustomPerMeshUniforms.gWindOffset.xy);

	float cloud_speed = lCloudProperties.mfAnimationScale;

	// cloud_top offset - push the tops of the clouds along this wind direction by this many units.
	float cloud_top_offset = 500.0;
	// skew in wind direction
	lRayVec3 += lfAtmosphereY * wind_direction * cloud_top_offset;

	//animate clouds in wind direction and add a small upward bias to the wind direction
	lRayVec3 += wind_direction * cloud_speed;

	vec4 lWeatherDataVec4 = SampleCoverage( lCustomPerMeshUniforms,
                                            lCloudProperties,
                                            lRayVec3,
                                            false );

	// cloud coverage is stored in the weather_data’s red channel.
	float cloud_coverage = lWeatherDataVec4.r * lCloudProperties.mfCloudRatio;

    // apply anvil deformations
    cloud_coverage = pow(cloud_coverage, Remap(lfAtmosphereY, 0.7, 0.8, 1.0, mix(1.0, 0.5, lCloudProperties.mfCloudBottomFade)));

    cloud_coverage = smoothstep(0.25, 1.0, cloud_coverage);

    if ( cloud_coverage <= 0.0 )
	{
		return max( 0.0, cloud_coverage );
	}

    //vec3 lLowFreqNoiseCoordsVec3 = vec3( lRayVec3 * lCloudProperties.mfBaseScale + (lCustomPerMeshUniforms.gBaseOffset.xyz * lCloudProperties.mfAnimationScale )); // * gfTime
    vec3 lLowFreqNoiseCoordsVec3 = vec3( lRayVec3 * lCloudProperties.mfBaseScale ); // * gfTime

    // wind settings
    
    // read the low frequency Perlin-Worley and Worley noises
    vec4 low_frequency_noises = texture3DLod( SAMPLER_GETLOCAL( lCustomPerMeshUniforms, gPerlin3D ), lLowFreqNoiseCoordsVec3, lfMipLevel );

    // build an fBm out of  the low frequency Worley noises that can be used to add detail to the Low frequency Perlin-Worley noise
    float low_freq_fBm = ( low_frequency_noises.g * 0.625 ) + ( low_frequency_noises.b * 0.25 ) + ( low_frequency_noises.a * 0.125 );

    // define the base cloud shape by dilating it with the low frequency fBm made of Worley noise.
    float base_cloud = Remap( low_frequency_noises.r, - ( 1.0 -  low_freq_fBm), 1.0, 0.0, 1.0 );

    // Get the density-height gradient using the density-height function (not included)
    //float density_height_gradient = GetDensityHeightGradientForPoint( lCustomPerMeshUniforms, lfAtmosphereY, kfHackDebugScreenSide > 0.5 ? lCloudProperties.mfLODDistance : lWeatherDataVec4.b );
    float density_height_gradient = GetDensityHeightGradientForPoint(lCustomPerMeshUniforms, lfAtmosphereY, lWeatherDataVec4.b);

    // apply the height function to the base cloud shape
    base_cloud *= density_height_gradient;

    //Use remapper to apply cloud coverage attribute
    float base_cloud_with_coverage = Remap( base_cloud, 1.0 - cloud_coverage, 1.0, 0.0, 1.0 ); 

    //Multiply result by cloud coverage so that smaller clouds are lighter and more aesthetically pleasing.
    //base_cloud_with_coverage *= cloud_coverage;
    //base_cloud_with_coverage *= 1.0 - cloud_coverage;

    //define final cloud value
    //float final_cloud = mix( base_cloud_with_coverage, smoothstep( 0.4, 0.5, cloud_coverage ), lCloudProperties.mfSampleThreshold );
    float final_cloud = base_cloud_with_coverage;

    //final_cloud = step(0.1, final_cloud);
    return final_cloud * lCloudProperties.mfSampleScalar;
	//return step( 0.5, cloud_coverage );
}


STATIC_CONST int kiLightSampleIterations = 6;

// a function to gather density in a cone for use with lighting clouds.
float 
SampleCloudDensityAlongCone( 
    in CustomPerMeshUniforms lCustomPerMeshUniforms,
    in cCloudProperties      lCloudProperties,
    //in vec3                  lRayStepVec3, 
    in vec3                  lRayVec3,
    in float                 lfMipLevel )
{

    float lfDensityAlongCone = 0.0;
    vec3  lLightRayVec3 = lRayVec3;

    float kfConeStep = lCloudProperties.mfConeRadius / kiLightSampleIterations;
    vec3  lLightRayStepVec3 = (-lCloudProperties.mLightDirectionVec3 * ( lCloudProperties.mfSunRayLength / kiLightSampleIterations ));

    // lighting ray march loop
    for( int i = 0; i < kiLightSampleIterations; i++ )
    {
        // add the current step offset to the sample position
        lLightRayVec3 += lLightRayStepVec3 + ( kfConeStep * RandomUnitSphere[i] * float(i) );
        // sample cloud density the expensive way
        //float lfMipOffset = kfHackDebugScreenSide > 0.5 ? floor( i * 0.5 ) : 0.0;
        float lfMipOffset   = float(i);//floor(i * 0.5);
        lfDensityAlongCone += SampleCloudLighting(  lCustomPerMeshUniforms,
                                            lCloudProperties, 
                                            lLightRayVec3, 
                                            //lWeatherDataVec4,
                                            lfMipLevel + lfMipOffset,
                                            //0.0,
                                            1 );
    }

    return lfDensityAlongCone;
}

float 
GetLightEnergy( 
    in cCloudProperties      lCloudProperties,
    in vec3                  lRayVec3, 
    in float                 lfAtmosphereY, 
	in float                 lfSampleDensity,  // dl is the density sampled along the light ray for the given sample position.
	in float                 lfSampleDensityLight,  // ds_lodded is the low lod sample of density at the given sample position.
    in float                 lfCosAngle, 
    in float                 lfStepSize )
{
    // Henyey
    float lfForwardPhase   = HenyeyGreensteinPhase( lfCosAngle, lCloudProperties.mfForwardScatteringG );
    float lfBackwardsPhase = HenyeyGreensteinPhase( lfCosAngle, lCloudProperties.mfBackwardScatteringG );
    //lfPhaseProbability = max( HenyeyGreensteinPhase( lfCosAngle, eccentricity), silver_intensity * HenyeyGreensteinPhase( lfCosAngle, 0.99 – silver_spread));
    float lfPhaseProbability = max( lfForwardPhase, lfBackwardsPhase * lCloudProperties.mfDarkOutlineScalar );
    //Energy = max( HG( cos(?), eccentricity), silver_intensity * HG( cos(?), 0.99 – silver_spread))

    // Beer
    float lfPrimaryAttenuation     = exp( -lfSampleDensityLight * lCloudProperties.mfDensity );
    float lfSecondaryAttenuation   = exp( -lfSampleDensityLight * lCloudProperties.mfDensity * 0.25) * 0.7;

	// Reduce the secondary component when we look toward the sun.
	lfSecondaryAttenuation = Remap(lfCosAngle, 0.7, 1.0, lfSecondaryAttenuation, lfSecondaryAttenuation * 0.25);
	float lfAttenuationProbability = max( lfSecondaryAttenuation, lfPrimaryAttenuation );

    // in-scattering – one difference from presentation slides – we also reduce this effect once light has attenuated to make it directional.
    //float depth_probability      = mix( 0.05 + pow( lfSampleDensityLoded, Remap( lfAtmosphereY, 0.3, 0.85, 0.5, 2.0 )), 1.0, saturate( lfSampleDensityLight / lfStepSize));
	//float lfDepthProbability = 0.05 + pow(lfSampleDensity, Remap(lfAtmosphereY, 0.3, 0.85, 0.5, 2.0));
	float lfDepthProbability      = 0.05 + pow(lfSampleDensity, Remap(lfAtmosphereY, 0.3, 0.85, 0.5, 2.0));
    float lfVerticalProbability   = pow( Remap( lfAtmosphereY, 0.07, 0.14, 0.1, 1.0 ), 0.8 );
    float lfInScatterProbability  = lfDepthProbability * lfVerticalProbability;

	return lfInScatterProbability * lfAttenuationProbability * lfPhaseProbability;
    //return lfPhaseProbability;
    //return 0.1;
}




vec4
SampleLightParticle(
    in CustomPerMeshUniforms lCustomPerMeshUniforms,
    in cCloudProperties      lCloudProperties,
    in float                 lfSampleDensity,
    in float                 lfSampleDensityLight,
    in float                 lfCosAngle,
    in vec3                  lRayVec3,
    //in float                 lfAtmosphereY,
    in bool                  lbHasHorizon )
{
    vec4  lParticleVec4;

    float lfAtmosphereY = NormalizedAtmosphereY( lRayVec3, lCloudProperties.mfEarthRadius, lCloudProperties.mfAtmosphereStartHeight, lCloudProperties.mfAtmosphereThickness );

    vec3 lAmbientLightVec3 = SampleAmbientLight( lCustomPerMeshUniforms.gCloudBaseColour.xyz, lCustomPerMeshUniforms.gCloudTopColour.xyz, pow( lfAtmosphereY, 0.5 ) * exp( -lfSampleDensityLight * lCloudProperties.mfAmbientDensity ) );
    
    //float lfNormalizedDepth = distance( lCloudProperties.mCameraPositionVec3, lRayVec3 ) / lCloudProperties.mfMaxDistance;
    //vec3 lSunLightVec3     = SampleLight( lCustomPerMeshUniforms, lCloudProperties, lRayVec3, lfDensity, lfCosAngle, lfNormalizedDepth, lbHasHorizon );

    float lfStepSize           = lCloudProperties.mfRayStepLengthMin;

    float lfLightEnergy = GetLightEnergy(   lCloudProperties,
                                            lRayVec3, 
                                            lfAtmosphereY, 
											lfSampleDensity,
											lfSampleDensityLight,
                                            lfCosAngle, 
                                            lfStepSize );

    vec3 lSunLightVec3 = lCloudProperties.mLightColourVec3 * lfLightEnergy;

    lSunLightVec3     *= lCloudProperties.mfLightScalar;
    lAmbientLightVec3 *= lCloudProperties.mfAmbientScalar;
    
    lParticleVec4.rgb  = lSunLightVec3 + lAmbientLightVec3;
    lParticleVec4.rgb *= lfSampleDensity;
    lParticleVec4.a    = lfSampleDensity;

    return lParticleVec4;
}


STATIC_CONST mat4 bayerfrac = mat4(
    vec4(1, 9, 3, 11),
    vec4(13, 5, 15, 7),
    vec4(4, 12, 2, 10),
    vec4(16, 8, 14, 6)
) / 17.0;


float
GetBayer(
    uvec2 lPos )
{
    

    //vec2 positionMod = vec2( uvec2( tc * lUniforms.mpPerFrame->gFrameBufferSizeVec4.xy ) & 3 );

    uvec2 positionMod = uvec2( lPos & 3 );

    float rndoffset = bayerfrac[positionMod.x][positionMod.y];
    //float rndoffset = float(positionMod.x) * 0.25;

    return rndoffset;
}


vec4 
Render( 
   in CustomPerMeshUniforms lCustomPerMeshUniforms,
   in cCloudProperties      lCloudProperties,
   in vec3                  lRayDirectionVec3,
   in vec3                  lRayStartVec3,
   in vec3                  lRayEndVec3,
   in vec3                  lRayHighVec3,
   in bool                  lbAboveHighClouds,
   in bool                  lbHasHorizon,
   in bool                  lbHighCloudsOnly,
   in vec2                  lTexCoords )
{
    vec4  lColourVec4         = vec4( 0.0, 0.0, 0.0, 0.0 );
    float lfMipLevel          = 0.0;
    float lfAtmosphereY       = 0.0;
    int   liIteration         = 0;
    int   liDebugMissedRays   = 0;
    int   liDebugRaysOutCloud = 0;
    int   liDebugRaysInCloud  = 0;
    float lfDensity = 0.0;


    

    vec3 lRayVec3 = lRayStartVec3;
    
    if ( !lbHasHorizon || lCloudProperties.mfRayAboveHorizon > lCloudProperties.mfRayMinimumY )
    {
        bool  lbIsInsideCloud          = false;
        float lfSampleDensityPrevious  = -1.0;
        int   liZeroDensitySampleCount = 0;
        int   liSampleCount            = lCloudProperties.miMaxIterations;
        float lfLOD = 1.0;

        float lfCosAngle               = dot( normalize(lRayDirectionVec3), -lCloudProperties.mLightDirectionVec3 );

        // Start the Ray March Loop
        if ( !lbHighCloudsOnly )
        {
            while (liIteration < liSampleCount)
            {

#ifdef D_RENDER_LOW

                float lfSampleDensity = SampleCloudMedium(lCustomPerMeshUniforms,
                    lCloudProperties,
                    lRayVec3,
                    3.0,
                    0); // Low Quality

                //lColourVec4 += vec4(lfSampleDensity, lfSampleDensity, lfSampleDensity, lfSampleDensity);
                //lColourVec4 += vec4(lfSampleDensity * 0.01, 0.0, 0.0, 0.0);
                lColourVec4 += vec4(lfSampleDensity, lfSampleDensity, lfSampleDensity, lfSampleDensity);

                lRayVec3 += lRayDirectionVec3 * (lCloudProperties.mfRayStepLengthMax);

#elif defined(D_RENDER_MED)  

                float lfSampleDensity = SampleCloudMedium(lCustomPerMeshUniforms,
                    lCloudProperties,
                    lRayVec3,
                    10.0,
                    1); // Med Quality

                lColourVec4 += vec4(lfSampleDensity, lfSampleDensity, lfSampleDensity, lfSampleDensity);
                //lColourVec4 += vec4(lfSampleDensity, 0.0, 0.0, 0.0);

                lRayVec3 += lRayDirectionVec3 * (lCloudProperties.mfRayStepLengthMax);

#else
                // lbIsInsideCloud starts as zero so we always evaluate the second case from the beginning
                if ( !lbIsInsideCloud )
                {
                    // Sample density the cheap way only using the low-frequency noise
                    float lfSampleDensity = SampleCloudMedium(      lCustomPerMeshUniforms,
                                                                    lCloudProperties,
                                                                    lRayVec3,
                                                                    //lfMipLevel,
                                                                    10.0,
                                                                    1 ); // Low Quality

                    // If we are outside of a cloud, then continue
                    //if ( lfSampleDensity <= lCloudProperties.mfSampleThreshold )
                    if (lfSampleDensity <= 0.0)
                    {
                        lRayVec3 += lRayDirectionVec3 * lCloudProperties.mfRayStepLengthMax;
                        
                        liDebugRaysOutCloud++;
                    }
                    else
                    {
                        //take a step back and capture area we skipped.
                        lRayVec3 -= lRayDirectionVec3 * lCloudProperties.mfRayStepLengthMax * lfSampleDensity;
                        //lRayVec3 -= lRayDirectionVec3 * lCloudProperties.mfRayStepLengthMax;

                        liDebugMissedRays++;
                        lbIsInsideCloud = true;
                    }
                }
                else
                {
                    // Sample Density the expensive way by setting the last parameter to false, indicating a full sample
                    float lfSampleDensity = SampleCloud(    lCustomPerMeshUniforms,
                                                            lCloudProperties,
                                                            lRayVec3,
                                                            lfMipLevel,
                                                            2 ); // High Quality

                    // If we just sampled a zero and the previous sample was zero, increment the counter
                    if ( lfSampleDensity <= lCloudProperties.mfSampleThreshold )
                    {
                        if ( lfSampleDensityPrevious <= lCloudProperties.mfSampleThreshold )
                        {
                            liZeroDensitySampleCount++;

                            if ( liZeroDensitySampleCount > 10 )
                            {
                                // If not then set cloud_test it zero so that we go back to cheap sample case
                                lbIsInsideCloud = false;
                                liZeroDensitySampleCount = 0;
                            }
                        }
                    }
                    else
                    {
                        lfDensity += lfSampleDensity;
                        float lfSampleDensityLight = SampleCloudDensityAlongCone(   lCustomPerMeshUniforms,
                                                                                    lCloudProperties,
                                                                                    lRayVec3,
                                                                                    lfMipLevel);

                        // get light energy here
                        // add to alpha here
                        // attenuate light energy by alpha here to get light intensity        
                        {
                            vec4 lParticleVec4 = SampleLightParticle(lCustomPerMeshUniforms,
                                lCloudProperties,
                                lfSampleDensity,
                                lfSampleDensityLight,
                                lfCosAngle,
                                lRayVec3,
                                lbHasHorizon);

                            lColourVec4 = (1.0 - lColourVec4.a) * lParticleVec4 + lColourVec4;

                        }

                    }

                    lRayVec3 += lRayDirectionVec3 * (lCloudProperties.mfRayStepLengthMin * lfLOD);
                    //lRayVec3 += lRayDirectionVec3 * (lCloudProperties.mfRayStepLengthMin);

                    lfSampleDensityPrevious = lfSampleDensity;
                    liDebugRaysInCloud++;
                }
#endif


                liIteration++;

                lColourVec4   = saturate(lColourVec4);

                lfAtmosphereY = NormalizedAtmosphereY(lRayVec3, lCloudProperties.mfEarthRadius, lCloudProperties.mfAtmosphereStartHeight, lCloudProperties.mfAtmosphereThickness);
                lfLOD         = floor(lfAtmosphereY * 3.0) + 1.0;

                if (lColourVec4.a > 0.99 || lfAtmosphereY >= 1.0 )
                {
                    break;
                }
            }
        }

        if (lCloudProperties.mfRayAboveHorizon > lCloudProperties.mfRayMinimumY)
        {
            vec3 lBlendWeights = pow( abs( normalize( lRayHighVec3 ) ), vec3(32.0, 32.0, 32.0) );
            lBlendWeights /= (lBlendWeights.x + lBlendWeights.y + lBlendWeights.z);

			vec3 lWindDirection = GetWindDirection( lBlendWeights, lCustomPerMeshUniforms.gWindOffset.zw );


            // get light energy here
            // add to alpha here
            // attenuate light energy by alpha here to get light intensity    
            
            vec3 lRayHighCoverageVec3 = lRayHighVec3 + (lWindDirection * lCloudProperties.mfAnimationScale * 0.5);
            //vec3 lRayHighCoverageVec3 = lRayHighVec3;

			vec4 lCoverageHighVec4 = GetTriPlanarMap( lBlendWeights, lRayHighCoverageVec3 * lCloudProperties.mfCoverageScale * 1.0, SAMPLER2DPARAM_LOCAL(lCustomPerMeshUniforms, gCoverage2D));

            vec3 lRayHighCloudVec3 = lRayHighVec3 + (lWindDirection * lCloudProperties.mfAnimationScale);
            //vec3 lRayHighCloudVec3 = lRayHighVec3;

			vec4 lCloudsHighVec4   = GetTriPlanarMap( lBlendWeights, lRayHighCloudVec3 * lCloudProperties.mfCoverageScale * 4.0, SAMPLER2DPARAM_LOCAL(lCustomPerMeshUniforms, gCloudsHigh2D));

            float lfCloudHighDensity =  Remap( lCloudsHighVec4.r, lCoverageHighVec4.r, 1.0, 0.0, 1.0 );
            lfCloudHighDensity *= lfCloudHighDensity;

            lfCloudHighDensity = max(lfCloudHighDensity, 0.01);
            
            vec4 lParticleVec4;
            lParticleVec4.rgb  = mix( lCustomPerMeshUniforms.gCloudTopColour.rgb, lCustomPerMeshUniforms.gCloudBaseColour.rgb, lfCloudHighDensity );
            lParticleVec4.a    = lfCloudHighDensity;
			lParticleVec4.rgb /= lParticleVec4.a;

            lParticleVec4.rgb = saturate(lParticleVec4.rgb);
			
			if (lbAboveHighClouds)
			{
				lColourVec4 = lParticleVec4.a * lParticleVec4 + (lColourVec4 * (1.0 - lParticleVec4.a));
			}
			else
			{
				lColourVec4 = lColourVec4.a * lColourVec4 + (lParticleVec4 * (1.0 - lColourVec4.a));
			}
        }

        if ( lbHasHorizon )
        {
            float lfFadeMinimumY = lCloudProperties.mfRayMinimumY;
            //float lfFadeMinimumY = 0.0;

            float fade = smoothstep(    lfFadeMinimumY,
                                        lfFadeMinimumY + ( 1.0 - lfFadeMinimumY ) * mix( lCloudProperties.mfHorizonFadeScalar, 0.0, lCloudProperties.mfRatioAbovePlanet),
                                        lCloudProperties.mfRayAboveHorizon );

            lColourVec4 *= lCloudProperties.mfHorizonFadeStartAlpha + fade * (1.0 - lCloudProperties.mfHorizonFadeStartAlpha);
        }
    }

    lColourVec4 = saturate(lColourVec4);
    
    /*
    if ( lTexCoords.x < 0.25 )
    {
        //return vec4( DebugColour( float(liDebugRaysOutCloud + liDebugRaysInCloud + liDebugMissedRays) / 128.0, 0.0, 1.0 ).xyz, 1.0 );
        return vec4(DebugColour(float(liDebugRaysInCloud) / 128.0, 0.0, 1.0).xyz, 1.0);
        //return vec4(DebugColour( lfDensity, 0.0, 1.0).xyz, 1.0);
        //return vec4( DebugColour( float(lCloudProperties.miMaxIterations) / 128.0, 0.0, 1.0 ).xyz, 1.0 );
    }
    else
    if ( lTexCoords.x < 0.5 )
    {
        return vec4( DebugColour( float( liDebugRaysInCloud ) / 128.0, 0.0, 1.0 ).xyz, 1.0 );
        //return vec4(DebugColour(lCloudProperties.mfRayStepLengthMin / 100.0, 0.0, 1.0).xyz, 1.0);
    }
    else
    if ( lTexCoords.x < 0.75 )
    {
        return vec4( DebugColour( float( liDebugRaysOutCloud ) / 128.0, 0.0, 1.0 ).xyz, 1.0 );
        //return vec4( DebugColour(  lCloudProperties.mfRayStepLengthMax / 100.0, 0.0, 1.0 ).xyz, 1.0 );
    }
    else
    {
        return lColourVec4;
    }*/

    return lColourVec4;
}


//-----------------------------------------------------------------------------
///
///     Fragment Main
///
///     @brief      Fragment Main
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------



FRAGMENT_MAIN_COLOUR01_SRT
{    
    cCloudProperties lCloudProperties;
    vec4             lCloudColourVec4;
    vec3             lPlanetRelativeCamera;
    vec3             lRayStartVec3;
	vec3             lRayEndVec3;

    vec3             lRayStartOuterVec3 = vec3( 0.0, 0.0, 0.0 );
    vec3             lRayEndOuterVec3   = vec3( 0.0, 0.0, 0.0 );

	vec3             lRayHighVec3;
    vec3             lRayDirectionVec3;
    float            lfCloudDepth;

    float            lfPlanetRadius   = lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w;
    bool             lbHasHorizon     = false;
    bool             lbHighCloudsOnly = false;

    vec3             lDebugColour = vec3(1.0, 0.0, 0.0);
    bool             lbDebug      = false;
    bool             lbTwoPass = false;
    bool             lbDiscard = false;


    {
        lCloudProperties.miMaxIterations         = int( lUniforms.mpCustomPerMesh->gCoverageParamsVec4.x );
        lCloudProperties.mfHorizonCoverageStart  = lUniforms.mpCustomPerMesh->gCoverageParamsVec4.y;
        lCloudProperties.mfHorizonCoverageEnd    = lUniforms.mpCustomPerMesh->gCoverageParamsVec4.z;
        lCloudProperties.mfAmbientDensity        = lUniforms.mpCustomPerMesh->gCoverageParamsVec4.w;

        lCloudProperties.mfLightScalar           = lUniforms.mpCustomPerMesh->gLightingParamsVec4.x;
        lCloudProperties.mfAmbientScalar         = lUniforms.mpCustomPerMesh->gLightingParamsVec4.y;
        lCloudProperties.mfSunRayLength          = lUniforms.mpCustomPerMesh->gLightingParamsVec4.z;
        lCloudProperties.mfConeRadius            = lUniforms.mpCustomPerMesh->gLightingParamsVec4.w;

        lCloudProperties.mfDensity               = lUniforms.mpCustomPerMesh->gLightScatteringParamsVec4.x;
        lCloudProperties.mfForwardScatteringG    = lUniforms.mpCustomPerMesh->gLightScatteringParamsVec4.y;
        lCloudProperties.mfBackwardScatteringG   = lUniforms.mpCustomPerMesh->gLightScatteringParamsVec4.z;
        lCloudProperties.mfDarkOutlineScalar     = lUniforms.mpCustomPerMesh->gLightScatteringParamsVec4.w;

        //lCloudProperties.mfAnimationScale        = lUniforms.mpCustomPerMesh->gAnimationParamsVec4.x * lUniforms.mpPerFrame->gfTime;
        lCloudProperties.mfAnimationScale        = lUniforms.mpCustomPerMesh->gAnimationParamsVec4.x *lUniforms.mpPerFrame->gfTime;
        lCloudProperties.mfCloudRatio            = lUniforms.mpCustomPerMesh->gAnimationParamsVec4.y;

        lCloudProperties.mfBaseScaleScalar       = lUniforms.mpCustomPerMesh->gModelingBaseParamsVec4.x;
        lCloudProperties.mfSampleScalar          = lUniforms.mpCustomPerMesh->gModelingBaseParamsVec4.y;
        lCloudProperties.mfSampleThreshold       = lUniforms.mpCustomPerMesh->gModelingBaseParamsVec4.z;
        lCloudProperties.mfCloudBottomFade       = lUniforms.mpCustomPerMesh->gModelingBaseParamsVec4.w;

        lCloudProperties.mfDetailScale           = lUniforms.mpCustomPerMesh->gModelingDetailParamsVec4.x;
        lCloudProperties.mfErosionEdgeSize       = lUniforms.mpCustomPerMesh->gModelingDetailParamsVec4.y;
        lCloudProperties.mfCloudDistortion       = lUniforms.mpCustomPerMesh->gModelingDetailParamsVec4.z;
        lCloudProperties.mfCloudDistortionScale  = lUniforms.mpCustomPerMesh->gModelingDetailParamsVec4.w;

        lCloudProperties.mfRayMinimumY           = lUniforms.mpCustomPerMesh->gOptimisationParamsVec4.x;
        lCloudProperties.mfLODDistance           = lUniforms.mpCustomPerMesh->gOptimisationParamsVec4.y;
        lCloudProperties.mfHorizonFadeStartAlpha = lUniforms.mpCustomPerMesh->gOptimisationParamsVec4.z;
        lCloudProperties.mfHorizonFadeScalar     = lUniforms.mpCustomPerMesh->gOptimisationParamsVec4.w;	// Fades clouds on horizon, 1.0 -> 10.0 (1.0 = smooth fade, 10 = no fade)

        lCloudProperties.mfHorizonDistance       = lUniforms.mpCustomPerMesh->gAtmosphereParamsVec4.x;
        lCloudProperties.mfAtmosphereStartHeight = lUniforms.mpCustomPerMesh->gAtmosphereParamsVec4.y;
        lCloudProperties.mfAtmosphereEndHeight   = lUniforms.mpCustomPerMesh->gAtmosphereParamsVec4.z;
		lCloudProperties.mfStratosphereHeight    = lUniforms.mpCustomPerMesh->gAtmosphereParamsVec4.w;
    }

    // Calculated Values
    {
        lCloudProperties.mfEarthRadius                   = lfPlanetRadius;
        // Do the opposite of this M_CALCULATE_PLANET_RADIUS
        lCloudProperties.mfHorizonDistance               = sqrt( ((lCloudProperties.mfEarthRadius+lCloudProperties.mfAtmosphereStartHeight)*lCloudProperties.mfAtmosphereStartHeight*2.0)-(lCloudProperties.mfAtmosphereStartHeight*lCloudProperties.mfAtmosphereStartHeight));
        lCloudProperties.mfAtmosphereThickness           = lCloudProperties.mfAtmosphereEndHeight - lCloudProperties.mfAtmosphereStartHeight;
        //lCloudProperties.mfEarthRadius                   = M_CALCULATE_PLANET_RADIUS( lCloudProperties.mfAtmosphereStartHeight, lCloudProperties.mfHorizonDistance );
        lCloudProperties.mfMaxDistance                   = M_CALCULATE_HORIZON_DISTANCE( lCloudProperties.mfEarthRadius, (lCloudProperties.mfEarthRadius + lCloudProperties.mfAtmosphereEndHeight) );
        lCloudProperties.mfBaseScale                     = 1.0 / lCloudProperties.mfAtmosphereEndHeight * lCloudProperties.mfBaseScaleScalar;
        lCloudProperties.mfCoverageScale                 = 1.0 / lCloudProperties.mfMaxDistance;
    }

    
    lPlanetRelativeCamera     = lUniforms.mpPerFrame->gViewPositionVec3 - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz;

    vec2  lTexCoords   = TEX_COORDS.xy;
    float lfFrameIndex = mod( lUniforms.mpCustomPerMesh->gCloudSubFrameParamsVec4.x, 16.0 );

    //kfHackDebugScreenSide = lTexCoords.x;
    
    
//     {
//         uvec2 luPixelCoordVec2 = uvec2( round( lTexCoords * vec2(1920.0,1080.0) ) );
//         luPixelCoordVec2 += InverseBayer( uint( floor( lfFrameIndex ) ) );
// 
//         lTexCoords = vec2(luPixelCoordVec2) * (1.0/vec2(1920.0,1080.0));
//     }
// 
//     if ( int(lfFrameIndex) != int(mod(lTexCoords.x * 960.0, 4.0)) )
//     {
//         discard;
//         //FRAGMENT_COLOUR = vec4( 1.0, 0.0, 0.0, 1.0 );
//         //return;
//     }

    lRayDirectionVec3         = GetScreenSpaceViewDir( lTexCoords, lUniforms.mpCommonPerMesh->gInverseProjectionMat4, lUniforms.mpCommonPerMesh->gInverseViewMat4 );
                              
    float lfCloudHeightMin    = lCloudProperties.mfAtmosphereStartHeight;
    float lfCloudHeightMax    = lCloudProperties.mfAtmosphereEndHeight;
	float lfStratoCloudsHeight  = lCloudProperties.mfStratosphereHeight;

    float lfHeightAbovePlanet = length( lPlanetRelativeCamera ) - lfPlanetRadius;

    lRayStartVec3 = normalize( lPlanetRelativeCamera ) * ( lCloudProperties.mfEarthRadius + lfHeightAbovePlanet );

    {
        float lfDeterminantMin  = 0.0;
		float lfDeterminantMax  = 0.0;
		float lfDeterminantHigh = 0.0;

        vec3 lIntersectionMinNear;
        vec3 lIntersectionMinFar;
        vec3 lIntersectionMaxNear;
        vec3 lIntersectionMaxFar;
		vec3 lIntersectionHighNear;
		vec3 lIntersectionHighFar;
        vec3 lCameraAt = MAT4_GET_COLUMN(lUniforms.mpPerFrame->gCameraMat4, 2).xyz;

        lfDeterminantMin  = GetRayIntersectionPoint( lRayStartVec3, lRayStartVec3 + lRayDirectionVec3, lCloudProperties.mfEarthRadius + lfCloudHeightMin,       lIntersectionMinNear,  lIntersectionMinFar );
        lfDeterminantMax  = GetRayIntersectionPoint( lRayStartVec3, lRayStartVec3 + lRayDirectionVec3, lCloudProperties.mfEarthRadius + lfCloudHeightMax,       lIntersectionMaxNear,  lIntersectionMaxFar );
		lfDeterminantHigh = GetRayIntersectionPoint( lRayStartVec3, lRayStartVec3 + lRayDirectionVec3, lCloudProperties.mfEarthRadius + lfStratoCloudsHeight,     lIntersectionHighNear, lIntersectionHighFar );

        if ( lfHeightAbovePlanet < lfCloudHeightMin )
        {
            /*
            float lfDepth = texture2D(SAMPLER_GETMAP(lUniforms.mpCustomPerMesh, gDepthMap), IN(mTexCoordsVec2)).r;

            if (lfDepth != 0.0)
            {
                lbDiscard = true;
            }
            */

            // On planet, below cloud layer
            if ( lfDeterminantMin == 0.0 )
            {
                //discard;
                lbDiscard = true;
            }

            lRayStartVec3 = lIntersectionMinFar;
            lRayEndVec3   = lIntersectionMaxFar;
			lRayHighVec3  = lIntersectionHighFar;
            lbHasHorizon  = true;

            lfCloudDepth = dot( lPlanetRelativeCamera - lRayStartVec3, lCameraAt );
        }
        else
        if ( lfHeightAbovePlanet > lfCloudHeightMax )
        {
            // Above planet, outside of ray marched cloud layer
            if ( lfDeterminantMax == 0.0 ||
                dot(lIntersectionMaxNear - lRayStartVec3, lRayDirectionVec3) < 0.0)
            {
                // We do not intersect with the outer sphere at all (no ray marched clouds)
                lbHighCloudsOnly = true;
            }

            if (lfHeightAbovePlanet > lfStratoCloudsHeight )
            {
                if ( lfDeterminantHigh == 0.0 ||
                     dot(lIntersectionHighNear - lRayStartVec3, lRayDirectionVec3) < 0.0)
                {
                    // We don't intersect with any clouds
                    //discard;
                    //lbDebug = true;
                    FRAGMENT_COLOUR0 = vec4(0.0, 0.0, 0.0, 0.0);
                    FRAGMENT_COLOUR1 = float2vec4( 1.0 );
                    return;
                }
            }
            
            
            if ( lfDeterminantMin == 0.0 )
            {
                // We don't intersect with the inner sphere
                lRayEndVec3 = lIntersectionMaxFar;
            }
            else
            {
                // We do intersect with the inner sphere
                lRayEndVec3 = lIntersectionMinNear;
            }


            lRayStartVec3  = lIntersectionMaxNear;

            if ( lfHeightAbovePlanet > lfStratoCloudsHeight )
            {
                lRayHighVec3 = lIntersectionHighNear;
                //lbDebug = true; 
            }
            else
            {
                lRayHighVec3 = lIntersectionHighFar;
            }

            //lfCloudDepth = abs(lPlanetRelativeCamera - lIntersectionMinNear).z;
            lfCloudDepth = dot(lPlanetRelativeCamera - lIntersectionMinNear, lCameraAt);
        }
        else
        {
            // In the ray marched clouds
            if ( lfDeterminantMin == 0.0 ||
                 dot(lIntersectionMinNear - lRayStartVec3, lRayDirectionVec3) < 0.0 )
            {
                lRayEndVec3 = lIntersectionMaxFar;
			}
			else
			{
				lRayEndVec3        = lIntersectionMinNear;
                lRayStartOuterVec3 = lIntersectionMinFar;
                lRayEndOuterVec3   = lIntersectionMaxNear;

                lbTwoPass = true;
            }

			lRayHighVec3 = lIntersectionHighFar;

            //lfCloudDepth = abs(lPlanetRelativeCamera - lRayStartVec3).z;
            //lfCloudDepth = length(lRayEndVec3);
            lfCloudDepth = 0.0;
        }

        float lfRayLength        = length( lRayEndVec3 - lRayStartVec3 );
        float lfRayStepLengthMin = lCloudProperties.mfAtmosphereThickness / (float(lCloudProperties.miMaxIterations) * 0.5);

        /*
        int liNumIterations      = min( lCloudProperties.miMaxIterations, int( lfRayLength / lfRayStepLengthMin) );
        lfRayLength = min( lfRayLength, lCloudProperties.mfAtmosphereThickness * 8.0 );
        lCloudProperties.mfRayStepLengthMin = lfRayLength / float(liNumIterations * 4.0);
        lCloudProperties.mfRayStepLengthMax = lfRayLength / (float(liNumIterations) * 0.5);
        lCloudProperties.miMaxIterations    = liNumIterations;
        */

        int liNumIterations = lCloudProperties.miMaxIterations;
        lCloudProperties.mfRayStepLengthMin = lfRayLength / float(liNumIterations);
        lCloudProperties.mfRayStepLengthMax = lfRayLength / float(liNumIterations * 0.5);
    }

    if (lbDiscard)
    {
        lCloudColourVec4 = vec4( 0.0, 0.0, 0.0, 0.0 );
        lfCloudDepth     = 0.0;
    }
    else
    {
        vec3 lNormalizedPlanetRelativeCamera;
		vec3 lBlendWeights;

        lNormalizedPlanetRelativeCamera = normalize( lPlanetRelativeCamera );

        //lBlendWeights = pow(abs(normalize(lRayStartVec3)), vec3(32.0, 32.0, 32.0));
        lBlendWeights = pow(abs(normalize(lRayStartVec3)), vec3(32.0, 32.0, 32.0));

		lBlendWeights /= (lBlendWeights.x + lBlendWeights.y + lBlendWeights.z);

        float lfDistanceFromPlanetCenter = length( lPlanetRelativeCamera );
        float lfOppositeSide             = sqrt( lfDistanceFromPlanetCenter*lfDistanceFromPlanetCenter - lfPlanetRadius*lfPlanetRadius );
        float lfCosB                     = dot( lRayDirectionVec3, lNormalizedPlanetRelativeCamera );
        float lfSinC                     = lfOppositeSide / lfDistanceFromPlanetCenter;

        lCloudProperties.mfRayMinimumY                   = -lfSinC;
        lCloudProperties.mfRayAboveHorizon               = lfCosB;
        lCloudProperties.mCameraPositionVec3             = lNormalizedPlanetRelativeCamera * ( lCloudProperties.mfEarthRadius + lfHeightAbovePlanet );
        lCloudProperties.mLightDirectionVec3             = -lUniforms.mpCustomPerMesh->gSunPositionVec4.xyz;//lUniforms.mpPerFrame->gLightDirectionVec4.xyz;
		lCloudProperties.mTriplanarBlendWeights          = lBlendWeights;
        lCloudProperties.mfRatioAbovePlanet              = saturate( lfHeightAbovePlanet / lCloudProperties.mfAtmosphereEndHeight );
        lCloudProperties.mLightColourVec3                = lUniforms.mpCommonPerMesh->gLightColourVec4.rgb;

        lCloudProperties.mWindDirection                  = GetWindDirection( lCloudProperties.mTriplanarBlendWeights, lUniforms.mpCustomPerMesh->gWindOffset.xy );


        
        /*
        vec2 lBlueNoiseCoords = lTexCoords * vec2((lUniforms.mpPerFrame->gFrameBufferSizeVec4.x) / 512.0, (lUniforms.mpPerFrame->gFrameBufferSizeVec4.y) / 512.0);
        float ditherValue = texture2DLod(SAMPLER_GETMAP(lUniforms.mpCustomPerMesh, gBlueNoiseMap), lBlueNoiseCoords, 0.0).x;
        ditherValue = (ditherValue * 2.0) -1.0;
        ditherValue = sign(ditherValue)*(1.0 - sqrt(1.0 - abs(ditherValue)));
        */
        float ditherValue = GetBayer( uvec2( lTexCoords * lUniforms.mpPerFrame->gFrameBufferSizeVec4.xy ) );

        lRayStartVec3 += lRayDirectionVec3 *  lCloudProperties.mfRayStepLengthMin * ditherValue;

		bool lbAboveHighClouds = lfHeightAbovePlanet > lfStratoCloudsHeight;

        lCloudColourVec4 = Render(          DEREF_PTR(lUniforms.mpCustomPerMesh), 
                                            lCloudProperties, 
                                            lRayDirectionVec3,
                                            lRayStartVec3,
                                            lRayEndVec3,
											lRayHighVec3,
											lbAboveHighClouds,
                                            //false,
                                            lbHasHorizon,
                                            //true,
                                            lbHighCloudsOnly,
                                            lTexCoords );

        /*
        if ( lbTwoPass )
        {
            float lfRayLength                   = length( lRayEndOuterVec3 - lRayStartOuterVec3 );
            int liNumIterations                 = min( lCloudProperties.miMaxIterations, int( lfRayLength / lCloudProperties.mfRayStepLengthMin ) );
            lCloudProperties.mfRayStepLengthMax = lfRayLength / liNumIterations;
            lCloudProperties.miMaxIterations    = liNumIterations;

            vec4 lCloudOuterColourVec4 = Render(    DEREF_PTR(lUniforms.mpCustomPerMesh), 
                                                    lCloudProperties, 
                                                    lRayDirectionVec3,
                                                    lRayStartOuterVec3,
											        lRayHighVec3,
											        lbAboveHighClouds,
                                                    true,
                                                    //false,
                                                    lbHighCloudsOnly,
                                                    lTexCoords );
        
            //lCloudColourVec4 = lCloudOuterColourVec4;

            //lCloudColourVec4 = lCloudOuterColourVec4.a * lCloudOuterColourVec4 + (lCloudColourVec4 * (1.0 - lCloudOuterColourVec4.a));
            lCloudColourVec4 = lCloudColourVec4.a * lCloudColourVec4 + (lCloudOuterColourVec4 * (1.0 - lCloudColourVec4.a));
        }*/


        lCloudColourVec4.a   *= 1.0 - smoothstep( lfStratoCloudsHeight * 4.0, lfStratoCloudsHeight * 5.0, lfHeightAbovePlanet );
        lCloudColourVec4.rgb *= lCloudColourVec4.a;

        
        //FRAGMENT_COLOUR = vec4(0.0);

        //float lfFrameIndex = 0.0;
        //lTexCoords += InverseBayer( lfFrameIndex ) * (lUniforms.mpPerFrame->gFrameBufferSizeVec4.zw * 0.25);  
        //lTexCoords += InverseBayer( lfFrameIndex ) * (1.0/(vec2( 1920.0, 1080.0 ) * 0.5));

        //flost lfPixelCoordX = lTexCoords.x * 960.0;
        
//         {
//             uint luPixelCoordX = uint(round(mod(lTexCoords.x * 960.0, 16.0)));
//             luPixelCoordX += InverseBayer(uint(floor(lfFrameIndex))).x;
// 
//             vec2 lModTex = mod((lTexCoords * vec2(1920.0, 1080.0) * 0.5), 16.0) * (1.0 / 16.0);
//             vec2 lModCheck = mod((lTexCoords * vec2(1920.0, 1080.0) * 0.5), 32.0) * (1.0 / 32.0);
//             float lfCheck = (lModCheck.x < 0.5 && lModCheck.y < 0.5) || (lModCheck.x > 0.5 && lModCheck.y > 0.5) ? 0.0 : 1.0;
// 
//             //lModTex *= lfCheck;
// 
//             //FRAGMENT_COLOUR = vec4( lModTex.x, lModTex.y, 1.0 - lfCheck, 1.0 );
// 
//             //vec4 lFragCol = RandomColour( int( lModTex.x * 16.0 ) );
//             vec4 lFragCol = RandomColour(luPixelCoordX);
//             //vec4 lFragCol = RandomColour( int( (lTexCoords.x * 1920.0 ) + lfFrameIndex ) );
// 
//             lFragCol = mix(lFragCol, vec4(0.5), lfCheck);
// 
//             FRAGMENT_COLOUR = vec4(lFragCol.xyz, 1.0);
//         }
    }

    FRAGMENT_COLOUR0 = lCloudColourVec4;
    FRAGMENT_COLOUR1 = float2vec4( FastNormaliseDepth( lUniforms.mpPerFrame->gClipPlanesVec4, max( lfCloudDepth, 40.0) ) );
}

#endif



#ifdef D_CLOUD_TEMPORAL

#include "Common/CommonPostProcess.shader.h"

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2,                  TEXCOORD0 )

DECLARE_END

/*
#define TAA_GET_LUMINANCE( col ) ( 0.25 * ( col.r + col.b ) + 0.5 * col.g )

vec3 SimpleUnReinhard( in vec3 col )
{
    return col / ( 1.0 - TAA_GET_LUMINANCE( col ) );
}
*/

//-----------------------------------------------------------------------------
///
///     GetPrevPosition
///
///     @brief      GetPrevPosition
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec2 GetPrevPositionSimple(
    in vec2 lFragCoordsVec2,
    in vec4 lClipPlanesVec4,
    in mat4 lInverseProjectionMat4,
    in mat4 lInverseViewMat4,
    in mat4 lProjectionMat4,
    in mat4 lViewProjectionMat4,
    in mat4 lPrevViewProjectionMat4,
    in vec3 lViewPositionVec3,
    in vec4 lMBlurSettingsVec4,
    //SAMPLER2DARG( lMotionMap ),
    //SAMPLER2DARG( lDepthMap ),
    out float lfOutDepth,
    out float lfOutDepthNormalised,
    out float lfOutRcpSpeed,
    out vec2 lOutDelta,
    out vec2 lOutEncodedDelta,
    out bool lWantsColorClipAA )
{
    vec2 lReprojectFragCoordsVec2;
    

    vec4 lPositionVec4;
    lPositionVec4.x = lFragCoordsVec2.x * 2.0 - 1.0;

#ifndef D_PLATFORM_OPENGL    
    lPositionVec4.y = ( 1.0f - lFragCoordsVec2.y ) * 2.0 - 1.0;
#else
    lPositionVec4.y = lFragCoordsVec2.y * 2.0 - 1.0;
#endif

    lPositionVec4.z = 0.0;
    lPositionVec4.w = 1.0;

    // Inverse projection
    lPositionVec4 = MUL( lInverseProjectionMat4, lPositionVec4 );
    lPositionVec4 = lPositionVec4 / lPositionVec4.w;

    // Inverse view
    mat4 lViewMat = lInverseViewMat4;
    MAT4_SET_POS( lViewMat, vec4( 0.0, 0.0, 0.0, 1.0 ) );
    lPositionVec4 = MUL( lViewMat, lPositionVec4 );

    vec3 lViewVectorVec3 = normalize( lPositionVec4.xyz );
    //lPositionVec4.xyz = lViewVectorVec3 * lfDepth + lViewPositionVec3;
    lPositionVec4.xyz = lViewVectorVec3 * lClipPlanesVec4.y + lViewPositionVec3;
    lPositionVec4.w = 1.0;

    lPositionVec4 = MUL( lPrevViewProjectionMat4, lPositionVec4 );

#ifndef D_PLATFORM_OPENGL    
    lPositionVec4.y = -lPositionVec4.y;
#endif

    lPositionVec4.xyz /= lPositionVec4.w;
    lReprojectFragCoordsVec2 = lPositionVec4.xy;
    lReprojectFragCoordsVec2 = lReprojectFragCoordsVec2 * 0.5 + 0.5;

    return lReprojectFragCoordsVec2;

}

#define GET_LUMINANCE( col ) ( 0.25 * ( col.r + col.b ) + 0.5 * col.g )

vec4 
clip_aabb(
    vec4 q,
    vec4 aabb_min,
    vec4 aabb_max )
{
    // note: only clips towards aabb center (but fast!)
    vec4 p_clip = 0.5 * ( aabb_max + aabb_min );
    vec4 e_clip = 0.5 * ( aabb_max - aabb_min );

    vec4 v_clip = q - p_clip;
    vec4 v_unit = e_clip / v_clip;
    vec4 a_unit = abs( v_unit );
    float ma_unit = saturate( min( a_unit.x, min( a_unit.y, a_unit.z ) ) );

    return p_clip + v_clip * ma_unit;
}

#ifdef D_PLATFORM_ORBIS    

float fastRcpNR0( float inX )
{
    return rcp( inX );
}

#else

float fastRcpNR0( float inX )
{
    return 1.0 / inX;
}

#endif

FRAGMENT_MAIN_COLOUR_SRT
{
    /*
     vec4 cloud;
     //cloud.xyz = SimpleUnReinhard( texture2D( lUniforms.mpCustomPerMesh->gBuffer3Map, IN( mTexCoordsVec2 ) ).xyz );
     cloud.xyz = texture2D( lUniforms.mpCustomPerMesh->gBuffer3Map, IN( mTexCoordsVec2 ) ).xyz;
     cloud.a = 1.0;
     FRAGMENT_COLOUR = cloud;

     return;
     */
    /*
    vec4  lFragmentColourVec4 = texture2D( lUniforms.mpCustomPerMesh->gBuffer2Map, IN( mTexCoordsVec2 ) );

    if ( lFragmentColourVec4.a <= 0.0 )
    {
        discard;

    }
    */
    
    /*
    float lfDepth      = DecodeDepthFromColour( texture2D( lUniforms.mpCustomPerMesh->gDepthMap, IN( mTexCoordsVec2 ) ) );
    float lfCloudDepth = DecodeDepthFromColour( texture2D( lUniforms.mpCustomPerMesh->gCloudDepthMap, IN( mTexCoordsVec2 ) ) );
    
    if ( lfDepth < lfCloudDepth )
    {
        //discard;
        FRAGMENT_COLOUR = vec4(0.0,0.0,0.0,0.0);
        return;
    }*/

    
    

    vec2  lFrameSizeVec2 = lUniforms.mpPerFrame->gFrameBufferSizeVec4.xy;
    float lfFrameIndex   = mod( lUniforms.mpCustomPerMesh->gCloudSubFrameParamsVec4.x, 16.0 );
    //float lfFrameIndex   = 0.0;
    float lfDitherValue  = Bayer( uvec2( IN( mTexCoordsVec2 ) * lFrameSizeVec2 ) ) - 1.0;


    vec4 lFragColVec4;

    //cloud = RandomColour( int(lfFrameIndex) );
    vec2  lDeditheredCoordVec2 = IN( mTexCoordsVec2 );

    {
        uvec2 luPixelCoordVec2 = uvec2( round( lDeditheredCoordVec2 * vec2( 1920.0, 1080.0 ) ) );
        luPixelCoordVec2 -= InverseBayer( uint( floor( lfFrameIndex ) ) );

        lDeditheredCoordVec2 = vec2( luPixelCoordVec2 ) * ( 1.0 / vec2( 1920.0, 1080.0 ) );
    }

    
    //lFragColVec4 = texture2D( lUniforms.mpCustomPerMesh->gBuffer2Map, lDeditheredCoordVec2 );
    
    //lFragColVec4 = texture2D( lUniforms.mpCustomPerMesh->gBuffer2Map, IN( mTexCoordsVec2 ) );
    
    
    if ( int(lfFrameIndex) == int(lfDitherValue) )
    {
        lFragColVec4 = texelFetch( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer2Map ), ivec2(IN( mTexCoordsVec2 ) * lFrameSizeVec2 * 0.25), 0 );
    }
    else
//     {
//         lFragColVec4 = texelFetch( lUniforms.mpCustomPerMesh->gBuffer5Map, ivec2( IN( mTexCoordsVec2 ) * lFrameSizeVec2 ), 0 );
// 
//         //discard;
//     }
    {
        vec2 lSample = IN( mTexCoordsVec2 );

        // compute the previous position of this pixel
        float lfDepth;
        float lfDepthNormalised;
        float lfSpeed;
        vec2  lMotion;
        vec2  lEncodedMotion;
        bool  lWantsColorClipAA;
        vec2  lSampleReproject = GetPrevPositionSimple(   lSample,
                                                    lUniforms.mpPerFrame->gClipPlanesVec4,
                                                    lUniforms.mpCommonPerMesh->gInverseProjectionMat4,
                                                    lUniforms.mpCommonPerMesh->gInverseViewMat4,
                                                    lUniforms.mpCommonPerMesh->gViewProjectionMat4,
                                                    lUniforms.mpPerFrame->gPrevViewProjectionMat4,
                                                    lUniforms.mpPerFrame->gViewPositionVec3,
                                                    lUniforms.mpPerFrame->gMBlurSettingsVec4,
                                                    //SAMPLER2DPARAM( lUniforms.mpCustomPerMesh->gBufferMap ),
                                                    //SAMPLER2DPARAM( lUniforms.mpCustomPerMesh->gBuffer1Map ),
                                                    lfDepth,
                                                    lfDepthNormalised,
                                                    lfSpeed,
                                                    lMotion,
                                                    lEncodedMotion,
                                                    lWantsColorClipAA );

        {
            //vec4 lPrevSpeeds = texture2D( lUniforms.mpCustomPerMesh->gBuffer4Map, lSampleReproject );
            //vec2 lPrevDelta = DecodeMotion( lPrevSpeeds.xy );

            if ( //lPrevSpeeds.z      != 1.0 ||
                 lSampleReproject.y <  0.0 || 
                 lSampleReproject.y >  1.0 || 
                 lSampleReproject.x <  0.0 || 
                 lSampleReproject.x >  1.0 )
            {
                lFragColVec4 = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer2Map ), IN( mTexCoordsVec2 ) );
                //lFragColVec4 = texture2D( lUniforms.mpCustomPerMesh->gBuffer2Map, lDeditheredCoordVec2 );
                //lFragColVec4 = texelFetch( lUniforms.mpCustomPerMesh->gBuffer2Map, ivec2(IN( mTexCoordsVec2 ) * lFrameSizeVec2 * 0.25), 0 );
                //lFragColVec4 = vec4(1.0, 0.0, 1.0, 1.0 );
            }
            else
            {
                //lFragColVec4 = texture2D( lUniforms.mpCustomPerMesh->gBuffer5Map, lSampleReproject );
                //lFragColVec4 = texelFetch( lUniforms.mpCustomPerMesh->gBuffer2Map, ivec2(IN( mTexCoordsVec2 ) * lFrameSizeVec2 * 0.25), 0 );
                //lFragColVec4 = texture2D( lUniforms.mpCustomPerMesh->gBuffer2Map, IN( mTexCoordsVec2 ) );
                lFragColVec4 = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer2Map ), lDeditheredCoordVec2 );

                vec4 lReprojectColVec4 = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer5Map ), lSampleReproject );

                //lFragColVec4 = lReprojectColVec4;

                //lFragColVec4 = mix( lReprojectColVec4, lFragColVec4, 0.025 );
                
                {
                    float rcpFrameOptZ = lUniforms.mpPerFrame->gFrameBufferSizeVec4.z * 0.25;
                    float rcpFrameOptW = lUniforms.mpPerFrame->gFrameBufferSizeVec4.w * 0.25;
                    //float rcpFrameOptZ = lUniforms.mpPerFrame->gFrameBufferSizeVec4.z * 1.0;
                    //float rcpFrameOptW = lUniforms.mpPerFrame->gFrameBufferSizeVec4.w * 1.0;
                    /*
                    if ( IN( mTexCoordsVec2 ).x < 0.5 )
                    {
                        rcpFrameOptZ = lUniforms.mpPerFrame->gFrameBufferSizeVec4.z * 0.125;
                        rcpFrameOptW = lUniforms.mpPerFrame->gFrameBufferSizeVec4.w * 0.125;
                    }
                    else
                    {
                        rcpFrameOptZ = lUniforms.mpPerFrame->gFrameBufferSizeVec4.z * 0.25;
                        rcpFrameOptW = lUniforms.mpPerFrame->gFrameBufferSizeVec4.w * 0.25;
                    }
                    */

                    vec4 cM = lFragColVec4;

                    vec3 cM3 = cM.rgb * cM.a;
                    //float wk = -TAA_GET_LUMINANCE( cM_jittered );
                    float wk = -GET_LUMINANCE( cM3 );

                    // four points, plus center, from the current screen
                    vec4 cL_M = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer2Map ), lDeditheredCoordVec2 + vec2( -rcpFrameOptZ, -rcpFrameOptW ) );
                    vec3 cL_M3 = cL_M.rgb * cL_M.a;
                    wk += GET_LUMINANCE( cL_M3 ) * 0.25;
                    vec4 cMax = cL_M;
                    vec4 cMin = cL_M;

                    vec4 cR_M = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer2Map ), lDeditheredCoordVec2 + vec2( rcpFrameOptZ, -rcpFrameOptW ) );
                    vec3 cR_M3 = cR_M.rgb * cR_M.a;
                    wk += GET_LUMINANCE( cR_M3 ) * 0.25;
                    cMax = max( cMax, cR_M );
                    cMin = min( cMin, cR_M );

                    vec4 cU_M = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer2Map ), lDeditheredCoordVec2 + vec2( -rcpFrameOptZ, rcpFrameOptW ) );
                    vec3 cU_M3 = cU_M.rgb * cU_M.a;
                    wk += GET_LUMINANCE( cU_M3 ) * 0.25;
                    cMax = max( cMax, cU_M );
                    cMin = min( cMin, cU_M );

                    vec4 cD_M = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer2Map ), lDeditheredCoordVec2 + vec2( rcpFrameOptZ, rcpFrameOptW ) );
                    vec3 cD_M3 = cD_M.rgb * cD_M.a;
                    wk += GET_LUMINANCE( cD_M3 ) * 0.25;
                    cMax = max( cMax, cD_M );
                    cMin = min( cMin, cD_M );

                    // edge detection filter, produces blend values for the front and back buffers
                    // bac buffer is more aggressive than the front buffer
                    //float gfTaaLowFreqConstant = 2.0f;
                    //float gfTaaHighFreqConstant = 4.0f;

                    float kl = ( lUniforms.mpPerFrame->gMBlurSettingsVec4.x );
                    float kh = ( lUniforms.mpPerFrame->gMBlurSettingsVec4.y );

                    // this math is from the Crytek SMAA filter
                    float blendAmount  = 1.0 - saturate( fastRcpNR0( mix( kl, kh, abs( wk ) * 5.0 ) ) );

                    cM = clip_aabb( cM, cMin, cMax );
                    cM = mix( cM, lReprojectColVec4, blendAmount );

                    //lFragColVec4 = vec4( cM, blendAmount );
                    lFragColVec4 = cM;
                    //lFragColVec4 = vec4( blendAmount, 0.0, 0.0, 1.0 );
                }

                
                //if ( abs(lReprojectColVec4.a - lFragColVec4.a) < 0.25 )
//                 if ( abs(lReprojectColVec4.a - lFragColVec4.a)          < 0.75 ||
//                      length( lReprojectColVec4.xyz - lFragColVec4.xyz ) < 0.75 )
//                 {
//                     lFragColVec4 = lReprojectColVec4;
//                 }
                
                //lFragColVec4 = vec4(1.0);
                //lFragColVec4 = texelFetch( lUniforms.mpCustomPerMesh->gBuffer5Map, ivec2( lSampleReproject * lFrameSizeVec2 ), 0 );
            }
        }
    }

    FRAGMENT_COLOUR = lFragColVec4;
    //FRAGMENT_COLOUR = vec4(1.0,0.0,0.0,1.0);
}

#endif


#ifdef D_CLOUD_COPY

//-----------------------------------------------------------------------------
//      Global Data



//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2,                  TEXCOORD0 )

DECLARE_END



//-----------------------------------------------------------------------------
//      Functions

FRAGMENT_MAIN_COLOUR_SRT
{
    vec4  lFragmentColourVec4 = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), IN( mTexCoordsVec2 ) );
    if ( lFragmentColourVec4.a == 0.0 )
    {
        discard;
    }
    float lfDepth = DecodeDepthFromColour( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gDepthMap ), IN( mTexCoordsVec2 ) ) );
    float lfCloudDepth = DecodeDepthFromColour( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gCloudDepthMap ), IN( mTexCoordsVec2 ) ) );

    if ( lfDepth < lfCloudDepth )
    {
        discard;
    }

    //vec4  lFragmentColourDebugVec4 = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer1Map ), IN( mTexCoordsVec2 ) );
    //vec4 lOutput = IN( mTexCoordsVec2 ).x > 0.0 ? lFragmentColourVec4 : lFragmentColourDebugVec4;
	vec4 lOutput = lFragmentColourVec4;
    //vec4 lOutput = IN( mTexCoordsVec2 ).x > 0.5 ? lFragmentColourVec4 : vec4( lFragmentColourDebugVec4.x, 0.0, 0.0, lFragmentColourDebugVec4.a );

    FRAGMENT_COLOUR = lOutput;
}

#endif

#ifdef D_CLOUD_SUN

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2,                  TEXCOORD0 )

DECLARE_END


float
ComputeScattering(
    in float costh,
    in float g)
{
	return (1.0 - g * g) / (4.0 * PI * pow(1.0 + g * g - 2.0 * g * costh, 3.0 / 2.0));
}

FRAGMENT_MAIN_COLOUR_SRT
{
    vec3 lLightPositionVec3 = lUniforms.mpCustomPerMesh->gSunPositionVec4.xyz/* - lUniforms.mpPerFrame->gViewPositionVec3*/;
    //vec3 lLightPositionVec3 = -lUniforms.mpPerFrame->gLightDirectionVec4.xyz;

    vec3 lViewDirection = GetScreenSpaceViewDir( IN( mTexCoordsVec2 ), lUniforms.mpCommonPerMesh->gInverseProjectionMat4, lUniforms.mpCommonPerMesh->gInverseViewMat4 );

    float lfLDN = dot( lViewDirection, normalize( lLightPositionVec3 ) );

    float lfAlpha = 1.0 - texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), IN( mTexCoordsVec2 ) ).a;

	float lfSunScatter = ComputeScattering( lfLDN, 0.6 );
    lfSunScatter *= lfAlpha;

	FRAGMENT_COLOUR = vec4( lfSunScatter, lfSunScatter, lfSunScatter, lfSunScatter );
    //FRAGMENT_COLOUR = lUniforms.mpCustomPerMesh->gSunPositionVec4;
}

#endif


#ifdef D_CLOUD_RAYS

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2,                  TEXCOORD0 )

DECLARE_END


#define NUM_GODRAY_SAMPLES 128



FRAGMENT_MAIN_COLOUR_SRT
{
    vec3 lLightPositionVec3 = lUniforms.mpPerFrame->gViewPositionVec3 + (lUniforms.mpCustomPerMesh->gSunPositionVec4.xyz * 10000.0);
    //vec3 lLightPositionVec3 =  lUniforms.mpPerFrame->gViewPositionVec3 + lUniforms.mpPerFrame->gLightDirectionVec4.xyz
    vec2 lTexCoord = IN( mTexCoordsVec2 ).xy;
    vec4 lScreenLightPosVec4 = MUL( lUniforms.mpCommonPerMesh->gViewProjectionMat4, vec4( lLightPositionVec3, 1.0 ) );

    lScreenLightPosVec4.xyz /= lScreenLightPosVec4.w;

    vec2 lSunTexCoords = ( lScreenLightPosVec4.xy * 0.5 ) + 0.5;
    //lSunTexCoords.y = 1.0 - lSunTexCoords.y;


    float lfDensity  = lUniforms.mpCustomPerMesh->gSunRayParams.x;
    float lfDecay    = lUniforms.mpCustomPerMesh->gSunRayParams.y;
    float lfExposure = lUniforms.mpCustomPerMesh->gSunRayParams.z;
    float lfWeight   = lUniforms.mpCustomPerMesh->gSunRayParams.w;


	if (lSunTexCoords.x <  0.0 ||
		lSunTexCoords.x >= 1.0 ||
		lSunTexCoords.y <  0.0 ||
		lSunTexCoords.y >= 1.0 )
    {
        discard;
    }

    // Calculate vector from pixel to light source in screen space.
    vec2 deltaTexCoord = ( lTexCoord - lSunTexCoords.xy );
    // Divide by number of samples and scale by control factor.
    deltaTexCoord *= 1.0 / NUM_GODRAY_SAMPLES * lfDensity;
    // Store initial sample.
    vec3 color = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), lTexCoord ).xyz;
    // Set up illumination decay factor.
    float lfIlluminationDecay = 1.0;
    // Evaluate summation from Equation 3 NUM_SAMPLES iterations.
    
    for ( int i = 0; i < NUM_GODRAY_SAMPLES; i++ )
    {
        // Step sample location along ray.
        lTexCoord -= deltaTexCoord;
        // Retrieve sample at new location.
        vec3 lSampleVec3 = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), lTexCoord ).xyz;
        // Apply sample attenuation scale/decay factors.
        lSampleVec3 *= lfIlluminationDecay * lfWeight;
        // Accumulate combined color.
        color += lSampleVec3;
        // Update exponential decay factor.
        lfIlluminationDecay *= lfDecay;
    }
    
    // Output final color with a further scale control factor.
    FRAGMENT_COLOUR = vec4( color * lfExposure, 1.0 );

    //FRAGMENT_COLOUR = vec4( vec3(length(lTexCoord - lSunTexCoords)), 1.0 );
    //FRAGMENT_COLOUR = vec4( 1.0 );

}

#endif

#ifdef D_CLOUD_RENDER2D
//-----------------------------------------------------------------------------
///
///     Fragment Main
///
///     @brief      Fragment Main
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------


DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2,                  TEXCOORD0 )
        
#if defined( D_CLOUD_RENDER2D )
    flat INPUT( vec4, mUpVec3_mfCameraHeight, TEXCOORD1 )
    flat INPUT( vec3, mCross1Vec3,            TEXCOORD2 )
    flat INPUT( vec3, mCross2Vec3,            TEXCOORD3 )
#endif

DECLARE_END

#ifdef D_PLATFORM_ORBIS
#pragma argument(maxvgprcount=28)
#endif

#include "Common/CommonNoise.shader.h"

vec3
DecodeGBufferPosition(
    in  vec2  lScreenPosVec2,
    in  vec4  lClipPlanes,
    in  mat4  lInverseProjectionMat4,
    in  mat4  lInverseViewMat4,
    in  vec3  lEyePositionVec3,
    SAMPLER2DARG( lBuffer1Map ) )
{
    vec4 lBuffer1_Vec4 = texture2D( lBuffer1Map, lScreenPosVec2 );

    float lfDepth = FastDenormaliseDepth( lClipPlanes, DecodeDepthFromColour( lBuffer1_Vec4 ) );
    vec3 lPositionVec3 = RecreatePositionFromDepth( lfDepth, lScreenPosVec2, lEyePositionVec3, lInverseProjectionMat4, lInverseViewMat4 );
    //vec3 lPositionVec3 = RecreateViewPositionFromDepth( lfDepth, lScreenPosVec2, lInverseProjectionMat4 );

    //return lPositionVec3 - lEyePositionVec3;
    return lPositionVec3;
}



FRAGMENT_MAIN_COLOUR_SRT
{
    vec4 lOutputVec4 = vec4( 0.0, 0.0, 0.0, 0.0 );
    vec2 lTexCoords = TEX_COORDS;


    vec3 lPositionVec3 = DecodeGBufferPosition(lTexCoords,
                                                lUniforms.mpPerFrame->gClipPlanesVec4,
                                                lUniforms.mpCommonPerMesh->gInverseProjectionMat4,
                                                lUniforms.mpCommonPerMesh->gInverseViewMat4,
                                                lUniforms.mpPerFrame->gViewPositionVec3,
                                                SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh, gBuffer1Map ) );

    // Needs to be planet relative for render offset stuff and for shadows anyway
    lPositionVec3 -= lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz;

    {
        vec3             lPlanetRelativeCamera;
        vec3             lRayVec3;
        float            lfPlanetRadius = lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w;

        float lfAtmosphereStartHeight = lUniforms.mpCustomPerMesh->gAtmosphereParamsVec4.y;
        float lfAtmosphereEndHeight   = lUniforms.mpCustomPerMesh->gAtmosphereParamsVec4.z;

        // Do the opposite of this M_CALCULATE_PLANET_RADIUS
		//float lfMaxDistance = sqrt( ( ( lfPlanetRadius + lfAtmosphereStartHeight) * lfAtmosphereStartHeight * 2.0 ) - ( lfAtmosphereStartHeight * lfAtmosphereStartHeight ) );
		float lfMaxDistance = M_CALCULATE_HORIZON_DISTANCE(lfPlanetRadius, (lfPlanetRadius + lfAtmosphereEndHeight)); 
        float lfCoverageScale = 1.0 / lfMaxDistance;
        lPlanetRelativeCamera = lUniforms.mpPerFrame->gViewPositionVec3 - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz;

        vec3 lNormalizedPlanetRelativeCamera = normalize( lPlanetRelativeCamera );
        vec3 lLightDirectionVec3 = ( lNormalizedPlanetRelativeCamera );//lUniforms.mpPerFrame->gLightDirectionVec4.xyz;
        //vec3 lLightDirectionVec3 = lUniforms.mpCustomPerMesh->gSunPositionVec4.xyz * 10000.0;
        vec3 lPlanetUpNormalVec3 = lNormalizedPlanetRelativeCamera;


        {
            vec3 lIntersectionNear;
            vec3 lIntersectionFar;
            float lfSphereDistance;
            float lfDistanceThroughClouds;
            float lfDeterminant;
            float lfHeightAbovePlanet = length( lPlanetRelativeCamera ) - lfPlanetRadius;

            //lfDistanceThroughClouds = saturate( (lfHeightAbovePlanet - lfAtmosphereStartHeight) / (lfAtmosphereEndHeight - lfAtmosphereStartHeight) );
            lfSphereDistance        = lfPlanetRadius;
            //lfSphereDistance       +=  mix(lfAtmosphereStartHeight, lfAtmosphereEndHeight, lfDistanceThroughClouds );
			lfSphereDistance       += lfAtmosphereStartHeight;
            lfDeterminant           = GetRayIntersectionPoint( lPositionVec3, lPositionVec3 + lLightDirectionVec3, lfSphereDistance, lIntersectionNear, lIntersectionFar );
			//lfDeterminant = GetRayIntersectionPoint(lPositionVec3, lPositionVec3 + lPlanetUpNormalVec3, lfSphereDistance, lIntersectionNear, lIntersectionFar);
            //lfDeterminant = GetRayIntersectionPoint( lRayStartVec3, lRayStartVec3 + lRayDirectionVec3, lfPlanetRadius + lfCloudHeightMin, lIntersectionNear, lIntersectionFar );

            lRayVec3 = lIntersectionFar;

            if ( lfDeterminant == 0.0 )
            {
                lOutputVec4 = vec4( 1.0, 1.0, 1.0, 1.0 );
            }
            else
            {                
                //lRayVec3 = MUL( lCoverageRotMat3, lRayVec3 );
				vec3 lBlendWeights;

				//lBlendWeights = pow(abs(lPlanetUpNormalVec3), vec3(32.0, 32.0, 32.0));
                lBlendWeights = pow(abs(normalize(lRayVec3)), vec3(32.0, 32.0, 32.0));
				lBlendWeights /= (lBlendWeights.x + lBlendWeights.y + lBlendWeights.z);

                vec3 wind_direction = GetWindDirection( lBlendWeights, lUniforms.mpCustomPerMesh->gWindOffset.xy);

                //vec3 wind_direction = lCustomPerMeshUniforms.gBaseOffset.xyz;
                float cloud_speed = lUniforms.mpCustomPerMesh->gAnimationParamsVec4.x * lUniforms.mpPerFrame->gfTime;

                //animate clouds in wind direction and add a small upward bias to the wind direction
                //lRayVec3+= (wind_direction + vec3(0.0, 0.1, 0.0)  ) * time * cloud_speed;
                lRayVec3 += wind_direction * cloud_speed;

                lRayVec3 *= lfCoverageScale;

				vec4 lCoverageVec4 = GetTriPlanarMap(lBlendWeights, lRayVec3, SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMesh, gCoverage2D));

                lCoverageVec4.r           *= lUniforms.mpCustomPerMesh->gAnimationParamsVec4.y;
				float lfCoverage           = (1.0 - smoothstep( 0.5, 1.0, lCoverageVec4.r )) * 0.5;
                float lfStratoCloudsHeight = lUniforms.mpCustomPerMesh->gAtmosphereParamsVec4.w;

                lfCoverage  = mix( lfCoverage, 1.0, smoothstep(lfStratoCloudsHeight * 4.0, lfStratoCloudsHeight * 5.0, lfHeightAbovePlanet) );
				lOutputVec4 = vec4(lfCoverage, lfCoverage, lfCoverage, lfCoverage);
            }
        }

    }
    
    FRAGMENT_COLOUR = saturate( lOutputVec4 );    
}
#endif
