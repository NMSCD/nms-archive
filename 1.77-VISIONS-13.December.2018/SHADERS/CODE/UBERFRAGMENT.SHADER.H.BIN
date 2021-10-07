////////////////////////////////////////////////////////////////////////////////
///
///     @file       UberFragmentShader.h
///     @author     User
///     @date       
///
///     @brief      UberFragmentShader
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 

#ifndef D_FRAGMENT
#define D_FRAGMENT
#endif

//#define D_USES_VERTEX_NORMAL

//-----------------------------------------------------------------------------
//      Include files

#include "UberCommon.h"

//#include "Common/Noise3d.glsl"
#include "Common/Common.shader.h"
#include "Common/CommonFragment.shader.h"
#include "Common/CommonDepth.shader.h"
//#include "Common/CommonNoise.shader.h"
#if defined( D_DEFER ) 
#include "Common/CommonGBuffer.shader.h"
#else
#include "Common/CommonLighting.shader.h"
#endif

#if defined( D_FADE ) 
    #include "Common/CommonFade.shader.h"
#endif


//-----------------------------------------------------------------------------
//      Global Data

//
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

    INPUT( vec4, mTexCoordsVec4,            TEXCOORD0 )

#ifdef _F21_VERTEXCOLOUR
    INPUT( vec4, mColourVec4,               TEXCOORD1 )
#endif

#ifdef _F30_REFRACTION_MAP
    INPUT( vec4, mProjectedTexCoordsVec4,   TEXCOORD2 )
#endif

#ifdef D_USES_WORLD_POSITION
    INPUT( vec4, mWorldPositionVec3_mfSpare,        TEXCOORD3 )
#endif

#if !defined( _F01_DIFFUSEMAP )
    INPUT( vec4,   mMaterialVec4,           TEXCOORD4 )
#endif

#ifdef D_USES_VERTEX_NORMAL
    INPUT( vec3, mTangentSpaceNormalVec3, TEXCOORD6 )
#endif

#if !defined( D_DEPTHONLY ) && !defined( D_DEPTH_CLEAR )

#ifdef _F20_PARALLAXMAP
    INPUT( vec3, mTangentSpaceEyeVec3,      TEXCOORD7 )
#endif

#if 0 // defined( D_OUTPUT_MOTION_VECTORS ) && defined( _F14_UVSCROLL )
    INPUT( vec4, mPrevTexCoordsVec4,        TEXCOORD8 )
#endif

#if !defined( D_DEFER ) && !defined( _F07_UNLIT )
    flat INPUT( mat3,  mUpMatrixMat3,        TEXCOORD10 )
#endif

//#if defined( _F44_IMPOSTER )
//    INPUT( vec3, mShadowWorldPositionVec3,  TEXCOORD8 )
//#endif

#if defined( _F03_NORMALMAP )  || defined( _F42_DETAIL_NORMAL )
    INPUT( vec3,   mTangentMatRow1Vec3,     TEXCOORD18 )
    INPUT( vec3,   mTangentMatRow2Vec3,     TEXCOORD19 )
    INPUT( vec3,   mTangentMatRow3Vec3,     TEXCOORD20 )
#endif


#if defined( _F58_USE_CENTRAL_NORMAL ) || defined( _F56_MATCH_GROUND )
    INPUT( vec3, mCenteralNormalVec3,		TEXCOORD14 )
#endif



#ifdef D_OUTPUT_MOTION_VECTORS
    INPUT( vec4,   mPrevScreenPosition,      TEXCOORD15 )
#endif
    
#endif

#ifdef _F37_RECOLOUR
    INPUT( vec4,   mPaletteColour1Vec4,     TEXCOORD16 )
    INPUT( vec4,   mPaletteColour2Vec4,     TEXCOORD17 )
#endif

    INPUT( vec4,   mScreenSpacePositionVec4, TEXCOORD21 )

    flat INPUT( vec3, mfFadeValueForInstance_mfLodIndex_mfShearMotionLength, TEXCOORD22 )

    INPUT_FRONTFACING

DECLARE_END

#ifdef D_DEFER
#include "OutputDeferred.shader.h"
#else
#include "OutputForward.shader.h"
#endif

#if defined( _F62_DETAIL_ALPHACUTOUT )
    STATIC_CONST float kfAlphaThreshold    = 0.1;
    STATIC_CONST float kfAlphaThresholdMax = 0.5;
#elif defined( _F11_ALPHACUTOUT )
    STATIC_CONST float kfAlphaThreshold    = 0.45;
    STATIC_CONST float kfAlphaThresholdMax = 0.8;
#else
    STATIC_CONST float kfAlphaThreshold    = 0.0001;
#endif

#ifdef D_INSTANCE
    STATIC_CONST vec4 gaDebugLodColours[ 5 ] =
    {
        vec4( 1.0, 0.0, 0.0, 1.0 ),
        vec4( 0.0, 1.0, 0.0, 1.0 ),
        vec4( 0.0, 0.0, 1.0, 1.0 ),
        vec4( 1.0, 1.0, 1.0, 1.0 ),
        vec4( 0.0, 0.0, 0.0, 1.0 )
    };
#endif

//-----------------------------------------------------------------------------
//    Functions

#ifdef D_DEFERRED_DECAL
vec2
GetDecalTexCoords(
    in PerFrameUniforms            lPerFrameUniforms,
    in CommonPerMeshUniforms       lPerMeshUniforms,
    SAMPLER2DARG(               lTexture ),
    in vec4                     lScreenSpacePositionVec4,
    out vec3                    lWorldPositionVec3 )
{
    vec3 lTexCoords;    
    vec2 lGBufferCoords = SCREENSPACE_AS_RENDERTARGET_UVS(( lScreenSpacePositionVec4.xy / lScreenSpacePositionVec4.w ) * 0.5 + 0.5);

    // Deferred Pos is in view space
    float lfDepth      = FastDenormaliseDepth( lPerFrameUniforms.gClipPlanesVec4, DecodeDepthFromColour( texture2D( lTexture, lGBufferCoords ) ) );
    lWorldPositionVec3 = RecreatePositionFromDepth( lfDepth, lGBufferCoords, lPerFrameUniforms.gViewPositionVec3, lPerMeshUniforms.gInverseProjectionMat4, lPerMeshUniforms.gInverseViewMat4 );
    
    vec3  lObjectSpaceVec3   = MUL( lPerMeshUniforms.gInverseModelMat4, vec4( lWorldPositionVec3, 1.0 ) ).xyz;
    //vec3  lObjectSpaceVec3   = MUL( inverse( lPerMeshUniforms.gWorldMat4 ), vec4( lWorldPositionVec3, 1.0 ) ).xyz;

    //float lfMaxScale = max( length( lPerMeshUniforms.gWorldMat4[0] ), max( length( lPerMeshUniforms.gWorldMat4[1] ), length( lPerMeshUniforms.gWorldMat4[2] ) ) );

    lTexCoords.xyz = (lObjectSpaceVec3.xyz) + 0.5;
    lTexCoords.y   = 1.0 - lTexCoords.y;
    //lTexCoords.xy *= 1.0 / (lfMaxScale * lfMaxScale);
    
    if ( lTexCoords.x <  0.0 ||
         lTexCoords.x >= 1.0 ||
         lTexCoords.y <  0.0 ||
         lTexCoords.y >= 1.0 ||
         lTexCoords.z <  0.0 ||
         lTexCoords.z >= 1.0 )
    {
        discard;
    }

    return lTexCoords.xy;
}
#endif

mat3
GetDecalTangentSpaceMatrix(
    in vec3 worldPosition,
    in mat4 lCameraMat4 )
{
    vec3 ddxWp    = dFdx( worldPosition );
    vec3 ddyWp    = dFdy( worldPosition );
    vec3 normal   = normalize( cross( ddyWp, ddxWp ) );
    vec3 binormal = normalize( ddxWp );
    vec3 tangent  = normalize( ddyWp );

    mat3 View;
    mat3 tangentToView;

    View[0] = lCameraMat4[0].xyz;
    View[1] = lCameraMat4[1].xyz;
    View[2] = lCameraMat4[2].xyz;

#ifndef D_PLATFORM_OPENGL
    tangentToView[0] = MUL( -tangent, View );
    tangentToView[1] = MUL( binormal, View );
    tangentToView[2] = MUL( -normal , View );
#else
    tangentToView[0] = MUL( tangent, View );
    tangentToView[1] = MUL( binormal, View );
    tangentToView[2] = MUL( normal , View );
#endif

    return tangentToView;
}

mat3 
GetCotangentFrame( vec3 lPositionVec3, vec3 lViewPosition, vec2 uv )
{    
    vec3 ddxWp = dFdx( lPositionVec3 );

#ifndef D_PLATFORM_OPENGL
    vec3 ddyWp = -dFdy( lPositionVec3 );
#else
    vec3 ddyWp = dFdy( lPositionVec3 );
#endif

    vec3 lVertexNormal = normalize( cross( ddyWp, ddxWp ) );
    vec3 lViewDirVec3 = -normalize( lViewPosition - lPositionVec3.xyz );

    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( lViewDirVec3 );
    vec3 dp2 = dFdy( lViewDirVec3 );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );

    // solve the linear system
    vec3 dp2perp = cross( dp2, lVertexNormal );
    vec3 dp1perp = cross( lVertexNormal, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;

    // construct a scale-invariant frame 
    float invmax = invsqrt(max(dot(T, T), dot(B, B)));
    return mat3( T * invmax, B * invmax, -lVertexNormal );

}
    
#if defined( _F56_MATCH_GROUND ) && !defined( D_DEPTHONLY )
vec3
GetGrassColour(
    in CommonPerMeshUniforms     lCommonUniforms,
    in CustomPerMaterialUniforms lCustomUniforms,
    in vec3                      lTextureColourVec3,
    in vec3                      lWorldPositionVec3,
    in float                     lfDistance )
{
   vec3 lTexturePosition = lWorldPositionVec3 - lCommonUniforms.gPlanetPositionVec4.xyz;

    vec4  lfNoiseSample  = texture3DLod( SAMPLER_GETLOCAL(lCustomUniforms, gPerlin3D), lTexturePosition * lCustomUniforms.gTileBlendScalesVec4.x, 0.0 );
    float lfPatch1       = saturate( ( ( lfNoiseSample.r + lfNoiseSample.g + lfNoiseSample.b + lfNoiseSample.a ) / 4.0 ) ) * 2.0 - 1.0;

    lfNoiseSample        = texture3DLod( SAMPLER_GETLOCAL(lCustomUniforms, gPerlin3D), lTexturePosition * lCustomUniforms.gTileBlendScalesVec4.y, 0.0 );
    float lfPatch2       = saturate( ( ( lfNoiseSample.r + lfNoiseSample.g + lfNoiseSample.b + lfNoiseSample.a ) / 4.0 ) ) * 2.0 - 1.0;

    float lfPatch        = lfPatch1 + lfPatch2 * lCustomUniforms.gTileBlendScalesVec4.z;
    lfPatch              = 1.0 - saturate( lfPatch * 0.5 + 0.5 + lCustomUniforms.gTileBlendScalesVec4.w );
    lfPatch              = smoothstep( 0.5 - 0.12, 0.5 + 0.12, lfPatch );

    vec3 lOriginalColour = RGBToHSV( lTextureColourVec3 );

    vec3 lColour1 = lCustomUniforms.gTerrainColour1Vec4.rgb;
    vec3 lColour2 = lCustomUniforms.gTerrainColour2Vec4.rgb;

    float lfValue1 = saturate( lOriginalColour.z * 2.0 ) * lColour1.z;
    float lfValue2 = saturate( lOriginalColour.z * 2.0 ) * lColour2.z;

    lColour1.z = mix( lColour1.z, lfValue1, lfDistance );
    lColour2.z = mix( lColour2.z, lfValue2, lfDistance );

    lColour1 = HSVToRGB( lColour1 );
    lColour2 = HSVToRGB( lColour2 );

    lColour1 = mix( lColour1, lColour2, lfPatch );

    lColour1 = GammaCorrectInput( lColour1 );

    return lColour1;
}
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
#if defined( D_DEPTH_CLEAR )
FRAGMENT_MAIN_COLOUR_DEPTH_SRT
#else
#if defined( D_DEPTHONLY )
VOID_MAIN_SRT
#else
FRAGMENT_MAIN_COLOUR_SRT
#endif
#endif
{

#if defined( D_IMPOSTER ) && defined( _F09_TRANSPARENT ) && ( defined( _F13_UVANIMATION ) || defined( _F14_UVSCROLL ) )

    discard;

#else

    //-----------------------------------------------------------------------------
    ///
    ///     Fading
    ///
    //-----------------------------------------------------------------------------
    #if  defined( D_FADE ) 
    {   
        #if defined( _F44_IMPOSTER )
        vec4 lTexCoords = IN( mTexCoordsVec4 ) * lUniforms.mpCustomPerMaterial->gImposterDataVec4.x;
        #else
        vec4 lTexCoords = IN( mTexCoordsVec4 );
        #endif
            
        CheckFade(
            SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMaterial, gFadeNoiseMap ),
            IN( mScreenSpacePositionVec4 ),
            IN( mfFadeValueForInstance_mfLodIndex_mfShearMotionLength ).x,
            lUniforms.mpPerFrame->gFrameBufferSizeVec4.xy );
    }
    #endif

#ifdef D_DEPTH_CLEAR
    FRAGMENT_COLOUR = vec4( 0.0, 0.0, 0.0, 0.0 );
    FRAGMENT_DEPTH = D_DEPTH_CLEARVALUE;
    return;
#else

    //-----------------------------------------------------------------------------
    ///
    ///     Diffuse Maps
    ///
    //-----------------------------------------------------------------------------
    vec4  lColourVec4;
    vec3  lNormalVec3         = vec3( 0.0, 1.0, 0.0 );
#ifdef D_USES_WORLD_POSITION
    vec3  lWorldPositionVec3  = IN( mWorldPositionVec3_mfSpare ).xyz;
#endif

#ifdef _F63_DISSOLVE
    float lDissolveEdge = 2.0f;
    if( lUniforms.mpCustomPerMaterial->gDissolveDataVec4.w != 0.0f )
    {
        float dissolveDist = abs( dot( lUniforms.mpCustomPerMaterial->gDissolveDataVec4.xyz, lWorldPositionVec3 ) ) - abs( lUniforms.mpCustomPerMaterial->gDissolveDataVec4.w );

        if( ( lUniforms.mpCustomPerMaterial->gDissolveDataVec4.w < 0.0f && dissolveDist < 0.0f ) || ( lUniforms.mpCustomPerMaterial->gDissolveDataVec4.w >= 0.0f && dissolveDist >= 0.0f ) )
        {
            lDissolveEdge = abs( dissolveDist ) / 0.05f;

            if( lDissolveEdge > 1.0f )
            {
                discard;
            }
        }
    }
#endif

    vec4  lPrevScreenPosition = vec4( 0.0, 0.0, 0.0, 0.0 );
    float lLowAlpha           = 1.0;
    float lHighAlpha          = 1.0;

    #ifdef _F55_MULTITEXTURE
    // {
    float lfMultiTextureIndex = lUniforms.mpCommonPerMesh->gUserDataVec4.w;
    // }
    #endif

    #ifdef D_TEXCOORDS
    //{
        vec4 lTexCoordsVec4;
        lTexCoordsVec4 = IN( mTexCoordsVec4 );


        #ifdef D_DEFERRED_DECAL
        {
            lTexCoordsVec4 = GetDecalTexCoords( 
                                DEREF_PTR( lUniforms.mpPerFrame ),
                                DEREF_PTR( lUniforms.mpCommonPerMesh ),
                                SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMaterial,gBufferMap ),
                                IN( mScreenSpacePositionVec4 ),
                                lWorldPositionVec3 ).xyxy;
        }
        #endif

    #if defined( D_IMPOSTER_COLOUR ) && defined( _F12_BATCHED_BILLBOARD )
        lTexCoordsVec4.y = 1.0 - lTexCoordsVec4.y;
    #endif


    //}
    #endif

    #ifdef _F01_DIFFUSEMAP
    {
        #ifdef _F55_MULTITEXTURE
                lColourVec4 = texture2DArray( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial,gDiffuseMap ), vec3(lTexCoordsVec4.xy, lfMultiTextureIndex));
                //lColourVec4 = vec4(1.0, 0.0, 0.0, 1.0);
        #else
                lColourVec4 = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial,gDiffuseMap ), lTexCoordsVec4.xy);
        #endif 

        #if  !defined( _F07_UNLIT ) && defined( _F39_METALLIC_MASK )
            #if defined( _F34_GLOW ) && defined( _F35_GLOW_MASK ) && !defined( _F09_TRANSPARENT )
                lHighAlpha           = GetUpperValue( lColourVec4.a );
            #else
                lHighAlpha           = ( lColourVec4.a );
            #endif
        #endif
                
        #if defined( _F34_GLOW ) && defined( _F35_GLOW_MASK ) && !defined( _F09_TRANSPARENT )
        lLowAlpha            = GetLowerValue( lColourVec4.a );
        #endif

        #if !defined( _F09_TRANSPARENT ) && !defined( _F11_ALPHACUTOUT )
        {
            // There is not a transparent texture
            lColourVec4.a = 1.0;
    }
        #endif

        #if defined( _F05_INVERT_ALPHA )
        {            
            lColourVec4.a = 1.0 - lColourVec4.a;
    }
        #endif
    }
    #else
    {
        lColourVec4 = IN( mMaterialVec4 );
    }
    #endif

    #ifdef D_MASKS
    //{
        #ifdef _F55_MULTITEXTURE
            vec4 lMasks = texture2DArray( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial,gMasksMap ), vec3(lTexCoordsVec4.xy, lfMultiTextureIndex) );
        #else
            vec4 lMasks = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial,gMasksMap ), lTexCoordsVec4.xy);
        #endif
    //}
    #endif

    #ifdef _F53_COLOURISABLE
    {
        #ifdef _F54_COLOURMASK
        {
            // Tint by mask
            float lfColourMask = lMasks.b;

            vec3  lTintPrimaryColourVec3   = vec3(0.0, 0.0, 0.0);
            vec3  lTintSecondaryColourVec3 = vec3(0.0, 0.0, 0.0);

            DecodeUnitScalars(lUniforms.mpCommonPerMesh->gUserDataVec4.x, lTintSecondaryColourVec3.x, lTintPrimaryColourVec3.x);
            DecodeUnitScalars(lUniforms.mpCommonPerMesh->gUserDataVec4.y, lTintSecondaryColourVec3.y, lTintPrimaryColourVec3.y);
            DecodeUnitScalars(lUniforms.mpCommonPerMesh->gUserDataVec4.z, lTintSecondaryColourVec3.z, lTintPrimaryColourVec3.z);

            /*vec3  lTintPrimaryColour = GammaCorrectInput(lUniforms.mpCommonPerMesh->gUserDataVec4.xyz);
            vec3  lTintSecondaryColour = vec3(0.0, 0.0, 1.0);*/

            lTintPrimaryColourVec3   = GammaCorrectInput(lTintPrimaryColourVec3);
            lTintSecondaryColourVec3 = GammaCorrectInput(lTintSecondaryColourVec3);

            if (lfColourMask > 0.25)
            {
                if (lfColourMask < 0.75)
                {
                    lColourVec4.rgb = lColourVec4.rgb * lTintSecondaryColourVec3;
                }
                else
                {
                    lColourVec4.rgb = lColourVec4.rgb * lTintPrimaryColourVec3;
                }
            }
        }
        #else
        {
            // Change HUE
            vec3 lOriginalHSVVec3 = RGBToHSV(lColourVec4.xyz);
            vec3 lTintHSVVec3     = RGBToHSV( GammaCorrectInput( lUniforms.mpCommonPerMesh->gUserDataVec4.xyz ) );

            vec3 lHSVColourVec3 = lOriginalHSVVec3;
            lHSVColourVec3.x    = lTintHSVVec3.x;

            if( lTintHSVVec3.y > 0 )
            {
                lColourVec4.rgb = HSVToRGB( lHSVColourVec3 );
            }
        }
        #endif
    }
    #endif
    
    #ifdef _F16_DIFFUSE2MAP
    //{
    vec4 lDiffuse2Vec4;

        lDiffuse2Vec4 = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial,gDiffuse2Map ), lTexCoordsVec4.zw );
    
    #ifndef _F17_MULTIPLYDIFFUSE2MAP
        {
            lColourVec4.rgb = mix( lColourVec4.rgb, lDiffuse2Vec4.rgb, lDiffuse2Vec4.a );
        }
    #endif

    //}
    #endif

    #ifdef _F21_VERTEXCOLOUR
    {
        lColourVec4 *= IN( mColourVec4 );	
    }
    #endif

    #if defined( D_OUTPUT_MOTION_VECTORS )

        lPrevScreenPosition = IN( mPrevScreenPosition );

        #if 0 // defined( D_TEXCOORDS ) && defined( _F14_UVSCROLL )
        {
            // note! this works, but I have had to turn it off - fast UV scrolling effects in
            // a few places are causing the motion blur to go nuts. Left here to enable possibly
            // a material flag to be added someday.

            // compute texture UV change
            vec2 lvTextureSpaceTexMotion = IN( mPrevTexCoordsVec4 ).xy - IN( mTexCoordsVec4 ).xy;

            // texture UV change is approximately (first-order taylor, no perspective)
            // dU = ddx(U) * dX + ddy(U) * dY
            // dV = ddx(V) * dX + ddy(V) * dY

            float dU = lvTextureSpaceTexMotion.x;
            float dV = lvTextureSpaceTexMotion.y;

            float ddxU = dFdx( lTexCoordsVec4.x );
            float ddyU = dFdy( lTexCoordsVec4.x );

            float ddxV = dFdx( lTexCoordsVec4.y );
            float ddyV = dFdy( lTexCoordsVec4.y );

            // now solve for dX and dY using Cramer's rule
            float lfDenom = ddxU * ddyV - ddxV * ddyU;

            float dX = ( dU * ddyV - dV * ddyU ) / lfDenom;
            float dY = ( ddxU * dV - ddxV * dU ) / lfDenom;

            // and again, first-order Taylor to estimate prev screen pos accounting for UV
            vec4 lTexPositionChange = dFdx( lPrevScreenPosition ) * dX + dFdy( lPrevScreenPosition ) * dY;
            // lPrevScreenPosition += lTexPositionChange;
        }
        #endif

    #endif

    //-----------------------------------------------------------------------------
    ///
    ///     Transparency
    ///
    //-----------------------------------------------------------------------------

    #if defined(D_PLANE_SPOT)
    {
        float lfDistance = length((lUniforms.mpCustomPerMaterial->gPlaneSpotPositionVec4).xyz - lWorldPositionVec3);
        float lfAngle = 1.5707963268 * saturate(lfDistance * 0.125);  //Radius 8 (1/0.125)
        lColourVec4.a *= cos(lfAngle);
    }
    #endif
 
    #ifdef _F22_TRANSPARENT_SCALAR
    {
        lColourVec4.a *= lUniforms.mpCustomPerMaterial->gMaterialColourVec4.a;
    }
    #endif

    // Discard fully transparent pixels
    #if defined( _F09_TRANSPARENT ) || defined( _F22_TRANSPARENT_SCALAR ) || defined( _F11_ALPHACUTOUT )
    {

        #if !defined(D_ZEQUALS)
        {
            if( lColourVec4.a < kfAlphaThreshold )
            {
                discard;
            }
        }
        #endif  

        #ifdef _F11_ALPHACUTOUT
        {

            lColourVec4.a = smoothstep( kfAlphaThreshold, kfAlphaThresholdMax, lColourVec4.a );
        }
        #endif
    }
    #endif

#if defined( _F60_ACUTE_ANGLE_FADE ) && !defined( D_IMPOSTER )
    vec3 lViewDirVec3 = normalize( lUniforms.mpPerFrame->gViewPositionVec3 - IN( mWorldPositionVec3_mfSpare ).xyz );

    float lfNdotCam = 1.0 - acos( abs( dot( lViewDirVec3, normalize( IN( mTangentSpaceNormalVec3 ).xyz ) ) ) ) / ( 3.14 * 0.5 );

    if( lfNdotCam < 0.5 )
    {
        CheckFade(
            SAMPLER2DPARAM_SRT( lUniforms.mpCustomPerMaterial,gFadeNoiseMap ),
            IN( mScreenSpacePositionVec4 ),
            lfNdotCam * 2.0,
            lUniforms.mpPerFrame->gFrameBufferSizeVec4.xy );
    }
#endif

#if defined( D_DEPTHONLY )

#if defined( D_PLATFORM_OPENGL )
    FRAGMENT_COLOUR  = vec4( 0.0 );
#endif

#else

    //-----------------------------------------------------------------------------
    ///
    ///     Detail Maps
    ///
    //-----------------------------------------------------------------------------

    #ifdef D_DETAIL
    //{
        float lfImageIndex      = floor( lMasks.a * 10.0 );
        float lfIsBlendImage    = ceil( clamp( lUniforms.mpCustomPerMesh->gCustomParams02Vec4.z - lfImageIndex, 0.0, 1.0 ) );
        float lfIsMultiplyImage = 1.0 - lfIsBlendImage;

        lfImageIndex = mix( lfImageIndex, lUniforms.mpCustomPerMesh->gCustomParams02Vec4.z + (lfImageIndex - (10.0 - lUniforms.mpCustomPerMesh->gCustomParams02Vec4.w)), lfIsMultiplyImage );

        float lfDetailScale         = mix( lUniforms.mpCustomPerMesh->gCustomParams02Vec4.x, lUniforms.mpCustomPerMesh->gCustomParams02Vec4.y, lfIsBlendImage );
        vec4  lDetailNormalVec4     = texture2DArray( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial,gDetailNormalMap ), vec3((lTexCoordsVec4.xy * lfDetailScale), lfImageIndex ) );

        float lfBlendSize = (step( 0.5, 1.0 - lMasks.b ) * 0.4) + 0.1;
        float lfBlendHeightMin = clamp( 1.0 - lDetailNormalVec4.r, 0.05, 0.95 );
        float lfBlendHeightMax = min( lfBlendHeightMin + lfBlendSize, 0.95 );

        #if defined( _F41_DETAIL_DIFFUSE ) || defined( _F39_METALLIC_MASK ) || defined( _F35_GLOW_MASK )
        {
            vec4   lDetailDiffuseVec4 = texture2DArray( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial,gDetailDiffuseMap ), vec3((lTexCoordsVec4.xy * lfDetailScale), lfImageIndex ) ); 

            #ifdef _F41_DETAIL_DIFFUSE
            {
                lColourVec4.rgb = mix( lColourVec4.xyz, lDetailDiffuseVec4.xyz,                   lfIsBlendImage    * smoothstep( lfBlendHeightMin, lfBlendHeightMax, lMasks.b ) );
                lColourVec4.rgb = mix( lColourVec4.xyz, lColourVec4.xyz * lDetailDiffuseVec4.xyz, lfIsMultiplyImage * smoothstep( lfBlendHeightMin, lfBlendHeightMax, lMasks.b ) );
            }
            #endif

            #if  !defined( _F07_UNLIT ) && defined( _F39_METALLIC_MASK )
                lHighAlpha = mix( lHighAlpha, GetUpperValue( lDetailDiffuseVec4.a ), lfIsBlendImage * smoothstep( lfBlendHeightMin, lfBlendHeightMax, lMasks.b ) );
                lHighAlpha = mix( lHighAlpha, GetUpperValue( lDetailDiffuseVec4.a ) * lHighAlpha, lfIsMultiplyImage * smoothstep( lfBlendHeightMin, lfBlendHeightMax, lMasks.b ) );
            #endif

            #if defined( _F34_GLOW ) && defined( _F35_GLOW_MASK ) && !defined( _F09_TRANSPARENT )
                lLowAlpha = mix( lLowAlpha, GetLowerValue( lDetailDiffuseVec4.a ), lfIsBlendImage * smoothstep( lfBlendHeightMin, lfBlendHeightMax, lMasks.b ) );
                lLowAlpha = mix( lLowAlpha, GetLowerValue( lDetailDiffuseVec4.a ) * lLowAlpha, lfIsMultiplyImage * smoothstep( lfBlendHeightMin, lfBlendHeightMax, lMasks.b ) );
            #endif
        }
        #endif
    //}
    #endif

    #ifdef _F24_AOMAP
    {
        lColourVec4.rgb *= lMasks.r;
    }
    #endif

    #ifdef _F17_MULTIPLYDIFFUSE2MAP
    {
        lColourVec4.rgb *= lDiffuse2Vec4.r;
    }
    #endif

    //-----------------------------------------------------------------------------
    ///
    ///     Normals
    ///
    //-----------------------------------------------------------------------------
    vec3 lFaceNormalVec3;
    #if defined( _F36_DOUBLESIDED )
    {
        vec3 lEyeVec3 = normalize( lUniforms.mpPerFrame->gViewPositionVec3 - lWorldPositionVec3 );

        vec3 ddxWp           = dFdx( lWorldPositionVec3 );
        vec3 ddyWp           = dFdy( lWorldPositionVec3 );
        lFaceNormalVec3      = normalize( cross( ddyWp, ddxWp ) );

        if( dot( lFaceNormalVec3, lEyeVec3 ) < 0.0 )
        {
            lFaceNormalVec3 = reflect( IN( mTangentSpaceNormalVec3 ), ddxWp );
        }
        else
        {
            lFaceNormalVec3 = IN( mTangentSpaceNormalVec3 );
        }
    }
    #endif

    #ifdef _F03_NORMALMAP
    //{
        #ifdef _F43_NORMAL_TILING
        {    
            lTexCoordsVec4.xy *= lUniforms.mpCustomPerMesh->gCustomParams01Vec4.z;
        }
        #endif
        
        #if defined( _F44_IMPOSTER )
            vec4 lTexColour = texture2DLod( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial,gNormalMap ), lTexCoordsVec4.xy, 1.0 );
            vec3 lNormalTexVec3 = -normalize(lTexColour.xyz * 2.0 - 1.0);
        #else
            #ifdef _F55_MULTITEXTURE
                    vec4 lTexColour = texture2DArray( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial,gNormalMap ), vec3(lTexCoordsVec4.xy, lfMultiTextureIndex ));
            #else
                    vec4 lTexColour = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial,gNormalMap ), lTexCoordsVec4.xy );
            #endif 
        
        vec3 lNormalTexVec3 = DecodeNormalMap( lTexColour );
        #endif
            
         lNormalVec3 = lNormalTexVec3;
    //}
    #elif defined( D_USES_VERTEX_NORMAL )
    {
        lNormalVec3 = IN( mTangentSpaceNormalVec3 );
    }
    #endif

    #ifdef _F42_DETAIL_NORMAL
    {
        vec3 lDetailNormalVec3 = DecodeNormalMap( lDetailNormalVec4 );

        lDetailNormalVec3   = mix( lDetailNormalVec3, lNormalVec3, 0.5 );
        lDetailNormalVec3.z = lNormalVec3.z;

        lNormalVec3 = mix( lNormalVec3, lDetailNormalVec3, smoothstep( lfBlendHeightMin, lfBlendHeightMax, lMasks.b ) );
    }
    #endif
    
    #if defined( _F03_NORMALMAP ) || defined( _F42_DETAIL_NORMAL )
    {
        mat3 lTangentSpaceMat;

        #ifdef _F52_DECAL_NORMAL
        {
           // lTangentSpaceMat = GetDecalTangentSpaceMatrix( lWorldPositionVec3, lUniforms.mpPerFrame->gViewMat4 );
            lTangentSpaceMat = GetCotangentFrame( lWorldPositionVec3, lUniforms.mpPerFrame->gViewPositionVec3, lTexCoordsVec4.xy );
        }
        #else
        {
            lTangentSpaceMat[0] = IN( mTangentMatRow1Vec3 );
            lTangentSpaceMat[1] = IN( mTangentMatRow2Vec3 );
            lTangentSpaceMat[2] = IN( mTangentMatRow3Vec3 );
        }
        #endif

        #if defined( _F36_DOUBLESIDED )

        #ifdef _F18_UVTILES // Tree with flat leaves which use multiple textures have different lighting
            lTangentSpaceMat[ 1 ] = cross( lFaceNormalVec3, lTangentSpaceMat[ 0 ] ) * IN( mWorldPositionVec3_mfSpare ).w;
            lTangentSpaceMat[ 2 ] = lFaceNormalVec3;
        #else

            vec3 lEyeVec3 = normalize( lUniforms.mpPerFrame->gViewPositionVec3 - lWorldPositionVec3 );

            mat3 lCotangentMat = GetCotangentFrame( lWorldPositionVec3, lUniforms.mpPerFrame->gViewPositionVec3, lTexCoordsVec4.xy );

            if( dot( IN( mTangentSpaceNormalVec3 ), lEyeVec3 ) < 0.0 )
            {
                lTangentSpaceMat = lCotangentMat;
            }
        #endif

        #endif

        lNormalVec3 = normalize( MUL( lTangentSpaceMat, lNormalVec3 ) ); 
           
    }
    #endif

    //-----------------------------------------------------------------------------
    ///
    ///     Lighting
    ///
    //-----------------------------------------------------------------------------
    
    float lfRoughness  = 1.0;
    float lfMetallic   = 0.0;
    float lfSubsurface = 0.0;

    #ifndef _F07_UNLIT
    {
        #ifndef _F44_IMPOSTER

            #ifdef _F25_ROUGHNESS_MASK
            {
                lfRoughness = lMasks.g;

                #ifdef D_DETAIL
                {
                    lfRoughness  = mix( lfRoughness, lDetailNormalVec4.b,               lfIsBlendImage    * smoothstep( lfBlendHeightMin, lfBlendHeightMax, lMasks.b ) );
                    lfRoughness  = mix( lfRoughness, lDetailNormalVec4.b * lfRoughness, lfIsMultiplyImage * smoothstep( lfBlendHeightMin, lfBlendHeightMax, lMasks.b ) );
                }
                #endif

                lfRoughness = 1.0 - lfRoughness;
            }
            #endif

            lfRoughness *= lUniforms.mpCustomPerMaterial->gMaterialParamsVec4.x;

        #else
        
            lfRoughness = lMasks.g;

        #endif

        #ifndef _F44_IMPOSTER

            #ifdef _F39_METALLIC_MASK
            {
                lfMetallic = lHighAlpha;

                #ifdef D_DETAIL
                {
                    lfMetallic = mix( lfMetallic, lDetailNormalVec4.b,               lfIsBlendImage    * smoothstep( lfBlendHeightMin, lfBlendHeightMax, lMasks.b ) );
                    lfMetallic = mix( lfMetallic, lDetailNormalVec4.b * lfRoughness, lfIsMultiplyImage * smoothstep( lfBlendHeightMin, lfBlendHeightMax, lMasks.b ) );
                }
                #endif        
            }
            #else 
            {
                lfMetallic = lUniforms.mpCustomPerMaterial->gMaterialParamsVec4.z;
            }
            #endif

        #else

            lfMetallic = lMasks.b;

        #endif

        #ifdef _F40_SUBSURFACE_MASK
        {
            lfSubsurface = lMasks.r;
        }
        #endif

        #if defined( _F58_USE_CENTRAL_NORMAL ) && defined( _F03_NORMALMAP )
        {
            float lfDistance = length(lWorldPositionVec3 - lUniforms.mpPerFrame->gViewPositionVec3);
            lfDistance       = ( lfDistance - 20.0 ) / 30.0;
            lfDistance       = 1.0 - saturate( lfDistance );
            lNormalVec3      = mix( normalize( IN( mCenteralNormalVec3 ).xyz ), lNormalVec3, lUniforms.mpCustomPerMaterial->gMaterialParamsVec4.w * lfDistance );
            //lNormalVec3 = normalize( IN( mCenteralNormalVec3 ).xyz );
        }
        #endif

    }
    #endif

    float lfGlow = 0.0;
    #if defined( _F34_GLOW ) && !defined( D_NO_GLOW )
    {
        #if defined( _F35_GLOW_MASK ) && !defined( _F09_TRANSPARENT )
        {
            lfGlow = lUniforms.mpCustomPerMesh->gCustomParams01Vec4.y * lLowAlpha;
        }
        #else
        {
            lfGlow = lUniforms.mpCustomPerMesh->gCustomParams01Vec4.y;
        }
        #endif
    }
    #endif
    
    
    #if defined( _F56_MATCH_GROUND ) && !defined( D_DEPTHONLY )

        float lfDistance = length( lWorldPositionVec3 - lUniforms.mpPerFrame->gViewPositionVec3 );
        lfDistance       = ( lfDistance - 20.0 ) / 30.0;
        lfDistance       = 1.0 - saturate( lfDistance );
        lColourVec4.rgb = GetGrassColour( 
            DEREF_PTR( lUniforms.mpCommonPerMesh ), 
            DEREF_PTR( lUniforms.mpCustomPerMaterial ),
            lColourVec4.rgb,
            lWorldPositionVec3,
            lfDistance );
    #endif
    
    //-----------------------------------------------------------------------------
    ///
    ///     Write Output
    ///
    //-----------------------------------------------------------------------------

    int liMaterialID = 0;

    #ifdef _F44_IMPOSTER
        if( lMasks.a > 0.5 )
        {
            liMaterialID = D_CLAMPAMBIENT;
        }
    #endif

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

    #if defined( _F61_CLAMP_AMBIENT )
        liMaterialID |= D_CLAMPAMBIENT;
    #endif

    #ifdef _F49_DISABLE_AMBIENT
        liMaterialID |= D_DISABLE_AMBIENT;
    #endif

    #if defined( _F44_IMPOSTER )
        //liMaterialID |= D_NORECEIVESHADOW;
        //liMaterialID |= D_EMBEDDED_SHADOW;
        //lfSubsurface = ComputeShadowIntensity(DEREF_PTR(lUniforms.mpCustomPerMaterial), DEREF_PTR(lUniforms.mpPerFrame), DEREF_PTR(lUniforms.mpCommonPerMesh), IN( mShadowWorldPositionVec3 ).xyz, lNormalVec3, vec2(0.0, 0.0), true);
    #endif

    #if defined( D_OUTPUT_MOTION_VECTORS ) && defined( D_INSTANCE )
        if( IN( mfFadeValueForInstance_mfLodIndex_mfShearMotionLength ).z > 0.0 )
        {
            liMaterialID |= D_CLAMP_AA;
        }
    #endif

    #if defined( _F14_UVSCROLL )
        liMaterialID |= D_CLAMP_AA;
    #endif

    vec4 lOutColours0Vec4;
    vec4 lOutColours1Vec4;
    vec4 lOutColours2Vec4;
    vec4 lOutColours3Vec4;
    vec4 lOutColours4Vec4;
    
#ifdef D_INSTANCE

    // Debug lod colouring. See giLodOverride in EgInstancedModelNode.cpp.
    // We only have 1 float in which to store useful info, so store either normal index, or index + 10 so
    // in the shader we can tell whether to render normally or debug. (see mLodIndexVec4 in unberfragment and vertex).


    int liLodIndex = int( IN( mfFadeValueForInstance_mfLodIndex_mfShearMotionLength ).y );
    lColourVec4 = liLodIndex < 10 ? lColourVec4 : gaDebugLodColours[ liLodIndex-10 ];
#endif

/*#if defined( _F36_DOUBLESIDED ) && !defined( D_IMPOSTER_NORMAL )
    {
        vec3 lEyeVec3 = normalize( lUniforms.mpPerFrame->gViewPositionVec3 - lWorldPositionVec3 );

        vec3 ddxWp           = dFdx( lWorldPositionVec3 );
        vec3 ddyWp           = dFdy( lWorldPositionVec3 );
        lFaceNormalVec3      = normalize( cross( ddyWp, ddxWp ) );

        if( dot( IN( mTangentSpaceNormalVec3 ), lEyeVec3 ) > 0.0 )
        {
            lColourVec4.rgb = vec3( 1.0, 0.0, 0.0 );
        }
        else
        {
            lColourVec4.rgb = vec3( 0.0, 1.0, 0.0 );
        }
    }
#endif*/


    //lColourVec4.rgb = lNormalVec3 * 0.5 + vec3( 0.5 );


    WriteOutput(
        lOutColours0Vec4,
        lOutColours1Vec4,
        lOutColours2Vec4,
        lOutColours3Vec4,
        lOutColours4Vec4,
        DEREF_PTR( lUniforms.mpPerFrame ),
        DEREF_PTR( lUniforms.mpCommonPerMesh ),
        DEREF_PTR( lUniforms.mpCustomPerMaterial ),
        lColourVec4,
        lWorldPositionVec3,
        lNormalVec3,
        liMaterialID,
        lfMetallic,
        lfRoughness,
        lfSubsurface,
        lfGlow,
#ifdef D_USE_SCREEN_POSITION   
        IN( mScreenSpacePositionVec4 ).xyzw ,
#else
        float2vec4(0.0),
#endif
#ifdef D_OUTPUT_MOTION_VECTORS   
        lPrevScreenPosition,
#else
        float2vec4( 0.0 ),
#endif
#if !defined( D_DEFER ) && !defined( _F07_UNLIT )
        IN( mUpMatrixMat3 ) ,
        SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMaterial,gDualPMapBack),
        SAMPLER2DPARAM_SRT(lUniforms.mpCustomPerMaterial,gDualPMapFront),
#endif
        0.0, // temp. Going to be for per pixel depth of imposters
        FRAGMENT_FRONTFACE
        );

#if defined( D_LIT_WITH_MASK )
    {
        float lfAlpha = saturate( ( ( DEREF_PTR( lUniforms.mpCustomPerMaterial ) ).gMaterialSFXColVec4.a * 2.0f ) );
        FRAGMENT_COLOUR0 = lOutColours0Vec4;
        FRAGMENT_COLOUR1 = vec4( lfAlpha, 0.0, 0.0, 1.0 );
    }
#elif !defined( D_ATTRIBUTES )    
    if ( lOutColours0Vec4.a < ( 1.0 / 255.0 ) )
    {
        // this pixel will have no effect on the framebuffer -
        // don't write the stencil buffer plz
        discard;
    }
    FRAGMENT_COLOUR  = lOutColours0Vec4;
#else

#ifdef _F63_DISSOLVE

    if( lDissolveEdge <= 1.0f )
    {
        lOutColours0Vec4 = mix( lOutColours0Vec4, vec4( 1.0, 1.0, 1.0, 1.0 ), clamp( lDissolveEdge, 0.0f, 1.0f ) );

        liMaterialID |= D_UNLIT;
        lOutColours3Vec4.r = float( liMaterialID ) / 255.0f;
    }
#endif

    #ifdef D_DEFERRED_DECAL
    {
        #ifdef _F51_DECAL_DIFFUSE
        {
            FRAGMENT_COLOUR0 = vec4( lOutColours0Vec4.xyz, lColourVec4.a );
            //FRAGMENT_COLOUR0 = vec4( lTexCoordsVec4.x, lTexCoordsVec4.y, 0.0, lColourVec4.a );
            //FRAGMENT_COLOUR0 = vec4( 1.0, 0.0, 0.0, lColourVec4.a );
        }
        #else
        {
            FRAGMENT_COLOUR0 = float2vec4(0.0); // Discard Colour
        }
        #endif

        //FRAGMENT_COLOUR1 = lOutColours1Vec4;
        FRAGMENT_COLOUR1 = float2vec4(0.0);
        
        #ifdef _F03_NORMALMAP
        {
            FRAGMENT_COLOUR2 = vec4( lOutColours2Vec4.xyz, lColourVec4.a );
        }
        #else
        {
            FRAGMENT_COLOUR2 = float2vec4(0.0); // Discard Colour
        }
        #endif

        FRAGMENT_COLOUR3 = lOutColours3Vec4;
    }
    #else
    {
        FRAGMENT_COLOUR0 = lOutColours0Vec4;
        FRAGMENT_COLOUR1 = lOutColours1Vec4;
        FRAGMENT_COLOUR2 = lOutColours2Vec4;
        FRAGMENT_COLOUR3 = lOutColours3Vec4;
    }
    #endif
    
#endif

#endif

    #endif
// D_DEPTH_CLEAR

#endif
// defined( D_IMPOSTER ) && defined( _F09_TRANSPARENT ) && ( defined( _F13_UVANIMATION ) || defined( _F14_UVSCROLL ) )
}
 
