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
void main(uint3 dispatchThreadID : S_DISPATCH_THREAD_ID, cTkUber3DCombineComputeUniforms* lpUniforms : S_SRT_DATA)
{
    //get index and bail if gone to far
    uint idx = dispatchThreadID.x;
    if (idx >= lpUniforms->miNumElements)
        return;

    float lfOneMinusSqrtRatio    = lpUniforms->mfOneMinusSqrtRegionRatio;
    float lfOneOverSqrtRatio     = lpUniforms->mfOneOverSqrtRegionRatio;
    float lfHeight               = lpUniforms->mfHeight;

    float lfNoise           = lpUniforms->mafUberNoise[idx];
    lfNoise                 = saturate( lfNoise );
    lfNoise                 = ScaleNoise(lfNoise,lfOneMinusSqrtRatio,lfOneOverSqrtRatio);
    lfNoise                *= lpUniforms->mafRegionNoise[idx];

    // plateau noise
    if( lpUniforms->mfPlateauStratas > 0.0f )
    {
        vec3 lPosition = vec3( lpUniforms->mafX[ idx ], lpUniforms->mafY[ idx ], lpUniforms->mafZ[ idx ] );

        float lfPlateauNoise = 0.0f;

        if( lpUniforms->mfPlateauRegionSize != 0.0f )
        {
            lfPlateauNoise = SmoothNoise(
                lPosition, 
                lpUniforms->mSeedOffset,
                lpUniforms->mfPlateauRegionSize,
                1 );
        }

        float lfFloor = floor( lfNoise * lpUniforms->mfPlateauStratas );
        float lfFract = frac( lfNoise * lpUniforms->mfPlateauStratas );

        lfFract = Gain( lfFract, lpUniforms->miPlateauSharpness );

        lfNoise = lerp( ( lfFloor + lfFract ) * ( 1.0f / lpUniforms->mfPlateauStratas ), lfNoise, lfPlateauNoise );
    }

    lfNoise                *= lpUniforms->mfHeight;

    lfNoise                 = ldexp(round(ldexp(lfNoise,10)),-10);

    lpUniforms->mafNoise[idx] = lfNoise;
}
  