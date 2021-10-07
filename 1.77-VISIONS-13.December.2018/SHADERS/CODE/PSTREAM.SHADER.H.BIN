////////////////////////////////////////////////////////////////////////////////
///
///     @file       
///     @author     hdenholm
///     @date       
///
///     @brief      
///
///     Copyright (c) 2014 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
//      Include files

#include "Common/Defines.shader.h"
#include "Common/CommonUniforms.shader.h"
#include "Common/CommonRandom.shader.h"
#include "Common/CommonNoise.shader.h"

struct CustomPerMaterialUniforms
{
    vec4 gSpaceCloudColourVec4;
    vec4 gSpaceNebulaColour3Vec4;
    vec4 gSpaceNebulaParamsVec4;
    vec4 gSpaceSkyColour1Vec4;

#if defined(D_PLATFORM_DX12)
};
#endif
    SAMPLER2D( gDistortMap );
    SAMPLER2D( gFieldMap );

    SAMPLER2D(  gNoiseMap );
    SAMPLER2D( gPlasmaMap );

#if !defined(D_PLATFORM_DX12)
};
#endif




struct CustomPerMeshUniforms
{
	vec4 gDoFParamsVec4;
};

//
// This is the SRT buffer that everything gets uploaded to (on PS4). PC just functions as normal. Declare it AFTER commonuniforms is included.
//
struct UniformBuffer
{
     DECLARE_PTR( PerFrameUniforms,             mpPerFrame )   /*: PER_MATERIAL*/ // sematics currently crash the compiler so the parser is hardcoded to look for names.
     DECLARE_PTR( CommonPerMeshUniforms,        mpCommonPerMesh )       /*: PER_MESH*/
     DECLARE_PTR( CustomPerMeshUniforms,        mpCustomPerMesh )       /*: PER_MESH*/
     DECLARE_PTR( CustomPerMaterialUniforms,    mpCustomPerMaterial )       /*: PER_MATERIAL*/
};


//-----------------------------------------------------------------------------
//    Functions

float DistanceFalloff(
    in vec4  lv_MinDist_RecipMaxDist,
    in float depth)
{
  return saturate( (depth - lv_MinDist_RecipMaxDist.x) * lv_MinDist_RecipMaxDist.y );
}



// =============================================================================================

#ifdef D_CARDS

#ifdef D_VERTEX

#include "Common/CommonDepth.shader.h"

//-----------------------------------------------------------------------------
DECLARE_INPUT
    INPUT(  vec2, mkfParticleCornerId,  POSITION0 )
    
    INPUT(  vec3, mkCustom1Vec4,        TEXCOORD0 )
    INPUT(  vec4, mkCustom2Vec4,        TEXCOORD1 )
    INPUT(  vec4, mkCustom3Vec4,        TEXCOORD2 )
DECLARE_END

DECLARE_OUTPUT
    OUTPUT_SCREEN_POSITION

    OUTPUT( vec2, mUV,                  TEXCOORD0 )
    OUTPUT( vec4, mDepth_Field_Seed2,   TEXCOORD1 )
    OUTPUT( vec4, mColour,              TEXCOORD2 )
    OUTPUT( vec3, mPlasmaChoice,        TEXCOORD3 )
DECLARE_END

//-----------------------------------------------------------------------------
VERTEX_MAIN_SRT
{
    vec3 lViewAt        = MAT4_GET_COLUMN( lUniforms.mpPerFrame->gCameraMat4, 2 );

    float lParticleSize = IN(mkCustom1Vec4).x;
    vec2 lCornerOffset  = IN(mkfParticleCornerId) - 0.5;

    vec3 lCardCenter    = IN(mkCustom2Vec4).xyz;
    vec3 lNormal        = IN(mkCustom3Vec4).xyz;

    vec3 lTangent1      = cross( lNormal, vec3( 0.0, 1.0, 0.0) );
    vec3 lTangent2      = cross( lNormal, lTangent1 );

    vec3 lCardEdge      = lCardCenter;
    lCardEdge          += lTangent1 * (lCornerOffset.x * lParticleSize);
    lCardEdge          += lTangent2 * (lCornerOffset.y * lParticleSize);

    vec4 lvPositionToTransformVec4 = vec4( lCardEdge, 1.0 );

    vec4 lvWorldPositionVec4 = MUL( lUniforms.mpCommonPerMesh->gWorldMat4, lvPositionToTransformVec4 );
    vec4 lScreenSpacePositionVec4 = MUL( lUniforms.mpCommonPerMesh->gViewProjectionMat4, lvWorldPositionVec4);


    vec3 lCamPos         = MAT4_GET_COLUMN( lUniforms.mpPerFrame->gCameraMat4, 3 );
    float lfDistToCamera = distance(lCamPos, lvWorldPositionVec4.xyz);
    float lfDistFromCameraNormalised = DistanceFalloff( lUniforms.mpCommonPerMesh->gLightColourVec4, lfDistToCamera );


    // we choose a fade value based on
    //      - world-point-to-camera angle (to fade as that diminishes)
    //      - alignment to normal (fade as we approach perp)
    //      - fade in and out on camera distance

    vec3 lWorldToCam  = normalize( lCamPos - lvWorldPositionVec4.xyz );
    float lAlpha      = saturate( dot( lWorldToCam, lViewAt ) );

    float lAngle      = abs( dot( lNormal, lViewAt ) );
    lAngle            = smoothstep( 0.5, 1.0, lAngle );

    lAlpha            = min( lAlpha, lAngle );

    lAlpha           *= 1.0 - smoothstep( 0.1, 0.75, lfDistFromCameraNormalised );
    lAlpha           *= smoothstep( 0.01, 0.05, lfDistFromCameraNormalised );


    // turn a 0..1 value into a 3-way choice, producing [1,0,0] -> [0,1,0] -> [0,0,1] for each third
    // this is then used to choose one channel from the RGB plasma texture
    vec3 lPlasmaShift = float2vec3( IN(mkCustom1Vec4).z );
    vec3 lPlasmaChoice = step( vec3(0.0, 0.33, 0.66), lPlasmaShift ) - step( vec3(0.33, 0.66, 1.0), lPlasmaShift );


    SCREEN_POSITION = lScreenSpacePositionVec4;

    OUT(mUV)                  = IN(mkfParticleCornerId);
    OUT(mDepth_Field_Seed2)   = vec4( IN(mkCustom2Vec4).w, 0.0, IN(mkCustom1Vec4).y, IN(mkCustom3Vec4).a );
    OUT(mColour)              = vec4( 0, 0, 0, lAlpha );
    OUT(mPlasmaChoice)        = lPlasmaChoice;
}

#endif // D_VERTEX



#ifdef D_FRAGMENT

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mUV,                 TEXCOORD0 )
    INPUT( vec4, mDepth_Field_Seed2,  TEXCOORD1 )
    INPUT( vec4, mColour,             TEXCOORD2 )
    INPUT( vec3, mPlasmaChoice,       TEXCOORD3 )
DECLARE_END

FRAGMENT_MAIN_COLOUR_SRT
{
    float lFade                 = IN(mColour).a;

    float lfDistToCentreOfUV    = distance( vec2(0.5, 0.5), IN(mUV) ) * 2.0;

    // fade out to edges
    lFade -= lfDistToCentreOfUV;

    if ( lFade <= 0.0 )
    {
        discard;
    }
    else
    {
        float lSubtraction          = IN(mDepth_Field_Seed2.x);
        float lRandomUVOffset       = IN(mDepth_Field_Seed2.w);

        // shuffle UV, textures are tiling
        vec2 lSampleUV              = IN(mUV) + vec2( lRandomUVOffset, lRandomUVOffset );

        // sample 3 different plasma images encoded in one rgb tex, mask out just one channel based on random input
        float lPlasma               = dot( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gPlasmaMap ), lSampleUV ).rgb, IN(mPlasmaChoice) );

        FRAGMENT_COLOUR             = vec4( lSubtraction, lSubtraction, lSubtraction, lPlasma * lFade * IN(mDepth_Field_Seed2.z) );
    }
}

#endif // D_FRAGMENT

#endif // D_CARDS


// =============================================================================================


#ifdef D_PSTREAM

#ifdef D_VERTEX

#include "Common/CommonDepth.shader.h"

//-----------------------------------------------------------------------------
DECLARE_INPUT
    INPUT(  vec2, mkfParticleCornerId,  POSITION0 )
    
    INPUT(  vec3, mkCustom1Vec4,        TEXCOORD0 )
    INPUT(  vec4, mkCustom2Vec4,        TEXCOORD1 )
    INPUT(  vec4, mkCustom3Vec4,        TEXCOORD2 )
DECLARE_END

DECLARE_OUTPUT
    OUTPUT_SCREEN_POSITION

    OUTPUT( vec2, mUV,                  TEXCOORD0 )
    OUTPUT( vec4, mDepth_Field_Seed2,   TEXCOORD1 )
    OUTPUT( vec4, mColour,              TEXCOORD2 )
#ifndef D_PSTREAM_FIELD
    OUTPUT( vec4, mBlends,              TEXCOORD3 )
    OUTPUT( vec4, mWorldPositionVec4,   TEXCOORD4 )
#endif

DECLARE_END



//-----------------------------------------------------------------------------
///
///     Vertex Main
///
//-----------------------------------------------------------------------------
VERTEX_MAIN_SRT
{
    vec4  lScreenSpacePositionVec4;
    float lfLogDepth = 0.0;

    mat4 lTranspViewMat4 = (lUniforms.mpPerFrame->gCameraMat4);
   // mat4 lTranspViewMat4 = lUniforms.mpPerFrame->gViewMat4;
    vec3 lViewUp    = MAT4_GET_COLUMN( lTranspViewMat4, 1 );
    vec3 lViewRight = MAT4_GET_COLUMN( lTranspViewMat4, 0 );


    float lParticleSize = IN(mkCustom1Vec4).x;
    vec2  lCornerOffset = IN(mkfParticleCornerId) - 0.5;

    vec3 lParticlePos = IN(mkCustom2Vec4).xyz;
    vec2  lHashValue    = Hash2to3( lParticlePos.xz ).xy;
    
    vec4 lInputColour = IN(mkCustom3Vec4);
    
#ifdef D_PSTREAM_FIELD
    float lParticleRandom = IN(mkCustom1Vec4).y;
    lInputColour.a = lParticleRandom;
#endif
    
    
    float lfFieldValue = 0.0;
#ifdef D_PSTREAM_FIELD
	vec3 lvStableNoiseSamplePos = lUniforms.mpCustomPerMesh->gDoFParamsVec4.xyz + (lParticlePos * 0.1);
    lfFieldValue = saturate( 0.5 + (iqNoise3(lvStableNoiseSamplePos) * 0.5) );
#endif


    vec4 lvPositionToTransformVec4 = vec4(lParticlePos, 1.0);

    vec4 lvWorldPositionVec4 = MUL( lUniforms.mpCommonPerMesh->gWorldMat4, lvPositionToTransformVec4 );

    // form view-facing billboard
    lvWorldPositionVec4.xyz += lViewRight * ( lCornerOffset.x * lParticleSize );
    lvWorldPositionVec4.xyz += lViewUp    * ( lCornerOffset.y * lParticleSize );

    lScreenSpacePositionVec4 = MUL( lUniforms.mpCommonPerMesh->gViewProjectionMat4, lvWorldPositionVec4 );

    vec3 lCamPos   = MAT4_GET_COLUMN( lUniforms.mpPerFrame->gCameraMat4, 3 );
    vec3 lToCamera = lvWorldPositionVec4.xyz - lCamPos;
    float lfDistToCamera = length( lToCamera );
    lToCamera = normalize( lToCamera );

    // gLightColourVec4 xy has the near fade and falloff
    float lfDistFromCameraNormalised = DistanceFalloff( lUniforms.mpCommonPerMesh->gLightColourVec4, lfDistToCamera );
    
    SCREEN_POSITION = lScreenSpacePositionVec4;
    
    vec2 lParticleUV = IN(mkfParticleCornerId);

    vec4 lBlendValues;

#ifdef D_PSTREAM_STARS

    float lfInverseDistanceFromCamera = 1.0 - lfDistFromCameraNormalised;
    lBlendValues.x   = smoothstep( 0.88, 1.0, lfInverseDistanceFromCamera ); // lfColourTransit_DistFromCam
    lBlendValues.y   = smoothstep( 0.96, 1.0, lfInverseDistanceFromCamera ); // lfCoronaBlend_DistFromCam
    lBlendValues.z   = smoothstep( 0.10, 1.0, lfInverseDistanceFromCamera ); // lfGlowFade_DistFromCam

    float lfCoreBlur    = saturate((lfDistFromCameraNormalised - 0.2f) * 1.6f);
    lfCoreBlur          = sin(lfCoreBlur * 1.5707963);
    lBlendValues.w      = lfCoreBlur;

    // blend a pale neutral blue to the star-colour based on being close to the camera
    //Do this based on the input alpha colour. When set to 1, we don't want this
    float lfBlend = (1.0 - lBlendValues.x) * (1.0 - lInputColour.a);
    vec3 lvTintColor = mix( 
        lInputColour.rgb, 
        vec3(0.873, 0.919, 1.0),
        lfBlend);

    lInputColour.rgb = lvTintColor;

#endif 

#ifdef D_PSTREAM_FIELD

    lBlendValues        = vec4( saturate( lfDistFromCameraNormalised * lfDistFromCameraNormalised * lfDistFromCameraNormalised ), 0, 0, 0 );

#endif 

    OUT(mUV)                = lParticleUV;
    OUT(mDepth_Field_Seed2) = vec4( lfDistFromCameraNormalised, lfFieldValue, lHashValue.x, lHashValue.y );
    OUT(mColour)            = lInputColour;
#ifndef D_PSTREAM_FIELD
    OUT(mBlends) = lBlendValues;
    OUT(mWorldPositionVec4) = vec4( normalize( lParticlePos ), 1.0 );
#else
    OUT(mDepth_Field_Seed2).x = lBlendValues.x;
#endif
	
}


#endif



// =============================================================================================




#ifdef D_FRAGMENT

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mUV,                 TEXCOORD0 )
    INPUT( vec4, mDepth_Field_Seed2,  TEXCOORD1 )
    INPUT( vec4, mColour,             TEXCOORD2 )
#ifndef D_PSTREAM_FIELD
    INPUT( vec4, mBlends,             TEXCOORD3 )
    INPUT( vec4, mWorldPositionVec4,  TEXCOORD4 )
#endif

DECLARE_END


#ifdef D_PSTREAM_STARS

//-----------------------------------------------------------------------------
///
///     Main
///
//-----------------------------------------------------------------------------
FRAGMENT_MAIN_COLOUR_SRT
{
    vec2 lvUV = IN(mUV);
    vec2 lvSeed = IN(mDepth_Field_Seed2).zw;

    float lfFODistanceFromCamera = IN(mDepth_Field_Seed2).x;

    vec4 lBlendValues = IN(mBlends);

#ifdef D_PSTREAM_SOLARSYSTEM
    float lfColourTransit_DistFromCam   = lBlendValues.x;
    float lfCoronaBlend_DistFromCam     = lBlendValues.y;
    float lfGlowFade_DistFromCam        = lBlendValues.z;
#else
    float lfColourTransit_DistFromCam   = 1.0;
    float lfCoronaBlend_DistFromCam     = 1.0;
    float lfGlowFade_DistFromCam        = 1.0;
#endif

    float lfDistToCentreOfUV            = distance( vec2(0.5, 0.5), lvUV );

    float lfUVDistInv                   = saturate( 1.0 - lfDistToCentreOfUV );
    float lfCoronaGlow                  = mix( 0.0, 1.0, smoothstep( 0.92,  0.96,  lfUVDistInv ) );
    float lfWhiteCorePrep               = mix( 0.0, 1.0, smoothstep( 0.94,  0.96,  lfUVDistInv ) );
    float lfWhiteCore                   = mix( 0.0, 1.0, smoothstep( 0.95,  0.96,  lfUVDistInv ) );
    float lfNoiseGlow                   = mix( 1.0, 0.0, smoothstep( 0.92,  0.96,  lfUVDistInv ) );
    float lfStarCoreAlpha               = lfWhiteCore;
    vec3 lvTintColor = IN(mColour.rgb);

    // glow halo
    {
      float lfNearShine = 1.0 / (1.0 + (lfDistToCentreOfUV * 6.0));
            lfNearShine = max( 0.0, lfNearShine - 0.25 ) * 1.4285;

      lfStarCoreAlpha = mix(
          lfStarCoreAlpha,
          1.0,
          (lfNearShine * lfNearShine) * lfGlowFade_DistFromCam );
    }

    vec3 lWhiteCoreColour = vec3( 1.0, 1.0, 1.0 );
    //Make the white core colour more like the tint colour based on the alpha value
    lWhiteCoreColour = mix( lWhiteCoreColour, lvTintColor, IN(mColour).a );

#ifndef D_PSTREAM_SOLARSYSTEM
    // the expensive-ish solar flare effect. hence the branch.
    if ( lfCoronaBlend_DistFromCam > 0.0 )
    {
        float lfTimeScale = 0.01 + (lvSeed.y * 0.01);
        float lfTimeShift = lvSeed.x - (lUniforms.mpCustomPerMesh->gDoFParamsVec4.w * lfTimeScale);  // we borrow gDoFParamsVec4 for some scale/time params

        vec2 lfP2 = ( lvUV - 0.5 );
        float lfPolar = atan2(lfP2.y, lfP2.x) * 0.1591549430918; // 1 / (2 * Pi)

        float lfDistortV = (lfDistToCentreOfUV * 1.5) + (lfTimeShift * 0.4);

        vec3 lWaveTex = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial , gDistortMap ), vec2( (lfTimeScale + lfPolar - lfDistortV), 1.0 * lfDistortV), 0.0 ).rgb;

        float lfCoronalDistortion  = lWaveTex.r;
              lfCoronalDistortion += lWaveTex.g;
              lfCoronalDistortion *= 0.75;


        float lfCoronal = lfCoronalDistortion * lfCoronaBlend_DistFromCam * lfCoronaGlow;

        lfStarCoreAlpha = mix(
            lfStarCoreAlpha,
            0.5 + (lWaveTex.b * 0.5),
            lfCoronal * lfCoronal );


        float lCoreTex = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gDistortMap ), vec2( lfPolar, lfTimeShift ), 0.0 ).b;
        
        lvTintColor = mix( lvTintColor, lvTintColor * 0.4, lCoreTex * lfCoronaBlend_DistFromCam * lfNoiseGlow );

        lvTintColor = mix( lvTintColor, lWhiteCoreColour, smoothstep( 0.75, 0.85, lfStarCoreAlpha ) );
        
    }
#endif

    vec4 lvOutputCol = vec4( lvTintColor, lfStarCoreAlpha );

    // push to bright white at the centre, depending on the alpha value in the colour
    vec4 lvTargetCore = mix( lvOutputCol, vec4( lWhiteCoreColour, 1.0 ), lfWhiteCorePrep );
    lvOutputCol       = mix( lvOutputCol, lvTargetCore, max( lfWhiteCore, IN( mColour ).a ) );

#ifdef D_PSTREAM_SOLARSYSTEM
    {
        float lfRandom      = lUniforms.mpCustomPerMaterial->gSpaceCloudColourVec4.a * 100.0f;// + lUniforms.mpPerFrame->gfTime * 0.001;
        float lfCloudScale1 = lUniforms.mpCustomPerMaterial->gSpaceNebulaColour3Vec4.a;
        float lfCloudBoost  = lUniforms.mpCustomPerMaterial->gSpaceNebulaParamsVec4.a;
        float lfCloudOffset = lUniforms.mpCustomPerMaterial->gSpaceSkyColour1Vec4.a;
        float lfCloudNoise1 = 1.0 - clamp( FractBrownianMotion4( IN( mWorldPositionVec4 ).xyz * lfCloudScale1 + vec3( lfRandom, lfRandom, lfRandom ), SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMaterial,gNoiseMap ) ) * 4.0 * lfCloudBoost + lfCloudOffset, 0.0, 1.0 );

        float lfSpeed       = 0.15;
        float lfCloudSpeed  = 0.15;
        float lfTimeFade1   = sin( lfSpeed      * 13.0 * lUniforms.mpPerFrame->gfTime + lvSeed.x * 11.0 );
        float lfTimeFade2   = sin( lfSpeed      * 23.0 * lUniforms.mpPerFrame->gfTime + lvSeed.x * 13.0 );
        float lfTimeFade3   = sin( lfSpeed      * 31.0 * lUniforms.mpPerFrame->gfTime + lvSeed.x * 31.0 );
        float lfTimeFade4   = sin( lfCloudSpeed * 2.0  * lUniforms.mpPerFrame->gfTime + lfCloudNoise1 * 23.0 );
        float lfTimeFade5   = sin( lfCloudSpeed * 5.0  * lUniforms.mpPerFrame->gfTime + lfCloudNoise1 * 47.0 );
        lfTimeFade1         = lfTimeFade1 * lfTimeFade2 * lfTimeFade3 * lfTimeFade4;
        lfTimeFade1         = clamp( lfTimeFade1 * 0.5 + 0.5, 0.0, 1.0 );

        lvOutputCol.rgb    *= lfTimeFade1 * ( lfCloudNoise1 + 0.3 ) * ( 1.0 + lfTimeFade4 * lfTimeFade5 * 0.6 );
        lvOutputCol.rgb    *= vec3( 3.0, 3.0, 3.0 );
        lvOutputCol.a      *= lfCloudNoise1;
    }
#endif

    // slow-in decay alpha away from camera
    float lfCoreBlur = lBlendValues.w;
    lvOutputCol.a = saturate(lvOutputCol.a - lfCoreBlur);
    //lvOutputCol.a = 1.0;

    if (lvOutputCol.a > 0.0)
    {
        FRAGMENT_COLOUR = lvOutputCol;
    } 
    else
    {
        discard;
    }
}

#endif // D_PSTREAM_STARS


#ifdef D_PSTREAM_FIELD

//-----------------------------------------------------------------------------
///
///     frag for the general diffuse particle effect
///
//-----------------------------------------------------------------------------
FRAGMENT_MAIN_COLOUR_SRT
{
    float lfFieldRandom             = IN(mDepth_Field_Seed2).y;
    vec2  lvUV                      = IN(mUV);

    float lfParticleDiffuse         = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gFieldMap ), lvUV ).r;
    if (lfParticleDiffuse == 0.0)
    {
        discard;
    }
    float lfFalloff     = IN(mDepth_Field_Seed2).x;

    lfParticleDiffuse  *= lfFieldRandom;
    lfParticleDiffuse  -= (lfFalloff * 0.4);
    if (lfParticleDiffuse <= 0.0)
    {
        discard;
    }
    lfParticleDiffuse   = saturate( lfParticleDiffuse );

    float lfAlpha       = (lfParticleDiffuse * IN(mColour).a);
    FRAGMENT_COLOUR     = vec4( IN(mColour).rgb, lfAlpha );
}

#endif // D_PSTREAM_FIELD

#endif

#endif // D_PSTREAM