////////////////////////////////////////////////////////////////////////////////
///
///     @file       UberFragmentShader.h
///     @author     User
///     @date       
///
///     @brief      UberFragmentShader
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
//      Compilation defines 




//-----------------------------------------------------------------------------
//    Functions



//-----------------------------------------------------------------------------
///
///     WriteOutput
///
///     @brief      WriteOutput
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
void
WriteOutput(
    out vec4                      lOutColours0Vec4,
    out vec4                      lOutColours1Vec4,
    out vec4                      lOutColours2Vec4,
    out vec4                      lOutColours3Vec4,
    out vec4                      lOutColours4Vec4,
    in  PerFrameUniforms          lPerFrameUniforms,
    in  CommonPerMeshUniforms     lMeshUniforms,
    in  CustomPerMaterialUniforms lCustomUniforms,
    in  vec4                      lColourVec4,
    in  vec3                      lPositionVec3,
    in  vec3                      lNormalVec3,
    in  int                       liMaterialID,
    in  float                     lfMetallic,
    in  float                     lfRoughness,
    in  float                     lfSubsurface,
    in  float                     lfGlow,
    in  vec4                      lScreenSpacePositionVec4,
    in  vec4                      lPrevScreenPositionVec4,
    in  float                     lfPixelDepth,
    in  bool                      lbFrontFacing)
{
    //-----------------------------------------------------------------------------
    ///
    ///     Output
    ///
    //-----------------------------------------------------------------------------	
    #if defined( D_IMPOSTER_COLOUR ) 
    {
        lOutColours0Vec4 = vec4(lColourVec4.x, lColourVec4.y, lColourVec4.z ,1.0);
        lOutColours1Vec4 = float2vec4(0.0);
        lOutColours2Vec4 = float2vec4(0.0);
        lOutColours3Vec4 = float2vec4(0.0);
        lOutColours4Vec4 = float2vec4(0.0);
    }
    #elif defined( D_IMPOSTER_NORMAL ) 
    {
        vec3 lEncodedNormal;

        lEncodedNormal = lNormalVec3;

        lEncodedNormal    = ( lEncodedNormal + 1.0 ) * 0.5;
        lOutColours0Vec4 = vec4( lEncodedNormal.x, lEncodedNormal.y, lEncodedNormal.z, 1.0 );
        lOutColours1Vec4 = float2vec4(0.0);
        lOutColours2Vec4 = float2vec4(0.0);
        lOutColours3Vec4 = float2vec4(0.0);
        lOutColours4Vec4 = float2vec4(0.0);
    }
    #elif defined( D_IMPOSTER_MASK )
    {
#ifdef _F61_CLAMP_AMBIENT
        float lfClampAmbient = 1.0;
#else
        float lfClampAmbient = 0.1;
#endif

        lOutColours0Vec4 = vec4( lfSubsurface, lfRoughness, lfMetallic, lfClampAmbient );
        lOutColours1Vec4 = float2vec4(0.0);
        lOutColours2Vec4 = float2vec4(0.0);
        lOutColours3Vec4 = float2vec4(0.0);
        lOutColours4Vec4 = float2vec4(0.0);
    }
    #else
    {
        #if !defined( D_ATTRIBUTES )
            lOutColours0Vec4     = vec4( lColourVec4.xyz, lfGlow );
            lOutColours1Vec4     = float2vec4(0.0);
            lOutColours2Vec4     = float2vec4(0.0);
            lOutColours3Vec4     = float2vec4(0.0);
            lOutColours4Vec4     = float2vec4(0.0);
        #else

        #if defined( D_OUTPUT_MOTION_VECTORS )
            vec2  lScreenSpaceMotionVec2 = lPrevScreenPositionVec4.xy / lPrevScreenPositionVec4.w - lScreenSpacePositionVec4.xy / lScreenSpacePositionVec4.w;
        #else   
            vec2  lScreenSpaceMotionVec2 = vec2( 1.0, 0.0 );
        #endif 

            vec4  lBuffer0_Vec4;
            vec4  lBuffer1_Vec4;
            vec4  lBuffer2_Vec4;
            vec4  lBuffer3_Vec4;
            vec4  lBuffer4_Vec4;

            EncodeGBuffer(
                lPerFrameUniforms.gClipPlanesVec4,
                lPerFrameUniforms.gViewPositionVec3,
                lPerFrameUniforms.gViewMat4,
                lBuffer0_Vec4,
                lBuffer1_Vec4,
                lBuffer2_Vec4,
                lBuffer3_Vec4,
                lBuffer4_Vec4,
                lColourVec4.xyz,
                lPositionVec3,
                lNormalVec3,
                liMaterialID,
                lfRoughness,
                lfMetallic,
                lfSubsurface,
                lfGlow,
                lScreenSpaceMotionVec2 );

            lOutColours0Vec4 = lBuffer0_Vec4;
            lOutColours1Vec4 = lBuffer1_Vec4;
            lOutColours2Vec4 = lBuffer2_Vec4;
            lOutColours3Vec4 = lBuffer3_Vec4;
            lOutColours4Vec4 = lBuffer4_Vec4;

        #endif
    }
    #endif
}
