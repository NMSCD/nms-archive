//-----------------------------------------------------------------------------
//Terrain gen core code shared between CPU and GPU
//This can be included from headers in c++ code
//-----------------------------------------------------------------------------

#ifndef _TGENSHARED_REGIONDECORATOR_H_
#define _TGENSHARED_REGIONDECORATOR_H_
#include "Custom/TerrainGenShared_Core.h"
#include "Custom/SharedBegin.inl"


//-----------------------------------------------------------------------------
//Basic terrain vertex types, and uniforms for the very simple unpack test
//shader
//-----------------------------------------------------------------------------

struct sTerrainVertexPacked
{
    sHalfVector4    mPosition;
    sHalfVector4    mTile;
    sHalfVector4    mNormal;
};
struct sTerrainVertexUnpacked
{
    vec3    mPosition;
    float   mfTileRatio;
    vec3    mSmoothNormal;
    float   mfTileMaterialA;
    float   mfTileMaterialB;
};
struct cTkUnpackTerrainVertexComputeUniforms
{
	ROBUFF(sTerrainVertexPacked)    maPacked;
	RWBUFF(sTerrainVertexUnpacked)  maUnpacked;
    mat4x4                          mPositionTransform;
    int                             miNumElements;
};

//-----------------------------------------------------------------------------
//Uniforms for the 'GetTriangleCount' shader used by region decorator. 
//-----------------------------------------------------------------------------
struct sGetTriangleCountOutput
{
    uint muPosStart;
    uint muPosCount;
    float mfRatio;
    uint muSeed;
    uint muMaterial;
    uint muSecondaryMaterial;
};
struct cTkGetTriangleCountComputeUniforms
{
	ROBUFF(sTerrainVertexUnpacked)  maVertices;
	RWBUFF(sGetTriangleCountOutput) maTriangleOutput;
    RWBUFF(uint)                    maPosToTriangleMapOutput; //buffer per output position that maps a pos to a triangle with [b0:16=offset,b17:31=tri]
    int                             miAppendCounterAddress; //address in GDS of append buffer counter
    float                           mfSpawnDensity;
    int                             miNumElements;
    uint                            muPosBufferSize;
};

//-----------------------------------------------------------------------------
//Per position region height result used by region decorator
//-----------------------------------------------------------------------------
//NOTE: TkRegionHeightTest.h contains a mirrored copy of this structure
//that the CPU uses. They're kept separate as GPU doesn't support as much stuff
//but must be kept in sync if you change one.
struct sRegionHeightResult
{
public:
    vec3    mPosition;
    float   mPad0;
    vec3    mNormal;
    float   mPad1;
    uint    mMaterial;
    uint    mSecondaryMaterial;
    float   mfSlopeValue;
    float   mPad3;
};

//-----------------------------------------------------------------------------
//Uniforms for the 'GetTrianglePositions' shader used by region decorator
//-----------------------------------------------------------------------------
struct cTkGetTrianglePositionsComputeUniforms
{
	ROBUFF(sTerrainVertexUnpacked)  maVertices;
	ROBUFF(sGetTriangleCountOutput) maTriangles;
    ROBUFF(uint)                    maPosToTriangleMap; //buffer per output position that maps a pos to a triangle with [b0:16=offset,b17:31=tri]
	RWBUFF(sRegionHeightResult)     maPositionOutput;
	RWBUFF(uint)                    maPositionSeedOutput;
    vec3                            mPlanetPosition;
    uint                            muNumElementsGDSAddress; //num elements generated dynamically and stored in GDS by prior compute job
};

//-----------------------------------------------------------------------------
//Structures for use by region decorate add
//-----------------------------------------------------------------------------
STATIC_CONST uint kuRegionDecoratorMaxObjectTypes = 256;
STATIC_CONST uint kuRegionDecoratorNumPlacementPriorities = 3;
struct sObjectTypePriorityMapEntry
{
    uint muStart;
    uint muCount;
};
struct sObjectTypeInfo
{
    float mfMaxDensity;
};

struct sObjectTypePriorityMap
{
    sObjectTypePriorityMapEntry maTypeMap[kuRegionDecoratorMaxObjectTypes];
    sObjectTypeInfo maTypeInfo[kuRegionDecoratorMaxObjectTypes]; //'technically' not priority info, but handy to have it here!
    uint maiPriorityNums[kuRegionDecoratorNumPlacementPriorities];
    uint maiPriorityStarts[kuRegionDecoratorNumPlacementPriorities];
};
struct sAddObjectPosIndex
{
    //note: GPU doesn't support 16 bit integers which is annoying, so we provide them in c++
    //but just use a uint in PSSL
    #ifdef __cplusplus
    union
    {
        sUInt32 muPosIndex_ObjectType; //[0:15=pos, 16:31=type]
        struct
        {
            sUInt16 muPosIndex;
            sUInt16 muObjectType;
        };
    };
    #else
    uint muPosIndex_ObjectType; //[0:15=pos, 16:31=type]
    #endif
    float   mfDensity;
};
struct cTkBuildObjectTypesPerPositionComputeUniforms
{
    sObjectTypePriorityMap mPriorityMap;
    ROBUFF(uint) mauPositionSeeds;
    RWBUFF(sAddObjectPosIndex) maObjectTypePerPositionBuffer;
    uint miNumObjectTypes;
    uint miObjectTypePerPositionBufferSize;
    uint muNumPositionsGDSAddress;          //generated dynamically and stored in GDS by prior compute job
    uint muNumOutputPositionsGDSAddress;    //output counter address
};

//-----------------------------------------------------------------------------
//Vertex unpacking
//-----------------------------------------------------------------------------

TKINLINE void 
UnpackTerrainVertex(
    INPARAM(sTerrainVertexPacked) lPacked, 
    OUTPARAM(sTerrainVertexUnpacked) lUnpacked)
{
    vec4 lPos,lTile,lNormal;
    UnpackHalfVector4(lPacked.mPosition,lPos);
    UnpackHalfVector4(lPacked.mTile,lTile);
    UnpackHalfVector4(lPacked.mNormal,lNormal);

    lUnpacked.mPosition = lPos.xyz;
    lUnpacked.mSmoothNormal = OctahedronNormalDecode(lNormal.zw);
    lUnpacked.mfTileMaterialA = lTile.x;
    lUnpacked.mfTileMaterialB = lTile.y;
    lUnpacked.mfTileRatio = lTile.z;
}

//-----------------------------------------------------------------------------
//Used for position generation code in region decorator
//-----------------------------------------------------------------------------

TKINLINE uint
GenTriangleSeed(
   vec3 lPos0, vec3 lPos1, vec3 lPos2)
{
    vec3 lTriangleCentroid = (lPos0 + lPos1 + lPos2) * (1.0f/3.0f);
    return HashMixUInt32(asuint(lTriangleCentroid.x) + asuint(lTriangleCentroid.y) * 4391 + asuint(lTriangleCentroid.z) * 59273);
}

TKINLINE void
GenRatioAndMaterialsForTile(
    float lfTile0Ratio, float lfTile1Ratio, float lfTile2Ratio, float lfTileMaterial0, float lfTileMaterial1,
    OUTPARAM(float) lfRatio, OUTPARAM(uint) leMaterial, OUTPARAM(uint) leSecondaryMaterial)
{
    lfRatio = (lfTile0Ratio + lfTile1Ratio + lfTile2Ratio) * (1.0f / 3.0f);
    if( lfRatio < 0.5f )
    {
        leMaterial          = ( ( (uint)lfTileMaterial0 / 2 ) + 1 );
        leSecondaryMaterial = ( ( (uint)lfTileMaterial1 / 2 ) + 1 );
    }
    else
    {
        leSecondaryMaterial = ( ( (uint)lfTileMaterial0 / 2 ) + 1 );
        leMaterial          = ( ( (uint)lfTileMaterial1 / 2 ) + 1 );
    }
}

TKINLINE void
GenPositionForTriangle(
    vec3 lPos0, vec3 lPos1, vec3 lPos2,
    vec3 lNorm0, vec3 lNorm1, vec3 lNorm2,
    float lfRatio, uint leMaterial, uint leSecondaryMaterial,
    vec3 lPlanetPosition, uint luTriangleSeed, uint luPosIndex,
    OUTPARAM(sRegionHeightResult) lResult,
    OUTPARAM(uint) luSeed)
{
    //get edges
    vec3 lEdge1 = ( lPos1 - lPos0 );
    vec3 lEdge2 = ( lPos2 - lPos0 );

    //use the triangle seed combined with the position index within the triangle to calculate 2 random unsigned ints
    uint luRandom1 = HashMixUInt32(luTriangleSeed+luPosIndex);
    uint luRandom2 = HashMixUInt32(luRandom1);

    //convert those to normalized floats and ensure they sum to < 1
    float lfRandom1 = U32ToNormalizedFloat(luRandom1);
    float lfRandom2 = U32ToNormalizedFloat(luRandom2);
    if( lfRandom1 + lfRandom2 >= 1.0f )
    {
        lfRandom1 = 1.0f - lfRandom1;
        lfRandom2 = 1.0f - lfRandom2;
    }

    //build position and normal using the random numbers
    lResult.mPosition = lPos0 + lEdge1 * lfRandom1 + lEdge2 * lfRandom2;
    //lResult.mInterpolatedLocalPosition = lResult.mPosition;
    lResult.mPosition += lPlanetPosition;
    lResult.mNormal = lNorm0 + ( lNorm1 - lNorm0 ) * lfRandom1 + ( lNorm2 - lNorm0 ) * lfRandom2;
    //TKASSERT( !lResult.mNormal.IsZero() );
    lResult.mNormal = normalize(lResult.mNormal);
    //TKASSERT( lResult.mNormal.IsValid() );

    //store the material info
    lResult.mMaterial           = leMaterial;
    lResult.mSecondaryMaterial  = leSecondaryMaterial;
    lResult.mfSlopeValue        = lfRatio;

    //output a unique seed for this point by hashing the last random value
    luSeed = HashMixUInt32(luRandom2);
}

TKINLINE uint
BuildObjectTypeForPosition(
    INPARAM(sObjectTypePriorityMap) lPriorityMap, 
    uint luPosIndex,
    uint luRandSeed,
    uint luTypeIndexOffset,
    OUTPARAM(sAddObjectPosIndex) lOutObject)
{
    //use the map of type-to-priority info to determine where in the types list object's with
    //this priority start, and how many there are
    uint liPriStart = lPriorityMap.maTypeMap[luTypeIndexOffset].muStart;
    uint liPriCount = lPriorityMap.maTypeMap[luTypeIndexOffset].muCount;

    //use the random offset and priority data to get a unique random object type index within
    //the types corresponding to this priority
    uint luObjectType = luRandSeed;      //start with random seed
    luObjectType += luTypeIndexOffset;      //step forward by an amount (doesn't matter exactly what, so long as it increments by 1 each step!)
    luObjectType %= liPriCount;             //mod with list count
    luObjectType += liPriStart;             //shift to start of list
    TKASSERT(luObjectType >= liPriStart && luObjectType < (liPriStart+liPriCount));

    //combine the random seed with type and build a normalized float we can use for a random density
    luRandSeed += luTypeIndexOffset;
    luRandSeed = HashMixUInt32(luRandSeed);
    float lfDensity = U32ToNormalizedFloat(luRandSeed);
    if (lfDensity <= lPriorityMap.maTypeInfo[luObjectType].mfMaxDensity)
    {
        //got an entry, so store it
        //lOutObject.muPosIndex    = (sUInt16)luPosIndex;
        //lOutObject.muObjectType  = (sUInt16)luObjectType;
        lOutObject.muPosIndex_ObjectType = luObjectType << 16 | luPosIndex;
        lOutObject.mfDensity     = lfDensity;
        return 1;
    }
    else
    {
        return 0;
    }
}

#include "Custom/SharedEnd.inl"
#endif
