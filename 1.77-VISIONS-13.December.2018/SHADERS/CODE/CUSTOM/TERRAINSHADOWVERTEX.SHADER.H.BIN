////////////////////////////////////////////////////////////////////////////////
///
///     @file       TerrainShadowVertex.h
///     @author     User
///     @date       
///
///     @brief      Terrain Shadow Vertex Shader
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
//      Compilation defines 


//-----------------------------------------------------------------------------
//      Include files


#ifndef D_VERTEX
#define D_VERTEX
#endif

#include "Common/Defines.shader.h"
#include "Common/CommonUniforms.shader.h"

#include "UberCommon.h"

#include "Common/CommonVertex.shader.h"

//-----------------------------------------------------------------------------
//      Global Data

#define D_PERSPECTIVE_SHADOWMAPx

//-----------------------------------------------------------------------------
//      Typedefs and Classes 
    
//-----------------------------------------------------------------------------
///
///     Input
///
///     @brief  Input
///
//-----------------------------------------------------------------------------
DECLARE_INPUT
    
	INPUT( vec3, mkLocalPositionVec4,  NORMAL0 )

DECLARE_END


//-----------------------------------------------------------------------------
///
///     Output
///
///     @brief  Output
///
//-----------------------------------------------------------------------------
DECLARE_OUTPUT

    OUTPUT_SCREEN_POSITION

DECLARE_END

//-----------------------------------------------------------------------------
//    Functions


//-----------------------------------------------------------------------------
///
///     Vertex Main
///
///     @brief      Vertex Main
///
///     @param      void
///     @return     Nothing.
///
//-----------------------------------------------------------------------------
VERTEX_MAIN_SRT
{
	vec4 lLocalPositionVec4;
	vec4 lWorldPositionVec4;
	vec4 lScreenPositionVec4;

	lLocalPositionVec4 = vec4( IN( mkLocalPositionVec4 ).xyz, 1.0 );


	lWorldPositionVec4  = CalcWorldPos( lUniforms.mpCommonPerMesh->gWorldMat4, , lLocalPositionVec4 );
    lScreenPositionVec4 = CalcScreenPosFromWorld( lUniforms.mpPerFrame->gViewMat4, lUniforms.mpCommonPerMesh->gProjectionMat4, lWorldPositionVec4 );
	  
	#ifndef D_PERSPECTIVE_SHADOWMAP
	
		//for ortho
		//we need to bias the depth from -0.5 to 0.5 range into 0 to 1 range
		//we also clamp objects behind the camera to a very small depth value
		//to avoid missing casters (pancaking)
		
		lScreenPositionVec4.z = max( lScreenPositionVec4.z, -0.9999999 ); // + lUniforms.mpCommonPerMesh->gfShadowBias + 0.001;
				
	#else

		//OUT( dist ) = length( gShadowMapPositions.xyz - lWorldPosition.xyz ) / gShadowMapPositions.w;

	#endif

	SCREEN_POSITION = lScreenPositionVec4;
}
