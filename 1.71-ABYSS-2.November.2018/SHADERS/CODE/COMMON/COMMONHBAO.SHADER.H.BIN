////////////////////////////////////////////////////////////////////////////////
///
///     @file       CommonHBAO.h
///     @author     User
///     @date       
///
///     @brief      CommonHBAO
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 

#ifndef D_COMMONHBAO_H
#define D_COMMONHBAO_H

//-----------------------------------------------------------------------------
//      Include files

//-----------------------------------------------------------------------------
//      Global Data


//-----------------------------------------------------------------------------
//      Functions


//-----------------------------------------------------------------------------
//      Global Data
    

#ifndef M_PI
#define M_PI 3.141592653589793
#endif
#define AO_RANDOMTEX_SIZE 4


// tweakables
STATIC_CONST float  NUM_STEPS      = 4;
STATIC_CONST float  NUM_DIRECTIONS = 8; // texRandom/g_Jitter initialization depends on this

STATIC_CONST vec3 randomTex16[16] = 
{
    vec3( 0.908532917, 0.417813331, 0.592844605  ),
    vec3( 0.846346378, 0.532632887, 0.844265759  ),
    vec3( 0.890019119, 0.455923200, 0.857945621  ),
    vec3( 0.909818292, 0.415006787, 0.847251713  ),
    vec3( 0.945151687, 0.326631814, 0.623563707  ),
    vec3( 0.874067128, 0.485805124, 0.384381711  ),
    vec3( 0.941520989, 0.336954355, 0.297534615  ),
    vec3( 0.764586449, 0.644521177, 0.0567129776 ),
    vec3( 0.726996362, 0.686641276, 0.272656292  ),
    vec3( 0.954994738, 0.296622723, 0.477665126  ),
    vec3( 0.812819958, 0.582515001, 0.812168717  ),
    vec3( 0.914957762, 0.403549612, 0.479977161  ),
    vec3( 0.902118862, 0.431487620, 0.392784804  ),
    vec3( 0.747196972, 0.664602637, 0.836078763  ),
    vec3( 0.998444080, 0.0557626523, 0.337396175 ),
    vec3( 0.997659504, 0.0683777928, 0.648171902 )
};

vec3
RecreateViewPositionFromDepth(
    in float lfDepth,
    in vec2  lFragCoordsVec2,
    in mat4  lInverseProjectionMatrix )
{
    vec4 lPositionVec4 = vec4(lFragCoordsVec2.xy, 0.0, 1.0);

    // Inverse projection
    lPositionVec4        = MUL( lInverseProjectionMatrix, lPositionVec4 );
    //lPositionVec4        = lPositionVec4 / lPositionVec4.w;

    vec3 lViewVectorVec3 = normalize( lPositionVec4.xyz );
    lPositionVec4.xyz    = lViewVectorVec3 * lfDepth;

    return lPositionVec4.xyz;
}

vec3
DecodeGBufferViewPosition(
    in  vec2  lScreenPosVec2,
    in  vec4  lClipPlanes,
    in  mat4  lInverseProjectionMat4,
    SAMPLER2DARG( lBuffer1Map ) )
{
    vec4 lBuffer1_Vec4 = texture2DLod( lBuffer1Map, lScreenPosVec2, 0.0 );
    float lfDepth      = FastDenormaliseDepth( lClipPlanes, DecodeDepthFromColour( lBuffer1_Vec4 ) );
    vec3 lPositionVec3 = RecreateViewPositionFromDepth( lfDepth, lScreenPosVec2, lInverseProjectionMat4 );
    return lPositionVec3;
}

#if 0
vec3 
MinDiff( 
    in vec3 P, 
    in vec3 Pr, 
    in vec3 Pl )
{
    vec3 V1 = Pr - P;
    vec3 V2 = P - Pl;
    return ( dot( V1, V1 ) < dot( V2, V2 ) ) ? V1 : V2;
}


vec3 
ReconstructNormal( 
    in  vec3  P,
    in  vec2  InvFullResolution,
    in  vec2  lScreenPosVec2,
    in  vec4  lClipPlanes,
    in  mat4  lInverseProjectionMat4,
    //in  mat4  lInverseViewMat4,
    //in  vec3  lEyePositionVec3,
    SAMPLER2DARG( lBuffer1Map ) )
{
    #define FetchViewPos( POS_OFFSET ) DecodeGBufferPosition( lScreenPosVec2 + POS_OFFSET, lClipPlanes, lInverseProjectionMat4, /*lInverseViewMat4, lEyePositionVec3,*/ SAMPLER2DPARAM( lBuffer1Map ) )

    vec3 Pr = FetchViewPos( vec2( InvFullResolution.x, 0  ) );
    vec3 Pl = FetchViewPos( vec2( -InvFullResolution.x, 0 ) );
    vec3 Pt = FetchViewPos( vec2( 0, InvFullResolution.y  ) );
    vec3 Pb = FetchViewPos( vec2( 0, -InvFullResolution.y ) );

    return normalize( cross( MinDiff( P, Pr, Pl ), MinDiff( P, Pt, Pb ) ) );
}
#endif


//----------------------------------------------------------------------------------
float 
Falloff( 
    in float DistanceSquare, 
    in float NegInvR2 )
{
    // 1 scalar mad instruction
    return DistanceSquare * NegInvR2 + 1.0;
}

//----------------------------------------------------------------------------------
// P = view-space position at the kernel center
// N = view-space normal at the kernel center
// S = view-space position of the current sample
//----------------------------------------------------------------------------------
float 
ComputeAO( 
    in vec3  P, 
    in vec3  N, 
    in vec3  S, 
    in float NDotVBias, 
    in float NegInvR2 )
{
    vec3 V = S - P;
    float VdotV = dot( V, V );
    float NdotV = dot( N, V ) * invsqrt( VdotV );

    // Use saturate(x) instead of max(x,0.f) because that is faster on Kepler
    return clamp( NdotV - NDotVBias, 0, 1 ) * clamp( Falloff( VdotV, NegInvR2 ), 0, 1 );
}



//----------------------------------------------------------------------------------
vec2 
RotateDirection( 
    in vec2 Dir, 
    in vec2 CosSin )
{
    return vec2( Dir.x*CosSin.x - Dir.y*CosSin.y,
        Dir.x*CosSin.y + Dir.y*CosSin.x );
}


/*
float hash( float n )
{
    return (fract( sin( n )*43758.5453 ) + 1.0) * 0.5;
}
*/

//----------------------------------------------------------------------------------
vec4 
GetJitter(
    vec2 lUV )
{
    // (cos(Alpha),sin(Alpha),rand1,rand2)
    uvec2 positionMod = uvec2( uvec2(lUV) & 3 );
    vec4 Jitter = vec4( randomTex16[(positionMod.x*4) + positionMod.y], 0.0 );

    return Jitter;
    //return vec4(1.0);
}

//----------------------------------------------------------------------------------
float 
ComputeCoarseAO( 
    SAMPLER2DARG( lBuffer1Map ),
    in vec4  lClipPlanes,
    in mat4  lInverseProjectionMat4,
    in vec3  lEyePositionVec3,
    in vec2  FullResUV, 
    in float RadiusPixels, 
    in vec4  Rand, 
    in vec3  ViewPosition, 
    in vec3  ViewNormal, 
    in vec2  InvFullResolution, 
    in float AOMultiplier, 
    in float NDotVBias, 
    in float NegInvR2 )
{

    // Divide by NUM_STEPS+1 so that the farthest samples are not fully attenuated
    float StepSizePixels = RadiusPixels / ( NUM_STEPS + 1 );

    const float Alpha = 2.0 * M_PI / NUM_DIRECTIONS;
    float AO = 0;
    float AO_ValidSamples = 0;
    for ( float DirectionIndex = 0; DirectionIndex < NUM_DIRECTIONS; ++DirectionIndex )
    {
        float Angle = Alpha * DirectionIndex;

        // Compute normalized 2D direction
        vec2 Direction = RotateDirection( vec2( cos( Angle ), sin( Angle ) ), Rand.xy );

        // Jitter starting sample within the first step
        float RayPixels = ( Rand.z * StepSizePixels + 1.0 );

        for ( float StepIndex = 0; StepIndex < NUM_STEPS; ++StepIndex )
        {
            vec2 SnappedUV = round( RayPixels * Direction ) * InvFullResolution + FullResUV;
            //vec3 S = FetchViewPos( SnappedUV );

            //vec4 lBuffer1_Vec4 = texture2D( lBuffer1Map, SnappedUV );

            float lfValidSample = (SnappedUV.x >= 0.0 && SnappedUV.y >= 0.0 && SnappedUV.x < 1.0 && SnappedUV.y < 1.0) ? 1.0 : 0.0;
            //if (lfValidSample != 0.0)
            {
                vec3 S = DecodeGBufferViewPosition( SnappedUV,
                                                lClipPlanes,
                                                lInverseProjectionMat4,
                                                //lEyePositionVec3,
                                                //lBuffer1_Vec4 );
                                                SAMPLER2DPARAM( lBuffer1Map ) );

            //S -= lEyePositionVec3;
           
                AO += ComputeAO(ViewPosition, ViewNormal, S, NDotVBias, NegInvR2) * lfValidSample;
                AO_ValidSamples += lfValidSample;
            }
            RayPixels += StepSizePixels;

        }
    }
    if (AO_ValidSamples != 0.0)
    {
        AO *= AOMultiplier / AO_ValidSamples;
    }
    return clamp( 1.0 - AO * 2.0, 0, 1 );
}


//-----------------------------------------------------------------------------
//      Functions 


vec4 HBAO(
	SAMPLER2DARG( lBuffer1Map ),
	SAMPLER2DARG( lBuffer2Map ),
    in vec2  lFragCoordsVec2,
    in vec4  lClipPlanesVec4,
    in vec4  lFoVValuesVec4,
    in vec4  lFrameBufferSizeVec4,
    in vec3  lViewPositionVec3,
    in mat4  lCameraMat4,
    in mat4  lInverseProjectionSCMat4,
    in vec4  lHBAOParamsVec4
)
{
    float lfDepth = texture2DLod(lBuffer1Map, lFragCoordsVec2, 0.0).x;
    //vec2  lFragCoordsVec2 = IN(mScreenPositionVec4).xy;
    //float lfDepth = lBuffer1Map.Load(int3(lFragCoordsVec2,0.0));
    //lFragCoordsVec2*=lFrameBufferSizeVec4.zw;
    float AO = 1.0;
    if (lfDepth <= 0.9999999)
    {
        vec3  lPositionVec3;
        vec3  lNormalVec3;

        {
            uvec2 luResolution = GetTexResolution(lBuffer2Map);
            vec2 lResolution = vec2(luResolution.x, luResolution.y);
            vec2 lPixelLoc = lFragCoordsVec2 * lResolution - vec2(0.5, 0.5);
            vec2 lSampleLoc = floor(lPixelLoc) / lResolution;
            vec2 lSubPixel = fract(lPixelLoc);
            vec4 lBilinearWeights = vec4(1.0 - lSubPixel.x, lSubPixel.x, lSubPixel.x, 1.0 - lSubPixel.x) *
                                    vec4(lSubPixel.y, lSubPixel.y, 1.0 - lSubPixel.y, 1.0 - lSubPixel.y);
            vec4 lTextureR      = textureGatherRed(lBuffer2Map, lSampleLoc);
            vec4 lTextureG      = textureGatherGreen(lBuffer2Map, lSampleLoc);
            vec4 lTextureB      = textureGatherBlue(lBuffer2Map, lSampleLoc);
            vec4 lBuffer2_Vec4  = vec4(dot(lTextureR, lBilinearWeights),
                                       dot(lTextureG, lBilinearWeights),
                                       dot(lTextureB, lBilinearWeights), 0.0);            
            //lBuffer2_Vec4 = texture2D(lBuffer2Map, lFragCoordsVec2);
            lNormalVec3    = DecodeNormal( lBuffer2_Vec4.xyz );
            lPositionVec3  = DecodeGBufferViewPosition( lFragCoordsVec2,
                                                    lClipPlanesVec4,
                                                   lInverseProjectionSCMat4,
                                                    SAMPLER2DPARAM( lBuffer1Map ) );
        }

        //vec3 ViewPosition = lPositionVec3 - lViewPositionVec3;
        vec3 ViewPosition = lPositionVec3;

        // Reconstruct view-space normal from nearest neighbors


        mat3 lCameraMatInverseRotMat3;
        mat4 lCameraMatInverseMat4;

        lCameraMatInverseMat4 = transpose( lCameraMat4 );
        lCameraMatInverseRotMat3[0] = lCameraMatInverseMat4[0].xyz;
        lCameraMatInverseRotMat3[1] = lCameraMatInverseMat4[1].xyz;
        lCameraMatInverseRotMat3[2] = lCameraMatInverseMat4[2].xyz;

        vec3 ViewNormal = MUL( lCameraMatInverseRotMat3, lNormalVec3 );
    
        /*
        vec3 ViewNormal = ReconstructNormal(
            ViewPosition,
            lFrameBufferSizeVec4.zw,
            lFragCoordsVec2,
            lClipPlanesVec4,
           lInverseProjectionMat4,
            //lUniforms.mpCommonPerMesh->gInverseViewMat4,
            //lViewPositionVec3,
            SAMPLER2DPARAM( lUniforms.mpCustomPerMaterial->gBuffer1Map )  );
            */
        //vec3 ViewNormal = lNormalVec3;

        vec2  InvFullResolution = lFrameBufferSizeVec4.zw;
        float lfBias            = lHBAOParamsVec4.x;
        float lfRadius          = lHBAOParamsVec4.y;
        float lfIntensity       = lHBAOParamsVec4.z;
        float NDotVBias         = lfBias;
        float AOMultiplier      = 1.0 / (1.0 - NDotVBias);
        float NegInvR2          = -1.0 / (lfRadius * lfRadius);
        float PowExponent       = lfIntensity;
        float projScale         = lFrameBufferSizeVec4.y / ( tan( lFoVValuesVec4.x ) * 2.0 ); // projScale = float(height) / (tanf( projection.fov * 0.5f) * 2.0f);
        float RadiusToScreen    = lfRadius * 0.5 * projScale; 

        // Compute projection of disk of radius control.R into screen space
        //float RadiusPixels = RadiusToScreen / length(ViewPosition);
        float RadiusPixels = RadiusToScreen / length(-ViewPosition.z);
        //float RadiusPixels = -ViewPosition.z;
        //float RadiusPixels = length(ViewPosition);
        //float RadiusPixels = RadiusToScreen;

        RadiusPixels = sign(RadiusPixels) * min( abs(RadiusPixels), lFrameBufferSizeVec4.y / 8.0 );

        // Get jitter vector for the current full-res pixel
        vec4 Rand = GetJitter( lFragCoordsVec2 * lFrameBufferSizeVec4.xy );

        AO = ComputeCoarseAO( SAMPLER2DPARAM( lBuffer1Map ),
                                    lClipPlanesVec4,
                                   lInverseProjectionSCMat4,
                                    lViewPositionVec3, 
                                    lFragCoordsVec2, 
                                    RadiusPixels, 
                                    Rand, 
                                    ViewPosition, 
                                    ViewNormal, 
                                    InvFullResolution, 
                                    AOMultiplier,
                                    NDotVBias,
                                    NegInvR2 );

        AO = pow(AO, PowExponent);
    }

    vec4 lFragCol = vec4( AO, 0.0, 0.0, 0.0);
	
    //lFragCol = vec4( pow( AO, PowExponent ), ViewPosition.z, 0.0, 0.0 );
    //lFragCol = vec4( ViewNormal, 1.0 );
    /*
    if ( RadiusPixels <= 0.0 )
    {
        lFragCol = ( vec4( 0.0, 0.0, 0.0, 1.0 ) );
    }
    else
    if ( RadiusPixels < 25.0 )
    {
        lFragCol = ( vec4( 1.0, 1.0, 0.0, 1.0 ) );
    }
    else
    if ( RadiusPixels < 50.0 )
    {
        lFragCol = ( vec4( 1.0, 0.0, 1.0, 1.0 ) );
    }
    else
    if ( RadiusPixels < 75.0 )
    {
        lFragCol = (vec4(1.0,0.0,0.0, 1.0));
    }
    else
    if ( RadiusPixels < 100.0 )
    {
        lFragCol = (vec4(0.0,0.0,1.0, 1.0));
    }
    else
    if ( RadiusPixels < 125.0 )
    {
        lFragCol = (vec4(1.0,0.0,1.0, 1.0));
    }
    else
    if ( RadiusPixels < 150.0 )
    {
        lFragCol = (vec4(1.0,1.0,0.0, 1.0));
    }
    else
    if ( RadiusPixels < 175.0 )
    {
        lFragCol = (vec4(0.0,1.0,1.0, 1.0));
    }
    else
    {
        lFragCol = (vec4(0.0,1.0,0.0, 1.0));
    }
    */
    
    /*
    if ( lFragCoordsVec2.x < 0.5 )
    {
        if ( lFragCoordsVec2.y < 0.5 )
        {
            lFragCol = Rand;
        }
        else
        {
            lFragCol = vec4( lViewPositionVec3, 0 );
        }
    }
    else
    {
        if ( lFragCoordsVec2.y < 0.5 )
        {
            lFragCol = vec4( vec3( RadiusPixels ), 0.0 );
        }
        else
        {
            lFragCol = vec4( vec3( pow( AO, PowExponent ) ), 0.0 );
        }
    }
    */
	return lFragCol;
}

#endif