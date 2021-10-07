////////////////////////////////////////////////////////////////////////////////
///
///     @file       CommonParticle.h
///     @author     User
///     @date       
///
///     @brief      CommonParticleShader
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 

#ifndef D_COMMONPARTICLE_H
#define D_COMMONPARTICLE_H

//-----------------------------------------------------------------------------
//      Include files


//-----------------------------------------------------------------------------
//      Global Data

#include "Common/CommonUniforms.shader.h"

//-----------------------------------------------------------------------------
//      Typedefs and Classes 


//-----------------------------------------------------------------------------
//    Functions



//-----------------------------------------------------------------------------
///
///     GetParticlePos
///
///     @brief      GetParticlePos
///
///     @param      void
///     @return     
///
//-----------------------------------------------------------------------------
vec4 
GetParticlePos(
    in PerFrameUniforms      lPerFrameUniforms,
    in CommonPerMeshUniforms lMeshUniforms,
    in vec3                  lVertexPositionVec3,
    in int                   liParticleID,
    in int                   liParticleCorner )
{
    
    mat4  lCameraMat4 = mat4( 1.0, 0.0, 0.0, 0.0, 
                              0.0, 1.0, 0.0, 0.0,
                              0.0, 0.0, 1.0, 0.0,
                              0.0, 0.0, 0.0, 1.0 );
    
    vec4  lRotatedPositionVec4;    
    vec4  lAlignedPositionVec4;
    vec3  lParticlePositionVec3;
    vec3  lCornerPositionVec3;
    float lfRotation;
    float lfSin;
    float lfCos;
    float lfScale;

    lCameraMat4 = lPerFrameUniforms.gCameraMat4;

    vec3 lRightVec3 = lMeshUniforms.gWorldMat4[0].xyz;

    lfScale    = ARRAY_LOOKUP_FS(lMeshUniforms, gaParticleSizeAndRotationsVec4, liParticleID).x * sqrt(dot(lRightVec3, lRightVec3));
    lfRotation = ARRAY_LOOKUP_FS(lMeshUniforms, gaParticleSizeAndRotationsVec4, liParticleID).y * 0.0174532925;
    
#ifdef _F26_STRETCHY_PARTICLE
    vec3 lStretchDirVec3    = vec3( ARRAY_LOOKUP_FS(lMeshUniforms, gaParticleSizeAndRotationsVec4, liParticleID).z, ARRAY_LOOKUP_FS(lMeshUniforms, gaParticleSizeAndRotationsVec4, liParticleID).w, ARRAY_LOOKUP_FS(lMeshUniforms, gaParticlePositionsVec4, liParticleID).w );    
    vec2 lScreenStretchVec2 = MUL( lMeshUniforms.gViewProjectionMat4, vec4( lStretchDirVec3, 0.0 ) ).xy; // project the world stretch dir into screen space
    lfRotation = atan2( lScreenStretchVec2.x, lScreenStretchVec2.y );
#endif

    lfSin = sin( lfRotation );
    lfCos = cos( lfRotation );

    mat4 lRotationMat4 = mat4( lfCos, -lfSin, 0, 0,
                               lfSin,  lfCos, 0, 0,
                               0,  0, 1, 0,
                               0,  0, 0, 1 );

    // lCameraMatrix[3] = vec4( ARRAY_LOOKUP_FS( lMeshUniforms, gaParticlePositionsVec4, liParticleID ).xyz, 1.0 );
    // Once alex's math stuff is in this can be removed.
    lCameraMat4[3] = vec4( 0.0, 0.0, 0.0, 1.0 );
    lCameraMat4[0][3] = 0.0;
    lCameraMat4[1][3] = 0.0;
    lCameraMat4[2][3] = 0.0;

    // Particle Position
    lParticlePositionVec3    = ARRAY_LOOKUP_FS( lMeshUniforms, gaParticlePositionsVec4, liParticleID ).xyz;
   
    // Corner Position
    lCornerPositionVec3      = (ARRAY_LOOKUP_FS(lMeshUniforms, gaParticleCornersVec4, liParticleCorner).xyz * lfScale);
    
    lRotatedPositionVec4     = MUL( lRotationMat4, vec4( lCornerPositionVec3, 1.0 ) );
    
    lAlignedPositionVec4     = MUL( lCameraMat4, lRotatedPositionVec4 );
    
    vec4 lFinalPositionVec4  = vec4( lAlignedPositionVec4.xyz + lParticlePositionVec3, 1.0 );
    
    // stretching
    #ifdef _F26_STRETCHY_PARTICLE
        vec3 lPrevPosVec3    = lParticlePositionVec3 + lStretchDirVec3.xyz;
        vec3 lPrevCornerVec3 = lPrevPosVec3 + lAlignedPositionVec4.xyz;

        if( length( lStretchDirVec3.xyz ) >= length( lPrevCornerVec3 - lParticlePositionVec3 ) )
        {
            lFinalPositionVec4 = vec4( lFinalPositionVec4.xyz - lStretchDirVec3.xyz, 0.0 );
        }        
    #endif

    return lFinalPositionVec4;    
}

#endif