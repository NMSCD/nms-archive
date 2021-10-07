//-----------------------------------------------------------------------------
//Terrain gen core code shared between CPU and GPU
//This can be included from headers in c++ code
//-----------------------------------------------------------------------------

#ifndef _TGENSHARED_LARGENOISE_H_
#define _TGENSHARED_LARGENOISE_H_
#include "Custom/TerrainGenShared_Core.h"
#include "Custom/TerrainGenShared_NoiseUniforms.h"
#include "Custom/SharedBegin.inl"

struct cTkLargeNoiseComputeUniforms
{
    ROBUFF(float)           mafPositionX;
    ROBUFF(float)           mafPositionY;
    ROBUFF(float)           mafPositionZ;
    RWBUFF(sVoronoiResults) maVoronoiResults;
    int                     miNumElements;
    float                   mfRegionVoronoiPointDivisions;
    int                     miRegionVoronoiSectorSeed;
    float                   mfRegionPlanetRadius;
};

#include "Custom/SharedEnd.inl"
#endif


