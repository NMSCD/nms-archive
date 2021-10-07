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

/*
//Performs histo-pyramid like sum to take a 'count' of entries for each lane, and calculate
//the offset in a buffer it should write entries to (sum of the counts of lower lanes), and
//the total (sum of the counts of all lanes)
thread_group_memory uint R00[64];
thread_group_memory uint R01[64];
thread_group_memory uint R02[64];
thread_group_memory uint R03[64];
thread_group_memory uint R04[64];
thread_group_memory uint R05[64];
thread_group_memory uint R06[64];
#define MEMSYNC() ThreadGroupMemoryBarrierSync()

void LaneSum(uint count, OUTPARAM(uint) outoffset, OUTPARAM(uint) outtotal)
{
    uint lane = GetThreadIdX();
    outoffset = 0;
    R00[lane] = count;                            
    MEMSYNC();      
    outoffset += (lane & 0x01) ? R00[(lane&~0x00)-1] : 0;

    R01[lane] = R00[lane] + R00[lane&~0x01];      
    MEMSYNC();      
    outoffset += (lane & 0x02) ? R01[(lane&~0x01)-1] : 0;

    R02[lane] = R01[lane] + R01[lane&~0x02];      
    MEMSYNC();      
    outoffset += (lane & 0x04) ? R02[(lane&~0x03)-1] : 0;

    R03[lane] = R02[lane] + R02[lane&~0x04];      
    MEMSYNC();      
    outoffset += (lane & 0x08) ? R03[(lane&~0x07)-1] : 0;

    R04[lane] = R03[lane] + R03[lane&~0x08];      
    MEMSYNC();      
    outoffset += (lane & 0x10) ? R04[(lane&~0x0F)-1] : 0;

    R05[lane] = R04[lane] + R04[lane&~0x10];      
    MEMSYNC();      
    outoffset += (lane & 0x20) ? R05[(lane&~0x1F)-1] : 0;

    R06[lane] = R05[lane] + R05[lane&~0x20];      
    MEMSYNC();      

    outtotal = R06[0x3F];
}
*/

//Performs histo-pyramid like sum to take a 'count' of entries for each lane, and calculate
//the offset in a buffer it should write entries to (sum of the counts of lower lanes), and
//the total (sum of the counts of all lanes). This is just an optimised version of the above
//that recycles the LDS buffer to minimize allocation size.
thread_group_memory uint LNS[64];
void LaneSum(uint count, OUTPARAM(uint) outoffset, OUTPARAM(uint) outtotal)
{
    uint lane = GetThreadIdX();
    outoffset = 0;
    LNS[lane] = count;                            
    TGMemBarrier();      
    outoffset += (lane & 0x01) ? LNS[(lane&~0x00)-1] : 0;
    LNS[lane] += LNS[lane&~0x01];      
    TGMemBarrier();      
    outoffset += (lane & 0x02) ? LNS[(lane&~0x01)-1] : 0;
    LNS[lane] += LNS[lane&~0x02];      
    TGMemBarrier();      
    outoffset += (lane & 0x04) ? LNS[(lane&~0x03)-1] : 0;
    LNS[lane] += LNS[lane&~0x04];      
    TGMemBarrier();      
    outoffset += (lane & 0x08) ? LNS[(lane&~0x07)-1] : 0;
    LNS[lane] += LNS[lane&~0x08];      
    TGMemBarrier();      
    outoffset += (lane & 0x10) ? LNS[(lane&~0x0F)-1] : 0;
    LNS[lane] += LNS[lane&~0x10];      
    TGMemBarrier();      
    outoffset += (lane & 0x20) ? LNS[(lane&~0x1F)-1] : 0;
    LNS[lane] += LNS[lane&~0x20];      
    TGMemBarrier();      
    outtotal = LNS[0x3F];
}

// Declare thread count for each thread group.
[NUM_THREADS(64, 1, 1)]


// Entry point for the compute shader with semantics that define the 
// variations of the ID to determine the thread instance invoked.
void main(uint3 dispatchThreadID : S_DISPATCH_THREAD_ID, cTkGetTriangleCountComputeUniforms* lpUniforms : S_SRT_DATA)
{
    //get index and bail if gone to far
	uint idx = dispatchThreadID.x;

    //if valid element, calculate number of points from the pre-transformed vertices and spawn desnity
    uint liNumPoints = 0;
    if (idx < lpUniforms->miNumElements)
    {
        //get verts
        sTerrainVertexUnpacked lVert0 = lpUniforms->maVertices[idx*3];
        sTerrainVertexUnpacked lVert1 = lpUniforms->maVertices[idx*3+1];
        sTerrainVertexUnpacked lVert2 = lpUniforms->maVertices[idx*3+2];

        vec3 lPos0 = lVert0.mPosition;
        vec3 lPos1 = lVert1.mPosition;
        vec3 lPos2 = lVert2.mPosition;

        //calc edges
        vec3 lEdge1 = ( lPos1 - lPos0 );
        vec3 lEdge2 = ( lPos2 - lPos0 );

        //calc area and use to get number of points
        float lfArea = length(0.5f * cross( lEdge1, lEdge2 ));
        liNumPoints = (uint)ceil( lfArea * lpUniforms->mfSpawnDensity );

        //calc and store extra triangle data
        uint luSeed = GenTriangleSeed(lPos0,lPos1,lPos2);
        float lfRatio;
        uint luMaterial,luSecondaryMaterial;
        GenRatioAndMaterialsForTile(lVert0.mfTileRatio,lVert1.mfTileRatio,lVert2.mfTileRatio,lVert0.mfTileMaterialA,lVert0.mfTileMaterialB,lfRatio,luMaterial,luSecondaryMaterial);
        lpUniforms->maTriangleOutput[idx].muSeed = luSeed;
        lpUniforms->maTriangleOutput[idx].mfRatio = lfRatio;
        lpUniforms->maTriangleOutput[idx].muMaterial = luMaterial;
        lpUniforms->maTriangleOutput[idx].muSecondaryMaterial = luSecondaryMaterial;

    }
    
    //use lane some algorithm to get:
    //- offset into buffer from start of points for this wave front (effectively how many points will be generated by lower idx threads in this wave front)
    //- total points generated by this wave front
    uint liOffset,liTotal;
    LaneSum(liNumPoints,liOffset,liTotal);

    //use ordered append unit to add the total points from this wave front to the overall position in the triangle buffer
    uint liGlobalCounter = OrderedCount( lpUniforms->miAppendCounterAddress, liTotal, K_ORDERED_WAVE_RELEASE | K_ORDERED_WAVE_DONE );

    //if valid element, output results
    if (idx < lpUniforms->miNumElements)
    {
        //position start is global counter plus our offset
        uint luPosStart = liGlobalCounter+liOffset;
        uint luPosEnd = luPosStart+liNumPoints;

        //avoid overflow of positions
        if (luPosEnd > lpUniforms->muPosBufferSize)
        {
            luPosEnd = lpUniforms->muPosBufferSize;
            if (luPosStart > luPosEnd)
                luPosStart = luPosEnd;
            liNumPoints = luPosEnd-luPosStart;
        }

        //write position data
        lpUniforms->maTriangleOutput[idx].muPosStart = luPosStart;
        lpUniforms->maTriangleOutput[idx].muPosCount = liNumPoints;

        //setup the point indices here too
        for (uint i = 0; i < liNumPoints; i++)
        {
            uint luPosId = (idx << 16) | i;
            lpUniforms->maPosToTriangleMapOutput[luPosStart+i] = luPosId;
        }
    }
}
  
