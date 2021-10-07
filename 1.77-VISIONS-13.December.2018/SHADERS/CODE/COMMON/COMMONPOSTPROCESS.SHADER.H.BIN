////////////////////////////////////////////////////////////////////////////////
///
///     @file       PostProcessFragment.h
///     @author     User
///     @date       
///
///     @brief      PostProcessFragment
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 

#ifndef D_COMMONPOSTPROCESS_H
#define D_COMMONPOSTPROCESS_H

#include "Common/CommonDepth.shader.h"
#include "Common/CommonGBuffer.shader.h"

//-----------------------------------------------------------------------------
//      Include files


//----------------------------------------------------------------------------- 
//      Global Data

STATIC_CONST float kfAvgLumR           = 0.5;
STATIC_CONST float kfAvgLumG           = 0.5;
STATIC_CONST float kfAvgLumB           = 0.5;
STATIC_CONST vec3  lLumCoefficientVec3 = vec3( 0.2125, 0.7154, 0.0721 );

//-----------------------------------------------------------------------------
//    Functions


/*
float A = 0.15;
float B = 0.50;
float C = 0.10;
float D = 0.10;
float E = 0.02;
float F = 0.60;
float W = 32.0;
*/
/*
STATIC_CONST float A = 0.22;
STATIC_CONST float B = 0.30;
STATIC_CONST float C = 0.10;
STATIC_CONST float D = 0.20;
STATIC_CONST float E = 0.01;
STATIC_CONST float F = 0.30;
STATIC_CONST float W = 11.2;
*/

float
KodakFunction(
    in float x )
{
    return (( x * ( 0.22 * x + 0.10 * 0.30 ) + 0.20 * 0.01 ) / ( x * ( 0.22 * x + 0.30 ) + 0.20 * 0.30 ) ) - 0.01 / 0.30;
}

vec3 
TonemapKodakYCgCo(
    in vec3 colIn )
{
    float lumIn = colIn.x;

    float newLum =  KodakFunction( lumIn );
    newLum *= 1.0 / KodakFunction( 1.0 );

    return colIn * float2vec3( max( 0.001, newLum ) / max( 0.001, lumIn ) );
}

vec3 
TonemapKodakLum(
    in vec3 colIn )
{
    float lumIn = 0.25 * ( colIn.r + colIn.b ) + 0.5 * colIn.g;

    float newLum = ( ( lumIn * ( 0.22 * lumIn + 0.10 * 0.30 ) + 0.20 * 0.01 ) / ( lumIn * ( 0.22 * lumIn + 0.30 ) + 0.20 * 0.30 ) ) - 0.01 / 0.30;

    return colIn * float2vec3( max( 0.001, newLum ) / max( 0.001, lumIn ) );
}


#ifdef D_TONEMAP_LUMINANCE_SPACE

vec3 
TonemapKodak(
    in vec3 x )
{
    //return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
    return TonemapKodakLum(x);
}

#else

vec3 
TonemapKodak(
    in vec3 x )
{
    //return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
    return (( x * ( 0.22 * x + 0.10 * 0.30 ) + 0.20 * 0.01 ) / ( x * ( 0.22 * x + 0.30 ) + 0.20 * 0.30 ) ) - 0.01 / 0.30 ;
}

#endif


vec3 
TonemapUDK(
    in vec3 x )
{
    return x / (x + 0.187) * 1.035;;
}

vec3 
TonemapExposure(
    in vec3   lInputVec3,
    in float  lfExposure,
    in float  lfThreshold,
    in float  lfOffset  )
{
    vec3 lResultVec3;

    lResultVec3  = 1.0 - exp2( -lfExposure * lInputVec3.xyz );
    lResultVec3  = max( lResultVec3 - lfThreshold, 0.0 );
    lResultVec3 /= lfOffset + lResultVec3;

    return lResultVec3;
}



//-----------------------------------------------------------------------------
///
///     GetTex2DBilinear
///
///     @brief      GetTex2DBilinear
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec4 
GetTex2DBilinear( 
    SAMPLER2DARG( lTextureMap ), 
    in vec2       lTexCoordsVec2, 
    in vec2       lTexSizeVec2 )
{
    // Bilinear filtering function. Useful when hardware filtering is not available, e.g. for
    // floating point textures on ATI 1xx0 cards
    
    vec2 lCoord0Vec2 = lTexCoordsVec2 - 0.5 / lTexSizeVec2;
    vec2 lCoord1Vec2 = lTexCoordsVec2 + 0.5 / lTexSizeVec2;
    vec2 lWeightVec2 = fract( lCoord0Vec2 * lTexSizeVec2 );
    
    vec4 lBottomVec4 = mix( texture2D( lTextureMap, lCoord0Vec2 ),
                    texture2D( lTextureMap, vec2( lCoord1Vec2.x, lCoord0Vec2.y ) ),
                    lWeightVec2.x );

    vec4 lTopVec4 = mix( texture2D( lTextureMap, vec2( lCoord0Vec2.x, lCoord1Vec2.y ) ),
                    texture2D( lTextureMap, lCoord1Vec2 ),
                    lWeightVec2.x );
    
    return mix( lBottomVec4, lTopVec4, lWeightVec2.y );
}

//-----------------------------------------------------------------------------
///
///     BlurKawase
///
///     @brief      BlurKawase
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec4 
BlurKawase( 
    SAMPLER2DARG( lTextureMap ), 
    in vec2       lTexCoordsVec2, 
    in vec2       lRecipTexSizeVec2, 
    in float      lfIteration )
{
    // Function assumes that tex is using bilinear hardware filtering
    vec2 lUVVec2 = (lfIteration + 0.5) * lRecipTexSizeVec2;
    
    vec4 lColourVec4 = texture2DLod( lTextureMap, lTexCoordsVec2 + vec2( -lUVVec2.x, lUVVec2.y ), 0.0 );	// Top left
    lColourVec4 +=	   texture2DLod( lTextureMap, lTexCoordsVec2 + lUVVec2, 0.0);							// Top right
    lColourVec4 +=	   texture2DLod( lTextureMap, lTexCoordsVec2 + vec2( lUVVec2.x, -lUVVec2.y ), 0.0);	// Bottom right
    lColourVec4 +=     texture2DLod( lTextureMap, lTexCoordsVec2 - lUVVec2, 0.0);							// Bottom left
    
    return lColourVec4 * 0.25;
}

//-----------------------------------------------------------------------------
///
///     RGBToYCgCo
///
///     @brief      RGBToYCgCo
///
///     @param      vec3
///     @return     vec3.
///
//-----------------------------------------------------------------------------
vec3 
RGBToYCgCo(
    vec3 lRgbVec3 )
{
    #if 1

    return lRgbVec3;

    #else
    float i1 = 0.25 * ( lRgbVec3.r + lRgbVec3.b );
    float i2 = 0.5 * lRgbVec3.g;

    float luminance = i1 + i2;
    float Cg = i2 - i1;
    float Co = 0.5 * ( lRgbVec3.r - lRgbVec3.b );

    //Cg += 0.5;
    //Co += 0.5;

    return vec3( luminance, Cg, Co );
    #endif
}

//-----------------------------------------------------------------------------
///
///     YCgCoToRGB
///
///     @brief      YCgCoToRGB
///
///     @param      vec3
///     @return     vec3.
///
//-----------------------------------------------------------------------------
vec3 
YCgCoToRGB(
    vec3 lYCgCoVec3 )
{
    #if 1

    return lYCgCoVec3;

    #else
    float Y = lYCgCoVec3.r;
    float Cg = lYCgCoVec3.g; // - 0.5;
    float Co = lYCgCoVec3.b; // - 0.5;

    float tmp = Y - Cg;
    float r = tmp + Co;
    float g = Y + Cg;
    float b = tmp - Co;

    return vec3( r, g, b );
    #endif
}


//-----------------------------------------------------------------------------
///
///     DecodeMotion
///
//-----------------------------------------------------------------------------
vec2
DecodeMotion(
    in vec2 lBuffer )
{

    {
        vec2 lMotionScaled = ( lBuffer - 127.0 / 255.0 );
        // length needs to be squared
        lMotionScaled *= length( lMotionScaled );

        #ifndef D_MOTION_VECTORS_ALREADY_RESOLVED
        lMotionScaled = ( lBuffer.x == 1.0 )? vec2(0,0) : lMotionScaled;
        #endif

        return lMotionScaled * float2vec2( 0.25 );
    }
}


float
ComputeMotionInverseSpeed(
    in vec2 lBuffer )
{
    {
        vec2 lMotionScaled = ( lBuffer - 127.0 / 255.0 );

        // lMotionScaled *= length( lMotionScaled );
        // lMotionScaled *= 0.25;

        // return 1.0 / ( 0.25 * dot( lMotionScaled, lMotionScaled ) );
        return 4.0 / ( dot( lMotionScaled, lMotionScaled ) );
    }
}

float
ComputeMotionInverseSpeedSq(
    in vec2 lBuffer )
{
    {
        return ComputeMotionInverseSpeed(lBuffer) * ComputeMotionInverseSpeed(lBuffer);
    }
}


vec2
GetDejitteredTexCoord(
    in vec2 lTexCoord,
    in vec4 lDejitterVec4 )
{
    return lTexCoord + DSCREENSPACE_AS_RENDERTARGET_UVS(lDejitterVec4.xy);
}

#ifdef D_PLATFORM_ORBIS

vec4 
Texture2DNoFiltering(
    SAMPLER2DARG( tex ),
    vec2 texcoord )
{
    // Setup the sampler descriptor
    sce::Gnm::Sampler sampler;
        sampler.init();
        sampler.setWrapMode(sce::Gnm::kWrapModeClampLastTexel, sce::Gnm::kWrapModeClampLastTexel, sce::Gnm::kWrapModeClampLastTexel );
        sampler.setXyFilterMode(sce::Gnm::kFilterModePoint, sce::Gnm::kFilterModePoint);
        sampler.setMipFilterMode(sce::Gnm::kMipFilterModeNone);

    // Create resource from descriptor
    SamplerState samp = SamplerState(sampler);
 
    vec4 color = tex.SampleLOD(samp, texcoord, 0.0); 
    return color;
}

#elif defined(D_PLATFORM_DX12)

vec4
Texture2DNoFiltering(
    SAMPLER2DARG(tex),
    vec2 texcoord)
{
    return texture2DLod(tex, texcoord, 0.0);
}

#else

vec4 
Texture2DNoFiltering(
    SAMPLER2DARG( tex ),
    in vec2 texcoord )
{
    return texture2D( tex, texcoord );
}

#endif

//-----------------------------------------------------------------------------
///
///     GetPrevPosition
///
///     @brief      GetPrevPosition
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec2 GetPrevPosition(
    in vec2 lFragCoordsVec2,
    in vec4 lClipPlanesVec4,
    in mat4 lInverseProjectionMat4,
    in mat4 lInverseViewMat4,
    in mat4 lViewProjectionMat4,
    in mat4 lPrevViewProjectionMat4,
    in vec3 lViewPositionVec3,
    in vec4 lMBlurSettingsVec4,
    SAMPLER2DARG( lMotionMap ),
    SAMPLER2DARG( lDepthMap ),
    out float lfOutDepth,
    out float lfOutDepthNormalised,
    out float lfOutRcpSpeed,
    out vec2 lOutDelta,
    out vec2 lOutEncodedDelta,
    out bool lWantsColorClipAA )
{
    vec2 lReprojectFragCoordsVec2;
    vec4 lMotionBuffer = Texture2DNoFiltering( SAMPLER2DPARAM( lMotionMap ), lFragCoordsVec2 );
    vec2 lDelta = lMotionBuffer.xy;

    #ifndef D_MOTION_BUFFER_HAS_DEPTH
        float lfDepth = DecodeDepthFromColour( Texture2DNoFiltering(SAMPLER2DPARAM( lDepthMap ), lFragCoordsVec2 ) );
        lfOutDepthNormalised = lfDepth;
        lfDepth = FastDenormaliseDepth( lClipPlanesVec4, lfDepth );
        lfOutDepth = lfDepth;
    #else
        lfOutDepth = FastDenormaliseDepth( lClipPlanesVec4, lMotionBuffer.z ); 
        lfOutDepthNormalised = lMotionBuffer.z;
        lfOutRcpSpeed = lMotionBuffer.w;
    #endif

    #ifndef D_MOTION_VECTORS_ALREADY_RESOLVED
    if( lDelta.x < 1.0 )
    #endif
    {  
        // motion already computed by a friendly object shader
        lOutEncodedDelta = lDelta * 0.5;

        #ifndef D_MOTION_BUFFER_HAS_DEPTH
        lfOutRcpSpeed = ComputeMotionInverseSpeed( lDelta ) * ( 1.0 / lMBlurSettingsVec4.z );
        #endif

        lDelta = DecodeMotion( lDelta );

        lReprojectFragCoordsVec2 = lFragCoordsVec2 + lDelta;

        lOutDelta = lDelta;
        lWantsColorClipAA = false;

        return lReprojectFragCoordsVec2;
    }

    #ifndef D_MOTION_VECTORS_ALREADY_RESOLVED

    // use gbuffer depth to reconstruct position in homogeneous clip space
    // THIS IS TERRIBLE

    vec4 lPositionVec4;
    lPositionVec4.xy = SCREENSPACE_AS_RENDERTARGET_UVS(lFragCoordsVec2.xy) * 2.0 - 1.0;
    lPositionVec4.z = 0.0;
    lPositionVec4.w = 1.0;

    // Inverse projection
    lPositionVec4        = MUL( lInverseProjectionMat4, lPositionVec4 );
    //lPositionVec4        = lPositionVec4 / lPositionVec4.w;
    lPositionVec4        = lPositionVec4 / abs(lPositionVec4.z);
    lPositionVec4        = lPositionVec4 * lfDepth;

    // Inverse view
    mat4 lViewMat   = lInverseViewMat4;
    MAT4_SET_POS( lViewMat, vec4( 0.0, 0.0, 0.0, 1.0 ) );
    lPositionVec4   = MUL( lViewMat, lPositionVec4 );
    //lPositionVec4   = lPositionVec4 / lPositionVec4.w;

    //lPositionVec4.xyz   -= lViewPosition;

    vec3 lViewVectorVec3 = ( lPositionVec4.xyz );
    lPositionVec4.xyz    = lViewVectorVec3 + lViewPositionVec3;
    lPositionVec4.w      = 1.0;

    lPositionVec4 = MUL( lPrevViewProjectionMat4, lPositionVec4 ); 




    // now finally after all that we have the position in homogeneous clip space
    //lPositionVec4 = MUL( lCameraDeltaMat4, lPositionVec4 );

    #ifndef D_PLATFORM_OPENGL
        lPositionVec4.y = -lPositionVec4.y;
    #endif

    lPositionVec4.xyz /= lPositionVec4.w;
    lReprojectFragCoordsVec2 = lPositionVec4.xy;
    lReprojectFragCoordsVec2 = lReprojectFragCoordsVec2 * 0.5 + 0.5;

    lDelta = lReprojectFragCoordsVec2 - lFragCoordsVec2;
    lfOutRcpSpeed = 1.0 / ( length( lDelta ) * lMBlurSettingsVec4.z );
    lOutDelta = lDelta;

    vec2 lDeltaFlipped = lDelta;
    #ifndef D_PLATFORM_OPENGL
        lDeltaFlipped.y = -lDeltaFlipped.y;
    #endif

    lOutEncodedDelta = EncodeMotion( lDeltaFlipped ) * 0.5;

    lWantsColorClipAA = ( lMotionBuffer.y > 0.75 );

    return lReprojectFragCoordsVec2;

    #endif
}

/*
//-----------------------------------------------------------------------------
///
///     RGBToHSL
///
///     @brief      RGBToHSL
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec3 
RGBToHSL(
    vec3 lColourVec3 )
{
    vec3 lHSLVec3 = vec3( 0.0, 0.0, 0.0 ); // init to 0 to avoid warnings ? (and reverse if + remove first part)
    
    float lfMin = min(min(lColourVec3.r, lColourVec3.g), lColourVec3.b);    //Min. value of RGB
    float lfMax = max(max(lColourVec3.r, lColourVec3.g), lColourVec3.b);    //Max. value of RGB
    float lfDelta = lfMax - lfMin;             //Delta RGB value

    lHSLVec3.z = (lfMax + lfMin) / 2.0; // Luminance

    if (lfDelta == 0.0)		//This is a gray, no chroma...
    {
        lHSLVec3.x = 0.0;	// Hue
        lHSLVec3.y = 0.0;	// Saturation
    }
    else                                    //Chromatic data...
    {
        if (lHSLVec3.z < 0.5)
            lHSLVec3.y = lfDelta / (lfMax + lfMin); // Saturation
        else
            lHSLVec3.y = lfDelta / (2.0 - lfMax - lfMin); // Saturation
        
        float lfDeltaR = (((lfMax - lColourVec3.r) / 6.0) + (lfDelta / 2.0)) / lfDelta;
        float lfDeltaG = (((lfMax - lColourVec3.g) / 6.0) + (lfDelta / 2.0)) / lfDelta;
        float lfDeltaB = (((lfMax - lColourVec3.b) / 6.0) + (lfDelta / 2.0)) / lfDelta;

        if (lColourVec3.r == lfMax )
            lHSLVec3.x = lfDeltaB - lfDeltaG; // Hue
        else if (lColourVec3.g == lfMax)
            lHSLVec3.x = (1.0 / 3.0) + lfDeltaR - lfDeltaB; // Hue
        else if (lColourVec3.b == lfMax)
            lHSLVec3.x = (2.0 / 3.0) + lfDeltaG - lfDeltaR; // Hue

        if (lHSLVec3.x < 0.0)
            lHSLVec3.x += 1.0; // Hue
        else if (lHSLVec3.x > 1.0)
            lHSLVec3.x -= 1.0; // Hue
    }

    return lHSLVec3;
}

//-----------------------------------------------------------------------------
///
///     HueToRGB
///
///     @brief      HueToRGB
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
float 
HueToRGB(
    float lf1, 
    float lf2, 
    float lfHue)
{
    if (lfHue < 0.0)
        lfHue += 1.0;
    else if (lfHue > 1.0)
        lfHue -= 1.0;
    float res;
    if ((6.0 * lfHue) < 1.0)
        res = lf1 + (lf2 - lf1) * 6.0 * lfHue;
    else if ((2.0 * lfHue) < 1.0)
        res = lf2;
    else if ((3.0 * lfHue) < 2.0)
        res = lf1 + (lf2 - lf1) * ((2.0 / 3.0) - lfHue) * 6.0;
    else
        res = lf1;
    return res;
}

//-----------------------------------------------------------------------------
///
///     HSLToRGB
///
///     @brief      HSLToRGB
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec3 
HSLToRGB(
    vec3 lHSLVec3 )
{
    vec3 lRGBVec3;
    
    if (lHSLVec3.y == 0.0)
        lRGBVec3 = vec3(lHSLVec3.z, lHSLVec3.z, lHSLVec3.z); // Luminance
    else
    {
        float lf2;
        
        if (lHSLVec3.z < 0.5)
            lf2 = lHSLVec3.z * (1.0 + lHSLVec3.y);
        else
            lf2 = (lHSLVec3.z + lHSLVec3.y) - (lHSLVec3.y * lHSLVec3.z);
            
        float lf1 = 2.0 * lHSLVec3.z - lf2;
        
        lRGBVec3.r = HueToRGB(lf1, lf2, lHSLVec3.x + (1.0/3.0));
        lRGBVec3.g = HueToRGB(lf1, lf2, lHSLVec3.x);
        lRGBVec3.b= HueToRGB(lf1, lf2, lHSLVec3.x - (1.0/3.0));
    }
    
    return lRGBVec3;
}

//-----------------------------------------------------------------------------
///
///     ContrastSaturationBrightness
///
///     @brief      ContrastSaturationBrightness
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
// For all settings: 1.0 = 100% 0.5=50% 1.5 = 150%
vec3 
ContrastSaturationBrightness(
    vec3   lColourVec3, 
    float lfBRT, 
    float lfSaturation, 
    float lfCon )
{
    float              lfDot;

    // Increase or decrease these values to adjust r, g and b color channels separately
    vec3 lAverageLuminosityVec3 = vec3(kfAvgLumR, kfAvgLumG, kfAvgLumB);

    vec3 lBRTColourVec3 = lColourVec3 * lfBRT;

    lfDot = dot(lBRTColourVec3, lLumCoefficientVec3);
    vec3 lIntensityVec3 = vec3( lfDot, lfDot, lfDot );
    vec3 lSaturationColourVec3 = mix(lIntensityVec3, lBRTColourVec3, lfSaturation);
    vec3 lConColorVec3 = mix(lAverageLuminosityVec3, lSaturationColourVec3, lfCon);
    return lConColorVec3;

}*/

//-----------------------------------------------------------------------------
///
///     GetBulgeDeform
///
///     @brief      GetBulgeDeform
///
///     @param      vec2 lCoordsVec2
///     @return     vec2
///
//-----------------------------------------------------------------------------
vec2 
GetBulgeDeform(
    vec2 lCoordsVec2 )
{
    // get the right pixel for the current position
    vec2 lBulgeCoordsVec2;

    // put into -1 -> 1 space
    lBulgeCoordsVec2   = lCoordsVec2*2.0 - 1.0;

    lBulgeCoordsVec2.y = lBulgeCoordsVec2.y * mix( 1.0, 1.1, sin(0.5*3.1416*(lBulgeCoordsVec2.x+1.0)) );
    lBulgeCoordsVec2.y -= 0.03;

    // put back into 0 -> 1 space
    lBulgeCoordsVec2 = (lBulgeCoordsVec2 + 1.0)*0.5;

    return lBulgeCoordsVec2;
}



//-----------------------------------------------------------------------------
///
///     GetDofPower
/// 
///     @brief      Works out the DOF power to pass to DOF
///
//-----------------------------------------------------------------------------
float GetDofPower(
    vec2 lTexCoordsVec2,
    SAMPLER2DARG( lDepthMap ),
    SAMPLER2DARG( lBlurMask ),
    vec4 lDofParamsVec4,
    vec4 lClipPlanesVec4 )
{
    float lfPower = 0.0;

    if( lDofParamsVec4.x != 0.0 ) // DOF amount
    {
        vec4 lBuffer1ColourVec4 = texture2DLod( lDepthMap, lTexCoordsVec2, 0.0);
        //FRAGMENT_DEPTH              = lBuffer1ColourVec4.x;

        vec3 lDofBlurCol;

        float lfLinearZ = FastDenormaliseDepth( lClipPlanesVec4, DecodeDepthFromColour( lBuffer1ColourVec4 ) );

        /*
        float lfNonLinearZ = lBuffer1ColourVec4.x;

        float lfLinearZ = ReverseZToLinearDepth( lClipPlanesVec4, lfNonLinearZ);
        */

        vec2 lDistMaskedVec2;

        float lfNearPlane = lDofParamsVec4.z;
        lDistMaskedVec2.x = smoothstep( lfNearPlane, lfNearPlane - 1.0, lfLinearZ ); // in this case edge0 > edge1 - glsl spec says this gives undefined behaviour... is this line correct?
        lDistMaskedVec2.y = smoothstep( lDofParamsVec4.y, lDofParamsVec4.y + lDofParamsVec4.w, lfLinearZ );

        lfPower = max( lDistMaskedVec2.x, lDistMaskedVec2.y );

        //// Blur Mask
        vec2 lDeformedCoordsVec2 = GetBulgeDeform( lTexCoordsVec2 );
        vec4 lBlurMaskVec4Warped = texture2DLod( lBlurMask, lDeformedCoordsVec2, 0.0);// texture2D( gBlurMask, lDeformedCoordsVec2 );
        vec4 lBlurMaskVec4 = texture2DLod( lBlurMask, lTexCoordsVec2, 0.0);
        lfPower = mix( 0.0, lfPower, lDofParamsVec4.x );
        lfPower = mix( lfPower, 1.0, max( lBlurMaskVec4Warped.r, lBlurMaskVec4.g ) );
    }

    return lfPower;
}


vec2 DebandNrand(vec2 n)
{
    vec2 result;
    result.x = fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
    result.y = fract(cos(mod(123456789.0, 1e-7 + 256. * dot(vec2(23.1406926327792690, 2.6651441426902251), n))));
    return result;
}

vec3 DebandFilter(
    in PerFrameUniforms        lRenderTargetUniforms,
    in vec3                    lFragCol,
    in vec2                    lTexCoords,
    SAMPLER2DARG(lDepthMap),
    SAMPLER2DARG(lColourMap)
    )
{
    float lfDepth = texture2DLod(lDepthMap, lTexCoords, 0.0).x;
    
    if (lfDepth < 0.00000001)
    {
        vec2 invViewportSize = lRenderTargetUniforms.gFrameBufferSizeVec4.zw;
        vec2 rand = DebandNrand(lTexCoords + fract(lRenderTargetUniforms.gfTime));
        vec2 offsetCoords = lTexCoords + (((rand * vec2(31.0, 31.0)) - 15.0) * invViewportSize.xy);
        lfDepth = texture2DLod(lDepthMap, offsetCoords, 0.0).x;
        if (lfDepth < 0.00000001)
        {
            vec3 lFragColRandomTap = texture2DLod(lColourMap, offsetCoords, 0.0).xyz;   //GammaCorrectInput
            vec3 diff = abs(lFragCol*lFragCol - lFragColRandomTap* lFragColRandomTap);
            lFragCol = ((diff.x + diff.y + diff.z) < (14.0 / 255.0)) ? lFragColRandomTap : lFragCol;
        }
    }
    
    return lFragCol;
}

#ifdef D_LUT

vec3
ApplyColourLUT(
    in PerFrameUniforms      lPerFrameUniforms,
    in CustomPerMeshUniforms lPerMeshUniforms,
    in vec3                  lFragmentColour,
    in float                 lfDepth,
	in float				 lfLUTEffectAmount )
{
    float lfNormalisedDepth = 0.0;
    
    lfNormalisedDepth = ReverseZToLinearDepth( lPerFrameUniforms.gClipPlanesVec4, lfDepth );

    vec3 lFragColBase   = texture3DLod( SAMPLER_GETLOCAL( lPerMeshUniforms, gColourLUTBase ),   lFragmentColour, 0.0 ).rgb;
    vec3 lFragColFar    = texture3DLod( SAMPLER_GETLOCAL( lPerMeshUniforms, gColourLUTFar ),    lFragmentColour, 0.0 ).rgb;
    vec3 lFragColStorm  = texture3DLod( SAMPLER_GETLOCAL( lPerMeshUniforms, gColourLUTStorm ),  lFragmentColour, 0.0 ).rgb;
    vec3 lFragColEffect = texture3DLod( SAMPLER_GETLOCAL( lPerMeshUniforms, gColourLUTEffect ), lFragmentColour, 0.0 ).rgb;

    lFragColFar    = mix( lFragColBase, lFragColFar,    saturate( lPerMeshUniforms.gColourLUTParamsVec4.x * lfNormalisedDepth ) );
    lFragColStorm  = mix( lFragColBase, lFragColStorm,  saturate( lPerMeshUniforms.gColourLUTParamsVec4.y * lfNormalisedDepth ) );
    

	
	lFragColEffect = mix( lFragColBase, lFragColEffect, saturate( lPerMeshUniforms.gColourLUTParamsVec4.z * lfNormalisedDepth ) );
    lFragColEffect = mix( lFragColBase, lFragColEffect, ( 1.0 - lfLUTEffectAmount ) );

    lFragmentColour = vec3(0.0, 0.0, 0.0);
    lFragmentColour += lFragColFar * lPerMeshUniforms.gColourLUTStrengthsVec4.x;
    lFragmentColour += lFragColStorm * lPerMeshUniforms.gColourLUTStrengthsVec4.y;
    lFragmentColour += lFragColEffect * lPerMeshUniforms.gColourLUTStrengthsVec4.z;

    return lFragmentColour;
}

#endif

#endif

