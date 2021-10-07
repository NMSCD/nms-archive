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
#include "Custom/ComputeNoiseCommon.h"

// Declare thread count for each thread group.
[NUM_THREADS(64, 1, 1)]

// Entry point for the compute shader with semantics that define the 
// variations of the ID to determine the thread instance invoked.
void main(uint3 dispatchThreadID : S_DISPATCH_THREAD_ID, Srt lUniforms : S_SRT_DATA)
{
	uint idx = dispatchThreadID.x;
	if (idx >= lUniforms.mpPerDispatchUniforms->miNumRequests)
		return;

	idx += lUniforms.mpPerDispatchUniforms->miFirstRequest;

	vec3 request = lUniforms.mpGlobalUniforms->mRequests[idx];

	lUniforms.mpGlobalUniforms->mNoiseResults[idx] = Noise3d(request.x, request.y, request.z);
}
  