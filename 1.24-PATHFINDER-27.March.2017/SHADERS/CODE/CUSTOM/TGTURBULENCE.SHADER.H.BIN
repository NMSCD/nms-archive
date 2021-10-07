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

// Declare thread count for each thread group.
[NUM_THREADS(64, 1, 1)]

// Entry point for the compute shader with semantics that define the 
// variations of the ID to determine the thread instance invoked.
void main(uint3 dispatchThreadID : S_DISPATCH_THREAD_ID, cTkTurbulenceComputeUniforms* lpUniforms : S_SRT_DATA)
{
    //get index and bail if gone to far
	uint idx = dispatchThreadID.x;
	if (idx >= lpUniforms->miNumElements)
		return;

    float x = lpUniforms->mafX[idx];
    float y = lpUniforms->mafY[idx];
    float z = lpUniforms->mafZ[idx];

    vec3 res = TurbulencePositions(vec3(x,y,z),lpUniforms->mFrequency,lpUniforms->mPower,lpUniforms->miNumOctaves);

    lpUniforms->mafX[idx] = res.x;
    lpUniforms->mafY[idx] = res.y;
    lpUniforms->mafZ[idx] = res.z;
}
  