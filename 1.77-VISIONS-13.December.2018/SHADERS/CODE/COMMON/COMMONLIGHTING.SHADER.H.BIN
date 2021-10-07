////////////////////////////////////////////////////////////////////////////////
///
///     @file       CommonLighting.h
///     @author     User
///     @date       
///
///     @brief      CommonLighting
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 

#ifndef D_COMMONLIGHTING_H
#define D_COMMONLIGHTING_H

//-----------------------------------------------------------------------------
//      Include files

#include "Common/Common.shader.h"
#include "Common/CommonDepth.shader.h"
#include "Common/CommonPlanet.shader.h"
#include "Common/CommonUniforms.shader.h"
#include "Common/CommonFragment.shader.h"
#include "Common/CommonGBuffer.shader.h"

//#define D_DEBUG_NO_COLOUR
//#define D_DEBUG_AMBIENT_ONLY
//-----------------------------------------------------------------------------
//      Global Data

#if !defined( D_NO_SHADOWS )

 STATIC_CONST float kfSHCubemapBrightnessScale = 0.45;
 STATIC_CONST float kfMaxShadowIntensity       = 0.5;

 STATIC_CONST vec4 kShadowPoissonTapsVec4[] =
 {
    vec4( 0.000000f, 0.000000f, 0.0, 0.0 ),
    vec4( 0.527837f,-0.085868f, 0.0, 0.0 ),
    vec4(-0.040088f, 0.536087f, 0.0, 0.0 ),
    vec4(-0.670445f,-0.179949f, 0.0, 0.0 ),
    vec4(-0.419418f,-0.616039f, 0.0, 0.0 ),
    vec4( 0.440453f,-0.639399f, 0.0, 0.0 ),
    vec4(-0.757088f, 0.349334f, 0.0, 0.0 ),
    vec4( 0.574619f, 0.685879f, 0.0, 0.0 )
} ;

#define D_NUMBER_OF_CASCADES (3.0)
#define D_CASCADE_SIZE (1.0 / D_NUMBER_OF_CASCADES)

#define D_INSIDE_SM_BOUNDS1( V, TEXEL_SIZE ) ( ( max(V.x,V.y) < (1.0-(TEXEL_SIZE*1.0)) ) && ( min(V.x,V.y) > (TEXEL_SIZE*1.0) ) && ( V.z < 1.0 ) && ( V.z >= 0.0 ) )
#define D_INSIDE_SM_BOUNDS2( V, TEXEL_SIZE ) ( ( max(V.x,V.y) < (1.0-(TEXEL_SIZE*1.0)) ) && ( min(V.x,V.y) > (TEXEL_SIZE*1.0) ) && ( V.z < 1.0 ) && ( V.z >= 0.0 ) )
#define D_INSIDE_SM_BOUNDS3( V, TEXEL_SIZE ) ( ( max(V.x,V.y) < (1.0-(TEXEL_SIZE*1.0)) ) && ( min(V.x,V.y) > (TEXEL_SIZE*1.0) ) && ( V.z < 1.0 ) && ( V.z >= 0.0 ) )

//-----------------------------------------------------------------------------
//      Typedefs and Classes 


//-----------------------------------------------------------------------------
//    Functions




//-----------------------------------------------------------------------------
///
///     SampleShadowMap_RPDB
///
///     @brief      SampleShadowMap_RPDB
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------

// This method is used in the Witness

float 
SampleShadowMap_RPDB(CustomPerMaterialUniforms lPerMaterialUniforms, in vec2 base_uv, in float u, in float v, in vec2 shadowMapSizeInv, in float depth, in vec2 receiverPlaneDepthBias, in float lfCascade)
{
    vec2 uv = base_uv + vec2(u, v) * shadowMapSizeInv;
    float z = depth + dot(vec2(u, v), receiverPlaneDepthBias);
    #ifdef D_ENABLE_REVERSEZ_PROJECTION    
    z = 1.0 - z;
    #endif
    z = clamp(z, 0.0, 1.0);
    uv.x = (uv.x + lfCascade) * D_CASCADE_SIZE;
#ifdef D_PLATFORM_DX12
    return shadow2D( gShadowMap, vec3(uv, z) ).x;
#else
    return shadow2D( lPerMaterialUniforms.gShadowMap, vec3(uv, z) ).x;
#endif
}

float
PCF_RPDB2(
    in CustomPerMaterialUniforms lPerMaterialUniforms,
    in CommonPerMeshUniforms   lUniforms,
    vec3                    lTexCoordVec3,
    in vec2                 lReceiverPlaneDepthBiasVec2,
    in vec4                 lShadowMapSize,
    in float                lfCascade,
    in bool                 lbHQ)
{   
    vec2 base_uv;
    vec2 uv;
    uv.x = lTexCoordVec3.x * lShadowMapSize.x;
    uv.y = lTexCoordVec3.y * lShadowMapSize.y;

    vec2 shadowMapSizeInv;
    shadowMapSizeInv.x = lShadowMapSize.z;
    shadowMapSizeInv.y = lShadowMapSize.w;
    base_uv.x = floor(uv.x + 0.5);
    base_uv.y = floor(uv.y + 0.5);
    float s = (uv.x + 0.5 - base_uv.x);
    float t = (uv.y + 0.5 - base_uv.y);
    base_uv -= vec2(0.5, 0.5);
    base_uv *= shadowMapSizeInv;

    float lightDepth = lTexCoordVec3.z;

    float sum = 0.0;

#ifndef D_PLATFORM_PC_LOWEND
    if (lfCascade == 0.0 && lbHQ)
    {
    float uw0 = (5 * s - 6);
    float uw1 = (11 * s - 28);
    float uw2 = -(11 * s + 17);
    float uw3 = -(5 * s + 1);

    float u0 = (4 * s - 5) / uw0 - 3;
    float u1 = (4 * s - 16) / uw1 - 1;
    float u2 = -(7 * s + 5) / uw2 + 1;
    float u3 = -s / uw3 + 3;

    float vw0 = (5 * t - 6);
    float vw1 = (11 * t - 28);
    float vw2 = -(11 * t + 17);
    float vw3 = -(5 * t + 1);

    float v0 = (4 * t - 5) / vw0 - 3;
    float v1 = (4 * t - 16) / vw1 - 1;
    float v2 = -(7 * t + 5) / vw2 + 1;
    float v3 = -t / vw3 + 3;

    sum += uw0 * vw0 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u0, v0, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);
    sum += uw1 * vw0 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u1, v0, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);
    sum += uw2 * vw0 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u2, v0, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);
    sum += uw3 * vw0 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u3, v0, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);

    sum += uw0 * vw1 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u0, v1, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);
    sum += uw1 * vw1 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u1, v1, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);
    sum += uw2 * vw1 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u2, v1, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);
    sum += uw3 * vw1 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u3, v1, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);

    sum += uw0 * vw2 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u0, v2, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);
    sum += uw1 * vw2 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u1, v2, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);
    sum += uw2 * vw2 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u2, v2, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);
    sum += uw3 * vw2 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u3, v2, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);

    sum += uw0 * vw3 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u0, v3, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);
    sum += uw1 * vw3 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u1, v3, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);
    sum += uw2 * vw3 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u2, v3, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);
    sum += uw3 * vw3 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u3, v3, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);

        return sum * 1.0 / 2704.0;
    }
    else if (lfCascade == 1.0 && lbHQ)
    {
    float uw0 = (4 - 3 * s);
    float uw1 = 7;
    float uw2 = (1 + 3 * s);

    float u0 = (3 - 2 * s) / uw0 - 2;
    float u1 = (3 + s) / uw1;
    float u2 = s / uw2 + 2;

    float vw0 = (4 - 3 * t);
    float vw1 = 7;
    float vw2 = (1 + 3 * t);

    float v0 = (3 - 2 * t) / vw0 - 2;
    float v1 = (3 + t) / vw1;
    float v2 = t / vw2 + 2;
    
    sum += uw0 * vw0 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u0, v0, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);
    sum += uw1 * vw0 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u1, v0, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);
    sum += uw2 * vw0 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u2, v0, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);

    sum += uw0 * vw1 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u0, v1, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);
    sum += uw1 * vw1 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u1, v1, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);
    sum += uw2 * vw1 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u2, v1, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);

    sum += uw0 * vw2 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u0, v2, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);
    sum += uw1 * vw2 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u1, v2, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);
    sum += uw2 * vw2 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u2, v2, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);

        return sum * 1.0 / 144.0;    
    }
    else
#endif
    {
        float uw0 = (3 - 2 * s);
        float uw1 = (1 + 2 * s);

        float u0 = (2 - s) / uw0 - 1;
        float u1 = s / uw1 + 1;

        float vw0 = (3 - 2 * t);
        float vw1 = (1 + 2 * t);

        float v0 = (2 - t) / vw0 - 1;
        float v1 = t / vw1 + 1;

        shadowMapSizeInv *= 0.5f;   //

        sum += uw0 * vw0 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u0, v0, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);
        sum += uw1 * vw0 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u1, v0, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);
        sum += uw0 * vw1 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u0, v1, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);
        sum += uw1 * vw1 * SampleShadowMap_RPDB(lPerMaterialUniforms, base_uv, u1, v1, shadowMapSizeInv, lightDepth, lReceiverPlaneDepthBiasVec2, lfCascade);

        return sum * 1.0 / 16.0;
    }
}


//-----------------------------------------------------------------------------
///
///     ComputeShadowIntensity
///
///     @brief      ComputeShadowIntensity
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------

vec2 ComputeReceiverPlaneDepthBias(vec3 texCoordDX, vec3 texCoordDY)
{
    vec2 biasUV;
    biasUV.x = texCoordDY.y * texCoordDX.z - texCoordDX.y * texCoordDY.z;
    biasUV.y = texCoordDX.x * texCoordDY.z - texCoordDY.x * texCoordDX.z;
    float Det =(texCoordDX.x * texCoordDY.y) - (texCoordDX.y * texCoordDY.x);
#ifndef D_PLATFORM_ORBIS
    if (Det == 0.0)
    {
        return vec2(0.0, 0.0);
    }
#endif
    biasUV *= 1.0 / ( sign(Det) * max(abs(Det), 1e-8) );
    return biasUV;
}

float
ComputeShadowIntensity(
    in CustomPerMaterialUniforms   lPerMaterialUniforms,
    in PerFrameUniforms            lRenderTargetUniforms,
    in CommonPerMeshUniforms       lUniforms,
    in vec3                     lPositionVec3,
    in vec3                     lNormalVec3,
    in vec2                     lScreenPositionVec2,
    in bool                     lbHQ )
{
    vec3   lProjectedPos;
    float lfDepth = 1.0;
    float lfTexelSize = lRenderTargetUniforms.gShadowSizeVec4.w; // 1.0 / lRenderTargetUniforms.gShadowSizeVec4.y;
    float lfCascade = 0.0;
    float lfShadowFade = 0.0;
    float lfAutoBias = lRenderTargetUniforms.gShadowProjScaleVec4[0].z;
    float lfAutoBiasBase = 0.08;
    lProjectedPos = MUL( lUniforms.gaShadowMat4[ 0 ], vec4( lPositionVec3, 1.0 ) ).xyz;
    
    // Check if you are outside the high detail shadow cascade
    if( !D_INSIDE_SM_BOUNDS1( ( lProjectedPos.xyz ), lfTexelSize ) ) 
    {
        lProjectedPos = MUL( lUniforms.gaShadowMat4[ 1 ], vec4( lPositionVec3, 1.0 ) ).xyz;		


        if (!D_INSIDE_SM_BOUNDS2( (lProjectedPos.xyz), lfTexelSize) )
        {
            lProjectedPos = MUL( lUniforms.gaShadowMat4[ 2 ], vec4(lPositionVec3, 1.0) ).xyz;

            if (!D_INSIDE_SM_BOUNDS3( (lProjectedPos.xyz), lfTexelSize) )
            {
                // outside all cascades
                return 1.0;
            }
            else
            {   // inside cascade 2
                lfCascade = 2.0;
                lfAutoBias = lRenderTargetUniforms.gShadowProjScaleVec4[2].z;
                lfAutoBiasBase = 0.004;
                lfShadowFade = clamp( (length(lRenderTargetUniforms.gViewPositionVec3-lPositionVec3)-lRenderTargetUniforms.gShadowFadeParamVec4.x ) * lRenderTargetUniforms.gShadowFadeParamVec4.y , 0.0, 1.0);
            }
        }
        else
        {   
            // inside cascade 1
            lfCascade = 1.0;
            lfAutoBias = lRenderTargetUniforms.gShadowProjScaleVec4[1].z;
            lfAutoBiasBase = 0.017;
        }
    }
    //inside cascade 0 (or cascade 1,2 fall through)  

     vec3 lProjectedPosS = lProjectedPos;
     vec3 lShadowPosDDXVec3 = dFdxFine(lProjectedPosS);
     vec3 lShadowPosDDYVec3 = dFdyFine(lProjectedPosS);
     
     vec2 lReceiverPlaneDepthBiasVec2 = ComputeReceiverPlaneDepthBias(lShadowPosDDXVec3, lShadowPosDDYVec3);
     lReceiverPlaneDepthBiasVec2 *= vec2(lRenderTargetUniforms.gShadowSizeVec4.z, lRenderTargetUniforms.gShadowSizeVec4.w);
     // Static depth biasing to make up for incorrect fractional sampling on the shadow map grid
     float fractionalSamplingError = dot(vec2(2.0, 2.0), abs(lReceiverPlaneDepthBiasVec2));
     // dynamic bias, based off Z view space range
     fractionalSamplingError += lfAutoBiasBase / lfAutoBias;

     lProjectedPos.z -= fractionalSamplingError;

     float lfShadow = PCF_RPDB2(lPerMaterialUniforms, lUniforms, lProjectedPos, lReceiverPlaneDepthBiasVec2, lRenderTargetUniforms.gShadowSizeVec4, lfCascade, lbHQ);
     return mix( lfShadow, 1.0, lfShadowFade );
     
 
}

#endif

#ifndef D_DEFER

//-----------------------------------------------------------------------------
///
///     EnvBRDFApprox
///
///     @brief      EnvBRDFApprox
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec3 EnvBRDFApprox( vec3 SpecularColor, float Roughness, float NoV )
{
    // [ Lazarov 2013, "Getting More Physical in Call of Duty: Black Ops II" ]
    // Adaptation to fit our G term.
    const vec4 c0 = vec4( -1, -0.0275, -0.572, 0.022 );
    const vec4 c1 = vec4( 1, 0.0425, 1.04, -0.04 );
    vec4 r = Roughness * c0 + c1;
    float a004 = min( r.x * r.x, exp2( -9.28 * NoV ) ) * r.x + r.y;
    vec2 AB = vec2( -1.04, 1.04 ) * a004 + r.zw;

    return SpecularColor * AB.x + AB.y;
}


//-----------------------------------------------------------------------------
///
///     PhongApprox
///
///     @brief      PhongApprox
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
float 
PhongApprox( 
    float Roughness, 
    float RoL )
{
    //float a = Roughness * Roughness;			// 1 mul
    //float a2 = a * a;						// 1 mul
    //float rcp_a2 = rcp(a2);					// 1 rcp
    float rcp_a2 = exp2( -6.88886882 * Roughness + 6.88886882 );
    //float rcp_a2 = 1.0 / a2;					// 1 rcp

    // Spherical Gaussian approximation: pow( x, n ) ~= exp( (n + 0.775) * (x - 1) )
    // Phong: n = 0.5 / a2 - 0.5
    // 0.5 / ln(2), 0.275 / ln(2)
    vec2 c = vec2( 0.72134752, 0.25 ) * rcp_a2 + vec2( 0.39674113, 0.75 );	// 1 mad
    return c.y * exp2( c.x * RoL - c.x );	// 2 mad, 1 exp2, 1 mul
    // Total 7 instr
}

//-----------------------------------------------------------------------------
///
///     GetImageBasedReflectionLighting
///
///     @brief      GetImageBasedReflectionLighting
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec3 GetImageBasedReflectionLighting(
    SAMPLER2DARG( lDualPMapBack ) ,
    SAMPLER2DARG( lDualPMapFront ) ,
    vec3 lReflectionVec3, 
    float Roughness)
{
    vec3 ProjectedCaptureVector = lReflectionVec3;	

    // Compute fractional mip from roughness
    //float AbsoluteSpecularMip = ComputeReflectionCaptureMipFromRoughness(Roughness);
    // Fetch from cubemap and convert to linear HDR

    vec3 SpecularIBL = ReadDualParaboloidMap(  SAMPLER2DPARAM( lDualPMapBack ), 
                                               SAMPLER2DPARAM( lDualPMapFront ), 
                                               ProjectedCaptureVector,
                                               int (min(Roughness, 0.99) * 7.0) ).xyz;
    return SpecularIBL;
}



STATIC_CONST float fLTDistortion     = 0.200000; // Translucency Distortion Scale Factor 
STATIC_CONST float fLTScale          = 1.000000; // Scale Factor 
STATIC_CONST float fLTPower          = 4.000000; // Power Factor
STATIC_CONST float fLTAmbient        = 0.000000; // Minimum front and back translucency
STATIC_CONST float fLTAmbientClamped = 0.250000; // Minimum front and back translucency


//-----------------------------------------------------------------------------
///
///     SubsurfaceScatter
///
///     @brief      SubsurfaceScatter
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
float 
SubsurfaceScatter(
    vec3   lViewDirVec3,
    vec3   lNormalVec3,
    vec3   lLightDirectionVec3,
    float  lfSubsurfaceFactor,
    float  lfAmbient ) 
{
    //float dot  = pow( saturate( dot( lViewDirVec3, -lLightDirectionVec3 ) ), fLTPower ) * fLTScale;

    float lfTest = saturate( dot( lViewDirVec3, -lLightDirectionVec3 ) );
    float lfDot  = pow( lfTest, fLTPower ) * fLTScale;
    float lt   = (lfDot + lfAmbient) * lfSubsurfaceFactor;

    return lt;
}

STATIC_CONST float kfRimFactor = 0.5;


//-----------------------------------------------------------------------------
///
///     ComputeLightColour
///
///     @brief      ComputeLightColour
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
float
ComputeCloudOverlay(
    in vec2       lCloudTexCoords,
    SAMPLER2DARG(lCloudShadowMap))
{
    return texture2D(lCloudShadowMap, lCloudTexCoords).r;
}

//-----------------------------------------------------------------------------
///
///     ComputeLightColour
///
///     @brief      ComputeLightColour
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec3 
ComputeLightColour( 
    in CustomPerMaterialUniforms lPerMaterialUniforms,
    in PerFrameUniforms         lPerFrameUniforms,
    in CommonPerMeshUniforms    lMeshUniforms,
	SAMPLER2DARG( lDualPMapBack ), 
    SAMPLER2DARG( lDualPMapFront ),
    in vec3                     lLightDirectionVec3,
    in vec3                     lLightColourVec3,
    in vec3                     lPositionVec3,
    in vec2                     lScreenPositionVec2,
    in vec3                     lNormalVec3,
    in vec3                     lInputColourVec3,
    in mat3                     lUpMatrix,
    in int                      liMaterialID,
    in float                    lfMetallic,
    in float                    lfRoughness,
    in float                    lfSubsurfaceFactor,
    in float                    lfNonMetalSpecularScale,
    in float                    lfOcclusion,
    inout vec3                  lFinalSun
     )
{
    vec3   lDiffuseColourVec3;
    vec3   lSpecularColourVec3;
    vec3   lViewDirVec3;

#ifdef D_SPLIT_SHADOW
    vec3   lFinalAmbient = float2vec3(0.0);
#else
    vec3   lFinalColour = float2vec3(0.0);
  #if defined( D_NO_SHADOWS )
    float  lfShadow      = 0.25;
  #else
    float  lfShadow      = 1.0;
  #endif
#endif // D_SPLIT_SHADOW

    float  lfNoV;
    float  lfNoL;
    vec3   lColourVec3 = lInputColourVec3;

    vec3  lWorldUpVec3  = GetWorldUp( lPositionVec3, lMeshUniforms.gLightOriginVec4 );
    float lfHeight      = dot( lWorldUpVec3, lNormalVec3 );

    lViewDirVec3        = normalize( lPerFrameUniforms.gViewPositionVec3 - lPositionVec3.xyz );
    
    lfNoV = max( 0.0, dot( lNormalVec3, lViewDirVec3        ) );
    lfNoL = max( 0.0, dot( lNormalVec3, lLightDirectionVec3 ) );

#ifdef _F48_WARPED_DIFFUSE_LIGHTING
    lfNoL = max( 0.0, (dot( lNormalVec3, lLightDirectionVec3 ) + 1.0)*0.5 );    
#endif   
    
#if !defined( D_SPOTLIGHT ) && !defined( D_NO_SHADOWS )

    vec2 lCloudScreenPositionVec2 = lScreenPositionVec2 * 0.5 + 0.5;
    #ifndef D_PLATFORM_OPENGL
    lCloudScreenPositionVec2.y = 1.0 - lCloudScreenPositionVec2.y;
    #endif

    float lfOverlayValue = ComputeCloudOverlay(lCloudScreenPositionVec2, SAMPLER2DPARAM_LOCAL(lPerMaterialUniforms, gCloudShadowMap));

  #ifndef D_SPLIT_SHADOW
    if ( (liMaterialID & D_NORECEIVESHADOW) == 0 )
    {   
        if ( (liMaterialID & D_EMBEDDED_SHADOW) != 0 )
        {
            lfShadow = lfSubsurfaceFactor;
            lfSubsurfaceFactor = 0.0;    
        } 
    #ifndef D_LIGHT_TERRAIN
        else
        {   
            lfShadow = ComputeShadowIntensity( lPerMaterialUniforms, lPerFrameUniforms, lMeshUniforms, lPositionVec3, lNormalVec3, lScreenPositionVec2, true );
        }
    #endif
        lfShadow *= lfOverlayValue;
    }
  #endif // D_SPLIT_SHADOW

#endif //!defined( D_SPOTLIGHT ) && !defined( D_NO_SHADOWS )

    {
        float DielectricSpecular = 0.08 * lfNonMetalSpecularScale;
        
        lDiffuseColourVec3  = lColourVec3 - (lColourVec3 * lfMetallic);	// 1 mad
        lSpecularColourVec3 = (DielectricSpecular - (DielectricSpecular * lfMetallic)) + (lColourVec3 * lfMetallic);	// 2 mad
    }

    {
        lSpecularColourVec3 = EnvBRDFApprox( lSpecularColourVec3, lfRoughness, lfNoV );
    }

    {
        vec3   lReflectionVec3     = reflect( -lViewDirVec3, lNormalVec3 );

        float  lfRoL = max( 0.0, dot( lReflectionVec3,  lLightDirectionVec3 ) );

        float lfSubSurface = 0.0;

        #ifdef _F40_SUBSURFACE_MASK
        {       
            float lfAmbient = fLTAmbient;
            if( ( liMaterialID & D_CLAMPAMBIENT ) != 0 )
            {
                lfAmbient = fLTAmbientClamped;
            }

            lfSubSurface  = SubsurfaceScatter( lViewDirVec3, lNormalVec3, lLightDirectionVec3, lfSubsurfaceFactor, lfAmbient );   
        }
        #endif

        #ifdef D_SPLIT_SHADOW
        lFinalAmbient += lfSubSurface * 0.2 * lLightColourVec3;
        lFinalSun     += (lfNoL + lfSubSurface * 0.8) * lLightColourVec3;  
        #else
        lfSubSurface *= 0.2 + (lfShadow * 0.8);
        //lFinalColour += ((lfShadow * lfNoL ) + lfSubSurface) * lLightColourVec3 * ((lDiffuseColourVec3 * lfOcclusion) + lSpecularColourVec3 * PhongApprox( lfRoughness, lfRoL ) );  
        lFinalColour += ((lfShadow * lfNoL ) + lfSubSurface) * lLightColourVec3;  
        #endif

        #ifdef D_DEBUG_NO_AMBIENT
        {
            return lFinalColour;
        }
        #endif

        vec3 lFresnelColVec3 = float2vec3( 0.0 );
        vec3 lAmbientColVec3 = float2vec3( 0.0 );

    #ifndef D_SPOTLIGHT
        {
            vec3 lTransformedRelectionVec3 = MUL( lUpMatrix, lReflectionVec3 );
            vec3 lTransformedNormalVec3    = MUL( lUpMatrix, lNormalVec3 );

        #ifdef D_PLATFORM_OPENGL
            lTransformedRelectionVec3   = -lTransformedRelectionVec3;
            lTransformedNormalVec3      = -lTransformedNormalVec3;
        #endif
            vec3 SpecularIBL = GetImageBasedReflectionLighting( SAMPLER2DPARAM( lDualPMapBack ), 
                                                                SAMPLER2DPARAM( lDualPMapFront ),
                                                                lTransformedRelectionVec3, 
                                                                lfRoughness);


            vec3 AmbientIBL = GetImageBasedReflectionLighting( SAMPLER2DPARAM( lDualPMapBack ),
                                                               SAMPLER2DPARAM( lDualPMapFront ),
                                                               lTransformedNormalVec3,
                                                               0.5 );

            // Ambient
            if ( ( liMaterialID & D_DISABLE_AMBIENT ) != 0 )            	
            {
                // branching around the whole ambient section, lowers PS4 shader occupancy            	
                AmbientIBL = float2vec3(0.0);
            }
            
            float lfBounceLight;
            float lfSkyLight;

            lfBounceLight     = saturate( ( ( -lfHeight )+ 1.0 ) * 0.5 );
            lfBounceLight     = pow( lfBounceLight, 4.0 ) * 1.0;

            lfSkyLight        = saturate( ( lfHeight + 1.0 ) * 0.5 );
            lfSkyLight        = pow( lfSkyLight, 2.0 ) * 1.0;

            //lAmbientColVec3 += AmbientIBL * lColourVec3 * ( lfBounceLight + lfSkyLight );
            vec3 lSkyColour  = lPerMaterialUniforms.gLightTopColourVec4.xyz;
            lAmbientColVec3 += lSkyColour * lfSkyLight;
            lAmbientColVec3 += AmbientIBL * lfBounceLight;
            
        #ifdef D_DEBUG_NO_COLOUR
             return AmbientIBL * ( lfBounceLight + lfSkyLight );
        #endif
                  
            // Rim
            /*
            {
                float lfNormalDotCamera;

                lfNormalDotCamera  = dot( lNormalVec3, -lViewDirVec3 );
                lfNormalDotCamera  = max( lfNormalDotCamera + 1.0, 0.0 ) / ( 1.0 +  1.0 );

                float lfSideRim = lfNormalDotCamera;

                lfSideRim *= 1.0 - (max( lfHeight, 0.0 ) + max( -lfHeight, 0.0 ));

                vec3 lRimColour = mix( lDiffuseColourVec3, SpecularIBL, 0.5 ) * lfSideRim;

                float lfTopRim = saturate( ( (lfHeight)+1.0 ) * 0.5 );
                lfTopRim = pow( lfTopRim, 30.0 ) * 0.3;

                lRimColour += mix( lDiffuseColourVec3, SpecularIBL, 1.0 ) * lfTopRim * lfShadow;

                #ifdef D_DEBUG_NO_AMBIENT
                {
                    return lRimColour;
                }
                #endif

                lFresnelColVec3 += lRimColour;
                //lAmbientColVec3 += lRimColour;
            }
            */

            lFresnelColVec3 = SpecularIBL * lSpecularColourVec3;

        #ifdef D_DEBUG_AMBIENT_ONLY    
            return lAmbientColVec3;
        #endif
        }
    #endif

        #ifdef D_SPLIT_SHADOW
        lFinalAmbient += lAmbientColVec3 * (0.5 + ((lfRoughness) * 0.5));
        lFinalAmbient *= lDiffuseColourVec3 + lSpecularColourVec3 * PhongApprox( lfRoughness, lfRoL );
        lFinalSun     *= lDiffuseColourVec3 + lSpecularColourVec3 * PhongApprox( lfRoughness, lfRoL );

        lFinalSun     += lFresnelColVec3;

        lFinalAmbient = mix( lDiffuseColourVec3 * lAmbientColVec3 * 0.5, lFinalAmbient, lfOcclusion );
        lFinalSun     = mix( float2vec3(0.0),                            lFinalSun,     lfOcclusion );
        #else
        lFinalColour += lAmbientColVec3 * (0.5 + ((lfRoughness) * 0.5));
        lFinalColour *= lDiffuseColourVec3 + lSpecularColourVec3 * PhongApprox( lfRoughness, lfRoL );

        lFinalColour += lfShadow * lFresnelColVec3;

        lFinalColour = mix( lDiffuseColourVec3 * lAmbientColVec3 * 0.5, lFinalColour, lfOcclusion );
        #endif


    }

    #ifdef D_SPLIT_SHADOW
    return lFinalAmbient;
    #else
    return lFinalColour;
    #endif
}

#endif	// D_DEFER

#endif	// D_COMMONLIGHTING_H

































