
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

float4x4 gWorldViewProjectionMat4 : register(c0);                       
                                                    
struct VS_IN                              
{                                        
    float4 ObjPos   : POSITION;             // Object space position 
    float4 Color    : COLOR;                // Vertex color          
    float2 UV       : TEXCOORD0;            // UVs
};                                      

struct VS_OUT                            
{                                        
    float4 ProjPos  : POSITION;             // Projected space position 
    float4 Color    : COLOR;                //
    float2 UV       : TEXCOORD0;            // UVs
};                                           
                                                    
 VS_OUT main( VS_IN In )                            
 {                                                  
     VS_OUT Out;                                    
     float4 pos = In.ObjPos;                        
     pos.w = 1.0;                                   
     Out.ProjPos = mul( gWorldViewProjectionMat4, pos );    
     Out.Color   = In.Color; 
     Out.UV      = In.UV;
     return Out;                                    
 }                                                 
