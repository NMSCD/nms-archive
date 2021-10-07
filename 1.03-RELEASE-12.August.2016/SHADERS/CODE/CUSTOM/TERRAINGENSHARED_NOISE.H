//-----------------------------------------------------------------------------
//Terrain gen core code shared between CPU and GPU
//This can be included from headers in c++ code
//-----------------------------------------------------------------------------

#ifndef _TGENSHARED_NOISE_H_
#define _TGENSHARED_NOISE_H_
#include "Custom/TerrainGenShared_Core.h"
#include "Custom/SharedBegin.inl"

float
Noise3d(float x, float y, float z);

float
Noise3d(float x, float y, float z, OUTPARAM(float) dnoise_dx, OUTPARAM(float) dnoise_dy, OUTPARAM(float) dnoise_dz);

float
ScaleNoise(
    float lfNoise,
    float lfAmount );

float
ScaleNoise(
    float lfNoise,
    float lfOneMinusAmount,
    float lfOneOverAmount);

float
UberNoise(
    vec3 lPosition,
    int   liOctaves,
    float lfPerturbFeatures,      
    float lfSharpToRoundFeatures, 
    float lfAltitudeErosion,      
    float lfRidgeErosion,         
    float lfSlopeErosion,         
    float lfLacunarity,           
    float lfGain );

float 
OctaveNoise( 
    vec3 lPosition,
    int liNumOctaves );

float 
SmoothNoise(
    vec3 lPosition, 
    vec3 lSeedOffset, 
    float lfFeatureScale, 
    int liNumOctaves);

vec3 
TurbulencePositions( 
    vec3 lPos,
    vec3 lFrequency, 
    vec3 lPower, 
    int liNumOctaves );

float
GenerateRegionNoise(     
    vec3        lPosition,
    float       lfRegionSize,
    float       lfRegionScale,
    float       lfRegionRatio,
    vec3        lSeedOffset,
	int         lbIsLegacy );

float
GenerateRegionNoise(
    vec3        lPosition,
    float       lfRegionSize,
    float       lfRegionScale,
    float       lfRegionRatio,
    int         liSeed,
    int         lbIsLegacy);

float
GenerateNoise2D(
    vec3        lPosition,
    float       lfRegionSize,
    float       lfRegionScale,
    float       lfRegionRatio,
    vec3        lSeedOffset,
	int         lbIsLegacy );

float
GenerateNoise2D(
    vec3        lPosition,
    float       lfRegionSize,
    float       lfRegionScale,
    float       lfRegionRatio,
    int         liSeed,
    int         lbIsLegacy);

void
VoronoiDistanceOnCube(
    vec3                    lSpherePosition,
    int                     liSeed,
    float                   lfRadius,
    float                   lfScaleFactor,
    OUTPARAM(uint64)        liID,
    OUTPARAM(float)         lfCenterDistance,
    OUTPARAM(vec3)          lCenterPosition,
    OUTPARAM(vec3)          lFacePosition,
    OUTPARAM(vec3)          lNormal
    #ifdef TK_TGEN_VORONOI_DEBUG
    ,OUTPARAM(sVoronoiDebugOutput)  lDebug
    #endif
    );

vec3
VoronoiRandom3f(
    vec3 lPosition,
    uint liSeed);

uint64
VoronoiID(
    uint    luSeed,
    vec3    lRandomOffset );



#include "Custom/SharedEnd.inl"
#endif
