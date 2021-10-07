#version 440 core
#extension GL_ARB_gpu_shader5 : enable
////////////////////////////////////////////////////////////////////////////////
///
///     @file       Im3dTexFragment.shader.h
///     @author     
///     @date       
///
///     @brief      Immediate Mode Textured Fragment
///
///     Copyright (c) 2009 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////


uniform sampler baseTexture;                                                          
varying vec4 color;                                     
varying vec3 texCoord;   

layout(location = 0) out vec4 out_color0;

void main()                                             
{                                                           
    vec2 lTexcoords = vec2( texCoord.x, texCoord.y );             
 
    out_color0.xyz = texture2D(baseTexture, lTexcoords ).xyz;
    out_color0.w = 1.0;
    //gl_FragColor = vec4(lTexcoords, 1.0); 
    //gl_FragColor = vec4( 0, 0, 1.0, 1.0 );
}