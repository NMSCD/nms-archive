////////////////////////////////////////////////////////////////////////////////
///
///     @file       CommonUniforms.h
///     @author     User
///     @date       
///
///     @brief      CommonUniforms
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////


#ifndef D_COMMONUNIFORMS2_H
#define D_COMMONUNIFORMS2_H


STATIC_CONST  mat4 kFSQuadProj = mat4(  2.0 / 1280.0, 0.0, 0.0, 0.0,
                                        0.0, -2.0 / 720.0, 0.0, 0.0,
                                        0.0, 0.0, -1.0, 0.0,
                                       -1.0, 1.0, 0.0, 1.0);

struct PerFrameUniforms
{
    vec4    gLightPositionVec4;                 // EShaderConst_LightPositionVec4
    vec4    gLightDirectionVec4;                // EShaderConst_LightDirectionVec4
    vec3    gViewPositionVec3;                  // EShaderConst_ViewPositionVec3
    float   gfTime;                             // EShaderConst_Time
    float   gfPrevTime;                         // EShaderConst_PrevTime
    vec4    gClipPlanesVec4;                    // EShaderConst_ClipPlanesVec4
    vec4    gClipPlanesRVec4;                   // EShaderConst_ClipPlanesRVec4

    mat4    gCameraMat4;                        // EShaderConst_CameraMat4
    mat4    gPrevViewProjectionMat4;            // EShaderConst_PrevViewProjectionMat4

    vec4    gMBlurSettingsVec4;                 // EShaderConst_BlurSettingsVec4
    vec4    gDeJitterVec4;                      // EShaderConst_DeJitterVec4
    vec4    gTaaSettingsVec4;                   // EShaderConst_TaaSettingsVec4
    vec4    gTessSettingsVec4;                  // EShaderConst_TessSettingsVec4

    vec4    gFrameBufferSizeVec4;  
    vec4    gFoVValuesVec4;                     // EShaderConst_FoVValuesVec4
    vec4    gShadowSizeVec4;  
    vec4    gShadowFadeParamVec4;  
    vec4    gShadowProjScaleVec4[3];            // EShaderConst_ShadowProjScaleVec4
    mat4    gViewMat4;                          // EShaderConst_ViewMat4

};

struct CommonPerMeshUniforms
{    
    // These are planet specific. Should they be here?
    vec4 gPlanetPositionVec4;

    mat4    gWorldMat4;                            // EShaderConst_WorldMat4
    mat4    gViewProjectionMat4;                	// EShaderConst_ViewProjectionMat4

#if defined( D_SHADOW )
    mat4    gWorldViewProjectionMat4;              // EShaderConst_WorldViewProjectionMat4
#endif
#if !defined( D_SHADOW ) || defined( _F60_ACUTE_ANGLE_FADE )
	mat4    gWorldNormalMat4;                      // EShaderConst_WorldNormalMat4
#endif


#if defined( D_FADE ) && !defined( D_INSTANCE )
    float   gfFadeValue;                           // EShaderConst_FadeTime
#else
    float   fdFadeValueDummy;
#endif
    
#if defined( D_SKINNING_UNIFORMS ) && !defined( D_PLATFORM_OPENGL )
    vec4   gaSkinMatrixRowsVec4[ 75 * 3 ];        
    vec4   gaPrevSkinMatrixRowsVec4[ 75 * 3 ];        
#endif
     
   //  have particle shader use a particlecommon instead of uber, and put these into it.
#if defined( D_PARTICLE_UNIFORMS ) && !defined( D_PLATFORM_OPENGL )
    vec4    gaParticleCornersVec4[ 4 ];            // EShaderConst_ParticleCornersVec4
    vec4    gaParticlePositionsVec4[ 32 ];         // EShaderConst_ParticlePositionsVec4
    vec4    gaParticleSizeAndRotationsVec4[ 32 ];  // EShaderConst_ParticleSizeAndRotationsVec4
    vec4    gaParticleNormalsVec4[ 32 ];           // EShaderConst_ParticleNormalsVec4
    vec4    gaParticleColoursVec4[ 32 ];           // EShaderConst_ParticleColoursVec4
#endif

#if !defined( D_DEPTHONLY )
     vec4    gLightColourVec4;                   // EShaderConst_LightColourVec4
#if !defined( D_DEFER ) && !defined(D_PARTICLE_UNIFORMS)
    vec4    gLightOriginVec4;
    // These shouldn't be per mesh, the should be per rendertarget. BUT we need to add them to the enum
    // in egShader and SetPErRenderTargetUniforms for that to work and we're trying to do a build for sony
    // so that will have to wait. (also probably need a way of setting per RT uniforms from Game).
    vec4    gScanParamsPosVec4;
    vec4    gScanParamsCfg1Vec4;
    vec4    gScanParamsCfg2Vec4;
    vec4    gScanParamsCfg3Vec4;
    vec4    gScanColourVec4;
    mat4    gaShadowMat4[3];                  // EShaderConst_ShadowMat4
#endif

#if !defined(D_TERRAIN)
    mat4    gProjectionMat4;                    // EShaderConst_ProjectionMat4
#if !defined(D_PARTICLE_UNIFORMS)
    mat4    gWorldMotionMat4;                      // EShaderConst_WorldMotionMat4
    vec4    gInverseWorldUpMatVec4[3];
    // Stop these being compiled in when recolouring as the recolour shader needs ALL tex units.
#if defined( D_DEFERRED_DECAL )
    mat4    gInverseModelMat4;                     // EShaderConst_InverseModelMat4 = 0
#else
    mat4    gInverseProjectionSCMat4;              // EShaderConst_InverseProjectionSCMat4
#endif
    mat4    gInverseViewMat4;                   // EShaderConst_InverseViewMat4
    mat4    gInverseProjectionMat4;             // EShaderConst_InverseProjectionMat4
#ifdef D_IBL    
   	vec4	gGenericParam0Vec4;					// EShaderConst_GenericParam0Vec4
#else
    vec4	gGenericParam0DummyVec4;
#endif    
#endif
#endif
#endif


#ifdef D_SPOTLIGHT
    vec4 gSpotlightPositionVec4;
    vec4 gSpotlightDirectionVec4;
#endif

    vec4	gUserDataVec4;    
};

#if defined( D_PARTICLE_UNIFORMS ) && defined( D_PLATFORM_OPENGL )
uniform vec4    gaParticleCornersVec4[4];            // EShaderConst_ParticleCornersVec4
uniform vec4    gaParticlePositionsVec4[32];         // EShaderConst_ParticlePositionsVec4
uniform vec4    gaParticleSizeAndRotationsVec4[32];  // EShaderConst_ParticleSizeAndRotationsVec4
uniform vec4    gaParticleNormalsVec4[32];           // EShaderConst_ParticleNormalsVec4
uniform vec4    gaParticleColoursVec4[32];           // EShaderConst_ParticleColoursVec4
#endif


#endif

