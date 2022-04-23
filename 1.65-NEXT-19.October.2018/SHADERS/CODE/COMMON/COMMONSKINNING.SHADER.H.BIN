////////////////////////////////////////////////////////////////////////////////
///
///     @file       SkinningVertex.shader.h
///     @author     Sean
///     @date       30 Jul 2009
///
///     @brief      SkinningVertex.shader
///
///     Copyright (c) 2009 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
//      Compilation defines 


//-----------------------------------------------------------------------------
//      Include files


//-----------------------------------------------------------------------------
//      Constant Data


//-----------------------------------------------------------------------------
//      Global Data

#if defined D_PLATFORM_OPENGL
	#define JOINT_TYPE    vec4
#elif defined(D_PLATFORM_DX12)
	#define JOINT_TYPE    uint4
#elif defined D_PLATFORM_ORBIS
    #define JOINT_TYPE    int4
#endif

#if defined D_SKINNING_UNIFORMS && defined D_PLATFORM_OPENGL
uniform vec4   gaSkinMatrixRowsVec4    [75 * 3];
uniform vec4   gaPrevSkinMatrixRowsVec4[75 * 3];
#endif

//-----------------------------------------------------------------------------
//      Typedefs and Classes 


//-----------------------------------------------------------------------------
//    Functions 

//-----------------------------------------------------------------------------
///
///     GetJointMat
///
///     @brief      GetJointMat
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
mat4 
GetJointMat( 
    in CommonPerMeshUniforms   lUniforms,
    in int                     liJointIndex )
{

    // Note: This matrix is transposed so vec/mat multiplications need to be done in reversed order
    mat4 lMat4;
    MAT4_SET_COLUMN( lMat4, 0, ARRAY_LOOKUP_FS( lUniforms, gaSkinMatrixRowsVec4, liJointIndex * 3 ) );
    MAT4_SET_COLUMN( lMat4, 1, ARRAY_LOOKUP_FS( lUniforms, gaSkinMatrixRowsVec4, liJointIndex * 3 + 1 ) );
    MAT4_SET_COLUMN( lMat4, 2, ARRAY_LOOKUP_FS( lUniforms, gaSkinMatrixRowsVec4, liJointIndex * 3 + 2 ) );
    MAT4_SET_COLUMN( lMat4, 3, vec4( 0.0, 0.0, 0.0, 1.0 ) );

    return lMat4;
}

//-----------------------------------------------------------------------------
///
///     GetPrevJointMat
///
///     @brief      GetPrevJointMat
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
mat4 
GetPrevJointMat( 
    in CommonPerMeshUniforms   lUniforms,
    in int                     liJointIndex )
{

    // Note: This matrix is transposed so vec/mat multiplications need to be done in reversed order
    mat4 lMat4;
    MAT4_SET_COLUMN( lMat4, 0, ARRAY_LOOKUP_FS( lUniforms, gaPrevSkinMatrixRowsVec4, liJointIndex * 3 ) );
    MAT4_SET_COLUMN( lMat4, 1, ARRAY_LOOKUP_FS( lUniforms, gaPrevSkinMatrixRowsVec4, liJointIndex * 3 + 1 ) );
    MAT4_SET_COLUMN( lMat4, 2, ARRAY_LOOKUP_FS( lUniforms, gaPrevSkinMatrixRowsVec4, liJointIndex * 3 + 2 ) );
    MAT4_SET_COLUMN( lMat4, 3, vec4( 0.0, 0.0, 0.0, 1.0 ) );

    return lMat4;
}


//-----------------------------------------------------------------------------
///
///     CalcSkinningMat
///
///     @brief      CalcSkinningMat
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
mat4 
CalcSkinningMat(
    in CommonPerMeshUniforms  lUniforms,
    in vec4                   lWeightsVec4,
    in JOINT_TYPE             lJointsVec4 )
{
    mat4 laSkinningMatrix;

    laSkinningMatrix      = lWeightsVec4.x * GetJointMat( lUniforms, int( lJointsVec4.x ) );
    if (lWeightsVec4.y > 0.001)    
        laSkinningMatrix += lWeightsVec4.y * GetJointMat( lUniforms, int( lJointsVec4.y ) );
    if (lWeightsVec4.z > 0.001)
        laSkinningMatrix += lWeightsVec4.z * GetJointMat( lUniforms, int( lJointsVec4.z ) );
    if (lWeightsVec4.w > 0.001)
        laSkinningMatrix += lWeightsVec4.w * GetJointMat( lUniforms, abs( int( lJointsVec4.w ) ) );

    return  laSkinningMatrix;
}


//-----------------------------------------------------------------------------
///
///     CalcPrevSkinningMat
///
///     @brief      CalcPrevSkinningMat
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
mat4 
CalcPrevSkinningMat(
    in CommonPerMeshUniforms  lUniforms,
    in vec4                   lWeightsVec4,
    in JOINT_TYPE             lJointsVec4 )
{
    mat4 laSkinningMatrix;

    laSkinningMatrix      = lWeightsVec4.x * GetPrevJointMat( lUniforms, int( lJointsVec4.x ) );
    if (lWeightsVec4.y > 0.001)    
        laSkinningMatrix += lWeightsVec4.y * GetPrevJointMat( lUniforms, int( lJointsVec4.y ) );
    if (lWeightsVec4.z > 0.001)
        laSkinningMatrix += lWeightsVec4.z * GetPrevJointMat( lUniforms, int( lJointsVec4.z ) );
    if (lWeightsVec4.w > 0.001)
        laSkinningMatrix += lWeightsVec4.w * GetPrevJointMat( lUniforms, abs( int( lJointsVec4.w ) ) );

    return  laSkinningMatrix;
}


//-----------------------------------------------------------------------------
///
///     GetSkinningMatVec
///
///     @brief      GetSkinningMatVec
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
mat3 
GetSkinningMatVec( 
    in mat4 lSkinningMat4 )
{
    //return mat3( lSkinningMat4[0].xyz, lSkinningMat4[1].xyz, lSkinningMat4[2].xyz );

    mat3 lMat3;

    MAT3_SET_COLUMN( lMat3, 0, MAT4_GET_COLUMN( lSkinningMat4, 0 ) );
    MAT3_SET_COLUMN( lMat3, 1, MAT4_GET_COLUMN( lSkinningMat4, 1 ) );
    MAT3_SET_COLUMN( lMat3, 2, MAT4_GET_COLUMN( lSkinningMat4, 2 ) );

    return lMat3;
}


//-----------------------------------------------------------------------------
///
///     GetSkinPosition
///
///     @brief      GetSkinPosition
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec4 
GetSkinPosition( 
    in CommonPerMeshUniforms  lUniforms,
    in vec4                   lPositionVec4,
    in vec4                   lWeightsVec4,
    in JOINT_TYPE             lJointsVec4 )
{
    vec4 lSkinnedPosVec4 = MUL(lPositionVec4, GetJointMat( lUniforms, int( lJointsVec4.x ) )) * lWeightsVec4.x;
    if (lWeightsVec4.y > 0.001)
        lSkinnedPosVec4 += MUL(lPositionVec4, GetJointMat( lUniforms, int( lJointsVec4.y ) )) * lWeightsVec4.y;
    if (lWeightsVec4.z > 0.001)
        lSkinnedPosVec4 += MUL(lPositionVec4, GetJointMat( lUniforms, int( lJointsVec4.z ) )) * lWeightsVec4.z;
    if (lWeightsVec4.w > 0.001)
        lSkinnedPosVec4 += MUL(lPositionVec4, GetJointMat( lUniforms, int( lJointsVec4.w ) )) * lWeightsVec4.w;

    // normalization step
    // lSkinnedPosVec4.w = 1.0;
    lSkinnedPosVec4 /= max( lSkinnedPosVec4.w, 0.000001 );

    return lSkinnedPosVec4;
}

//-----------------------------------------------------------------------------
///
///     GetSkinPosition
///
///     @brief      GetSkinPosition
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec4 
GetSkinPosition( 
    in vec4 lPositionVec4, 
    in mat4 lSkinningMat4 )
{
    vec4 lSkinnedPosVec4 = MUL( lPositionVec4, lSkinningMat4 );

    // normalization step
    lSkinnedPosVec4 /= max(lSkinnedPosVec4.w, 0.000001);
    
    return lSkinnedPosVec4;
}


//-----------------------------------------------------------------------------
///
///     GetSkinVector
///
///     @brief      GetSkinVector
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec3 
GetSkinVector( 
    in vec3 lVector3, 
    in mat3 lSkinningMat3 )
{
    return MUL( lVector3, lSkinningMat3 );
}


//-----------------------------------------------------------------------------
///
///     DecompressLdsLo
///
///     @brief      DecompressLdsLo
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec3
DecompressLdsLo(
    in vec4 lds )
{
    vec3 unpck;

    #ifdef D_PLATFORM_OPENGL
    unpck.xy = unpackHalf2x16( floatBitsToInt( lds.x ) );
    #else
    uint bits = floatToIntBits( lds.x );
    unpck.x = f16tof32( bits | 0xffff );
    unpck.y = f16tof32( ( bits >> 16 ) | 0xffff );
    #endif

    unpck.z  = lds.y;
    return unpck;
}

//-----------------------------------------------------------------------------
///
///     DecompressLdsHi
///
///     @brief      DecompressLdsHi
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec3
DecompressLdsHi(
    in vec4 lds )
{
    vec3 unpck;

    #ifdef D_PLATFORM_OPENGL
    unpck.xy = unpackHalf2x16( floatBitsToInt( lds.z ) );
    #else
    uint bits = floatToIntBits( lds.z );
    unpck.x = f16tof32( bits | 0xffff );
    unpck.y = f16tof32( ( bits >> 16 ) | 0xffff );
    #endif

    unpck.z  = lds.w;
    return unpck;
}

//-----------------------------------------------------------------------------
///
///     GetJointLdsWind
///
///     @brief      GetJointLdsWind
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec3 
GetJointLdsWind( 
    in CommonPerMeshUniforms   lUniforms,
    in int                  liJointIndex,
    in vec3                 lPositionVec3,
    in vec3                 lEigCoeffs0,
    in vec3                 lEigCoeffs1,
    in vec4                 lShearValues )
{
    vec3 lOut = DecompressLdsLo( ARRAY_LOOKUP_FS( lUniforms, gaSkinMatrixRowsVec4, liJointIndex * 4 + 0 ) ) * lEigCoeffs0.x;
    lOut     += DecompressLdsHi( ARRAY_LOOKUP_FS( lUniforms, gaSkinMatrixRowsVec4, liJointIndex * 4 + 0 ) ) * lEigCoeffs0.y;
    lOut     += DecompressLdsLo( ARRAY_LOOKUP_FS( lUniforms, gaSkinMatrixRowsVec4, liJointIndex * 4 + 1 ) ) * lEigCoeffs0.z;

    // just check if one of the values is 0, on cpu it's set to nonzero if any of the last three eigens is active
    if( abs( lEigCoeffs1.x ) > 0 )
    {
        lOut     += DecompressLdsHi( ARRAY_LOOKUP_FS( lUniforms, gaSkinMatrixRowsVec4, liJointIndex * 4 + 1 ) ) * lEigCoeffs1.x;
        lOut     += DecompressLdsLo( ARRAY_LOOKUP_FS( lUniforms, gaSkinMatrixRowsVec4, liJointIndex * 4 + 2 ) ) * lEigCoeffs1.y;
        lOut     += DecompressLdsHi( ARRAY_LOOKUP_FS( lUniforms, gaSkinMatrixRowsVec4, liJointIndex * 4 + 2 ) ) * lEigCoeffs1.z;
    }

    if( ARRAY_LOOKUP_FS( lUniforms, gaSkinMatrixRowsVec4, liJointIndex * 4 + 3 ).w > 0 )
    {
        vec3 lBindPos = ARRAY_LOOKUP_FS( lUniforms, gaSkinMatrixRowsVec4, liJointIndex * 4 + 3 ).xyz;

        vec2 lLocalPosVec2 = lPositionVec3.xz - lBindPos.xz;
        vec2 lShear = lShearValues.xy + lShearValues.zw * lLocalPosVec2;

        lOut.xz += lShear;
    }

    return lOut;
}

//-----------------------------------------------------------------------------
///
///     GetSkinPosition
///
///     @brief      GetSkinPosition
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec3 
GetLdsWindPosition( 
    in CommonPerMeshUniforms  lUniforms,
    in vec3                   lPositionVec3,
    in vec4                   lWeightsVec4,
    in JOINT_TYPE             lJointsVec4,
    in vec3                   lEigCoeffs0,
    in vec3                   lEigCoeffs1,
    in vec4                   lShearValues  )
{
    vec3 lWindPosVec3 = lPositionVec3;

    lWindPosVec3 += GetJointLdsWind( lUniforms, int( lJointsVec4.x ), lPositionVec3, lEigCoeffs0, lEigCoeffs1, lShearValues ) * lWeightsVec4.x;

    if( lWeightsVec4.y > 0.0001 )
    {
        lWindPosVec3 += GetJointLdsWind( lUniforms, int( lJointsVec4.y ), lPositionVec3, lEigCoeffs0, lEigCoeffs1, lShearValues ) * lWeightsVec4.y;
    }

    if( lWeightsVec4.z > 0.0001 )
    {
        lWindPosVec3 += GetJointLdsWind( lUniforms, int( lJointsVec4.z ), lPositionVec3, lEigCoeffs0, lEigCoeffs1, lShearValues ) * lWeightsVec4.z;
    }

    if( lWeightsVec4.w > 0.0001 )
    {
        lWindPosVec3 += GetJointLdsWind( lUniforms, int( lJointsVec4.w ), lPositionVec3, lEigCoeffs0, lEigCoeffs1, lShearValues ) * lWeightsVec4.w;
    }


    return lWindPosVec3;
}
