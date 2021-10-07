
#include "Common/CommonScattering.shader.h"
#include "Common/CommonFog.shader.h"
#include "Common/CommonPlanet.shader.h"
#include "Common/CommonNoise.shader.h"

//-----------------------------------------------------------------------------
//    Functions


//-----------------------------------------------------------------------------
// the original scan shader is here, it's useful reference!

/*
    vec3  lWorldUp          = GetWorldUp( IN(mWorldPositionVec4).xyz, lUniforms.mpCommonPerMaterial->gPlanetPositionVec4 );
    vec3  lViewToPos        = IN(mWorldPositionVec4).xyz - lUniforms.mpCommonPerMaterial->gViewPositionVec3;
    float lfViewToPosHeight = dot( lWorldUp, lViewToPos );
    vec3  lViewToPosFlat    = lViewToPos - lWorldUp*lfViewToPosHeight;

    float lfDistance        = length(lViewToPosFlat) * lUniforms.mpCustomPerMesh->gScanParams2Vec4.r;
    lfDistance              += lfViewToPosHeight * lUniforms.mpCustomPerMesh->gScanParamsVec4.b;
    float lfLine            = sin( lfDistance * 10.0 - lUniforms.mpCustomPerMesh->gScanParamsVec4.g * 30.0 ) * smoothstep( 0.6, 1.0, cos( lfDistance * lUniforms.mpCustomPerMesh->gScanParamsVec4.a - lUniforms.mpCustomPerMesh->gScanParamsVec4.g * 5.0 ) );
    vec3 lLineColour         = vec3( smoothstep( 0.75, 0.95, lfLine ), smoothstep( 0.9, 0.95, lfLine ), smoothstep( 0.95, 0.97, lfLine ) );
    lLineColour             *= 1.0 - clamp( lfDistance / 100.0, 0.0, 1.0 );
    lLineColour             *= lUniforms.mpCustomPerMesh->gScanParams2Vec4.g * lUniforms.mpCustomPerMesh->gScanParams2Vec4.a;
#ifndef _F45_SCANABLE
    lLineColour             *= lUniforms.mpCustomPerMesh->gScanParams2Vec4.b;
#endif
    lFragmentColourVec4.xyz += lLineColour;
*/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Old scan shader
/*vec3
ScanPulseEffectOverlay(
    vec3 lInputColour,
    vec3 lWorldPosition,
    vec4 lScanConfigPos,
    vec4 lScanConfigA,
    vec4 lScanConfigB )
{
    if ( lScanConfigA.x <= 0.0 ) 
      return lInputColour;
    
    vec3  lWorldToScanPt    = lWorldPosition - lScanConfigPos.xyz;
    float lfDistToScanPt    = length(lWorldToScanPt);

    // adjust distance so that values inside the pulse radius fall 0..1
    float lfCurrent01       = lfDistToScanPt * lScanConfigPos.w;
    float lfTarget          = lScanConfigA.y;
    float lfScanTime        = lScanConfigA.z;
    
    float lfClipOutEdge     = saturate( 2.0 - lfCurrent01 );
          lfClipOutEdge     = smoothstep( 0.9, 1.0, lfClipOutEdge );

    float lfAbsTargetToCurrent = abs(lfTarget - lfCurrent01) * lScanConfigB.x;
    float lfCurrentToTarget = (lfCurrent01 - lfTarget) * lScanConfigB.y;

    float lfLargeWaveMask = 1.0 - lfAbsTargetToCurrent;
    lfLargeWaveMask -= lfCurrentToTarget * 0.8;
//     lfLargeWaveMask = clamp(lfLargeWaveMask, 0.0, 1.0);


    float lfBaseWaveMask = 1.0 - (lfAbsTargetToCurrent * 2.0);
    float lfEdge = clamp(lfBaseWaveMask, 0.0, 1.0);
    lfBaseWaveMask -= lfCurrentToTarget * 2.0;
    lfBaseWaveMask = clamp(lfBaseWaveMask, 0.0, 1.0);

    lfBaseWaveMask = smoothstep(0.0, 1.1, lfBaseWaveMask * lfBaseWaveMask);

    float lfWaveMult = lScanConfigB.z * lfBaseWaveMask;

    float waveScaleRoot = lScanConfigB.w;
    float waveScale1    = waveScaleRoot;
    float waveScale2    = waveScaleRoot * 0.44;
    float waveScale3    = waveScaleRoot * 0.11;

    float waveShift1    = lfScanTime * -0.8;
    //float waveShift2    = lfScanTime * 1.6;

    float wave1 = lfBaseWaveMask + cos( (lfCurrent01 * waveScale1 + waveShift1) )    * lfWaveMult;
    float wave2 = lfBaseWaveMask + sin( (lfCurrent01 * waveScale2 + waveShift1) )    * lfWaveMult;
    float wave3 = lfBaseWaveMask + cos( (lfCurrent01 * waveScale3) + (lfLargeWaveMask * lfLargeWaveMask * waveScale3) ) * 1.25;

    float waveC =  mix( wave1, wave2, wave3 * wave3 );
    

    waveC += smoothstep(0.88, 0.98, lfEdge * lfEdge) * 5.0;
    waveC  = max(waveC, 0.0);

    waveC *= lfClipOutEdge;
    
    vec3 lScanFX = vec3(waveC, waveC * 0.72, waveC * 0.15);

    return lInputColour + ( lScanFX * lScanConfigA.x );
}*/
//-----------------------------------------------------------------------------

vec3
ScanPulseEffectOverlay(
    float lfTime,
    vec2 lScreenSpaceUV,
    vec3 lInputColour,
    vec3 lWorldPosition,
    vec4 lScanConfigPos,
    vec4 lColour1AndBlend,
    vec4 lColour2AndTime,
    inout vec3 lSunColourVec3 )
{
    // MIXING COLOURS
    if (lColour1AndBlend.w <= 0.0)
        return lInputColour;

    // Make the scan effect slower at the beginning
    lColour2AndTime.w = pow(lColour2AndTime.w, 3.0);

    vec3  lWorldToScanPt = lWorldPosition - lScanConfigPos.xyz;
    float lfDistToScanPt = length(lWorldToScanPt);

    // adjust distance so that values inside the pulse radius fall 0..1
    float lfDistanceScale = 2.0 / lfDistToScanPt;
    float lfRadius = 1.0 / lScanConfigPos.w;
    float lfCurrent01 = lfDistToScanPt * lScanConfigPos.w * lfRadius * lfDistanceScale;
    float lfTarget = lColour2AndTime.w * lfRadius * lfDistanceScale;


    float lfAbsTargetToCurrent = abs(lfTarget - lfCurrent01) * 3.0;
    float lfCurrentToTarget = (lfCurrent01 - lfTarget) * 2.9;

    float lfBaseWaveMask = 1.0 - (lfAbsTargetToCurrent * 2.0);
    float lfEdge = clamp(lfBaseWaveMask, -0.0, 1.0);
    lfBaseWaveMask -= lfCurrentToTarget * 2.0;
    lfBaseWaveMask = clamp(lfBaseWaveMask, 0.0, 1.0);

    lfBaseWaveMask = smoothstep(0.0, 1.1, pow(lfBaseWaveMask, 8.0));
    float lfFineEdgeMask = smoothstep(0.88, 0.98, lfEdge * lfEdge) * 5.0;

    vec3 scanCol = lColour1AndBlend.xyz;
    vec3 scanCol2 = lColour2AndTime.xyz;

    vec3 origColour = lInputColour;
    vec3 origSun    = lSunColourVec3;

    // Wave
    vec3 waveCol = mix(scanCol2, scanCol, lfAbsTargetToCurrent);
    lInputColour   = mix(lInputColour,   waveCol,   lfBaseWaveMask);
    lSunColourVec3 = mix(lSunColourVec3, float2vec3(0.0), lfBaseWaveMask);

    // Detail scan lines
    float scanDetailLines = clamp(
        sin((lScreenSpaceUV.y) * 1500.0 + lfTime* 5.0) * 0.8,
        0.0, 1.0);
    lInputColour = mix(lInputColour, scanCol, scanDetailLines * lfBaseWaveMask);
    lSunColourVec3 = mix(lSunColourVec3, float2vec3(0.0), scanDetailLines * lfBaseWaveMask);

    // Edge line
    vec3 lEdgeColour = mix(float2vec3(1.0), vec3(1.0, 0.0, 0.0), smoothstep(0.25, 1.0, 0.9));
    //lInputColour = mix(lInputColour, vec3(1.0), lfFineEdgeMask);
    lInputColour += float2vec3(1.0) * lfFineEdgeMask;

    float lfScanLuminance = (lInputColour.x * 0.3) + (lInputColour.y * 0.59) + (lInputColour.z * 0.11);

    float alpha = 0.9;
    origColour = mix(origColour,
        origColour * 0.0,
        (1.0 - lfScanLuminance) * scanDetailLines * lfBaseWaveMask * lColour1AndBlend.w);

    lInputColour   = mix(origColour, lInputColour,   alpha*lColour1AndBlend.w);
    lSunColourVec3 = mix(origSun,    lSunColourVec3, alpha*lColour1AndBlend.w);

    return lInputColour;
}

//-----------------------------------------------------------------------------
vec3
ApplyMaterialFX(
    vec3 lInputColour,
    vec3 lWorldPosition,
    vec4 lMaterialFXConfig )
{
    return mix(lInputColour.xyz, lMaterialFXConfig.xyz, lMaterialFXConfig.w * 0.8);
}


//-----------------------------------------------------------------------------
vec4
MiningHotspot(
    vec4 lInputColourVec4,
    vec4 lWorldPositionVec4,
    vec4 lMiningPosVec4 )
{
    const float kfSpotRadius = 4.0f;
    const vec3  kfSpotColour = vec3( 1.0, 0.0, 0.0 );

    vec4 lOutputColourVec4 = lInputColourVec4;
     
    float lfHeat = clamp( length( lWorldPositionVec4.xyz - lMiningPosVec4.xyz ) / kfSpotRadius, 0.0, 1.0 );
    lfHeat = (1.0 - lfHeat)*(1.0 - lfHeat) * lMiningPosVec4.w;
    
    lOutputColourVec4.xyz = mix( lOutputColourVec4.xyz, kfSpotColour, lfHeat );

    return lOutputColourVec4;
}

vec3
GetWaterScattering(
    in PerFrameUniforms          lPerFrameUniforms,
    in CommonPerMeshUniforms     lMeshUniforms,
    in CustomPerMaterialUniforms lCustomUniforms,
    in vec3                      lColourVec3,
    in vec3                      lPositionVec3,
    inout vec3                   lSunColourVec3
     )
{
    vec3  lFragmentColourVec3 = lColourVec3;

    //// Water Fog
    float lfHeightFade    = length( lPerFrameUniforms.gViewPositionVec3 - lMeshUniforms.gPlanetPositionVec4.xyz ) - lMeshUniforms.gPlanetPositionVec4.w;
    lfHeightFade          = 1.0 - clamp( ( lfHeightFade - 700.0 ) / 200.0, 0.0, 1.0 );

    float lfLowHeightFade = length( lPerFrameUniforms.gViewPositionVec3 - lMeshUniforms.gPlanetPositionVec4.xyz ) - lMeshUniforms.gPlanetPositionVec4.w;
    lfLowHeightFade       = 1.0 - clamp( ( lfLowHeightFade - 250.0 ) / 200.0, 0.0, 1.0 );

    vec3  lPlanetPosition          = lMeshUniforms.gPlanetPositionVec4.xyz;
    float lfRadius                 = lMeshUniforms.gPlanetPositionVec4.w;
       
    vec3  lWaterColourNearVec3     = /*GammaCorrectInput*/( lCustomUniforms.gWaterFogColourNearVec4.rgb );
    vec3  lWaterColourFarVec3      = /*GammaCorrectInput*/( lCustomUniforms.gWaterFogColourFarVec4.rgb );
    vec3  lLightColourVec3         = /*GammaCorrectInput*/( lMeshUniforms.gLightColourVec4.rgb );
    float lfWaterHeight            = lCustomUniforms.gWaterFogVec4.r;
    float lfWaterStrength          = lCustomUniforms.gWaterFogVec4.g;
    float lfWaterColourStrength    = lCustomUniforms.gWaterFogVec4.b;
    float lfWaterMultiplyStrength  = lCustomUniforms.gWaterFogVec4.a;
    float lfWaterMultiplyMax       = lCustomUniforms.gWaterFogColourNearVec4.a;

    vec3  lNearPosition = lPerFrameUniforms.gViewPositionVec3 - lPlanetPosition;
    vec3  lFarPosition  = lPositionVec3 - lPlanetPosition;
    if( length( lNearPosition ) <=  lfRadius + lfWaterHeight || length( lFarPosition ) <= lfRadius + lfWaterHeight )
    {
        float lfDarken          = clamp( dot( -lPerFrameUniforms.gLightDirectionVec4.xyz, normalize( lFarPosition ) ), 0.0, 1.0 );

        float lfFogDistance     = length( lFarPosition - lNearPosition );

        float lfFogValue        = lfFogDistance * lfWaterStrength;
        lfFogValue              = 1.0 / exp( lfFogValue * lfFogValue );
        lfFogValue              = 1.0 - clamp( lfFogValue, 0.0, 1.0 );

        float lfWaterFade       = lfFogDistance * lfWaterColourStrength;
        lfWaterFade             = 1.0 / exp( lfWaterFade * lfWaterFade );
        lfWaterFade             = 1.0 - clamp( lfWaterFade, 0.0, 1.0 );

        float lfMultiplyFade    = lfFogDistance * lfWaterMultiplyStrength;
        lfMultiplyFade          = 1.0 / exp( lfMultiplyFade * lfMultiplyFade );
        lfMultiplyFade          = 1.0 - clamp( lfMultiplyFade, 1.0 - lfWaterMultiplyMax, 1.0 );

        float lfEdgeFade = 1.0;
        if( length( lNearPosition ) > lfRadius + lfWaterHeight )
        {
            lfEdgeFade = 1.0 - saturate( ( length( lFarPosition ) - ( lfRadius + lfWaterHeight - 1.0 ) ) );
        }

        vec3 lWaterColour       = mix( lWaterColourNearVec3, lWaterColourFarVec3, clamp( lfWaterFade, 0.0, 1.0 ) ) * lLightColourVec3;// * lfDarken;
        lFragmentColourVec3     = mix( lColourVec3, lColourVec3 * lWaterColourNearVec3, lfEdgeFade * lfLowHeightFade * lfMultiplyFade );
        lFragmentColourVec3     = mix( lFragmentColourVec3, lWaterColour, clamp( lfEdgeFade * lfHeightFade * lfFogValue, 0.0, 1.0 ) );

        #ifdef D_SPLIT_SHADOW
        lSunColourVec3          = mix( lSunColourVec3, lSunColourVec3 * lWaterColourNearVec3, lfEdgeFade * lfLowHeightFade * lfMultiplyFade );
        lSunColourVec3          = mix( lSunColourVec3, vec3(0.0,0.0,0.0), clamp( lfEdgeFade * lfHeightFade * lfFogValue, 0.0, 1.0 ) );
        #endif
    }

    return lFragmentColourVec3;
}

vec3 
Scanline(
    vec2  uv, 
    float angle, 
    vec3  color, 
    float size, 
    float strength, 
    float decay ) 
{
    //uv[1] -= 0.5 + 0.5 * cos(mod(angle, 3.14*2.0) / 2.0);
    uv[1] -= 0.5 + 0.5 * cos(mod(angle, 3.14*2.0) / 2.0);
    uv[1] *= 1000.0 * size;
    float col = 1.0 / uv[1];
    float damp = clamp(pow(abs(uv[0]), decay) + pow(abs(1.0 - uv[0]), decay), 0.0, 1.0);
    col -= damp * 0.2;
    col = clamp(col, 0.0, strength);
    return color * col;
}

vec3 ScanlineWorldPos(
    vec2 uv, 
    float angle, 
    float size, 
    vec3 color, 
    float strength, 
    float decay)
{
    float PI = 3.1416;

    //uv.y += sin(uv.x * 4.0 + sin(uv.x*30.63)) * 0.1;

    float val = sin(uv.y*size + angle);
    float val2 = sin(uv.y*size*2.0 + angle * 2.0 + PI);

    //val2 = pow(val2, -1.0);
    val2 = 1.0 / val2;
    val2 = clamp(val2, 0.0, 1.0);

    float damp = clamp(pow(abs(val), decay), 0.0, 1.0);
    val *= val2 * damp;
    val = clamp(val, 0.0, 1.0);

    return val * color * strength;
}

vec4 ScanEffect(
    float lfTime,
    vec2 lScreenSpaceUV,
    vec4 lInputColour,
    vec4 lScanColour,
    vec4 lScanEffectConfig,
    vec3 lViewPositionVec3,
    vec3 lWorldPositionVec3,
    vec3 lNormalVec3,
    vec4 lPlanetPositionVec4,
    inout vec3 lSunColourVec3
    )
{
    if (lScanColour.w <= 0.0)
        return lInputColour;
    
    float lfScanBaseLevel       = abs( lScanEffectConfig.x );
    float lfScanLinesSeparation = abs( lScanEffectConfig.y );
    float lfFresnelIntensity    = abs( lScanEffectConfig.z );


    int lConfigInt = int(lScanEffectConfig.w);
    
    // Decode scanner config
    float lfWavesOffset   = float(lConfigInt & 0x000F) / 15.0;
    float lfTransparent   = float(lConfigInt & 0x0020) / (1 << 5);
    float lfWavesActive   = float(lConfigInt & 0x0040) / (1 << 6);
    float lfFixedUpAxis   = float(lConfigInt & 0x0080) / (1 << 7);
    float lfGlowIntensity = float(lConfigInt & 0xFF00) / (255 << 8);
    float lfAnimsActive   = step( 0.0, lScanEffectConfig.x );
    
    lfTime *= lfAnimsActive;

    // Variables
    float lfScanSpeed = 2.0;
    float lfScanAnim = sin(lfTime * lfScanSpeed) * 0.5 + 1.0;
    float lfFlickering = 0.4 * step(0.9, sin(lfTime + 9.0 * cos(lfTime*3.0))) *
        (sin(lfTime)* sin(lfTime * 20.0) +
        (0.5 + 0.1 * sin(lfTime * 2000.) * cos(lfTime)));

    // Planet height
    float lfHeight = GetHeight(lWorldPositionVec3, lPlanetPositionVec4) * (1.0 - lfFixedUpAxis) + lWorldPositionVec3.y * lfFixedUpAxis;

    // Colours
    vec3 lScanColorVec3 = lScanColour.rgb;// vec3(0.0, 0.8, 1.0);
    vec3 lScanColorSecondaryVec3 = vec3(0.8, 0.8, 1.0);
    vec3 lScanColorFresnelVec3 = float2vec3(1.0);

    // Fresnel
    float lfFresnel = clamp(dot(normalize(lViewPositionVec3 - lWorldPositionVec3), lNormalVec3), 0.0, 1.0);
    lfFresnel = 1.0 - lfFresnel;
    float lfFresnel2 = pow(lfFresnel, 3.0 * 0.15); // Pulse
    lfFresnel = pow(lfFresnel, lfScanAnim * lfFresnelIntensity); // Pulse

    //vec3 lColVec3 = lInputColour.rgb * scanBlue * 0.5;
    vec3 lColVec3 = lScanColorVec3 * lfScanBaseLevel;

    // Fresnel outline
    vec3 lScanColVec3 = mix(lScanColorFresnelVec3, lScanColorVec3, smoothstep(0.25, 1.0, 1.0 - (lfFresnel)));
    lColVec3 = mix(lColVec3, lScanColVec3, max(lfFresnel - lfFlickering*0.6, 0.0));

    // Detail scan lines
    lScreenSpaceUV.y *= lfScanLinesSeparation;
    float lfScanDetailLinesVal = (lScreenSpaceUV.y + sin(lScreenSpaceUV.x*500.0)*0.007*lfFlickering);
    float lfScanDetailLines = clamp(sin((lfScanDetailLinesVal + lfFlickering) * 1500.0 + lfTime * 5.0) * 0.5, 0.0, 1.0);
    lColVec3 = mix( lColVec3, lScanColVec3, lfScanDetailLines*lfFresnel2*step( 0.0, lfScanLinesSeparation) );

    // Scanline waves (screen space)
    //lColVec3 += Scanline(lScreenSpaceUV, lfTime*lfScanSpeed + 1.3, lScanColVec3, 0.05, 0.5, 3.0);
    //lColVec3 += Scanline(lScreenSpaceUV, lfTime*lfScanSpeed + 1.3, scanBlue2, 0.5, 0.6, 10.0);

    lfFlickering = clamp(sin(lfTime * 1.0 + sin(lfTime*3.63)) * 0.5 + 0.5, 0.3, 1.0);
    float lfDisplacement = sin(lfTime * 3.0 + sin(lfTime*3.63)) * 0.5 + 0.5;

    vec2 lPositionVec2 = vec2(0.0, lfHeight + lfDisplacement);

    // Scanline waves (world pos)
    lColVec3 += ScanlineWorldPos(lPositionVec2, lfTime * lfScanSpeed * 1.0 + lfWavesOffset, 0.2, lScanColVec3, 0.8, 1.0)  * lfFlickering * lfWavesActive;
    lColVec3 += ScanlineWorldPos(lPositionVec2, lfTime * lfScanSpeed * 1.0 + lfWavesOffset, 0.2, lScanColorSecondaryVec3, 0.8, 50.0) * lfFlickering * lfWavesActive;

    // "Noisy" scanlines
    lfFlickering = clamp(lfDisplacement, 0.0, 1.0);
    lfDisplacement = sin(lfTime * 2.0 + sin(lfTime*3.63)) * 0.5 + 0.5;
    lPositionVec2.y = lfHeight + lfDisplacement * 3.0;

    lColVec3 += ScanlineWorldPos(lPositionVec2, lfTime * lfScanSpeed * 4.0 + lfWavesOffset, 0.6, lScanColVec3, 0.1, 2.0)  * lfFlickering * lfWavesActive;
    lColVec3 += ScanlineWorldPos(lPositionVec2, lfTime * lfScanSpeed * 4.0 + lfWavesOffset, 0.6, lScanColorSecondaryVec3, 0.1, 20.0) * lfFlickering * lfWavesActive;

    float lfAlpha = max(max(max(lColVec3.x, lColVec3.y), lColVec3.z), 0.4);

    float flickering = 0.95 + 0.05 * sin(110.0*lfTime);
    lColVec3 *= flickering;

    lColVec3 += lfFresnel * lfGlowIntensity * 7.0;

    lInputColour.rgb = mix(lInputColour.rgb, saturate( lColVec3 ), saturate( lScanColour.a * lfAlpha ) * 0.9 );

    if( lfTransparent > 0 )
    {
        lInputColour.a = saturate( lScanColour.a * lfAlpha );
    }

    if( lScanEffectConfig.z < 0.0 )
    {
        lInputColour.a = saturate( lScanColour.a * lfAlpha );

        float lfFresnel3 = 1.0 - clamp( dot( normalize( lViewPositionVec3 - lWorldPositionVec3 ), lNormalVec3 ), 0.0, 1.0 );
        
        if( lScanEffectConfig.y < 0.0 )
        {
        lfFresnel3 = smoothstep( 0.05 + lfFlickering*0.3, 0.7, lfFresnel3 );
        }

        lInputColour.a = min( lInputColour.a, lfFresnel3 );
    }

    #ifdef D_SPLIT_SHADOW
    lSunColourVec3 = mix(lInputColour.rgb, vec3(0.0,0.0,0.0), saturate( lScanColour.a * lfAlpha ) * 0.9 );
    #endif

    return lInputColour;
    
}

//-----------------------------------------------------------------------------
///
///     PostProcess
///
///     @brief      PostProcess
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------

vec4
PostProcessPreLighting(
    in    PerFrameUniforms          lPerFrameUniforms,
    in    CommonPerMeshUniforms     lMeshUniforms,
    in    CustomPerMaterialUniforms lCustomUniforms,
    in    vec4                      lColourVec4,
    in    vec3                      lPositionVec3,
    inout vec3                      lNormalVec3,
    inout float                     lfRoughness,
    in    int                       liMaterialID,
    in    vec4                      lScreenSpacePositionVec4 )
{
    vec4 lFragmentColourVec4 = lColourVec4;

    if( ( liMaterialID & D_DETAILOVERLAY ) != 0 )
    {
        vec3 lUpDirection = normalize( lPositionVec3 );
        float lfTile = max( dot( lUpDirection, lNormalVec3 ), 0.0 );
        lfTile       = asin( lfTile ) / ( 3.1415 * 0.5 );
        lfTile       = lfTile * lfTile;

        vec3 lRelativePositionVec3 = ( lPositionVec3 - lMeshUniforms.gPlanetPositionVec4.xyz );

        vec3 lOverlayVec3;
        vec3 lOverlayNormalVec3;

        float lfDistance = length( lPositionVec3 - lPerFrameUniforms.gViewPositionVec3 );
        float lfFade0 = saturate( ( lfDistance - 40.0f )  / 50.0f );
        float lfFade1 = saturate( ( lfDistance - 150.0f ) / 100.0f );

        vec3 lMultiplyVec3 = float2vec3( 1.0 );

        if( ( liMaterialID & D_CLAMPAMBIENT ) == 0 )
        {
            if( lCustomUniforms.gHueOverlayParamsVec4.z > 0.0 )
            {
                lOverlayVec3       = GetTriPlanarColourMM( lNormalVec3, lRelativePositionVec3, vec2( 100.0, 0.0 ), lCustomUniforms.gHueOverlayParamsVec4.y, SAMPLER2DPARAM_LOCAL( lCustomUniforms, gHSVOverlay ) ).rgb;
                lOverlayNormalVec3 = GetTriPlanarNormal( lNormalVec3, lRelativePositionVec3, vec2( 100.0, 0.0 ), lCustomUniforms.gHueOverlayParamsVec4.y, SAMPLER2DPARAM_LOCAL( lCustomUniforms, gHSVNormalOverlay ) ).rgb;

                lOverlayVec3 = lOverlayVec3 * lCustomUniforms.gHueOverlayParamsVec4.z + ( 1.0 - lCustomUniforms.gHueOverlayParamsVec4.z );
                lOverlayVec3 = ( lOverlayVec3 + lCustomUniforms.gHueOverlayParamsVec4.x );
                lMultiplyVec3 *= lOverlayVec3;
            }
        }
        
        if( lCustomUniforms.gValueOverlayParamsVec4.z > 0.0 )
        {
            lOverlayVec3 = GetTriPlanarColourMM( lNormalVec3, lRelativePositionVec3, vec2( 100.0, 0.0 ), lCustomUniforms.gValueOverlayParamsVec4.y, SAMPLER2DPARAM_LOCAL( lCustomUniforms, gHSVOverlay ) ).rgb;
            lOverlayNormalVec3 += GetTriPlanarNormal( lNormalVec3, lRelativePositionVec3, vec2( 100.0, 0.0 ), lCustomUniforms.gValueOverlayParamsVec4.y, SAMPLER2DPARAM_LOCAL( lCustomUniforms, gHSVNormalOverlay ) ).rgb;

            lOverlayVec3 = lOverlayVec3 * lCustomUniforms.gValueOverlayParamsVec4.z + ( 1.0 - lCustomUniforms.gValueOverlayParamsVec4.z );
            lOverlayVec3 = ( lOverlayVec3 + lCustomUniforms.gValueOverlayParamsVec4.x );
            lMultiplyVec3 *= lOverlayVec3;
        }

        if( lCustomUniforms.gSaturationOverlayParamsVec4.z > 0.0 )
        {
            lOverlayVec3 = GetTriPlanarColourMM( lNormalVec3, lRelativePositionVec3, vec2( 100.0, 0.0 ), lCustomUniforms.gSaturationOverlayParamsVec4.y, SAMPLER2DPARAM_LOCAL( lCustomUniforms, gHSVOverlay ) ).rgb;
            lOverlayNormalVec3 += GetTriPlanarNormal( lNormalVec3, lRelativePositionVec3, vec2( 100.0, 0.0 ), lCustomUniforms.gSaturationOverlayParamsVec4.y, SAMPLER2DPARAM_LOCAL( lCustomUniforms, gHSVNormalOverlay ) ).rgb;

            lOverlayVec3 = lOverlayVec3 * lCustomUniforms.gSaturationOverlayParamsVec4.z + ( 1.0 - lCustomUniforms.gSaturationOverlayParamsVec4.z );
            lOverlayVec3 = ( lOverlayVec3 + lCustomUniforms.gSaturationOverlayParamsVec4.x );
            lMultiplyVec3 *= lOverlayVec3;
        }

        lFragmentColourVec4.rgb *= lMultiplyVec3;
    }

    return lFragmentColourVec4;
}

//-----------------------------------------------------------------------------
///
///     PostProcess
///
///     @brief      WriteOutput
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------

vec4
PostProcess(
    in PerFrameUniforms          lPerFrameUniforms,
    in CommonPerMeshUniforms     lMeshUniforms,
    in CustomPerMaterialUniforms lCustomUniforms,
    in vec4                      lColourVec4,
    in vec3                      lPositionVec3,
    in vec3                      lNormalVec3,
    in int                       liMaterialID,
    in vec4                      lScreenSpacePositionVec4,
    inout vec3                   lSunColourVec3
    )
{
    vec4 lFragmentColourVec4;

    //-----------------------------------------------------------------------------
    ///
    ///     Fog
    ///
    //-----------------------------------------------------------------------------

    //// Caustics
    lFragmentColourVec4 = lColourVec4;

    if( lCustomUniforms.gWaterFogVec4.r > 0.0 )
    {
        float lfWaterFade = length(lPerFrameUniforms.gViewPositionVec3 - lMeshUniforms.gPlanetPositionVec4.xyz) - lMeshUniforms.gPlanetPositionVec4.w;
        lfWaterFade = clamp((lfWaterFade - 700.0) / 200.0, 0.0, 1.0);

        float lfDistanceFade = length(lPerFrameUniforms.gViewPositionVec3 - lPositionVec3);
        lfDistanceFade = clamp((lfDistanceFade - 200.0) / 100.0, 0.0, 1.0);

        vec3 lSunCausticsVec3 = lSunColourVec3;

        vec3 lCausticsVec3 = Caustics(
            lColourVec4.rgb,
            lPositionVec3 - lMeshUniforms.gPlanetPositionVec4.xyz,
            lNormalVec3,
            normalize( lPerFrameUniforms.gViewPositionVec3 - lMeshUniforms.gPlanetPositionVec4.xyz ),
            lCustomUniforms.gWaterFogVec4.r + lMeshUniforms.gPlanetPositionVec4.w,
            lPerFrameUniforms.gfTime.x,
            SAMPLER2DPARAM_LOCAL( lCustomUniforms, gCausticMap ),
            SAMPLER2DPARAM_LOCAL( lCustomUniforms, gCausticOffsetMap ),
            lSunColourVec3 );

        float lfEdgeFade        = 1.0 - saturate( length( lPositionVec3 - lMeshUniforms.gPlanetPositionVec4.xyz ) - ( lCustomUniforms.gWaterFogVec4.r + lMeshUniforms.gPlanetPositionVec4.w - 2.0 ) );
        lCausticsVec3           = mix( lFragmentColourVec4.rgb, lCausticsVec3,           lfEdgeFade );
        lFragmentColourVec4.rgb = mix( lCausticsVec3,           lFragmentColourVec4.rgb, max( lfWaterFade, lfDistanceFade ) ); 
        lSunColourVec3          = mix( lSunCausticsVec3,        lSunColourVec3,          max( lfWaterFade, lfDistanceFade ) );

        //-----------------------------------------------------------------------------
        ///     
        ///     Water
        ///     
        //-----------------------------------------------------------------------------
        if( ( liMaterialID & D_NOWATER ) == 0 )
        {
            lFragmentColourVec4.rgb = GetWaterScattering( 
                lPerFrameUniforms, 
                lMeshUniforms, 
                lCustomUniforms, 
                lFragmentColourVec4.rgb, 
                lPositionVec3,
                lSunColourVec3
                );
        }
    }

    //-----------------------------------------------------------------------------
    ///
    ///     Scan / Mtl FX
    ///
    //-----------------------------------------------------------------------------	

    // pulse-from-point global scan overlay effect
    vec2 lScreenSpaceUV = ((lScreenSpacePositionVec4.xy / lScreenSpacePositionVec4.w) * 0.5) + float2vec2(0.5);
    lFragmentColourVec4.rgb = ScanPulseEffectOverlay(
        lPerFrameUniforms.gfTime.x,
        lScreenSpaceUV,
        lFragmentColourVec4.rgb,
        lPositionVec3,
        lMeshUniforms.gScanParamsPosVec4,
        lMeshUniforms.gScanParamsCfg1Vec4,
        lMeshUniforms.gScanParamsCfg2Vec4,
        lSunColourVec3 );

    // Second scan wave
    /*lFragmentColourVec4.rgb = ScanPulseEffectOverlay(
        lPerFrameUniforms.gfTime.x,
        lScreenSpaceUV,
        lFragmentColourVec4.rgb,
        lPositionVec3,
        lMeshUniforms.gScanParamsPosVec4 * vec4(1.0, 1.0, 1.0, 2.0),
        lMeshUniforms.gScanParamsCfg1Vec4,
        lMeshUniforms.gScanParamsCfg2Vec4 + vec4(0.0, 0.0, 0.0, -0.3));*/

#if !defined( D_TERRAIN )
    {        
        // FX applied via material-cloning per node; eg. scan analysis
        lFragmentColourVec4 = ScanEffect(
            lPerFrameUniforms.gfTime.x,
            lScreenSpaceUV,
            lFragmentColourVec4,
            lCustomUniforms.gMaterialSFXColVec4,
            lCustomUniforms.gMaterialSFXVec4,
            lPerFrameUniforms.gViewPositionVec3,
            lPositionVec3,
            lNormalVec3,
            lMeshUniforms.gPlanetPositionVec4,
            lSunColourVec3
            );
    }
#endif

    return lFragmentColourVec4;
}
