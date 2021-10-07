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

sampler2D ColorTexture : register(s0);           

struct PS_IN                                     
{                                                
    float4 Color : COLOR;                          // Interpolated color from                      
    float2 UV    : TEXCOORD0;                      // UVs
};                                                 // the vertex shader

float4 main( PS_IN In ) : COLOR             
{                                                  
    float4 lAlbedo;                              
    lAlbedo = In.Color;                          
    return In.Color;    
}                                                 