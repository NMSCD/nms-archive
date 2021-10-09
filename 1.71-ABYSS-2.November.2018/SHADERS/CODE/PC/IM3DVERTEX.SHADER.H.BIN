#version 440 core
#extension GL_ARB_gpu_shader5 : enable
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



in vec4 mkLocalPositionVec4;
in vec2 mkTexCoordsVec4;
in vec4 mkColourVec4;

out vec4 color;    
out vec2 texCoord;
//varying vec2 depthVals;

uniform mat4 gWorldViewProjectionMat4;
uniform float gfUserZ;

void main()                                             
{   
    float lfFarClipPlane = 5000000.0;
        
    vec4 lScreenSpacePositionVec4 = gWorldViewProjectionMat4 * vec4(mkLocalPositionVec4.xyz,1.0);


    texCoord = mkTexCoordsVec4.xy;
    color    = mkColourVec4;

    //depthVals.x = lScreenSpacePositionVec4.z + gfUserZ;
    //depthVals.y = 1.0 / log2(lfFarClipPlane + 1.0);
    lScreenSpacePositionVec4.z += gfUserZ;
    gl_Position = lScreenSpacePositionVec4;

}