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

#include "Common/Defines.shader.h"
#include "Common/CommonUniforms.shader.h"
#include "Fullscreen/PostCommon.h"
#include "Common/Common.shader.h"
#include "Common/CommonPostProcess.shader.h"
#include "Common/CommonDepth.shader.h"


// =================================================================================================

#if defined( D_GUASS_BLUR_RADIUS_4 )
    #define D_GUASS_BLUR_RADIUS 4
#elif defined( D_GUASS_BLUR_RADIUS_3 )
    #define D_GUASS_BLUR_RADIUS 3
#elif defined( D_GUASS_BLUR_RADIUS_2 )
    #define D_GUASS_BLUR_RADIUS 2
#endif


// =================================================================================================
//
// PARTICLE_BLEND
//
// =================================================================================================

#ifdef D_POSTPROCESS_PARTICLE_BLEND

//-----------------------------------------------------------------------------
//      Global Data

//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION
    
    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END


//-----------------------------------------------------------------------------
//      Functions 

FRAGMENT_MAIN_COLOUR_SRT
{
    //vec4 lFragmentTexVec4;
    vec4 lParticleTexVec4;

    //lFragmentTexVec4 = texture2D( gBufferMap, IN(mTexCoordsVec2)  );  
    lParticleTexVec4 = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer1Map ), TEX_COORDS.xy, 0.0);

    //FRAGMENT_COLOUR = vec4( (lFragmentTexVec4.rgb * lParticleTexVec4.a) + lParticleTexVec4.rgb, lFragmentTexVec4.a ); 
    
    float lfAlpha =  1.0 - lParticleTexVec4.a;
    vec4 lOutColour;
    if (lfAlpha != 0.0)
    {
        #ifdef D_BLEND_POST_TAA
            lOutColour = vec4( lParticleTexVec4.rgb, lfAlpha );
        #else
            lOutColour = vec4( (lParticleTexVec4.rgb / lfAlpha) , lfAlpha );
        #endif
    } else
    {
        lOutColour = vec4( 0.0, 0.0, 0.0, 0.0 );
    }

    #ifdef D_BLEND_POST_TAA
    lOutColour.a = 1.0 - lOutColour.a;
    vec4 lPostTaaTexVec4 = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), TEX_COORDS.xy, 0.0);
    lOutColour.rgb = lPostTaaTexVec4.rgb + lOutColour.rgb * lPostTaaTexVec4.a;
    lOutColour.a *= lPostTaaTexVec4.a;
    #endif

    #ifdef D_COMPUTE
    // manual blending in compute-land
    FRAGMENT_COLOUR = lOutColour + FRAGMENT_COLOUR * lOutColour.a;
    #else
    FRAGMENT_COLOUR = lOutColour;
    #endif
}

#endif

// =================================================================================================
//
// PARTICLE CLEAR
//
// =================================================================================================

#ifdef D_POSTPROCESS_PARTICLE_CLEAR

//-----------------------------------------------------------------------------
//      Global Data



//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
INPUT_SCREEN_POSITION

INPUT(vec2, mTexCoordsVec2, TEXCOORD0)
DECLARE_END

//-----------------------------------------------------------------------------
//      Functions 

FRAGMENT_MAIN_COLOUR_SRT
{
    FRAGMENT_COLOUR = vec4(0.0, 0.0, 0.0, 1.0);
}

#endif


// =================================================================================================
//
// COPY
//
// =================================================================================================

#ifdef D_POSTPROCESS_COPY

//-----------------------------------------------------------------------------
//      Global Data



//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END


//-----------------------------------------------------------------------------
//      Functions 

FRAGMENT_MAIN_COLOUR_SRT
{			
    //FRAGMENT_COLOUR = vec4( texture2D( lUniforms.mpCustomPerMesh->gBufferMap, IN(mTexCoordsVec2) ).xyz, 1.0 );	
#ifdef D_POSTPROCESS_COPYRGBONLY
    FRAGMENT_COLOUR = vec4( texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), TEX_COORDS, 0.0).rgb,1.0) ;
#else
    #ifdef D_POSTPROCESS_COPY_ADD
    FRAGMENT_COLOUR += texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), TEX_COORDS, 0.0);
    #elif defined( D_POSTPROCESS_COPY_MULADD )
    vec4 lReadColour = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), TEX_COORDS, 0.0);
    FRAGMENT_COLOUR = FRAGMENT_COLOUR * lReadColour.a + lReadColour;
    #else
        //vec4 lReadColour = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), TEX_COORDS, 0.0 );
        //lReadColour.rgb = DebandFilter( DEREF_PTR( lUniforms.mpPerFrame ), lReadColour.rgb, IN( mTexCoordsVec2 ), SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh, gBuffer1Map ), SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh, gBufferMap ) );
        //FRAGMENT_COLOUR = lReadColour;

    FRAGMENT_COLOUR = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), TEX_COORDS, 0.0);
    #endif
#endif
}

#endif

// =================================================================================================
//
// COPY
//
// =================================================================================================

#ifdef D_POSTPROCESS_STAA_COPY

//-----------------------------------------------------------------------------
//      Global Data



//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END


//-----------------------------------------------------------------------------
//      Functions 

FRAGMENT_MAIN_COLOUR_SRT
{           
    //FRAGMENT_COLOUR = vec4( texture2D( lUniforms.mpCustomPerMesh->gBufferMap, IN(mTexCoordsVec2) ).xyz, 1.0 );    
    FRAGMENT_COLOUR0 = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ),  IN(mTexCoordsVec2) );   
    FRAGMENT_COLOUR1 = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBuffer1Map ), IN(mTexCoordsVec2) );   
}

#endif

// =================================================================================================
//
// COPY
//
// =================================================================================================

#ifdef D_POSTPROCESS_COPY_YCGCO_TO_RGB

//-----------------------------------------------------------------------------
//      Global Data



//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END


//-----------------------------------------------------------------------------
//      Functions 

FRAGMENT_MAIN_COLOUR_SRT
{   
    vec2 tc = SCREENSPACE_AS_RENDERTARGET_UVS(IN(mTexCoordsVec2));

    vec3 colIn = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), tc ).rgb;  

    vec4 colOut = vec4(1.0);

    colOut.rgb = YCgCoToRGB( colIn );
    colOut.rgb = saturate( TonemapKodak( colOut.rgb ) / TonemapKodak( float2vec3(1.0) ) );
    colOut.rgb = GammaCorrectOutput( colOut.rgb  );
    FRAGMENT_COLOUR = colOut;
}

#endif

// =================================================================================================
//
// DEJITTER
//
// =================================================================================================

#ifdef D_POSTPROCESS_DEJITTER

//-----------------------------------------------------------------------------
//      Global Data



//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END


//-----------------------------------------------------------------------------
//      Functions 

FRAGMENT_MAIN_COLOUR_SRT
{           
    //FRAGMENT_COLOUR = vec4( texture2D( lUniforms.mpCustomPerMesh->gBufferMap, IN(mTexCoordsVec2) ).xyz, 1.0 );    
    vec2 lDeJitterVec = DSCREENSPACE_AS_RENDERTARGET_UVS(lUniforms.mpPerFrame->gDeJitterVec4.xy);

    vec2 ldejit = IN(mTexCoordsVec2) + lDeJitterVec;

    FRAGMENT_COLOUR = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), ldejit.xy );   
}

#endif


// =================================================================================================
//
// CONVERTHDR
//
// =================================================================================================

#ifdef D_POSTPROCESS_CONVERTHDR

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
INPUT_SCREEN_POSITION

INPUT(vec2, mTexCoordsVec2, TEXCOORD0)
DECLARE_END


//-----------------------------------------------------------------------------
//      Functions 

FRAGMENT_MAIN_COLOUR_SRT
{      
    vec4 lFragCol = texture2DLod(SAMPLER_GETMAP(lUniforms.mpCustomPerMesh, gBufferMap), TEX_COORDS.xy, 0.0);
    lFragCol.xyz = pow(lFragCol.xyz, 2.6);
    lFragCol.xyz *= 3.35;   // exposure
    lFragCol.xyz = mul(BT709_TO_BT2020, lFragCol.xyz);
    lFragCol.w = 1.0;
    lFragCol.xyz = PQ_OETF_Fast(lFragCol.xyz);
    FRAGMENT_COLOUR = vec4(lFragCol.xyz, 1.0);
}

#endif


// =================================================================================================
//
// COPY_DEPTH
//
// =================================================================================================

#ifdef D_POSTPROCESS_COPY_DEPTH


//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END


//-----------------------------------------------------------------------------
//      Functions 

VOID_MAIN_DEPTH_SRT
{
    FRAGMENT_DEPTH =  texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), TEX_COORDS ).x;
}

#endif


// =================================================================================================
//
// DEPTH_REVERSE_TO_LINEAR
//
// =================================================================================================

#ifdef D_POSTPROCESS_REVERSE_TO_LINEAR

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END


//-----------------------------------------------------------------------------
//      Functions 

#ifdef D_PLATFORM_ORBIS
#pragma PSSL_target_output_format(target 0 FMT_32_AR)
#endif

FRAGMENT_MAIN_COLOUR_SRT
{
    float lfReverseDepth = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), TEX_COORDS ).x;
    float lfLinearDepthNorm = ReverseZToLinearDepthNorm( lUniforms.mpPerFrame->gClipPlanesVec4, lfReverseDepth );
    FRAGMENT_COLOUR = EncodeDepthToColour( lfLinearDepthNorm );
}

#endif


// =================================================================================================
//
// DEPTH_REVERSE_TO_LINEAR_LESS
//
// =================================================================================================

#ifdef D_POSTPROCESS_REVERSE_TO_LINEAR_LESS

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
INPUT_SCREEN_POSITION

INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END


//-----------------------------------------------------------------------------
//      Functions 

#ifdef D_PLATFORM_ORBIS
#pragma PSSL_target_output_format(target 0 FMT_32_AR)
#endif

FRAGMENT_MAIN_COLOUR_DEPTH_SRT
{
    float lfReverseDepth  = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), TEX_COORDS ).x;
    //float lfReverseDepth2 = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer1Map ), TEX_COORDS ).x;
   // lfReverseDepth = lfReverseDepth2;//max( lfReverseDepth, lfReverseDepth2 );

    float lfLinearDepthNorm = ReverseZToLinearDepthNorm( lUniforms.mpPerFrame->gClipPlanesVec4, lfReverseDepth );
    FRAGMENT_COLOUR = EncodeDepthToColour( lfLinearDepthNorm );
    FRAGMENT_DEPTH  = lfReverseDepth;
}

#endif


// =================================================================================================
//
// DOWNSAMPLE_GBUFFERDEPTH
//
// =================================================================================================

#ifdef D_POSTPROCESS_DOWNSAMPLE_GBUFFERDEPTH

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
INPUT_SCREEN_POSITION

INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END


//-----------------------------------------------------------------------------
//      Functions 

#ifdef D_PLATFORM_ORBIS
#pragma PSSL_target_output_format(target 0 FMT_32_AR)
#endif

FRAGMENT_MAIN_COLOUR_SRT
{
    vec2 lFlooredTexCoordsVec2 = TEX_COORDS.xy;
    FRAGMENT_COLOUR = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), lFlooredTexCoordsVec2, 0.0);
}

#endif


// =================================================================================================
//
// DOWNSAMPLE_DEPTH 
//
// =================================================================================================

#ifdef D_POSTPROCESS_DOWNSAMPLE_DEPTH

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
INPUT_SCREEN_POSITION

INPUT(vec2, mTexCoordsVec2, TEXCOORD0)
DECLARE_END


//-----------------------------------------------------------------------------
//      Functions 

#ifdef D_PLATFORM_ORBIS
#pragma PSSL_target_output_format(target 0 FMT_32_AR)
#endif

FRAGMENT_MAIN_COLOUR_DEPTH_SRT
{
    vec2 lTexCoordsVec2 = GetDejitteredTexCoord(IN(mTexCoordsVec2).xy, lUniforms.mpPerFrame->gDeJitterVec4);

    float lfDepth = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), lTexCoordsVec2).x;
    FRAGMENT_COLOUR = vec4(ReverseZToLinearDepth(lUniforms.mpPerFrame->gClipPlanesVec4, lfDepth), 0.0, 0.0, 0.0);
    FRAGMENT_DEPTH = lfDepth;
}


#endif




// =================================================================================================
//
// BRIGHTPASS
//
// =================================================================================================

#ifdef D_POSTPROCESS_BRIGHTPASS

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END


//-----------------------------------------------------------------------------
//      Functions 

vec3 
Threshold(
    in vec3 lColour,
    in float lfThreshold,      
    in float lfGain )
{
    vec3 lumcoeff = vec3(0.299,0.587,0.114);
    //vec3 lumcoeff = normalize(vec3(1.0,1.0,1.0));

    float lum = dot(lColour.rgb, lumcoeff);

    float thresh = max((lum-lfThreshold)*lfGain, 0.0);
    return mix( vec3(0.0, 0.0, 0.0), lColour, thresh );
    //return vec3( lum );
}

FRAGMENT_MAIN_COLOUR_SRT
{
    vec3 lBrightColourVec3;
    lBrightColourVec3 = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), TEX_COORDS, 0.0).xyz;

    #ifdef D_COMPUTE
    // for async, the glow is stored already in the bloom buffer
    float lfGlowAlpha = FRAGMENT_COLOUR.r;
    #else
     float lfGlowAlpha = 1.0 - texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBuffer1Map ), TEX_COORDS, 0.0).a;
    #endif

    lBrightColourVec3.xyz = TonemapKodak(lBrightColourVec3.xyz) / TonemapKodak( vec3(1.0,1.0,1.0) );

    lBrightColourVec3 = GammaCorrectOutput( lBrightColourVec3 );

    lBrightColourVec3 = Threshold(  lBrightColourVec3, 
                                    min( lUniforms.mpCustomPerMesh->gHDRParamsVec4.y, lfGlowAlpha),  // Threshold
                                    lUniforms.mpCustomPerMesh->gHDRParamsVec4.z );// Offset

    lBrightColourVec3 = clamp( lBrightColourVec3, 0.0, 1.0 );
    FRAGMENT_COLOUR = vec4( lBrightColourVec3, 1.0 );
    //FRAGMENT_COLOUR = vec4( 0.0, 0.0, 1.0, 1.0 );
}


#endif




// =================================================================================================
//
// BRIGHTPASS_COPY
//
// =================================================================================================

#ifdef D_POSTPROCESS_BRIGHTPASS_COPY

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END


//-----------------------------------------------------------------------------
//      Functions 

FRAGMENT_MAIN_COLOUR_SRT
{
    float lfGlowAlpha = 1.0 - texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), TEX_COORDS ).a;

    FRAGMENT_COLOUR = vec4( vec3( lfGlowAlpha,lfGlowAlpha,lfGlowAlpha ), 1.0 );
}


#endif

// =================================================================================================
//
// BRIGHTPASS
//
// =================================================================================================

#ifdef D_POSTPROCESS_DOWNSAMPLE

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END


//-----------------------------------------------------------------------------
//      Functions 

FRAGMENT_MAIN_COLOUR01_SRT
{
    vec2 coord2 = TEX_COORDS; // +vec2(2.0, 2.0) * lUniforms.mpPerFrame->gFrameBufferSizeVec4.zw;
    vec3 lTextureColourVec3;

    lTextureColourVec3.rgb = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), coord2, 0.0).rgb;

    float lfPower = GetDofPower( coord2,
        SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMesh,gBuffer1Map),
        SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMesh,gBlurMask),
        lUniforms.mpCustomPerMesh->gDoFParamsVec4,
        lUniforms.mpPerFrame->gClipPlanesVec4);

    FRAGMENT_COLOUR0 = vec4( lTextureColourVec3, 1.0 );
    FRAGMENT_COLOUR1 = vec4( lfPower, 0.0, 0.0,  1.0 );
}


#endif


// =================================================================================================
//
// BLUR
//
// =================================================================================================

#ifdef D_POSTPROCESS_GUASS

//-----------------------------------------------------------------------------
//      Global Data

//blur params x = bool x-axis, y = bool y-axis, z = Sigma
// The sigma value for the gaussian function: higher value means more blur
// A good value for 9x9 is around 3 to 5
// A good value for 7x7 is around 2.5 to 4
// A good value for 5x5 is around 2 to 3.5
// ... play around with this based on what you need :)



//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END


//-----------------------------------------------------------------------------
//      Functions 


STATIC_CONST float pi = 3.14159265;


FRAGMENT_MAIN_COLOUR_SRT
{    
    vec3   incrementalGaussian; // Incremental Gaussian Coefficent Calculation (See GPU Gems 3 pp. 877 - 889)
    vec4   avgValue = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec4   lCenter;
    float coefficientSum = 0.0f;
    float sigma;


    lCenter = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), TEX_COORDS, 0.0 );

    // The sigma value for the gaussian function: higher value means more blur
    // A good value for 9x9 is around 3 to 5
    // A good value for 7x7 is around 2.5 to 4
    // A good value for 5x5 is around 2 to 3.5
    // ... play around with this based on what you need :)
    sigma = lUniforms.mpCustomPerMesh->gBlurParamsVec4.z;

    incrementalGaussian.x = 1.0f / (sqrt(2.0f * pi) * sigma);
    incrementalGaussian.y = exp(-0.5f / (sigma * sigma));
    incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;

    // Take the central sample first...
    avgValue                = lCenter * incrementalGaussian.x;
    coefficientSum         += incrementalGaussian.x;
    incrementalGaussian.xy *= incrementalGaussian.yz;

    // Go through the remaining X directional samples (D_GUASS_BLUR_TAPS on each side of the center)
    for (float i = 1.0f; i <= D_GUASS_BLUR_RADIUS ; i++) 
    {
        vec4 lTexPos;
        vec4 lTexNeg;

        lTexPos = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), TEX_COORDS.xy + i * (lUniforms.mpCustomPerMesh->gBlurParamsVec4.xy * lUniforms.mpPerFrame->gFrameBufferSizeVec4.zw), 0.0);
        lTexNeg = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), TEX_COORDS.xy - i * (lUniforms.mpCustomPerMesh->gBlurParamsVec4.xy * lUniforms.mpPerFrame->gFrameBufferSizeVec4.zw), 0.0);

        avgValue += lTexNeg * incrementalGaussian.x;         
        avgValue += lTexPos * incrementalGaussian.x;
               
        coefficientSum         += 2 * incrementalGaussian.x;
        incrementalGaussian.xy *= incrementalGaussian.yz;
    }

    #ifdef D_GUASS_BLUR_ADD
    FRAGMENT_COLOUR += vec4( avgValue.rgb / coefficientSum, 1.0 );
    #else
    FRAGMENT_COLOUR = vec4( avgValue.rgb / coefficientSum, 1.0 );
    #endif
   
}
#endif


// =================================================================================================
//
// BLUR_KAWASE
//
// =================================================================================================

#if defined(D_POSTPROCESS_BLUR_KAWASE) || defined(D_POSTPROCESS_BLUR_KAWASE_PRESERVE_ALPHA)

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

FRAGMENT_MAIN_COLOUR_SRT
{
    vec2 lRecipTexSize = vec2(1.0 / 480.0, 1.0 / 270.0);

#ifdef D_POSTPROCESS_BLUR_KAWASE
    #ifdef D_BLUR_KAWASE_ADD
        FRAGMENT_COLOUR += vec4( BlurKawase( SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBufferMap ), TEX_COORDS, lRecipTexSize, lUniforms.mpCustomPerMesh->gBlurParamsVec4.x ).xyz, 1.0 );
    #else
        FRAGMENT_COLOUR = vec4( BlurKawase( SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBufferMap ), TEX_COORDS, lRecipTexSize, lUniforms.mpCustomPerMesh->gBlurParamsVec4.x ).xyz, 1.0 );
    #endif
#else
    FRAGMENT_COLOUR =       BlurKawase( SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBufferMap ), TEX_COORDS, lRecipTexSize, lUniforms.mpCustomPerMesh->gBlurParamsVec4.x );
#endif 
    
}

#endif



// =================================================================================================
//
// DEBUG
//
// =================================================================================================

#ifdef D_POSTPROCESS_COMBINE

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END

//-----------------------------------------------------------------------------
//      Functions 

FRAGMENT_MAIN_COLOUR01_SRT
{                
    vec4  lFragCol;
    vec2  newCoords;
	float lfLUTEffectAmount;

    newCoords = IN(mTexCoordsVec2).xy;

    lFragCol          = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap  ), newCoords);
	lfLUTEffectAmount = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer2Map ), newCoords).r;

    // Deband... but don't deband anything other than the background sky/space

    lFragCol.rgb = DebandFilter(DEREF_PTR(lUniforms.mpPerFrame), lFragCol.rgb, newCoords, SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMesh,gBuffer1Map), SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMesh,gBufferMap));

    lFragCol.rgb = clamp( GammaCorrectInput( lFragCol.rgb ), 0.0, 1.0 );
    
    lFragCol.rgb = TonemapKodak(lFragCol.rgb) / TonemapKodak( vec3(1.0,1.0,1.0) );

    lFragCol.rgb = GammaCorrectOutput( lFragCol.rgb );

    float lfDepth = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer1Map ), IN( mTexCoordsVec2 ).xy ).x;

    lFragCol.rgb = ApplyColourLUT( DEREF_PTR( lUniforms.mpPerFrame ), DEREF_PTR( lUniforms.mpCustomPerMesh ), lFragCol.rgb, lfDepth, lfLUTEffectAmount );

    vec3 lumcoeff = vec3(0.299,0.587,0.114);
    FRAGMENT_COLOUR0 = lFragCol;
	FRAGMENT_COLOUR1 = vec4(dot(lFragCol.rgb, lumcoeff), 0.0, 0.0, 1.0);
}

#endif

// =================================================================================================
//
// COMBINE_AND_COPY_DEPTH
//
// =================================================================================================

#ifdef D_POSTPROCESS_COMBINE_AND_COPY_DEPTH

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
INPUT_SCREEN_POSITION

INPUT(vec2, mTexCoordsVec2, TEXCOORD0)
DECLARE_END

//-----------------------------------------------------------------------------
//      Functions 

FRAGMENT_MAIN_COLOUR_DEPTH_SRT
{
    vec3  lFragCol;
    vec2  newCoords;
    float lfLUTEffectAmount;

    newCoords = TEX_COORDS.xy;

    lFragCol          = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), newCoords).rgb;
    lfLUTEffectAmount = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer2Map ), newCoords).r;

    // Deband... but don't deband anything other than the background sky/space

    lFragCol = DebandFilter(DEREF_PTR(lUniforms.mpPerFrame), lFragCol, newCoords, SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMesh,gBuffer1Map), SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMesh,gBufferMap));

    lFragCol = clamp( GammaCorrectInput(lFragCol), 0.0, 1.0 );

    lFragCol.xyz = TonemapKodak(lFragCol) / TonemapKodak(vec3(1.0, 1.0, 1.0));

    lFragCol = GammaCorrectOutput(lFragCol);

    float lfDepth = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer1Map ), TEX_COORDS.xy).x;

    lFragCol = ApplyColourLUT( DEREF_PTR( lUniforms.mpPerFrame ), DEREF_PTR( lUniforms.mpCustomPerMesh ), lFragCol, lfDepth, lfLUTEffectAmount );

    FRAGMENT_COLOUR = vec4(lFragCol, 1.0);
    FRAGMENT_DEPTH  = lfDepth;
}

#endif

// =================================================================================================
//
// COMBINEHDR
//
// =================================================================================================

#ifdef D_POSTPROCESS_COMBINEHDR

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
INPUT_SCREEN_POSITION

INPUT(vec2, mTexCoordsVec2, TEXCOORD0)
DECLARE_END

//-----------------------------------------------------------------------------
//      Functions 

FRAGMENT_MAIN_COLOUR_SRT
{
    vec2 newCoords = IN(mTexCoordsVec2).xy;

    vec4  lFragCol          = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap  ), newCoords);
    float lfLUTEffectAmount = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer2Map ), newCoords).r;

    lFragCol.rgb = DebandFilter(DEREF_PTR(lUniforms.mpPerFrame), lFragCol.rgb, newCoords, SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMesh, gBuffer1Map), SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMesh,gBufferMap));

    // We calculate the original constrast scale that colourcube would have produced and apply to our HDR colour
    vec3 lToneMapCol = TonemapKodak(clamp(GammaCorrectInput(lFragCol.rgb), 0.0, 1.0)) / TonemapKodak(vec3(1.0, 1.0, 1.0));
    vec3 lToneMapColG = GammaCorrectOutput(lToneMapCol);

    float lfDepth = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer1Map ), IN( mTexCoordsVec2 ).xy ).x;

    vec3 lLutScale = ApplyColourLUT( DEREF_PTR( lUniforms.mpPerFrame ), DEREF_PTR( lUniforms.mpCustomPerMesh ), lFragCol.rgb, lfDepth, lfLUTEffectAmount );

    lFragCol.rgb *= lLutScale / lToneMapColG; // *lToneMapScale;
	FRAGMENT_COLOUR = lFragCol;
}

#endif

// =================================================================================================
//
// COMBINEHDR_AND_COPY_DEPTH
//
// =================================================================================================

#ifdef D_POSTPROCESS_COMBINEHDR_AND_COPY_DEPTH

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
INPUT_SCREEN_POSITION

INPUT(vec2, mTexCoordsVec2, TEXCOORD0)
DECLARE_END

//-----------------------------------------------------------------------------
//      Functions 

FRAGMENT_MAIN_COLOUR_DEPTH_SRT
{
    vec2 newCoords = IN(mTexCoordsVec2).xy;

    vec3 lFragCol = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), newCoords).rgb;
    float lfLUTEffectAmount = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer2Map ), newCoords).r;
    lFragCol = DebandFilter(DEREF_PTR(lUniforms.mpPerFrame), lFragCol, newCoords, SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMesh,gBuffer1Map), SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMesh,gBufferMap));

    // We calculate the original constrast scale that colourcube would have produced and apply to our HDR colour
    vec3 lToneMapCol = TonemapKodak( clamp( GammaCorrectInput(lFragCol), 0.0, 1.0 ) ) / TonemapKodak(vec3(1.0, 1.0, 1.0));
    vec3 lToneMapColG = GammaCorrectOutput(lToneMapCol);

    float lfDepth = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer1Map ), IN( mTexCoordsVec2 ).xy ).x;

    vec3 lLutScale = ApplyColourLUT( DEREF_PTR( lUniforms.mpPerFrame ), DEREF_PTR( lUniforms.mpCustomPerMesh ), lFragCol, lfDepth, lfLUTEffectAmount );

    lFragCol *= lLutScale / lToneMapColG; // *lToneMapScale;

    FRAGMENT_COLOUR = vec4(lFragCol, 1.0);
    FRAGMENT_DEPTH = lfDepth;
}

#endif


// =================================================================================================
//
// ADDITION
//
// =================================================================================================


#ifdef D_POSTPROCESS_ADDITION_4

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END

//-----------------------------------------------------------------------------
//      Functions 


FRAGMENT_MAIN_COLOUR_SRT
{   
    vec3 lFragCol;
    vec2 newCoords = IN(mTexCoordsVec2).xy;

    // Combine
    lFragCol  =                  ( texture2D(SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBuffer1Map ), newCoords).xyz );
    lFragCol += GammaCorrectInput( texture2D(SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBuffer2Map ), newCoords).xyz );
    lFragCol += GammaCorrectInput( texture2D(SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBuffer3Map ), newCoords).xyz );
    lFragCol +=                  ( texture2D(SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBuffer4Map ), newCoords).xyz );
    lFragCol  = GammaCorrectOutput( lFragCol );

    FRAGMENT_COLOUR = vec4( lFragCol, 1.0 );    
}

#endif

// =================================================================================================
//
// ADDITION_2
//
// =================================================================================================

#ifdef D_POSTPROCESS_ADDITION_2

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END

//-----------------------------------------------------------------------------
//      Functions 

FRAGMENT_MAIN_COLOUR_SRT
{

    //Debug

    vec3 lFragCol;
    vec2 newCoords;

    newCoords = IN( mTexCoordsVec2 ).xy;

    lFragCol  = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer1Map ), newCoords ).xyz;
    lFragCol += texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer2Map ), newCoords ).xyz;

    FRAGMENT_COLOUR = vec4( lFragCol, 1.0 );


}

#endif

// =================================================================================================
//
// MOTIONBLUR
//
// =================================================================================================

#ifdef D_POSTPROCESS_MOTIONBLUR

//-----------------------------------------------------------------------------
//      Global Data

#if defined( D_MOTIONBLUR_SAMPLE_COUNT_ULTRA )

#define D_MOTIONBLUR_SAMPLES 24

#elif defined( D_MOTIONBLUR_SAMPLE_COUNT_HIGH ) 

#define D_MOTIONBLUR_SAMPLES 12

#elif defined( D_MOTIONBLUR_SAMPLE_COUNT_MED ) 

#define D_MOTIONBLUR_SAMPLES 8

#else

#define D_MOTIONBLUR_SAMPLES 4

#endif

//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT      
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END


#ifdef D_PLATFORM_ORBIS
// doesn't seem to result in any spills if we push to max occupancy
#pragma argument(targetoccupancy_atallcosts=100)
#endif


//-----------------------------------------------------------------------------
//      Functions 


#define SOFT_Z_EXTENT 0.1

float cone( in float lSampleDist, in float lfRcpSpeed )
{
    return saturate( 1.0 - lSampleDist * lfRcpSpeed );
}

float smoothstep_approx( float inval )
{
    // replaces the cylinder approximation
    // want a function that looks a bit like

    // 1       /----\
    //        |      |
    //        |      |
    //        |      |      
    // 0 ____/        \______

    // centered around 0

    // first apply an inverse parabola - this parabola is 1 at +/-0.95
    // and 0 at +/-1.05
    float x = saturate( 5.5125 - 5 * inval * inval );

    // then apply smoothstep
    float x2 = x*x;
    return ( 3.0 * x2 - 2.0 * (x * x2) );
}

float cylinder( in float lSampleDist, in float lfRcpSpeed )
{
    // return 1.0 - smoothstep( 0.95 * lfSpeed, 1.05 * lfSpeed, lSampleDist );

    return smoothstep_approx( lSampleDist * lfRcpSpeed ); 
}

float softDepthCompare( in float zA, in float zB )
{
    return saturate( 1.0 - ( zA - zB ) * (1.0 / SOFT_Z_EXTENT) );
}

float hardDepthCompare( in float zA, in float zB )
{
    return zA < zB? 1.0 : 0.0;
}


vec3 SimpleReinhard( in vec3 col )
{
    vec3 lHalfCol = col / ( vec3(1.0, 1.0, 1.0) + max( vec3(0.0, 0.0, 0.0), col ) );
    return lHalfCol * 2.0;
}

vec3 SimpleUnReinhard( in vec3 col )
{
    vec3 lHalfCol = col * 0.5;
    return lHalfCol / ( vec3(1.0, 1.0, 1.0) - min( vec3(0.9999, 0.9999, 0.9999), lHalfCol ) );
}

vec3 SimpleHalfReinhard( in vec3 col )
{
    #ifdef D_MBLUR_REINHARD_APPROX
    vec3 lHalfCol = col / ( vec3(1.0, 1.0, 1.0) + col );
    return lHalfCol / 0.75;
    #else
    return col;
    #endif
}

vec3 SimpleUnHalfReinhard( in vec3 col )
{
    #ifdef D_MBLUR_REINHARD_APPROX
    vec3 lHalfCol = col * 0.75;
    return lHalfCol / ( vec3(1.0) - lHalfCol );
    #else
    return col;
    #endif
}

float Bayer(
    uvec2 lPos )
{
    const mat4 bayer = mat4( 
        vec4(1, 9, 3, 11),
        vec4(13, 5, 15, 7),
        vec4(4, 12, 2, 10),
        vec4(16, 8, 14, 6)
    ) / 17.0;

    //vec2 positionMod = vec2( uvec2( tc * lUniforms.mpPerFrame->gFrameBufferSizeVec4.xy ) & 3 );

    uvec2 positionMod = uvec2( lPos & 3 );

    float rndoffset = bayer[positionMod.x][positionMod.y];

    #ifdef D_PASS_2
    rndoffset = 1.0 - rndoffset;
    #endif

    return rndoffset;
}

FRAGMENT_MAIN_COLOUR_SRT
{           
    vec2 tc = TEX_COORDS;

    float lfBaseSampleWeight = 1.0;

    float wacc = 0.0;
    vec4  acc  = vec4(0.0, 0.0, 0.0, 0.0);

    vec4 lTileMinMax = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer1Map ), tc, 0.0);
    lTileMinMax.xy = DecodeMotion(lTileMinMax.xy) * lUniforms.mpPerFrame->gMBlurSettingsVec4.z;  
    lTileMinMax.zw = DecodeMotion(lTileMinMax.zw) * lUniforms.mpPerFrame->gMBlurSettingsVec4.z;  
    
    vec2  lDelta = lTileMinMax.xy;

    vec2 lDeltaInPixels = lDelta * lUniforms.mpPerFrame->gFrameBufferSizeVec4.xy;
    float lDistInPixels = length( lDeltaInPixels );
    float lWantedDistInPixels = max( 0.0, lDistInPixels - abs( lUniforms.mpPerFrame->gTaaSettingsVec4.x ) );


    float lNumSamples = min( D_MOTIONBLUR_SAMPLES, ( lWantedDistInPixels * 1.5 ) );

    #if defined( D_MBLUR_DISCARD_IF_DOING_NOTHING ) && !defined D_COMPUTE
    if( lNumSamples < 0.1 )
    {
        discard;
    }
    #endif

    #ifdef D_MBLUR_DOWNRES_MODE
    vec4 lBaseSample = vec4( 0.0, 0.0, 0.0, 0.0 );    
    #else
    vec4 lBaseSample = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), tc, 0.0 );
    #endif
    vec4 lOutColor = lBaseSample;

    if( lNumSamples >= 2.0 )
    {
        lBaseSample.xyz = SimpleUnHalfReinhard( lBaseSample.xyz );

        #ifndef D_MOTIONBLUR_SAMPLE_COUNT_ULTRA
        lNumSamples = D_MOTIONBLUR_SAMPLES;
        #endif

        vec2 lMaxMinDifference = ( lTileMinMax.xy - lTileMinMax.zw ) * lUniforms.mpPerFrame->gFrameBufferSizeVec4.xy;


        #ifdef D_MOTIONBLUR_JITTER

        #ifndef D_PLATFORM_OPENGL

            float rndoffset = Bayer( uvec2( tc * lUniforms.mpPerFrame->gFrameBufferSizeVec4.xy ) ) - 0.5;

        #else

            //float rndoffset = 0.0;
            #ifdef D_PASS_2
            float rndoffset = fract( sin( dot( tc.xy, vec2( 12.9898, 78.233 ) ) )* 43758.5453 ) - 0.5;
            #else
            float rndoffset = fract( sin( dot( tc.yx, vec2( 12.9898, 78.233 ) ) )* 43758.5453 ) - 0.5;
            #endif

        #endif

        #else

        float rndoffset = 0.0;

        #endif

        rndoffset *= 2.0;
        



        #ifndef D_MOTIONBLUR_ALWAYS_MULTIDIR

        bool lNeedsComplexTest = dot( lMaxMinDifference, lMaxMinDifference ) > 25.0f;

        #if defined( D_PLATFORM_ORBIS ) || defined( D_PLATFORM_XBOXONE )
        lNeedsComplexTest = ( ballot(lNeedsComplexTest) != 0 ); 
        #endif

        if( !lNeedsComplexTest )
        {
            // tile velocity is all identical, fall back on simple blur
            float fRecipSamples = 1.0 / ( lNumSamples - 1 );
            vec2  cVelocity = lDelta * (lWantedDistInPixels / lDistInPixels);
            float cSpeed = length( cVelocity );
            float lfRcpSpeed = 1.0 / ( length( cVelocity ) );

            vec2  lSampleStep   = cVelocity * fRecipSamples;
            vec2  lSampleCenter = tc - rndoffset * lSampleStep;
            vec2  lSampleStart  = lSampleCenter - (lNumSamples - 1) * 0.5 * lSampleStep;

            float lSampleStepLength = fRecipSamples * cSpeed;
            float lSampleStartDist = - ( lNumSamples * 0.5 ) * lSampleStepLength;

            for( float i = 0; i < lNumSamples; i++ )
            {
                vec2 lSamplePos = lSampleStart;
                lSampleStart += lSampleStep;

                float lSampleDist = abs( lSampleStartDist );
                lSampleStartDist += lSampleStepLength;

                //float weight = cylinder( lSampleDist, lfRcpSpeed );
                float weight = cylinder( i - ( lNumSamples * 0.5 ), 1.0 / ( lNumSamples ) );

                #ifdef D_PASS_2
                lSamplePos = tc + (lSamplePos - tc) * fRecipSamples * 2.0;
                #endif

                vec4 lNewSample = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), lSamplePos, 0.0 );
                lNewSample.xyz = SimpleUnHalfReinhard( lNewSample.xyz );
                acc  += lNewSample * weight;  
                wacc += weight;
            }
        }
        else
        #endif

        {
            float lfBaseDepth;
            float lfBaseDepthNormalised;
            float lfBaseRcpSpeed;
            vec2 lBaseDelta;
            vec2 lEncodedBaseDelta;
            bool lWantsColorClipAA;
            vec2 reproj_tc = GetPrevPosition(tc, 
                                             lUniforms.mpPerFrame->gClipPlanesVec4,
                                             lUniforms.mpCommonPerMesh->gInverseProjectionMat4,
                                             lUniforms.mpCommonPerMesh->gInverseViewMat4,
                                             lUniforms.mpCommonPerMesh->gViewProjectionMat4,
                                             lUniforms.mpPerFrame->gPrevViewProjectionMat4,
                                             lUniforms.mpPerFrame->gViewPositionVec3,
                                             lUniforms.mpPerFrame->gMBlurSettingsVec4,
                                             SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBuffer2Map ), 
                                             SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBuffer3Map ),
                                             lfBaseDepth,
                                             lfBaseDepthNormalised,
                                             lfBaseRcpSpeed,
                                             lBaseDelta,
                                             lEncodedBaseDelta,
                                             lWantsColorClipAA );

            lBaseDelta  *= lUniforms.mpPerFrame->gMBlurSettingsVec4.z;
            vec2 lBasePerp = lBaseDelta - lDelta * dot( lBaseDelta,lDelta ) / dot( lDelta,lDelta );
            vec2 lBaseNorm = lBaseDelta * lfBaseRcpSpeed;

            vec2 lDeltaNorm;
            vec2 lBasePerpInPixels = lBasePerp * lUniforms.mpPerFrame->gFrameBufferSizeVec4.xy;
            if( Bayer( uvec2( tc * lUniforms.mpPerFrame->gFrameBufferSizeVec4.xy )  ) < length(lBasePerpInPixels) / 30.0 )
            {
                lDelta = lBaseDelta;
                lDeltaNorm = lBaseNorm;
            }  
            else
            {
                lDeltaNorm = normalize( lDelta );
            }

            vec2  cVelocity = lDelta * (lWantedDistInPixels / lDistInPixels);
            float cSpeed = length( cVelocity );

            float fRecipSamples = 1.0 / ( lNumSamples - 1 );
            vec2  lSampleStep   = cVelocity * fRecipSamples;
            vec2  lSampleCenter = tc - rndoffset * lSampleStep;
            vec2  lSampleStart  = lSampleCenter - (lNumSamples - 1) * 0.5 * lSampleStep;

            float lSampleStepLength = fRecipSamples * cSpeed;
            float lSampleStartDist = - ( lNumSamples * 0.5 ) * lSampleStepLength;


            for( float i = 0; i < lNumSamples; i++ )
            {
                vec2 lSamplePos =  lSampleStart + lSampleStep * i;
                float lSampleDist = abs( lSampleStartDist + lSampleStepLength * i );
                vec2 lSampleOffset = lSamplePos - lSampleCenter;

                float lfSampleDepth;
                float lfSampleDepthNormalised;
                float lfSampleRcpSpeed;
                vec2 lSampleDelta;
                vec2 lEncodedSampleDelta;
                vec2 reproj_sample = GetPrevPosition(lSamplePos, 
                                             lUniforms.mpPerFrame->gClipPlanesVec4,
                                             lUniforms.mpCommonPerMesh->gInverseProjectionMat4,
                                             lUniforms.mpCommonPerMesh->gInverseViewMat4,
                                             lUniforms.mpCommonPerMesh->gViewProjectionMat4,
                                             lUniforms.mpPerFrame->gPrevViewProjectionMat4,
                                             lUniforms.mpPerFrame->gViewPositionVec3,
                                             lUniforms.mpPerFrame->gMBlurSettingsVec4,
                                             SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBuffer2Map ), 
                                             SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBuffer3Map ),
                                             lfSampleDepth,
                                             lfSampleDepthNormalised,
                                             lfSampleRcpSpeed,
                                             lSampleDelta,
                                             lEncodedSampleDelta,
                                             lWantsColorClipAA );


                lSampleDelta  *= lUniforms.mpPerFrame->gMBlurSettingsVec4.z;
                vec2 lSampleDeltaNorm = lSampleDelta * lfSampleRcpSpeed;

                float lfBaseCylWeight   = cylinder( lSampleDist, lfBaseRcpSpeed );
                float lfSampleCylWeight = cylinder( lSampleDist, lfSampleRcpSpeed );

                /*

                // possible x/y slope version of the cone check, might well be usable

                float ySampleAmt = abs( dot( lDeltaNorm, lEncodedSampleDelta ) );
                float xSampleAmt = abs( dot( vec2( -lDeltaNorm.y, lDeltaNorm.x ), lEncodedSampleDelta ) );

                float yBaseAmt = abs( dot( lDeltaNorm, lBaseDelta ) );
                float xBaseAmt = abs( dot( vec2( -lDeltaNorm.y, lDeltaNorm.x ), lBaseDelta ) );

                float lFrontVelWeight = (xSampleAmt < ySampleAmt)? 1.0 : 0.0;
                float lBackVelWeight  = (xBaseAmt < yBaseAmt)? 1.0 : 0.0;

                */


                float lFrontVelWeight = abs( dot( lDeltaNorm, lSampleDeltaNorm ) );
                float lBackVelWeight  = abs( dot( lDeltaNorm, lBaseNorm ) );

                lFrontVelWeight *= lFrontVelWeight;
                lBackVelWeight  *= lBackVelWeight;
                lFrontVelWeight *= lFrontVelWeight;
                lBackVelWeight  *= lBackVelWeight;

                float lfFrontSampleWeight = lfSampleCylWeight * lFrontVelWeight;
                float lfBackSampleWeight  = lfBaseCylWeight   * lBackVelWeight;

                float weight = ( lfBaseDepth < lfSampleDepth? lfBackSampleWeight : lfFrontSampleWeight );
                weight = saturate( weight );

                // need to work out if the base sample *would* be blended into the far sample
                // (when the far sample does its gather)
                // which we can compute easily by swapping around some of these numbers

                float weightMirror = ( lfBaseDepth < lfSampleDepth? lfFrontSampleWeight : lfBackSampleWeight );
                weightMirror = 1.0 - saturate( weightMirror );

                // if the base sample is going to be blended into a far sample,
                // duck it down by an appropriate amount so we don't gain energy
                lfBaseSampleWeight += weightMirror;

                float fRecipSamplesMultiplier = (weight > 0.0) ? fRecipSamples : 0.0;
                #ifdef D_PASS_2
                lSamplePos = tc + (lSamplePos - tc) * fRecipSamplesMultiplier * 2.0;
                #endif

                vec4 lNewSample = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), lSamplePos, 0.0 );

                lNewSample.xyz = SimpleUnHalfReinhard( lNewSample.xyz );
                acc  += lNewSample * weight;  
                wacc += weight;

            }
        }

        //FRAGMENT_COLOUR = vec4( reproj_tc, 0.0, 1.0 );    
        //FRAGMENT_COLOUR = vec4( texture2D( lUniforms.mpCustomPerMesh->gBufferMap, IN(mTexCoordsVec2) ).xyz, 1.0 );    
        //FRAGMENT_COLOUR = vec4( GammaCorrectOutput( acc.xyz / wacc ), 1.0 );
        //FRAGMENT_COLOUR = vec4(  (lBaseDelta*100)+0.5, 0.0, 1.0 );    

        lOutColor  = lBaseSample * max( 0.0, lfBaseSampleWeight - wacc ) + acc;
        lOutColor /= max( lfBaseSampleWeight, wacc );

        lOutColor.xyz = SimpleHalfReinhard( lOutColor.xyz );
    }


    #ifdef D_PASS_2  
    //FRAGMENT_COLOUR = vec4( lOutColor, 1.0 );
    FRAGMENT_COLOUR = vec4( SimpleUnReinhard( lOutColor.xyz ), lOutColor.a );

    #else
    // tonemap in pass 1
    //FRAGMENT_COLOUR = vec4( GammaCorrectOutput( TonemapKodak( lOutColor ) / TonemapKodak( vec3( 1.0,1.0,1.0 ) ) ), 1.0 );
    FRAGMENT_COLOUR = vec4( SimpleReinhard( lOutColor.xyz ), lOutColor.a );

    #endif

}
#endif

// =================================================================================================
//
// MOTION VECTOR DILATE
//
// =================================================================================================

#ifdef D_POSTPROCESS_MOTIONDILATE_PASS1

//-----------------------------------------------------------------------------
//      Global Data

#define D_NEIGHBORHOOD_SIZE 16.0
#define D_NEIGHBORHOOD_OVER 0.5
#define D_BUFFER_SCALING 0.125

//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END

#ifdef D_PLATFORM_ORBIS
#pragma argument(targetoccupancy_atallcosts=70)
#pragma argument(nounrollallloops)
#endif

//-----------------------------------------------------------------------------
//      Functions 

vec2
GetPixelEncodedMotion(
    SAMPLER2DARG( lMotionMap ),
    in vec2 lTexCoords )
{
    vec2 lDelta = Texture2DNoFiltering( SAMPLER2DPARAM( lMotionMap ), lTexCoords ).xy;
    return lDelta * 0.5;
}

void
MinMaxNewSpeed(
    SAMPLER2DARG( lMotionMap ),
    in vec2 lTexCoords, 
    inout float lMinSpeed,
    inout float lMaxSpeed,
    inout vec2 lMinMotion,
    inout vec2 lMaxMotion )
{
    vec2 lEncodedMotion = GetPixelEncodedMotion( SAMPLER2DPARAM( lMotionMap ), lTexCoords ) * 2.0;
  
    vec2  lCenteredMotion = lEncodedMotion   - vec2( 0.5, 0.5 );
    float lSpeedSquared   = dot( lCenteredMotion, lCenteredMotion );

    if( lSpeedSquared > lMaxSpeed )
    {
        lMaxSpeed  = lSpeedSquared;
        lMaxMotion = lEncodedMotion;
    } 

    if( lSpeedSquared < lMinSpeed )
    {
        lMinSpeed  = lSpeedSquared;
        lMinMotion = lEncodedMotion;
    } 
}

FRAGMENT_MAIN_COLOUR_SRT
{           

    #if defined( D_PLATFORM_ORBIS ) || defined( D_PLATFORM_XBOXONE )
    //#if 0

    #if 1

    vec2 lSample = TEX_COORDS;

    // read the texture on the current pixel, but also the quad above and below
    vec2 lMinMotion   = GetPixelEncodedMotion( SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh, gBufferMap ), lSample ) * 2.0;
    vec2  lCenteredMotion   = lMinMotion   - vec2( 0.5,0.5 );
    float lMinSpeed   = dot( lCenteredMotion,   lCenteredMotion );
    float lMaxSpeed = lMinSpeed;
    vec2 lMaxMotion = lMinMotion;

    vec2 lSampleLt = lSample - vec2( lUniforms.mpPerFrame->gFrameBufferSizeVec4.z * 8.0, 0.0 );
    vec2 lSampleRt = lSample + vec2( lUniforms.mpPerFrame->gFrameBufferSizeVec4.z * 8.0, 0.0 );

    MinMaxNewSpeed( SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBufferMap ), lSampleLt, lMinSpeed, lMaxSpeed, lMinMotion, lMaxMotion );
    MinMaxNewSpeed( SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBufferMap ), lSampleRt, lMinSpeed, lMaxSpeed, lMinMotion, lMaxMotion );

        for( float xx = 0.0; xx < 1.0; xx += D_NEIGHBORHOOD_OVER )
        {
            vec2 lOffset = vec2( xx, 0.0 ) * lUniforms.mpPerFrame->gFrameBufferSizeVec4.zw;
            
            MinMaxNewSpeed( SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBufferMap ), lSample   + lOffset, lMinSpeed, lMaxSpeed, lMinMotion, lMaxMotion );
            MinMaxNewSpeed( SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBufferMap ), lSampleLt + lOffset, lMinSpeed, lMaxSpeed, lMinMotion, lMaxMotion );
            MinMaxNewSpeed( SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBufferMap ), lSampleRt + lOffset, lMinSpeed, lMaxSpeed, lMinMotion, lMaxMotion );
        }

    // swap low bit of X
    float lAlt0MaxSpeed = LaneSwizzle( lMaxSpeed,   0x1f, 0, 1 );

    float lAlt0MaxMotionX  = LaneSwizzle( lMaxMotion.x, 0x1f, 0, 1 );
    float lAlt0MaxMotionY  = LaneSwizzle( lMaxMotion.y, 0x1f, 0, 1 );

    float lAlt0MinSpeed = LaneSwizzle( lMinSpeed,   0x1f, 0, 1 );

    float lAlt0MinMotionX  = LaneSwizzle( lMinMotion.x, 0x1f, 0, 1 );
    float lAlt0MinMotionY  = LaneSwizzle( lMinMotion.y, 0x1f, 0, 1 );

    if( lAlt0MaxSpeed > lMaxSpeed )
    {
        lMaxSpeed  = lAlt0MaxSpeed;
        lMaxMotion.x = lAlt0MaxMotionX;
        lMaxMotion.y = lAlt0MaxMotionY;
    }

    if( lAlt0MinSpeed < lMinSpeed )
    {
        lMinSpeed  = lAlt0MinSpeed;
        lMinMotion.x = lAlt0MinMotionX;
        lMinMotion.y = lAlt0MinMotionY;
    } 

    // swap bit 1 of X
    float lAlt1MaxSpeed = LaneSwizzle( lMaxSpeed,   0x1f, 0, 4 );

    float lAlt1MaxMotionX  = LaneSwizzle( lMaxMotion.x, 0x1f, 0, 4 );
    float lAlt1MaxMotionY  = LaneSwizzle( lMaxMotion.y, 0x1f, 0, 4 );

    float lAlt1MinSpeed = LaneSwizzle( lMinSpeed,   0x1f, 0, 4 );

    float lAlt1MinMotionX  = LaneSwizzle( lMinMotion.x, 0x1f, 0, 4 );
    float lAlt1MinMotionY  = LaneSwizzle( lMinMotion.y, 0x1f, 0, 4 );

    if( lAlt1MaxSpeed > lMaxSpeed )
    {
        lMaxSpeed  = lAlt1MaxSpeed;
        lMaxMotion.x = lAlt1MaxMotionX;
        lMaxMotion.y = lAlt1MaxMotionY;
    }

    if( lAlt1MinSpeed < lMinSpeed )
    {
        lMinSpeed  = lAlt1MinSpeed;
        lMinMotion.x = lAlt1MinMotionX;
        lMinMotion.y = lAlt1MinMotionY;
    } 

    // swap bit 2 of X
    float lAlt2MaxSpeed = LaneSwizzle( lMaxSpeed,   0x1f, 0, 16 );

    float lAlt2MaxMotionX  = LaneSwizzle( lMaxMotion.x, 0x1f, 0, 16 );
    float lAlt2MaxMotionY  = LaneSwizzle( lMaxMotion.y, 0x1f, 0, 16 );

    float lAlt2MinSpeed = LaneSwizzle( lMinSpeed,   0x1f, 0, 16 );

    float lAlt2MinMotionX  = LaneSwizzle( lMinMotion.x, 0x1f, 0, 16 );
    float lAlt2MinMotionY  = LaneSwizzle( lMinMotion.y, 0x1f, 0, 16 );

    if( lAlt2MaxSpeed > lMaxSpeed )
    {
        lMaxSpeed  = lAlt2MaxSpeed;
        lMaxMotion.x = lAlt2MaxMotionX;
        lMaxMotion.y = lAlt2MaxMotionY;
    }

    if( lAlt2MinSpeed < lMinSpeed )
    {
        lMinSpeed  = lAlt2MinSpeed;
        lMinMotion.x = lAlt2MinMotionX;
        lMinMotion.y = lAlt2MinMotionY;
    } 

    #else

    vec2 lSample = IN(mTexCoordsVec2);

    uint liLaneIndex = __v_mbcnt_lo_u32_b32(uint(-1), __v_mbcnt_hi_u32_b32(uint(-1), 0u));

    uint liLaneX = liLaneIndex & 0x55;          
         liLaneX = (liLaneX ^ (liLaneX >>  1)) & 0x33;
         liLaneX = (liLaneX ^ (liLaneX >>  2)) & 0x0f;
    uint liLaneY = (liLaneIndex >> 1) & 0x55;    
         liLaneY = (liLaneY ^ (liLaneY >>  1)) & 0x33;
         liLaneY = (liLaneY ^ (liLaneY >>  2)) & 0x0f;

    // read the texture on the current pixel, but also the quad above and below
    vec2 lEncodedMotion   = GetPixelEncodedMotion( SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBufferMap ), lSample ) * 2.0;
    vec2 lEncodedMotionLt = GetPixelEncodedMotion( SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBufferMap ), lSample - vec2( lUniforms.mpPerFrame->gFrameBufferSizeVec4.z * 8.0, 0.0 ) ) * 2.0;
    vec2 lEncodedMotionRt = GetPixelEncodedMotion( SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBufferMap ), lSample + vec2( lUniforms.mpPerFrame->gFrameBufferSizeVec4.z * 8.0, 0.0 ) ) * 2.0;

    vec2  lCenteredMotion   = lEncodedMotion   - vec2( 0.5 );
    vec2  lCenteredMotionLt = lEncodedMotionLt - vec2( 0.5 );
    vec2  lCenteredMotionRt = lEncodedMotionRt - vec2( 0.5 );

    float lSpeedSquared   = dot( lCenteredMotion,   lCenteredMotion );
    float lSpeedSquaredLt = dot( lCenteredMotionLt, lCenteredMotionLt );
    float lSpeedSquaredRt = dot( lCenteredMotionRt, lCenteredMotionRt );

    for( uint xx=0; xx<8; ++xx )
    {
        // re-encode the loop X to morton tiling order
        uint xCode = (xx    ^ (xx    << 2)) & 0x33;
             xCode = (xCode ^ (xCode << 1)) & 0x55;

        // here is how we would get the lane with current Y but desired X
        // uint altLane = ( liLaneIndex & 0xAA ) | xCode;

        // the shuffle instruction allows us to do this though
        float lCurSpeedSquared   = LaneSwizzle( lSpeedSquared,   0xa, xCode, 0 );
        float lCurSpeedSquaredLt = LaneSwizzle( lSpeedSquaredLt, 0xa, xCode, 0 );
        float lCurSpeedSquaredRt = LaneSwizzle( lSpeedSquaredRt, 0xa, xCode, 0 );

        // important: do these swizzles while the execution mask is full
        // otherwise some threads may unexpectedly get back 0s
        float lCurMotionX = LaneSwizzle( lEncodedMotion.x, 0xa, xCode, 0 );
        float lCurMotionY = LaneSwizzle( lEncodedMotion.y, 0xa, xCode, 0 );

        float lCurMotionLtX = LaneSwizzle( lEncodedMotionLt.x, 0xa, xCode, 0 );
        float lCurMotionLtY = LaneSwizzle( lEncodedMotionLt.y, 0xa, xCode, 0 );

        float lCurMotionRtX = LaneSwizzle( lEncodedMotionRt.x, 0xa, xCode, 0 );
        float lCurMotionRtY = LaneSwizzle( lEncodedMotionRt.y, 0xa, xCode, 0 );

        if( lCurSpeedSquared > lMaxSpeed )
        {
            lMaxSpeed  = lCurSpeedSquared;
            lMaxMotion.x = lCurMotionX;
            lMaxMotion.y = lCurMotionY;
        } 

        if( lCurSpeedSquared < lMinSpeed )
        {
            lMinSpeed  = lCurSpeedSquared;
            lMinMotion.x = lCurMotionX;
            lMinMotion.y = lCurMotionY;
        } 

        if( xx >= liLaneX )
        {
            if( lCurSpeedSquaredLt > lMaxSpeed )
            {
                lMaxSpeed  = lCurSpeedSquaredLt;
                lMaxMotion.x = lCurMotionLtX;
                lMaxMotion.y = lCurMotionLtY;
            } 

            if( lCurSpeedSquaredLt < lMinSpeed )
            {
                lMinSpeed  = lCurSpeedSquaredLt;
                lMinMotion.x = lCurMotionLtX;
                lMinMotion.y = lCurMotionLtY;
            } 
        }

        if( xx <= liLaneX )
        {
            if( lCurSpeedSquaredRt > lMaxSpeed )
            {
                lMaxSpeed  = lCurSpeedSquaredRt;
                lMaxMotion.x = lCurMotionRtX;
                lMaxMotion.y = lCurMotionRtY;
            } 

            if( lCurSpeedSquaredRt < lMinSpeed )
            {
                lMinSpeed  = lCurSpeedSquaredRt;
                lMinMotion.x = lCurMotionRtX;
                lMinMotion.y = lCurMotionRtY;
            } 
        }
    }

    #endif

    #else


    float lMaxSpeed = 0.0;
    vec2 lMaxMotion = vec2( 0.5, 0.5 );

    float lMinSpeed = 10000.0;
    vec2 lMinMotion = vec2( 0.5, 0.5 );

    vec2 lPixelSize = lUniforms.mpPerFrame->gFrameBufferSizeVec4.zw * D_NEIGHBORHOOD_OVER;

    vec2 lStartUpperCorner = vec2( D_NEIGHBORHOOD_SIZE, D_NEIGHBORHOOD_SIZE * D_BUFFER_SCALING );

    // first pixel loc, the upper left 
    vec2 lStartSample = (TEX_COORDS) - lStartUpperCorner * lPixelSize * 0.5 + lPixelSize * 0.5;
    vec2 lSample = lStartSample;

    #ifndef D_DILATE_HORZ
    for( float y = 0; y < D_NEIGHBORHOOD_SIZE; y++ )
    #endif
    {

        #ifndef D_DILATE_VERT
        for( float x = 0; x < D_NEIGHBORHOOD_SIZE; x++ )
        #endif
        {
            // compute the previous position of this pixel
            vec2 lEncodedMotion = GetPixelEncodedMotion( SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBufferMap ), lSample ) * 2.0;

            vec2  lCenteredMotion = lEncodedMotion - vec2( 0.5, 0.5 );
            float lSpeedSquared = dot( lCenteredMotion, lCenteredMotion );

            if( lSpeedSquared > lMaxSpeed )
            {
                lMaxSpeed  = lSpeedSquared;
                lMaxMotion = lEncodedMotion;
            } 

            if( lSpeedSquared < lMinSpeed )
            {
                lMinSpeed  = lSpeedSquared;
                lMinMotion = lEncodedMotion;
            } 

            lSample.x += lPixelSize.x;
        }

        lSample.x = lStartSample.x;
        lSample.y += lPixelSize.y;
    }


    #endif


    FRAGMENT_COLOUR = vec4( lMaxMotion, lMinMotion );    
}

#endif

// =================================================================================================
//
// MOTION VECTOR DILATE
//
// =================================================================================================

#ifdef D_POSTPROCESS_MOTIONDILATE_PASS2

//-----------------------------------------------------------------------------
//      Global Data

#define D_NEIGHBORHOOD_SIZE 16.0
#define D_NEIGHBORHOOD_OVER 0.5
#define D_BUFFER_SCALING 0.125

//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END

#ifdef D_PLATFORM_ORBIS
#pragma argument(targetoccupancy_atallcosts=70)
#pragma argument(nounrollallloops)
#endif

vec2 ClipVector(
    in vec2 lMotion,
    in vec2 lRcpMaxVectorLength )
{
    vec2 lMotionNorm = abs( max( vec2( 1.0, 1.0 ), lMotion * lRcpMaxVectorLength ) );
    return lMotion / lMotionNorm;
}

//-----------------------------------------------------------------------------
//      Functions 

FRAGMENT_MAIN_COLOUR_SRT
{         
    vec2 lPixelSize = lUniforms.mpPerFrame->gFrameBufferSizeVec4.zw * D_NEIGHBORHOOD_OVER;

    vec2 lStartUpperCorner = vec2( D_NEIGHBORHOOD_SIZE * D_BUFFER_SCALING, D_NEIGHBORHOOD_SIZE );

    // first pixel loc, the upper left 
    vec2 lStartSample = TEX_COORDS - lStartUpperCorner * lPixelSize * 0.5 + lPixelSize * 0.5;
    vec2 lSample = lStartSample;

    float lMaxSpeed = 0.0;
    vec2 lMaxMotion = vec2( 0.5, 0.5 );

    float lMinSpeed = 10000.0;
    vec2 lMinMotion = vec2( 0.5, 0.5 );

    #ifndef D_DILATE_HORZ
    for( float y = 0; y < D_NEIGHBORHOOD_SIZE; y++ )
    #endif
    {

        #ifndef D_DILATE_VERT
        for( float x = 0; x < D_NEIGHBORHOOD_SIZE; x++ )
        #endif
        {
            vec4 lMotionBuffer = Texture2DNoFiltering( SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBufferMap ), lSample );

            vec2 lSmpMaxMotion = lMotionBuffer.xy;
            vec2 lSmpMinMotion = lMotionBuffer.zw;

            float lMaxSpeedSquared = dot( lSmpMaxMotion - vec2( 0.5, 0.5 ), lSmpMaxMotion - vec2( 0.5, 0.5 ) );
            float lMinSpeedSquared = dot( lSmpMinMotion - vec2( 0.5, 0.5 ), lSmpMinMotion - vec2( 0.5, 0.5 ) );

            if( lMaxSpeedSquared > lMaxSpeed )
            {
                lMaxSpeed  = lMaxSpeedSquared;
                lMaxMotion = lSmpMaxMotion;
            } 

            if( lMinSpeedSquared < lMinSpeed )
            {
                lMinSpeed  = lMinSpeedSquared;
                lMinMotion = lSmpMinMotion;
            } 

            lSample.x += lPixelSize.x;
        }

        lSample.x = lStartSample.x;
        lSample.y += lPixelSize.y;
    }

    // clip the vectors

    vec2 lMaxVectorLength = lUniforms.mpPerFrame->gFrameBufferSizeVec4.xy * (D_NEIGHBORHOOD_SIZE / D_BUFFER_SCALING);
    vec2 lRcpMaxVectorLength = vec2( 1.0, 1.0 ) / lMaxVectorLength;

    vec2 lMaxMotionClip = lMaxMotion;// EncodeMotion( ClipVector( DecodeMotion( lMaxMotion ), lRcpMaxVectorLength ) );
    vec2 lMinMotionClip = lMinMotion;// EncodeMotion( ClipVector( DecodeMotion( lMinMotion ), lRcpMaxVectorLength ) );

    FRAGMENT_COLOUR = vec4( lMaxMotionClip, lMinMotionClip );    
}

#endif

// =================================================================================================
//
// MOTION VECTOR RESOLVE
//
// =================================================================================================

#ifdef D_POSTPROCESS_MOTIONRESOLVE

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END

//-----------------------------------------------------------------------------
//      Functions 

FRAGMENT_MAIN_COLOUR_SRT
{           
    vec2 lSample = IN(mTexCoordsVec2);

    // compute the previous position of this pixel
    float lfDepth;
    float lfDepthNormalised;
    float lfSpeed;
    vec2 lMotion;
    vec2 lEncodedMotion;
    bool lWantsColorClipAA;
    vec2 lSampleReproject = GetPrevPosition(lSample, 
                                     lUniforms.mpPerFrame->gClipPlanesVec4,
                                     lUniforms.mpCommonPerMesh->gInverseProjectionMat4,
                                     lUniforms.mpCommonPerMesh->gInverseViewMat4,
                                     lUniforms.mpCommonPerMesh->gViewProjectionMat4,
                                     lUniforms.mpPerFrame->gPrevViewProjectionMat4,
                                     lUniforms.mpPerFrame->gViewPositionVec3,
                                     lUniforms.mpPerFrame->gMBlurSettingsVec4,
                                     SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBufferMap ), 
                                     SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBuffer1Map ),
                                     lfDepth,
                                     lfDepthNormalised,
                                     lfSpeed,
                                     lMotion,
                                     lEncodedMotion,
                                     lWantsColorClipAA );

    #ifdef D_WRITE_WANTS_CLIP
    lWantsColorClipAA = true;
    #endif

    if( !lWantsColorClipAA )
    {
        int liMaterialID = int( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer3Map ), lSample ).r * 255.0 );
        lWantsColorClipAA = ( liMaterialID & D_CLAMP_AA ) != 0;
    }

    float lfEncodedSettings = 0.0;
    if( lWantsColorClipAA ) lfEncodedSettings += 0.5;

    #ifdef D_WRITE_IS_TERRAIN
    lfEncodedSettings += 0.25;
    #endif

    FRAGMENT_COLOUR = vec4( lEncodedMotion * 2.0, lfEncodedSettings, 1.0 );    
}

#endif


// =================================================================================================
//
// TEMPORAL AA
//
// =================================================================================================


#ifdef D_POSTPROCESS_STAA_APPLY


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

#define TAA_BACK_IS_TONEMAP 0
#define TAA_BACK_IS_TONEMAP_YCGCO 0
#define TAA_BACK_IS_YCGCO 0

#define D_SPEED_MULTIPLIER 1000.0

#if TAA_BACK_IS_TONEMAP

#define TAA_ACC_BUFFER_INPUT( pos ) texture2DLod(lUniforms.mpCustomPerMesh->gBuffer1Map, pos, 0.0).rgb
#define TAA_MAIN_BUFFER_INPUT( pos ) saturate( TonemapKodak( texture2DLod(lUniforms.mpCustomPerMesh->gBufferMap, pos, 0.0).rgb ) / TonemapKodak( vec3( 1.0,1.0,1.0 ) ) )

#define TAA_ACC_BUFFER_OUTPUT( col ) ( col )
#define TAA_MAIN_BUFFER_OUTPUT( col ) GammaCorrectOutput( col )

#define TAA_GET_LUMINANCE( col ) ( 0.25 * ( col.r + col.b ) + 0.5 * col.g )

#elif TAA_BACK_IS_TONEMAP_YCGCO

#define TAA_ACC_BUFFER_INPUT( pos ) ( RGBToYCgCo( texture2DLod(lUniforms.mpCustomPerMesh->gBuffer1Map, pos, 0.0).rgb ) )
#define TAA_MAIN_BUFFER_INPUT( pos ) ( TonemapKodakYCgCo( RGBToYCgCo( texture2DLod(lUniforms.mpCustomPerMesh->gBufferMap, pos, 0.0).rgb ) ) )

#define TAA_ACC_BUFFER_OUTPUT( col ) ( YCgCoToRGB(  col ) )
#define TAA_MAIN_BUFFER_OUTPUT( col ) GammaCorrectOutput( YCgCoToRGB(  col ) )

#define TAA_GET_LUMINANCE( col ) ( col.x )

#elif TAA_BACK_IS_YCGCO

#define TAA_ACC_BUFFER_INPUT( pos ) texture2DLod(lUniforms.mpCustomPerMesh->gBuffer1Map, pos, 0.0).rgb
#define TAA_MAIN_BUFFER_INPUT( pos ) SimpleReinhardLum( RGBToYCgCo( texture2DLod(lUniforms.mpCustomPerMesh->gBufferMap, pos, 0.0).rgb ) )

#define TAA_ACC_BUFFER_OUTPUT( col ) ( col )
#define TAA_MAIN_BUFFER_OUTPUT( col ) GammaCorrectOutput( TonemapKodak( YCgCoToRGB( SimpleUnReinhardLum( col ) ) ) / TonemapKodak( vec3( 1.0,1.0,1.0 ) ) )

#define TAA_GET_LUMINANCE( col ) ( col.x )

#else

#if defined( D_PLATFORM_DX12 )

#define TAA_ACC_BUFFER_INPUT( pos ) texture2DLod( gBuffer1Map, pos, 0.0).rgb
#define TAA_MAIN_BUFFER_INPUT( pos ) ( SimpleReinhard( clamp( texture2DLod( gBufferMap, pos, 0.0).rgb, 0.0, 1024.0 )  ) )

#else

#define TAA_ACC_BUFFER_INPUT( pos ) texture2DLod(lUniforms.mpCustomPerMesh->gBuffer1Map, pos, 0.0).rgb
#define TAA_MAIN_BUFFER_INPUT( pos ) ( SimpleReinhard( clamp( texture2DLod(lUniforms.mpCustomPerMesh->gBufferMap, pos, 0.0).rgb, 0.0, 1024.0 )  ) )

#endif

#define TAA_ACC_BUFFER_OUTPUT( col ) ( col )
//#define TAA_MAIN_BUFFER_OUTPUT( col ) GammaCorrectOutput( TonemapKodak( SimpleUnReinhard( col ) ) / TonemapKodak( vec3( 1.0,1.0,1.0 ) ) )
#define TAA_MAIN_BUFFER_OUTPUT( col ) ( SimpleUnReinhard( col ) )


#define TAA_GET_LUMINANCE( col ) ( 0.25 * ( col.r + col.b ) + 0.5 * col.g )

#endif



vec3 SimpleReinhard( in vec3 col )
{
    return col / ( 1.0 + TAA_GET_LUMINANCE( col ) );
}

vec3 SimpleUnReinhard( in vec3 col )
{
    return col / ( 1.0 - TAA_GET_LUMINANCE( col ) );
}


vec3 SimpleReinhardLum( in vec3 col )
{
    return col * 1.0 / ( 1.0 + col.x );
}

vec3 SimpleUnReinhardLum( in vec3 col )
{
    return ( col ) * 1.0 / ( 1.0 - col.x );
}


#ifdef D_PLATFORM_ORBIS    
//#pragma argument (O4; fastmath; scheduler=minpressure)
//#pragma argument(reservelds=28)
//#pragma warning (disable:7203)
//#pragma argument(targetoccupancy_atallcosts=90)
#if !defined( D_SPLITSCREEN_TEST ) && !defined( D_TERRAIN_MODE )
#pragma argument(maxvgprcount=36)
#endif

float fastRcpSqrtNR0(float inX)
{
    return rsqrt(inX);
}

float fastRcpNR0(float inX)
{
    return rcp(inX);
}

#else

float fastRcpSqrtNR0( float inX )
{
    return 1.0 / sqrt(inX);
}

float fastRcpNR0( float inX )
{
    return 1.0 / inX;
}

#endif

// returns from 1.0 for no clip, down to 0.0 for "the aabb is size 0"
float get_clip_factor(
    vec3 q,
    vec3 aabb_min,
    vec3 aabb_max )
{
    vec3 p_clip = 0.5 * (aabb_max + aabb_min);
    vec3 e_clip = 0.5 * (aabb_max - aabb_min);

    vec3 v_clip = q - p_clip;
    vec3 v_unit = v_clip / e_clip;
    vec3 a_unit = abs(v_unit);

    float ma_unit = max( a_unit.x, max( a_unit.y, a_unit.z ) );

    if( ma_unit > 1.0 )
        return 1.0 / ma_unit;
    else
        return 1.0;
}

vec3 clip_aabb(
    vec3 q,
    vec3 aabb_min,
    vec3 aabb_max )
{
        // note: only clips towards aabb center (but fast!)
        vec3 p_clip = 0.5 * (aabb_max + aabb_min);
        vec3 e_clip = 0.5 * (aabb_max - aabb_min);

        vec3 v_clip = q - p_clip;
        vec3 v_unit = e_clip / v_clip;
        vec3 a_unit = abs(v_unit);
        float ma_unit = saturate( min(a_unit.x, min(a_unit.y, a_unit.z)) );

        return p_clip + v_clip * ma_unit;
}

vec3 clip_lum(
    vec3 q,
    vec3 aabb_min,
    vec3 aabb_max )
{
    vec3  p_clip = 0.5 * (aabb_max + aabb_min);
    float e_clip = 0.5 * (aabb_max.x - aabb_min.x);

    vec3  v_clip = q - p_clip;

    float v_abs = abs( v_clip.x );

    if( v_abs > e_clip )
    {
        float a_unit = v_abs / e_clip;
        return p_clip + v_clip / a_unit;
    }
    else
    {
        return q;
    }
}

vec3 clip_lum_flat(
    vec3 q,
    float aabb_min,
    float aabb_max )
{
    float p_clip = 0.5 * (aabb_max + aabb_min);
    float e_clip = 0.5 * (aabb_max - aabb_min);

    float v_clip = q.x - p_clip;

    float v_abs = abs( v_clip.x );

    if( v_abs > e_clip )
    {
        float a_unit = v_abs / e_clip;
        return q * ( p_clip + v_clip / a_unit ) / q.x;
    }
    else
    {
        return q;
    }
}

FRAGMENT_MAIN_COLOUR_SRT
{           
    #ifdef D_ALWAYS_COLOUR_CLIP
        bool lbForceColorClip = true;
    #else
        bool lbForceColorClip = false;
    #endif

    #ifdef D_TERRAIN_MODE
        bool lbIsTerrain = true;
    #else
        bool lbIsTerrain = false;
    #endif

    vec2 lTexCoords = TEX_COORDS;


    // compute the previous position of this pixel
    vec3 lCenterSmpMotion = Texture2DNoFiltering( SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh, gBuffer2Map ), lTexCoords ).xyz;

    vec2 lEncodedDeltaDilated = lCenterSmpMotion.xy;
    vec2 lEncodedDeltaMinDilated = lCenterSmpMotion.xy;

    vec4 lLowResMotion = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer4Map ), lTexCoords, 0.0);

    bool lbUsePrevAccum = abs( lLowResMotion.x - 0.5 ) < 0.45;
    lbUsePrevAccum = lbUsePrevAccum && abs( lLowResMotion.y - 0.5 ) < 0.45;

    if( lUniforms.mpPerFrame->gTaaSettingsVec4.x < 0.0 )
    {
        lbForceColorClip = true;
    }

    if( lCenterSmpMotion.z >= 0.5 )
    {
        lbForceColorClip = true;
        lCenterSmpMotion.z -= 0.5;
    }   

    if( lCenterSmpMotion.z >= 0.25 )
    {
        lbIsTerrain = true;
        lCenterSmpMotion.z -= 0.25;
    }   

    #ifndef D_NO_MOTION_DILATE
    {
        vec2 lLowResMotionDiff = lLowResMotion.xy - lLowResMotion.zw;

        float lfSpeedDilated = dot( lCenterSmpMotion.xy - 0.5, lCenterSmpMotion.xy - 0.5 );
        if( lfSpeedDilated > 0.235 )
        {
            lbForceColorClip = true;
        }   

        // dilate the motion vectors in a 3x3 max/min pattern so we don't miss nearby moving edges

        #ifndef D_ALWAYS_COLOUR_CLIP
        //if( dot( lLowResMotionDiff, lLowResMotionDiff ) > 0.0001 )
        {
            float lfMinSpeedDilated = lfSpeedDilated;

            for( float dy = -1.0; dy <= 1.0; dy += 1.0 )
            {
                for( float dx = -1.0; dx <= 1.0; dx += 1.0 )
                {
                    if( dx != 0.0 || dy != 0.0 )
                    {
                        vec2 lCoordsOffset = vec2( dx, dy ) * lUniforms.mpPerFrame->gFrameBufferSizeVec4.zw;
                        vec2 lSubTexCoords = lTexCoords + lCoordsOffset;

                        vec3 lSmpMotion = Texture2DNoFiltering( SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh, gBuffer2Map ), lSubTexCoords ).xyz;
                        float lSpeedSquared = dot( lSmpMotion.xy - 0.5, lSmpMotion.xy - 0.5 );    
                            
                        if( lSmpMotion.z >= 0.5 )
                        {                        
                            lbForceColorClip = true;
                        }

                        if( lSpeedSquared >= lfSpeedDilated )
                        {
                            lfSpeedDilated      = lSpeedSquared;
                            lEncodedDeltaDilated = lSmpMotion.xy;
                        }

                        if( lSpeedSquared  < lfMinSpeedDilated )
                        {
                            lfMinSpeedDilated = lSpeedSquared;
                            lEncodedDeltaMinDilated = lSmpMotion.xy;
                        }
                    }
                }
            }
        }
        #endif
    }
    #endif

    vec2 ldejit = GetDejitteredTexCoord( lTexCoords.xy, lUniforms.mpPerFrame->gDeJitterVec4 );

    // four points, plus center, from the current screen
    vec3 cM = TAA_MAIN_BUFFER_INPUT( ldejit.xy  );

    // also the current screen but without dejitter, to blend to backbuffer
    vec3 cM_jittered = TAA_MAIN_BUFFER_INPUT(lTexCoords.xy);

    vec2 lDeltaDilated = DecodeMotion( lEncodedDeltaDilated );

    vec2 lTexCoordsReproject = lTexCoords + lDeltaDilated;

    float lfSpeed       = length( lDeltaDilated ) * D_SPEED_MULTIPLIER;
    bool lReadAccBuffer = ( ( saturate( lTexCoordsReproject.x ) == lTexCoordsReproject.x ) && 
                            ( saturate( lTexCoordsReproject.y ) == lTexCoordsReproject.y ) && 
                            ( lfSpeed < 128.0 ) );

    vec3 cAcc = cM_jittered;
    vec3 cAcc_as_read = cAcc;  // (for debug output)

    float largeVelBlend = 0.0;
    // if reproj is inside the back texture
    if( lReadAccBuffer )
    {
        // accum buffer result for current pixel
        cAcc = TAA_ACC_BUFFER_INPUT(lTexCoordsReproject);  
        cAcc_as_read = cAcc;

        // blend out if velocity is a lot of the screen
        largeVelBlend = saturate( (1.0/64.0) * ( lfSpeed - 64.0 ) );
        cAcc = mix( cAcc, cM_jittered, largeVelBlend );
    }

    // if there is no disagreement over the pixel color,
    // we can skip a LOT of texture reads, logic, etc
    vec3 cDiff = cM_jittered - cAcc;
    vec3 cDiff2 = cM - cAcc;

    lbUsePrevAccum = lbUsePrevAccum && dot( cDiff, cDiff ) + dot( cDiff2, cDiff2 ) > 1.0 / 1024.0;

    float lfSpeedBlend      = 1.0;

    if ( lbUsePrevAccum )
    {
        vec2 lPrevDeltaEncoded = lEncodedDeltaDilated;
        vec2 lPrevMinDeltaEncoded = lEncodedDeltaMinDilated;

        #ifndef D_ALWAYS_COLOUR_CLIP
        //if( !lbForceColorClip )
        {
            vec4 lPrevSpeeds = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer5Map ), lTexCoordsReproject, 0.0);

            lPrevDeltaEncoded  = lPrevSpeeds.xy;
            vec2 lPrevDelta    = DecodeMotion( lPrevSpeeds.xy );

            largeVelBlend = saturate( (1.0/64.0) * ( length( lPrevDelta ) * D_SPEED_MULTIPLIER - 64.0 ) );
            cAcc = mix( cAcc, cM_jittered, largeVelBlend );

            if( lPrevSpeeds.z != 1.0 )
            {
                lPrevMinDeltaEncoded = lPrevSpeeds.zw;

                vec2 lPrevMinDelta = DecodeMotion( lPrevSpeeds.zw );
                vec2 lDeltaMinDilated = DecodeMotion( lEncodedDeltaMinDilated );

                #ifndef D_TERRAIN_MODE
                if( !lbIsTerrain )
                {
                    // blending constant to kill the backbuffer when movement changes
                    // prevZ == 1.0 means we clipped last frame, no motion info, need to clip again this frame
                    lfSpeedBlend = saturate( max( max( length( lPrevMinDelta - lDeltaDilated ), 
                                                       length( lPrevDelta    - lDeltaMinDilated ) ),
                                                  max( length( lPrevMinDelta - lDeltaMinDilated ), 
                                                       length( lPrevDelta    - lDeltaDilated ) ) ) * D_SPEED_MULTIPLIER );

                }
                #endif
            }
        }
        #endif

        float rcpFrameOptZ = lUniforms.mpPerFrame->gFrameBufferSizeVec4.z * lUniforms.mpPerFrame->gTaaSettingsVec4.z;
        float rcpFrameOptW = lUniforms.mpPerFrame->gFrameBufferSizeVec4.w * lUniforms.mpPerFrame->gTaaSettingsVec4.z;

        // X-shaped sample pattern

        float wk = -TAA_GET_LUMINANCE( cM_jittered );

        // four points, plus center, from the current screen
        vec3 cL_M = TAA_MAIN_BUFFER_INPUT( lTexCoords + vec2( -rcpFrameOptZ, -rcpFrameOptW ) );
        wk += TAA_GET_LUMINANCE(cL_M) * 0.25;
        vec3 cMax = cL_M;
        vec3 cMin = cL_M;

        vec3 cR_M = TAA_MAIN_BUFFER_INPUT( lTexCoords + vec2(  rcpFrameOptZ, -rcpFrameOptW ) );
        wk += TAA_GET_LUMINANCE(cR_M) * 0.25;
        cMax = max( cMax, cR_M );
        cMin = min( cMin, cR_M );

        vec3 cU_M = TAA_MAIN_BUFFER_INPUT( lTexCoords + vec2( -rcpFrameOptZ,  rcpFrameOptW ) );
        wk += TAA_GET_LUMINANCE(cU_M) * 0.25;
        cMax = max( cMax, cU_M );
        cMin = min( cMin, cU_M );

        vec3 cD_M = TAA_MAIN_BUFFER_INPUT( lTexCoords + vec2(  rcpFrameOptZ,  rcpFrameOptW ) );
        wk += TAA_GET_LUMINANCE(cD_M) * 0.25;
        cMax = max( cMax, cD_M );
        cMin = min( cMin, cD_M );

        // edge detection filter, produces blend values for the front and back buffers
        // bac buffer is more aggressive than the front buffer
        float kl = ( lUniforms.mpPerFrame->gMBlurSettingsVec4.x );
        float kh = ( lUniforms.mpPerFrame->gMBlurSettingsVec4.y );

        // this math is from the Crytek SMAA filter
        float blendAmountAcc  = 1.0 - saturate( fastRcpNR0( mix(kl, kh, abs( wk ) * 10.0) ) );
        float blendAmountMain = 1.0 - saturate( fastRcpNR0( mix(kl, kh, abs( wk ) * 1.0 ) ) );

        #ifdef D_TERRAIN_MODE

        blendAmountAcc = mix( lUniforms.mpPerFrame->gTaaSettingsVec4.w, lUniforms.mpPerFrame->gTaaSettingsVec4.y, blendAmountAcc);

        #else

        blendAmountAcc = mix( lUniforms.mpPerFrame->gTaaSettingsVec4.w, lUniforms.mpPerFrame->gMBlurSettingsVec4.w, blendAmountAcc);

        #endif

        //blendAmountMain = min( blendAmountMain, get_clip_factor(cM_jittered, cMin, cMax) );

        #ifdef D_NO_BACKBUFFER

        cM = mix( cM, cAcc, blendAmountMain );
        cM = clip_aabb(cM, cMin, cMax);

        #else

        // clip the new value to the AABB _without_ the pixel center
        // this tends to kill alias-y razor lines in the front buffer
        vec3 cM_clip = clip_aabb(cM, cMin, cMax);

        // but for the back buffer value, clip _with_ the pixel center
        // thus allowing bright razor lines that have been smoothed well
        // to come forward, where they can edge things nicely
        vec3 cMin_WithPix = min( cM, cMin );
        vec3 cMax_WithPix = max( cM, cMax );
        vec3 cAcc_clip = clip_aabb(cAcc, cMin_WithPix, cMax_WithPix);

        cM = mix( cM_clip, cAcc_clip, blendAmountMain );

        // idea from staring at Uncharted 4 (before the actual presentation came out)
        // blend _aggressively_ to the backbuffer when change in motion vectors is small 
        //if( !lbForceColorClip )
        {
            cM = mix(cAcc, cM, lbForceColorClip ? 1.0 : lfSpeedBlend);
        }


        cAcc = mix( cAcc, cAcc_clip, saturate( lfSpeedBlend * 2.0 ) );
        cAcc = mix(cM_jittered, cAcc, blendAmountAcc );

        #endif
    }
    else
    {
        cAcc = cM;
    }

    cAcc = max( vec3( 0.0, 0.0, 0.0 ), min( vec3( 10000.0, 10000.0, 10000.0 ), cAcc ) );

    #ifdef D_NO_BACKBUFFER
    cM = max( vec3( 0.0, 0.0, 0.0 ), min( vec3( 10000.0, 10000.0, 10000.0 ), cM ) );
    cAcc = cM;
    #endif

    // debugging
    #ifdef D_SPLITSCREEN_TEST

    //FRAGMENT_COLOUR0 = vec4( vec3( lfSpeedBlend ), 1.0 );

    float lTestBoxesY = lTexCoords.y;
    #ifndef D_PLATFORM_OPENGL
    lTestBoxesY = 1.0 - lTestBoxesY;
    #endif   

    if( lTexCoords.x > 0.66 )
    {

        if( lTestBoxesY > 0.5 )
        {
            FRAGMENT_COLOUR0 = vec4( lLowResMotion.xy, 0.0, 1.0 );
        }
        else
        {
            FRAGMENT_COLOUR0 = vec4( lEncodedDeltaDilated, 0.0, 1.0 );
        }

    }
    else if( lTexCoords.x > 0.33 )
    {
        if( lTestBoxesY > 0.5 )
        {
            FRAGMENT_COLOUR0 = vec4( TAA_MAIN_BUFFER_OUTPUT( cAcc_as_read ), 1.0 );
        }
        else
        {
            FRAGMENT_COLOUR0 = vec4( vec3( lbForceColorClip? 1.0 : 0.0, 0.0, lfSpeedBlend ), 1.0 );
        }
    }
    else
    {
        if( lTestBoxesY > 0.5 )
        {
            FRAGMENT_COLOUR0 = vec4( TAA_MAIN_BUFFER_OUTPUT( cM ), 1.0 );
        }
        else
        {
            FRAGMENT_COLOUR0 = vec4( TAA_MAIN_BUFFER_OUTPUT( cM_jittered ), 1.0 );
        }
    }

    /*

    if( lTexCoords.x > 0.5 )
    {
        if( lTexCoords.y > 0.5 )
        {
        FRAGMENT_COLOUR0 = vec4( TAA_MAIN_BUFFER_OUTPUT( cAcc_NoReproject ), 1.0 );
        }
        else
        {


        //FRAGMENT_COLOUR0 = lReadAccBuffer? vec4( vec3( 1.0 ), 1.0 ) : vec4( vec3( 0.0 ), 1.0 );
         //   FRAGMENT_COLOUR0 =  vec4( vec3( largeVelBlend ), 1.0 );
          // FRAGMENT_COLOUR0 =  vec4( lEncodedDeltaDilated, 0.0, 1.0 );
          FRAGMENT_COLOUR0 =  vec4( lDeltaDilated, 0.0, 1.0 );

                
            if( lTexCoords.y > 0.25 )
            {
                if( lTexCoords.x < 0.75 )
                {
                    FRAGMENT_COLOUR0 = vec4( abs( DecodeMotion( lEncodedDeltaMinDilated ) ) * 20.0, 0.0, 1.0 );
                }
                else
                {
                    FRAGMENT_COLOUR0 = vec4( abs( lDeltaDilated ) * 20.0, 0.0, 1.0 );
                }
            }
            else
            {
                if( lTexCoords.x < 0.75 )
                {
                    FRAGMENT_COLOUR0 = vec4( abs( DecodeMotion( lPrevMinDeltaEncoded ) ) * 20.0, 0.0, 1.0 );
                }
                else
                {
                    FRAGMENT_COLOUR0 = vec4( abs( DecodeMotion( lPrevDeltaEncoded ) ) * 20.0, 0.0, 1.0 );
                }

            } 
        }
    }
    else if( lTexCoords.y > 0.5 )
    {
        FRAGMENT_COLOUR0 = vec4( TAA_MAIN_BUFFER_OUTPUT( cAcc ), 1.0 );
    }
    else
    {
        FRAGMENT_COLOUR0 = vec4( TAA_MAIN_BUFFER_OUTPUT( cM ), 1.0 );
    }
    */

    #else

    FRAGMENT_COLOUR0 = vec4( TAA_MAIN_BUFFER_OUTPUT( cM ), 1.0 );

    #endif


    // output a new value to the accum byffer
    FRAGMENT_COLOUR1 = vec4( TAA_ACC_BUFFER_OUTPUT( cAcc ), 1.0 );

    if( lbForceColorClip )
    {
        FRAGMENT_COLOUR2 = vec4( lEncodedDeltaDilated, 1.0, 1.0 );
    }
    else
    {
        FRAGMENT_COLOUR2 = vec4( lEncodedDeltaDilated, lEncodedDeltaMinDilated );
    }


}

#endif


// =================================================================================================
//
// GAMMACORRECT
//
// =================================================================================================

#ifdef D_POSTPROCESS_GAMMACORRECT

//-----------------------------------------------------------------------------
//      Global Data



//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END

//-----------------------------------------------------------------------------
//      Functions 

FRAGMENT_MAIN_COLOUR_SRT
{
    //Debug
    vec4 lColourVec4 = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), IN(mTexCoordsVec2).xy );

    vec3 lFragCol;

    lFragCol     = lColourVec4.rgb;

    lFragCol.xyz = TonemapKodak(lFragCol) / TonemapKodak( float2vec3(1.0) );

    lFragCol     = GammaCorrectOutput( lFragCol );

    lFragCol = texture3DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gColourLUTBase ), lFragCol, 0.0 ).rgb;

    FRAGMENT_COLOUR = vec4( lFragCol, lColourVec4.a );
}

#endif



// =================================================================================================
//
// GAMMACORRECT
//
// =================================================================================================

#ifdef D_POSTPROCESS_DEGAMMA

//-----------------------------------------------------------------------------
//      Global Data



//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
INPUT_SCREEN_POSITION

INPUT(vec2, mTexCoordsVec2, TEXCOORD0)
DECLARE_END

//-----------------------------------------------------------------------------
//      Functions 

FRAGMENT_MAIN_COLOUR_SRT
{
    #ifdef D_COMPUTE
    vec4 lColourVec4 = FRAGMENT_COLOUR;
    #else
    vec4 lColourVec4 = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), TEX_COORDS.xy);
    #endif
    FRAGMENT_COLOUR = vec4(GammaCorrectInput(lColourVec4.rgb), lColourVec4.a);
}

#endif

// =================================================================================================
//
// CLEAR GBUFFER
//
// =================================================================================================

#ifdef D_POSTPROCESS_CLEAR_GBUFFER

//-----------------------------------------------------------------------------
//      Global Data



//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
INPUT_SCREEN_POSITION

INPUT(vec2, mTexCoordsVec2, TEXCOORD0)
DECLARE_END

//-----------------------------------------------------------------------------
//      Functions 

FRAGMENT_MAIN_COLOUR_SRT
{
    FRAGMENT_COLOUR0 = vec4(0.0, 0.0, 0.0, 0.0);
    FRAGMENT_COLOUR1 = vec4(1.0, 1.0, 1.0, 1.0);
    FRAGMENT_COLOUR2 = vec4(0.0, 0.0, 0.0, 0.0);
    FRAGMENT_COLOUR3 = vec4(0.0, 0.0, 0.0, 0.0);
}

#endif





// =================================================================================================
//
// BILATERAL UPSAMPLE
//
// =================================================================================================

#ifdef D_POSTPROCESS_BILATERAL_UPSAMPLE
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

#ifndef D_PLATFORM_OPENGL
    vec2
    BlurUVOffsets( in const uint luIdx )
    {
        // this form allows the nearest depth index search to compute the offsets without branching.
        return vec2( (luIdx & 1) ? 0.5 : -0.5, (luIdx & 2) ? 0.5 : -0.5 );
    }
#else       
    #define BlurUVOffsets( idx )    ( kaBlurUVOffsets[ ( idx ) ] )
    const vec2 kaBlurUVOffsets[ 4 ] = vec2[ 4 ]( vec2( -0.5, -0.5 ), vec2( 0.5, -0.5 ), vec2( -0.5, 0.5 ), vec2( 0.5, 0.5 ) );
#endif    



#if defined( D_POSTPROCESS_BILATERAL_REDONLY )


vec4
BilateralBlur(
    SAMPLER2DARG( lColourTexture ),
    SAMPLER2DARG( lLowResDepthTexture ),
    SAMPLER2DARG( lHighResDepthTexture ),
    in vec2 lTexCoordsVec2,
    in vec2 lTexSizeVec2,
    in vec2 lRecipTexSizeVec2,
    in vec4 lClipPlanes )
{
    float lafDownsampledDepth[4];
    float lfFullResDepth;
    vec4  lColourReadVec4;
    vec2  lAdjustedTexCoordsVec2;
    float lafBilinearWeights[4];

    lfFullResDepth           = DecodeDepthFromColour( texture2D( lHighResDepthTexture, lTexCoordsVec2 ) );

    vec4 lvRawDepths = textureGatherRed( lLowResDepthTexture, lTexCoordsVec2 );

    lafDownsampledDepth[ 0 ] = DecodeDepthFromColour( float2vec4( lvRawDepths.x ) );
    lafDownsampledDepth[ 1 ] = DecodeDepthFromColour( float2vec4( lvRawDepths.y ) );
    lafDownsampledDepth[ 2 ] = DecodeDepthFromColour( float2vec4( lvRawDepths.z ) );
    lafDownsampledDepth[ 3 ] = DecodeDepthFromColour( float2vec4( lvRawDepths.w ) );

    vec2 lPixelLoc  = lTexCoordsVec2 * lTexSizeVec2 - vec2(0.5, 0.5);
    vec2 lSubPixel = fract( lPixelLoc ); 

    lafBilinearWeights[ 0 ] = ( 1.0 - lSubPixel.x )*(       lSubPixel.y );
    lafBilinearWeights[ 1 ] = (       lSubPixel.x )*(       lSubPixel.y );
    lafBilinearWeights[ 2 ] = (       lSubPixel.x )*( 1.0 - lSubPixel.y );
    lafBilinearWeights[ 3 ] = ( 1.0 - lSubPixel.x )*( 1.0 - lSubPixel.y );

    const float lfUpsampleDepthThreshold = 0.01 * lfFullResDepth;        //  / farplane

    for ( uint i = 0u; i < 4u; i++ )
    {
        float currentDepthDiff    = abs( lafDownsampledDepth[i] - lfFullResDepth );
        bool  lbRejectSample      = currentDepthDiff > lfUpsampleDepthThreshold;

        if( lbRejectSample )
        {
            lafBilinearWeights[ i ] = 0.0;
        }
    }

    float lfTotalWeight = lafBilinearWeights[ 0 ] + lafBilinearWeights[ 1 ] + lafBilinearWeights[ 2 ] + lafBilinearWeights[ 3 ];

    if( lfTotalWeight > 0.0 )
    {
        vec4 lvWeights = vec4( lafBilinearWeights[ 0 ], lafBilinearWeights[ 1 ], lafBilinearWeights[ 2 ], lafBilinearWeights[ 3 ] );
        lvWeights /= lfTotalWeight;

        lColourReadVec4.r = dot( lvWeights, textureGatherRed  ( lColourTexture, lTexCoordsVec2 ) );
        lColourReadVec4.g = dot( lvWeights, textureGatherGreen( lColourTexture, lTexCoordsVec2 ) );
        lColourReadVec4.b = dot( lvWeights, textureGatherBlue ( lColourTexture, lTexCoordsVec2 ) );
        lColourReadVec4.a = dot( lvWeights, textureGatherAlpha( lColourTexture, lTexCoordsVec2 ) );
    }
    else
    {
        lColourReadVec4 = texture2D( lColourTexture, lTexCoordsVec2 );
    }

    lColourReadVec4.a = clamp( lColourReadVec4.a, 0.0, 1.0 );

    return lColourReadVec4;
}

#else

vec4
BilateralBlur(
    SAMPLER2DARG( lColourTexture ),
    SAMPLER2DARG( lLowResDepthTexture ),
    SAMPLER2DARG( lHighResDepthTexture ),
    in vec2 lTexCoordsVec2,
    in vec2 lTexSizeVec2,
    in vec2 lRecipTexSizeVec2,
    in vec4 lClipPlanes )
{
    float lafDownsampledDepth[4];
    float lfFullResDepth;
    vec4  lColourReadVec4;
    vec2  lAdjustedTexCoordsVec2;

    lfFullResDepth           = DecodeDepthFromColour( texture2DLod( lHighResDepthTexture, lTexCoordsVec2, 0.0) );
    lafDownsampledDepth[ 0 ] = DecodeDepthFromColour( texture2DLod( lLowResDepthTexture, lTexCoordsVec2 + ( BlurUVOffsets( 0 ) * lRecipTexSizeVec2 ), 0.0) );
    lafDownsampledDepth[ 1 ] = DecodeDepthFromColour( texture2DLod( lLowResDepthTexture, lTexCoordsVec2 + ( BlurUVOffsets( 1 ) * lRecipTexSizeVec2 ), 0.0) );
    lafDownsampledDepth[ 2 ] = DecodeDepthFromColour( texture2DLod( lLowResDepthTexture, lTexCoordsVec2 + ( BlurUVOffsets( 2 ) * lRecipTexSizeVec2 ), 0.0) );
    lafDownsampledDepth[ 3 ] = DecodeDepthFromColour( texture2DLod( lLowResDepthTexture, lTexCoordsVec2 + ( BlurUVOffsets( 3 ) * lRecipTexSizeVec2 ), 0.0) );

    const float lfUpsampleDepthThreshold = 10.0 * lClipPlanes.w;        //  / 1.0 / farplane

    uint  luNearestDepthIndex = 0u;
    float currentDepthDiff    = abs( lafDownsampledDepth[0] - lfFullResDepth );
    bool  lbRejectSample      = currentDepthDiff < lfUpsampleDepthThreshold;
    float lfMinDepthDiff      = currentDepthDiff;
    
    for ( uint i = 1u; i < 4u; i++ )
    {
        currentDepthDiff = abs( lafDownsampledDepth[i] - lfFullResDepth );
        lbRejectSample   = lbRejectSample && currentDepthDiff < lfUpsampleDepthThreshold;

        if ( currentDepthDiff < lfMinDepthDiff )
        {
            lfMinDepthDiff      = currentDepthDiff;
            luNearestDepthIndex = i;
        }
    }

    // Avoid blocky artifacts using edge detection
    if ( lbRejectSample )
    {
        lAdjustedTexCoordsVec2 = lTexCoordsVec2;
    }
    else
    {
        lAdjustedTexCoordsVec2  = lTexCoordsVec2 + ( BlurUVOffsets( luNearestDepthIndex ) * lRecipTexSizeVec2 );
    }

    lColourReadVec4 = texture2DLod( lColourTexture, lAdjustedTexCoordsVec2, 0.0);

    lColourReadVec4.a = clamp( lColourReadVec4.a, 0.0, 1.0 );

    return lColourReadVec4;
}
#endif
 
FRAGMENT_MAIN_COLOUR_SRT
{
    vec4 lResult = BilateralBlur(    SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBufferMap ),
                                     SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBuffer1Map ),
                                     SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBuffer2Map ),
                                     TEX_COORDS,
                                     lUniforms.mpPerFrame->gFrameBufferSizeVec4.xy * 0.5,
                                     lUniforms.mpPerFrame->gFrameBufferSizeVec4.zw * 2.0,
                                     lUniforms.mpPerFrame->gClipPlanesVec4 );

    #ifdef D_BLEND_SCREEN
    {
        // Screen Overlay Mode for Light Shafts

        float lfLightAccumulation = lResult.r;
        lResult.rgb = mix(lUniforms.mpCustomPerMesh->gLightShaftColourBottomVec4.rgb, lUniforms.mpCustomPerMesh->gLightShaftColourTopVec4.rgb, lfLightAccumulation);

        lResult.rgb *= lfLightAccumulation;

        lResult = lResult * lUniforms.mpCustomPerMesh->gSkyUpperParamsVec4.y;

        vec3 lBase = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBuffer2Map ), TEX_COORDS, 0.0).xyz;
        lResult.xyz = (1.0 - sqrt(lBase.rrr)) * (lResult.xyz);
        lResult.a   = 1.0;
    }
    #endif

#if defined( D_POSTPROCESS_BILATERAL_REDONLY )
    FRAGMENT_COLOUR = vec4(lResult.r, 0.0, 0.0, 1.0);
#else
    #if defined( D_BLEND_ADD ) && defined D_COMPUTE
    vec4 lStoredColour = FRAGMENT_COLOUR;
    lStoredColour.rgb += lResult.rgb;
    FRAGMENT_COLOUR = lStoredColour;
    #elif defined( D_BLEND_BLEND ) && defined D_COMPUTE
    FRAGMENT_COLOUR = FRAGMENT_COLOUR * ( 1.0 - lResult.a ) + lResult * lResult.a;
    #else
    FRAGMENT_COLOUR = lResult;
    #endif
#endif
}

#endif


// =================================================================================================
// 
// D_POSTPROCESS_DEPTH_AWARE_H_GUASS / D_POSTPROCESS_DEPTH_AWARE_V_GUASS
//
// =================================================================================================

#if defined( D_POSTPROCESS_DEPTH_AWARE_H_GUASS ) || defined( D_POSTPROCESS_DEPTH_AWARE_V_GUASS )

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2, TEXCOORD0 )
DECLARE_END


//-----------------------------------------------------------------------------
//      Functions 

#define GaussFilterWeights( idx )       gauss_filter_weights[ ( idx ) ]

// http://dev.theomader.com/gaussian-kernel-calculator/

vec4
GatherGauss(
    SAMPLER2DARG( lColourTexture ),
    SAMPLER2DARG( lDepthTexture ),
    in vec2 lFrameBufferSizeRecipVec2,
    in vec2 blurDirection,
    in vec2 uv,
    in vec4 lClipPlanes )
{    
#ifndef D_PLATFORM_OPENGL
#define NUM_SAMPLES_HALF 4
// sigma 2.4 Kernel Size = 9
    const float gauss_filter_weights[5] = { 0.175713,	0.161305,	0.124789,	0.081355,	0.044695 };
#else
// sigma 2.765 Kernel Size = 15
#define NUM_SAMPLES_HALF 7
    const float gauss_filter_weights[8] = { 0.14446445, 0.13543542, 0.11153505, 0.08055309, 0.05087564, 0.02798160, 0.01332457, 0.00545096 };
#endif
    float centerDepth;
    float BLUR_DEPTH_FALLOFF = lClipPlanes.y * 2.0; // weighting fall off to zero over 50cm
    centerDepth = DecodeDepthFromColour( texture2DLod( lDepthTexture, uv, 0.0) );
    centerDepth *= BLUR_DEPTH_FALLOFF;

    float accumWeights = GaussFilterWeights( 0 );
    vec4  accumResult = texture2DLod(lColourTexture, uv, 0.0) * accumWeights;

    for ( int r = 1; r <= NUM_SAMPLES_HALF; ++r )
    {        
        vec2 uvOffset      = lFrameBufferSizeRecipVec2 * blurDirection * float( r );
        vec4 kernelSample1 = texture2DLod(lColourTexture, uv - uvOffset, 0.0);
        vec4 kernelSample2 = texture2DLod(lColourTexture, uv + uvOffset, 0.0);

        float kernelDepth1 = DecodeDepthFromColour(texture2DLod(lDepthTexture, uv - uvOffset, 0.0)) * BLUR_DEPTH_FALLOFF;
        float kernelDepth2 = DecodeDepthFromColour(texture2DLod(lDepthTexture, uv + uvOffset, 0.0)) * BLUR_DEPTH_FALLOFF;
        
        // Simple depth-aware filtering
        float depthDiff1 = abs(kernelDepth1 - centerDepth);
        float depthDiff2 = abs(kernelDepth2 - centerDepth);

        float g1 = saturate(1.0 - (depthDiff1 * depthDiff1));
        float g2 = saturate(1.0 - (depthDiff2 * depthDiff2));

        float gauss_weight = GaussFilterWeights( r );
        float weight1 = g1 * gauss_weight;
        float weight2 = g2 * gauss_weight;

        accumResult += weight1 * kernelSample1; 
        accumResult += weight2 * kernelSample2;

        accumWeights += weight1;
        accumWeights += weight2;
    }

    return  accumResult / accumWeights;
    //return vec4( accumWeights );
}

FRAGMENT_MAIN_COLOUR_SRT
{
    vec4 lResult = GatherGauss( 
                            SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBufferMap ), 
                            SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBuffer1Map ),
                            lUniforms.mpPerFrame->gFrameBufferSizeVec4.zw,
#if defined( D_POSTPROCESS_DEPTH_AWARE_H_GUASS )
                            vec2(1.0, 0.0),
#else
                            vec2(0.0, 1.0),
#endif
                            TEX_COORDS,
                            lUniforms.mpPerFrame->gClipPlanesVec4 );
#if defined( D_POSTPROCESS_DEPTH_AWARE_REDONLY )
    FRAGMENT_COLOUR = vec4(lResult.r, 0.0, 0.0, 1.0);
#else
    FRAGMENT_COLOUR = lResult;
#endif
}

#endif


#ifdef D_POSTPROCESS_GUASS_SIMPLE

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
INPUT_SCREEN_POSITION

INPUT(vec2, mTexCoordsVec2, TEXCOORD0)
DECLARE_END

//-----------------------------------------------------------------------------
//      Functions 


STATIC_CONST float gOffsets[3] = { 0.0, 1.3846153846, 3.2307692308 };
STATIC_CONST float gWeights[3] = { 0.2270270270, 0.3162162162, 0.0702702703 };

FRAGMENT_MAIN_COLOUR_SRT
{
    vec2 lRecipTexSize = vec2(1.0 / 480.0, 1.0 / 270.0);

    vec2 centreUV = TEX_COORDS;

    vec4 lFragColorVec4 = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), centreUV) * gWeights[0];
    for (int i = 1; i < 3; i++)
    {
        vec4 col;
        vec2 texCoordOffset = lUniforms.mpCustomPerMesh->gBlurParamsVec4.xy * gOffsets[i] * lRecipTexSize;

        col  = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), centreUV + texCoordOffset );
        col += texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), centreUV - texCoordOffset );

        lFragColorVec4 += col * gWeights[i];
    }

    FRAGMENT_COLOUR = vec4(lFragColorVec4);
}
#endif



