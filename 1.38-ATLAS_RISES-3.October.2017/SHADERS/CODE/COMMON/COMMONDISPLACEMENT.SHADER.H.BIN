////////////////////////////////////////////////////////////////////////////////
///
///     @file       CommonDisplacement.h
///     @author     User
///     @date       
///
///     @brief      CommonDisplacement
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///     http://http.developer.nvidia.com/GPUGems/gpugems_ch42.html
///     http://www.ozone3d.net/tutorials/mesh_deformer_p2.php
///
////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      Compilation defines 

#ifndef D_COMMONDISPLACEMENT_H
#define D_COMMONDISPLACEMENT_H


//-----------------------------------------------------------------------------
//      Include files


//-----------------------------------------------------------------------------
//      Global Data
/*
struct CommonDisplacementUniforms
{
    vec4 gWaveOneAmplitudeVec4;
    vec4 gWaveOneFrequencyVec4;
    vec4 gWaveOneFallOffAndSpeedVec4;

    vec4 gWaveTwoAmplitudeVec4;
    vec4 gWaveTwoFrequencyVec4;
    vec4 gWaveTwoFallOffAndSpeedVec4;
};
UNIFORM( CommonDisplacementUniforms, gCommonDisplacementUniforms );
*/

//-----------------------------------------------------------------------------
//      Typedefs and Classes 


//-----------------------------------------------------------------------------
//      Functions

//-----------------------------------------------------------------------------
///
///     ComputeJacobianFallOff
///
///     @brief      ComputeJacobianFallOff
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
mat3 
ComputeJacobianFallOff(  
	in vec3   lVertexPositionVec3,
	in vec3   lInOutAxisVec3,
	in vec3   lAlongAxisVec3,
	in vec3   lWaveVec3,
	in vec3   lFallOffVec3, 
    in float  lfTime,
    in float  lfFallOffFactor )
{
	mat3 lJacobianMat3;

	float lfCoefficent;
	
	lfCoefficent  = cos( dot( lAlongAxisVec3, lVertexPositionVec3 ) + lfTime );
	lfCoefficent *= lfFallOffFactor;
			
	// A matrix is an array of column vectors so J[2] is 
	// the third column of J.
	lJacobianMat3[0][0] = ( lAlongAxisVec3.x * lInOutAxisVec3.x * lfCoefficent ) + (lFallOffVec3.x * lWaveVec3.x);
	lJacobianMat3[0][1] = ( lAlongAxisVec3.x * lInOutAxisVec3.y * lfCoefficent ) + (lFallOffVec3.x * lWaveVec3.y);
	lJacobianMat3[0][2] = ( lAlongAxisVec3.x * lInOutAxisVec3.z * lfCoefficent ) + (lFallOffVec3.x * lWaveVec3.x);
	
	lJacobianMat3[1][0] = ( lAlongAxisVec3.y * lInOutAxisVec3.x * lfCoefficent ) + (lFallOffVec3.y * lWaveVec3.x);
	lJacobianMat3[1][1] = ( lAlongAxisVec3.y * lInOutAxisVec3.y * lfCoefficent ) + (lFallOffVec3.y * lWaveVec3.y);
	lJacobianMat3[1][2] = ( lAlongAxisVec3.y * lInOutAxisVec3.z * lfCoefficent ) + (lFallOffVec3.y * lWaveVec3.z);

	lJacobianMat3[2][0] = ( lAlongAxisVec3.z * lInOutAxisVec3.x * lfCoefficent ) + (lFallOffVec3.z * lWaveVec3.x);
	lJacobianMat3[2][1] = ( lAlongAxisVec3.z * lInOutAxisVec3.y * lfCoefficent ) + (lFallOffVec3.z * lWaveVec3.y);
	lJacobianMat3[2][2] = ( lAlongAxisVec3.z * lInOutAxisVec3.z * lfCoefficent ) + (lFallOffVec3.z * lWaveVec3.z);

	return lJacobianMat3;
}

//-----------------------------------------------------------------------------
///
///     ComputeJacobian
///
///     @brief      ComputeJacobian
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
mat3 
ComputeJacobian(  
	in vec3   lVertexPositionVec3,
	in vec3   lInOutAxisVec3,
	in vec3   lAlongAxisVec3, 
    in float  lfTime )
{
	mat3 lJacobianMat3;

	float lfCoefficent = cos( dot( lAlongAxisVec3, lVertexPositionVec3 ) + lfTime );
			
	// A matrix is an array of column vectors so J[2] is 
	// the third column of J.
	lJacobianMat3[0][0] = ( lAlongAxisVec3.x * lInOutAxisVec3.x * lfCoefficent );
	lJacobianMat3[0][1] = ( lAlongAxisVec3.x * lInOutAxisVec3.y * lfCoefficent );
	lJacobianMat3[0][2] = ( lAlongAxisVec3.x * lInOutAxisVec3.z * lfCoefficent );
	
	lJacobianMat3[1][0] = ( lAlongAxisVec3.y * lInOutAxisVec3.x * lfCoefficent );
	lJacobianMat3[1][1] = ( lAlongAxisVec3.y * lInOutAxisVec3.y * lfCoefficent );
	lJacobianMat3[1][2] = ( lAlongAxisVec3.y * lInOutAxisVec3.z * lfCoefficent );

	lJacobianMat3[2][0] = ( lAlongAxisVec3.z * lInOutAxisVec3.x * lfCoefficent );
	lJacobianMat3[2][1] = ( lAlongAxisVec3.z * lInOutAxisVec3.y * lfCoefficent );
	lJacobianMat3[2][2] = ( lAlongAxisVec3.z * lInOutAxisVec3.z * lfCoefficent );

	return lJacobianMat3;
}

//-----------------------------------------------------------------------------
///
///     ComputeNormal
///
///     @brief      ComputeNormal
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
vec3 
ComputeNormal(  
	mat3        lJacobianMat3, 
    in vec3     lTangentVec3, 
    in vec3     lBinormalVec3 )
{
	lJacobianMat3[0][0] += 1.0;
	lJacobianMat3[1][1] += 1.0;
	lJacobianMat3[2][2] += 1.0;
	
	vec3 lTranformedTangentVec3 = MUL( lJacobianMat3, lTangentVec3 );
	vec3 lTransformedBinormalVec3 = MUL( lJacobianMat3, lBinormalVec3 );
	
	vec3 lResultVec3 = cross(lTransformedBinormalVec3, lTranformedTangentVec3);

	return lResultVec3;
}

//-----------------------------------------------------------------------------
///
///     DisplaceVertexFunc
///
///     @brief      DisplaceVertexFunc
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------	
vec3 
DisplaceVertexFunc( 
	in vec3   lVertexPositionVec3,
	in vec3   lInOutAxisVec3,
	in vec3   lAlongAxisVec3,
	in float  lfTime ) 
{
	float lfAlong;
	float lfWave;
	
	lfAlong = dot( lAlongAxisVec3, lVertexPositionVec3 );
	
	lfWave  = sin( ( lfAlong + lfTime ) );
	
	return ( vec3( lfWave, lfWave, lfWave ) * lInOutAxisVec3 );
}

//-----------------------------------------------------------------------------
///
///     DisplaceVertex
///
///     @brief      DisplaceVertex
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------	
void 
DisplaceVertex( 
	inout vec4   lPositionVec4,
	out   vec3   laNewVertexVec3[2],
	out   vec3   laFallOffRecipVec3[2],
	out   float  lafFallOffScale[2],
	in    float  lfTime,
    in    CustomPerMaterialUniforms lMaterialUniforms ) 
{
	laFallOffRecipVec3[ 0 ] = vec3( 0.0, 0.0, 0.0 );
	laFallOffRecipVec3[ 1 ] = vec3( 0.0, 0.0, 0.0 );
	
	laNewVertexVec3[0] = DisplaceVertexFunc( lPositionVec4.xyz,
										lMaterialUniforms.gWaveOneAmplitudeVec4.xyz,
										lMaterialUniforms.gWaveOneFrequencyVec4.xyz,
										lfTime * lMaterialUniforms.gWaveOneFallOffAndSpeedVec4.w );
										
	lafFallOffScale[0] = 1.0;
										 
	if ( length( lMaterialUniforms.gWaveOneFallOffAndSpeedVec4.xyz ) > 0.0 )
	{
		laFallOffRecipVec3[0]  = vec3( lMaterialUniforms.gWaveOneFallOffAndSpeedVec4.x > 0.0 ? 1.0 / lMaterialUniforms.gWaveOneFallOffAndSpeedVec4.x : 0.0,
								  lMaterialUniforms.gWaveOneFallOffAndSpeedVec4.y > 0.0 ? 1.0 / lMaterialUniforms.gWaveOneFallOffAndSpeedVec4.y : 0.0,
								  lMaterialUniforms.gWaveOneFallOffAndSpeedVec4.z > 0.0 ? 1.0 / lMaterialUniforms.gWaveOneFallOffAndSpeedVec4.z : 0.0 );
								  
		lafFallOffScale[0] = dot( laFallOffRecipVec3[0], lPositionVec4.xyz );
	}
										 
	laNewVertexVec3[1] = DisplaceVertexFunc(	lPositionVec4.xyz,
										lMaterialUniforms.gWaveTwoAmplitudeVec4.xyz,
										lMaterialUniforms.gWaveTwoFrequencyVec4.xyz,
										lfTime * lMaterialUniforms.gWaveTwoFallOffAndSpeedVec4.w );
										
	lafFallOffScale[1] = 1.0;
	
	if ( length( lMaterialUniforms.gWaveTwoFallOffAndSpeedVec4.xyz ) > 0.0 )
	{
		laFallOffRecipVec3[1]  = vec3( lMaterialUniforms.gWaveTwoFallOffAndSpeedVec4.x > 0.0 ? 1.0 / lMaterialUniforms.gWaveTwoFallOffAndSpeedVec4.x : 0.0,
								  lMaterialUniforms.gWaveTwoFallOffAndSpeedVec4.y > 0.0 ? 1.0 / lMaterialUniforms.gWaveTwoFallOffAndSpeedVec4.y : 0.0,
								  lMaterialUniforms.gWaveTwoFallOffAndSpeedVec4.z > 0.0 ? 1.0 / lMaterialUniforms.gWaveTwoFallOffAndSpeedVec4.z : 0.0 );

		lafFallOffScale[1] = dot( laFallOffRecipVec3[1], lPositionVec4.xyz );
	}
											 
	lPositionVec4.xyz += (laNewVertexVec3[0] * lafFallOffScale[0]) + 
				     (laNewVertexVec3[1] * lafFallOffScale[1]);
}

//-----------------------------------------------------------------------------
///
///     DisplaceVertex
///
///     @brief      DisplaceVertex
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------	
void 
DisplaceVertex( 
	inout vec4  lPositionVec4,
	in    float lfTime,
    in    CustomPerMaterialUniforms lMaterialUniforms ) 
{
	vec3   laNewVertexVec3[2];
	vec3   laFallOffRecipVec3[2];
	float lafFallOffScale[2];
	
	DisplaceVertex( lPositionVec4,
					laNewVertexVec3,
					laFallOffRecipVec3,
					lafFallOffScale,
					lfTime,
                    lMaterialUniforms );
}

//-----------------------------------------------------------------------------
///
///     DisplaceVertexAndNormal
///
///     @brief      DisplaceVertexAndNormal
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------	
void 
DisplaceVertexAndNormal( 
	inout vec4   lPositionVec4,
	inout vec3   lNormalVec3,
	in    vec3   lTangentVec3, 
	in    float  lfTime,
    in    CustomPerMaterialUniforms lMaterialUniforms ) 
{
	mat3   laJacobianMat3[2];
	vec3   laNewVertexVec3[2];
	vec3   laFallOffRecipVec3[2];
	float lafFallOffScale[2];
	
	DisplaceVertex( lPositionVec4,
					laNewVertexVec3,
					laFallOffRecipVec3,
					lafFallOffScale,
					lfTime,
                    lMaterialUniforms);

	{
		if ( lafFallOffScale[0] != 1.0 )
		{
			laJacobianMat3[0] = ComputeJacobianFallOff(	lPositionVec4.xyz,
															lMaterialUniforms.gWaveOneAmplitudeVec4.xyz,
															lMaterialUniforms.gWaveOneFrequencyVec4.xyz,
															laNewVertexVec3[0],
															laFallOffRecipVec3[0],
															lfTime * lMaterialUniforms.gWaveOneFallOffAndSpeedVec4.w,
															lafFallOffScale[0] );
		}
		else
		{
			laJacobianMat3[0] = ComputeJacobian(	lPositionVec4.xyz,
													lMaterialUniforms.gWaveOneAmplitudeVec4.xyz,
													lMaterialUniforms.gWaveOneFrequencyVec4.xyz, 
													lfTime * lMaterialUniforms.gWaveOneFallOffAndSpeedVec4.w );
		}
		
		if ( lafFallOffScale[1] != 1.0 )
		{
			laJacobianMat3[1] = ComputeJacobianFallOff(	lPositionVec4.xyz,
															lMaterialUniforms.gWaveTwoAmplitudeVec4.xyz,
															lMaterialUniforms.gWaveTwoFrequencyVec4.xyz,
															laNewVertexVec3[1],
															laFallOffRecipVec3[1],
															lfTime * lMaterialUniforms.gWaveTwoFallOffAndSpeedVec4.w,
															lafFallOffScale[1] );
		}
		else
		{
			laJacobianMat3[1] = ComputeJacobian(	lPositionVec4.xyz,
													lMaterialUniforms.gWaveTwoAmplitudeVec4.xyz,
													lMaterialUniforms.gWaveTwoFrequencyVec4.xyz, 
													lfTime * lMaterialUniforms.gWaveTwoFallOffAndSpeedVec4.w );
		}
		
		{
			vec3 lBinormalVec3;
			vec3 lTangentNormalisedVec3; 

			lTangentNormalisedVec3 = normalize(lTangentVec3);
			
			lBinormalVec3 = cross(lNormalVec3, lTangentNormalisedVec3); 
			lBinormalVec3 = normalize( lBinormalVec3 );
			
			lNormalVec3 = -ComputeNormal(	laJacobianMat3[0] + laJacobianMat3[1],
										lTangentNormalisedVec3, 
										lBinormalVec3 );

            lNormalVec3 = normalize( lNormalVec3 );
        }
	}
}

#endif