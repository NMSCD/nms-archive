////////////////////////////////////////////////////////////////////////////////
///
///     @file       CloudFragment.h
///     @author     User
///     @date       
///
///     @brief      CloudFragment
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
//      Compilation defines 

#if defined( D_CLOUD_RENDER2D_VS )
#define D_VERTEX
#elif !defined D_FRAGMENT
#define D_FRAGMENT
#endif

#define D_USE_NOISETEXTURE
#define D_NORMALISED_NOISE

//-----------------------------------------------------------------------------
//      Include files

#include "Common/Defines.shader.h"
#include "Common/CommonUniforms.shader.h"
#include "Common/Common.shader.h"
#include "Common/CommonDepth.shader.h"

#include "Custom/CloudCommon.h"

#if defined(D_CLOUD_RENDER_REFLECTIONS)
#define CLOUD_RENDER_NUM_RAYS 10

#ifdef D_PLATFORM_ORBIS
#pragma argument(unrollallloops)
#endif

#else

#ifdef D_PLATFORM_PC_LOWEND

#define CLOUD_RENDER_NUM_RAYS 22

#else

#define CLOUD_RENDER_NUM_RAYS 44

#endif

#endif

#if defined( D_CLOUD_RENDER2D_VS )

#include "Common/CommonPlanet.shader.h"

// Custom Cloud Vertex Shader for 2D rendering

DECLARE_INPUT
    INPUT(  vec4, mkLocalPositionVec4, POSITION0 )
    INPUT(  vec2, mkTexCoordsVec4,     TEXCOORD0 )
DECLARE_END

DECLARE_OUTPUT

    OUTPUT_SCREEN_POSITION
    OUTPUT( vec2, mTexCoordsVec2,         TEXCOORD0 )
    flat OUTPUT( vec4, mUpVec3_mfCameraHeight, TEXCOORD1 )
    flat OUTPUT( vec3, mCross1Vec3,            TEXCOORD2 )
    flat OUTPUT( vec3, mCross2Vec3,            TEXCOORD3 )
    
DECLARE_END
         
VERTEX_MAIN_SRT
{    
    vec2 lTexCoordsVec2 = IN( mkTexCoordsVec4 );    

    vec4 lWorldPositionVec4 = MUL( kFSQuadProj, IN( mkLocalPositionVec4 ) );
    OUT( mTexCoordsVec2 ) = lTexCoordsVec2;

    SCREEN_POSITION = lWorldPositionVec4;
    
    // compute some (expensive) constants to pass along to the PS
    
    float lfCameraHeight = length( lUniforms.mpPerFrame->gViewPositionVec3 - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz ) - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w;
    lfCameraHeight      = ( lfCameraHeight - 800.0 ) / 200.0;
    lfCameraHeight      = 1.0 - clamp( lfCameraHeight, 0.0, 1.0f );    

    // point where the light ray meets the cloud plane
    vec3 lUp     = GetWorldUp( lUniforms.mpPerFrame->gViewPositionVec3, lUniforms.mpCommonPerMesh->gPlanetPositionVec4 );
    vec3 lCross1 = normalize( cross( lUp, vec3( 0.0, 0.0, 1.0 ) ) );
    vec3 lCross2 = normalize( cross( lUp, lCross1 ) );
    lCross1      = normalize( cross( lUp, lCross2 ) );
    
    OUT( mUpVec3_mfCameraHeight ) = vec4(lUp.xyz, lfCameraHeight);
    OUT( mCross1Vec3 )            = lCross1;
    OUT( mCross2Vec3 )            = lCross2;    
}

#endif


#if defined( D_CLOUD_RENDER ) || defined( D_CLOUD_RENDER2D )

#include "Common/CommonPlanet.shader.h"
#include "Common/Noise3D.glsl"
#include "Common/CommonNoise.shader.h"

//-----------------------------------------------------------------------------
//      Global Data


STATIC_CONST float kfCloudSharpness = 1.0;
STATIC_CONST vec3 kBRDF1Vec3        = vec3( 0.65, 0.68, 0.7 );
STATIC_CONST vec3 kBRDF2Vec3        = vec3( 0.7,  0.5,  0.3 );
STATIC_CONST vec3 kColourAVec3      = vec3( 0.6,  0.71, 0.75);
STATIC_CONST vec3 kColourBVec3      = vec3( 1.0,  0.5,  1.0 );
STATIC_CONST vec3 kColourCVec3      = vec3( 1.0,  0.6,  0.1 );
STATIC_CONST vec3 kColourDVec3      = vec3( 1.0,  0.4,  0.2 );


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

    INPUT( vec2, mTexCoordsVec2,                  TEXCOORD0 )
#if defined( D_CLOUD_RENDER2D )// && !defined D_COMPUTE
    flat INPUT( vec4, mUpVec3_mfCameraHeight, TEXCOORD1 )
    flat INPUT( vec3, mCross1Vec3,            TEXCOORD2 )
    flat INPUT( vec3, mCross2Vec3,            TEXCOORD3 )
#endif
DECLARE_END


//-----------------------------------------------------------------------------
//      Functions


//-----------------------------------------------------------------------------
///
///     Map
///
///     @brief      Map
///
///     @param      vec3 lfIndex 
///     @return     
///
//-----------------------------------------------------------------------------
float 
Map( 
    vec3  lViewPosition,
    vec3  lPositionVec3,
    vec4  lPlanetPositionVec4,
    float lfTime,
    float lfCloudRatio,
    float lfCloudHeight,
    float lfCloudDepth,
    SAMPLER2DARG( lNoiseMap )    
    )
{
    vec3  lViewPositionCopy = lViewPosition;
    vec3  lPositionCopy     = lPositionVec3;
    float lfResult = 1.0;
    
    
	lfResult = FractBrownianMotion4( ( lPositionCopy - lPlanetPositionVec4.xyz ) * 0.01 + ( float2vec3( 1.0 ) * 0.025 * lfTime ), SAMPLER2DPARAM( lNoiseMap ) );

    lfResult = clamp( lfResult, 0.0, 1.0 );
    
    {
        float kfValueA = 1.0 / ( kfCloudSharpness - lfCloudRatio ); 
        float kfValueB = -lfCloudRatio / ( kfCloudSharpness - lfCloudRatio );

        lfResult = clamp( ( ( lfResult * kfValueA ) + kfValueB ), 0.0, 1.0 );
    }

    lfResult *= 3.0;
    lfResult = clamp( lfResult, 0.0, 1.0 );
 
    {
        float lfYPlane;

        
        if ( GetHeight( lViewPositionCopy, lPlanetPositionVec4 ) > lfCloudHeight + lfCloudDepth )
        {
            vec3   lToViewPositionVec3 = lPositionVec3 - lViewPositionCopy;
            //lToViewPositionVec3.y = 0.0;
            lfCloudHeight = lfCloudHeight - length( lToViewPositionVec3 ) * 0.2;
        }
        

        lfYPlane = lfCloudHeight - GetHeight( lPositionCopy, lPlanetPositionVec4 );// lPositionVec3.y;
        lfYPlane = lfYPlane / lfCloudDepth;
        lfYPlane = abs( lfYPlane );
        lfYPlane *= lfYPlane;

        lfResult -= lfYPlane;
    }
    
    lfResult = clamp( lfResult, 0.0, 1.0 );

    return lfResult;
}

//-----------------------------------------------------------------------------
///
///     Raymarch
///
///     @brief      Raymarch
///
///     @param      vec3 lfIndex 
///     @return     
///
//-----------------------------------------------------------------------------
vec4 
Raymarch( 
    in vec3 lLightDirectionVec3,
    in vec3 lViewPosition,
    in vec3 lROVec3, 
    in vec3 lRDVec3,
    in vec4 lPlanetPosition,
    int     liRays,
    float lfCloudRatio,
    float lfCloudHeight,
    float lfCloudDepth,
    float lfTime,
    SAMPLER2DARG( lNoiseMap ),
    out float lfDepth )
{
    vec4 lSumVec4 = float2vec4( 0.0 );

    float lfSum   = 0.0;
    for( int liIndex = 0; liIndex < liRays; liIndex++ )
    {
        float lfDensity;
        vec4   lColourVec4;
        vec3   lPositionVec3;

        lPositionVec3 = lROVec3;
        lROVec3 += (10.0 * lRDVec3);
        lfDensity     = Map( lViewPosition, lPositionVec3, lPlanetPosition, lfTime, lfCloudRatio, lfCloudHeight, lfCloudDepth, SAMPLER2DPARAM( lNoiseMap ) );
        if (lfDensity != 0.0)
        {
            lColourVec4.xyz = mix( float2vec3( 0.0 ), float2vec3( 1.0 ), lfDensity );
            lColourVec4.w   = lfDensity;

            float lfDifference = clamp( ( lColourVec4.w - Map( lViewPosition, lPositionVec3 + ( 30.0 * lLightDirectionVec3 ), lPlanetPosition, lfTime, lfCloudRatio, lfCloudHeight, lfCloudDepth, SAMPLER2DPARAM( lNoiseMap ) ) ) / 0.6, 0.0, 1.0 );

            vec3 lBRDFVec3 = kBRDF1Vec3 * 1.35 + 0.45 * kBRDF2Vec3 * lfDifference;
            lColourVec4.xyz *= lBRDFVec3;
            lColourVec4.a   *= 0.35;
            lColourVec4.rgb *= lColourVec4.a;

            lfSum += lfDensity;
            lSumVec4 = lSumVec4 + lColourVec4 * ( 1.0 - lSumVec4.a );
            if( lfSum > 2.0 && lfDepth == 0.0 )
            {
                lfDepth = float(liIndex+1) * 10.0 ;
            }            
        }

    }

    lSumVec4.xyz /= ( 0.001 + lSumVec4.w );

    return clamp( lSumVec4, 0.0, 1.0 );
}

#ifdef D_CLOUD_RENDER

#if defined( D_PLATFORM_ORBIS ) 
#pragma argument(maxvgprcount=36)
#endif

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

FRAGMENT_MAIN_COLOUR_DEPTH_SRT
{    

    vec3   lRayStartVec3;
    vec3   lRayDirectionVec3;
    vec2   lPixelCoordsVec2 = SCREENSPACE_AS_RENDERTARGET_UVS(TEX_COORDS);
    float  lfDepth          = 0.0;

    vec4 lCloudRatioVec4 = lUniforms.mpCustomPerMesh->gCloudRatioVec4;
    float lfCloudRatio   = 1.0 - lCloudRatioVec4.x;
    float lfCloudHeight  = lCloudRatioVec4.z;
    float lfCloudDepth	 = lCloudRatioVec4.w - lCloudRatioVec4.z;
    lfCloudHeight += lfCloudDepth / 2.0;

    lRayStartVec3 = lUniforms.mpPerFrame->gViewPositionVec3;

    mat4 lCameraMat4;
    
    lCameraMat4    = PLATFORM_TRANSPOSE( lUniforms.mpPerFrame->gCameraMat4 ); 	

    lRayDirectionVec3 = float2vec3( 0.0 );
#if defined(D_CLOUD_RENDER_REFLECTIONS)    
    lRayDirectionVec3 += normalize(lCameraMat4[0].xyz) * ((lPixelCoordsVec2.x * 2.0) - 1.0);
    lRayDirectionVec3 += normalize(lCameraMat4[1].xyz) * ((lPixelCoordsVec2.y * 2.0) - 1.0);
    lRayDirectionVec3 += normalize(lCameraMat4[2].xyz);
#else
    lCameraMat4[2] = -lCameraMat4[2];

    float lAspect = lUniforms.mpPerFrame->gFrameBufferSizeVec4.x * lUniforms.mpPerFrame->gFrameBufferSizeVec4.w;

    lRayDirectionVec3 += lCameraMat4[ 0 ].xyz * ( ( ( lPixelCoordsVec2.x * 2.0 ) -1.0 ) * lAspect );
    lRayDirectionVec3 += lCameraMat4[ 1 ].xyz * (   ( lPixelCoordsVec2.y * 2.0 ) -1.0 );
    lRayDirectionVec3 += lCameraMat4[ 2 ].xyz * lUniforms.mpPerFrame->gFoVValuesVec4.w;    
#endif
    lRayDirectionVec3 = normalize ( lRayDirectionVec3 );


    float lfHeight = GetHeight( lUniforms.mpPerFrame->gViewPositionVec3,lUniforms.mpCommonPerMesh->gPlanetPositionVec4 );
    float lfPlaneDistance = 0.0;
    
    {

        if( lfHeight < lfCloudHeight - lfCloudDepth )
        {
            // if below clouds
            vec3 lUpNormalVec3          = GetWorldUp( lUniforms.mpPerFrame->gViewPositionVec3, lUniforms.mpCommonPerMesh->gPlanetPositionVec4 );
            float lfCosTheta            = dot( lRayDirectionVec3, lUpNormalVec3 );
            float lfCloudsAboveViewDist = ( lfCloudHeight - lfCloudDepth ) - lfHeight;

            lfPlaneDistance = lfCloudsAboveViewDist / lfCosTheta; // Adjacent over CosTheta
            
        }
        else
        if ( lfHeight > lfCloudHeight + lfCloudDepth &&
             lfHeight < lfCloudHeight + ( lfCloudDepth * 8.0 ) )
        {
            vec3 lUpNormalVec3          = GetWorldUp( lUniforms.mpPerFrame->gViewPositionVec3, lUniforms.mpCommonPerMesh->gPlanetPositionVec4 );
            float lfCosTheta            = dot( lRayDirectionVec3, lUpNormalVec3 );
            float lfCloudsAboveViewDist = ( lfCloudHeight + lfCloudDepth ) - lfHeight;

            lfPlaneDistance = lfCloudsAboveViewDist / lfCosTheta; // Adjacent over CosTheta
        }
        
        vec4 lFragmentColourVec4 = float2vec4(0.0);
        float lfCloudFade = 1.0 - clamp((lfPlaneDistance - 800.0) / 300, 0.0, 1.0);
        if ( lfCloudFade != 0.0 )
        {
            if (lfPlaneDistance > 0.0 &&
                lfPlaneDistance < 1500.0)
            {
                lRayStartVec3 = lRayStartVec3 + (lRayDirectionVec3 * lfPlaneDistance);
            }

            lFragmentColourVec4 = Raymarch( 
                                            -lUniforms.mpPerFrame->gLightDirectionVec4.rgb, 
                                            lUniforms.mpPerFrame->gViewPositionVec3, 
                                            lRayStartVec3, 
                                            lRayDirectionVec3, 
                                            lUniforms.mpCommonPerMesh->gPlanetPositionVec4,
                                            CLOUD_RENDER_NUM_RAYS, 
                                            lfCloudRatio, 
                                            lfCloudHeight, 
                                            lfCloudDepth, 
                                            lUniforms.mpPerFrame->gfTime,
                                            SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh, gNoiseMap ),
                                            lfDepth );

        
            lFragmentColourVec4.a = pow( lFragmentColourVec4.a, lUniforms.mpCustomPerMesh->gCloudParamsVec4.x );
            lFragmentColourVec4.a *= lfCloudFade;
            lFragmentColourVec4.xyz = mix( /*GammaCorrectInput*/(lUniforms.mpCustomPerMesh->gCloudColourExternalVec4.rgb), /*GammaCorrectInput*/(lUniforms.mpCustomPerMesh->gCloudColourInternalVec4.rgb), saturate(lFragmentColourVec4.r));
        }
#if defined(D_CLOUD_RENDER_REFLECTIONS)        
        if (lFragmentColourVec4.a == 0.0)
        {
            discard;
        }
#endif

        FRAGMENT_COLOUR = lFragmentColourVec4;
        
        lfDepth = max( lfPlaneDistance, 20.0 );
        float lfLinearDepth = FastNormaliseDepth(lUniforms.mpPerFrame->gClipPlanesVec4, lfDepth);
#if defined( D_CLOUD_RENDER_REFLECTIONS )       
        FRAGMENT_DEPTH = 1.0 - lfLinearDepth;
#else
        FRAGMENT_DEPTH = lfLinearDepth;
#endif

    }

}
#endif

#ifdef D_CLOUD_RENDER2D
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

#ifdef D_PLATFORM_ORBIS
#pragma argument(maxvgprcount=28)
#endif


FRAGMENT_MAIN_COLOUR_SRT
{
    vec3   lRayStartVec3;
    vec3   lRayDirectionVec3;
    vec2   lPixelCoordsVec2 = TEX_COORDS;
    float  lfFOVLength      = 1.7;

    vec3  lColour  = float2vec3( 0.0 );
    float lfShadow = 0.0f;
    float lfCloudHeight = lUniforms.mpCustomPerMesh->gCloudRatioVec4.z;
    float lfCloudDepth  = lUniforms.mpCustomPerMesh->gCloudRatioVec4.w - lUniforms.mpCustomPerMesh->gCloudRatioVec4.z;

#ifdef D_COMPUTE

    // can't move this stuff to the vert shader if we are compute
    float lfCameraHeight = length( lUniforms.mpPerFrame->gViewPositionVec3 - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.xyz ) - lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w;
    lfCameraHeight      = ( lfCameraHeight - 800.0 ) / 200.0;
    lfCameraHeight      = 1.0 - clamp( lfCameraHeight, 0.0, 1.0f );    

    // point where the light ray meets the cloud plane
    vec3 lUp     = GetWorldUp( lUniforms.mpPerFrame->gViewPositionVec3, lUniforms.mpCommonPerMesh->gPlanetPositionVec4 );
    vec3 lCross1 = normalize( cross( lUp, vec3( 0.0, 0.0, 1.0 ) ) );
    vec3 lCross2 = normalize( cross( lUp, lCross1 ) );
    lCross1      = normalize( cross( lUp, lCross2 ) );

#else

    // point where the light ray meets the cloud plane
    vec3 lUp     = IN( mUpVec3_mfCameraHeight ).xyz;
    vec3 lCross1 = IN( mCross1Vec3 ).xyz;
    vec3 lCross2 = IN( mCross2Vec3 ).xyz;
    float lfCameraHeight = IN( mUpVec3_mfCameraHeight ).w;
    
#endif

    lPixelCoordsVec2 = lPixelCoordsVec2 * 2.0 - float2vec2( 1.0 );
    vec3 lPosition   = lCross1 * lPixelCoordsVec2.x + lCross2 * lPixelCoordsVec2.y;
    lPosition       /= 0.005;
    lPosition       += lUp * ( lUniforms.mpCommonPerMesh->gPlanetPositionVec4.w + lfCloudHeight + lfCloudDepth * 0.5 );

    if (lfCameraHeight != 0.0)
    {
        float lfHue        = FractBrownianMotion4( lPosition * lUniforms.mpCustomPerMesh->gHueOverlayParamsVec4.y         + vec3( 100.0, 100.0, 100.0 ), SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gNoiseMap ) );
        float lfSaturation = FractBrownianMotion4( lPosition * lUniforms.mpCustomPerMesh->gSaturationOverlayParamsVec4.y  + vec3( 200.0, 200.0, 200.0 ), SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gNoiseMap ) );
        float lfValue      = FractBrownianMotion4( lPosition * lUniforms.mpCustomPerMesh->gValueOverlayParamsVec4.y       + vec3( 300.0, 300.0, 300.0 ), SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gNoiseMap ) );

        lfHue         = ( lfHue        - lUniforms.mpCustomPerMesh->gHueOverlayParamsVec4.x )        / ( 1.0 - lUniforms.mpCustomPerMesh->gHueOverlayParamsVec4.x );
        lfSaturation  = ( lfSaturation - lUniforms.mpCustomPerMesh->gSaturationOverlayParamsVec4.x ) / ( 1.0 - lUniforms.mpCustomPerMesh->gSaturationOverlayParamsVec4.x );
        lfValue       = ( lfValue      - lUniforms.mpCustomPerMesh->gValueOverlayParamsVec4.x )      / ( 1.0 - lUniforms.mpCustomPerMesh->gValueOverlayParamsVec4.x );

        lfHue         = clamp( lfHue,        0.0, 1.0 );
        lfSaturation  = clamp( lfSaturation, 0.0, 1.0 );
        lfValue       = clamp( lfValue,      0.0, 1.0 );
        
        lfHue         *= lUniforms.mpCustomPerMesh->gHueOverlayParamsVec4.z;
        lfSaturation  *= lUniforms.mpCustomPerMesh->gSaturationOverlayParamsVec4.z;
        lfValue       *= lUniforms.mpCustomPerMesh->gValueOverlayParamsVec4.z;

        lColour.r = lfHue        * lfCameraHeight;
        lColour.g = lfSaturation * lfCameraHeight;
        lColour.b = lfValue      * lfCameraHeight; 

        float lfDist = max( abs( lPixelCoordsVec2.x ), abs( lPixelCoordsVec2.y ) );
        lfDist       = 1.0 - saturate( ( lfDist - 0.9 ) / 0.1 );
        lColour     *= lfDist;
    }

    /*{
        float lfCloudRatio = 1.0 - lUniforms.mpCustomPerMesh->gCloudRatioVec4.x;
        float kfValueA = 1.0 / ( kfCloudSharpness - lfCloudRatio );
        float kfValueB = -lfCloudRatio / ( kfCloudSharpness - lfCloudRatio );

        lfShadow = clamp( FractBrownianMotion4( lPosition * 0.01 + ( vec3( 1.0, 1.0, 1.0 ) * 0.025 * lUniforms.mpPerFrame->gfTime ), SAMPLER2DPARAM( lUniforms.mpCustomPerMesh->gNoiseMap )  ), 0.0, 1.0 );
        lfShadow = clamp( ( ( lfShadow * kfValueA ) + kfValueB ) * 3.0, 0.0, 1.0 );

        vec3 lColour = vec3( lPosition.x > 0.0 ? 1.0 : 0.0, lPosition.y > 0.0 ? 1.0 : 0.0, lPosition.z > 0.0 ? 1.0 : 0.0 );

    }*/

    //FRAGMENT_COLOUR = saturate( vec4( lColour, 1.0 ) );    
    FRAGMENT_COLOUR = saturate(vec4(1.0, 1.0, 1.0, 1.0));
}
#endif

#endif

// =================================================================================================

#ifdef D_CLOUD_COPY

//-----------------------------------------------------------------------------
//      Global Data



//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2,     TEXCOORD0 )
DECLARE_END


//-----------------------------------------------------------------------------
//      Functions

FRAGMENT_MAIN_COLOUR_SRT
{
    vec4  lFragmentColourVec4 = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ),     IN( mTexCoordsVec2 ) );
    
    if (lFragmentColourVec4.a == 0.0)
    {
        discard;
    }

    float lfDepth             = DecodeDepthFromColour( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gDepthMap ),      IN( mTexCoordsVec2 ) ) );
    float lfCloudDepth        = DecodeDepthFromColour( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gCloudDepthMap ), IN( mTexCoordsVec2 ) ) );


    if (lfDepth < lfCloudDepth)
    {
        discard;
    }

    if (lFragmentColourVec4.a > 0.0)
    {
        lFragmentColourVec4.rgb /= lFragmentColourVec4.a;
    }
    
    FRAGMENT_COLOUR = lFragmentColourVec4;

    
    /*
    if (IN(mTexCoordsVec2).y < 0.5)
    {
        if (lfDepth < lfCloudDepth)
        {
            discard;
        }

        FRAGMENT_COLOUR = lFragmentColourVec4;
    }
    else
    {
        FRAGMENT_COLOUR = IN(mTexCoordsVec2).x < 0.5 ? vec4(lfDepth, 0.0, 0.0, 1.0) : vec4(lfCloudDepth, 0.0, 0.0, 1.0);
        //FRAGMENT_COLOUR = vec4( abs(lfDepth - lfCloudDepth) * 1000.0, 0.0, 0.0, 1.0 );
    }
    */

}

#endif
