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
void main(uint3 dispatchThreadID : S_DISPATCH_THREAD_ID, cTkGetTrianglePositionsComputeUniforms* lpUniforms : S_SRT_DATA)
{
    //get index and bail if gone to far
	uint idx = dispatchThreadID.x;
    uint luNumElements = ReadGDS_u32(lpUniforms->muNumElementsGDSAddress);
	if (idx >= luNumElements)
		return;

    //use pos map to extract triangle and point index
    uint luPosMap       = lpUniforms->maPosToTriangleMap[idx];
    uint luTriangleIdx  = luPosMap >> 16;
    uint luPointIdx     = luPosMap & 0xffff;

    //get the triangle and each vertex
    sGetTriangleCountOutput lTriangle   = lpUniforms->maTriangles[luTriangleIdx];
    sTerrainVertexUnpacked lVert0       = lpUniforms->maVertices[luTriangleIdx*3+0];
    sTerrainVertexUnpacked lVert1       = lpUniforms->maVertices[luTriangleIdx*3+1];
    sTerrainVertexUnpacked lVert2       = lpUniforms->maVertices[luTriangleIdx*3+2];

    //extract the required precomputed data to generate the point positions
    vec3 lPos0                  = lVert0.mPosition;
    vec3 lPos1                  = lVert1.mPosition;
    vec3 lPos2                  = lVert2.mPosition;
    vec3 lNorm0                 = lVert0.mSmoothNormal;
    vec3 lNorm1                 = lVert1.mSmoothNormal;
    vec3 lNorm2                 = lVert2.mSmoothNormal;
    vec3 lPlanetPosition        = lpUniforms->mPlanetPosition;
    uint luTriangleSeed         = lTriangle.muSeed;
    float lfRatio               = lTriangle.mfRatio;
    uint luMaterial             = lTriangle.muMaterial;
    uint luSecondaryMaterial    = lTriangle.muSecondaryMaterial;

    //use the shared GPU 
    GenPositionForTriangle(
        lPos0,lPos1,lPos2, 
        lNorm0,lNorm1,lNorm2, 
        lfRatio, luMaterial,luSecondaryMaterial, lPlanetPosition, luTriangleSeed, luPointIdx, 
        lpUniforms->maPositionOutput[idx], lpUniforms->maPositionSeedOutput[idx]);
}
  