////////////////////////////////////////////////////////////////////////////////
///
///     @file       Im3dVertex.shader.h
///     @author     
///     @date       
///
///     @brief      Immediate Mode Vertex Shader
///
///     Copyright (c) 2009 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

varying vec3 texCoord;                                      
varying vec4 color;                                         
uniform float userZ; 
uniform mat4 gWorldViewProjectionMat4;

void main()                                             
{                                                           
    texCoord      = gl_MultiTexCoord0.xyz;                       
    color         = gl_Color;                                   
    gl_Position   = gWorldViewProjectionMat4 * gl_Vertex;
    gl_Position.z = userZ;                                     
}