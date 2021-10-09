//-----------------------------------------------------------------------------
//Terrain gen core code shared between CPU and GPU
//This can be included from headers in c++ code
//-----------------------------------------------------------------------------

#ifndef _TGENSHARED_NOISEUNIFORMS_H_
#define _TGENSHARED_NOISEUNIFORMS_H_
#include "Custom/TerrainGenShared_Noise.h"
#include "Custom/SharedBegin.inl"

//-----------------------------------------------------------------------------
//uniforms for uber noise layer generation
//-----------------------------------------------------------------------------
struct cTkGenerateUberNoiseLayer3DComputeUniforms
{
	ROBUFF(float) mafX;
	ROBUFF(float) mafY;
	ROBUFF(float) mafZ;

	RWBUFF(float) mafNoise;
	RWBUFF(vec4) mavDebugOutput;

    vec3    mSeedOffset;
    int     miNumElements;
    int     miSeed;
    int     miOctaves;
    float   mfFeatureScale;
    float   mfSharpToRoundFeatures;
    float   mfAmplifyFeatures;
    float   mfPerturbFeatures;
    float   mfAltitudeErosion;
    float   mfRidgeErosion;
    float   mfSlopeErosion;
    float   mfLacunarity;
    float   mfGain;
};

//-----------------------------------------------------------------------------
//uniforms for smooth noise layer generation
//-----------------------------------------------------------------------------
struct cTkGenerateSmoothNoiseLayer3DComputeUniforms
{
	ROBUFF(float) mafX;
	ROBUFF(float) mafY;
	ROBUFF(float) mafZ;

	RWBUFF(float) mafNoise;
	RWBUFF(vec4) mavDebugOutput;

    vec3    mSeedOffset;
    int     miNumElements;
    int     miSeed;
    int     miOctaves;
    float   mfFeatureScale;
};

//-----------------------------------------------------------------------------
//uniforms for region noise generation
struct cTkGenerateRegionNoiseComputeUniforms
{
	ROBUFF(float) mafX;
	ROBUFF(float) mafY;
	ROBUFF(float) mafZ;

	RWBUFF(float) mafNoise;
	RWBUFF(vec4) mavDebugOutput;

    vec3    mSeedOffset;
    int     miNumElements;
    float   mfFeatureScale;
    float   mfRegionRatio;
    float   mfOneMinusSqrtRegionRatio;
    float   mfOneOverSqrtRegionRatio;
};

//-----------------------------------------------------------------------------
//uniforms for shader that combines the uber and region noise at the end
//of uber noise generation
//-----------------------------------------------------------------------------
struct cTkUber3DCombineComputeUniforms
{
	ROBUFF(float) mafUberNoise;
	ROBUFF(float) mafRegionNoise;
	RWBUFF(float) mafNoise;

    int     miNumElements;
    float   mfOneMinusSqrtRegionRatio;
    float   mfOneOverSqrtRegionRatio;
    float   mfHeight;
};

//-----------------------------------------------------------------------------
// uniforms for turbulence noise shader. this modifies the RW X/Y/Z streams
//-----------------------------------------------------------------------------
struct cTkTurbulenceComputeUniforms
{
	RWBUFF(float) mafX;
	RWBUFF(float) mafY;
	RWBUFF(float) mafZ;

    vec3    mFrequency; 
    vec3    mPower; 
    int     miNumOctaves;
    int     miNumElements;
};

//-----------------------------------------------------------------------------
//io structures and uniforms for voronoi noise
//-----------------------------------------------------------------------------
struct sVoronoiRequest
{
    vec3        mSpherePosition;
    int         miSeed;
    float       mfRadius;
    float       mfScaleFactor;

};
struct sVoronoiResults
{
    uint64      miID;
    float       mfCenterDistance;
    vec3        mCenterPosition;
    vec3        mFloorPosition;
    vec3        mNormal;
};

#ifdef TK_TGEN_VORONOI_DEBUG
struct sVoronoiDebugOutput
{
    vec3        mSpherePosition;
    vec3        mNormPosition;
    vec3        mToCubePosition;
    vec3        mScaledPosition;
    vec3        mFloorPosition;
    vec3        mFracPosition;
    uint        muSeed;
    vec3        mRandInputs[9];
    vec3        mRandOffsets[9];
    vec3        mPrePosIdx[9];
    vec3        mPostPosIdx[9];
    vec3        mCenterPositions[9];
    float       mFaceX[9];
    float       mFaceY[9];
    int         mFaceIdx[9];
    float       mfDistances[9];
    uint64      mOutId;
    vec3        mOutCentre;
    float       mfOutCentreDistance;
};
#endif

struct cTkVoronoiComputeUniforms
{
	ROBUFF(sVoronoiRequest)     maRequests;
	RWBUFF(sVoronoiResults)     maResults;
    #ifdef TK_TGEN_VORONOI_DEBUG
	RWBUFF(sVoronoiDebugOutput) maDebugResults;
    #endif
    int                         miNumElements;
};
struct cTkVoronoiPosComputeUniforms
{
	ROBUFF(vec3)                maRequests;
	RWBUFF(sVoronoiResults)     maResults;
    #ifdef TK_TGEN_VORONOI_DEBUG
	RWBUFF(sVoronoiDebugOutput) maDebugResults;
    #endif
    int                         miSeed;
    float                       mfRadius;
    float                       mfScaleFactor;
    int                         miNumElements;
};
struct cTkVoronoiStreamComputeUniforms
{
	ROBUFF(float)         mafX;
	ROBUFF(float)         mafY;
	ROBUFF(float)         mafZ;

    RWBUFF(uint64)        maiID;
    RWBUFF(float)         mafCenterDistance;
	RWBUFF(float)         maCenterX;
	RWBUFF(float)         maCenterY;
	RWBUFF(float)         maCenterZ;
	RWBUFF(float)         maFloorPositionX;
	RWBUFF(float)         maFloorPositionY;
	RWBUFF(float)         maFloorPositionZ;
	RWBUFF(float)         maNormalX;
	RWBUFF(float)         maNormalY;
	RWBUFF(float)         maNormalZ;

    #ifdef TK_TGEN_VORONOI_DEBUG
	RWBUFF(sVoronoiDebugOutput) maDebugResults;
    #endif
    int                         miSeed;
    float                       mfRadius;
    float                       mfScaleFactor;
    int                         miNumElements;
};

#include "Custom/SharedEnd.inl"
#endif
