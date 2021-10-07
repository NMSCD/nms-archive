////////////////////////////////////////////////////////////////////////////////
///
///     @file       ScreenFragmentShader.h
///     @author     User
///     @date       
///
///     @brief      ScreenFragmentShader
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
//      Compilation defines 

#ifndef D_FRAGMENT
#define D_FRAGMENT
#endif

//-----------------------------------------------------------------------------
//      Include files

#include "UberCommon.h"

#include "Common/CommonUniforms.shader.h"




#if defined( D_DEFER ) 
#include "Common/CommonGBuffer.shader.h"
#else
#include "Common/CommonLighting.shader.h"
#include "Common/CommonPlanet.shader.h"
#endif

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

INPUT(vec4, mTexCoordsVec4, TEXCOORD0)

#ifdef _F21_VERTEXCOLOUR
INPUT(vec4, mColourVec4, TEXCOORD1)
#endif

#ifdef _F30_REFRACTION_MAP
INPUT(vec4, mProjectedTexCoordsVec4, TEXCOORD2)
#endif

//#ifdef D_USES_WORLD_POSITION
#if defined( D_DEPTHONLY )
flat INPUT(vec4, mWorldPositionVec3_mfSpare, TEXCOORD3)
#else
INPUT(vec4, mWorldPositionVec3_mfSpare, TEXCOORD3)
#endif
//#endif

#if !defined( _F01_DIFFUSEMAP ) || defined( _F22_TRANSPARENT_SCALAR )
INPUT(vec4, mMaterialVec4, TEXCOORD4)
#endif

#ifdef D_USES_VERTEX_NORMAL
INPUT(vec3, mTangentSpaceNormalVec3, TEXCOORD6)
#endif

#if !defined( D_DEPTHONLY ) && !defined( D_DEPTH_CLEAR )

#ifdef _F20_PARALLAXMAP
INPUT(vec3, mTangentSpaceEyeVec3, TEXCOORD7)
#endif

#if 0 // defined( D_OUTPUT_MOTION_VECTORS ) && defined( _F14_UVSCROLL )
INPUT(vec4, mPrevTexCoordsVec4, TEXCOORD8)
#endif

#if !defined( D_DEFER ) && !defined( _F07_UNLIT )
flat INPUT(mat3, mUpMatrixMat3, TEXCOORD10)
#endif

//#if defined( _F44_IMPOSTER )
//    INPUT( vec3, mShadowWorldPositionVec3,  TEXCOORD8 )
//#endif

#if defined( _F03_NORMALMAP )  || defined( _F42_DETAIL_NORMAL )
INPUT(vec3, mTangentMatRow1Vec3, TEXCOORD18)
INPUT(vec3, mTangentMatRow2Vec3, TEXCOORD19)
INPUT(vec3, mTangentMatRow3Vec3, TEXCOORD20)
#endif


#if defined( _F58_USE_CENTRAL_NORMAL ) || defined( _F56_MATCH_GROUND )
INPUT(vec3, mCenteralNormalVec3, TEXCOORD14)
#endif



#ifdef D_OUTPUT_MOTION_VECTORS
INPUT(vec4, mPrevScreenPosition, TEXCOORD15)
#endif

#endif

#ifdef _F37_RECOLOUR
INPUT(vec4, mPaletteColour1Vec4, TEXCOORD16)
INPUT(vec4, mPaletteColour2Vec4, TEXCOORD17)
#endif

#ifdef D_USE_SCREEN_POSITION
INPUT(vec4, mScreenSpacePositionVec4, TEXCOORD21)
#endif

flat INPUT(vec3, mfFadeValueForInstance_mfLodIndex_mfShearMotionLength, TEXCOORD22)

//INPUT_FRONTFACING

DECLARE_END



#ifdef D_DEFER
    #include "OutputDeferred.shader.h"
#else
    #include "OutputForward.shader.h"
#endif


//-----------------------------------------------------------------------------
//    Functions


//-----------------------------------------------------------------------------
///
///     Fragment Main
///
///     @brief      Fragment Main
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
#if defined( D_DEPTH_CLEAR )         
FRAGMENT_MAIN_COLOUR_DEPTH_SRT
#else
FRAGMENT_MAIN_COLOUR_SRT
#endif
{

#ifdef D_DEPTH_CLEAR
    FRAGMENT_COLOUR = vec4( 0.0, 0.0, 0.0, 0.0 );
    FRAGMENT_DEPTH = D_DEPTH_CLEARVALUE;
    return;
#endif

    vec4 lFragmentColourVec4;
    vec3 lNormalVec3 = vec3(0.0,1.0,0.0);

    #ifdef D_TEXCOORDS
    //{
        vec4 lTexCoordsVec4;
        lTexCoordsVec4 = IN( mTexCoordsVec4 );

        // WARNING THIS IS TEMPORARY AND ALSO SHIT
#ifdef D_PLATFORM_OPENGL
        lTexCoordsVec4.y = 1.0 - lTexCoordsVec4.y;
#endif

    //}
    #endif

    //-----------------------------------------------------------------------------
    ///     Diffuse
    //-----------------------------------------------------------------------------
    vec4 lDiffuseColourVec4;

    #ifdef _F01_DIFFUSEMAP
    {
        

        {   
            vec2 lFracTexCoordsVec2;
            lFracTexCoordsVec2.x = fract(lTexCoordsVec4.x);
            lFracTexCoordsVec2.y = fract(lTexCoordsVec4.y);

            // --- float lfColourSeperateAmount = lUniforms.mpCustomPerMaterial->gUIDeformVec4.z; // [peter] was never used

            float lfOverallMagnitude     = 1.0;
            float lfFlickerAmount        = 0.5;
            vec2   lDeformedCoordsVec2   = lFracTexCoordsVec2;
            lDeformedCoordsVec2.x += 
                ((lfFlickerAmount * 0.1) + 2.0 * (max( sin( lUniforms.mpPerFrame->gfTime + sin( lUniforms.mpPerFrame->gfTime * 31.0 ) ), 0.98) - 0.98)) * 
                0.05 *
                sin( 113.0 * lUniforms.mpPerFrame->gfTime * sin( lDeformedCoordsVec2.y * 137.0 ) ) * 
                lfOverallMagnitude;

            
            //lfColourSeperateAmount = lfColourSeperateAmount*(max( sin( gCommon.gfTime + sin( gCommon.gfTime * 13.0 ) ), 0.0));
            // --- lfColourSeperateAmount = 4.0/lUniforms.mpCustomPerMaterial->gUIDeformVec4.z; // [peter] was never used

            // Colour separation
            //lEffectsColourVec3.r = texture2D(gDiffuseMap,vec2( lDeformedCoordsVec2.x+lfColourSeperateAmount, lDeformedCoordsVec2.y)).x;
            //lEffectsColourVec3.g = texture2D(gDiffuseMap,vec2( lDeformedCoordsVec2.x, lDeformedCoordsVec2.y)).y;
            //lEffectsColourVec3.b = texture2D(gDiffuseMap,vec2( lDeformedCoordsVec2.x-lfColourSeperateAmount, lDeformedCoordsVec2.y)).z;
            //lFragmentColourVec3 = clamp(lFragmentColourVec3*0.5+0.5*lFragmentColourVec3*lFragmentColourVec3*1.2,0.0,1.0);

            /*lEffectsColourVec3 *= 2.5;
            lEffectsColourVec3.x = pow( lEffectsColourVec3.x, 3.0 );
            lEffectsColourVec3.y = pow( lEffectsColourVec3.y, 3.0 );
            lEffectsColourVec3.z = pow( lEffectsColourVec3.z, 3.0 );*/

            float lfPulseIntensity = 0.9 + 0.3 * sin(13.0*lUniforms.mpPerFrame->gfTime)*sin(5.0*lUniforms.mpPerFrame->gfTime)*sin(23.0*lUniforms.mpPerFrame->gfTime);
                        
            // Glitch
            float lfGlitch = sin(18.245*lUniforms.mpPerFrame->gfTime)*cos(11.323*lUniforms.mpPerFrame->gfTime)*sin(4.313*lUniforms.mpPerFrame->gfTime);
            lfGlitch *= max(0.0, sin(10.0*lUniforms.mpPerFrame->gfTime));
            lfGlitch *= lfGlitch;
            lTexCoordsVec4.x += sin(lTexCoordsVec4.y*19.1)*lfGlitch*.01;
            lTexCoordsVec4.x += sin(lTexCoordsVec4.y*459.1)*lfGlitch*lfGlitch*.02;

            lDiffuseColourVec4 = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gDiffuseMap ), lTexCoordsVec4.xy);
            
            vec3  lEffectsColourVec3 = lDiffuseColourVec4.xyz;

            // Gamma corection
            lEffectsColourVec3 = GammaCorrectInput(lEffectsColourVec3);

            // Vignette
            float lfVignette  = lTexCoordsVec4.x*lTexCoordsVec4.y*(1.0-lTexCoordsVec4.x)*(1.0-lTexCoordsVec4.y) * 10.0;
            lfVignette = clamp( lfVignette, 0.0, 1.0 );
            lEffectsColourVec3 = mix( vec3(0.0,0.0,0.0), lEffectsColourVec3, lfVignette );


            lEffectsColourVec3 *= 1.0 + 0.04 * sin(5.0 * lUniforms.mpPerFrame->gfTime - lDeformedCoordsVec2.y*150.0) * (1.0 - lTexCoordsVec4.y) * (1.0 - lTexCoordsVec4.y);           // Scan lines

            lEffectsColourVec3 *= lfPulseIntensity; // Pulse

            lEffectsColourVec3 *= (12.0 + mod(lDeformedCoordsVec2.y * 30.0 + lUniforms.mpPerFrame->gfTime, 2.0)) / 13.0;

            //lEffectsColourVec3.b *= 1.0 + (1.0 - lDeformedCoordsVec2.y) * 2.0;

            lDiffuseColourVec4.xyz  = lEffectsColourVec3;
        }

    }
    #else
    {
        lDiffuseColourVec4 = IN( mMaterialVec4 );
    }
    #endif

    #ifdef _F21_VERTEXCOLOUR
    {
        lDiffuseColourVec4 *= IN( mColourVec4 );	
    }
    #endif

    lFragmentColourVec4 = lDiffuseColourVec4;
    lFragmentColourVec4.a = 1.0;

    //-----------------------------------------------------------------------------
    ///
    ///     Transparency
    ///
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    ///
    ///     Output
    ///
    //-----------------------------------------------------------------------------	

    int liMaterialID = 0;

#ifdef _F10_NORECEIVESHADOW
    liMaterialID |= D_NORECEIVESHADOW;
#endif

#ifdef _F50_DISABLE_POSTPROCESS
    liMaterialID |= D_DISABLE_POSTPROCESS;
#endif

#ifdef _F07_UNLIT
    liMaterialID |= D_UNLIT;
#endif

#if defined( _F57_DETAIL_OVERLAY )
    liMaterialID |= D_DETAILOVERLAY;
#endif

    liMaterialID |= D_CLAMP_AA;

    vec4 lOutColours0Vec4;
    vec4 lOutColours1Vec4;
    vec4 lOutColours2Vec4;
    vec4 lOutColours3Vec4;
    vec4 lOutColours4Vec4;

    WriteOutput(
        lOutColours0Vec4,
        lOutColours1Vec4,
        lOutColours2Vec4,
        lOutColours3Vec4,
        lOutColours4Vec4,
        DEREF_PTR( lUniforms.mpPerFrame ),
        DEREF_PTR( lUniforms.mpCommonPerMesh ),
        DEREF_PTR( lUniforms.mpCustomPerMaterial ),
        lFragmentColourVec4,
        IN( mWorldPositionVec3_mfSpare ).xyz,
        lNormalVec3,
        liMaterialID,
        0.0,
        1.0,
        0.0,
        0.0,
        #ifdef D_USE_SCREEN_POSITION 
        IN( mScreenSpacePositionVec4 ).xyzw,
        #else
        float2vec4( 0.0 ),
        #endif 
        #ifdef D_OUTPUT_MOTION_VECTORS 
        IN( mPrevScreenPosition ).xyzw,
        #else
        float2vec4( 0.0 ),
        #endif 
#if !defined( D_DEFER ) && !defined( _F07_UNLIT )
        GetInverseWorldUpTransform(lUniforms.mpPerFrame->gViewPositionVec3, lUniforms.mpCommonPerMesh->gLightOriginVec4),
        SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMaterial,gDualPMapBack),
        SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMaterial,gDualPMapFront),
#endif
        0.0,
        false
        );

#if !defined( D_ATTRIBUTES )
    FRAGMENT_COLOUR  = lOutColours0Vec4;
#else
    FRAGMENT_COLOUR0 = lOutColours0Vec4;
    FRAGMENT_COLOUR1 = lOutColours1Vec4;
    FRAGMENT_COLOUR2 = lOutColours2Vec4;
    FRAGMENT_COLOUR3 = lOutColours3Vec4;
#endif

}
