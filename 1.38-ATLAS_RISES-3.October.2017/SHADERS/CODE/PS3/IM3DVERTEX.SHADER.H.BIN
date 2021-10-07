
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

struct VS_OUT_DEBUG                                              
{                                                               
    float4 mPosition	: POSITION;                             
    float4 mColour		: COLOR;                                 
    float2 mUV			: TEXCOORD0;                             
};                                                               

VS_OUT_DEBUG main(                                               
    float4 lPosition	: POSITION, 
    float4 lColour      : COLOR,  
    float2 lUV		    : TEXCOORD,                   
    UNIFORM float4x4      gWorldViewProjectionMat4 )                     
{                                                                
    VS_OUT_DEBUG Out;                                            

    Out.mColour		= lColour;                                  
    Out.mUV			= lUV;                                      
    Out.mPosition	= mul( gWorldViewProjectionMat4, lPosition );   

    return Out;                                                 
}                                                               