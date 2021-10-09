////////////////////////////////////////////////////////////////////////////////
///
///     @file       UberVertexShader.h
///     @author     User
///     @date       
///
///     @brief      UberVertexShader
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
//      Compilation defines 


//-----------------------------------------------------------------------------
//      Include files

float
CalculateImposterTexCoordsU( 
    vec3 lWorldPositionVec3, 
    vec3 lViewPositionVec3, 
    vec3 lImposterAt, 
    vec3 lWindingAxis, 
    vec2 lTexCoordsVec2,
    vec4 lImposterDataVec4 )
{
    
    vec3 lCameraAtVec3  = normalize( lViewPositionVec3 - lWorldPositionVec3 );
    //vec3 lCameraAtVec3  = normalize( lCameraAt );
    lImposterAt         = normalize( lImposterAt );
    
    /*
    vec3 lCameraAtVec3   = gImposterDebug.xyz;
    lCameraAtVec3 = normalize( lCameraAtVec3 );
    lImposterAt = vec3( 0.0, 0.0, -1.0 );
    */
    float lfAngle;
    vec3   lAxis;

    // Dot them together to get cosine of angle between them
    lfAngle = dot( -lCameraAtVec3, lImposterAt );

    // Ensure it's in the appropriate range (it might be just outside this range due to rounding errors)
    lfAngle = clamp( lfAngle, -1.0, 1.0 );
    lfAngle = acos(lfAngle) * 57.29577951;

    // Cross products wind counter-clockwise, so everything will be negated.
    lAxis = normalize( cross(lImposterAt, lCameraAtVec3) );
    if ( dot( lAxis, lWindingAxis ) >= 0.0 )
    {
        lfAngle = 360.0 - lfAngle;
    }

    float lfFakeDot = lfAngle * 0.002778; // 1.0 / 360.0 = 0.00277777777..
    
    // n discrete views - must match the settings in GcFadeNode.cpp
    float lfU = ( 1.0 - lTexCoordsVec2.x ) * lImposterDataVec4.y; //lImposterDataVec4.x = kiNumImposterViews. lIposterData.y = 1.0f / kiNumImposterViews.
    lfU = ( lfU + ( floor( lfFakeDot * lImposterDataVec4.x ) * lImposterDataVec4.y ) );
    //lfU = lfU + floor( lfFakeDot );

    // all imposters (debug)
    //float lfU = lTexCoordsVec2.x;

    return lfU;

}
