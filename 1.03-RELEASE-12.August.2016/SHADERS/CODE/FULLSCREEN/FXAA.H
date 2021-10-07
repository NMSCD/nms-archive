/*============================================================================


                     NVIDIA FXAA 3.9 by TIMOTHY LOTTES

------------------------------------------------------------------------------
                            INTEGRATION CHECKLIST
------------------------------------------------------------------------------
(1.) 
In the shader source,
setup defines for the desired configuration. 
Example,

  #define FXAA_PC 1
  #define FXAA_HLSL_3 1

(2.) 
Then include this file,

(3.) 
Then call the FXAA pixel shader from within your desired shader,

  return FxaaPixelShader(pos, posPos, tex, rcpFrame, rcpFrameOpt);

(4.) 
Insure pass prior to FXAA outputs RGBL.
See next section.

(5.) 
Setup engine to provide "rcpFrame" and "rcpFrameOpt" constants.
Not using constants will result in a performance loss.

  // {x_} = 1.0/screenWidthInPixels
  // {_y} = 1.0/screenHeightInPixels
  vec2 rcpFrame

  // This must be from a constant/UNIFORM.
  // {x___} = 2.0/screenWidthInPixels
  // {_y__} = 2.0/screenHeightInPixels
  // {__z_} = 0.5/screenWidthInPixels
  // {___w} = 0.5/screenHeightInPixels
  vec4 rcpFrameOpt 
    
(6.) 
Have FXAA vertex shader run as a full screen triangle,
and output "pos" and "posPos" such that inputs in the pixel shader provide,
  
  // {xy} = center of pixel
  vec2 pos,
    
  // {xy__} = upper left of pixel
  // {__zw} = lower right of pixel
  vec4 posPos,

(7.) 
Insure the texture sampler used by FXAA is set to bilinear filtering.


------------------------------------------------------------------------------
                     INTEGRATION - RGBL AND COLORSPACE
------------------------------------------------------------------------------
FXAA3 requires RGBL as input.

RGB should be LDR (low dynamic range). 
Specifically do FXAA after tonemapping.

RGB data as returned by a texture fetch can be linear or non-linear.
Note an "sRGB format" texture counts as linear, 
because the result of a texture fetch is linear data.
Regular "RGBA8" textures in the sRGB colorspace are non-linear.

Luma must be stored in the alpha channel prior to running FXAA.
This luma should be in a perceptual space (could be gamma 2.0).
Example pass before FXAA where output is gamma 2.0 encoded,

  color.rgb = ToneMap(color.rgb); // linear color output
  color.rgb = sqrt(color.rgb);    // gamma 2.0 color output
  return color;

To use FXAA,

  color.rgb = ToneMap(color.rgb);  // linear color output
  color.rgb = sqrt(color.rgb);     // gamma 2.0 color output
  color.a = dot(color.rgb, vec3(0.299, 0.587, 0.114)); // compute luma
  return color;

Another example where output is linear encoded,
say for instance writing to an sRGB formated render target,
where the render target does the conversion back to sRGB after blending,

  color.rgb = ToneMap(color.rgb); // linear color output
  return color;

To use FXAA,

  color.rgb = ToneMap(color.rgb); // linear color output
  color.a = sqrt(dot(color.rgb, vec3(0.299, 0.587, 0.114))); // compute luma
  return color;

Getting luma correct is required for the algorithm to work correctly.

------------------------------------------------------------------------------
                            COMPLEX INTEGRATION
------------------------------------------------------------------------------
Q. What if the engine is blending into RGB before wanting to run FXAA?

A. In the last opaque pass prior to FXAA,
   have the pass write out luma into alpha.
   Then blend into RGB only.
   FXAA should be able to run ok 
   assuming the blending pass did not any add aliasing.
   This should be the common case for particles and common blending passes.

============================================================================*/

/*============================================================================

                            INTEGRATION KNOBS


/*--------------------------------------------------------------------------*/

#define FXAA_DISCARD 0
    // 
    // Only valid for PC OpenGL currently.
    // 
    // 1 = Use discard on pixels which don't need AA.
    //     For APIs which enable concurrent TEX+ROP from same surface.
    // 0 = Return unchanged color on pixels which don't need AA.
    // 
  

/*--------------------------------------------------------------------------*/

//
// 1 = API supports gather4 on alpha channel.
// 0 = API does not support gather4 on alpha channel.
//
#ifdef GL_ARB_gpu_shader5
    #define FXAA_GATHER4_ALPHA 1
#endif
#ifdef GL_NV_gpu_shader5
    #define FXAA_GATHER4_ALPHA 1
#endif
#if defined ( D_PLATFORM_ORBIS )
#define FXAA_GATHER4_ALPHA 1
#endif

#ifndef FXAA_GATHER4_ALPHA
    #define FXAA_GATHER4_ALPHA 0
#endif

/*============================================================================
                         FXAA QUALITY - TUNING KNOBS
============================================================================*/

//
// The minimum amount of local contrast required to apply algorithm.
//
// 1/3 - too little
// 1/4 - low quality
// 1/6 - default
// 1/8 - high quality
// 1/16 - overkill
//
#define FXAA_QUALITY__EDGE_THRESHOLD (1.0/8.0)

/*--------------------------------------------------------------------------*/

//
// Trims the algorithm from processing darks.
//
// 1/32 - visible limit
// 1/16 - high quality
// 1/12 - upper limit (default, the start of visible unfiltered edges)
//
#define FXAA_QUALITY__EDGE_THRESHOLD_MIN (1.0/12.0)

/*--------------------------------------------------------------------------*/

//
// Choose the amount of sub-pixel aliasing removal.
//
// 1   - upper limit (softer)
// 3/4 - default amount of filtering
// 1/2 - lower limit (sharper, less sub-pixel aliasing removal)
//
//

//#define FXAA_QUALITY__SUBPIX ( 3.0 / 4.0 )
#define FXAA_QUALITY__SUBPIX 1.0


/*============================================================================

                                 API PORTING
                                 
============================================================================*/

#define FxaaTexTop(t, p) texture2DLod(t, p, 0.0)

#if defined ( D_PLATFORM_ORBIS )
    #define FxaaTexOff(t, p, o, r) FxaaTexTop( t, p + vec2( o ) * r )
#else
    #define FxaaTexOff(t, p, o, r) textureLodOffset(t, p, 0.0, o)
#endif

#if (FXAA_GATHER4_ALPHA == 1)
#if defined ( D_PLATFORM_ORBIS )
    #define FxaaTexAlpha4(T, p, r) 		 T.GatherAlpha( T##SS, p )
    #define FxaaTexOffAlpha4(T, p, o, r) T.GatherAlpha( T##SS, p, o )
#else
    // use #extension GL_ARB_gpu_shader5 : enable
    #define FxaaTexAlpha4(t, p, r)       textureGather(t, p, 3)
    #define FxaaTexOffAlpha4(t, p, o, r) textureGatherOffset(t, p, o, 3)
#endif    
#endif        


/*============================================================================

                              FXAA3 QUALITY - PC

============================================================================*/

/*--------------------------------------------------------------------------*/
vec4 FxaaPixelShader(
    // {xy} = center of pixel
    vec2 pos,
    // {xyzw} = not used on FXAA3 Quality
    vec4 posPos,       
    // {rgb_} = color in linear or perceptual color space
    // {___a} = luma in perceptual color space (not linear) 
    SAMPLER2DARG( tex ),
    // This must be from a constant/UNIFORM.
    // {x_} = 1.0/screenWidthInPixels
    // {_y} = 1.0/screenHeightInPixels
    vec2 rcpFrame,
    // {xyzw} = not used on FXAA3 Quality
    vec4 rcpFrameOpt ) 
{   
/*--------------------------------------------------------------------------*/
    vec2 posM;
    posM.x = pos.x;
    posM.y = pos.y;
    #if (FXAA_GATHER4_ALPHA == 1)
        #if (FXAA_DISCARD == 0)
            vec4 rgbyM = FxaaTexTop(tex, posM);
            #define lumaM rgbyM.w
        #endif            
        vec4 luma4A = FxaaTexAlpha4(tex, posM, rcpFrame.xy);
        vec4 luma4B = FxaaTexOffAlpha4(tex, posM, ivec2(-1, -1), rcpFrame.xy);
        #if (FXAA_DISCARD == 1)
            #define lumaM luma4A.w
        #endif
        #define lumaE luma4A.z
        #define lumaS luma4A.x
        #define lumaSE luma4A.y
        #define lumaNW luma4B.w
        #define lumaN luma4B.z
        #define lumaW luma4B.x
    #else
        vec4 rgbyM = FxaaTexTop(tex, posM);
        #define lumaM rgbyM.w
        float lumaS = FxaaTexOff(tex, posM, ivec2( 0, 1), rcpFrame.xy).w;
        float lumaE = FxaaTexOff(tex, posM, ivec2( 1, 0), rcpFrame.xy).w;
        float lumaN = FxaaTexOff(tex, posM, ivec2( 0,-1), rcpFrame.xy).w;
        float lumaW = FxaaTexOff(tex, posM, ivec2(-1, 0), rcpFrame.xy).w;
    #endif
/*--------------------------------------------------------------------------*/
    float maxSM = max(lumaS, lumaM);
    float minSM = min(lumaS, lumaM);
    float maxESM = max(lumaE, maxSM); 
    float minESM = min(lumaE, minSM); 
    float maxWN = max(lumaN, lumaW);
    float minWN = min(lumaN, lumaW);
    float rangeMax = max(maxWN, maxESM);
    float rangeMin = min(minWN, minESM);
    float rangeMaxScaled = rangeMax * FXAA_QUALITY__EDGE_THRESHOLD;
    float range = rangeMax - rangeMin;
    float rangeMaxClamped = max(FXAA_QUALITY__EDGE_THRESHOLD_MIN, rangeMaxScaled);
    bool earlyExit = range < rangeMaxClamped;
/*--------------------------------------------------------------------------*/
    if(earlyExit) 
        #if (FXAA_DISCARD == 1)
            discard;
        #else
            return rgbyM;
        #endif
/*--------------------------------------------------------------------------*/
    #if (FXAA_GATHER4_ALPHA == 0) 
        float lumaNW = FxaaTexOff(tex, posM, ivec2(-1,-1), rcpFrame.xy).w;
        float lumaSE = FxaaTexOff(tex, posM, ivec2( 1, 1), rcpFrame.xy).w;
        float lumaNE = FxaaTexOff(tex, posM, ivec2( 1,-1), rcpFrame.xy).w;
        float lumaSW = FxaaTexOff(tex, posM, ivec2(-1, 1), rcpFrame.xy).w;
    #else
        float lumaNE = FxaaTexOff(tex, posM, ivec2(1, -1), rcpFrame.xy).w;
        float lumaSW = FxaaTexOff(tex, posM, ivec2(-1, 1), rcpFrame.xy).w;
    #endif
/*--------------------------------------------------------------------------*/
    float lumaNS = lumaN + lumaS;
    float lumaWE = lumaW + lumaE;
    float subpixRcpRange = 1.0/range;
    float subpixNSWE = lumaNS + lumaWE;
    float edgeHorz1 = (-2.0 * lumaM) + lumaNS;
    float edgeVert1 = (-2.0 * lumaM) + lumaWE;
/*--------------------------------------------------------------------------*/
    float lumaNESE = lumaNE + lumaSE;
    float lumaNWNE = lumaNW + lumaNE;
    float edgeHorz2 = (-2.0 * lumaE) + lumaNESE;
    float edgeVert2 = (-2.0 * lumaN) + lumaNWNE;
/*--------------------------------------------------------------------------*/
    float lumaNWSW = lumaNW + lumaSW;
    float lumaSWSE = lumaSW + lumaSE;
    float edgeHorz4 = (abs(edgeHorz1) * 2.0) + abs(edgeHorz2);
    float edgeVert4 = (abs(edgeVert1) * 2.0) + abs(edgeVert2);
    float edgeHorz3 = (-2.0 * lumaW) + lumaNWSW;
    float edgeVert3 = (-2.0 * lumaS) + lumaSWSE;
    float edgeHorz = abs(edgeHorz3) + edgeHorz4;
    float edgeVert = abs(edgeVert3) + edgeVert4;
/*--------------------------------------------------------------------------*/
    float subpixNWSWNESE = lumaNWSW + lumaNESE; 
    float lengthSign = rcpFrame.x;
    bool horzSpan = edgeHorz >= edgeVert;
    float subpixA = subpixNSWE * 2.0 + subpixNWSWNESE; 
/*--------------------------------------------------------------------------*/
    if(!horzSpan) lumaN = lumaW; 
    if(!horzSpan) lumaS = lumaE;
    if(horzSpan) lengthSign = rcpFrame.y;
    float subpixB = (subpixA * (1.0/12.0)) - lumaM;
/*--------------------------------------------------------------------------*/
    float gradientN = lumaN - lumaM;
    float gradientS = lumaS - lumaM;
    float lumaNN = lumaN + lumaM;
    float lumaSS = lumaS + lumaM;
    bool pairN = abs(gradientN) >= abs(gradientS);
    float gradient = max(abs(gradientN), abs(gradientS));
    if(pairN) lengthSign = -lengthSign;
    float subpixC = saturate(abs(subpixB) * subpixRcpRange);
/*--------------------------------------------------------------------------*/
    vec2 posB;
    posB.x = posM.x;
    posB.y = posM.y;
    vec2 offNP;
    offNP.x = (!horzSpan) ? 0.0 : rcpFrame.x;
    offNP.y = ( horzSpan) ? 0.0 : rcpFrame.y;
    if(!horzSpan) posB.x += lengthSign * 0.5;
    if( horzSpan) posB.y += lengthSign * 0.5;
/*--------------------------------------------------------------------------*/
    vec2 posN;
    posN.x = posB.x - offNP.x;
    posN.y = posB.y - offNP.y;
    vec2 posP;
    posP.x = posB.x + offNP.x;
    posP.y = posB.y + offNP.y;
    float subpixD = ((-2.0)*subpixC) + 3.0;
    float lumaEndN = FxaaTexTop(tex, posN).w;
    float subpixE = subpixC * subpixC;
    float lumaEndP = FxaaTexTop(tex, posP).w;
/*--------------------------------------------------------------------------*/
    if(!pairN) lumaNN = lumaSS;
    float gradientScaled = gradient * 1.0/4.0;
    float lumaMM = lumaM - lumaNN * 0.5;
    float subpixF = subpixD * subpixE;
    bool lumaMLTZero = lumaMM < 0.0;
/*--------------------------------------------------------------------------*/
    lumaEndN -= lumaNN * 0.5;
    lumaEndP -= lumaNN * 0.5;
    bool doneN = abs(lumaEndN) >= gradientScaled;
    bool doneP = abs(lumaEndP) >= gradientScaled;
    if(!doneN) posN.x -= offNP.x * 1.5;
    if(!doneN) posN.y -= offNP.y * 1.5;
    bool doneNP = (!doneN) || (!doneP);
    if(!doneP) posP.x += offNP.x * 1.5;
    if(!doneP) posP.y += offNP.y * 1.5;
    if(doneNP) {
/*--------------------------------------------------------------------------*/
        if(!doneN) lumaEndN = FxaaTexTop(tex, posN.xy).w;
        if(!doneP) lumaEndP = FxaaTexTop(tex, posP.xy).w;
        if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
        if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
        doneN = abs(lumaEndN) >= gradientScaled;
        doneP = abs(lumaEndP) >= gradientScaled;
        if(!doneN) posN.x -= offNP.x * 2.0;
        if(!doneN) posN.y -= offNP.y * 2.0;
        doneNP = (!doneN) || (!doneP);
        if(!doneP) posP.x += offNP.x * 2.0;
        if(!doneP) posP.y += offNP.y * 2.0;
        if(doneNP) {
/*--------------------------------------------------------------------------*/
            if(!doneN) lumaEndN = FxaaTexTop(tex, posN.xy).w;
            if(!doneP) lumaEndP = FxaaTexTop(tex, posP.xy).w;
            if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
            if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
            doneN = abs(lumaEndN) >= gradientScaled;
            doneP = abs(lumaEndP) >= gradientScaled;
            if(!doneN) posN.x -= offNP.x * 2.0;
            if(!doneN) posN.y -= offNP.y * 2.0;
            doneNP = (!doneN) || (!doneP);
            if(!doneP) posP.x += offNP.x * 2.0;
            if(!doneP) posP.y += offNP.y * 2.0;
            if(doneNP) {
/*--------------------------------------------------------------------------*/
                if(!doneN) lumaEndN = FxaaTexTop(tex, posN.xy).w;
                if(!doneP) lumaEndP = FxaaTexTop(tex, posP.xy).w;
                if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
                if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
                doneN = abs(lumaEndN) >= gradientScaled;
                doneP = abs(lumaEndP) >= gradientScaled;
                if(!doneN) posN.x -= offNP.x * 4.0;
                if(!doneN) posN.y -= offNP.y * 4.0;
                doneNP = (!doneN) || (!doneP);
                if(!doneP) posP.x += offNP.x * 4.0;
                if(!doneP) posP.y += offNP.y * 4.0;
                if(doneNP) {
/*--------------------------------------------------------------------------*/
                    if(!doneN) lumaEndN = FxaaTexTop(tex, posN.xy).w;
                    if(!doneP) lumaEndP = FxaaTexTop(tex, posP.xy).w;
                    if(!doneN) lumaEndN = lumaEndN - lumaNN * 0.5;
                    if(!doneP) lumaEndP = lumaEndP - lumaNN * 0.5;
                    doneN = abs(lumaEndN) >= gradientScaled;
                    doneP = abs(lumaEndP) >= gradientScaled;
                    if(!doneN) posN.x -= offNP.x * 2.0;
                    if(!doneN) posN.y -= offNP.y * 2.0;
                    if(!doneP) posP.x += offNP.x * 2.0; 
                    if(!doneP) posP.y += offNP.y * 2.0; } } } }
/*--------------------------------------------------------------------------*/
    float dstN = posM.x - posN.x;
    float dstP = posP.x - posM.x;
    if(!horzSpan) dstN = posM.y - posN.y;
    if(!horzSpan) dstP = posP.y - posM.y;
/*--------------------------------------------------------------------------*/
    bool goodSpanN = (lumaEndN < 0.0) != lumaMLTZero;
    float spanLength = (dstP + dstN);
    bool goodSpanP = (lumaEndP < 0.0) != lumaMLTZero;
    float spanLengthRcp = 1.0/spanLength;
/*--------------------------------------------------------------------------*/
    bool directionN = dstN < dstP;
    float dst = min(dstN, dstP);
    bool goodSpan = directionN ? goodSpanN : goodSpanP;
    float subpixG = subpixF * subpixF;
    float pixelOffset = (dst * (-spanLengthRcp)) + 0.5;
    float subpixH = subpixG * FXAA_QUALITY__SUBPIX;
/*--------------------------------------------------------------------------*/
    float pixelOffsetGood = goodSpan ? pixelOffset : 0.0;
    float pixelOffsetSubpix = max(pixelOffsetGood, subpixH);
    if(!horzSpan) posM.x += pixelOffsetSubpix * lengthSign;
    if( horzSpan) posM.y += pixelOffsetSubpix * lengthSign;
    
	//return vec4( 1.0, 1.0, 0.0, 1.0 );

	return FxaaTexTop(tex, posM); 
}
/*==========================================================================*/



