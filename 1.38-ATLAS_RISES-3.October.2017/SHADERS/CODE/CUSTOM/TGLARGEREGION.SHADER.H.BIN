////////////////////////////////////////////////////////////////////////////////
///
///     @file       ComputeNoise.h
///     @author     ccummings
///     @date       
///
///     @brief      Basic shader to execute a series of Noise3D samples
///
///     Copyright (c) 2016 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "Common/Defines.shader.h"
#include "Custom/TerrainGenCommon.h"

#pragma argument(nofastmath)

// Declare thread count for each thread group.
[NUM_THREADS(64, 1, 1)]

// Entry point for the compute shader with semantics that define the 
// variations of the ID to determine the thread instance invoked.
void main(uint3 dispatchThreadID : S_DISPATCH_THREAD_ID, cTkLargeNoiseComputeUniforms* lpUniforms : S_SRT_DATA)
{
    //get index and bail if gone to far
	uint idx = dispatchThreadID.x;
	if (idx >= lpUniforms->miNumElements)
		return;

    vec3 lTurbPos = TurbulencePositions(vec3(lpUniforms->mafPositionX[idx], lpUniforms->mafPositionY[idx], lpUniforms->mafPositionZ[idx]), 1.0f / 20.0f, 5.0f, 2);

    float lfDivisionFactor = lpUniforms->mfRegionVoronoiPointDivisions * 0.5f;

    sVoronoiResults lResults;
    VoronoiDistanceOnCube(lTurbPos, lpUniforms->miRegionVoronoiSectorSeed, lpUniforms->mfRegionPlanetRadius, lfDivisionFactor, lResults.miID, lResults.mfCenterDistance, lResults.mCenterPosition, lResults.mFloorPosition, lResults.mNormal);

    lpUniforms->maVoronoiResults[idx] = lResults;
}
  