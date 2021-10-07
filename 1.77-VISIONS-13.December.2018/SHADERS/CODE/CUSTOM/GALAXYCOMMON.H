////////////////////////////////////////////////////////////////////////////////
///
///     @file       GalaxyCommon.h
///     @author     User
///     @date       
///
///     @brief      GalaxyCommon
///
///     Copyright (c) 2015 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 


//-----------------------------------------------------------------------------
//      Include files

#include "Common/Defines.shader.h"

STATIC_CONST  mat4 kFSQuadProj = mat4(  2.0 / 1280.0, 0.0, 0.0, 0.0,
                                        0.0, -2.0 / 720.0, 0.0, 0.0,
                                        0.0, 0.0, -1.0, 0.0,
                                       -1.0, 1.0, 0.0, 1.0);

struct TracingFsFxUniforms
{ 
  vec4    gTraceOrigin;
  vec4    gTraceDir;

  vec4    gTracePDX;
  vec4    gTracePDY;
  vec4    gTraceScreenCenter;

  vec4    gGoalCenterDir;
  vec4    gGoalCenterPerpU;
  vec4    gGoalCenterPerpV;

  vec4    gAnoStreakConfig1;
  vec4    gAnoStreakConfig2;
  vec4    gAnoStreakConfig3;

  vec4    gGalacticScale;
  vec4    gInterest;
  vec4    gTiming;
  
  vec4    gScreen;
  vec4    gOriginSS;
};


struct FsMapUniforms
{
  vec4    gSunCoreConfig;
  vec4    gSunCoreColour;

  vec4    gBGCellConfig;
  vec4    gBGColourScales;
  vec4    gBGColourConfig;
  vec4    gBGLensFlareColour;
  vec4    gBGLensFlareSpread;
  
  vec4    gNebulaeStepRange_AlphaPow;
  vec4    gCompositeControlBVCG;
  vec4    gNebulaeTraceConfig;
  vec4    gVignetteLensFlareConfig;

BEGIN_SAMPLERBLOCK
  SAMPLER2D( gNebulaeMap );
  SAMPLER2D( gAtmosMap );
  SAMPLER2D( gNoiseMap );
END_SAMPLERBLOCK

#ifndef D_COMPUTE

#define TEX_COORDS IN(mTexCoordsVec2)

#elif defined D_PLATFORM_ORBIS

//-----------------------------------------------------------------------------
///
///     ComputeOutputUniforms
///
///     @brief      ComputeOutputUniforms
///
///     Refs to output textures for compute quad shaders
//-----------------------------------------------------------------------------

struct ComputeOutputUniforms
{
    RW_Texture2D<float4> gOutTexture0;
    RW_Texture2D<float4> gOutTexture1;
    RW_Texture2D<float4> gOutTexture2;
    RW_Texture2D<float4> gOutTexture3;
    RW_Texture2D<float4> gOutTexture4;
};
#define TEX_COORDS ( ( vec2( dispatchThreadID.xy ) + vec2(0.5,0.5) ) / vec2( GetImgResolution( lUniforms.mpCmpOutPerMesh->gOutTexture0 ) ) )

#else

RW_Texture2D<float4> gOutTexture0 : register(u0);
RW_Texture2D<float4> gOutTexture1 : register(u1);
RW_Texture2D<float4> gOutTexture2 : register(u2);
RW_Texture2D<float4> gOutTexture3 : register(u3);
RW_Texture2D<float4> gOutTexture4 : register(u4);

#define TEX_COORDS ( ( vec2( dispatchThreadID.xy ) + vec2(0.5,0.5) ) / vec2( GetImgResolution( gOutTexture0 ) ) )

#endif


#if defined(D_PLATFORM_OPENGL)

// workaround for another AMD slowness problem

uniform vec4 gLargeAreaPrimaryLUT[10];
uniform vec4 gLargeAreaSecondaryLUT[10];

struct ColourPalettes
{
    vec4    gLargeAreaPrimaryLUT_NOTUSED[10];
    vec4    gLargeAreaSecondaryLUT_NOTUSED[10];
};


#else

struct ColourPalettes
{
    vec4    gLargeAreaPrimaryLUT[10];
    vec4    gLargeAreaSecondaryLUT[10];
};


#endif


struct UniformBuffer
{
     DECLARE_PTR( TracingFsFxUniforms,  mpTracingUniformsPerMesh )
     DECLARE_PTR( FsMapUniforms,        mpFsMapUniformsPerMesh   )
     DECLARE_PTR( ColourPalettes,       mpColourPalettesPerMesh  )
#if defined D_PLATFORM_ORBIS && defined D_COMPUTE
     DECLARE_PTR(ComputeOutputUniforms, mpCmpOutPerMesh)   /* hack - marked 'per mesh' so it'll be alloced in cmd buf */
#endif
};

#ifndef M_PI
#define M_PI 3.141592653589793
#endif