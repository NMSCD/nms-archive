
#if defined( _F02_SKINNED )
    #define D_SKINNING_UNIFORMS
#endif

#if defined( _F60_ACUTE_ANGLE_FADE )
    #define D_FADE
#endif

#define D_DEPTHONLY
#define D_SHADOW

#ifndef D_FRAGMENT
#define D_FRAGMENT
#endif


#include "UberCommon.h"
#include "Common/CommonDepth.shader.h"

#if defined( D_FADE )
#include "Common/CommonFade.shader.h"
#endif

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec4, mScreenSpacePositionVec4, TEXCOORD0 )

#if !defined( D_TERRAIN )
//#ifdef _F11_ALPHACUTOUT
    INPUT( vec4, mTexCoordsVec4,           TEXCOORD2 )
//#endif
#endif

#if (defined( D_FADE ) && defined( D_INSTANCE )) || defined( _F44_IMPOSTER )
    INPUT( vec3, mPixelZandW_mfFadeValueForInstance,    TEXCOORD1 )
#endif    


#ifdef _F60_ACUTE_ANGLE_FADE
    INPUT( vec4, mWorldPositionVec3_mfSpare, TEXCOORD3 )
    INPUT( vec3, mTangentSpaceNormalVec3,    TEXCOORD6 )
#endif


DECLARE_END


#ifdef D_PLATFORM_OPENGL
FRAGMENT_MAIN_COLOUR_SRT
#else
#if defined(_F44_IMPOSTER )
VOID_MAIN_DEPTH_SRT
#elif defined( D_FADE )
VOID_MAIN_SRT
#else
VOID_MAIN_COLOUR_EARLYZ_SRT
#endif
#endif
{ 

#ifdef D_FADE    
    
       CheckFade(
            SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMaterial, gFadeNoiseMap ),
            IN( mScreenSpacePositionVec4 ),
#ifdef  D_INSTANCE             
           IN( mPixelZandW_mfFadeValueForInstance ).z,
#else
           lUniforms.mpCommonPerMesh->gfFadeValue,
#endif 
           lUniforms.mpPerFrame->gFrameBufferSizeVec4.xy
         );    
       

#endif

    #ifdef _F01_DIFFUSEMAP
    {
        #if defined( _F11_ALPHACUTOUT )
        {
            vec4 lDiffuseColourVec4 = vec4(1.0, 1.0, 1.0, 1.0);
            vec4 lTexCoordsVec4;

            lTexCoordsVec4 = IN(mTexCoordsVec4);

            lDiffuseColourVec4 = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gDiffuseMap ), lTexCoordsVec4.xy);

            if ( lDiffuseColourVec4.a < 0.5 ) // needs to be alpharef passed into constant buffer
            {
                discard;
            }
        }
        #endif 
    }
    #endif
    
    #if defined( _F60_ACUTE_ANGLE_FADE )
        vec3 lViewDirVec3  = normalize( lUniforms.mpPerFrame->gViewPositionVec3                 - IN( mWorldPositionVec3_mfSpare ).xyz );

        float lfNdotCam   = 1.0 - acos( abs( dot( lViewDirVec3, normalize( IN( mTangentSpaceNormalVec3 ).xyz ) ) ) ) / ( 3.14 * 0.5 );

        if( lfNdotCam < 0.5 )
        {
            CheckFade(
                SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMaterial, gFadeNoiseMap ),
                IN( mScreenSpacePositionVec4 ),
                lfNdotCam * 2.0,
                lUniforms.mpPerFrame->gFrameBufferSizeVec4.xy );
        }

    #endif

    #if defined( _F44_IMPOSTER )
        // Output modified imposter depth to avoid crap self shadowing artefacts.
        //vec4 lTexCoordsImp = IN( mTexCoordsVec4 );
        
        //lTexCoordsImp *= lUniforms.mpCustomPerMaterial->gImposterDataVec4.x;

        float lfImposterDepth = IN( mPixelZandW_mfFadeValueForInstance.x );
        FRAGMENT_DEPTH = lfImposterDepth;
    #endif


#ifdef D_PLATFORM_OPENGL
    float lfShadowAlpha = 1.0;
    FRAGMENT_COLOUR = vec4(1.0, 0.0, 1.0, lfShadowAlpha);
#endif    
    
}