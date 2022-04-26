////////////////////////////////////////////////////////////////////////////////
///
///     @file       CommonFragment.h
///     @author     User
///     @date       
///
///     @brief      CommonFogShader
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 

#ifndef D_COMMONFRAGMENT_H
#define D_COMMONFRAGMENT_H

//-----------------------------------------------------------------------------
//      Include files

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
///
///     CalculateFadeValue
///
///     @brief      Hash
///
///     @param      vec2 lPositionVec2 
///     @return     float
///
//-----------------------------------------------------------------------------
vec3 
DecodeNormalMap(
vec4 lNormalTexVec4 )
{
    lNormalTexVec4 = ( lNormalTexVec4 * ( 2.0 * 255.0 / 256.0 ) ) - 1.0;

    return ( vec3( lNormalTexVec4.r, lNormalTexVec4.g, sqrt( max( 1.0 - lNormalTexVec4.r*lNormalTexVec4.r - lNormalTexVec4.g*lNormalTexVec4.g, 0.0 ) ) ) );
}


//-----------------------------------------------------------------------------
///
///     ReadDualParaboloidMap
///
///     @brief      ReadDualParaboloidMap
///
///     @param      lBackMap 
///     @param      lFrontMap 
///     @param      in vec3 lReflectionVec3
///     @return     vec3
///
//-----------------------------------------------------------------------------
vec4
ReadDualParaboloidMap(
    SAMPLER2DARG( lBackMap ),
    SAMPLER2DARG( lFrontMap ),
    in vec3 lReflectionVec3,
    in int  liMipLevel )
{
    vec2 lEnvCoordsVec2;
    lEnvCoordsVec2.xy = lReflectionVec3.xy / ( 1.0 + abs(lReflectionVec3.z) );
    lEnvCoordsVec2.x =  0.5 * lEnvCoordsVec2.x + 0.5; //bias and scale to correctly sample a d3d texture
    lEnvCoordsVec2.y = -0.5 * lEnvCoordsVec2.y + 0.5;
#ifndef D_PLATFORM_OPENGL
    //lEnvCoordsVec2.y = 1.0 - lFrontCoordsVec2.y;    
#endif

    // Potentially use clever math, and write them both to one image, ala Cascading Shadow Map
    float isBack = lReflectionVec3.z > 0.0 ? 1.0 : 0.0;

    if( isBack > 0.0 )
    {
        lEnvCoordsVec2.x = 1.0 - lEnvCoordsVec2.x;
    }

    vec4 lEnvironmentMapBackVec3 = texture2DLod( lBackMap, lEnvCoordsVec2, float( liMipLevel ) );   // sample the front paraboloid map
    vec4 lEnvironmentMapFrontVec3 = texture2DLod( lFrontMap, lEnvCoordsVec2, float( liMipLevel ) ); // sample the back paraboloid map
    return lEnvironmentMapBackVec3 * isBack + lEnvironmentMapFrontVec3 * (1.0 - isBack);
}

//-----------------------------------------------------------------------------
///
///     GetUpperValue
///
//-----------------------------------------------------------------------------
float 
GetUpperValue( 
    float lValue )
{
    int a = int( lValue * 255 );
    return ( float(a >> 4) / 16.0 );
}

//-----------------------------------------------------------------------------
///
///     GetLowerValue
///
///
//-----------------------------------------------------------------------------
float 
GetLowerValue( 
    float lValue )
{
    int a = int( lValue * 255 );
    float lReturn = float( a & 0xf ) / 16.0;
    lReturn = clamp( lReturn - GetUpperValue( lValue ), 0.0, 1.0 );
    return lReturn;
}

//-----------------------------------------------------------------------------
///
///     GetSparseSampleMip
///
///
//-----------------------------------------------------------------------------
vec4 GetSparseSampleMip(
    SAMPLER2DARG( lSparseMap ),
    vec4 lStatusMip,
    vec2 lCoords,
    vec2 lvBorder,
    vec2 dPdx,
    vec2 dPdy )
{
    float lPagesInvSparseDim = lStatusMip.z;

    dPdx *= (lPagesInvSparseDim);
    dPdy *= (lPagesInvSparseDim);

    lCoords += lvBorder * lStatusMip.w;
    lCoords = lCoords * (lPagesInvSparseDim) + lStatusMip.xy;

    #ifdef D_PLATFORM_OPENGL
    lCoords.y = 1.0 - lCoords.y;
    #endif

    #ifdef D_DOMAIN
    return texture2DLod( lSparseMap, lCoords, 0 );
    #else
    return texture2D( lSparseMap, lCoords );
    //return textureGrad( lSparseMap, lCoords, dPdx, dPdy );
    #endif
}

float GetBorderMul(
    float lfStatusBorder,
    out float lfMip )
{
    lfMip = fract( lfStatusBorder) * 8.0;
    #ifdef D_DOMAIN
    int liMip = int( lfMip );
    return float( ( 1 << liMip ) - 1 );
    #else
    return floor( lfStatusBorder );
    #endif
}

//-----------------------------------------------------------------------------
///
///     GetSparseSample
///
///
//-----------------------------------------------------------------------------
vec4 ReadStatusMap(
    SAMPLER2DARG( lStatusMap ),
    vec2 lvStatusCoords,
    float lfMip )
{
    #ifdef D_PLATFORM_PC_LOWEND
    float lfInvMip = 1.0 / exp2( lfMip );
    lvStatusCoords.y = lvStatusCoords.y * ( lfInvMip );
    lvStatusCoords.x = lvStatusCoords.x * ( lfInvMip * 0.5 ) + ( 1.0 - lfInvMip );
    return texture2DLod( lStatusMap, lvStatusCoords, 0.0 );
    #else
    return texture2DLod( lStatusMap, lvStatusCoords, lfMip );
    #endif
}

vec4 GetSparseSample(
    SAMPLER2DARG( lSparseMap ),
    SAMPLER2DARG( lStatusMap ),
    vec2 lCoords01,
    vec2 lvBorder,
    vec2 dPdx,
    vec2 dPdy,
    uvec2 lStatusDim,
    float lfDesiredMip,
    out float lfMaxMip )
{
    float lfMipBlend = fract( lfDesiredMip );

    // 'brilinear' blending -- only blend when close to the next mip down
    lfMipBlend = saturate( ( lfMipBlend - 0.5 ) * 2.0 );

    lfMaxMip = 0.0;

    vec2 lvStatusCoords = lCoords01.xy + lvBorder * 2.0;

    if( lfMipBlend == 0.0 )
    {
        float liLowMip  = floor( lfDesiredMip );
        vec2 lvMipCoords = lCoords01;
        vec4 lStatusMip = ReadStatusMap( SAMPLER2DPARAM( lStatusMap ), lvStatusCoords, liLowMip );
        lStatusMip.w = GetBorderMul( lStatusMip.w, lfMaxMip );

        return GetSparseSampleMip( SAMPLER2DPARAM( lSparseMap ), lStatusMip, lvMipCoords, lvBorder, dPdx, dPdy );
    }
    else if( lfMipBlend == 1.0 )
    {
        float liHighMip = ceil( lfDesiredMip );
        vec2 lvMipCoords = lCoords01;
        vec4 lStatusMip = ReadStatusMap( SAMPLER2DPARAM( lStatusMap ), lvStatusCoords, liHighMip );
        lStatusMip.w = GetBorderMul( lStatusMip.w, lfMaxMip );

        return GetSparseSampleMip( SAMPLER2DPARAM( lSparseMap ), lStatusMip, lvMipCoords, lvBorder, dPdx, dPdy );
    }
    else
    {
        float liLowMip  = floor( lfDesiredMip );
        vec2 lvMipCoords = lCoords01;
        vec4 lStatusLowMip  = ReadStatusMap( SAMPLER2DPARAM( lStatusMap ), lvStatusCoords, liLowMip );
        lStatusLowMip.w  = GetBorderMul( lStatusLowMip.w, lfMaxMip );
        vec4 lvRead  = GetSparseSampleMip( SAMPLER2DPARAM( lSparseMap ), lStatusLowMip, lvMipCoords, lvBorder, dPdx, dPdy );

        if( lfMaxMip <= liLowMip )
        {
            vec4 lStatusHighMip = ReadStatusMap( SAMPLER2DPARAM( lStatusMap ), lvStatusCoords, liLowMip + 1.0 );
            lStatusHighMip.w = GetBorderMul( lStatusHighMip.w, lfMaxMip );

            vec4 lvHighRead = GetSparseSampleMip( SAMPLER2DPARAM( lSparseMap ), lStatusHighMip, lvMipCoords, lvBorder, dPdx, dPdy );
            lvRead = mix( lvRead, lvHighRead, lfMipBlend );
        }
        return lvRead;
    }

    return vec4(0,0,0,0);
}

//-----------------------------------------------------------------------------
///
///     ReadTextureCache
///
///
//-----------------------------------------------------------------------------
void ReadTextureCache(
    vec2 lCacheCoords01,
    vec2 lvBorder,
    vec2 dPdx,
    vec2 dPdy,
    ivec2 lResolution,
    float lfDesiredMip,
    SAMPLER2DARG( lSparseDiffMap ),
    SAMPLER2DARG( lSparseNormMap ),
    SAMPLER2DARG( lStatusDiffMap ),
    SAMPLER2DARG( lStatusNormMap ),
    out vec3 lFragmentColourVec3,
    inout vec3 lNormalVec3,
    out vec4 lMasksVec4,
    out float lfHeight )
{
    float lFragMipPresent = 0, lNormMipPresent = 0;
    vec4 lFragmentColourVec4 = GetSparseSample( SAMPLER2DPARAM( lSparseDiffMap ), SAMPLER2DPARAM( lStatusDiffMap ), lCacheCoords01, lvBorder, dPdx, dPdy, lResolution, lfDesiredMip, lFragMipPresent );
    vec4 lNormalVec4 = GetSparseSample( SAMPLER2DPARAM( lSparseNormMap ), SAMPLER2DPARAM( lStatusNormMap ), lCacheCoords01, lvBorder, dPdx, dPdy, lResolution, lfDesiredMip, lNormMipPresent );

    lFragmentColourVec3 = lFragmentColourVec4.rgb;
    lNormalVec3 = lNormalVec4.rag * 2.0 - 1.0;
    lNormalVec3 = (abs(lNormalVec3) * lNormalVec3) * 4.0;


    lMasksVec4.x = lNormalVec4.g;
    lMasksVec4.y = ( lNormalVec4.b * lNormalVec4.b * 2.0 );
    lMasksVec4.z = 0.0;
    lMasksVec4.w = 0.0;

    lfHeight = lFragmentColourVec4.a;
}

//-----------------------------------------------------------------------------
///
///     WriteTextureCacheFeedback
///
///
//-----------------------------------------------------------------------------
void WriteTextureCacheFeedback(
    vec2 lCacheCoords,
    ivec2 lResolution,
    float lfDesiredMip,
    RWINTIMAGE2DARG( r32i, lFeedbackMap ),
    uint liFeedbackIndex )
{
    ivec2 lStatusCoords   = ivec2( floor( vec2(lResolution.xy) * lCacheCoords ) );

    ivec2 lFeedbackSample = lStatusCoords;

    if( WaveIsFirstLane() )
    {
        int liHighDetailMip = int( floor( max(0.0, lfDesiredMip) ) );
        int lFeedbackMip = min( 5, liHighDetailMip );

        #ifdef D_PLATFORM_OPENGL
        ivec4 lStoreVal = ivec4( 1, 0, 0, 0 );
        #else
        int lStoreVal = 1;
        #endif

        // always mark the bottom mip as a "this was touched" marker
        int lFbEnd = lResolution.x * lResolution.y * 2;
        imageStore( lFeedbackMap, ivec2( lFbEnd - 1, liFeedbackIndex ), lStoreVal );

        if( liHighDetailMip < 5 )
        {
            lFeedbackSample.xy >>= liHighDetailMip;
            lResolution.xy >>= liHighDetailMip;
            int lMipStart = lFbEnd - lResolution.x * lResolution.y * 2;
            imageStore( lFeedbackMap, ivec2( lMipStart + lFeedbackSample.y * lResolution.x + lFeedbackSample.x, liFeedbackIndex ), lStoreVal );

            lFeedbackMip += 1;
            lFeedbackSample.xy /= 2;
            lResolution.xy /= 2;

            lMipStart = lFbEnd - lResolution.x * lResolution.y * 2;
            imageStore( lFeedbackMap, ivec2( lMipStart + lFeedbackSample.y * lResolution.x + lFeedbackSample.x, liFeedbackIndex ), lStoreVal );
        }
    }

}

#endif

