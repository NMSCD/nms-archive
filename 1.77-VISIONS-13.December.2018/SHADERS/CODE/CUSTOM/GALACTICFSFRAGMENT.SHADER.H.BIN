////////////////////////////////////////////////////////////////////////////////
///
///     @file       GalacticBG.h
///     @author     hdd
///     @date       
/// 
///     @brief      Experiments
///
///     Copyright (c) 2014 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 


//-----------------------------------------------------------------------------
//      Include files

#ifndef D_FRAGMENT
#define D_FRAGMENT
#endif
#include "Common/Defines.shader.h"
//#include "Common/CommonUniforms.shader.h"
#include "Common/Common.Shader.h"
#include "Common/CommonRandom.Shader.h"
#include "Common/CommonIntersection.Shader.h"
#include "Common/Wombat/SimplexPerlin3D.glsl"
#include "Common/Wombat/Cellular3D.glsl"
#include "Common/Wombat/Value3D.glsl"

#include "Custom/GalaxyCommon.h"

//-----------------------------------------------------------------------------
//      Global Data

//-----------------------------------------------------------------------------
//      Typedefs and Classes 

//-----------------------------------------------------------------------------
///
///     Input
///
///     @brief  Input
///
//-----------------------------------------------------------------------------
DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2,     TEXCOORD0 )

DECLARE_END


//-----------------------------------------------------------------------------
//    Functions

vec3 ComputeRaymarchDir( vec2 lUV , TracingFsFxUniforms lTracingUniforms )
{
  vec3 lvRaymarchDir   = ( lTracingUniforms.gTracePDX.xyz * (1.0 - lUV.x) ) + lTracingUniforms.gTraceScreenCenter.xyz;
       lvRaymarchDir  += ( lTracingUniforms.gTracePDY.xyz * (1.0 - lUV.y) ) ;
       lvRaymarchDir   = normalize( lvRaymarchDir );

  return lvRaymarchDir;
}


vec3 LookupCLUTDual( float lfThroughClut, float lfBetweenClut, ColourPalettes lColourPalettes )
{
  float lfBaseRow     = lfThroughClut * 9.0;
  float lfBaseRowFrac = fract( lfBaseRow );

  int liFromRowIndex  = int( floor(lfBaseRow) );
  int liToRowIndex    = int(  ceil(lfBaseRow) );

  return mix(
#ifndef D_PLATFORM_OPENGL
         mix( lColourPalettes.gLargeAreaPrimaryLUT[liFromRowIndex].rgb,    lColourPalettes.gLargeAreaPrimaryLUT[liToRowIndex].rgb,   lfBaseRowFrac),
         mix( lColourPalettes.gLargeAreaSecondaryLUT[liFromRowIndex].rgb,  lColourPalettes.gLargeAreaSecondaryLUT[liToRowIndex].rgb, lfBaseRowFrac),
#else
         mix(gLargeAreaPrimaryLUT[liFromRowIndex].rgb, gLargeAreaPrimaryLUT[liToRowIndex].rgb, lfBaseRowFrac),
         mix(gLargeAreaSecondaryLUT[liFromRowIndex].rgb, gLargeAreaSecondaryLUT[liToRowIndex].rgb, lfBaseRowFrac),
#endif
         lfBetweenClut);
}


vec3 LookupCLUT( float lfThroughClut, ColourPalettes lColourPalettes )
{
  float lfBaseRow     = lfThroughClut * 9.0;
  float lfBaseRowFrac = fract( lfBaseRow );

  int liFromRowIndex  = int( floor(lfBaseRow) );
  int liToRowIndex    = int(  ceil(lfBaseRow) );

  return mix(
#ifndef D_PLATFORM_OPENGL
      lColourPalettes.gLargeAreaPrimaryLUT[liFromRowIndex].rgb, 
      lColourPalettes.gLargeAreaPrimaryLUT[liToRowIndex].rgb, 
#else
      gLargeAreaPrimaryLUT[liFromRowIndex].rgb,
      gLargeAreaPrimaryLUT[liToRowIndex].rgb,
#endif
      lfBaseRowFrac);
}


float Vignette( vec2 uv, FsMapUniforms lMapUniforms )
{
  uv = (uv - 0.5) * lMapUniforms.gVignetteLensFlareConfig.x;
  return clamp( cos(uv.x * M_PI) * cos(uv.y * M_PI), lMapUniforms.gVignetteLensFlareConfig.y, 1.0 );
}


vec3 ComputeLensflare(vec2 uv, vec2 pos, vec4 spread, float inf, float stretch)
{
  vec2 uvd = uv * ( length(uv) * stretch );

  vec3 edge;
  edge.r  = (1.0 / (1.0 + 32.0 * pow( length(uvd+spread.x * pos), spread.w) ) )   * 1.02;
  edge.g  = (1.0 / (1.0 + 32.0 * pow( length(uvd+spread.y * pos), spread.w) ) )   * 0.94;
  edge.b  = (1.0 / (1.0 + 32.0 * pow( length(uvd+spread.z * pos), spread.w) ) )   * 0.90;
  
  return saturate(edge) * inf;
}


vec3 ComputeGoalVirtualPlane( vec3 lvRaymarchDir, TracingFsFxUniforms lTracingUniforms )
{
    vec3 lvGoalDir    = lTracingUniforms.gGoalCenterDir.xyz;
    vec3 lvGoalPerpA  = lTracingUniforms.gGoalCenterPerpU.xyz;
    vec3 lvGoalPerpB  = lTracingUniforms.gGoalCenterPerpV.xyz;

    // intersect with place facing from goal
    float lfT   = dot( lvGoalDir,   lvRaymarchDir );

    // generate UV for lens fx
    float lfU   = dot( lvGoalPerpA, lvRaymarchDir );
    float lfV   = dot( lvGoalPerpB, lvRaymarchDir );

    return vec3( saturate(lfT), lfU, lfV );
}



//----------------------------------------------------------------------------------------------------------------------------------------------
// bg colour

#ifdef D_GALACTIC_BACKGROUND


FRAGMENT_MAIN_COLOUR_SRT
{
    vec2   lPixelCoordsVec2 = SCREENSPACE_AS_RENDERTARGET_UVS(TEX_COORDS);

    float lcfScreenX        = lUniforms.mpTracingUniformsPerMesh->gScreen.x;
    float lcfScreenY        = lUniforms.mpTracingUniformsPerMesh->gScreen.y;
    float lcfScreenAspect   = lUniforms.mpTracingUniformsPerMesh->gScreen.z;

    vec3 lvRaymarchDir = ComputeRaymarchDir( lPixelCoordsVec2, DEREF_PTR( lUniforms.mpTracingUniformsPerMesh ) );
    vec3 lvCentralMarchDir = ComputeRaymarchDir( vec2(0.5, 0.5), DEREF_PTR( lUniforms.mpTracingUniformsPerMesh ) );

    float tGal = dot(lvRaymarchDir, vec3(0.0f, 0.0f, 1.0f));

    float tGalacticCenter = saturate((tGal + 1.0) * 0.5);

    float tCellScaleTowardsCenter =  lUniforms.mpFsMapUniformsPerMesh->gBGCellConfig.x - ( tGalacticCenter * (lUniforms.mpFsMapUniformsPerMesh->gBGCellConfig.x * 0.6) );
    float tCellStructure = 0.25f;// Cellular3D( (lvRaymarchDir * tCellScaleTowardsCenter) + (lUniforms.mpTracingUniformsPerMesh->gGalacticScale.xyz * lUniforms.mpFsMapUniformsPerMesh->gBGCellConfig.y) );

    vec3 bgRootPoint  = ( lUniforms.mpTracingUniformsPerMesh->gGalacticScale.xyz * 100.0 );
         bgRootPoint += lvCentralMarchDir;

    float bgCluster   = Value3D( bgRootPoint * lUniforms.mpFsMapUniformsPerMesh->gBGCellConfig.w );
    vec3 bgClusterColour = LookupCLUT( bgCluster, DEREF_PTR( lUniforms.mpColourPalettesPerMesh ) );

    vec3 bgBase       = bgClusterColour * lUniforms.mpFsMapUniformsPerMesh->gBGColourScales.x;

    float cCellHorizonInf = lUniforms.mpFsMapUniformsPerMesh->gBGCellConfig.z;
    float cCellHorizonInfHalf = cCellHorizonInf * -0.5;
    float cCellZ = ( cCellHorizonInfHalf + (tCellStructure * cCellHorizonInf) ) * saturate(0.9 - tGalacticCenter);
    float tHorizon = dot(lvRaymarchDir, vec3(0.0, 1.0, cCellZ));

    float zDir = 1.0 - abs(tHorizon);

    zDir = saturate(zDir);
    float tDir = pow(zDir, 4.0);

    vec3 fringeCol = mix(
        bgBase * lUniforms.mpFsMapUniformsPerMesh->gBGColourScales.y,
        bgBase * lUniforms.mpFsMapUniformsPerMesh->gBGColourScales.z,
        tGalacticCenter);

    vec3 outputCol = mix(
        bgBase * lUniforms.mpFsMapUniformsPerMesh->gBGColourScales.w,
        fringeCol,
        tDir + (tCellStructure * tCellStructure * (tDir * tDir * tDir) * lUniforms.mpFsMapUniformsPerMesh->gBGColourConfig.x)
        );

    outputCol = mix( vec3(0.5, 0.5, 0.5), outputCol, 1.3 );

    vec4 lOutputColour = vec4( saturate(outputCol), 1.0 );


    vec3 lvGoalTUV      = ComputeGoalVirtualPlane( lvRaymarchDir, DEREF_PTR( lUniforms.mpTracingUniformsPerMesh ) );
    float lfGoalSize    = lUniforms.mpTracingUniformsPerMesh->gGoalCenterDir.w;



    vec4 lNebulaeColour = texture2DLod( SAMPLER_GETMAP( lUniforms.mpFsMapUniformsPerMesh, gNebulaeMap), TEX_COORDS, 0.0);
    vec4 lAtmosColour   = texture2DLod( SAMPLER_GETMAP( lUniforms.mpFsMapUniformsPerMesh, gAtmosMap),   TEX_COORDS, 0.0);


    float lfInvNebulaeSrcAlpha = 1.0 - lNebulaeColour.a;
    {
      // D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA
      lOutputColour = (lOutputColour * lfInvNebulaeSrcAlpha) + lNebulaeColour;
    }
    {
      lOutputColour.rgb += lAtmosColour.rgb * lUniforms.mpFsMapUniformsPerMesh->gBGColourConfig.z;
    }

    {
      // galactic sun overlay; always visible through everything - bg effect will add the rest
      float galSunPulse = mix(lUniforms.mpFsMapUniformsPerMesh->gSunCoreConfig.x, lUniforms.mpFsMapUniformsPerMesh->gSunCoreConfig.y, lUniforms.mpTracingUniformsPerMesh->gInterest.z);

      float lfRescale = 1.5 - lfGoalSize;

      float galSunCore = 1.0 / ( length( vec2(-lvGoalTUV.y, lvGoalTUV.z) ) * ( galSunPulse * lfRescale ) );
      galSunCore *= lUniforms.mpFsMapUniformsPerMesh->gSunCoreConfig.z * lvGoalTUV.x;

      lOutputColour = mix(
            lOutputColour,
            lUniforms.mpFsMapUniformsPerMesh->gSunCoreColour,
            saturate( galSunCore * lfInvNebulaeSrcAlpha ) );
    }

    {
      float lfLensFlareStrength = lUniforms.mpFsMapUniformsPerMesh->gBGColourConfig.w + (lUniforms.mpTracingUniformsPerMesh->gInterest.y * 0.3);
      float lfLensFlareInf      = lvGoalTUV.x;
      float lfLensStretch       = mix(lUniforms.mpFsMapUniformsPerMesh->gVignetteLensFlareConfig.z, lUniforms.mpFsMapUniformsPerMesh->gVignetteLensFlareConfig.w, 1.0 - lvGoalTUV.x);

      vec2 lvLensFlareUV        = vec2( (lPixelCoordsVec2.x - 0.5) * lcfScreenAspect, (lPixelCoordsVec2.y - 0.5) );
      vec2 lvLensFlareLocation  = vec2( -lvGoalTUV.y, lvGoalTUV.z );

      lOutputColour.rgb +=
          lUniforms.mpFsMapUniformsPerMesh->gBGLensFlareColour.rgb *
          ComputeLensflare(lvLensFlareUV, lvLensFlareLocation, lUniforms.mpFsMapUniformsPerMesh->gBGLensFlareSpread, lfLensFlareInf, lfLensStretch) *
          lfLensFlareStrength;
          /*lfGoalSize;*/
    }

    // final composition adjustment

    lOutputColour.rgb = BrightnessVibranceContrast(
      lOutputColour.rgb, 
      lUniforms.mpFsMapUniformsPerMesh->gCompositeControlBVCG.x,
      lUniforms.mpFsMapUniformsPerMesh->gCompositeControlBVCG.y,
      lUniforms.mpFsMapUniformsPerMesh->gCompositeControlBVCG.z );

    // fast anomorphic flare thing
    // abusing https://www.shadertoy.com/view/Md3SDS
    {
      vec4 lAnoStreak1 = lUniforms.mpTracingUniformsPerMesh->gAnoStreakConfig1;
      vec4 lAnoStreak2 = lUniforms.mpTracingUniformsPerMesh->gAnoStreakConfig2;
      vec4 lAnoStreak3 = lUniforms.mpTracingUniformsPerMesh->gAnoStreakConfig3;

      vec2 A  = sin( vec2( 0, 1.57 ) );

      vec2 U  = vec2( (lPixelCoordsVec2.x - 0.5), (lPixelCoordsVec2.y - 0.5) );
           U += lAnoStreak1.xy;
           U  = MUL( abs( MUL(U,  mat2( A, -A.y, A.x )) ), mat2( 0, 0, lAnoStreak1.w, lAnoStreak2.w ) );

      float lfStreak  = 0.2 / max( U.x, U.y );
            lfStreak *= lAnoStreak1.z;
            lfStreak *= ( 1.0 - (lfInvNebulaeSrcAlpha * 0.3) );

      vec3  lStreakColour = mix( lAnoStreak2.xyz, lAnoStreak3.xyz, saturate( lfStreak ) );

      lOutputColour.rgb += lStreakColour * pow( lfStreak, lAnoStreak3.w );
    }


    lOutputColour.rgb = Desaturate(
      lOutputColour.rgb, 
      lUniforms.mpFsMapUniformsPerMesh->gCompositeControlBVCG.w );

    lOutputColour.rgb *= Vignette( lPixelCoordsVec2, DEREF_PTR( lUniforms.mpFsMapUniformsPerMesh ) );

    FRAGMENT_COLOUR = saturate(lOutputColour);
}

#endif // D_GALACTIC_BACKGROUND




//----------------------------------------------------------------------------------------------------------------------------------------------
// nebulae

#ifdef D_GALACTIC_FOREGROUND


#ifdef D_PLATFORM_ORBIS
#pragma argument(targetoccupancy_atallcosts=60)
#endif

STATIC_CONST mat3 kNoiseMat3      = mat3(  0.00,  0.80,  0.60, 
                                            -0.80,  0.36, -0.48, 
                                            -0.60, -0.48,  0.64 );

float Noise3D_Texture( vec3 lPositionVec3, SAMPLER2DARG( lNoiseMap ) )
{
    vec3 p = floor( lPositionVec3 );
    vec3 f = fract( lPositionVec3 );

    f = f*f*(3.0-2.0*f);
    vec2 uv = (p.xy+vec2(37.0,17.0)*p.z) + f.xy;
    uv = (uv+0.5)/256.0;
    vec2 rg = texture2DLod( lNoiseMap, uv, 0.0 ).yx;

    return -1.0 + ( mix( rg.x, rg.y, f.z ) * 2.0 );
}

//-----------------------------------------------------------------------------

float Nebulae_Sample4(vec3 lvPos, SAMPLER2DARG( lNoiseMap ) )
{
    vec3 lvSample = 2.5 * lvPos;

    float lfOut;
    lfOut  = 0.50000 * Noise3D_Texture( lvSample, SAMPLER2DPARAM( lNoiseMap ) ); 
    lvSample = MUL( kNoiseMat3, ( lvSample * 2.02 ) );

    lfOut += 0.25000 * Noise3D_Texture( lvSample, SAMPLER2DPARAM( lNoiseMap ) ); 
    lvSample = MUL( kNoiseMat3, ( lvSample * 2.03 ) );

    lfOut += 0.12500 * Noise3D_Texture( lvSample, SAMPLER2DPARAM( lNoiseMap ) );
    lvSample = MUL( kNoiseMat3, ( lvSample * 2.01 ) );

    lfOut += 0.06250 * Noise3D_Texture( lvSample, SAMPLER2DPARAM( lNoiseMap ) );

    return saturate( lfOut );
}

float Nebulae_Sample3(vec3 lvPos, SAMPLER2DARG( lNoiseMap ) )
{
    vec3 lvSample = 2.5 * lvPos;

    float lfOut;
    lfOut  = 0.50000 * Noise3D_Texture( lvSample, SAMPLER2DPARAM( lNoiseMap ) ); 
    lvSample = MUL( kNoiseMat3, ( lvSample * 2.02 ) );

    lfOut += 0.25000 * Noise3D_Texture( lvSample, SAMPLER2DPARAM( lNoiseMap ) ); 
    lvSample = MUL( kNoiseMat3, ( lvSample * 2.03 ) );

    lfOut += 0.12500 * Noise3D_Texture( lvSample, SAMPLER2DPARAM( lNoiseMap ) );

    return saturate( lfOut );
}

float Nebulae_Sample2(vec3 lvPos, SAMPLER2DARG( lNoiseMap ) )
{
    vec3 lvSample = 2.5 * lvPos;

    float lfOut;
    lfOut  = 0.50000 * Noise3D_Texture( lvSample, SAMPLER2DPARAM( lNoiseMap ) ); 
    lvSample = MUL( kNoiseMat3, ( lvSample * 2.02 ) );

    lfOut += 0.25000 * Noise3D_Texture( lvSample, SAMPLER2DPARAM( lNoiseMap ) ); 

    return saturate( lfOut );
}

float Nebulae_Sample1(vec3 lvPos, SAMPLER2DARG( lNoiseMap ) )
{
    vec3 lvSample = 2.5 * lvPos;

    float lfOut;
    lfOut  = 0.50000 * Noise3D_Texture( lvSample, SAMPLER2DPARAM( lNoiseMap ) );

    return saturate( lfOut );
}

//-----------------------------------------------------------------------------
float TraceShadowRay( vec3 p, SAMPLER2DARG( lNoiseMap ) )
{
  float shadow = 1.0;
  vec3 shadpos = p;

  {
    shadpos.z += 0.05;
    shadow = shadow - Nebulae_Sample4( shadpos, SAMPLER2DPARAM( lNoiseMap ) ) * shadow;
  }
  {
    shadpos.z += 0.05;
    shadow = shadow - Nebulae_Sample3( shadpos, SAMPLER2DPARAM( lNoiseMap ) ) * shadow;
  }

  return shadow;
}


#define _NEBULA_RAYMARCH_LP(_SAMPLES,_LOD,_DENSMUL )                                    \
{																																												\
  for( int i=0; i<_SAMPLES; i++ )                                                       \
  {                                                                                     \
    if( sum.a > cAlphaThreshold )                                                       \
      break;                                                                            \
                                                                                        \
                                                                                        \
    vec3 pos = ro + ( t * rd );                                                         \
                                                                                        \
    float value  = Nebulae_Sample##_LOD( pos, SAMPLER2DPARAM( lNoiseMap ) );            \
    if (value != 0.0)																	\
    {																						\
	    float cluster = Nebulae_Sample3( pos * cScaleMultiplier, SAMPLER2DPARAM( lNoiseMap ) );                          \
	    float clusterColour = Value3D(pos * lMapUniforms.gBGCellConfig.w);                  \
	    float shadow = TraceShadowRay( pos, SAMPLER2DPARAM( lNoiseMap ) );                  \
	                                                                                        \
	    vec3 col = LookupCLUTDual( clusterColour, smoothstep(-1.0, 1.0, cluster), lColourPalettes );                 \
	                                                                                        \
	    col.xyz *= mix( cShadowMultiplier, vec3(1.0, 1.0, 1.0), shadow );                   \
	                                                                                        \
		float cutOff = cDensityCutOff + ( cluster * _DENSMUL );                             \
                                                                                        	\
    	value  = value * saturate(cutOff);                                                  \
    	value *= cValueMultiplier;                                                          \
	    col *= value;                                                                       \
	    sum = sum + vec4( col, value ) * ( 1.0 - value );                                   \
	}                                                                                       \
                                                                                        \
    t += max( lMapUniforms.gNebulaeStepRange_AlphaPow.x,                                \
              lMapUniforms.gNebulaeStepRange_AlphaPow.y * t );                          \
  }																																											\
}


//-----------------------------------------------------------------------------
vec4 DoRaymarch( vec3 ro, vec3 rd, float t, FsMapUniforms lMapUniforms, ColourPalettes lColourPalettes, SAMPLER2DARG( lNoiseMap ) )
{
  vec4 sum = vec4(0.0, 0.0, 0.0, 0.0);

  vec3 cShadowMultiplier    = vec3( 0.086, 0.086, 0.089 );
  float cValueMultiplier    = lMapUniforms.gNebulaeTraceConfig.x;
  float cScaleMultiplier    = lMapUniforms.gNebulaeTraceConfig.y;
  float cDensityMultiplier  = lMapUniforms.gNebulaeTraceConfig.z;
  float cDensityCutOff      = lMapUniforms.gNebulaeTraceConfig.w;
  float cAlphaThreshold     = 0.99;

  _NEBULA_RAYMARCH_LP( 50, 4, (cDensityMultiplier       )  )
  
  _NEBULA_RAYMARCH_LP( 40, 3, (cDensityMultiplier * 0.98)  )
  
  _NEBULA_RAYMARCH_LP( 30, 2, (cDensityMultiplier * 0.96)  )

  return saturate( sum );
}

//-----------------------------------------------------------------------------
FRAGMENT_MAIN_COLOUR_SRT
{
    vec2   lPixelCoordsVec2 = SCREENSPACE_AS_RENDERTARGET_UVS(TEX_COORDS);

    float lcfScreenX        = lUniforms.mpTracingUniformsPerMesh->gScreen.x;
    float lcfScreenY        = lUniforms.mpTracingUniformsPerMesh->gScreen.y;
    float lcfScreenAspect   = lUniforms.mpTracingUniformsPerMesh->gScreen.z;

    vec3 lvRaymarchDir = ComputeRaymarchDir( lPixelCoordsVec2, DEREF_PTR( lUniforms.mpTracingUniformsPerMesh ) );

    // establish a consistent trace origin based on the galactic megascale
    vec3 lvTracePoint = (lUniforms.mpTracingUniformsPerMesh->gGalacticScale.xyz * 100.0);

    // choose an offset along the ray to help break up stepping patterns on near clouds; this will then be further blurred 
    float t = Dither(0.0f, lvRaymarchDir.xy) * 4.0;

    // trace the nebulae
    vec4 lOutputColour = DoRaymarch( 
      lvTracePoint, 
      lvRaymarchDir, 
      t, 
      DEREF_PTR( lUniforms.mpFsMapUniformsPerMesh ), 
      DEREF_PTR( lUniforms.mpColourPalettesPerMesh ),
      SAMPLER2DPARAM_SRT( lUniforms.mpFsMapUniformsPerMesh, gNoiseMap )
      );

    {
      vec3 lvGoalTUV = ComputeGoalVirtualPlane( lvRaymarchDir, DEREF_PTR( lUniforms.mpTracingUniformsPerMesh ) );
      float lfGoalSize = lUniforms.mpTracingUniformsPerMesh->gGoalCenterDir.w;

      // galactic sun overlay; always visible through everything - bg effect will add the rest
      float galSunPulse = mix(lUniforms.mpFsMapUniformsPerMesh->gSunCoreConfig.x, lUniforms.mpFsMapUniformsPerMesh->gSunCoreConfig.y, lUniforms.mpTracingUniformsPerMesh->gInterest.z) * 2.0;

      float lfRescale = 1.5 - lfGoalSize;

      float galSunCore = 1.0 / ( length( vec2(-lvGoalTUV.y, lvGoalTUV.z) ) * ( galSunPulse * lfRescale ) );
      galSunCore *= lUniforms.mpFsMapUniformsPerMesh->gSunCoreConfig.w * lvGoalTUV.x;

      lOutputColour = mix(
            lOutputColour,
            lUniforms.mpFsMapUniformsPerMesh->gSunCoreColour,
            saturate(galSunCore) );
    }

    lOutputColour.a = pow( lOutputColour.a, lUniforms.mpFsMapUniformsPerMesh->gNebulaeStepRange_AlphaPow.z );

    FRAGMENT_COLOUR = lOutputColour;
}

#endif // D_GALACTIC_FOREGROUND



