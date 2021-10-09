////////////////////////////////////////////////////////////////////////////////
///
///     @file       DoFFragment.h
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


//-----------------------------------------------------------------------------
//      Include files

#ifndef D_FRAGMENT
#define D_FRAGMENT
#endif
#include "Common/Defines.shader.h"
#include "Common/CommonUniforms.shader.h"
#include "Fullscreen/PostCommon.h"
#include "Common/Common.shader.h"
#include "Common/CommonDepth.shader.h"
#include "Common/CommonPostProcess.shader.h"



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
// TONEMAP_COPY
//
// =================================================================================================

#ifdef D_DEPTHOFFIELD_TONEMAP_DEPTH

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
    #ifdef D_DOWNSAMPLE_4

    vec2 lPixelStep = lUniforms.mpPerFrame->gFrameBufferSizeVec4.zw * 0.25;   

    vec2 lTexCoords = IN(mTexCoordsVec2);

    vec2 lTexCoords0 = lTexCoords + vec2( -lPixelStep.x, -lPixelStep.y );
    vec2 lTexCoords1 = lTexCoords + vec2(  lPixelStep.x, -lPixelStep.y );
    vec2 lTexCoords2 = lTexCoords + vec2( -lPixelStep.x,  lPixelStep.y );
    vec2 lTexCoords3 = lTexCoords + vec2(  lPixelStep.x,  lPixelStep.y );

    vec2 lTexCoords4 = lTexCoords + vec2( -lPixelStep.x * 3.0, 0.0 );
    vec2 lTexCoords5 = lTexCoords + vec2(  lPixelStep.x * 3.0, 0.0 );
    vec2 lTexCoords6 = lTexCoords + vec2(  0.0,          -lPixelStep.y * 3.0 );
    vec2 lTexCoords7 = lTexCoords + vec2(  0.0,           lPixelStep.y * 3.0 );

    /*
    vec4 lFragmentColourVec4 = vec4(0,0,0,1);

    vec4 lFragmentColour0Vec4 = texture2D( lUniforms.mpCustomPerMesh->gBufferMap, lTexCoords0 );
    vec4 lFragmentColour1Vec4 = texture2D( lUniforms.mpCustomPerMesh->gBufferMap, lTexCoords1 );
    vec4 lFragmentColour2Vec4 = texture2D( lUniforms.mpCustomPerMesh->gBufferMap, lTexCoords2 );
    vec4 lFragmentColour3Vec4 = texture2D( lUniforms.mpCustomPerMesh->gBufferMap, lTexCoords3 );

    lFragmentColour0Vec4.xyz     = TonemapKodak(lFragmentColour0Vec4.rgb) / TonemapKodak( vec3(1.0,1.0,1.0) );
    lFragmentColour1Vec4.xyz     = TonemapKodak(lFragmentColour1Vec4.rgb) / TonemapKodak( vec3(1.0,1.0,1.0) );
    lFragmentColour2Vec4.xyz     = TonemapKodak(lFragmentColour2Vec4.rgb) / TonemapKodak( vec3(1.0,1.0,1.0) );
    lFragmentColour3Vec4.xyz     = TonemapKodak(lFragmentColour3Vec4.rgb) / TonemapKodak( vec3(1.0,1.0,1.0) );

    lFragmentColourVec4.xyz = 0.25 * ( lFragmentColour0Vec4.xyz + lFragmentColour1Vec4.xyz + lFragmentColour2Vec4.xyz + lFragmentColour3Vec4.xyz );
*/

    vec4 lFragmentColourVec4 = vec4(0,0,0,1); 

    lFragmentColourVec4.xyz += TonemapKodak( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), lTexCoords0 ).xyz );
    lFragmentColourVec4.xyz += TonemapKodak( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), lTexCoords1 ).xyz );
    lFragmentColourVec4.xyz += TonemapKodak( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), lTexCoords2 ).xyz );
    lFragmentColourVec4.xyz += TonemapKodak( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), lTexCoords3 ).xyz );

    lFragmentColourVec4.xyz += TonemapKodak( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), lTexCoords4 ).xyz );
    lFragmentColourVec4.xyz += TonemapKodak( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), lTexCoords5 ).xyz );
    lFragmentColourVec4.xyz += TonemapKodak( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), lTexCoords6 ).xyz );
    lFragmentColourVec4.xyz += TonemapKodak( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), lTexCoords7 ).xyz );

    lFragmentColourVec4.xyz /= TonemapKodak( vec3(1.0,1.0,1.0) );
    lFragmentColourVec4.xyz /= 8.0;

    #elif defined( D_DOWNSAMPLE_16 )

    vec2 lPixelStep = lUniforms.mpPerFrame->gFrameBufferSizeVec4.zw * 0.25;   

    vec2 lTexCoords = IN(mTexCoordsVec2);
    vec2 lTexCoordsStart = lTexCoords + vec2( -lPixelStep.x, -lPixelStep.y ) * 1.5;

    vec4 lFragmentColourVec4 = vec4(0,0,0,1);

    for( float y = 0.0; y < 4.0; y += 1.0 )
    {
        for( float x = 0.0; x < 4.0; x += 1.0 )
        {
            
            vec2 lTexCoords0 = lTexCoordsStart + lPixelStep * vec2( x, y );   
            lFragmentColourVec4.xyz += TonemapKodak( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), lTexCoords0 ).xyz );
        }
    }

    for( float i = -1.0; i <= 1.0; i += 1.0 )
    {
        vec2 lTexCoords0 = lTexCoords + lPixelStep * vec2(  2.5, i );   
        vec2 lTexCoords1 = lTexCoords + lPixelStep * vec2( -2.5, i );   
        vec2 lTexCoords2 = lTexCoords + lPixelStep * vec2( i,  2.5 );   
        vec2 lTexCoords3 = lTexCoords + lPixelStep * vec2( i, -2.5 );   

        lFragmentColourVec4.xyz += TonemapKodak( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), lTexCoords0 ).xyz );
        lFragmentColourVec4.xyz += TonemapKodak( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), lTexCoords1 ).xyz );
        lFragmentColourVec4.xyz += TonemapKodak( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), lTexCoords2 ).xyz );
        lFragmentColourVec4.xyz += TonemapKodak( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), lTexCoords3 ).xyz );
    }


    lFragmentColourVec4.xyz /= TonemapKodak( vec3(1.0,1.0,1.0) );
    lFragmentColourVec4.xyz *= ( 1.0 / 28.0 );

    #else

    vec4 lFragmentColourVec4    = texture2DLod(SAMPLER_GETMAP(lUniforms.mpCustomPerMesh, gBufferMap), TEX_COORDS, 0.0);

    //if (lUniforms.mpCustomPerMesh->gHDRParamsVec4.z == 0.0)        // Only tonemap if we aren't in true HDR mode
    {
        lFragmentColourVec4.xyz     = TonemapKodak(lFragmentColourVec4.rgb) / TonemapKodak( vec3(1.0,1.0,1.0) );
    }
    #endif

    //lFragmentColourVec4.xyz     = GammaCorrectOutput( lFragmentColourVec4.xyz );
    
    FRAGMENT_COLOUR             = vec4( lFragmentColourVec4.xyz, 1.0 );
    //FRAGMENT_COLOUR             = vec4( 1.0, 0.0, 0.0,  1.0 );
}

#endif



// =================================================================================================
//
// BLEND
//
// =================================================================================================

#ifdef D_DEPTHOFFIELD_BLEND

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
    vec4 lFragmentColourVec4;
    lFragmentColourVec4.a   = texture2DLod(SAMPLER_GETMAP(lUniforms.mpCustomPerMesh, gBuffer1Map), TEX_COORDS, 0.0).r;
    if (lFragmentColourVec4.a == 0.0)
    {
        #ifdef D_COMPUTE
        return;
        #else
        discard;
        #endif
    }

    #ifdef D_BLEND_POISSON
    // blend in full when the CoC is two pixels or larger
    float lfSampleCoCSizeInPixels = lFragmentColourVec4.a * lUniforms.mpCustomPerMesh->gBlurParamsVec4.x;
    lFragmentColourVec4.a = saturate( lfSampleCoCSizeInPixels / 2.0 );
    #endif

    lFragmentColourVec4.rgb = texture2DLod(SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), TEX_COORDS, 0.0).rgb;
    #ifdef D_COMPUTE
    #ifdef D_BLEND_INVSRCA
    FRAGMENT_COLOUR = lFragmentColourVec4 + FRAGMENT_COLOUR * ( 1.0 - lFragmentColourVec4.a );
    #else
    FRAGMENT_COLOUR = lFragmentColourVec4 * lFragmentColourVec4.a + FRAGMENT_COLOUR * ( 1.0 - lFragmentColourVec4.a );
    #endif
    #else
    FRAGMENT_COLOUR = lFragmentColourVec4;
    #endif
}

#endif


// =================================================================================================
//
// GUASS_DEPTH
//
// =================================================================================================

#ifdef D_DEPTHOFFIELD_GUASS_DEPTH

#ifdef D_PLATFORM_ORBIS
//#pragma argument(unrollallloops)
#endif


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
    vec4   avgValue = vec4(0.0, 0.0, 0.0, 0.0);
    vec4   lCenter;
    float coefficientSum = 0.0;
    float sigma;


    lCenter.rgb = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ),  TEX_COORDS, 0.0).rgb;
    lCenter.w   = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBuffer1Map ), TEX_COORDS, 0.0).r;

    if ( lCenter.w == 0.0 )
    {
#if defined ( D_GUASS_BLUR_DISCARD )
        #ifdef D_COMPUTE
        return;
        #else
		discard;
        #endif
#else		    	
        FRAGMENT_COLOUR = lCenter;
        return;
#endif              
    }
    
    vec4 lBlurParamsVec4 = lUniforms.mpCustomPerMesh->gBlurParamsVec4;

    // The sigma value for the gaussian function: higher value means more blur
    // A good value for 9x9 is around 3 to 5
    // A good value for 7x7 is around 2.5 to 4
    // A good value for 5x5 is around 2 to 3.5
    // ... play around with this based on what you need :)
    sigma = lBlurParamsVec4.z * lCenter.w * lUniforms.mpCustomPerMesh->gHDRParamsVec4.w;

    incrementalGaussian.x = 1.0 / (sqrt(2.0 * 3.14159265) * sigma);
    incrementalGaussian.y = exp(-0.5 / (sigma * sigma));
    incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;

    // Take the central sample first...
    avgValue.xyz            = lCenter.xyz * incrementalGaussian.x;
    coefficientSum         += incrementalGaussian.x;
    incrementalGaussian.xy *= incrementalGaussian.yz;
    
    // Go through the remaining 8 vertical samples (4 on each side of the center)
    for (float i = 1.0; i <= 8 ; i++) 
    {
        vec4 lTexPos;
        vec4 lTexNeg;

        lTexPos = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), TEX_COORDS.xy + i * (lBlurParamsVec4.xy * lUniforms.mpPerFrame->gFrameBufferSizeVec4.zw), 0.0);
        lTexNeg = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), TEX_COORDS.xy - i * (lBlurParamsVec4.xy * lUniforms.mpPerFrame->gFrameBufferSizeVec4.zw), 0.0);
        
        
        avgValue.xyz += mix( lCenter.xyz, lTexNeg.xyz, lTexNeg.w ) * incrementalGaussian.x;         
        avgValue.xyz += mix( lCenter.xyz, lTexPos.xyz, lTexPos.w ) * incrementalGaussian.x; 
        
        
      //  avgValue.xyz += lTexNeg.xyz * incrementalGaussian.x;         
      //  avgValue.xyz += lTexPos.xyz * incrementalGaussian.x;
               
        coefficientSum         += 2.0 * incrementalGaussian.x;
        incrementalGaussian.xy *= incrementalGaussian.yz;
    }

    FRAGMENT_COLOUR = vec4( avgValue.xyz / coefficientSum, lCenter.w );
}

#endif


// =================================================================================================
//
// GUASS_DEPTH
//
// =================================================================================================

#ifdef D_DEPTHOFFIELD_POISSON

#ifdef D_PLATFORM_ORBIS
//#pragma argument(unrollallloops)
#endif


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

STATIC_CONST vec2 laSamples[45] = 
   { vec2( 0.8754178f,    0.3382074f  ),
     vec2( 0.5275468f,    0.2914778f  ),
     vec2( 0.5049985f,    0.5552282f  ),
     vec2( 0.7118022f,    0.6907374f  ),
     vec2( 0.5725119f,   0.03819193f  ),
     vec2( 0.8589842f,   0.04751138f  ),
     vec2( 0.8518827f,   -0.1962615f  ),
     vec2( 0.4514706f,     -0.19837f  ),
     vec2( 0.1709622f,   -0.2410721f  ),
     vec2( 0.3157806f,   -0.6142841f  ),
     vec2( 0.5513017f,   -0.4672555f  ),
     vec2( 0.1904773f,     0.149644f  ),
     vec2( 0.8132944f,   -0.4643302f  ),
     vec2( 0.1006938f,   -0.4733312f  ),
     vec2( -0.08163998f,  0.4283098f  ),
     vec2( -0.1772187f,  0.07816181f  ),
     vec2( 0.1771218f,    0.4520946f  ),
     vec2( -0.07170758f,  -0.183087f  ),
     vec2( -0.06419997f, -0.6770517f  ),
     vec2( -0.2768793f,  -0.3098489f  ),
     vec2( -0.3298995f,  -0.5832252f  ),
     vec2( 0.08280449f,  -0.9254491f  ),
     vec2( 0.2840817f,     0.674038f  ),
     vec2( -0.007580524f, 0.7572141f  ),
     vec2( -0.2420327f,    0.643879f  ),
     vec2( 0.3602086f,   -0.8637535f  ),
     vec2( -0.458824f,   0.08196559f  ),
     vec2( -0.4708944f,  -0.1652989f  ),
     vec2( -0.3954335f,   0.3991694f  ),
     vec2( 0.6875755f,   -0.6883809f  ),
     vec2( -0.5267925f,  -0.4263104f  ),
     vec2( -0.2631851f,  -0.8735525f  ),
     vec2( -0.5798454f,  -0.8112526f  ),
     vec2( -0.7460729f,   -0.540382f  ),
     vec2( -0.1599769f,   0.9526677f  ),
     vec2( -0.3796223f,    0.848577f  ),
     vec2( -0.5240754f,   0.6447846f  ),
     vec2( 0.08384748f,   0.9811494f  ),
     vec2( -0.6598641f,   0.2601505f  ),
     vec2( -0.8042434f,  0.06738558f  ),
     vec2( -0.7151833f,  -0.1793314f  ),
     vec2( -0.7985358f,   0.5841647f  ),
     vec2( 0.3488737f,    0.9293902f  ),
     vec2( -0.8974248f,   0.3213811f  ),
     vec2( -0.9874689f,   -0.1555209f )  };

/*
STATIC_CONST vec2 laSamples[24] = 
  { vec2(  0.09544561,   0.5344577 ),   
    vec2( -0.1486088,    0.0238491 ),   
    vec2( -0.4032282,    0.5343456 ),   
    vec2(  0.3859451,    0.7922104 ),   
    vec2(  0.4575927,    0.1388439 ),   
    vec2( -0.3071378,    0.9057993 ),   
    vec2(  0.06146561,   0.8775184 ),   
    vec2(  0.7016879,    0.5860649 ),   
    vec2( -0.455029,     0.1312986 ),   
    vec2( -0.8232734,    0.1447836 ),   
    vec2( -0.8279951,    0.5159868 ),   
    vec2( -0.3977252,   -0.3014547 ),   
    vec2( -0.8262658,   -0.3100798 ),   
    vec2(  0.8206691,    0.2426701 ),   
    vec2(  0.8599781,   -0.2079414 ),   
    vec2(  0.5364645,   -0.2567468 ),   
    vec2(  0.1989562,   -0.3102687 ),   
    vec2( -0.3420317,   -0.8343504 ),   
    vec2( -0.6823936,   -0.6383406 ),   
    vec2( -0.02091655,  -0.6036361 ),   
    vec2(  0.3553747,   -0.6527243 ),   
    vec2(  0.1292934,   -0.8970188 ),   
    vec2(  0.1388188,    0.2019703 ),   
    vec2(  0.6832563,   -0.5721989 )  };
    */

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

FRAGMENT_MAIN_COLOUR01_SRT
{    
    vec4   acc = vec4(0.0, 0.0, 0.0, 0.0);
    float  wacc = 0.0;

    vec4   lBaseSample;
    float  lfBaseSampleWeight = 1.0;


    vec4 lBlurParamsVec4 = lUniforms.mpCustomPerMesh->gBlurParamsVec4;

    lBaseSample = vec4(0.0, 0.0, 0.0, 0.0); // texture2D( lUniforms.mpCustomPerMesh->gBufferMap, IN(mTexCoordsVec2) );


    vec2 lMaxMinCoC = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer3Map ), TEX_COORDS, 0.0).rg;

    vec4 lOutColor = lBaseSample;

    if( lMaxMinCoC.x > 0.0 )
    {
        if( lMaxMinCoC.x == lMaxMinCoC.y )
        {
            float lfBaseCoC   = lMaxMinCoC.x;
            float lfBaseDepth = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBuffer2Map ), TEX_COORDS, 0.0).r;

            float lfBaseCoCSize = lfBaseCoC * lBlurParamsVec4.x * 2.0;
            float lfBaseCoCSizeRcp = 1.0 / lfBaseCoCSize;

            float lfSampleCoCSize = lfBaseCoCSize;
            float lfSampleCoCSizeRcp = lfBaseCoCSizeRcp;

            for (int i = 0; i < 45 ; i++) 
            {
                vec2 lSampleOffset = laSamples[ i ] * lBlurParamsVec4.x * ( 0.5 * lMaxMinCoC.x + 0.5 );
                vec2 lSamplePos = TEX_COORDS.xy + lSampleOffset * lUniforms.mpPerFrame->gFrameBufferSizeVec4.zw;

                vec4  lNewSample    = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), lSamplePos, 0.0);

                acc  += lNewSample;  
                wacc += 1.0;
            }

            lOutColor  = acc / wacc;
        }
        else
        {

            float lfBaseCoC   = texture2DLod(SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBuffer1Map ), TEX_COORDS, 0.0).r;
            float lfBaseDepth = texture2DLod(SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBuffer2Map ), TEX_COORDS, 0.0).r;

            float lfBaseCoCSize = lfBaseCoC * lBlurParamsVec4.x * 2.0;
            float lfBaseCoCSizeRcp = 1.0 / lfBaseCoCSize;

        /*
            if ( lBaseSample.w == 0.0 )
            {
        #if defined ( D_GUASS_BLUR_DISCARD )
                discard;
        #else               
                FRAGMENT_COLOUR = lBaseSample;
                return;
        #endif              
            }
            */

            // Go through the remaining 8 vertical samples (4 on each side of the center)
            for (int i = 0; i < 45 ; i++) 
            {
                vec2 lSampleOffset = laSamples[ i ] * lBlurParamsVec4.x * ( 0.5 * lMaxMinCoC.x + 0.5 );
                vec2 lSamplePos = TEX_COORDS.xy + lSampleOffset * lUniforms.mpPerFrame->gFrameBufferSizeVec4.zw;

                float lSampleDist = length(lSampleOffset);

                vec4  lNewSample    = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), lSamplePos, 0.0);
                float lfSampleCoC   = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBuffer1Map ), lSamplePos, 0.0).r;
                float lfSampleDepth = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBuffer2Map ), lSamplePos, 0.0).r;

                float lfSampleCoCSize = lfSampleCoC * lBlurParamsVec4.x * 2.0;
                float lfSampleCoCSizeRcp = 1.0 / lfSampleCoCSize;

                float lfBaseCylWeight   = cylinder( lSampleDist, lfBaseCoCSizeRcp );
                float lfSampleCylWeight = cylinder( lSampleDist, lfSampleCoCSizeRcp );

                float weight = ( lfBaseDepth < lfSampleDepth? lfBaseCylWeight : lfSampleCylWeight );
                weight = saturate( weight );

                // need to work out if the base sample *would* be blended into the far sample
                // (when the far sample does its gather)
                // which we can compute easily by swapping around some of these numbers

                float weightMirror = ( lfBaseDepth < lfSampleDepth? lfSampleCylWeight : lfBaseCylWeight );
                weightMirror = 1.0 - saturate( weightMirror );

                // if the base sample is going to be blended into a far sample,
                // duck it down by an appropriate amount so we don't gain energy
                lfBaseSampleWeight += weightMirror;

                acc  += lNewSample * weight;  
                wacc += weight;
            }

            lOutColor  = lBaseSample * max( 0.0, lfBaseSampleWeight - wacc ) + acc;
            lOutColor /= max( lfBaseSampleWeight, wacc );
        }
    }

    FRAGMENT_COLOUR0 = vec4( lOutColor.xyz, 1.0 );
    FRAGMENT_COLOUR1 = float2vec4( lOutColor.w );
}

#endif


// =================================================================================================
//
// Dilate CoC
//
// =================================================================================================

#ifdef D_DEPTH_DILATE_COC

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

//-----------------------------------------------------------------------------
//      Functions 

FRAGMENT_MAIN_COLOUR_SRT
{         
    vec2 lPixelSize = lUniforms.mpPerFrame->gFrameBufferSizeVec4.zw * D_NEIGHBORHOOD_OVER;

    #ifdef D_DILATE_HORZ
    vec2 lStartUpperCorner = vec2( D_NEIGHBORHOOD_SIZE, D_NEIGHBORHOOD_SIZE * D_BUFFER_SCALING );
    #else
    vec2 lStartUpperCorner = vec2( D_NEIGHBORHOOD_SIZE * D_BUFFER_SCALING, D_NEIGHBORHOOD_SIZE );
    #endif

    // first pixel loc, the upper left 
    vec2 lStartSample = TEX_COORDS - lStartUpperCorner * lPixelSize * 0.5 + lPixelSize * 0.5;
    vec2 lSample = lStartSample;

    float lMaxCoC = 0.0;
    float lMinCoC = 10000.0;

    #ifndef D_DILATE_HORZ
    for( float y = 0; y < D_NEIGHBORHOOD_SIZE; y++ )
    #endif
    {

        #ifndef D_DILATE_VERT
        for( float x = 0; x < D_NEIGHBORHOOD_SIZE; x++ )
        #endif
        {
            #ifdef D_DILATE_HORZ
            float lCoCBuffer = Texture2DNoFiltering( SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBufferMap ), lSample ).r;

            lMaxCoC = max( lMaxCoC, lCoCBuffer );
            lMinCoC = min( lMinCoC, lCoCBuffer );
            #else
            vec2 lCoCBuffer = Texture2DNoFiltering( SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMesh,gBufferMap ), lSample ).rg;

            lMaxCoC = max( lMaxCoC, lCoCBuffer.r );
            lMinCoC = min( lMinCoC, lCoCBuffer.g );
            #endif


            lSample.x += lPixelSize.x;
        }

        lSample.x = lStartSample.x;
        lSample.y += lPixelSize.y;
    }

    FRAGMENT_COLOUR = vec4( lMaxCoC, lMinCoC, 0.0, 1.0 );    
}

#endif

