
#if !defined( _F50_DISABLE_POSTPROCESS )
#include "OutputPostProcess.shader.h"
#endif

//-----------------------------------------------------------------------------
//    Functions



//-----------------------------------------------------------------------------
///
///     WriteOutput
///
///     @brief      WriteOutput
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
void
WriteOutput(    
    out vec4                      lOutColours0Vec4,
    out vec4                      lOutColours1Vec4,
    out vec4                      lOutColours2Vec4,
    out vec4                      lOutColours3Vec4,
    out vec4                      lOutColours4Vec4,
    in  PerFrameUniforms          lPerFrameUniforms,
    in  CommonPerMeshUniforms     lMeshUniforms,
    in  CustomPerMaterialUniforms lCustomUniforms,
    in  vec4                      lColourVec4,
    in  vec3                      lPositionVec3,
    in  vec3                      lNormalVec3,
    in  int                       liMaterialID,
    in  float                     lfMetallic,
    in  float                     lfRoughness,
    in  float                     lfSubsurface,
    in  float                     lfGlow,
    in  vec4                      lScreenSpacePositionVec4,
    in  vec4                      lPrevScreenPositionVec4,
#ifndef _F07_UNLIT
    in  mat3                      lUpMatrix,
    SAMPLER2DARG(                 lDualPMapBack),
    SAMPLER2DARG(                 lDualPMapFront),
#endif
    in  float                     lfPixelDepth,
    in  bool                      lbFrontFacing)
{
    vec4 lFragmentColourVec4;
    vec3 lSunColourVec3 = float2vec3(0.0);

    lFragmentColourVec4 = lColourVec4;

    //-----------------------------------------------------------------------------
    ///
    ///     Lighting
    ///
    //-----------------------------------------------------------------------------
    #ifndef _F07_UNLIT
    {
        vec3 lLightDirectionVec3 = -lPerFrameUniforms.gLightDirectionVec4.xyz;
        lFragmentColourVec4.rgb = ComputeLightColour(
            lCustomUniforms,
            lPerFrameUniforms,
            lMeshUniforms,
            SAMPLER2DPARAM(lDualPMapBack),
            SAMPLER2DPARAM(lDualPMapFront),
            lLightDirectionVec3,
            lMeshUniforms.gLightColourVec4.xyz * lMeshUniforms.gLightColourVec4.w,
            lPositionVec3,
            lScreenSpacePositionVec4.xy / lScreenSpacePositionVec4.w,
            lNormalVec3,
            lColourVec4.rgb,
            lUpMatrix,
            liMaterialID,
            lfMetallic,
            lfRoughness,
            lfSubsurface,
	        0.5,
            1.0,
            lSunColourVec3 );
    }
    #endif

    #ifdef _F34_GLOW
    {
        lFragmentColourVec4.rgb = mix( lFragmentColourVec4.rgb, lColourVec4.rgb, lfGlow );

        #ifdef _F35_GLOW_MASK
        {
            lFragmentColourVec4.a   = lfGlow;
        }
        #endif

    }
    #endif

    #if !defined( _F50_DISABLE_POSTPROCESS ) && !defined( D_NO_POSTPROCESS ) 
    {
        lFragmentColourVec4 = PostProcess(
            lPerFrameUniforms,
            lMeshUniforms,
            lCustomUniforms,
            lFragmentColourVec4,
            lPositionVec3,
            lNormalVec3,
			liMaterialID,
            lScreenSpacePositionVec4,
            lSunColourVec3);
    }
    #endif
    
    //-----------------------------------------------------------------------------
    ///
    ///     Output
    ///
    //-----------------------------------------------------------------------------	
    lOutColours0Vec4 = lFragmentColourVec4;
    lOutColours1Vec4 = vec4(0.0, 0.0, 0.0, 0.0);
    lOutColours2Vec4 = vec4(0.0, 0.0, 0.0, 0.0);
    lOutColours3Vec4 = vec4(0.0, 0.0, 0.0, 0.0);
    lOutColours4Vec4 = vec4(0.0, 0.0, 0.0, 0.0);
}

