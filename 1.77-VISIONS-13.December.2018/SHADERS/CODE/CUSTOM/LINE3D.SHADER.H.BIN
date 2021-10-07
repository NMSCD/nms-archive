////////////////////////////////////////////////////////////////////////////////
///
///     @file       Im3dFragment.shader.h
///     @author     
///     @date       
///
///     @brief      Immediate Mode Fragment
///
///     Copyright (c) 2009 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "Common/Defines.shader.h"


STATIC_CONST float radius      = 1.0;
STATIC_CONST float invScrRatio = 1280.0 / 720.0;



#define D_PARTICLE_UNIFORMS

#include "Common/CommonVertex.shader.h"
#include "Common/CommonUniforms.shader.h"



//-----------------------------------------------------------------------------
///
///     CustomPerMaterialUniforms
///
///     @brief      CustomPerMaterialUniforms
///
///     Stuff that is only used for these types of meshes.
//-----------------------------------------------------------------------------
struct CustomPerMaterialUniforms
{
BEGIN_SAMPLERBLOCK

    SAMPLER2D( gDiffuseMap );

END_SAMPLERBLOCK

struct CustomPerMeshUniforms
{
    vec4 gColour;
};

//
// This is the SRT buffer that everything gets uploaded to (on PS4). PC just functions as normal.
//
struct UniformBuffer
{
    //DECLARE_PTR( CommonPerMaterialUniforms, mpCommonPerMaterial )    // sematics currently crash the compiler so the parser is hardcoded to look for names.  
    DECLARE_PTR( PerFrameUniforms,          mpPerFrame )    // sematics currently crash the compiler so the parser is hardcoded to look for names.  
    DECLARE_PTR( CommonPerMeshUniforms,     mpCommonPerMesh )        // sematics currently crash the compiler so the parser is hardcoded to look for names.  
    DECLARE_PTR( CustomPerMeshUniforms,     mpCustomPerMesh )        // sematics currently crash the compiler so the parser is hardcoded to look for names.  
    DECLARE_PTR( CustomPerMaterialUniforms, mpCustomPerMaterial )     
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 // D_VERTEX
 //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#if defined( D_VERTEX )

           
//-----------------------------------------------------------------------------
///
///     Input
///
///     @brief  Input
///
//-----------------------------------------------------------------------------
DECLARE_INPUT

	INPUT(  vec4, mkLocalPositionVec4, POSITION0 )

	INPUT( vec4, mkLocalNormalVec4,     TEXCOORD0     )
	INPUT( vec4, mkCustom1Vec4,			TEXCOORD1     )
	INPUT( vec4, mkCustom2Vec4,			TEXCOORD2     )
	INPUT( vec4, mkColourVec4,			TEXCOORD3     )

DECLARE_END                                    


//-----------------------------------------------------------------------------
///
///     Output
///
///     @brief  Output
///
//-----------------------------------------------------------------------------
DECLARE_OUTPUT

	OUTPUT_SCREEN_POSITION
	OUTPUT( vec2,  mfTexCoord, TEXCOORD1 )
    OUTPUT( float, mfFade,     TEXCOORD2 )
    OUTPUT( vec4,  mColourVec4,TEXCOORD4 )
    

DECLARE_END                                       
         


vec4
GetLinePosition(
    vec4 lScreenPositionA,
    vec4 lScreenPositionB,
    vec2 lOffset )
{
    vec2 lineDirProj;

    //  line direction in screen space (perspective division required)
    lineDirProj = radius * normalize( lScreenPositionA.xy/lScreenPositionA.ww - lScreenPositionB.xy/lScreenPositionB.ww );

    // small trick to avoid inversed line condition when points are not on the same side of Z plane
    if( sign(lScreenPositionA.w) != sign(lScreenPositionB.w) )
        lineDirProj = -lineDirProj;
        
    vec4 vMVP     = lScreenPositionA;

    // offset position in screen space along line direction and orthogonal direction
    vMVP.xy += lineDirProj.xy							* lOffset.xx * vec2(1.0,invScrRatio);
    vMVP.xy += lineDirProj.yx	* vec2( 1.0, -1.0 )		* lOffset.yy * vec2(1.0,invScrRatio);

    return vMVP;
}

VERTEX_MAIN_SRT
 {   
    vec4 OffsetUV;
    vec4 laPosition[3];
    vec4 laScreenPosition[3];
    vec3 lTemp = float2vec3(0.0);
    
    laPosition[0] = IN( mkLocalPositionVec4 );
    laPosition[1] = IN( mkLocalNormalVec4 );
    laPosition[2] = IN( mkCustom1Vec4 );

    laScreenPosition[0] = CalcScreenPosFromWorld( lUniforms.mpCommonPerMesh->gViewProjectionMat4, laPosition[0] );
    laScreenPosition[1] = CalcScreenPosFromWorld( lUniforms.mpCommonPerMesh->gViewProjectionMat4, laPosition[1] );
    laScreenPosition[2] = CalcScreenPosFromWorld( lUniforms.mpCommonPerMesh->gViewProjectionMat4, laPosition[2] );

    OffsetUV = IN( mkCustom2Vec4 );

    // scale
    //OffsetUV.y *= lUniforms.mpCommonPerMesh->gaParticleSizeAndRotationsVec4[ 0 ].x;

    vec4 lFinalScreenPosition;

    vec4 vMVP_A  = GetLinePosition( laScreenPosition[0], laScreenPosition[1], vec2( 0.0, OffsetUV.y ) );
    vec4 vMVP_B  = GetLinePosition( laScreenPosition[0], laScreenPosition[2], vec2( 0.0, -OffsetUV.y ) );

    lFinalScreenPosition    = (vMVP_A + vMVP_B) * 0.5;

    vec2 lNormal = normalize( ( (vMVP_A.xy + vMVP_B.xy) * 0.5)  - laScreenPosition[0].xy  );
    
    // Keep the size constant regardless of distance. (pre multiply by homogenous w as it will get divided by W by the hardware after this.
    //float lfScale = ( lUniforms.mpCustomPerMaterial->gLineWidthPixels.x / lUniforms.mpPerFrame->gFrameBufferSizeVec4.y ) * lFinalScreenPosition.w; 
    float lfScale = ( abs( OffsetUV.y ) / lUniforms.mpPerFrame->gFrameBufferSizeVec4.y ) * lFinalScreenPosition.w; 

    lFinalScreenPosition.xy = (laScreenPosition[0].xy + (lNormal * vec2(1.0,invScrRatio) * lfScale ));

    OUT( mfTexCoord ) = OffsetUV.zw;
    OUT(mfFade) = ARRAY_LOOKUP_FP(lUniforms.mpCommonPerMesh, gaParticleSizeAndRotationsVec4, 0).y;

    OUT( mColourVec4 )     = IN( mkColourVec4 );
    SCREEN_POSITION = lFinalScreenPosition;                                    
 }                                                 



#endif

 //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 // D_FRAGMENT
 //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#if defined( D_FRAGMENT )

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2,  mfTexCoord, TEXCOORD1 )
    INPUT( float, mfFade,     TEXCOORD2 )
    INPUT( vec4,  mColourVec4,TEXCOORD4 )

DECLARE_END

    /*
struct cOutput 
{ 
    vec4  mColour : S_TARGET_OUTPUT; 
    float mDepth  : S_DEPTH_OUTPUT; 
};	
*/
//void main( PS_IN In,  out cOutput Out, UniformBuffer lUniforms : S_SRT_DATA  ) 

    FRAGMENT_MAIN_COLOUR_SRT
{                                                  
    vec4 lAlbedo;
    
    lAlbedo = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMaterial, gDiffuseMap ), IN( mfTexCoord ) );
  //  lAlbedo *= In.Color; 
    
    FRAGMENT_COLOUR = lAlbedo * IN( mColourVec4 );//lUniforms.mpCustomPerMesh->gColour;//vec4( 1.0, 0.0, 0.0, 1.0 );//
}      



#endif


