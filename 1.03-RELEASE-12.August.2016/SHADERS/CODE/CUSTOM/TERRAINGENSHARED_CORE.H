//-----------------------------------------------------------------------------
//Terrain gen core code shared between CPU and GPU
//This can be included from headers in c++ code
//-----------------------------------------------------------------------------

#ifndef _TGENSHARED_CORE_H_
#define _TGENSHARED_CORE_H_
#include "Custom/SharedBegin.inl"

//#define TK_TGEN_VORONOI_IDENICAL 
//#define TK_TGEN_VORONOI_DEBUG

//number of available counters (stored in GDS)
#define TK_TGEN_NUM_COUNTERS (1024)

//sizes of various buffers in GDS memory
#define TK_TGEN_GDS_COUNTER_BUFFER_SIZE (TK_TGEN_NUM_COUNTERS*4)
#define TK_TGEN_GDS_NOISE_BUFFER_SIZE   (512*4)

//base address of terrain gen GDS memory
#define TK_TGEN_GDS_BASE 16*1024

//address of each buffer in GDS memory
#define TK_TGEN_GDS_COUNTER_BUFFER_START TK_TGEN_GDS_BASE
#define TK_TGEN_GDS_NOISE_BUFFER_START (TK_TGEN_GDS_COUNTER_BUFFER_START+TK_TGEN_GDS_COUNTER_BUFFER_SIZE)

//buffer types
#ifdef __cplusplus
    #ifdef D_PLATFORM_ORBIS
        #define ROBUFF(ty)   sce::Shader::Srt::RegularBuffer<ty>
        #define RWBUFF(ty)   sce::Shader::Srt::RW_RegularBuffer<ty>
    #else
        #define ROBUFF(ty) const ty* 
        #define RWBUFF(ty) ty*
    #endif
    #define UNIFORMPARAM(ty) const ty&
    #define INPARAM(ty)  const ty&
    #define OUTPARAM(ty) ty&
    #define INOUTPARAM(ty) ty&
#else
    #define ROBUFF(ty)   RegularBuffer<ty>
    #define RWBUFF(ty)   RW_RegularBuffer<ty>
    #define UNIFORMPARAM(ty) uniform ty
    #define INPARAM(ty)  in ty
    #define OUTPARAM(ty) out ty
    #define INOUTPARAM(ty) inout ty
#endif

//access registers in compute shaders
#define GetThreadGroupSize() __getSpecialSgprUint(__s_tg_size)
#define GetThreadGroupIdX() __getSpecialSgprUint(__s_tgid_x) 
#define GetThreadGroupIdY() __getSpecialSgprUint(__s_tgid_y) 
#define GetThreadGroupIdZ() __getSpecialSgprUint(__s_tgid_z) 
#define GetThreadGroupId() vec3(GetThreadGroupIdX(),GetThreadGroupIdY(),GetThreadGroupIdZ())
#define GetThreadIdX() __getSpecialVgprUint(__v_thread_id_x)
#define GetThreadIdY() __getSpecialVgprUint(__v_thread_id_y)
#define GetThreadIdZ() __getSpecialVgprUint(__v_thread_id_z)
#define GetThreadId() vec3(GetThreadIdX(),GetThreadIdY(),GetThreadIdZ())
#define GetExecMask() __s_read_exec() 
#define GetOrderedWavefrontIndex() ((GetThreadGroupSize() >> 6) & 0x3ff)

//basic read/write to GDS
#define ReadGDS_u32(_addr)          __ds_read_u32((_addr), __kDs_GDS)
#define WriteGDS_u32(_addr,_val)    __ds_write_u32((_addr),(_val),__kDs_GDS)
#define ReadGDS_i32(_addr)          __ds_read_i32((_addr), __kDs_GDS)
#define WriteGDS_i32(_addr,_val)    __ds_write_i32((_addr),(_val),__kDs_GDS)
#define ReadGDS_f32(_addr)          __ds_read_f32((_addr), __kDs_GDS)
#define WriteGDS_f32(_addr,_val)    __ds_write_f32((_addr),(_val),__kDs_GDS)

//local store memory barrier (used to sync threads in a thread group after write to LDS)
#define TGMemBarrier() ThreadGroupMemoryBarrierSync()

//ordered append. notes:
//- K_ORDERED_WAVE_RELEASE must be supplied for last append to a given counter within a thread (this releases the append unit for the next wave to use)
//- K_ORDERED_WAVE_DONE must be done for the last append in a thread or the GPU will DIE!
//- although this can be called on all threads, the '_val' value will be read from the highest active thread. you can think of it as the instruction only does something on the highest active thread.
//- to use ordered append, shaders must be dispatched in ordered append mode
#define K_ORDERED_WAVE_RELEASE (0x1 << 8)
#define K_ORDERED_WAVE_DONE (0x2 << 8)
#define OrderedCount(_addr,_val,_flags) __ds_ordered_count( ((_addr)<<16) | GetOrderedWavefrontIndex(), _val, _flags );

//-----------------------------------------------------------------------------
//Uniforms for the basic job that sets up an indirect dispatch command using
//a GDS counter
//-----------------------------------------------------------------------------
struct cTkIndirectDispatchComputeUniforms
{
    RWBUFF(uint) mauIndirectDispatchArgs; //3 uints (latter 2 will be set to '1')
    uint muPreClamp;            //value to clamp the GDS counter to before doing anything (ignored if 0)
    uint muGDSCounterAddress;   //address in GDS of the counter
    uint muThreadGroupSize;     //size of a thread group
    uint muMultiply;            //amount to multiply the counter by
};

//-----------------------------------------------------------------------------
//Some complimentary math stuff
//-----------------------------------------------------------------------------
TKINLINE float 
smoothstep5(
    float edge0, 
    float edge1, 
    float x )
{
    // Scale, and clamp x to 0..1 range
    x = clamp( (x - edge0) / (edge1 - edge0), 0.0f, 1.0f );
    // Evaluate polynomial
    return x*x*x*(x*(x*6.0f - 15.0f) + 10.0f);
}


//-----------------------------------------------------------------------------
//Very rudimentry half-vector support - works for unpacking/packing but not
//much else
//-----------------------------------------------------------------------------
#ifdef __cplusplus
struct sHalfVector4
{
    sUInt16  x;
    sUInt16  y;
    sUInt16  z;
    sUInt16  w;
};
#else
struct sHalfVector4
{
    uint2 xyzw;
};
#endif

TKINLINE vec3 
OctahedronNormalDecode( 
    vec2 lEncoded )
{
    vec3 lNormal;
    lNormal.z = 1.0f - abs( lEncoded.x ) - abs( lEncoded.y );

    if( lNormal.z >= 0.0f )
    {
        lNormal.xy = lEncoded.xy;
    }
    else
    {
        lNormal.x = 1.0f - abs( lEncoded.y );
        lNormal.y = 1.0f - abs( lEncoded.x );

        lNormal.x *= lEncoded.x < 0.0f ? -1.0f : 1.0f;
        lNormal.y *= lEncoded.y < 0.0f ? -1.0f : 1.0f;
    }

    lNormal = normalize( lNormal );

    return lNormal;
}

TKINLINE void
UnpackHalfVector4(
    INPARAM(sHalfVector4) lPacked, 
    OUTPARAM(vec4) lUnpacked)
{
    #ifdef __cplusplus
    lUnpacked.x = kTkMath.HalfToFloat( lPacked.x );
    lUnpacked.y = kTkMath.HalfToFloat( lPacked.y );
    lUnpacked.z = kTkMath.HalfToFloat( lPacked.z );
    lUnpacked.w = kTkMath.HalfToFloat( lPacked.w );
    #else
    uint2 lXZ = lPacked.xyzw & uint2(0xffff);
    uint2 lYW = lPacked.xyzw >> 16;
    lUnpacked = f16tof32(uint4(lXZ.x,lYW.x,lXZ.y,lYW.y));
    #endif   
}

//-----------------------------------------------------------------------------
//Hash functions for making random-ish values
//-----------------------------------------------------------------------------
TKINLINE uint HashMixUInt32( uint luValue, uint luKey = 0x1b873593 )
{
    luValue = (luValue ^ 61) ^ (luValue >> 16);
    luValue = luValue + (luValue << 3);
    luValue = luValue ^ (luValue >> 4);
    luValue = luValue * 0x1b873593;
    luValue = luValue ^ (luValue >> 15);

    return luValue;
}

TKINLINE uint64 
HashMixUInt64( uint64 luValue )
{
    luValue ^= luValue >> 33;
    luValue *= 0x64dd81482cbd31d7;
    luValue ^= luValue >> 33;
    luValue *= 0xe36aa5c613612997;
    luValue ^= luValue >> 33;

    return luValue;
}
TKINLINE uint64 
HashMixFloatTo64(
    float lfValue )
{
    float lfInverse = lfValue * -2.0f;
    uint64 luHi = asuint(lfValue);
    uint64 luLo = asuint(lfInverse);
    uint64 luRes = luHi << 32 | luLo;
    return HashMixUInt64( luRes );
}

TKINLINE uint64 
HashMixPairU64(
    uint64 luValue1, 
    uint64 luValue2,
    uint64 kMul = 0x9ddfea08eb382d69 ) 
{
    // Murmur-inspired hashing
    uint64 a = (luValue2 ^ luValue1) * kMul;
    a ^= (a >> 47);
    uint64 b = (luValue1 ^ a) * kMul;
    b ^= (b >> 47);
    b *= kMul;
    return b;
}

STATIC_CONST uint kFPUMantissa       = 0x007FFFFF; // FPU mantissa
STATIC_CONST uint kFPUOne            = 0x3F800000; // Binary value of 1.0 in IEEE
TKINLINE float U32ToNormalizedFloat(uint luInput)
{
    //do the FPU magic shit to format it as a number between 1 and 2
    uint luMantissa      = luInput & kFPUMantissa;
    uint luNormed        = luMantissa | kFPUOne;

    //return it as a number between 0 and 1
    return asfloat(luNormed)-1.0f;
   
}

TKINLINE float HashMixU32ToFloat(uint luInput, uint luSeed = 0x1b873593)
{
    uint luHash          = HashMixUInt32(luInput,luSeed);
    return U32ToNormalizedFloat(luHash);
}
//-----------------------------------------------------------------------------
//Calculate an id based off a seed and a random offset
//-----------------------------------------------------------------------------
TKINLINE float RandFloatFromFloat(float lfInput, uint luSeed = 0x1b873593)
{
    //extract uint bits of float
    uint luAsUint           = asuint(lfInput);

    //hash em
    uint luHash             = HashMixUInt32(luAsUint,luSeed);

    //do the FPU magic shit to format it as a number between 0 and 1
    uint luMantissa         = luHash & kFPUMantissa;
    uint luNormed           = luMantissa | kFPUOne;
    float lfRes             = asfloat(luNormed)-1.0f;
    TKASSERT(!kTkMath.IsNaN(lfRes));

    //return that shizzle
    return lfRes;
}


//-----------------------------------------------------------------------------
//Get intersection point of a plane and a line segment
//-----------------------------------------------------------------------------
TKINLINE vec3
PlaneLineIntersection(
    vec3    lvPlaneNorm,
    float   lfPlaneDist,
    vec3    lvPoint1,
    vec3    lvPoint2 )
{
    vec3    lvDiff   = lvPoint2-lvPoint1;
    float   lfDenom  = dot( lvPlaneNorm, lvDiff );
    float   lfParamT = -(lfPlaneDist+dot(lvPlaneNorm,lvPoint1) )/lfDenom;
    return lvPoint1 + (lvDiff*lfParamT);
}

//-----------------------------------------------------------------------------
//Returns the closest axis aligned with a given vector
//-----------------------------------------------------------------------------
TKINLINE vec3
GetDominantUpVector(
    vec3 lPosition )
{
    vec3 lAbsVector = abs(lPosition);
    vec3 lUp = vec3(0.0f);

    if (lAbsVector.y >= lAbsVector.x && lAbsVector.y >= lAbsVector.z )
    {
        lUp.y = sign(lPosition.y);
    }
    else if( lAbsVector.x >= lAbsVector.y && lAbsVector.x >= lAbsVector.z )
    {
        lUp.x = sign(lPosition.x);
    }
    else
    {
        lUp.z = sign(lPosition.z);
    }
    return lUp;
}

//-----------------------------------------------------------------------------
//Projects a position on the face of a cube to the surface of a sphere
//-----------------------------------------------------------------------------
TKINLINE vec3
ProjectCubeToSphere(
    vec3 lCubePos,
    vec3 lFaceNormal)
{
    // Wrap position in unit cube face to sphere
    vec3 lPosition  = lCubePos;
    float lfSize    = dot( lFaceNormal, lCubePos );
    if( lfSize != 0.0f )
    {
        lPosition /= lfSize;
    }

    //get abs normal for zero tests
    vec3 lAbsFaceNormal = abs(lFaceNormal);

    // Determine which axis the face is on
    if( lAbsFaceNormal.x > 1.525878906e-5f )
    {
        // Spherify
		float lfSum = lPosition.y * lPosition.y + lPosition.z * lPosition.z + 1.0f;
		float lfSqrt = sqrt(lfSum);
		lPosition /= lfSqrt;
    }
    else if( lAbsFaceNormal.y > 1.525878906e-5f )
    { 
		float lfSum = lPosition.x * lPosition.x + lPosition.z * lPosition.z + 1.0f;
		float lfSqrt = sqrt(lfSum);
		lPosition /= lfSqrt;
    }
    else if( lAbsFaceNormal.z > 1.525878906e-5f )
    {
		float lfSum = lPosition.y * lPosition.y + lPosition.x * lPosition.x + 1.0f;
		float lfSqrt = sqrt(lfSum);
		lPosition /= lfSqrt;
    }

    // Scale the sphere to the same size as the cube
    lPosition *= lfSize;

    return lPosition;
}

//-----------------------------------------------------------------------------
//Projects position on surface of a sphere to surface of a cube
//-----------------------------------------------------------------------------
TKINLINE vec3
ProjectSphereToCube(
    vec3            lPosition,
    OUTPARAM(vec3)  lNormal,
    float           lfCubeRadius )
{
    vec3  lCubePos;
        
    lNormal     = GetDominantUpVector( lPosition );

    lCubePos    = PlaneLineIntersection( lNormal, -lfCubeRadius, vec3(0.0f), lPosition );

    return lCubePos;
}




#include "Custom/SharedEnd.inl"
#endif
