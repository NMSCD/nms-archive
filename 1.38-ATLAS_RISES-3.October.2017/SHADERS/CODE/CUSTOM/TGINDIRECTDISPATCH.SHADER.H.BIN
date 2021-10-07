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
[NUM_THREADS(1, 1, 1)]

// Entry point for the compute shader with semantics that define the 
// variations of the ID to determine the thread instance invoked.
void main(uint3 dispatchThreadID : S_DISPATCH_THREAD_ID, cTkIndirectDispatchComputeUniforms* lpUniforms : S_SRT_DATA)
{
    //read and if necessary clamp and store the counter
    uint luCounter = ReadGDS_u32(lpUniforms->muGDSCounterAddress);
    if (lpUniforms->muPreClamp > 0)
    {
        luCounter = min(luCounter, lpUniforms->muPreClamp);
        WriteGDS_u32(lpUniforms->muGDSCounterAddress,luCounter);
    }

    //scale by the multiplier
    luCounter *= lpUniforms->muMultiply;

    //calculate number of thread groups based on counter and thread group size
    uint luThreadGroupSize = lpUniforms->muThreadGroupSize;
    uint luGroups = max((luCounter+luThreadGroupSize-1) / luThreadGroupSize,1);

    //fill out the args (note: must clamp to min of 1 group, as a 0 dispatch is invalid)
    lpUniforms->mauIndirectDispatchArgs[0] = max(luGroups,1);
    lpUniforms->mauIndirectDispatchArgs[1] = 1;
    lpUniforms->mauIndirectDispatchArgs[2] = 1;
}
  
