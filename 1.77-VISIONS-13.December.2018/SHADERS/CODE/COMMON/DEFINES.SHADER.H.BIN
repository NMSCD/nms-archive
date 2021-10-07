// =================================================================================================

#ifndef D_DEFINES
#define D_DEFINES

#if defined __XBOX_ONE && !defined D_PLATFORM_XBOXONE
#define D_PLATFORM_XBOXONE
#endif
#if defined( D_PLATFORM_XBOXONE )
// spilling to LDS is an error
#pragma warning( error:4803 )
#endif

#if defined D_PLATFORM_PC && !defined D_PLATFORM_DX12

#define D_PLATFORM_OPENGL

#elif defined D_PLATFORM_XBOXONE && !defined D_PLATFORM_DX12

#define D_PLATFORM_DX12

#endif

// =================================================================================================
// Platform defines
// =================================================================================================
#if defined D_PLATFORM_OPENGL

    #define D_ENABLE_REVERSEZ_PROJECTION  (1)

    #pragma optionNV(strict on)
    #extension GL_ARB_gpu_shader5 : enable
    #extension GL_ARB_fragment_coord_conventions : enable
	#extension GL_ARB_derivative_control : enable

    #if defined(GL_SPIRV)
        #extension GL_ARB_gpu_shader_int64 : enable
    #endif

    #if defined( GL_NV_gpu_shader5 )
        #extension GL_NV_gpu_shader5 : enable
    #endif

    #if defined( GL_AMD_gpu_shader_half_float )
        #extension GL_AMD_gpu_shader_half_float : enable
    #endif

    #if defined( GL_ARB_shader_ballot )
        #extension GL_ARB_shader_ballot : enable
    #endif

    #if defined( D_FRAGMENT ) && defined( _F64_ )
        layout(early_fragment_tests) in;
    #endif

    #ifdef D_COMPUTE
        #define D_PLATFORM_PC_COMPUTE
    #endif

#elif defined D_PLATFORM_DX12

	#define D_ENABLE_REVERSEZ_PROJECTION  (1)
    #define __XBOX_CONTROL_NONIEEE 0

#elif defined( D_PLATFORM_ORBIS )

	#define D_SRT
    #define D_ENABLE_REVERSEZ_PROJECTION  (1)

    // use this with sdk 2.0 compiler 
   // #pragma argument (allow-scratch-buffer-spill)

    //define these flags so they don't get ignored in build process and in the comb mask
    //this is because materials, vertex layouts and shaders need to be synced on 360 to avoid patching
    #ifdef _F27_VBTANGENT
    #endif
    #ifdef _F28_VBSKINNED
    #endif
    #ifdef _F29_VBCOLOUR
    #endif
    #ifdef _F21_VERTEXCOLOUR
    #endif
    #ifdef _F02_SKINNED
    #endif
    #ifdef _F03_NORMALMAP
    #endif
    #if defined( _F01_DIFFUSEMAP ) || defined( D_LOD0 ) || defined( D_LOD1 ) || defined( D_LOD2 ) || defined( D_LOD3) || defined( D_LOD4 ) 
    #endif
    #ifdef _F01_DIFFUSEMAP
    #endif
    #ifdef _F09_TRANSPARENT
    #endif
    #ifdef _F10_NORECEIVESHADOW
    #endif

    // disable warnings for unused parameters. This happens a lot because of defining different things.
    #pragma warning (disable: 5203)

    // temp thing to know what things are still required on ps4.
    #define D_PLATFORM_ORBIS_FIX

    #ifdef __PSSL_CS__
    #define D_PLATFORM_ORBIS_COMPUTE
    #define D_COMPUTE
    #endif

    #ifdef __PSSL_HS__
    #define D_HULL
    #endif

    #ifdef __PSSL_DS__
    #define D_DOMAIN
    #endif

    #ifdef __PSSL_VS__
    #define D_VERTEX
    #endif

    #ifdef __PSSL_VS_LS__
    // this is a vertex shader that's part of a tessellation chain
    #define D_VERTEX
    #endif

    #ifdef __PSSL_GS__
    #define D_GEOMETRY
    #endif

#endif

// =================================================================================================
// Basic Types
// =================================================================================================
#ifdef D_PLATFORM_OPENGL
    //#define CONST         const
    #define STATIC_CONST  const
    #define lerp          mix
    #define half4 f16vec4
    #define half3 f16vec3
    #define half2 f16vec2
    #define half float16_t
    #define float2vec2(_a) vec2(_a)
    #define float2vec3(_a) vec3(_a)
    #define float2vec4(_a) vec4(_a)
    #define cast2mat3(_m)  mat3(_m)
	#define groupID				gl_WorkGroupID
	#define groupThreadID		gl_LocalInvocationID
	#define dispatchThreadID	gl_GlobalInvocationID
	#define THREADGROUP_LOCAL   layout( shared )
	#define THREADGROUP_BARRIER groupMemoryBarrier()

#elif defined(D_PLATFORM_DX12)

	#define vec2            float2
	#define vec3            float3
	#define vec4            float4
	#define ivec2           int2
	#define ivec3           int3
	#define ivec4           int4
	#define uvec2           uint2
	#define uvec3           uint3
	#define uvec4           uint4

    #define rgba32f         float4
    #define r32f            float
    #define r32i            int
    #define rgba32ui        uint4
    #define rg32ui          uint2
	// NOTE: 
	// operator[] accesses rows, not columns 
	// matrix constructors interpret the passed vectors as row vectors, not column vectors
	#define mat2            float2x2
	#define mat3            float3x3
	#define mat4            float4x4

	#define Texture2D_Array		Texture2DArray
	#define RW_Texture2D		RWTexture2D
    #define RW_Texture3D        RWTexture3D
	#define RW_DataBuffer		RWBuffer
	#define DataBuffer			Buffer
	#define RW_RegularBuffer	RWStructuredBuffer
	#define RegularBuffer		StructuredBuffer
	#define SampleLOD			SampleLevel
	#define SampleCmpLOD0		SampleCmpLevelZero
	#define SampleGradient		SampleGrad

	#define THREADGROUP_LOCAL       groupshared
	#define THREADGROUP_BARRIER     GroupMemoryBarrier()
	#define S_GROUP_ID              SV_GroupID
	#define S_GROUP_THREAD_ID       SV_GroupThreadID
	#define S_DISPATCH_THREAD_ID    SV_DispatchThreadID

	#define SRT(STRUCTURE, NAME)	NAME

	#define STATIC_CONST    static const

	#define float2vec2( _a ) _a
	#define float2vec3( _a ) _a
	#define float2vec4( _a ) _a
    #define cast2mat3( _m )  (mat3) _m
    //#define lerp          mix

#elif defined D_PLATFORM_ORBIS

    #define vec2            float2
    #define vec3            float3
    #define vec4            float4
    #define ivec2           int2
    #define ivec3           int3
    #define ivec4           int4
    #define uvec2           uint2
    #define uvec3           uint3
    #define uvec4           uint4

    #define rgba32f         float4
    #define r32f            float
    #define r32i            int
    #define rgba32ui        uint4
    #define rg32ui          uint2
    // NOTE: 
    // operator[] accesses rows, not columns 
    // matrix constructors interpret the passed vectors as row vectors, not column vectors
    #define mat2            row_major float2x2
    #define mat3            row_major float3x3
    #define mat4            row_major float4x4

	#define SRT(STRUCTURE, NAME)	STRUCTURE->NAME
    #define STATIC_CONST    static const
   // #define const           ERROR, DON'T USE CONST FOR PS4. USE STATIC_CONST INSTEAD FOR A COMPILED IN CONSTANT. OTHERWISE IT TRIES TO PUT IT IN A CONSTANT BUFFER AND FOR SOME REASON IT DOESN'T WORK.

	#define float2vec2( _a ) vec2( _a )
	#define float2vec3( _a ) vec3( _a )
	#define float2vec4( _a ) vec4( _a )    
    #define cast2mat3( _m )  (mat3) _m


    #define THREADGROUP_LOCAL    thread_group_memory
    #define THREADGROUP_BARRIER  ThreadGroupMemoryBarrier()

#endif

#if defined(D_PLATFORM_DX12)


#if defined( D_TESS_SHADERS_PRESENT )

#define ROOT_FLAGS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"

#else

#define ROOT_FLAGS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | DENY_DOMAIN_SHADER_ROOT_ACCESS | DENY_GEOMETRY_SHADER_ROOT_ACCESS | DENY_HULL_SHADER_ROOT_ACCESS),"

#endif

    #define ROOT_SIG_VS_PS\
        ROOT_FLAGS                                            \
        "CBV(b0, space = 0,visibility=SHADER_VISIBILITY_ALL),"\
        "DescriptorTable( SRV(t0, numDescriptors=24, space = 0), UAV(u0, numDescriptors=8),  visibility=SHADER_VISIBILITY_ALL)," \
        "DescriptorTable( Sampler(s0, numDescriptors=24, space = 0),  visibility=SHADER_VISIBILITY_ALL)"

    #define ROOT_SIG_CS\
        ROOT_FLAGS                                            \
        "CBV(b0, space = 0,visibility=SHADER_VISIBILITY_ALL),"\
        "DescriptorTable( SRV(t0, numDescriptors=24, space = 0),  UAV(u0, numDescriptors=8),  visibility=SHADER_VISIBILITY_ALL)," \
        "DescriptorTable( Sampler(s0, numDescriptors=24, space = 0),  visibility=SHADER_VISIBILITY_ALL)"


	#define ROOT_SIG_VS			ROOT_SIG_VS_PS
	#define ROOT_SIG_PS			ROOT_SIG_VS_PS
	#define ROOT_SIG_GS			ROOT_SIG_VS_PS
	#define ROOT_SIG_HS			ROOT_SIG_VS_PS
	#define ROOT_SIG_DS			ROOT_SIG_VS_PS

    #define S_POSITION  		 SV_Position        
    #define S_TARGET_OUTPUT0     SV_Target0
    #define S_TARGET_OUTPUT1     SV_Target1
    #define S_TARGET_OUTPUT2     SV_Target2
    #define S_TARGET_OUTPUT3     SV_Target3
    #define S_TARGET_OUTPUT4     SV_Target4
    #define S_TARGET_OUTPUT5     SV_Target5
    #define S_TARGET_OUTPUT6     SV_Target6
    #define S_TARGET_OUTPUT7     SV_Target7
    #define S_TARGET_OUTPUT      SV_Target
    #define S_DEPTH_OUTPUT       SV_Depth
    #define S_DEPTH_GE_OUTPUT    SV_DepthGreater
    #define S_DEPTH_LE_OUTPUT    SV_DepthLessEqual
    #define S_FRONT_FACE         SV_IsFrontFace

    #define  floatToIntBits( _X )  asuint( _X )
    #define  pow( _X, _Y )         pow( max( _X, 0.0 ) , _Y )

#if defined( D_PLATFORM_XBOXONE )
    bool  ballot(uint _X)
    {
        uint2 luVal32Vec2 = __XB_Ballot64(_X);
        return (luVal32Vec2[0] | luVal32Vec2[1]) ? true : false;
    }
    float LaneSwizzle(float x, const uint andMask, const uint orMask, const uint xorMask)
    {
        uint offset10<> = uint4(andMask, orMask, xorMask, 0);
        return __XB_LaneSwizzle(x, offset10);
    }

    uint __v_cndmask_b32(uint vsrc0, uint vsrc1, unsigned int ssrc1, unsigned int ssrc2)
    {
        return __XB_MBCNT64(uint2(ssrc1, ssrc2));
    }
#endif

#endif

// =================================================================================================
// Functions
// =================================================================================================

#ifdef D_PLATFORM_OPENGL

	#define saturate( V )  clamp(V, 0.0, 1.0)
    #define atan2( Y, X )  atan( Y, X )
    #define invsqrt( X )   inversesqrt( X )

#ifdef D_COMPUTE

    #define groupID           gl_WorkGroupID
    #define groupThreadID     gl_LocalInvocationID
    #define dispatchThreadID  gl_GlobalInvocationID

    shared float gDerivBufferf[8][8];
    shared vec2  gDerivBuffer2[8][8];
    shared vec3  gDerivBuffer3[8][8];
    shared vec4  gDerivBuffer4[8][8];

    float dFdx( float var ) 
    { 
        gDerivBufferf[groupThreadID.x][groupThreadID.y] = var; 
        groupMemoryBarrier(); 
        float delta = var - gDerivBufferf[groupThreadID.x ^ 1][groupThreadID.y]; 
        delta = (groupThreadID.x & 1) == 1? -delta : delta;
        return delta;
    }

    vec2 dFdx( vec2 var ) 
    { 
        gDerivBuffer2[groupThreadID.x][groupThreadID.y] = var; 
        groupMemoryBarrier(); 
        vec2 delta = var - gDerivBuffer2[groupThreadID.x ^ 1][groupThreadID.y]; 
        delta = (groupThreadID.x & 1) == 1? -delta : delta;
        return delta;
    }

    vec3 dFdx( vec3 var ) 
    { 
        gDerivBuffer3[groupThreadID.x][groupThreadID.y] = var; 
        groupMemoryBarrier(); 
        vec3 delta = var - gDerivBuffer3[groupThreadID.x ^ 1][groupThreadID.y]; 
        delta = (groupThreadID.x & 1) == 1? -delta : delta;
        return delta;
    }

    vec4 dFdx( vec4 var ) 
    { 
        gDerivBuffer4[groupThreadID.x][groupThreadID.y] = var; 
        groupMemoryBarrier(); 
        vec4 delta = var - gDerivBuffer4[groupThreadID.x ^ 1][groupThreadID.y]; 
        delta = (groupThreadID.x & 1) == 1? -delta : delta;
        return delta;
    }

    float dFdy( float var ) 
    { 
        gDerivBufferf[groupThreadID.x][groupThreadID.y] = var; 
        groupMemoryBarrier(); 
        float delta = var - gDerivBufferf[groupThreadID.x][groupThreadID.y ^ 1]; 
        delta = (groupThreadID.y & 1) == 1? -delta : delta;
        return delta;
    }

    vec2 dFdy( vec2 var ) 
    { 
        gDerivBuffer2[groupThreadID.x][groupThreadID.y] = var; 
        groupMemoryBarrier(); 
        vec2 delta = var - gDerivBuffer2[groupThreadID.x][groupThreadID.y ^ 1]; 
        delta = (groupThreadID.y & 1) == 1? -delta : delta;
        return delta;
    }

    vec3 dFdy( vec3 var ) 
    { 
        gDerivBuffer3[groupThreadID.x][groupThreadID.y] = var; 
        groupMemoryBarrier(); 
        vec3 delta = var - gDerivBuffer3[groupThreadID.x][groupThreadID.y ^ 1]; 
        delta = (groupThreadID.y & 1) == 1? -delta : delta;
        return delta;
    }

    vec4 dFdy( vec4 var ) 
    { 
        gDerivBuffer4[groupThreadID.x][groupThreadID.y] = var; 
        groupMemoryBarrier(); 
        vec4 delta = var - gDerivBuffer4[groupThreadID.x][groupThreadID.y ^ 1]; 
        delta = (groupThreadID.y & 1) == 1? -delta : delta;
        return delta;
    }

    #define dFdxFine        dFdx
    #define dFdyFine        dFdy

#endif

#else

#define mix             lerp
#define fract           frac
#define mod				fmod
#define saturate( V )   ( min( max( V, 0.0) , 1.0) )
#define invsqrt( X )    rsqrt( X )

#if defined(D_COMPUTE) && defined(D_PLATFORM_ORBIS)

    float dFdx( float var ) { float delta = var - LaneSwizzle( var, 0x1f, 0, 1 ); return __v_cndmask_b32(0, 1, 0xAAAAAAAAAAAAAAAA ) ? delta : -delta;  }
    float dFdy( float var ) { float delta = var - LaneSwizzle( var, 0x1f, 0, 8 ); return __v_cndmask_b32(0, 1, 0xFF00FF00FF00FF00 ) ? delta : -delta;  }

    vec2 dFdx( vec2 var ) { vec2 delta = vec2( var.x - LaneSwizzle( var.x, 0x1f, 0, 1 ), var.y - LaneSwizzle( var.y, 0x1f, 0, 1 ) ); return __v_cndmask_b32(0, 1, 0xAAAAAAAAAAAAAAAA ) ? delta : -delta;  }
    vec2 dFdy( vec2 var ) { vec2 delta = vec2( var.x - LaneSwizzle( var.x, 0x1f, 0, 8 ), var.y - LaneSwizzle( var.y, 0x1f, 0, 8 ) ); return __v_cndmask_b32(0, 1, 0xFF00FF00FF00FF00 ) ? delta : -delta;  }

    vec3 dFdx( vec3 var ) { vec3 delta = vec3( var.x - LaneSwizzle( var.x, 0x1f, 0, 1 ), var.y - LaneSwizzle( var.y, 0x1f, 0, 1 ), var.z - LaneSwizzle( var.z, 0x1f, 0, 1 ) ); return __v_cndmask_b32(0, 1, 0xAAAAAAAAAAAAAAAA ) ? delta : -delta;  }
    vec3 dFdy( vec3 var ) { vec3 delta = vec3( var.x - LaneSwizzle( var.x, 0x1f, 0, 8 ), var.y - LaneSwizzle( var.y, 0x1f, 0, 8 ), var.z - LaneSwizzle( var.z, 0x1f, 0, 8 ) ); return __v_cndmask_b32(0, 1, 0xFF00FF00FF00FF00 ) ? delta : -delta;  }

    vec4 dFdx( vec4 var ) { vec4 delta = vec4( var.x - LaneSwizzle( var.x, 0x1f, 0, 1 ), var.y - LaneSwizzle( var.y, 0x1f, 0, 1 ), var.z - LaneSwizzle( var.z, 0x1f, 0, 1 ), var.z - LaneSwizzle( var.w, 0x1f, 0, 1 ) ); return __v_cndmask_b32(0, 1, 0xAAAAAAAAAAAAAAAA ) ? delta : -delta;  }
    vec4 dFdy( vec4 var ) { vec4 delta = vec4( var.x - LaneSwizzle( var.x, 0x1f, 0, 8 ), var.y - LaneSwizzle( var.y, 0x1f, 0, 8 ), var.z - LaneSwizzle( var.z, 0x1f, 0, 8 ), var.z - LaneSwizzle( var.w, 0x1f, 0, 8 ) ); return __v_cndmask_b32(0, 1, 0xFF00FF00FF00FF00 ) ? delta : -delta;  }


    #define dFdxFine        dFdx
    #define dFdyFine        dFdy

#elif defined(D_COMPUTE) && defined(__XBOX_ONE)

    float dFdx(float var) { float delta = var - LaneSwizzle(var, 0x1f, 0, 1); return __v_cndmask_b32(0, 1, 0xAAAAAAAA, 0xAAAAAAAA) ? delta : -delta; }
    float dFdy(float var) { float delta = var - LaneSwizzle(var, 0x1f, 0, 8); return __v_cndmask_b32(0, 1, 0xFF00FF00, 0xFF00FF00) ? delta : -delta; }

    vec2 dFdx(vec2 var) { vec2 delta = vec2(var.x - LaneSwizzle(var.x, 0x1f, 0, 1), var.y - LaneSwizzle(var.y, 0x1f, 0, 1)); return __v_cndmask_b32(0, 1, 0xAAAAAAAA, 0xAAAAAAAA) ? delta : -delta; }
    vec2 dFdy(vec2 var) { vec2 delta = vec2(var.x - LaneSwizzle(var.x, 0x1f, 0, 8), var.y - LaneSwizzle(var.y, 0x1f, 0, 8)); return __v_cndmask_b32(0, 1, 0xFF00FF00, 0xFF00FF00) ? delta : -delta; }

    vec3 dFdx(vec3 var) { vec3 delta = vec3(var.x - LaneSwizzle(var.x, 0x1f, 0, 1), var.y - LaneSwizzle(var.y, 0x1f, 0, 1), var.z - LaneSwizzle(var.z, 0x1f, 0, 1)); return __v_cndmask_b32(0, 1, 0xAAAAAAAA, 0xAAAAAAAA) ? delta : -delta; }
    vec3 dFdy(vec3 var) { vec3 delta = vec3(var.x - LaneSwizzle(var.x, 0x1f, 0, 8), var.y - LaneSwizzle(var.y, 0x1f, 0, 8), var.z - LaneSwizzle(var.z, 0x1f, 0, 8)); return __v_cndmask_b32(0, 1, 0xFF00FF00, 0xFF00FF00) ? delta : -delta; }

    vec4 dFdx(vec4 var) { vec4 delta = vec4(var.x - LaneSwizzle(var.x, 0x1f, 0, 1), var.y - LaneSwizzle(var.y, 0x1f, 0, 1), var.z - LaneSwizzle(var.z, 0x1f, 0, 1), var.z - LaneSwizzle(var.w, 0x1f, 0, 1)); return __v_cndmask_b32(0, 1, 0xAAAAAAAA, 0xAAAAAAAA) ? delta : -delta; }
    vec4 dFdy(vec4 var) { vec4 delta = vec4(var.x - LaneSwizzle(var.x, 0x1f, 0, 8), var.y - LaneSwizzle(var.y, 0x1f, 0, 8), var.z - LaneSwizzle(var.z, 0x1f, 0, 8), var.z - LaneSwizzle(var.w, 0x1f, 0, 8)); return __v_cndmask_b32(0, 1, 0xFF00FF00, 0xFF00FF00) ? delta : -delta; }

    #define dFdxFine        dFdx
    #define dFdyFine        dFdy

#elif defined(D_COMPUTE) && defined(D_PLATFORM_DX12)
    float dFdx( float var ) { return 0;  }
    float dFdy( float var ) { return 0;  }

	vec2 dFdx( vec2 var ) { return vec2(0, 0); }
    vec2 dFdy( vec2 var ) { return vec2(0, 0); }

    vec3 dFdx( vec3 var ) { return vec3(0, 0, 0); }
    vec3 dFdy( vec3 var ) { return vec3(0, 0, 0); }

    vec4 dFdx( vec4 var ) { return vec4(0, 0, 0, 0); }
    vec4 dFdy( vec4 var ) { return vec4(0, 0, 0, 0); }

    #define dFdxFine        dFdx
    #define dFdyFine        dFdy

#else

    #define dFdx            ddx
    #define dFdy            ddy
    #define dFdxFine        ddx_fine
    #define dFdyFine        ddy_fine

#endif

#endif

// =================================================================================================
// Samplers and textures
// =================================================================================================

#ifdef D_PLATFORM_OPENGL
  #ifdef D_USE_UBO
    #ifndef D_SAMPLERS_ARE_GLOBAL
      #define D_SAMPLERS_ARE_GLOBAL
    #endif
  #endif

    #define TEXTURECUBE                     texture
    #define TEXTURECUBELOD                  texture

	#define shadow2D( S, UV )				texture( S, UV )

    #define SAMPLER2DSHADOW( NAME, REG )    uniform sampler2DShadow NAME

    #define SAMPLERCUBE( NAME )             samplerCube NAME
    #define SAMPLERCUBEARG( NAME )          in samplerCube NAME
    #define SAMPLERCUBEPARAM( NAME )        NAME

    #define imageAtomicAddOut( T, C, V, O )    O = imageAtomicAdd( T, C, V )

  #ifdef D_SAMPLERS_ARE_GLOBAL
    #define BEGIN_SAMPLERBLOCK              };
    #define END_SAMPLERBLOCK                
    #define BEGIN_IMAGEBLOCK
    #define END_IMAGEBLOCK
    #define SAMPLER_GETMAP( _S, _V )        _V
    #define SAMPLER_GETLOCAL( _S, _V )      _V
  #else
    #define BEGIN_SAMPLERBLOCK
    #define END_SAMPLERBLOCK                };
    #define BEGIN_IMAGEBLOCK                };
    #define END_IMAGEBLOCK
    #define SAMPLER_GETMAP( _S, _V )        _S._V
    #define SAMPLER_GETLOCAL( _S, _V )      _S._V
  #endif

    #define IMAGE_GETMAP( _S, _V )        _V
    #define IMAGE_GETLOCAL( _S, _V )      _V

#if defined( D_TEXTURE_FEEDBACK )

    #define texture2D( T, C )               Tex2dFeedback( T, T##FB, C )

    #define texture2DLod( T, C, N )         Tex2dLodFeedback( T, T##FB, C, N )  
    #define texture2DArray(  T, C )         Tex2dArrayFeedback( T, T##FB, C )

    #define texture3D( S, UV )              Tex3dFeedback( S, S##FB, UV )
    #define texture3DLod( S, UV, LOD )      Tex3dLodFeedback( S, S##FB, UV, LOD )

    #define SAMPLER2DARG( NAME )            in sampler2D NAME, in int NAME##FB
    #define SAMPLER2DPARAM( NAME )          NAME, NAME##FB

    #define SAMPLER2DARRAYARG( NAME )       in sampler2DArray NAME, in int NAME##FB
    #define SAMPLER2DARRAYPARAM( NAME )     NAME, NAME##FB

    #define SAMPLER2D( NAME )               sampler2D       NAME; int NAME##FB
    #define SAMPLER2DARRAY( NAME )          sampler2DArray  NAME; int NAME##FB
    #define SAMPLER3D( NAME )               sampler3D       NAME; int NAME##FB
    #define SAMPLER2DSHADOW_SRT( NAME )     sampler2DShadow NAME; int NAME##FB

  #else

  #if defined(D_DOMAIN) || defined(D_HULL)
    #define texture2D( T, C )               textureLod( T, C, 0 )
    #define texture2DArray(  T, C )         textureLod( T, C, 0 )
    #define texture2DComputeGrad(  T, C )   textureLod( T, C, 0 )
  #else
    #define texture2DComputeGrad( T, C )    texture2D( T, C )
  #endif

    #define texture2DBias( T, C, B )        texture( T, C, B )
    #define texture2DLod( S, UV, LOD )      textureLod( S, UV, LOD )
    #define texture3DLod( S, UV, LOD )      textureLod( S, UV, LOD )

    #define SAMPLER2DARG( NAME )            in sampler2D NAME
    #define ISAMPLER2DARG( TYPE, NAME )     in isampler2D NAME
    #define SAMPLER2DPARAM( NAME )          NAME
    #define SAMPLER2DPARAM_SRT( STRUCTURE, NAME )  SAMPLER_GETMAP( STRUCTURE, NAME ) 

    #define SAMPLER3DARG( NAME )            in sampler3D NAME
    #define SAMPLER3DPARAM( NAME )          NAME
    #define SAMPLER3DPARAM_SRT( STRUCTURE, NAME )  SAMPLER_GETMAP( STRUCTURE, NAME ) 

  #ifdef D_SAMPLERS_ARE_GLOBAL
    #define SAMPLER2DPARAM_LOCAL( STRUCTURE, NAME )  NAME  

    #define SAMPLER2D( NAME )               uniform sampler2D       NAME
    #define SAMPLER2DARRAY( NAME )          uniform sampler2DArray  NAME
    #define SAMPLER3D( NAME )               uniform sampler3D       NAME
    #define SAMPLER2DSHADOW_SRT( NAME )     uniform sampler2DShadow NAME
    #define ISAMPLER2D( TYPE, NAME )        uniform isampler2D       NAME

    #else
    #define SAMPLER2DPARAM_LOCAL( STRUCTURE, NAME )  STRUCTURE.NAME  

    #define SAMPLER2D( NAME )               sampler2D       NAME
    #define SAMPLER2DREG( NAME, REG )       sampler2D       NAME
    #define SAMPLER2DARRAY( NAME )          sampler2DArray  NAME
    #define SAMPLER2DARRAYREG( NAME, REG )  sampler2DArray  NAME
    #define SAMPLER3D( NAME )               sampler3D       NAME
    #define SAMPLER3DREG( NAME, REG )       sampler3D       NAME
    #define SAMPLER2DSHADOW_SRT( NAME )     sampler2DShadow NAME
    #define ISAMPLER2D( TYPE, NAME )        isampler2D      NAME
    #endif

    #define RWIMAGE2D( TYPE, NAME )         uniform layout( TYPE ) image2D NAME 

    #define RWINTIMAGE2D( TYPE, NAME )      uniform layout( TYPE ) iimage2D NAME 

    #define REGULARBUFFER( TYPE, NAME )    layout(std430) buffer NAME ## _buf  { TYPE NAME[]; }
    #define RW_REGULARBUFFER( TYPE, NAME ) REGULARBUFFER( TYPE, NAME )

    #define SAMPLER2DARRAYARG( NAME )       in sampler2DArray NAME
    #define RWIMAGE2DARG( TYPE, NAME )      layout( TYPE ) image2D NAME
    #define RWINTIMAGE2DARG( TYPE, NAME )   layout( TYPE ) iimage2D NAME
    #define SAMPLER2DARRAYPARAM( NAME )     NAME
    #define SAMPLER2DARRAYPARAM_SRT( STRUCTURE, NAME ) SAMPLER_GETMAP( STRUCTURE, NAME ) 
    
#endif

    #define textureGatherRed(   lTex, lSamp ) textureGather( lTex, lSamp, 0 )
    #define textureGatherGreen( lTex, lSamp ) textureGather( lTex, lSamp, 1 )
    #define textureGatherBlue(  lTex, lSamp ) textureGather( lTex, lSamp, 2 )
    #define textureGatherAlpha( lTex, lSamp ) textureGather( lTex, lSamp, 3 )

#else //!D_PLATFORM_OPENGL

#if defined(D_PLATFORM_DX12)
  	#define BEGIN_SAMPLERBLOCK  };
  	#define END_SAMPLERBLOCK
    #define BEGIN_IMAGEBLOCK           
    #define END_IMAGEBLOCK
    #define SAMPLER_GETMAP(_S, _V)			_V
    #define SAMPLER_GETLOCAL( _S, _V )      _V
    #define IMAGE_GETMAP(_S, _V)            _V
    #define IMAGE_GETLOCAL( _S, _V )        _V
    #define SAMPLER2DPARAM_LOCAL( STRUCTURE, NAME )  SAMPLER2DPARAM( NAME )
#else
    #define BEGIN_SAMPLERBLOCK
    #define END_SAMPLERBLOCK				};
    #define BEGIN_IMAGEBLOCK                
    #define END_IMAGEBLOCK                  };
    #define SAMPLER_GETMAP( _S, _V )		_S->_V
    #define SAMPLER_GETLOCAL( _S, _V )      _S._V
    #define IMAGE_GETMAP( _S, _V )          _S->_V
    #define IMAGE_GETLOCAL( _S, _V )        _S._V
    #define SAMPLER2DPARAM_LOCAL( STRUCTURE, NAME )   SAMPLER2DPARAM( STRUCTURE.NAME )
#endif

  #if defined(D_DOMAIN)
    #define texture2D( T, C )               T.SampleLOD( T##SS, C, 0)
    #define texture2DArray(  T, C )         T.SampleLOD( T##SS, C, 0)
    #define texture2DArrayLod(  T, C, L )   T.SampleLOD( T##SS, C, L )
    #define texture2DComputeGrad(  T, C )   T.SampleLOD( T##SS, C, 0)
  #elif defined(D_COMPUTE)
	#define texture2D( T, C )		        T.SampleLOD( T##SS, C, 0)
	#define texture2DArray(  T, C )         T.SampleLOD( T##SS, C, 0)
    #define TEXTURECUBE( T, C )		        T.SampleLOD( T##SS, C )

	#define texture2DComputeGrad( T, C )    T.SampleGradient(T##SS, C, vec2( dFdx( C ) ), vec2( dFdy( C ) ) )
	#define shadow2D( T, C )                T.SampleCmpLOD0(T##SS, C.xy, C.z)
  #else
	#define texture2D( T, C )		           T.Sample( T##SS, C )
    #define texture2DBias( T, C, B )           T.SampleBias( T##SS, C, B )
	#define texture2DArray(  T, C )            T.Sample( T##SS, C )
    #define texture2DArrayLod(  T, C, L )      T.SampleLOD( T##SS, C, L )
    #define TEXTURECUBE( T, C )		           T.Sample( T##SS, C )
	#define texture2DComputeGrad( T, C )       T.Sample(T##SS, C )
	#define shadow2D( T, C )                   T.SampleCmp(T##SS, C.xy, C.z )
  #endif

    

	#define texture2DLod( T, C, N )           T.SampleLOD(T##SS, C, N )
	#define texture3DLod( T, C, N )           T.SampleLOD(T##SS, C, N )
	#define TEXTURECUBELOD( T, C, N )	      T.Sample( T##SS, C, N )
	#define textureGrad( T, C, DDX, DDY )     T.SampleGradient(T##SS, C, DDX, DDY )                    

	#define textureGatherRed(   lTex, lSamp ) lTex.GatherRed  ( lTex##SS, lSamp )
	#define textureGatherGreen( lTex, lSamp ) lTex.GatherGreen( lTex##SS, lSamp )
	#define textureGatherBlue(  lTex, lSamp ) lTex.GatherBlue ( lTex##SS, lSamp )
	#define textureGatherAlpha( lTex, lSamp ) lTex.GatherAlpha( lTex##SS, lSamp )

    #define RWIMAGE2D( TYPE, NAME )         RW_Texture2D< TYPE > NAME; SamplerState NAME##SS

    #define RWINTIMAGE2D( TYPE, NAME )         RW_Texture2D< TYPE > NAME; SamplerState NAME##SS

    #define REGULARBUFFER( TYPE, NAME )     RegularBuffer< TYPE > NAME
    #define RW_REGULARBUFFER( TYPE, NAME )  RW_RegularBuffer< TYPE > NAME

	#define imageAtomicAdd( T, C, V )         AtomicAdd( T[ C ], V )
	#define imageAtomicAddOut( T, C, V, O )   AtomicAdd( T[ C ], V, O )
    #define imageLoad( T, C )               ( T[C] )
	#define imageStore( T, C, V )             T[C] = V


    #define SAMPLER3D( NAME )                 Texture3D NAME; SamplerState NAME##SS
    //#define SAMPLERCUBE( NAME, REG )          SamplerState NAME##SS : register( s##REG ); TextureCube NAME##TU : register( t##REG )
    #define SAMPLERCUBE( NAME )               TextureCube NAME; SamplerState NAME##SS
    #define SAMPLER3DARG( NAME )              Texture3D NAME, SamplerState NAME##SS
    #define SAMPLER3DPARAM( NAME )            NAME, NAME##SS
    #define SAMPLER3DPARAM_SRT( STRUCTURE, NAME )		SAMPLER3DPARAM(SAMPLER_GETMAP(STRUCTURE, NAME))

    #define SAMPLER2D( NAME )                 Texture2D NAME; SamplerState NAME##SS

#if defined(D_PLATFORM_DX12)
    #define SAMPLER2DREG( NAME, REG )         Texture2D NAME : register(t##REG); SamplerState NAME##SS : register(s##REG)
    #define SAMPLER3DREG( NAME, REG )         Texture3D NAME : register(t##REG); SamplerState NAME##SS : register(s##REG)
    #define SAMPLER2DARRAYREG( NAME, REG )    Texture2D_Array NAME : register(t##REG); SamplerState NAME##SS : register(s##REG)
#else
    #define SAMPLER2DREG( NAME, REG )         Texture2D NAME; SamplerState NAME##SS
    #define SAMPLER3DREG( NAME, REG )         Texture3D NAME; SamplerState NAME##SS
    #define SAMPLER2DARRAYREG( NAME, REG )    Texture2D_Array NAME; SamplerState NAME##SS
#endif

    #define SAMPLER2DARG( NAME )              Texture2D NAME, SamplerState NAME##SS
    #define SAMPLER2DPARAM( NAME )            NAME, NAME##SS
    #define SAMPLER2DPARAM_SRT( STRUCTURE, NAME )		SAMPLER2DPARAM(SAMPLER_GETMAP(STRUCTURE, NAME))
    #define SAMPLER2DSHADOW_SRT( NAME )       Texture2D NAME; SamplerComparisonState NAME##SS
             
    #define SAMPLER2DARRAY( NAME )            Texture2D_Array NAME; SamplerState NAME##SS
    #define SAMPLER2DARRAYARG( NAME )         Texture2D_Array NAME, SamplerState NAME##SS
    #define ISAMPLER2D( TYPE, NAME )          Texture2D<TYPE> NAME; SamplerState NAME##SS
    #define ISAMPLER2DARG( TYPE, NAME )       Texture2D<TYPE> NAME, SamplerState NAME##SS
    #define SAMPLER2DARRAYPARAM( NAME )		  NAME, NAME##SS
    #define SAMPLER2DARRAYPARAM_SRT( STRUCTURE, NAME )	SAMPLER2DPARAM(SAMPLER_GETMAP(STRUCTURE,NAME))

    #define RWIMAGE2DARG( TYPE, NAME )        RW_Texture2D< TYPE > NAME
    #define RWINTIMAGE2DARG( TYPE, NAME )     RW_Texture2D< TYPE > NAME

  #if defined(D_PLATFORM_DX12)
    #define textureQueryLod( lTex, lCoords )  vec2( lTex.CalculateLevelOfDetailUnclamped( lTex##SS, lCoords ), 0.0 )
    #define texelFetch( lTex, lSamp, lLod ) lTex.Load( int3((lSamp), (lLod)) )
  #else
    #define textureQueryLod( lTex, lCoords )  vec2( lTex.GetLODUnclamped( lTex##SS, lCoords ), 0.0 )
    #define texelFetch( lTex, lSamp, lLod ) lTex.MipMaps((lLod), (lSamp))
  #endif

#endif

// =================================================================================================
// Matrices
// =================================================================================================
#if defined(D_PLATFORM_OPENGL)

    #define MUL( INPUT_A, INPUT_B )         (INPUT_A * INPUT_B)
    #define PLATFORM_TRANSPOSE
    #define MAT4_SET_POS( M, P )            M[ 3 ] = P
    #define MAT4_SET_TRANSLATION( M, T )    M[ 3 ].xyz = T
    #define MAT4_GET_COLUMN( M, C )         M[ C ].xyz
    #define MAT3_GET_COLUMN( M, C )         M[ C ]
    #define MAT4_GET_COLUMN_VEC4( M, C )    M[ C ]

    #define MAT3_SET_COLUMN( M, C, V )      M[ C ] = V;
    #define MAT4_SET_COLUMN( M, C, V )      M[ C ] = V;

#else

    #define MUL( INPUT_A, INPUT_B )         mul( INPUT_B, INPUT_A )
    #define PLATFORM_TRANSPOSE
    #define MAT4_SET_POS( M, P )            M[ 3 ] = P
    #define MAT4_SET_TRANSLATION( M, T )    M[ 3 ].xyz = T
    #define MAT4_GET_COLUMN( M, C )         M[ C ].xyz
    #define MAT3_GET_COLUMN( M, C )         M[ C ]
    #define MAT4_GET_COLUMN_VEC4( M, C )    M[ C ]

    #define MAT3_SET_COLUMN( M, C, V )      M[ C ] = V;
    #define MAT4_SET_COLUMN( M, C, V )      M[ C ] = V;

#endif

// =================================================================================================
// Arrays (workaround AMD shader compiler issues by making arrays have global scope)
// =================================================================================================

#if defined(D_PLATFORM_OPENGL)

#define ARRAY_LOOKUP_FS( _UNIFORMS, _ELEMENT, _INDEX)   _ELEMENT[_INDEX]
#define ARRAY_LOOKUP_FP( _UNIFORMS, _ELEMENT, _INDEX)   _ELEMENT[_INDEX]

#elif defined(D_PLATFORM_DX12)

#define ARRAY_LOOKUP_FS( _UNIFORMS, _ELEMENT, _INDEX)   _UNIFORMS._ELEMENT[_INDEX]
#define ARRAY_LOOKUP_FP( _UNIFORMS, _ELEMENT, _INDEX)   _UNIFORMS._ELEMENT[_INDEX]

#elif defined(D_PLATFORM_ORBIS)

#define ARRAY_LOOKUP_FS( _UNIFORMS, _ELEMENT, _INDEX)   _UNIFORMS._ELEMENT[_INDEX]
#define ARRAY_LOOKUP_FP( _UNIFORMS, _ELEMENT, _INDEX)   _UNIFORMS->_ELEMENT[_INDEX]

#endif


// =================================================================================================
// Input and Output
// =================================================================================================

//interpolation:
//centroid    flat    smooth    noperspective 

#if defined( D_PLATFORM_PC_COMPUTE )

    #define DECLARE_INPUT                  
    #define DECLARE_END                     
    
  #ifdef D_USE_UBO

    #define DECLARE_UNIFORMS                
    #define DECLARE_UNIFORMS_END            
    #define DECLARE_PTR( TYPE, NAME )       uniform NAME ## _BLK { TYPE NAME; };
    #define UNIFORM( INST, VAR )            INST.VAR

    #define DEREF_UNIFORM( VAR )            VAR

  #else

    #define DECLARE_UNIFORMS                struct UniformBuffer {
    #define DECLARE_UNIFORMS_END            };
    #define DECLARE_PTR( TYPE, NAME )       TYPE NAME;
    #define UNIFORM( INST, VAR )            lUniforms.INST.VAR

    #define DEREF_UNIFORM( VAR )            lUniforms.VAR

  #endif


    #define INPUT_SCREEN_POSITION
    #define INPUT(   TYPE, NAME, REG )              
    #define INPUT_NOINTERP(   TYPE, NAME, REG )     

    #define IN_SCREEN_POSITION              

    #define WRITE_FRAGMENT_COLOUR( VAL )   imageStore( gOutTexture0, ivec2( dispatchThreadID ).xy, VAL ) 
    #define WRITE_FRAGMENT_COLOUR0( VAL )  imageStore( gOutTexture0, ivec2( dispatchThreadID ).xy, VAL ) 
    #define WRITE_FRAGMENT_COLOUR1( VAL )  imageStore( gOutTexture1, ivec2( dispatchThreadID ).xy, VAL ) 
    #define WRITE_FRAGMENT_COLOUR2( VAL )  imageStore( gOutTexture2, ivec2( dispatchThreadID ).xy, VAL ) 
    #define WRITE_FRAGMENT_COLOUR3( VAL )  imageStore( gOutTexture3, ivec2( dispatchThreadID ).xy, VAL ) 
    #define WRITE_FRAGMENT_COLOUR4( VAL )  imageStore( gOutTexture4, ivec2( dispatchThreadID ).xy, VAL ) 
    #define WRITE_FRAGMENT_DEPTH( VAL )    imageStore( gOutTextureDepth, ivec2( dispatchThreadID ).xy, VAL ) 

    #define FRAGMENT_COLOUR   imageLoad( SAMPLER_GETMAP( mpCmpOutPerMesh, gOutTexture0 ), ivec2( dispatchThreadID ).xy ); 
    #define FRAGMENT_COLOUR0  imageLoad( SAMPLER_GETMAP( mpCmpOutPerMesh, gOutTexture0 ), ivec2( dispatchThreadID ).xy ); 
    #define FRAGMENT_COLOUR1  imageLoad( SAMPLER_GETMAP( mpCmpOutPerMesh, gOutTexture1 ), ivec2( dispatchThreadID ).xy ); 
    #define FRAGMENT_COLOUR2  imageLoad( SAMPLER_GETMAP( mpCmpOutPerMesh, gOutTexture2 ), ivec2( dispatchThreadID ).xy ); 
    #define FRAGMENT_COLOUR3  imageLoad( SAMPLER_GETMAP( mpCmpOutPerMesh, gOutTexture3 ), ivec2( dispatchThreadID ).xy ); 
    #define FRAGMENT_COLOUR4  imageLoad( SAMPLER_GETMAP( mpCmpOutPerMesh, gOutTexture4 ), ivec2( dispatchThreadID ).xy ); 

    #define DEREF_PTR( VAR )                VAR


#elif defined( D_PLATFORM_OPENGL )

    #define DECLARE_INPUT
    #define DECLARE_OUTPUT
    #define DECLARE_END  
	#define DEREF_PTR( VAR )                VAR

  #ifdef D_USE_UBO

    #define DECLARE_UNIFORMS                
    #define DECLARE_UNIFORMS_END            
    #define DECLARE_PTR( TYPE, NAME )       uniform NAME ## _BLK { TYPE NAME; };
    #define UNIFORM( INST, VAR )            INST.VAR

    #define DEREF_UNIFORM( VAR )            VAR

    #define DECLARE_UNIFORM( TYPE, NAME )   uniform NAME ## _BLK { TYPE NAME; };

  #else

    #define DECLARE_UNIFORMS                struct UniformBuffer {
    #define DECLARE_UNIFORMS_END            };
    #define DECLARE_PTR( TYPE, NAME )       TYPE NAME;
    #define UNIFORM( INST, VAR )            lUniforms.INST.VAR

    #define DEREF_UNIFORM( VAR )            lUniforms.VAR

    #define DECLARE_UNIFORM( TYPE, NAME )   uniform TYPE NAME;

  #endif

    #define DECLARE_PATCH_INPUT_TRI
    #define DECLARE_PATCH_OUTPUT_TRI
    #define IN_PATCH_TRI_TESS_CONSTANTS
    #define OUT_PATCH_TRI_TESS_CONSTANTS

    #define DECLARE_PATCH_INPUT_QUAD
    #define DECLARE_PATCH_OUTPUT_QUAD
    #define IN_PATCH_QUAD_TESS_CONSTANTS
    #define OUT_PATCH_QUAD_TESS_CONSTANTS

  #if defined( D_HULL )

    #define INPUT(            TYPE, NAME, REG )   in          TYPE NAME [];
    #define PATCH_OUTPUT(     TYPE, NAME, REG )   patch out   TYPE NAME;
    #define OUTPUT(           TYPE, NAME, REG )   out         TYPE NAME [];

  #elif defined( D_DOMAIN )

    #define PATCH_INPUT(      TYPE, NAME, REG )   patch in    TYPE NAME;
    #define INPUT(            TYPE, NAME, REG )   in          TYPE NAME [];
    #define OUTPUT(           TYPE, NAME, REG )   out         TYPE NAME;

  #else

    #define INPUT(            TYPE, NAME, REG )   in          TYPE NAME;
    #define OUTPUT(           TYPE, NAME, REG )   out         TYPE NAME;

  #endif

    #define FRAGMENT_COLOUR_UVEC4_DEFINE    layout(location = 0) out uvec4 outu_color0;
    #define FRAGMENT_COLOUR_UVEC4           outu_color0

    #define FRAGMENT_COLOUR                 out_color0
    #define FRAGMENT_COLOUR0                out_color0
    #define FRAGMENT_COLOUR1                out_color1
    #define FRAGMENT_COLOUR2                out_color2
    #define FRAGMENT_COLOUR3                out_color3
    #define FRAGMENT_COLOUR4                out_color4
    #if defined( D_TAA_RENDER_TARGETS )	
        #define FRAGMENT_COLOUR_VEC4_DEFINE layout(location = 0) out vec4 out_color0;  layout(location = 1) out vec4 out_color1;  layout(location = 2) out vec4 out_color2;
    #elif defined( D_LIT_WITH_MASK )    
        #define FRAGMENT_COLOUR_VEC4_DEFINE layout(location = 0) out vec4 out_color0;  layout(location = 1) out vec4 out_color1;
    #elif !defined( D_ATTRIBUTES )
        #define FRAGMENT_COLOUR_VEC4_DEFINE layout(location = 0) out vec4 out_color0;
    #else
        #define FRAGMENT_COLOUR_VEC4_DEFINE layout(location = 0) out vec4 out_color0;  layout(location = 1) out vec4 out_color1;  layout(location = 2) out vec4 out_color2; layout(location = 3) out vec4 out_color3; layout(location = 4) out vec4 out_color4;
    #endif    
    #define FRAGMENT_COLOUR01_VEC4_DEFINE   layout(location = 0) out vec4 out_color0;  layout(location = 1) out vec4 out_color1;

    #define FRAGMENT_DEPTH                  gl_FragDepth
    #define FRAGMENT_FRONTFACE              gl_FrontFacing
    #define INPUT_FRONTFACING

    #define PRECISE                         precise
    #define INVARIANT       

    #define DEREF_PTR( VAR )                VAR

  #if defined( D_HULL ) 

    #define IN(  VAR )                      VAR[ gl_InvocationID ]
    #define OUT( VAR )                      VAR[ gl_InvocationID ]
    #define PATCH_IN( VAR, IND )            VAR[ IND ]
    #define PATCH_OUT( VAR )                VAR

    #define OUT_SCREEN_POSITION             gl_out[ gl_InvocationID ].gl_Position
    #define IN_SCREEN_POSITION              gl_in [ gl_InvocationID ].gl_Position

    #define TESS_LEVEL_EDGE( IND )          gl_TessLevelOuter[ IND ]
    #define TESS_LEVEL_INNER( IND )         gl_TessLevelInner[ IND ]

    #define HULL_TRI_CONSTANTS_SRT          void ConstantsHS(                           \
                                                UniformBuffer lUniforms )
    #define HULL_QUAD_CONSTANTS_SRT( PATCHCNT ) void ConstantsHS(                           \
                                                     UniformBuffer lUniforms )

    #define CALL_HULL_CONSTANTS             ConstantsHS( lUniforms );

  #elif defined( D_DOMAIN )

    #define IN(  VAR, IND )                 VAR[ IND ]
    #define OUT( VAR )                      VAR
    #define PATCH_IN(  VAR )                VAR

    #define OUT_SCREEN_POSITION             gl_Position
    #define IN_SCREEN_POSITION( IND )       gl_in [ IND ].gl_Position

    #define DOMAIN_COORDS                   gl_TessCoord

  #elif defined( D_GEOMETRY )

    #define IN(  VAR, IND )                 VAR[ IND ]
    #define OUT( VAR )                      VAR

    #define OUT_SCREEN_POSITION				gl_Position
    #define IN_SCREEN_POSITION(IND)			gl_in [ IND ].gl_Position

  #else

    #define IN(  VAR )                      VAR
    #define OUT( VAR )                      VAR

    #if defined( D_FRAGMENT )
    #define IN_SCREEN_POSITION              gl_FragCoord
    #else
    #define IN_SCREEN_POSITION
    #endif
    

  #endif

    #ifdef D_VERTEX
      #define OUTPUT_SCREEN_POSITION			OUTPUT( vec4, mScreenPositionVec4, S_POSITION )
      #define SCREEN_POSITION					gl_Position
      #define VSPS								OUTPUT
    #elif defined D_FRAGMENT
      #define INPUT_SCREEN_POSITION             INPUT( vec4, mScreenPositionVec4, S_POSITION )	
      #define SCREEN_POSITION					IN( mScreenPositionVec4 )
      #define VSPS      INPUT
    #elif defined( D_DOMAIN ) || defined( D_HULL )
      #define OUTPUT_SCREEN_POSITION  
      #define INPUT_SCREEN_POSITION  
    #endif

#else // !D_PLATFORM_OPENGL

    #define DECLARE_OUTPUT                  struct cOutput {
    #define DECLARE_INPUT                   struct cInput  {
    #define DECLARE_END                     };
    
    #define DECLARE_UNIFORMS                struct UniformBuffer {
    #define DECLARE_UNIFORMS_END            };
    
    #define DECLARE_UNIFORMS                struct UniformBuffer {
    #define DECLARE_UNIFORMS_END            };
    
    #define DECLARE_UNIFORMS                struct UniformBuffer {
    #define DECLARE_UNIFORMS_END            };

    #define INPUT(TYPE, NAME, REG )         TYPE NAME : REG;
    #define OUTPUT(TYPE, NAME, REG )		TYPE NAME : REG;    

  #ifdef D_PLATFORM_ORBIS

	#define	flat			nointerp
	#define	smooth			linear
	#define	noperspective	nopersp
    #define PRECISE         
    #define INVARIANT       __invariant

    #define DECLARE_PTR( TYPE, NAME )       TYPE* NAME;
	// TODO get rid of this - don't pass struct through functinos, pass members.
	#define DEREF_PTR( VAR )                *VAR
    #define DEREF_UNIFORM( VAR )            *lUniforms.VAR
    #define UNIFORM( INST, VAR )            lUniforms.INST->VAR
  #else
    
    #if defined( D_PLATFORM_DX12 ) && !defined( D_PLATFORM_XBOXONE )
    //these currently causes problems if defined on PC
    #define	flat		    
    #define	smooth			
    #else
	#define	flat			nointerpolation
	#define	smooth			linear
    #endif

    #define PRECISE         precise
    #define INVARIANT       

    #define DECLARE_PTR( TYPE, NAME )       TYPE NAME;
	#define DEREF_PTR( VAR )                VAR
    #define DEREF_UNIFORM( VAR )            lUniforms.VAR
    #define UNIFORM( INST, VAR )            lUniforms.INST.VAR
  #endif


  #if defined D_COMPUTE
  #if defined D_PLATFORM_ORBIS
	#define FRAGMENT_COLOUR                 lUniforms.mpCmpOutPerMesh->gOutTexture0[dispatchThreadID.xy] 
	#define FRAGMENT_COLOUR0                lUniforms.mpCmpOutPerMesh->gOutTexture0[dispatchThreadID.xy] 
	#define FRAGMENT_COLOUR1                lUniforms.mpCmpOutPerMesh->gOutTexture1[dispatchThreadID.xy] 
	#define FRAGMENT_COLOUR2                lUniforms.mpCmpOutPerMesh->gOutTexture2[dispatchThreadID.xy] 
	#define FRAGMENT_COLOUR3                lUniforms.mpCmpOutPerMesh->gOutTexture3[dispatchThreadID.xy] 
	#define FRAGMENT_COLOUR4                lUniforms.mpCmpOutPerMesh->gOutTexture4[dispatchThreadID.xy] 
    #define FRAGMENT_DEPTH                  lUniforms.mpCmpOutPerMesh->gOutTextureDepth[dispatchThreadID.xy] 
   #else						                
	#define FRAGMENT_COLOUR                 gOutTexture0[dispatchThreadID.xy] 
	#define FRAGMENT_COLOUR0                gOutTexture0[dispatchThreadID.xy] 
	#define FRAGMENT_COLOUR1                gOutTexture1[dispatchThreadID.xy] 
	#define FRAGMENT_COLOUR2                gOutTexture2[dispatchThreadID.xy] 
	#define FRAGMENT_COLOUR3                gOutTexture3[dispatchThreadID.xy] 
	#define FRAGMENT_COLOUR4                gOutTexture4[dispatchThreadID.xy] 
    #define FRAGMENT_DEPTH                  gOutTextureDepth[dispatchThreadID.xy] 
  #endif
  #else
  #ifdef D_FRAGMENT
	#define INPUT_FRONTFACING				INPUT( bool, mbFrontFacing, S_FRONT_FACE )
	#define FRAGMENT_FRONTFACE              In.mbFrontFacing
	#define FRAGMENT_COLOUR                 Out.mColour
	#define FRAGMENT_COLOUR0                Out.mColour0
	#define FRAGMENT_COLOUR1                Out.mColour1
	#define FRAGMENT_COLOUR2                Out.mColour2
	#define FRAGMENT_COLOUR3                Out.mColour3
	#define FRAGMENT_COLOUR4                Out.mColour4
	#define FRAGMENT_DEPTH                  Out.mDepth
	#define FRAGMENT_COLOUR_UVEC4           Out.mColour
  #else
	#define INPUT_FRONTFACING
  #endif
  #endif

  #if defined( D_HULL ) 

    #define IN(  VAR )                      In[ uCPID ].VAR
    #define OUT( VAR )                      Out.VAR
    #define PATCH_IN(VAR, IND)              In[ IND ].VAR
    #define PATCH_OUT( VAR )                Out.VAR

    #define IN_SCREEN_POSITION		        IN( mScreenPositionVec4 )
    #define OUT_SCREEN_POSITION             OUT( mScreenPositionVec4 )

    #define TESS_LEVEL_EDGE( IND )          Out.edge_ts[ IND ]
    #define TESS_LEVEL_INNER( IND )         Out.insi_ts[ IND ]

    #define gl_InvocationID                 uCPID

  #if defined D_PLATFORM_ORBIS
    #define DECLARE_PATCH_OUTPUT_TRI        [DOMAIN_PATCH_TYPE("tri")]  struct HSConstantOutputData {
    #define OUT_PATCH_TRI_TESS_CONSTANTS    float edge_ts[3]            : S_EDGE_TESS_FACTOR;    \
                                            float insi_ts[1]            : S_INSIDE_TESS_FACTOR;

    #define DECLARE_PATCH_OUTPUT_QUAD       [DOMAIN_PATCH_TYPE("quad")] struct HSConstantOutputData {
    #define OUT_PATCH_QUAD_TESS_CONSTANTS   float edge_ts[4]            : S_EDGE_TESS_FACTOR;    \
                                            float insi_ts[2]            : S_INSIDE_TESS_FACTOR;
   #else                                        
    #define DECLARE_PATCH_OUTPUT_TRI        struct HSConstantOutputData {
    #define OUT_PATCH_TRI_TESS_CONSTANTS    float edge_ts[3]            : SV_TessFactor;    \
                                            float insi_ts[1]            : SV_InsideTessFactor;

    #define DECLARE_PATCH_OUTPUT_QUAD       struct HSConstantOutputData {
    #define OUT_PATCH_QUAD_TESS_CONSTANTS   float edge_ts[4]            : SV_TessFactor;    \
                                            float insi_ts[2]            : SV_InsideTessFactor;
   #endif

    #define PATCH_OUTPUT(TYPE, NAME, REG )  TYPE NAME : REG;

    #define CALL_HULL_CONSTANTS


  #elif defined( D_DOMAIN )

    #define IN(VAR, IND)                    In[ IND ].VAR
    #define OUT(VAR)                        Out.VAR
	#define PATCH_IN(VAR)                   patchIn.VAR

    #define OUT_SCREEN_POSITION				OUT( mScreenPositionVec4 )
    #define IN_SCREEN_POSITION(IND)			IN( mScreenPositionVec4, IND )

    #define DOMAIN_COORDS                   domainCoordinates

  #if defined D_PLATFORM_ORBIS

    #define DECLARE_PATCH_INPUT_TRI         [DOMAIN_PATCH_TYPE("tri")]  struct HSConstantOutputData {
    #define IN_PATCH_TRI_TESS_CONSTANTS     float edge_ts[3]            : S_EDGE_TESS_FACTOR;    \
                                            float insi_ts[1]            : S_INSIDE_TESS_FACTOR;

    #define DECLARE_PATCH_INPUT_QUAD        [DOMAIN_PATCH_TYPE("quad")] struct HSConstantOutputData {
    #define IN_PATCH_QUAD_TESS_CONSTANTS    float edge_ts[4]            : S_EDGE_TESS_FACTOR;    \
                                            float insi_ts[2]            : S_INSIDE_TESS_FACTOR;
   #else                                        

    #define DECLARE_PATCH_INPUT_TRI         struct HSConstantOutputData {
    #define IN_PATCH_TRI_TESS_CONSTANTS     float edge_ts[3]            : SV_TessFactor;    \
                                            float insi_ts[1]            : SV_InsideTessFactor;

    #define DECLARE_PATCH_INPUT_QUAD        struct HSConstantOutputData {
    #define IN_PATCH_QUAD_TESS_CONSTANTS    float edge_ts[4]            : SV_TessFactor;    \
                                            float insi_ts[2]            : SV_InsideTessFactor;
   #endif

    #define PATCH_INPUT(TYPE, NAME, REG)    TYPE NAME : REG;

  #elif defined( D_GEOMETRY )

    #define IN(  VAR, IND )                 In[ IND ].VAR
    #define OUT( VAR )                      Out.VAR

    #define OUT_SCREEN_POSITION				OUT( mScreenPositionVec4 )
    #define IN_SCREEN_POSITION( IND )		IN( mScreenPositionVec4, IND )

    #define EMIT_VERTEX                     TriStream.Append( Out )
    #define END_PRIMITIVE                   TriStream.RestartStrip()

  #else

    #define IN(  VAR )                      In.VAR
    #define OUT( VAR )                      Out.VAR

  #if defined( D_FRAGMENT )                  
    #define IN_SCREEN_POSITION              IN( mScreenPositionVec4 )
  #endif

  #endif


    #ifndef D_FRAGMENT
      #define OUTPUT_SCREEN_POSITION            OUTPUT( vec4, mScreenPositionVec4, S_POSITION )
    #endif
    #ifndef D_VERTEX
      #define INPUT_SCREEN_POSITION             INPUT( vec4, mScreenPositionVec4, S_POSITION )  
    #endif

    #ifdef D_VERTEX
      #define SCREEN_POSITION                   OUT( mScreenPositionVec4 )
      #define VSPS                              OUTPUT
    #elif defined D_FRAGMENT
      #define SCREEN_POSITION					IN( mScreenPositionVec4 )
      #define VSPS      INPUT
    #endif

#endif



// =================================================================================================
// Main
// =================================================================================================
#if defined( D_PLATFORM_PC_COMPUTE )

  #ifdef D_USE_UBO
    #define UNIFORM_DECL                        
  #else
    #define UNIFORM_DECL                        uniform UniformBuffer lUniforms;
  #endif

    #define COMPUTE_MAIN_SRT( X, Y, Z )         layout (local_size_x = X, local_size_y = Y, local_size_z = Z) in; UNIFORM_DECL void main( void )

    #define FRAGMENT_MAIN_COLOUR_SRT            COMPUTE_MAIN_SRT( 8,8,1 )
    #define FRAGMENT_MAIN_COLOUR01_SRT          COMPUTE_MAIN_SRT( 8,8,1 )
    #define FRAGMENT_MAIN_COLOUR_DEPTH_SRT      COMPUTE_MAIN_SRT( 8,8,1 )

#elif defined( D_PLATFORM_OPENGL )

  #ifdef D_USE_UBO
    #define UNIFORM_DECL                        
  #else
	#define UNIFORM_DECL 						uniform UniformBuffer lUniforms;
  #endif

    #define VERTEX_MAIN                         void main( void )
    #define VERTEX_MAIN_SRT                     UNIFORM_DECL void main( void )

    #define HULL_TRI_MAIN_SRT                   layout( vertices = 3 ) out; UNIFORM_DECL void main( void )
    #define HULL_QUAD_MAIN_SRT( PATCHCNT )      layout( vertices = 4 ) out; UNIFORM_DECL void main( void )

    #define DOMAIN_TRI_MAIN_SRT                 layout( triangles, equal_spacing, ccw ) in; UNIFORM_DECL void main( void )
    #define DOMAIN_QUAD_MAIN_SRT                layout( quads, equal_spacing, ccw ) in; UNIFORM_DECL void main( void )

    #define FRAGMENT_MAIN_COLOUR                FRAGMENT_COLOUR_VEC4_DEFINE void main( void )
    #define VOID_MAIN_COLOUR                    FRAGMENT_COLOUR_VEC4_DEFINE void main( void )
    #define FRAGMENT_MAIN_COLOUR_DEPTH          FRAGMENT_COLOUR_VEC4_DEFINE void main( void )
    #define FRAGMENT_MAIN_COLOUR_DEPTH_GE_SRT   FRAGMENT_COLOUR_VEC4_DEFINE UNIFORM_DECL void main( void )
    #define FRAGMENT_MAIN_COLOUR_DEPTH_LE_SRT   FRAGMENT_COLOUR_VEC4_DEFINE UNIFORM_DECL void main( void )
    #define FRAGMENT_MAIN_COLOUR_EARLYZ_SRT     FRAGMENT_COLOUR_VEC4_DEFINE UNIFORM_DECL void main( void )

    #define VOID_MAIN_SRT                       FRAGMENT_COLOUR_VEC4_DEFINE UNIFORM_DECL void main( void )
    #define VOID_MAIN_DEPTH_SRT                 FRAGMENT_COLOUR_VEC4_DEFINE UNIFORM_DECL void main( void )

	#define FRAGMENT_MAIN_COLOUR_SRT            FRAGMENT_COLOUR_VEC4_DEFINE UNIFORM_DECL void main( void )
	#define FRAGMENT_MAIN_COLOUR_DEPTH_SRT      FRAGMENT_COLOUR_VEC4_DEFINE UNIFORM_DECL void main( void )
	#define FRAGMENT_MAIN_COLOUR01_SRT          FRAGMENT_COLOUR01_VEC4_DEFINE UNIFORM_DECL void main( void )
	#define FRAGMENT_MAIN_COLOUR01_DEPTH_SRT    FRAGMENT_COLOUR01_VEC4_DEFINE UNIFORM_DECL void main( void )
    #define FRAGMENT_MAIN_UICOLOUR_SRT          FRAGMENT_COLOUR_UVEC4_DEFINE UNIFORM_DECL void main( void )

    // icky workaround for fxc compiler not supporting output parameters on hull shaders >:[
    #define DECLARE_HULL_OUTPUT 
    #define RETURN_HULL_OUTPUT       

#else

  #ifdef D_PLATFORM_ORBIS
	#define DEF_SRT(S,N)			S N : S_SRT_DATA
	#define DEF_SRTP(S,N)			S *N : S_SRT_DATA
	#define UNIFORMS_CB
	#define UNIFORMS_SRT			, UniformBuffer lUniforms : S_SRT_DATA
	#define ROOT_SIG(P)
	#define FRAG					[RE_Z] 
	#define COMPUTE_PARAMS(X,Y,Z)	[NUM_THREADS(X, Y, Z)]

    #define HULL_PARAMS(domain, partitioning, topology, points, maxtess)    [DOMAIN_PATCH_TYPE(#domain)] [PARTITIONING_TYPE(#partitioning)] [OUTPUT_TOPOLOGY_TYPE(#topology)] [OUTPUT_CONTROL_POINTS(points)] [PATCH_CONSTANT_FUNC("ConstantsHS")] [MAX_TESS_FACTOR(maxtess)]
    #define DOMAIN_PARAMS(domain)                                           [DOMAIN_PATCH_TYPE(#domain)]
  #else
	#define DEF_SRT(S,N)			cbuffer _##S : register(b0) { S N; }
	#define DEF_SRTP(S,N)			DEF_SRT(S,N)
	#define UNIFORMS_CB				DEF_SRT(UniformBuffer, lUniforms);
	#define UNIFORMS_SRT
	#define ROOT_SIG(P)				[RootSignature(ROOT_SIG_##P)]
	#define FRAG					
	#define COMPUTE_PARAMS(X,Y,Z)	[numthreads(X, Y, Z)] ROOT_SIG(CS) 
    #define S_OUTPUT_CONTROL_POINT_ID SV_OutputControlPointID
    #define S_DOMAIN_LOCATION           SV_DomainLocation
    #define HULL_PARAMS(dmn, prt, topology, points, maxtess)    ROOT_SIG(HS) [domain(#dmn)] [partitioning(#prt)] [outputtopology(#topology)] [outputcontrolpoints(points)] [patchconstantfunc("ConstantsHS")] [maxtessfactor(maxtess)]
    #define DOMAIN_PARAMS(dmn)                                  ROOT_SIG(DS) [domain(#dmn)]
  #endif

    #define VOID_MAIN_SRT    					UNIFORMS_CB                                                     	ROOT_SIG(PS) void main(cInput In UNIFORMS_SRT)
	#define VOID_MAIN_COLOUR					UNIFORMS_CB struct cOutput { vec4  mColour : S_TARGET_OUTPUT; };	ROOT_SIG(PS) void main(cInput In, out cOutput Out )
	#define VERTEX_MAIN																								ROOT_SIG(VS) void main(cInput In, out cOutput Out )
    #define VERTEX_MAIN_SRT						UNIFORMS_CB															ROOT_SIG(VS) void main(cInput In, out cOutput Out UNIFORMS_SRT)
    #define HULL_TRI_MAIN_SRT					HULL_PARAMS(tri,integer,triangle_cw,3,32.0)		                    cOutput main(InputPatch<cInput, 3> In, uint uCPID : S_OUTPUT_CONTROL_POINT_ID UNIFORMS_SRT)
	#define HULL_QUAD_MAIN_SRT( PATCHCNT )		HULL_PARAMS(quad,integer,triangle_cw,4,32.0)		                cOutput main(InputPatch<cInput, PATCHCNT> In, uint uCPID : S_OUTPUT_CONTROL_POINT_ID UNIFORMS_SRT)
    #define DOMAIN_TRI_MAIN_SRT					UNIFORMS_CB DOMAIN_PARAMS(tri)										void main(HSConstantOutputData patchIn, const OutputPatch<cInput, 3> In, out cOutput Out, float3 domainCoordinates : S_DOMAIN_LOCATION UNIFORMS_SRT)
	#define DOMAIN_QUAD_MAIN_SRT				UNIFORMS_CB DOMAIN_PARAMS(quad)										void main(HSConstantOutputData patchIn, const OutputPatch<cInput, 4> In, out cOutput Out, float2 domainCoordinates : S_DOMAIN_LOCATION UNIFORMS_SRT)
    #define GEOMETRY_MAIN_SRT(MAX_VERTS)		UNIFORMS_CB cOutput Out; [MAX_VERTEX_COUNT(MAX_VERTS)]				void main(inout TriangleBuffer<cOutput> TriStream, Triangle cInput In[3] UNIFORMS_SRT)
	#define FRAGMENT_MAIN_COLOUR				UNIFORMS_CB struct cOutput { vec4  mColour : S_TARGET_OUTPUT; };									ROOT_SIG(PS) FRAG void main(cInput In, out cOutput Out UNIFORMS_SRT)
	#define FRAGMENT_MAIN_COLOUR_DEPTH			UNIFORMS_CB struct cOutput { vec4  mColour : S_TARGET_OUTPUT; float mDepth  : S_DEPTH_OUTPUT; };	ROOT_SIG(PS) FRAG void main(cInput In, out cOutput Out UNIFORMS_SRT)
    #define HULL_TRI_CONSTANTS_SRT              UNIFORMS_CB void ConstantsHS( InputPatch<cInput, 3> In, out HSConstantOutputData Out UNIFORMS_SRT )
    #define HULL_QUAD_CONSTANTS_SRT( PATCHCNT ) UNIFORMS_CB void ConstantsHS( InputPatch<cInput, PATCHCNT> In, out HSConstantOutputData Out UNIFORMS_SRT )

    // icky workaround for fxc compiler not supporting output parameters on hull shaders >:[
    #define DECLARE_HULL_OUTPUT cOutput Out
    #define RETURN_HULL_OUTPUT  return Out

  #if defined D_COMPUTE

	#define COMPUTE_MAIN_SRT( X, Y, Z )			UNIFORMS_CB COMPUTE_PARAMS(X, Y, Z) void main(uint3 groupID : S_GROUP_ID, uint3 groupThreadID : S_GROUP_THREAD_ID, uint3 dispatchThreadID : S_DISPATCH_THREAD_ID UNIFORMS_SRT)
	#define FRAGMENT_MAIN_COLOUR_SRT			COMPUTE_MAIN_SRT( 8,8,1 )
	#define FRAGMENT_MAIN_COLOUR01_SRT			COMPUTE_MAIN_SRT( 8,8,1 )
	#define FRAGMENT_MAIN_COLOUR_DEPTH_SRT		COMPUTE_MAIN_SRT( 8,8,1 )
    #define FRAGMENT_MAIN_UICOLOUR_SRT          COMPUTE_MAIN_SRT( 8,8,1 )
    #define VOID_MAIN_DEPTH_SRT         		COMPUTE_MAIN_SRT( 8,8,1 )

  #elif defined( D_TAA_RENDER_TARGETS )		   
											   
	#define FRAGMENT_MAIN_COLOUR_SRT			UNIFORMS_CB struct cOutput { vec4  mColour0 : S_TARGET_OUTPUT0; vec4  mColour1 : S_TARGET_OUTPUT1; vec4  mColour2 : S_TARGET_OUTPUT2; }; ROOT_SIG(PS) [RE_Z] void main(cInput In, out cOutput Out UNIFORMS_SRT)

  #elif defined( D_LIT_WITH_MASK )		   
    
    #define FRAGMENT_MAIN_COLOUR_DEPTH_SRT		UNIFORMS_CB struct cOutput { vec4  mColour0 : S_TARGET_OUTPUT0; vec4  mColour1 : S_TARGET_OUTPUT1; float mDepth : S_DEPTH_OUTPUT;    }; ROOT_SIG(PS) FRAG	  void main(cInput In, out cOutput Out UNIFORMS_SRT)
    #define FRAGMENT_MAIN_COLOUR_SRT			UNIFORMS_CB struct cOutput { vec4  mColour0 : S_TARGET_OUTPUT0; vec4  mColour1 : S_TARGET_OUTPUT1; };                                   ROOT_SIG(PS)          void main(cInput In, out cOutput Out UNIFORMS_SRT)
	
  #elif !defined( D_ATTRIBUTES )

	#define FRAGMENT_MAIN_COLOUR_DEPTH_SRT		UNIFORMS_CB struct cOutput { vec4 mColour : S_TARGET_OUTPUT; float mDepth : S_DEPTH_OUTPUT;    }; ROOT_SIG(PS) FRAG	        void main(cInput In, out cOutput Out UNIFORMS_SRT)
	#define FRAGMENT_MAIN_COLOUR_DEPTH_GE_SRT	UNIFORMS_CB struct cOutput { vec4 mColour : S_TARGET_OUTPUT; float mDepth : S_DEPTH_GE_OUTPUT; }; ROOT_SIG(PS) FRAG	        void main(cInput In, out cOutput Out UNIFORMS_SRT)
	#define FRAGMENT_MAIN_COLOUR_DEPTH_LE_SRT	UNIFORMS_CB struct cOutput { vec4 mColour : S_TARGET_OUTPUT; float mDepth : S_DEPTH_LE_OUTPUT; }; ROOT_SIG(PS) FRAG	        void main(cInput In, out cOutput Out UNIFORMS_SRT)
	#define FRAGMENT_MAIN_UICOLOUR_SRT			UNIFORMS_CB struct cOutput { uint4 mColour : S_TARGET_OUTPUT; };	                              ROOT_SIG(PS)              void main(cInput In, out cOutput Out UNIFORMS_SRT)
    #define FRAGMENT_MAIN_COLOUR_SRT			UNIFORMS_CB struct cOutput { vec4  mColour : S_TARGET_OUTPUT; };	                              ROOT_SIG(PS)              void main(cInput In, out cOutput Out UNIFORMS_SRT)
	#define VOID_MAIN_DEPTH_SRT					UNIFORMS_CB struct cOutput {                                 float mDepth : S_DEPTH_OUTPUT;    }; ROOT_SIG(PS) FRAG 	    void main(cInput In, out cOutput Out UNIFORMS_SRT)
	#define VOID_MAIN_COLOUR_EARLYZ_SRT			UNIFORMS_CB 	                                				               	 ROOT_SIG(PS) [FORCE_EARLY_DEPTH_STENCIL]   void main(cInput In UNIFORMS_SRT )
	#define FRAGMENT_MAIN_COLOUR01_SRT			UNIFORMS_CB struct cOutput { vec4  mColour0 : S_TARGET_OUTPUT0; vec4  mColour1 : S_TARGET_OUTPUT1; };  ROOT_SIG(PS) FRAG    void main(cInput In, out cOutput Out UNIFORMS_SRT)
	#define FRAGMENT_MAIN_COLOUR01_DEPTH_SRT	UNIFORMS_CB struct cOutput { vec4  mColour0 : S_TARGET_OUTPUT0; vec4  mColour1 : S_TARGET_OUTPUT1;float mDepth : S_DEPTH_OUTPUT; }; ROOT_SIG(PS) FRAG void main( cInput In, out cOutput Out UNIFORMS_SRT  )

  #else
        // #pragma PSSL_target_output_format(target 1 FMT_32_AR)
	#define FRAGMENT_MAIN_COLOUR_DEPTH_SRT		UNIFORMS_CB struct cOutput { vec4  mColour0 : S_TARGET_OUTPUT0; vec4  mColour1 : S_TARGET_OUTPUT1; vec4  mColour2 : S_TARGET_OUTPUT2; vec4  mColour3 : S_TARGET_OUTPUT3; float mDepth   : S_DEPTH_OUTPUT; }; ROOT_SIG(PS) [RE_Z] void main( cInput In, out cOutput Out UNIFORMS_SRT  )
	#define FRAGMENT_MAIN_COLOUR_SRT			UNIFORMS_CB struct cOutput { vec4  mColour0 : S_TARGET_OUTPUT0; vec4  mColour1 : S_TARGET_OUTPUT1; vec4  mColour2 : S_TARGET_OUTPUT2; vec4  mColour3 : S_TARGET_OUTPUT3; }; ROOT_SIG(PS) [RE_Z] void main( cInput In, out cOutput Out UNIFORMS_SRT )
	#define FRAGMENT_MAIN_COLOUR_EARLYZ_SRT		UNIFORMS_CB struct cOutput { vec4  mColour0 : S_TARGET_OUTPUT0; vec4  mColour1 : S_TARGET_OUTPUT1; vec4  mColour2 : S_TARGET_OUTPUT2; vec4  mColour3 : S_TARGET_OUTPUT3; }; ROOT_SIG(PS)  [FORCE_EARLY_DEPTH_STENCIL] void main( cInput In, out cOutput Out UNIFORMS_SRT )                                                    
  #endif

#endif


bool WaveIsFirstLane()
{
#if defined( D_PLATFORM_OPENGL) && defined( GL_ARB_shader_ballot )
    return gl_SubGroupInvocationARB == readFirstInvocationARB( gl_SubGroupInvocationARB );
#elif defined( D_PLATFORM_ORBIS )
    ulong exec = __s_read_exec();
    ulong lane_mask = exec^(exec&(exec-1));
    return __v_cndmask_b32(0, 1, lane_mask);
#elif defined( D_PLATFORM_XBOXONE )
    return __XB_GetLaneID() == __XB_S_FF1_U64(__XB_GetEntryActiveMask64());
#else
    return true;
#endif
}

// =================================================================================================
// Texture resolution
// =================================================================================================

#if defined( D_PLATFORM_OPENGL)
 
#define GetTexResolution( TEX ) textureSize( TEX, 0 )
#define GetImgResolution( IMG ) imageSize( IMG )

#elif defined D_PLATFORM_DX12

uvec2 GetTexResolution(Texture2D lTexture)
{
    uvec2 lResolution;
    lTexture.GetDimensions(lResolution.x, lResolution.y);
    return lResolution;
}

uvec2 GetTexResolution(Texture2D<rgba32ui> lTexture )
{
    uvec2 lResolution;
    lTexture.GetDimensions(lResolution.x, lResolution.y);
    return lResolution;
}

uvec2 GetImgResolution(RW_Texture2D<float4> lTexture)
{
    uvec2 lResolution;
    lTexture.GetDimensions(lResolution.x, lResolution.y);
    return lResolution;
}

uvec2 GetImgResolution(RW_Texture2D<int> lTexture)
{
    uvec2 lResolution;
    lTexture.GetDimensions(lResolution.x, lResolution.y);
    return lResolution;
}

#elif defined D_PLATFORM_ORBIS

uvec2 GetTexResolution(Texture2D lTexture )
{
    uvec2 lResolution;
    lTexture.GetDimensionsFast(lResolution.x, lResolution.y);
    return lResolution;
}

uvec2 GetTexResolution(Texture2D<rgba32ui> lTexture )
{
    uvec2 lResolution;
    lTexture.GetDimensionsFast(lResolution.x, lResolution.y);
    return lResolution;
}

uvec2 GetImgResolution(RW_Texture2D<float4> lTexture )
{
    uvec2 lResolution;
    lTexture.GetDimensionsFast(lResolution.x, lResolution.y);
    return lResolution;
}

uvec2 GetImgResolution(RW_Texture2D<int> lTexture )
{
    uvec2 lResolution;
    lTexture.GetDimensionsFast(lResolution.x, lResolution.y);
    return lResolution;
}

#endif

// =================================================================================================
// Viewport
// =================================================================================================
#ifdef D_PLATFORM_OPENGL

	vec2 SCREENSPACE_AS_RENDERTARGET_UVS(vec2 A) { return A; }
	vec2 DSCREENSPACE_AS_RENDERTARGET_UVS(vec2 A) { return A; }

#else

	float2 SCREENSPACE_AS_RENDERTARGET_UVS(float2 A) { return float2(A.x, 1.0 - A.y); }
	float2 DSCREENSPACE_AS_RENDERTARGET_UVS(float2 A) { return float2(A.x, -A.y); }

#endif

    #define D_DEPTH_CLEARVALUE (0.0)

// =================================================================================================
// Texture usage feedback
// =================================================================================================
#if defined( D_TEXTURE_FEEDBACK )
	
#ifdef D_PLATFORM_OPENGL

layout(r32i) uniform  iimage2D gTexFeedbackImg;

void WriteTexFeedback( in int liCounter, in float liMip )
{
    if( liCounter != 0 )
    {
        #if defined( GL_ARB_shader_ballot ) && ( GL_ARB_shader_ballot == 1 )
        if( readFirstInvocationARB( gl_SubGroupInvocationARB ) == gl_SubGroupInvocationARB )
        #endif
        {
            int liIntMip = int(floor(liMip));
            //imageStore( gTexFeedbackImg, ivec2( liCounter, liIntMip ), ivec4(1,0,0,0) );
            imageAtomicAdd( gTexFeedbackImg, ivec2( liCounter, liIntMip ), int(1) );
        }
    }
}

vec4 Tex2dFeedback( in sampler2D lSamp, in int liCounter, in vec2 lCoords )
{
    float liLod = textureQueryLOD( lSamp, lCoords ).x;
    WriteTexFeedback( liCounter, liLod );
    return texture( lSamp, lCoords );
}

vec4 Tex2dLodFeedback( in sampler2D lSamp, in int liCounter, in vec2 lCoords, in float liLod )
{
    WriteTexFeedback( liCounter, liLod );
    return textureLod( lSamp, lCoords, liLod );
}

vec4 Tex2dArrayFeedback( in sampler2DArray lSamp, in int liCounter, in vec3 lCoords )
{
    float liLod = textureQueryLOD( lSamp, lCoords.xy ).x;
    WriteTexFeedback( liCounter, liLod );
    return texture( lSamp, lCoords );
}

vec4 Tex3dFeedback( in sampler3D lSamp, in int liCounter, in vec3 lCoords )
{
    float liLod = textureQueryLOD( lSamp, lCoords ).x;
    WriteTexFeedback( liCounter, liLod );
    return texture( lSamp, lCoords );
}

vec4 Tex3dLodFeedback( in sampler3D lSamp, in int liCounter, in vec3 lCoords, in float liLod )
{
    WriteTexFeedback( liCounter, liLod );
    return textureLod( lSamp, lCoords, liLod );
}

#else

RW_Texture2D<uint> gTexFeedbackImg;

void WriteTexFeedback( in int counter, in float mip )
{
    if ( counter != 0 )
		InterlockedAdd(gTexFeedbackImg[int2(counter, int(mip))], 1);
}

vec4 Tex2dFeedback(in Texture2D tex, in SamplerState samp, int counter, in vec2 coords )
{
    WriteTexFeedback( counter, tex.CalculateLevelOfDetail(samp, coords));
    return tex.Sample(samp, coords);
}

vec4 Tex2dLodFeedback(in Texture2D tex, in SamplerState samp, int counter, in vec2 coords, in float lod )
{
    WriteTexFeedback( counter, lod);
    return tex.SampleLOD(samp, coords, lod);
}

vec4 Tex2dArrayFeedback( in Texture2D tex, SamplerState samp, in int counter, in vec3 coords )
{
    WriteTexFeedback(counter, tex.CalculateLevelOfDetail(samp, coords));
    return tex.Sample(samp, coords);
}

vec4 Tex3dFeedback( in Texture3D tex, in SamplerState samp, in int counter, in vec3 coords )
{
    WriteTexFeedback( counter, tex.CalculateLevelOfDetail(samp, coords));
    return tex.Sample(samp, coords);
}

vec4 Tex3dLodFeedback( in Texture3D tex, in SamplerState samp, in int counter, in vec3 coords, in float lod )
{
    WriteTexFeedback( counter, lod);
    return tex.SampleLOD(samp, coords, lod);
}

#endif

#endif

#endif	//D_DEFINES
