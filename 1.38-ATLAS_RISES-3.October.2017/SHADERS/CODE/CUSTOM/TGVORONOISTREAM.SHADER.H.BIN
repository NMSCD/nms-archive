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
void main(uint3 dispatchThreadID : S_DISPATCH_THREAD_ID, cTkVoronoiStreamComputeUniforms* lpUniforms : S_SRT_DATA)
{
    //get index and bail if gone to far
	uint idx = dispatchThreadID.x;
	if (idx >= lpUniforms->miNumElements)
		return;

    vec3 lPos = vec3(lpUniforms->mafX[idx], lpUniforms->mafY[idx], lpUniforms->mafZ[idx]);

    uint64 liId;
    float lfCenterDistance;
    vec3 lCenterPosition;
    vec3 lFloorPosition;
    vec3 lNormal;

    #ifdef TK_TGEN_VORONOI_DEBUG
    sVoronoiDebugOutput lDbg;
    VoronoiDistanceOnCube(lPos, lpUniforms->miSeed, lpUniforms->mfRadius, lpUniforms->mfScaleFactor, liId, lfCenterDistance, lCenterPosition, lFloorPosition, lNormal, lDbg);
    lpUniforms->maDebugResults[idx] = lDbg;
    #else
    VoronoiDistanceOnCube(lPos, lpUniforms->miSeed, lpUniforms->mfRadius, lpUniforms->mfScaleFactor, liId, lfCenterDistance, lCenterPosition, lFloorPosition, lNormal);
    #endif

    lpUniforms->maiID[idx] = liId;
    lpUniforms->mafCenterDistance[idx] = lfCenterDistance;
    lpUniforms->maCenterX[idx] = lCenterPosition.x;
    lpUniforms->maCenterY[idx] = lCenterPosition.y;
    lpUniforms->maCenterZ[idx] = lCenterPosition.z;
    lpUniforms->maFloorPositionX[idx] = lFloorPosition.x;
    lpUniforms->maFloorPositionY[idx] = lFloorPosition.y;
    lpUniforms->maFloorPositionZ[idx] = lFloorPosition.z;
    lpUniforms->maNormalX[idx] = lNormal.x;
    lpUniforms->maNormalY[idx] = lNormal.y;
    lpUniforms->maNormalZ[idx] = lNormal.z;  
}
  