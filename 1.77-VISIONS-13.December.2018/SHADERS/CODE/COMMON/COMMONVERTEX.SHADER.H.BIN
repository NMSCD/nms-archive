////////////////////////////////////////////////////////////////////////////////
///
///     @file       CommonVertex.h
///     @author     User
///     @date       
///
///     @brief      CommonVertex
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 


//-----------------------------------------------------------------------------
//      Include files


//-----------------------------------------------------------------------------
//      Global Data

#include "Common/CommonUniforms.shader.h"
#include "Common/Common.shader.h"

STATIC_CONST float kfScaleUpFactor = 1.001;

//-----------------------------------------------------------------------------
///
///     CalcWorldPos
///
///     @brief      CalcWorldPos
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec4 
CalcWorldPos( 
    in mat4 lWorldMat4,
    in vec4 lLocalPositionVec4 )
{
    return MUL( lWorldMat4, lLocalPositionVec4 );
}

#ifdef D_INSTANCE
//-----------------------------------------------------------------------------
///
///     CalcWorldPosInstanced
///
///     @brief      CalcWorldPosInstanced
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec4 
CalcWorldPosInstanced( 
    in mat4 lWorldMat4,
    in mat4 lWorldInstanceMat4,
    in vec4 lLocalPositionVec4 )
{
    vec4 lWorldPositionVec4 = MUL( lWorldMat4, lLocalPositionVec4 );
    return MUL( lWorldInstanceMat4, lWorldPositionVec4 );
}
#endif



//-----------------------------------------------------------------------------
///
///     CalcScreenPosFromWorld
///
///     @brief      CalcScreenPosFromWorld
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec4 
CalcScreenPosFromWorld( 
    in mat4 lViewMatrix4,
    in mat4 lProjectionMatrix4,
    in vec4 lWorldPositionVec4 )
{    
    vec4 lViewPositionVec4 = MUL( lViewMatrix4, lWorldPositionVec4 );
    return MUL( lProjectionMatrix4, lViewPositionVec4 );
}


//-----------------------------------------------------------------------------
///
///     CalcScreenPosFromWorld
///
///     @brief      CalcScreenPosFromWorld
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec4 
CalcScreenPosFromWorld( 
    in mat4 lViewProjectionMatrix4,
    in vec4 lWorldPositionVec4 )
{    
    return MUL( lViewProjectionMatrix4, lWorldPositionVec4 );
}

//-----------------------------------------------------------------------------
///
///     CalcScreenPosFromLocal
///
///     @brief      CalcScreenPosFromLocal
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec4 
CalcScreenPosFromLocal( 
    in vec4 lLocalPositionVec4 )
{    
    mat4 lTemp;
    vec4 lWorldPositionVec4 = CalcWorldPos( lTemp, lLocalPositionVec4 );
    return CalcScreenPosFromWorld( lTemp, lTemp, lWorldPositionVec4 );
}

//-----------------------------------------------------------------------------
///
///     CalcScreenPosFromLocal
///
///     @brief      CalcScreenPosFromLocal
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec4 
CalcScreenPosFromLocal( 
    in mat4 lWorldViewProjectionMatrix4,
    in vec4 lLocalPositionVec4 )
{    
    return MUL( lWorldViewProjectionMatrix4, lLocalPositionVec4 );
}

#ifdef _F19_BILLBOARD 

//-----------------------------------------------------------------------------
///
///     CalcBillboardPos
///
///     @brief      CalcBillboardPosFromDirection
///
///     @param      in vec4 lDirection
///     @param      in vec4 lLocalPositionVec4
///     @param      in float lfSphereFactor
///     @param      in mat4 lWorldMat4
///     @param      out vec4 lOutWorldPositionVec4
///     @return     vec4
///
//-----------------------------------------------------------------------------
vec4 
CalcBillboardPos( 
    in mat4   lCameraMatrix4,
    in vec4   lLocalPositionVec4,
    in float  lfSphereFactor,
    in mat4   lWorldMat4,
    in vec4   lShearParamsVec4,
    out vec4  lOutWorldPositionVec4 ) 
{
    vec3 lUpVector;
    vec3 lCamUp    = MAT4_GET_COLUMN( lCameraMatrix4, 1 );
    vec3 lCamRight = MAT4_GET_COLUMN( lCameraMatrix4, 0 );
    vec3 lCamAt    = MAT4_GET_COLUMN( lCameraMatrix4, 2 );
    vec3 lCamPos   = MAT4_GET_COLUMN( lCameraMatrix4, 3 );
    vec3 lWorldPos = MAT4_GET_COLUMN( lWorldMat4, 3 );
    vec3 lMatUpVector = MAT4_GET_COLUMN( lWorldMat4, 1 );

#if defined( _F44_IMPOSTER ) || (defined( _F12_BATCHED_BILLBOARD ) && defined( D_SHADOW_VERTEX ) )
    // imposters will keep pointing 'up' even if the camera rolls
    lCamUp    = normalize( MAT4_GET_COLUMN( lWorldMat4, 1 ) );
    lCamRight = normalize( cross( lCamUp, lCamAt ) );

    // make sure that an orthonormal frame is created
    // ==> make these 3 vectors pairwise orthogonal to each other
    lCamAt    = normalize( cross( lCamRight, lCamUp ) );
    lUpVector = lCamUp;
#else
    lCamRight = MAT4_GET_COLUMN( lCameraMatrix4, 0 );
    lCamAt    = MAT4_GET_COLUMN( lCameraMatrix4, 2 );
    lCamUp    = MAT4_GET_COLUMN( lCameraMatrix4, 1 );

#ifdef D_INSTANCE
    // undo the shear... I am not sure this is a super great way to do this...
    lMatUpVector = MAT4_GET_COLUMN( lWorldMat4, 1 ) - lShearParamsVec4.x * MAT4_GET_COLUMN( lWorldMat4, 0 ) - lShearParamsVec4.y * MAT4_GET_COLUMN( lWorldMat4, 2 );
#endif
    
    lUpVector = mix( lMatUpVector, lCamUp, lfSphereFactor );

    lUpVector      = normalize( lUpVector );

#endif

    //                                | Cylinder Values                                       | Sphere Values      
    mat3 lBillboardMat3;    

#ifdef _F46_BILLBOARD_AT
    vec3 lWorldAt     = MAT4_GET_COLUMN( lWorldMat4, 2 );
    MAT3_SET_COLUMN( lBillboardMat3, 2, lWorldAt );
    MAT3_SET_COLUMN( lBillboardMat3, 1, normalize( cross( lWorldAt, ( lCamPos - lWorldPos ) ) ) );
    MAT3_SET_COLUMN( lBillboardMat3, 0, normalize( cross( MAT3_GET_COLUMN( lBillboardMat3, 1 ), MAT3_GET_COLUMN( lBillboardMat3, 2 ) ) ) );
#else
    MAT3_SET_COLUMN( lBillboardMat3, 2, normalize( mix( normalize( lCamPos - lWorldPos ), lCamAt, lfSphereFactor*lfSphereFactor*lfSphereFactor) ) ); 
    MAT3_SET_COLUMN( lBillboardMat3, 0, normalize( mix( cross( lUpVector, MAT3_GET_COLUMN( lBillboardMat3, 2 ) ),  lCamRight, lfSphereFactor) ) );
    MAT3_SET_COLUMN( lBillboardMat3, 1, lUpVector );
#endif

    vec4 lBillboardPositionVec4;
    lBillboardPositionVec4.xyz = MUL( lBillboardMat3, lLocalPositionVec4.xyz ).xyz; //transform by billboard's orthonormal basis
    lBillboardPositionVec4.w   = 1.0;

    lOutWorldPositionVec4 = lBillboardPositionVec4;
    vec3 lWorldMatScale = vec3( length( MAT4_GET_COLUMN_VEC4( lWorldMat4, 0 ) ), length( lMatUpVector ), length( MAT4_GET_COLUMN_VEC4( lWorldMat4, 2 ) ) );
    lOutWorldPositionVec4.xyz *= lWorldMatScale;
    lOutWorldPositionVec4.xyz += lWorldPos.xyz;

    mat4 lOriginModelInvMat4;

    lOriginModelInvMat4      = Inverse( lWorldMat4 );
    MAT4_SET_COLUMN( lOriginModelInvMat4, 0, normalize( MAT4_GET_COLUMN_VEC4( lOriginModelInvMat4, 0 ) ) );
    MAT4_SET_COLUMN( lOriginModelInvMat4, 1, normalize( MAT4_GET_COLUMN_VEC4( lOriginModelInvMat4, 1 ) ) );
    MAT4_SET_COLUMN( lOriginModelInvMat4, 2, normalize( MAT4_GET_COLUMN_VEC4( lOriginModelInvMat4, 2 ) ) );

    MAT4_SET_POS( lOriginModelInvMat4, vec4( 0.0, 0.0, 0.0, 1.0 ) );
        
    lBillboardPositionVec4 = MUL( lOriginModelInvMat4, lBillboardPositionVec4 );

#ifdef D_INSTANCE
    lOutWorldPositionVec4.xyz += lLocalPositionVec4.y * ( MAT4_GET_COLUMN( lWorldMat4, 0 ) * lShearParamsVec4.x + MAT4_GET_COLUMN( lWorldMat4, 2 ) * lShearParamsVec4.y );
#endif

    return lBillboardPositionVec4;
}

#ifdef _F12_BATCHED_BILLBOARD

//-----------------------------------------------------------------------------
///
///     CalcBatchedBillboardMat4
///
///     @brief      CalcBatchedBillboardMat4
///
///     @param      in vec4 lTexCoords
///     @param      in mat4 lWorldMat
///     @return     mat4
///
//-----------------------------------------------------------------------------
mat4
CalcBatchedBillboardMat4(
    in  mat4 lGlobalWorldMat,
    in  vec4 lTexCoords, 
    in  vec4 lLocalPosVec4,
    in  mat4 lWorldMat4,
    out   vec4 lOutCenterVec4 )
{
    vec2 lBillboardPositionVec2;
    vec4 lLocalPositionVec4;
    vec4 lCenterPositionVec4;
    mat4 lBillboardWorldMat4;

    lLocalPositionVec4 = lLocalPosVec4;

    // Calculate center position as offset from local position, using UVs
    lBillboardPositionVec2 = lTexCoords.xy;
    lBillboardPositionVec2 = vec2( lBillboardPositionVec2.x - 0.5,  0.5 - lBillboardPositionVec2.y);
    
#ifdef D_INSTANCE
    // Scale
    lBillboardPositionVec2.x *= length( MAT4_GET_COLUMN( lGlobalWorldMat, 0 ) );
    lBillboardPositionVec2.y *= length( MAT4_GET_COLUMN( lGlobalWorldMat, 1 ) );

    lLocalPositionVec4.xyz = MUL( lGlobalWorldMat, lLocalPositionVec4 ).xyz;
#endif

    // Calculate offset of center position from model position
    lCenterPositionVec4.xy = lLocalPositionVec4.xy - lBillboardPositionVec2.xy;
    lCenterPositionVec4.z  = lLocalPositionVec4.z;

    lBillboardWorldMat4  = lWorldMat4;

    vec4 lTransPos = MUL( lBillboardWorldMat4, vec4(lCenterPositionVec4.xyz, 1.0));

    MAT4_SET_POS( lBillboardWorldMat4, lTransPos );

    lOutCenterVec4 = vec4(lBillboardPositionVec2.xy, 0.0, 1.0);

    return lBillboardWorldMat4;
}

#endif

//-----------------------------------------------------------------------------
///
///     CalcBillboardValues
///
///     @brief      CalcBillboardValues
///
///     @param      in  vec4 lLocalPositionVec4
///     @param      in  mat4 lWorldMat4
///     @param      in  vec4 lTexCoordsVec4
///     @param      out vec4 lOutLocalPositionVec4
///     @param      out vec4 lOutWorldPositionVec4
///     @param      out vec3 lOutLocalNormalVec3
///     @param      out vec3 lOutWorldNormalVec3
///     @return     void
///
//-----------------------------------------------------------------------------
void 
CalcBillboardValues(
    in  mat4   lCameraMatrix,
    in  mat4   lGlobalWorldMat,
    in  vec4   lLocalPositionVec4,   
    in  vec4   lCustomParams01Vec4,   
    in  mat4   lWorldMat4,
    in  vec4   lTexCoordsVec4,
    in  vec4   lShearParamsVec4,
    out   vec4   lOutLocalPositionVec4,
    out   vec4   lOutWorldPositionVec4,
    out   vec3   lOutLocalNormalVec3,
    out   vec3   lOutWorldNormalVec3 )
{
#ifdef D_INSTANCE
    mat4 lCombinedWorldMat4 = MUL( lWorldMat4, lGlobalWorldMat );
#else    
    mat4 lCombinedWorldMat4 = lGlobalWorldMat;
#endif    

#ifdef D_IMPOSTER
    lCameraMatrix[ 2 ] = -lCameraMatrix[ 2 ];
#endif
    
#ifdef _F12_BATCHED_BILLBOARD
    {
        mat4 lBillboardWorldMat4;
        vec4 lBillboardLocalCenterVec4;
        lBillboardWorldMat4 = CalcBatchedBillboardMat4( lGlobalWorldMat,  lTexCoordsVec4, lLocalPositionVec4, lWorldMat4, lBillboardLocalCenterVec4 );

        lOutLocalPositionVec4  = CalcBillboardPos( lCameraMatrix, lBillboardLocalCenterVec4, lCustomParams01Vec4.x, lBillboardWorldMat4, lShearParamsVec4, lOutWorldPositionVec4 );
        lOutWorldPositionVec4  = MUL( lBillboardWorldMat4, vec4(lOutLocalPositionVec4.xyz, 1.0));   
        lOutLocalNormalVec3    = normalize( lOutLocalPositionVec4.xyz );

        // Calculate normal
        float  lfRadius        = length( MAT4_GET_COLUMN( lGlobalWorldMat, 0 ) ) * 0.5;
        vec3   lMidPointVec3   = MAT4_GET_COLUMN( lBillboardWorldMat4, 3 );
        vec3   lSphereApexVec3 = lMidPointVec3 - ( MAT4_GET_COLUMN( lCameraMatrix, 2 ) * lfRadius);
        lOutWorldNormalVec3    = normalize( lOutWorldPositionVec4.xyz - lSphereApexVec3 );
    }			
#else
    {
        // Billboarding for mesh which is already a single quad
        lOutLocalPositionVec4 = CalcBillboardPos( lCameraMatrix, lLocalPositionVec4, lCustomParams01Vec4.x, lCombinedWorldMat4, lShearParamsVec4, lOutWorldPositionVec4 );
#if 0
        // Original and test code to check results            
        // lOutWorldPositionVec4 = CalcWorldPos( lWorldMat4, lOutLocalPositionVec4 ); 
        vec4 lOutWorldPositionVec4_2 = CalcWorldPos( lWorldMat4, lOutLocalPositionVec4 );         
        float lfDiff = abs(lOutWorldPositionVec4_2.x - lOutWorldPositionVec4.x) + abs(lOutWorldPositionVec4_2.y - lOutWorldPositionVec4.y) + abs(lOutWorldPositionVec4_2.z - lOutWorldPositionVec4.z);
        if (lfDiff > 0.001)     
        {
            _SCE_BREAK();
        }
#endif        
        // Calculate normal
        float  lfRadius        = length( MAT4_GET_COLUMN( lCombinedWorldMat4, 0 ) ) * 0.5;
        vec3   lMidPointVec3   = MAT4_GET_COLUMN( lCombinedWorldMat4, 3 );
        vec3   lSphereApexVec3 = lMidPointVec3 - ( MAT4_GET_COLUMN( lCameraMatrix, 2 ) * lfRadius);
        lOutWorldNormalVec3    = normalize( lOutWorldPositionVec4.xyz - lSphereApexVec3 );
        lOutLocalNormalVec3    = normalize( lOutLocalPositionVec4.xyz );
    }
#endif
}

#endif


//-----------------------------------------------------------------------------
///
///     CalcWorldVec
///
///     @brief      CalcWorldVec
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec3 
CalcWorldVec( 
    in mat4 lWorldNormalMat4,
    in vec3 lLocalVec3 )
{
    return MUL( lWorldNormalMat4, vec4( lLocalVec3, 1.0 ) ).xyz;
    //return lLocalVec3;
}

#ifdef D_INSTANCE
//-----------------------------------------------------------------------------
///
///     CalcWorldVecInstanced
///
///     @brief      CalcWorldVecInstanced
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec3 
CalcWorldVecInstanced( 
    in mat4 lWorldNormalMat4,
    in mat4 lWorldInstancedMat4,
    in vec3 lLocalVec3 )
{
    mat4 lWorldInstancedNormalMat4;
    lWorldInstancedNormalMat4 = mat4( normalize(vec3(lWorldInstancedMat4[0][0], lWorldInstancedMat4[0][1], lWorldInstancedMat4[0][2])), 0.0,   
                                      normalize(vec3(lWorldInstancedMat4[1][0], lWorldInstancedMat4[1][1], lWorldInstancedMat4[1][2])), 0.0,   
                                      normalize(vec3(lWorldInstancedMat4[2][0], lWorldInstancedMat4[2][1], lWorldInstancedMat4[2][2])), 0.0,   
                                                     0.0,			            0.0,			           0.0,			                1.0 ); 
    vec3 lWorldNormal = MUL( lWorldNormalMat4, vec4( lLocalVec3, 1.0 ) ).xyz;
    return MUL( lWorldInstancedNormalMat4, vec4( lWorldNormal, 1.0 ) ).xyz;
}
#endif

//-----------------------------------------------------------------------------
///
///     CalcTanToWorldMat
///
///     @brief      CalcTanToWorldMat
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
mat3 
CalcTanToWorldMat( 
    in vec3 lTangentVec3, 
    in vec3 lBitangentVec3, 
    in vec3 lNormalVec3 )
{
    return mat3( lTangentVec3, lBitangentVec3, lNormalVec3 );
}

//-----------------------------------------------------------------------------
///
///     CalcTanVec
///
///     @brief      CalcTanVec
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
#ifdef _F20_PARALLAXMAP

vec3 
CalcTanVec( 
    in vec3 lViewPositionVec3,
    in vec4 lWorldPositionVec4, 
    in vec3 lTangentVec3, 
    in vec3 lBitangentVec3, 
    in vec3 lNormalVec3 )
{
    vec3 lResultVec3;
    vec3 lDirectionVec3;

    lDirectionVec3 = lViewPositionVec3 - lWorldPositionVec4.xyz;

    lResultVec3.x = dot( lDirectionVec3, lTangentVec3 );
    lResultVec3.y = dot( lDirectionVec3, lBitangentVec3 );
    lResultVec3.z = dot( lDirectionVec3, lNormalVec3 );
    
    return lResultVec3;
}

#endif

//-----------------------------------------------------------------------------
///
///     ApplyRotation
///
///     @brief      ApplyRotation
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
#ifdef _F17_VERTEX_ROTATION
vec3 
ApplyRotation( 
    float lfTheta, 
    vec3   lLocalPositionVec3 )
{
    mat3 lRotationMat3;

    float lfCosTheta = cos( lfTheta * 3.0 );
    float lfSinTheta = sin( lfTheta * 3.0 );

    MAT3_SET_COLUMN( lRotationMat3, 0, vec3( lfCosTheta, 0.0, lfSinTheta ) );
    MAT3_SET_COLUMN( lRotationMat3, 1, vec3( 0.0, 1.0, 0.0 ) );
    MAT3_SET_COLUMN( lRotationMat3, 2, vec3( -lfSinTheta, 0.0, lfCosTheta ) );

    return MUL( lRotationMat3, lLocalPositionVec3 );
}
#endif

//-----------------------------------------------------------------------------
///
///     CalcScreenPosFromWorld
///
///     @brief      CalcScreenPosFromWorld
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec4 
CalcCompressedVertexWorldPosition( 
    in mat4 lWorldMatrix,
    in vec4 lLocalPositionVec4 )
{    
    // [peter] NOT USED ANYMORE? IF YES, NEED TO REWRITE MATRIX ACCESSOR TO BE PLATFORM INDEPENDENT

    //ALEXCHECK - multiplication order
    vec4 lWorldPositionVec4;
    mat4 lWorldMat4 = lWorldMatrix;

    // Scale up slightly to hide cracks
    lWorldMat4[0][0] *= kfScaleUpFactor;
    lWorldMat4[1][1] *= kfScaleUpFactor;
    lWorldMat4[2][2] *= kfScaleUpFactor;

    // Strip scale from model position and add this first
    lWorldPositionVec4  = lLocalPositionVec4;
    lWorldPositionVec4 += vec4( lWorldMat4[ 3 ][ 0 ] / lWorldMat4[ 0 ][ 0 ], lWorldMat4[ 3 ][ 1 ] / lWorldMat4[ 1 ][ 1 ], lWorldMat4[ 3 ][ 2 ] / lWorldMat4[ 2 ][ 2 ], 0.0 );

    // Scale up world position
    lWorldMat4[3]       = vec4( 0.0, 0.0, 0.0, 1.0 );
    lWorldPositionVec4  = MUL( lWorldMat4, lWorldPositionVec4 );

    return lWorldPositionVec4;
}


//-----------------------------------------------------------------------------
///
///     CalcDualParaboloidScreenPosition
///
///     @brief      CalcDualParaboloidScreenPosition
///
///     @param      in mat4 lViewMat4
///     @param      in vec4 lWorldPositionVec4
///     @param      in vec2 lClipPlanes
///     @return     vec4
///
//-----------------------------------------------------------------------------
vec4 
CalcDualParaboloidScreenPosition( 
    in mat4 lViewMat4,
    in vec4 lWorldPositionVec4,
    in vec2 lClipPlanes )
{
    vec4 lScreenSpacePositionVec4 = MUL( lViewMat4, lWorldPositionVec4 );	                    // transform vertex into the maps basis
    
    // Our Camera At is backwards
    lScreenSpacePositionVec4.z = -lScreenSpacePositionVec4.z;

    //Next we need to find the vector from the the vertex to the origin of the paraboloid, which is simply:
    float L = length( lScreenSpacePositionVec4.xyz );				                        // determine the distance between (0,0,0) and the vertex
    lScreenSpacePositionVec4.xyz = lScreenSpacePositionVec4.xyz / L;					            // divide the vertex position by the distance 

    //Now we need to find the x and y coordinates of the point where the incident ray intersects the paraboloid surface.
    lScreenSpacePositionVec4.z = lScreenSpacePositionVec4.z + 1.0;				            // add the reflected vector to find the normal vector
    lScreenSpacePositionVec4.x = lScreenSpacePositionVec4.x / lScreenSpacePositionVec4.z;	// divide x coord by the new z-value
    lScreenSpacePositionVec4.y = lScreenSpacePositionVec4.y / lScreenSpacePositionVec4.z;	// divide y coord by the new z-value

    // Finally we set the z value as the distance from the vertex to the origin of the paraboloid, scaled and biased by the near and far planes of the paraboloid 'camera'.					
    lScreenSpacePositionVec4.z = (L - lClipPlanes.x) / (lClipPlanes.y - lClipPlanes.x);  // set a depth value for correct z-buffering
    lScreenSpacePositionVec4.z = 1.0 - lScreenSpacePositionVec4.z;                          // reverse Z
    lScreenSpacePositionVec4.w = 1;									                                 // set w to 1 so there is no w divide


    return lScreenSpacePositionVec4;
}

//-----------------------------------------------------------------------------
///
///     OctahedronNormalEncode
///
///     @brief      OctahedronNormalEncode
///
///     @param      vec3 lNormal
///     @return     vec2
///
//-----------------------------------------------------------------------------
vec2
OctahedronNormalEncode( 
    vec3 lNormal )
{
    vec2 lEncoded;
    lNormal /= ( abs( lNormal.x ) + abs( lNormal.y ) + abs( lNormal.z ) );

    if( lNormal.z > 0.0 )
    {
        lEncoded = vec2( lNormal.x, lNormal.y );
    }
    else
    {
        lEncoded = vec2( 1.0 - abs( lNormal.y ), 1.0 - abs( lNormal.x ) );

        lEncoded.x *= lNormal.x < 0.0 ? -1.0 : 1.0;
        lEncoded.y *= lNormal.y < 0.0 ? -1.0 : 1.0;
    }

    return lEncoded;
}

//-----------------------------------------------------------------------------
///
///     OctahedronNormalDecode
///
///     @brief      OctahedronNormalDecode
///
///     @param      vec2 lEncoded
///     @return     vec3
///
//-----------------------------------------------------------------------------
vec3 
OctahedronNormalDecode( 
    vec2 lEncoded )
{
    vec3 lNormal;
    lNormal.z = 1.0 - abs( lEncoded.x ) - abs( lEncoded.y );

    if( lNormal.z >= 0.0 )
    {
        lNormal.xy = lEncoded.xy;
    }
    else
    {
        lNormal.x = 1.0 - abs( lEncoded.y );
        lNormal.y = 1.0 - abs( lEncoded.x );

        lNormal.x *= lEncoded.x < 0.0 ? -1.0 : 1.0;
        lNormal.y *= lEncoded.y < 0.0 ? -1.0 : 1.0;
    }

    lNormal = normalize( lNormal );

    return lNormal;
}