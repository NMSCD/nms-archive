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

in vec4 color;
in vec2 texCoord;

uniform sampler2D  baseTexture;
uniform mat4 gWorldViewProjectionMat4;
uniform float gfUserZ;
uniform float gfUserMipLevel;
//varying vec2 depthVals;

layout(location = 0) out vec4 out_color0;

void main()                                             
{                                                           
    vec2 lTexcoords = texCoord;             
    vec4 lAlbedo;

    if (gfUserMipLevel >= 0.0f)
    {
        lAlbedo = textureLod(baseTexture, lTexcoords, gfUserMipLevel);
    }
    else
    {
        lAlbedo = texture2D(baseTexture, lTexcoords);

    }
    out_color0 =  lAlbedo * color;
    //gl_FragDepth = log2( depthVals.x ) * depthVals.y;
}