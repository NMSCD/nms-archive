
////////////////////////////////////////////////////////////////////////////////
///
///     @file       Im3dVertex.shader.h
///     @author     
///     @date       
///
///     @brief      Immediate Mode Vertex
///
///     Copyright (c) 2009 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////


#ifndef D_VERTEX
#define D_VERTEX
#endif

#include "Common/Defines.shader.h"


struct CommonPerMeshUniforms
{
    vec4 gImmediateRenderParams;
    mat4 gWorldViewProjectionMat4;
	
BEGIN_SAMPLERBLOCK

	SAMPLER2D( gColourMap );

END_SAMPLERBLOCK

struct UniformBuffer
{
     DECLARE_PTR( CommonPerMeshUniforms, mpCommonPerMesh )       /*: PER_MESH*/
};


                                                    
DECLARE_INPUT

    vec4 ObjPos   : POSITION0;            // Object space position      
    vec2 UV       : TEXCOORD0;            // UVs
    vec4 Color    : COLOR;                // Vertex color     

DECLARE_END
	
DECLARE_OUTPUT

     vec4 ProjPos  : S_POSITION;           // Projected space position 
     vec2 UV       : TEXCOORD0;            // UVs
     vec4 Color    : COLOR;                // Vertex color  
     vec2 mDepthVal : TEXCOORD1;
DECLARE_END                                           
    
VERTEX_MAIN_SRT
{          
    float lfFarClipPlane = 5000000.0;
     
    vec4 pos = IN( ObjPos );                        
    pos.w = 1.0;       

    vec4 lScreenSpacePositionVec4 = MUL( lUniforms.mpCommonPerMesh->gWorldViewProjectionMat4, pos ); 

    OUT( mDepthVal ).x = lScreenSpacePositionVec4.z+ lUniforms.mpCommonPerMesh->gImmediateRenderParams.y;
    OUT( mDepthVal ).y = 1.0 / log2( lfFarClipPlane + 1.0 );

    OUT( ProjPos ) = lScreenSpacePositionVec4; 
    OUT( Color )   = IN( Color ); 
    OUT( UV )      = IN( UV );      
     
 }                                                 


