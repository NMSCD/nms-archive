#ifdef D_PLATFORM_ORBIS
#pragma loop(unroll:always)
#endif

#ifndef D_FRAGMENT
#define D_FRAGMENT
#endif
#include "Common/Defines.shader.h"
#include "Common/CommonUniforms.shader.h"
#include "Common/CommonFragment.shader.h"

#include "Fullscreen/PostCommon.h"
#ifndef M_PI
#define M_PI 3.141592653589793
#endif

#ifdef D_PLATFORM_OPENGL

#if 0

// re-enable this path if SAMPLES is ever set above 256 in the future.

float hammersley_radical_inverse_VdC(uint bits) 
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

#else

// Hammersley Radical inverse limited to 8bits.

float hammersley_radical_inverse_VdC(uint bits) 
{
	bits = (((bits * 0x0802u & 0x22110u) | (bits * 0x8020u & 0x88440u)) * 0x10101u) & 0x00ff0000u;
    return float(bits) * (1.0/16777216.0); 
}

#endif

#else

#if defined D_PLATFORM_XBOXONE
#define ReverseBits __XB_S_BREV_U32
#elif defined D_PLATFORM_DX12
#define ReverseBits reversebits
#endif

float hammersley_radical_inverse_VdC(uint bits) 
{
	return float(ReverseBits(bits)) * 2.3283064365386963e-10; // / 0x100000000
}

#endif


vec2 Hammersley(uint i, uint N) 
{
   return vec2(float(i)/float(N), hammersley_radical_inverse_VdC(i));
}


// VERIFIED
vec3 ImportanceSampleGGX( vec2 Xi, float Roughness, vec3 N )
{
    float a = Roughness * Roughness;
    float Phi = 2 * M_PI * Xi.x;
    float CosTheta = sqrt( (1 - Xi.y) / ( 1 + (a*a - 1) * Xi.y ) );
    float SinTheta = sqrt( 1 - CosTheta * CosTheta );
    vec3 H;
    H.x = SinTheta * cos( Phi );
    H.y = SinTheta * sin( Phi );
    H.z = CosTheta;
    vec3 UpVector = abs(N.z) < 0.999 ? vec3(0,0,1) : vec3(1,0,0);
    vec3 TangentX = normalize( cross( UpVector, N ) );
    vec3 TangentY = cross( N, TangentX );
    // Tangent to world space
    return TangentX * H.x + TangentY * H.y + N * H.z;
}

vec3 
PrefilterEnvMap( 
    in float      Roughness, 
    in vec3       R, 
    in uint       NumSamples,
    in int        MipLevel,
    SAMPLER2DARG( lDualPMapBack ) ,
    SAMPLER2DARG( lDualPMapFront ) )
{
    vec3 N = R;
    vec3 V = R;
    vec3 PrefilteredColor = float2vec3(0.0);

    float TotalWeight = 0.0;

    for( uint i = 0U; i < NumSamples; i++ )
    {
        vec2 Xi = Hammersley( i, NumSamples );
        vec3 H = ImportanceSampleGGX( Xi, Roughness, N );
        vec3 L = 2 * dot( V, H ) * H - V;
        float NoL = saturate( dot( N, L ) );
        if( NoL > 0 )
        {
            //PrefilteredColor += EnvMap.SampleLevel( EnvMapSampler, L, 0 ).rgb * NoL;
            PrefilteredColor += ReadDualParaboloidMap(  SAMPLER2DPARAM( lDualPMapBack ), 
                                                        SAMPLER2DPARAM( lDualPMapFront ), 
                                                        L, MipLevel ).xyz  * NoL;
			//PrefilteredColor += L * NoL;			
            TotalWeight += NoL;
        }
    }

    return PrefilteredColor / TotalWeight;
}

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2,     TEXCOORD0 )

DECLARE_END


//-----------------------------------------------------------------------------
//      Functions
 
FRAGMENT_MAIN_COLOUR_SRT
{
    vec3 lDirection;

    lDirection.xy = (vec2(IN( mTexCoordsVec2 ).x, 1.0 - IN( mTexCoordsVec2 ).y) - 0.5) * 2.0;

    lDirection.z = 0.5 - (0.5 * ( (lDirection.x * lDirection.x) + (lDirection.y * lDirection.y) ));

    lDirection.xz *= lUniforms.mpCommonPerMesh->gGenericParam0Vec4.w;

	uint lNumSamples = uint( lUniforms.mpCommonPerMesh->gGenericParam0Vec4.z );

	if (lNumSamples == 1)
	{
	    FRAGMENT_COLOUR = vec4( PrefilterEnvMap(    lUniforms.mpCommonPerMesh->gGenericParam0Vec4.x, 
	                                                normalize(lDirection), 
	                                                1, 
	                                                int(  lUniforms.mpCommonPerMesh->gGenericParam0Vec4.y ),
                                                    SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMesh,gDualPMapBack),
                                                    SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMesh,gDualPMapFront)),

                                                    1.0 );
	} 
	else
	{
	    FRAGMENT_COLOUR = vec4( PrefilterEnvMap(    lUniforms.mpCommonPerMesh->gGenericParam0Vec4.x, 
	                                                normalize(lDirection), 
#if defined( D_PLATFORM_OPENGL ) && !defined( D_PLATFORM_PC_LOWEND )     	                                                
	                                                128, 
#else	                                                
	                                                64, 
#endif	                                                
	                                                int(  lUniforms.mpCommonPerMesh->gGenericParam0Vec4.y ),
	                                                SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gDualPMapBack ), 
	                                                SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gDualPMapFront ) ), 
	                                                1.0 );		
	}	                                               
                                                

}
