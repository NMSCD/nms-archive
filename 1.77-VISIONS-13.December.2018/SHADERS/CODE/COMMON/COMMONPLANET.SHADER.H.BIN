////////////////////////////////////////////////////////////////////////////////
///
///     @file       CommonPlanet.h
///     @author     User
///     @date       
///
///     @brief      CommonPlanetShader
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 

#ifndef D_COMMONPLANET_H
#define D_COMMONPLANET_H

//-----------------------------------------------------------------------------
//      Include files


//-----------------------------------------------------------------------------
//      Global Data


STATIC_CONST vec3  kUp        = vec3( 0.0, 1.0, 0.0 );
STATIC_CONST float kfPi       = 3.14159;
STATIC_CONST float kfAccuracy = 0.001;

//-----------------------------------------------------------------------------
//      Functions


//-----------------------------------------------------------------------------
///
///     GetHeight
///
///     @brief      GetHeight
///
///     @param      in vec3 lWorldPosition
///     @return     float
///
//-----------------------------------------------------------------------------
float 
GetHeight(
    in vec3 lWorldPosition,
    in vec4 lPlanetPosition )
{
    vec3 lOffset = lWorldPosition - lPlanetPosition.xyz;
    return length(lOffset) - lPlanetPosition.w;
}

//-----------------------------------------------------------------------------
///
///     GetHeight
///
///     @brief      GetHeight
///
///     @param      in vec3 lWorldPosition
///     @return     float
///
//-----------------------------------------------------------------------------
float 
GetDistanceFromCenter(
    in vec3 lWorldPosition,
    in vec4 lPlanetPosition )
{
    vec3 lOffset = lWorldPosition - lPlanetPosition.xyz;
    return length(lOffset);
}

//-----------------------------------------------------------------------------
///
///     GetWorldUp
///
///     @brief      GetWorldUp
///
///     @param      in vec3 lWorldPosition
///     @return     vec3
///
//-----------------------------------------------------------------------------
vec3 
GetWorldUp(
    in vec3 lWorldPosition,
    in vec4 lPlanetPosition )
{
#ifdef D_ASTEROID
    return kUp;
#else
    return normalize(lWorldPosition / lPlanetPosition.w - lPlanetPosition.xyz / lPlanetPosition.w);
#endif
}

//-----------------------------------------------------------------------------
///
///     GetWorldUp
///
///     @brief      GetWorldUp
///
///     @param      in  vec3  lWorldPosition
///     @param      out float lfHeight
///     @return     vec3
///
//-----------------------------------------------------------------------------
vec3 
GetWorldUp(
    in  vec3  lWorldPosition,
    in  vec4  lPlanetPosition,
    out float lfHeight )
{   
    vec3 lOffset;

    // Get the offset from planet centre
    lOffset = (lWorldPosition - lPlanetPosition.xyz);

    // Get the length of the offset
    lfHeight = length(lOffset);

    // Normalise the offset to get the world up
    lOffset  /= lfHeight;

    // Remove the planet radius to get the height
    lfHeight -= lPlanetPosition.w;

    return lOffset;
}

//-----------------------------------------------------------------------------
///
///     GetWorldUpTransform
///
///     @brief      GetWorldUpTransform
///
///     @param      in vec3 lViewPosition
///     @return     mat3
///
//-----------------------------------------------------------------------------
mat3
GetRotation(
    in vec3 lFromVec3,
    in vec3 lToVec3 )
{
    mat3   lMatrix;
    vec3   lAxisVec3 = cross( lFromVec3, lToVec3 );
    float lfDot      = dot( lFromVec3, lToVec3 );
    float lfAngle    = length( lAxisVec3 );

    if( lfAngle > kfAccuracy && lfDot < ( 1.0 - kfAccuracy ) && lfDot > -( 1.0 - kfAccuracy ) )
    {
        lfAngle   = length( lAxisVec3 );
        lAxisVec3 = normalize( lAxisVec3 );
    }
    else
    {
        return mat3( vec3( 1.0, 0.0, 0.0 ), vec3( 0.0, 1.0, 0.0 ), vec3( 0.0, 0.0, 1.0 ) );
    }

    lfAngle = clamp( lfAngle, -1.0, 1.0 );
    lfAngle = asin( lfAngle );

    if( lfDot < 0.0 )
    {
        lfAngle = kfPi - lfAngle;
    }

    float lfCos = cos( lfAngle );
    float lfInv = 1.0 - lfCos;
    float lfSin = sin( lfAngle );

    MAT3_SET_COLUMN( lMatrix, 0, vec3( lfCos + lAxisVec3.x * lAxisVec3.x * lfInv, lAxisVec3.y * lAxisVec3.z * lfInv + lAxisVec3.z * lfSin, lAxisVec3.z * lAxisVec3.x * lfInv - lAxisVec3.y * lfSin ) );
    MAT3_SET_COLUMN( lMatrix, 1, vec3( lAxisVec3.x * lAxisVec3.y * lfInv - lAxisVec3.z * lfSin, lfCos + lAxisVec3.y * lAxisVec3.y * lfInv, lAxisVec3.z * lAxisVec3.y * lfInv + lAxisVec3.x * lfSin ) );
    MAT3_SET_COLUMN( lMatrix, 2, vec3( lAxisVec3.x * lAxisVec3.z * lfInv + lAxisVec3.y * lfSin, lAxisVec3.y * lAxisVec3.z * lfInv - lAxisVec3.x * lfSin, lfCos + lAxisVec3.z * lAxisVec3.z * lfInv ) );

    return lMatrix;
}



//-----------------------------------------------------------------------------
///
///     GetWorldUpTransform
///
///     @brief      GetWorldUpTransform
///
///     @param      in vec3 lViewPosition
///     @return     mat3
///
//-----------------------------------------------------------------------------
mat3 
GetWorldUpTransform(
    in vec3 lViewPosition,
    in vec4 lPlanetPosition )
{
    vec3 lUp      = vec3( 0.0, 1.0, 0.0);
    vec3 lWorldUp = GetWorldUp( lViewPosition, lPlanetPosition );

    return GetRotation( lUp, lWorldUp );
}

//-----------------------------------------------------------------------------
///
///     GetWorldUpTransform
///
///     @brief      GetWorldUpTransform
///
///     @param      in vec3 lViewPosition
///     @return     mat3
///
//-----------------------------------------------------------------------------
mat3
GetInverseWorldUpTransform(
    in vec3 lViewPosition,
    in vec4 lPlanetPosition )
{
    vec3 lUp      = vec3( 0.0, 1.0, 0.0 );
    vec3 lWorldUp = GetWorldUp( lViewPosition, lPlanetPosition );

    return GetRotation( lWorldUp, lUp );
}


#endif