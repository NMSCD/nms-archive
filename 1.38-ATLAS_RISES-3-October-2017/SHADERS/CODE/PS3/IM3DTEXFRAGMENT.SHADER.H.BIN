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

struct FS_IN                                                     
{                                                           
    float4 mColour		: COLOR;                             
    float2 mUV			: TEXCOORD0;                         
};                                                          
                                                                                                                 
sampler2D  gColourMap : register(s0);                             
                                                            
float4 main( FS_IN In ) : COLOR                             
{                                        
    float2 lUVs    = float2( In.mUV.x, 1.0 - In.mUV.y ); 
    float4 lColour = In.mColour * tex2D( gColourMap, lUVs );	
    return lColour;							
}            


