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

struct GlobalUniforms
{
	RegularBuffer<float>		mInputs;
	RW_RegularBuffer<float>		mOutputs;
};

struct PerDispatchUniforms
{
	int	    miOperation;  //0=add, 1=sub, 2=mul, 3=div
    int     miNumOperations;
    int     miFirstAInput;
    int     miFirstBInput;
    int     miFirstOutput;
};
struct Srt
{
	GlobalUniforms* mpGlobalUniforms;
	PerDispatchUniforms* mpPerDispatchUniforms;
};

// Declare thread count for each thread group.
[NUM_THREADS(64, 1, 1)]

// Entry point for the compute shader with semantics that define the 
// variations of the ID to determine the thread instance invoked.
void main(uint3 dispatchThreadID : S_DISPATCH_THREAD_ID, Srt* lUniforms : S_SRT_DATA)
{
    //get index and bail if gone to far
	uint idx = dispatchThreadID.x;
	if (idx >= lUniforms->mpPerDispatchUniforms->miNumOperations)
		return;

    //calculate buffer positions
	int lAIdx = idx + lUniforms->mpPerDispatchUniforms->miFirstAInput;    //add offset into buffer
	int lBIdx = idx + lUniforms->mpPerDispatchUniforms->miFirstBInput;    //add offset into buffer
	int lOutIdx = idx + lUniforms->mpPerDispatchUniforms->miFirstOutput;   //add offset into buffer

    //do the operation
    if (lUniforms->mpPerDispatchUniforms->miOperation == 0)
        lUniforms->mpGlobalUniforms->mOutputs[lOutIdx] = lUniforms->mpGlobalUniforms->mInputs[lAIdx]+lUniforms->mpGlobalUniforms->mInputs[lBIdx];
    else if (lUniforms->mpPerDispatchUniforms->miOperation == 1)
        lUniforms->mpGlobalUniforms->mOutputs[lOutIdx] = lUniforms->mpGlobalUniforms->mInputs[lAIdx]-lUniforms->mpGlobalUniforms->mInputs[lBIdx];
    else if (lUniforms->mpPerDispatchUniforms->miOperation == 2)
        lUniforms->mpGlobalUniforms->mOutputs[lOutIdx] = lUniforms->mpGlobalUniforms->mInputs[lAIdx]*lUniforms->mpGlobalUniforms->mInputs[lBIdx];
    else if (lUniforms->mpPerDispatchUniforms->miOperation == 3)
        lUniforms->mpGlobalUniforms->mOutputs[lOutIdx] = lUniforms->mpGlobalUniforms->mInputs[lAIdx]/lUniforms->mpGlobalUniforms->mInputs[lBIdx];
}
  