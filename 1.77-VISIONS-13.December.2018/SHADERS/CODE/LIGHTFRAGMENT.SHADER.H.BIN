////////////////////////////////////////////////////////////////////////////////
///
///     @file       LightFragment.h
///     @author     User
///     @date       
///
///     @brief      LightFragmentShader
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 

#if defined(D_VERTEX_SUNLIGHT)
#define D_VERTEX
#endif

#if !defined(D_VERTEX) && !defined(D_FRAGMENT)
#define D_FRAGMENT
#endif

//-----------------------------------------------------------------------------
//      Include files

#include "Common/Defines.shader.h"
#include "Common/CommonUniforms.shader.h"
#include "LightCommon.shader.h"
#include "Common/CommonGBuffer.shader.h"

#if defined( D_SPLIT_SHADOW ) && !defined( D_PLATFORM_ORBIS_COMPUTE)
#pragma argument(targetoccupancy_atallcosts=70)
#endif

#if defined( D_SHADOW_APPLY ) && !defined( D_PLATFORM_ORBIS_COMPUTE)
#pragma argument(targetoccupancy_atallcosts=60)
#endif

#if defined(D_VERTEX) && !defined(D_VERTEX_SUNLIGHT)

//-----------------------------------------------------------------------------
//      Compilation defines 


//-----------------------------------------------------------------------------
//      Include files


#include "Common/CommonDepth.shader.h"


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
    INPUT(  vec3, mkLocalPositionVec3, POSITION0 )
DECLARE_END

DECLARE_OUTPUT
    OUTPUT_SCREEN_POSITION

    #ifdef D_SPHERE
        OUTPUT( vec4, mPositionVec4, TEXCOORD0 )
        OUTPUT( vec4, mScreenSpacePositionVec4, TEXCOORD4 )
    #else
        OUTPUT( vec4, mScreenSpacePositionVec4, TEXCOORD0 )
    #endif
DECLARE_END

//-----------------------------------------------------------------------------
///
///     Output
///
///     @brief  Output
///
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//    Functions

//-----------------------------------------------------------------------------
///
///     Vertex Main
///
///     @brief      Vertex Main
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------

VERTEX_MAIN_SRT
{
#ifdef D_SPOTLIGHT
    vec4 lWorldPositionVec4       = MUL( lUniforms.mpCommonPerMesh->gWorldMat4, vec4(IN(mkLocalPositionVec3), 1.0) );
    vec4 lScreenSpacePositionVec4 = MUL( lUniforms.mpCommonPerMesh->gViewProjectionMat4, lWorldPositionVec4 );
#else 
    vec4 lScreenSpacePositionVec4 = MUL( kFSQuadProj, vec4(IN(mkLocalPositionVec3), 1.0) );
#endif

    OUT(mScreenSpacePositionVec4) = lScreenSpacePositionVec4;

    SCREEN_POSITION = lScreenSpacePositionVec4;
#ifdef D_SPOTLIGHT   
    OUT(mScreenSpacePositionVec4).xy = vec2(0.5,0.5) * (OUT(mScreenSpacePositionVec4).xy + OUT(mScreenSpacePositionVec4).ww);
#ifndef D_PLATFORM_OPENGL
    OUT(mScreenSpacePositionVec4).y = (1.0 * OUT(mScreenSpacePositionVec4).w) - OUT(mScreenSpacePositionVec4).y;
#endif
#endif

}


#endif // D_VERTEX

#ifdef D_VERTEX_SUNLIGHT

//-----------------------------------------------------------------------------
//      Compilation defines 


//-----------------------------------------------------------------------------
//      Include files

#include "Common/CommonPlanet.shader.h"

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
    INPUT(  vec3, mkLocalPositionVec3, POSITION0 )
 	INPUT(  vec2, mkTexCoordsVec4,     TEXCOORD0 )    
DECLARE_END

DECLARE_OUTPUT
    OUTPUT_SCREEN_POSITION
 	OUTPUT( vec2, mTexCoordsVec2,     TEXCOORD0 )
    flat OUTPUT( mat3, mUpMatrix,     TEXCOORD1 ) 	
DECLARE_END

//-----------------------------------------------------------------------------
///
///     Output
///
///     @brief  Output
///
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//    Functions

//-----------------------------------------------------------------------------
///
///     Vertex Main
///
///     @brief      Vertex Main
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
VERTEX_MAIN_SRT
{   
    SCREEN_POSITION = MUL( kFSQuadProj, vec4( IN( mkLocalPositionVec3 ), 1.0 ) );

    OUT( mTexCoordsVec2 ) = IN( mkTexCoordsVec4 );    
	OUT ( mUpMatrix )     = GetInverseWorldUpTransform( lUniforms.mpPerFrame->gViewPositionVec3, lUniforms.mpCommonPerMesh->gLightOriginVec4 );	
}


#endif // D_VERTEX_SUNLIGHT



#ifdef D_NULL

//-----------------------------------------------------------------------------
//      Global Data

//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION
    INPUT( vec2, mTexCoordsVec2,     TEXCOORD0 )
DECLARE_END

//-----------------------------------------------------------------------------
//    Functions


//-----------------------------------------------------------------------------
///
///     Main
///
///     @brief      Fragment Main 
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
FRAGMENT_MAIN_COLOUR_SRT
{
}

#endif // D_NULL

#ifdef D_LIGHT

//-----------------------------------------------------------------------------
//      Includes

#ifdef D_WATER
    #include "Custom/WaterCommon.h"
#else
    #include "Common/CommonLighting.shader.h"
    #include "OutputPostProcess.shader.h"
#endif
//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Typedefs and Classes  
DECLARE_INPUT
#ifdef D_SPOTLIGHT
    INPUT_SCREEN_POSITION
    INPUT( vec4, mScreenSpacePositionVec4, TEXCOORD0 )
#else
    INPUT_SCREEN_POSITION
    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
    flat INPUT( mat3, mUpMatrix     , TEXCOORD1 )
#endif
DECLARE_END

//-----------------------------------------------------------------------------
//    Functions


//-----------------------------------------------------------------------------
///
///     Main
///
///     @brief      Fragment Main
///
///     @param      void
///     @return     Nothing. 
///
//-----------------------------------------------------------------------------

#if defined( D_SPOTLIGHT_OUTER )
FRAGMENT_MAIN_COLOUR_SRT
#elif defined( D_SPLIT_SHADOW )
FRAGMENT_MAIN_COLOUR01_SRT
#else
FRAGMENT_MAIN_COLOUR_SRT
#endif
{
    vec2 lFragCoordsVec2;
#ifdef D_SPOTLIGHT

#ifndef D_PLATFORM_OPENGL
    lFragCoordsVec2.xy = IN(mScreenPositionVec4).xy * lUniforms.mpPerFrame->gFrameBufferSizeVec4.zw;
#else
    lFragCoordsVec2 = IN(mScreenSpacePositionVec4).xy / IN(mScreenSpacePositionVec4).w;
#endif

#else
    lFragCoordsVec2 = TEX_COORDS;
#endif

    vec4 lBuffer1_Vec4;
    lBuffer1_Vec4 = READ_GBUFFER( lUniforms.mpCustomPerMaterial, gBuffer1Map, lFragCoordsVec2 );

    #ifdef D_COMPUTE
        if (lBuffer1_Vec4.x > 0.9999999)
        {
            FRAGMENT_COLOUR = vec4( 0.0, 0.0, 0.0, 1.0 );
            return;
        }
    #endif

    vec3  lLightColourVec3;
    vec3  lPositionVec3;
    vec3  lLightDirectionVec3;
    float lfAttenuation;
    lPositionVec3 = DecodeGBufferPosition(lFragCoordsVec2,
        lUniforms.mpPerFrame->gClipPlanesVec4,
        lUniforms.mpCommonPerMesh->gInverseProjectionMat4,
        lUniforms.mpCommonPerMesh->gInverseViewMat4,
#ifdef D_SPOTLIGHT
        float2vec3( 0.0 ), 
        lBuffer1_Vec4,
        true
#else
        lUniforms.mpPerFrame->gViewPositionVec3,
        lBuffer1_Vec4,

        false
#endif
        );

    #ifdef D_SPOTLIGHT
    {       
        float lfLightIntensity = lUniforms.mpCommonPerMesh->gLightColourVec4.w;
        //float lfLightRadius       = lUniforms.mpCommonPerMesh->gSpotlightPositionVec4.w;
        float lfFalloffType       = lUniforms.mpCommonPerMesh->gSpotlightPositionVec4.w;
        vec3 lLightPositionVec3  = lUniforms.mpCommonPerMesh->gSpotlightPositionVec4.xyz;
                            
        vec3 lPosToLight         = lLightPositionVec3 - lPositionVec3;

        float lfCutOff = 0.05;

        if (lfFalloffType == 2.0)
        {
            // Quadratic Distance attenuation
            float lfDistanceSquared = dot(lPosToLight, lPosToLight);
            lfAttenuation = lfLightIntensity / max(1.0, lfDistanceSquared);
            if (lfAttenuation <= (lfCutOff / (1.0 - lfCutOff)))
            {
                discard;
            }
        }
        else if (lfFalloffType == 1.0)
        {
            // Linear Distance attenuation
            //float lfLightDistance = length(lPosToLight);
            //lfAttenuation = 1.0 / max(1.0, lfLightDistance);
            float lfDistanceSquared = dot(lPosToLight, lPosToLight);
            lfAttenuation = invsqrt(lfDistanceSquared);
            lfAttenuation = min( lfAttenuation, 1.0 );
            lfAttenuation *= lfLightIntensity;
            if (lfAttenuation <= (lfCutOff / (1.0 - lfCutOff)))
            {
                discard;
            }
        }
        else
        {
            // Constant Distance attenuation
            lfAttenuation = lfLightIntensity;
        }

        lLightDirectionVec3 = normalize(lPosToLight);

        // Conelight falloff (this can only attenuate down)
        float lfLightFOV = lUniforms.mpCommonPerMesh->gSpotlightDirectionVec4.w;
        if (lfLightFOV > -2.0)
        {
            float lfConeAngle = dot( lUniforms.mpCommonPerMesh->gSpotlightDirectionVec4.xyz, -lLightDirectionVec3 );
            float lfConeAttenuation   = saturate( (lfConeAngle - lfLightFOV) * 5.0 );
            //lfConeAttenuation  *= lfConeAttenuation;                           
            lfAttenuation      *= lfConeAttenuation;                                  
            if (lfAttenuation <= (lfCutOff / (1.0 - lfCutOff)))
            {
                discard;
            }
        }

        lLightColourVec3    = lUniforms.mpCommonPerMesh->gLightColourVec4.xyz * lUniforms.mpCommonPerMesh->gLightColourVec4.w;
        lfAttenuation = (lfAttenuation - lfCutOff) / (1.0 - lfCutOff);
        lfAttenuation = max(lfAttenuation, 0.0);
        //FRAGMENT_COLOUR = vec4( 0.0, 1.0 * lfAttenuation, 0.0, 1.0 );//lUniforms.mpCommonPerMesh->gSpotlightPositionVec4;
        //FRAGMENT_COLOUR = lUniforms.mpCommonPerMesh->gSpotlightDirectionVec4;
        //return;
        lPositionVec3 += lUniforms.mpPerFrame->gViewPositionVec3;
    } 
    #else
    {
        lLightColourVec3    = lUniforms.mpCommonPerMesh->gLightColourVec4.xyz * lUniforms.mpCommonPerMesh->gLightColourVec4.w;
        lLightDirectionVec3 = -lUniforms.mpPerFrame->gLightDirectionVec4.xyz;
        lfAttenuation       = 1.0;
    }
    #endif

    vec3  lFragmentColourVec3;
    vec3  lSunColourVec3 = float2vec3(0.0);
    vec3  lColourVec3;
    vec3  lNormalVec3;
    float lfMetallic;
    float lfRoughness;
    float lfGlow;
    int   liMaterialID;
    float lfSubsurface;

    {
        vec4 lBuffer0_Vec4;
        vec4 lBuffer2_Vec4;
        vec4 lBuffer3_Vec4;
        lBuffer0_Vec4 = READ_GBUFFER( lUniforms.mpCustomPerMaterial, gBufferMap,  lFragCoordsVec2 );
        lBuffer2_Vec4 = READ_GBUFFER( lUniforms.mpCustomPerMaterial, gBuffer2Map, lFragCoordsVec2 );
        lBuffer3_Vec4 = READ_GBUFFER( lUniforms.mpCustomPerMaterial, gBuffer3Map, lFragCoordsVec2 );

        DecodeGBuffer(
            lFragCoordsVec2,
            lUniforms.mpPerFrame->gClipPlanesVec4,
            lUniforms.mpCommonPerMesh->gInverseProjectionMat4,
            lUniforms.mpCommonPerMesh->gInverseViewMat4,
            lUniforms.mpPerFrame->gViewPositionVec3,
            lBuffer0_Vec4,
            lBuffer1_Vec4,
            lBuffer2_Vec4,
            lBuffer3_Vec4,
            false,
            true,
            lColourVec3,
            lPositionVec3,
            lNormalVec3,
            liMaterialID,
            lfRoughness,
            lfMetallic,
            lfSubsurface,
            lfGlow);
    }

    /*
#if !defined( D_SPOTLIGHT )
    if( ( liMaterialID & D_DISABLE_POSTPROCESS ) == 0 )
    {
        vec4 lScreenSpacePos = vec4( lFragCoordsVec2.x, lFragCoordsVec2.y, 1.0, 1.0 );
        lColourVec3 = PostProcessPreLighting(
            DEREF_PTR( lUniforms.mpPerFrame ),
            DEREF_PTR( lUniforms.mpCommonPerMesh ),
            DEREF_PTR( lUniforms.mpCustomPerMaterial ),
            vec4( lColourVec3, 1.0 ),
            lPositionVec3,
            lNormalVec3,
            lfRoughness,
            liMaterialID,
            lScreenSpacePos 
            ).rgb;
    }
#endif
    */

#ifndef D_LIGHT_TERRAIN
    if ( (liMaterialID & D_UNLIT) != 0 )
    {
        lFragmentColourVec3 = lColourVec3;
    }
    else
#endif
    {
    	mat3 lUpMatrix;
#ifndef D_SPOTLIGHT    	
        #ifdef D_COMPUTE
        MAT3_SET_COLUMN(lUpMatrix, 0, lUniforms.mpCommonPerMesh->gInverseWorldUpMatVec4[0].xyz);
        MAT3_SET_COLUMN(lUpMatrix, 1, lUniforms.mpCommonPerMesh->gInverseWorldUpMatVec4[1].xyz);
        MAT3_SET_COLUMN(lUpMatrix, 2, lUniforms.mpCommonPerMesh->gInverseWorldUpMatVec4[2].xyz);
        #else
    	lUpMatrix = IN( mUpMatrix );   // GetInverseWorldUpTransform( lPerFrameUniforms.gViewPositionVec3, lMeshUniforms.gPlanetPositionVec4 ); 
        #endif
#endif
        float lAO = texture2DLod(SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gBuffer4Map ), lFragCoordsVec2, 0.0).x;
#ifdef D_SPOTLIGHT
        lAO = mix(0.61, 1.0, lAO);
#endif
        lFragmentColourVec3 = ComputeLightColour(
            DEREF_PTR( lUniforms.mpCustomPerMaterial ),
            DEREF_PTR( lUniforms.mpPerFrame ),
            DEREF_PTR( lUniforms.mpCommonPerMesh ),
            SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMaterial, gDualPMapBack),
            SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMaterial, gDualPMapFront),
            lLightDirectionVec3,
            lLightColourVec3,
            lPositionVec3,
            lFragCoordsVec2,
            lNormalVec3,
            lColourVec3,
            lUpMatrix,
            liMaterialID,
            lfMetallic,
            lfRoughness,
            lfSubsurface,
            0.5,
            //1.0 );
            lAO,
            lSunColourVec3
            );
    }

    lFragmentColourVec3 *= lfAttenuation;

//#ifdef D_SPOTLIGHT
//    float lfLength       = length( lFragmentColourVec3 );
//    lfLength             = smoothstep( 0.05, 0.1, lfLength );
//    lFragmentColourVec3  *= lfLength;
//#endif

#if !defined( D_SPOTLIGHT )
    if ( (liMaterialID & D_DISABLE_POSTPROCESS) == 0 )
    {
        vec4 lScreenSpacePos = vec4(lFragCoordsVec2.x, lFragCoordsVec2.y, 1.0, 1.0);
        lFragmentColourVec3 = PostProcess(
            DEREF_PTR( lUniforms.mpPerFrame ),
            DEREF_PTR( lUniforms.mpCommonPerMesh ),
            DEREF_PTR( lUniforms.mpCustomPerMaterial ),
            vec4( lFragmentColourVec3, 1.0 ),
            lPositionVec3,
            lNormalVec3,
            liMaterialID,
            lScreenSpacePos,
            lSunColourVec3
            ).rgb;
    }
#endif

    #ifdef D_SPLIT_SHADOW
    FRAGMENT_COLOUR0 = vec4( lFragmentColourVec3, 1.0 ); 
    FRAGMENT_COLOUR1 = vec4( lSunColourVec3, 1.0 ); 
    #else
    FRAGMENT_COLOUR = vec4( lFragmentColourVec3, 1.0 ); 
    #endif

    //FRAGMENT_COLOUR = vec4( lfRoughness * ( 1.0 / 16.0 ), saturate(lfMetallic), saturate(lfSubsurface), 1.0); 
    //FRAGMENT_COLOUR = vec4( 1.0, 0.0, 0.0, 1.0 );
}

#endif // D_LIGHT

#ifdef D_POSTPROCESS

//-----------------------------------------------------------------------------
//      Includes

#include "Common/CommonLighting.shader.h"
#include "OutputPostProcess.shader.h"

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION
    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END

//-----------------------------------------------------------------------------
//    Functions


//-----------------------------------------------------------------------------
///
///     Main
///
///     @brief      Fragment Main
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
FRAGMENT_MAIN_COLOUR_SRT
{
    
    vec2 lFragCoordsVec2 = IN( mTexCoordsVec2 );

    //vec3  lFragmentColourVec3;
    vec3  lFragmentColourVec3;
    vec3  lColourVec3;
    vec3  lPositionVec3;
    vec3  lNormalVec3;
    float lfMetallic;
    float lfRoughness;            
    float lfGlow;            
    int   liMaterialID;            
    float lfSubsurface;

    {
        vec4 lBuffer0_Vec4;
        vec4 lBuffer1_Vec4;
        vec4 lBuffer2_Vec4;
        vec4 lBuffer3_Vec4;

        lBuffer0_Vec4 = texture2D( lUniforms.mpCustomPerMaterial->gBufferMap,  lFragCoordsVec2 );
        lBuffer1_Vec4 = texture2D( lUniforms.mpCustomPerMaterial->gBuffer1Map, lFragCoordsVec2 );
        lBuffer2_Vec4 = texture2D( lUniforms.mpCustomPerMaterial->gBuffer2Map, lFragCoordsVec2 );
        lBuffer3_Vec4 = texture2D( lUniforms.mpCustomPerMaterial->gBuffer3Map, lFragCoordsVec2 );

        DecodeGBuffer(
        	lFragCoordsVec2,
        	lUniforms.mpPerFrame->gClipPlanesVec4,
            lUniforms.mpCommonPerMesh->gInverseProjectionMat4,
            lUniforms.mpCommonPerMesh->gInverseViewMat4,
            lUniforms.mpPerFrame->gViewPositionVec3,
            lBuffer0_Vec4,
            lBuffer1_Vec4, 
            lBuffer2_Vec4,
            lBuffer3_Vec4,
            true,
            false,
            lColourVec3,
            lPositionVec3,
            lNormalVec3,
            liMaterialID,
            lfRoughness,
            lfMetallic,
            lfSubsurface,
            lfGlow );
    }
    
    if ( (liMaterialID & D_DISABLE_POSTPROCESS) == 0 )
    {
        vec4 lColourVec4 = vec4( lColourVec3, 1.0 );
        vec4 lScreenSpacePos = vec4(lFragCoordsVec2.x, lFragCoordsVec2.y, 1.0, 1.0);
        lColourVec3 = PostProcess(
            DEREF_PTR( lUniforms.mpPerFrame ),
            DEREF_PTR( lUniforms.mpCommonPerMesh ),
            DEREF_PTR( lUniforms.mpCustomPerMaterial ),
            lColourVec4,
            lPositionVec3,
            lNormalVec3,
            liMaterialID,
            lScreenSpacePos ).rgb;
    }

    //if( IN( mTexCoordsVec2 ).x < 0.5 )
    //{
    //    lColourVec3 = normalize( lPositionVec3 - lUniforms.mpPerFrame->gViewPositionVec3 ) * 0.5 + vec3( 0.5 );
    //}

 
    FRAGMENT_COLOUR = vec4( lColourVec3, /* lfGlow */ 1.0 );
}

#endif // D_POSTPROCESS

#ifdef D_SHADOW_APPLY

//-----------------------------------------------------------------------------
//      Includes

#include "Common/CommonLighting.shader.h"
#include "OutputPostProcess.shader.h"

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION
    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
    flat INPUT( mat3, mUpMatrix,     TEXCOORD1 )   
DECLARE_END

//-----------------------------------------------------------------------------
//    Functions

//-----------------------------------------------------------------------------
///
///     Main
///
///     @brief      Fragment Main
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
FRAGMENT_MAIN_COLOUR_SRT
{
    vec2 lFragCoordsVec2 = TEX_COORDS;

    vec4 lBuffer1_Vec4 = READ_GBUFFER( lUniforms.mpCustomPerMaterial, gBuffer1Map, lFragCoordsVec2 );

    vec3 lPositionVec3 = DecodeGBufferPosition(
        lFragCoordsVec2,
        lUniforms.mpPerFrame->gClipPlanesVec4,
        lUniforms.mpCommonPerMesh->gInverseProjectionMat4,
        lUniforms.mpCommonPerMesh->gInverseViewMat4,
        lUniforms.mpPerFrame->gViewPositionVec3,
        lBuffer1_Vec4,
        false );

    float lfShadow = ComputeShadowIntensity( 
        DEREF_PTR( lUniforms.mpCustomPerMaterial ), 
        DEREF_PTR( lUniforms.mpPerFrame ), 
        DEREF_PTR( lUniforms.mpCommonPerMesh ), 
        lPositionVec3, 
        vec3( 0.0, 0.0, 0.0 ), 
        lFragCoordsVec2, 
        true );

    float lfCloudShadow = ComputeCloudOverlay(
        lFragCoordsVec2,
        SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMaterial, gCloudShadowMap));

    lfShadow *= lfCloudShadow;


    //if( IN( mTexCoordsVec2 ).x < 0.5 )
    //{
    //    lColourVec3 = normalize( lPositionVec3 - lUniforms.mpPerFrame->gViewPositionVec3 ) * 0.5 + vec3( 0.5 );
    //}

    vec3 lSunColour = READ_GBUFFER( lUniforms.mpCustomPerMaterial, gBufferMap, lFragCoordsVec2 ).rgb;
 
    FRAGMENT_COLOUR = vec4( lSunColour * lfShadow, 1.0 );
}

#endif // D_POSTPROCESS

