////////////////////////////////////////////////////////////////////////////////
///
///     @file       PostProcessFragment.h
///     @author     User
///     @date       
///
///     @brief      DepthOfFieldFragmentShader
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

#include "Common/CommonUniforms.shader.h"

#include "Common/Common.shader.h"


//-----------------------------------------------------------------------------
///
///     CustomPerMaterialUniforms
///
///     @brief      CustomPerMaterialUniforms
///
///     Stuff that is only used for these types of meshes.
//-----------------------------------------------------------------------------



#include "Fullscreen/PostCommon.h"

#include "Common/CommonPostProcess.shader.h"
#include "Common/CommonDepth.shader.h"

// must include this after CommonUniforms
//#include "Fullscreen/PostCommon.h"

#include "Common/CommonGBuffer.shader.h"

//-----------------------------------------------------------------------------
//      Const Data


STATIC_CONST float kfDistanceThreshold = 5.0;
STATIC_CONST float kfFilterPixels      = 20.0;

STATIC_CONST int sample_count = 16;
STATIC_CONST vec2 poisson16[16] =
#ifdef D_PLATFORM_OPENGL
vec2[16](    // These are the Poisson Disk Samples
#else
{
#endif
    vec2( -0.94201624, -0.39906216 ),
    vec2( 0.94558609, -0.76890725 ),
    vec2( -0.094184101, -0.92938870 ),
    vec2( 0.34495938, 0.29387760 ),
    vec2( -0.91588581, 0.45771432 ),
    vec2( -0.81544232, -0.87912464 ),
    vec2( -0.38277543, 0.27676845 ),
    vec2( 0.97484398, 0.75648379 ),
    vec2( 0.44323325, -0.97511554 ),
    vec2( 0.53742981, -0.47373420 ),
    vec2( -0.26496911, -0.41893023 ),
    vec2( 0.79197514, 0.19090188 ),
    vec2( -0.24188840, 0.99706507 ),
    vec2( -0.81409955, 0.91437590 ),
    vec2( 0.19984126, 0.78641367 ),
    vec2( 0.14383161, -0.14100790 )
#ifdef D_PLATFORM_OPENGL
    );
#else
};
#endif


// =================================================================================================
//
// COPY
//
// =================================================================================================

//#ifdef D_SSAO

//-----------------------------------------------------------------------------
//      Global Data



//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
	INPUT_SCREEN_POSITION
	INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END


vec3
DecodeGBufferPosition(
    in  vec2  lScreenPosVec2,
    in  vec4  lClipPlanes,
    in  mat4  lInverseProjectionMat4,
    in  mat4  lInverseViewMat4,
    in  vec3  lViewPositionVec3,
    in  vec4  lBuffer1_Vec4 )
{
    float lfDepth = FastDenormaliseDepth( lClipPlanes, DecodeDepthFromColour( lBuffer1_Vec4 ) );
    vec3 lPositionVec3 = RecreatePositionFromDepth( lfDepth, lScreenPosVec2, lViewPositionVec3, lInverseProjectionMat4, lInverseViewMat4 );

    return lPositionVec3;
}


//-----------------------------------------------------------------------------
//      Functions 


FRAGMENT_MAIN_COLOUR_SRT
{
    //FRAGMENT_COLOUR = texture2D( lUniforms.mpCustomPerMesh->gBufferMap, IN(mTexCoordsVec2) );

    vec2  filterRadius = lUniforms.mpPerFrame->gFrameBufferSizeVec4.zw * kfFilterPixels;
    vec2  lFragCoordsVec2 = IN( mTexCoordsVec2 );

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
        vec4 lBuffer0_Vec4	= texture2D(FRAGMENT_COLOUR1, lFragCoordsVec2 );
        vec4 lBuffer1_Vec4	= texture2D(FRAGMENT_COLOUR2, lFragCoordsVec2 );
        vec4 lBuffer2_Vec4	= texture2D(FRAGMENT_COLOUR3, lFragCoordsVec2 );
        vec4 lBuffer3_Vec4	= texture2D(FRAGMENT_COLOUR4, lFragCoordsVec2 );

//        lBuffer0_Vec4 = texture2D( lUniforms.mpCustomPerMesh->gBufferMap, lFragCoordsVec2 );
//        lBuffer1_Vec4 = texture2D( lUniforms.mpCustomPerMesh->gBuffer1Map, lFragCoordsVec2 );
//        lBuffer2_Vec4 = texture2D( lUniforms.mpCustomPerMesh->gBuffer2Map, lFragCoordsVec2 );
//        lBuffer3_Vec4 = texture2D( lUniforms.mpCustomPerMesh->gBuffer3Map, lFragCoordsVec2 );

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
            lfGlow );
    }

    // reconstruct position from depth, USE YOUR CODE HERE
    //float depth  = texture( depthTexture, texcoordFS ).r;
    //vec3 viewPos = calulatePosition( texcoordFS, depth );
    //vec3 viewPos = lPositionVec3 - lUniforms.mpPerFrame->gViewPositionVec3;
    vec3 viewPos = lPositionVec3;

    // get the view space normal, USE YOUR CODE HERE
    //vec2 normalXY   = texture( normalTexture, texcoordFS ).xy * 2.0 - 1.0;
    //vec3 viewNormal = decodeNormal( normalXY );
    //vec3 viewNormal = MUL( lUniforms.mpPerFrame->gViewMat4, lNormalVec3 );
    mat3 lCameraMatInverseRotMat3;
    mat4 lCameraMatInverseMat4;

    lCameraMatInverseMat4 = transpose( lUniforms.mpPerFrame->gCameraMat4 );
    lCameraMatInverseRotMat3[0] = lCameraMatInverseMat4[0].xyz;
    lCameraMatInverseRotMat3[1] = lCameraMatInverseMat4[1].xyz;
    lCameraMatInverseRotMat3[2] = lCameraMatInverseMat4[2].xyz;

    //vec3 viewNormal = MUL( lCameraMatInverseRotMat3, lNormalVec3 );
    vec3 viewNormal = lNormalVec3;

    float ambientOcclusion = 0.0;
    // perform AO
    for ( int i = 0; i < sample_count; ++i )
    {
        // sample at an offset specified by the current Poisson-Disk sample and scale it by a radius (has to be in Texture-Space)
        vec2 sampleTexCoord = lFragCoordsVec2 + ( poisson16[i] * ( filterRadius ) );
        //float sampleDepth = texture( depthTexture, sampleTexCoord ).r;

        //vec3 samplePos = calculatePosition( sampleTexCoord, sampleDepth * 2 - 1 );
        vec3 samplePos;

        {
//            vec4 lBuffer1_Vec4 = texture2D( lUniforms.mpCustomPerMesh->gBuffer1Map, sampleTexCoord );
            vec4 lBuffer1_Vec4 = texture2D( FRAGMENT_COLOUR1, sampleTexCoord );

            samplePos = DecodeGBufferPosition(  sampleTexCoord,
                                                lUniforms.mpPerFrame->gClipPlanesVec4,
                                                lUniforms.mpCommonPerMesh->gInverseProjectionMat4,
                                                lUniforms.mpCommonPerMesh->gInverseViewMat4,
                                                lUniforms.mpPerFrame->gViewPositionVec3,
                                                lBuffer1_Vec4 );

            //samplePos -= lUniforms.mpPerFrame->gViewPositionVec3;
        }

        vec3 sampleDir = normalize( samplePos - viewPos );

        // angle between SURFACE-NORMAL and SAMPLE-DIRECTION (vector from SURFACE-POSITION to SAMPLE-POSITION)
        float NdotS = max( dot( viewNormal, sampleDir ), 0.0 );
        // distance between SURFACE-POSITION and SAMPLE-POSITION


        //float VPdistSP = distance( viewPos, samplePos );
        float VPdistSP = length( samplePos - viewPos );

        // a = distance function
        float a = 1.0 - smoothstep( kfDistanceThreshold, kfDistanceThreshold * 2.0, VPdistSP );
        // b = dot-Product
        //float b = NdotS;
        float b = max(NdotS, 0.15);

        ambientOcclusion += ( a * b );
    }

    ambientOcclusion = 1.0 - ( ambientOcclusion / sample_count );

    FRAGMENT_COLOUR = vec4( vec3( ambientOcclusion ), 1.0 );
}

//#endif