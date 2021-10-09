
#include "Common/CommonUniforms.shader.h"
#include "Common/Common.shader.h"

#include "Fullscreen/PostCommon.h"
#include "Common/CommonPostProcess.shader.h"
#include "Common/CommonDepth.shader.h"
#include "Common/CommonGBuffer.shader.h"


//sampler2D _MainTex;
//sampler2D _CameraDepthTexture;
//sampler2D _BackFaceDepthTex;
			
//sampler2D _CameraGBufferTexture1;	// Specular color (RGB), roughness (A)
//sampler2D _CameraGBufferTexture2;	// World space normal (RGB), unused (A)
			
mat4x4 _CameraProjectionMatrix;			// projection matrix that maps to screen pixels (not NDC)
//vec4x4 _CameraInverseProjectionMatrix;	// inverse projection matrix (NDC to camera space)
float _Iterations;							// maximum ray iterations
float _BinarySearchIterations;				// maximum binary search refinement iterations
float _PixelZSize;							// Z size in camera space of a pixel in the depth buffer
float _PixelStride;							// number of pixels per ray step close to camera
float _PixelStrideZCuttoff;					// ray origin Z at this distance will have a pixel stride of 1.0
float _MaxRayDistance;						// maximum distance of a ray
float _ScreenEdgeFadeStart;					// distance to screen edge that ray hits will start to fade (0.0 -> 1.0)
float _EyeFadeStart;						// ray direction's Z that ray hits will start to fade (0.0 -> 1.0)
float _EyeFadeEnd;							// ray direction's Z that ray hits will be cut (0.0 -> 1.0)

vec4 _ProjectionParams;

//vec4x4 _NormalMatrix; // worldToCameraMatrix
vec2 _RenderBufferSize;
vec2 _OneDividedByRenderBufferSize;		// Optimization: removes 2 divisions every itteration

/*
struct v2f {
	vec4 position : SV_POSITION;
	vec2 uv : TEXCOORD0;
	vec3 cameraRay : TEXCOORD2;
};

v2f vert(appdata_img v)
{
	v2f o;
	o.position = mul( UNITY_MATRIX_MVP, v.vertex);
	o.uv = v.texcoord;
				
	vec4 cameraRay = vec4( v.texcoord * 2.0 - 1.0, 1.0, 1.0);
	cameraRay = mul( _CameraInverseProjectionMatrix, cameraRay);
	o.cameraRay = cameraRay / cameraRay.w;
				
	return o;
}*/
	
/*
vec3 
ScreenSpaceToViewSpace( vec3 cameraRay, float depth)
{
	return (cameraRay * depth);
}
*/
			
void 
swapIfBigger( 
    inout float aa, 
    inout float bb )
{
	if( aa > bb)
	{
		float tmp = aa;
		aa = bb;
		bb = tmp;
	}
}
			
bool 
rayIntersectsDepthBF( 
    float zA, 
    float zB, 
    vec2  lScreenPosVec2,
    vec4  lClipPlanes,
    SAMPLER2DARG( lBuffer1Map ) )
{
	//vec4 uv4 = vec4( lScreenPosVec2, 0.0, 0.0);
	//float cameraZ = Linear01Depth( tex2Dlod( _CameraDepthTexture, uv4).r) * -_ProjectionParams.z;	
	//float backZ = tex2Dlod( _BackFaceDepthTex, uv4).r * -_ProjectionParams.z;
    //return zB <= cameraZ && zA >= backZ - _PixelZSize;

    vec4 lBuffer1_Vec4 = texture2D( lBuffer1Map, lScreenPosVec2 );

    float lfDepth = FastDenormaliseDepth( lClipPlanes, DecodeDepthFromColour( lBuffer1_Vec4 ) );

    // Is it the sky
    float lfModifiedDepth = lfDepth;
    /*if ( lfDepth >= lClipPlanes.y - 100.0 )
    {
        lfModifiedDepth = 50000.0;
    }*/
			    
    return zB <= lfModifiedDepth /*&& zA >= lfModifiedDepth - _PixelZSize;*/;
}
			
float distanceSquared( vec2 a, vec2 b) { a -= b; return dot(a, a); }


// Trace a ray in screenspace from rayOrigin (in camera space) pointing in rayDirection (in camera space)
// using jitter to offset the ray based on (jitter * _PixelStride).
//
// Returns true if the ray hits a pixel in the depth buffer
// and outputs the hitPixel (in UV space), the hitPoint (in camera space) and the number
// of iterations it took to get there.
//
// Based on Morgan McGuire & Mike Mara's GLSL implementation:
// http://casual-effects.blogspot.com/2014/08/screen-space-ray-tracing.html

bool 
TraceScreenSpaceRay( 
    vec3          rayOrigin, 
	vec3          rayDirection, 
	float         jitter, 
	out vec2      hitPixel, 
	out vec3      hitPoint, 
	out float     iterationCount,
    vec4          lClipPlanes,
    SAMPLER2DARG( lBuffer1Map ) )
{
	// Clip to the near plane    
	//float rayLength = ((rayOrigin.z + rayDirection.z * _MaxRayDistance) > -_ProjectionParams.y) ?
	//		        	(-_ProjectionParams.y - rayOrigin.z) / rayDirection.z : _MaxRayDistance;
    float rayLength;
    
    if ( ( rayOrigin.z + rayDirection.z * _MaxRayDistance ) > _ProjectionParams.y )
    {
        rayLength = (_ProjectionParams.y - rayOrigin.z) / rayDirection.z;
    }
    else
    {
        rayLength = _MaxRayDistance;
    }

	vec3 rayEnd = rayOrigin + rayDirection * rayLength;
			 
	// Project into homogeneous clip space
	vec4 H0 = MUL( _CameraProjectionMatrix, vec4( rayOrigin, 1.0));
	vec4 H1 = MUL( _CameraProjectionMatrix, vec4( rayEnd,    1.0));

    /*
    H0 = ( H0 * 0.5) - 0.5;
    H1 = ( H1 * 0.5) - 0.5;
    */
			    
	float k0 = 1.0 / H0.w;
    float k1 = 1.0 / H1.w;
			 
	// The interpolated homogeneous version of the camera-space points  
	vec3 Q0 = rayOrigin * k0;
    vec3 Q1 = rayEnd    * k1;
			 	
	// Screen-space endpoints
	vec2 P0 = H0.xy * k0;
    vec2 P1 = H1.xy * k1;

    P0 = ( P0 * 0.5 + 0.5 ) * _RenderBufferSize;
    P0 = ( P1 * 0.5 + 0.5 ) * _RenderBufferSize;
			    
	// If the line is degenerate, make it cover at least one pixel
	// to avoid handling zero-pixel extent as a special case later
	P1 += (distanceSquared(P0, P1) < 0.0001) ? 0.01 : 0.0;
			    
	vec2 delta = P1 - P0;
			 
	// Permute so that the primary iteration is in x to collapse
	// all quadrant-specific DDA cases later
	bool permute = false;
	if (abs(delta.x) < abs(delta.y)) 
    { 
		// This is a more-vertical line
		permute = true; 
        delta = delta.yx; 
        P0 = P0.yx; 
        P1 = P1.yx; 
	}
			 
	float stepDir = sign(delta.x);
	float invdx = stepDir / delta.x;
			 
	// Track the derivatives of Q and k
	vec3  dQ = (Q1 - Q0) * invdx;
	float dk = (k1 - k0) * invdx;
	vec2  dP = vec2(stepDir, delta.y * invdx);
			 
	// Calculate pixel stride based on distance of ray origin from camera.
	// Since perspective means distant objects will be smaller in screen space
	// we can use this to have higher quality reflections for far away objects
	// while still using a large pixel stride for near objects (and increase performance)
	// this also helps mitigate artifacts on distant reflections when we use a large
	// pixel stride.
	float strideScaler = 1.0 - min( 1.0, -rayOrigin.z / _PixelStrideZCuttoff);
	float pixelStride  = 1.0 + strideScaler * _PixelStride;
			    
	// Scale derivatives by the desired pixel stride and then
	// offset the starting values by the jitter fraction
	dP *= pixelStride; 
    dQ *= pixelStride; 
    dk *= pixelStride;

	P0 += dP * jitter; 
    Q0 += dQ * jitter; 
    k0 += dk * jitter;
			 
	float i, zA = 0.0, zB = 0.0;
			    
	// Track ray step and derivatives in a vec4 to parallelize
	vec4 pqk  = vec4( P0, Q0.z, k0);
	vec4 dPQK = vec4( dP, dQ.z, dk);

	bool intersect = false;
			    
	for( i=0; i<_Iterations && intersect == false; i++)
	{
		pqk += dPQK;
			    	
		zA = zB;
		zB = (dPQK.z * 0.5 + pqk.z) / (dPQK.w * 0.5 + pqk.w);
		swapIfBigger( zB, zA);
			    	
		hitPixel = permute ? pqk.yx : pqk.xy;
		hitPixel *= _OneDividedByRenderBufferSize;
			        
		intersect = rayIntersectsDepthBF( zA, zB, hitPixel, lClipPlanes, SAMPLER2DPARAM( lBuffer1Map ) );
	}
			    
	// Binary search refinement
	if( pixelStride > 1.0 && intersect)
	{
		pqk -= dPQK;
		dPQK /= pixelStride;
			    	
		float originalStride = pixelStride * 0.5;
	    float stride = originalStride;
	        		
	    zA = pqk.z / pqk.w;
	    zB = zA;
	        		
	    for( float j=0; j<_BinarySearchIterations; j++)
		{
			pqk += dPQK * stride;
				    	
			zA = zB;
	    	zB = (dPQK.z * -0.5 + pqk.z) / (dPQK.w * -0.5 + pqk.w);
	    	swapIfBigger( zB, zA);
				    	
			hitPixel = permute ? pqk.yx : pqk.xy;
			hitPixel *= _OneDividedByRenderBufferSize;
				        
			originalStride *= 0.5;
			stride = rayIntersectsDepthBF( zA, zB, hitPixel, lClipPlanes, SAMPLER2DPARAM( lBuffer1Map ) ) ? -originalStride : originalStride;
		}
	}

			    
	Q0.xy += dQ.xy * i;
	Q0.z = pqk.z;
	hitPoint = Q0 / pqk.w;
	iterationCount = i;
			        	
	return intersect;
}
			
float 
CalculateAlphaForIntersection( 
    bool  intersect, 
    float iterationCount, 
    float specularStrength,
    vec2  hitPixel,
    vec3  hitPoint,
    vec3  vsRayOrigin,
    vec3  vsRayDirection)
{
	float alpha = min( 1.0, specularStrength * 1.0);
				
	// Fade ray hits that approach the maximum iterations
	alpha *= 1.0 - (iterationCount / _Iterations);
  		
	// Fade ray hits that approach the screen edge
	float screenFade = _ScreenEdgeFadeStart;
	vec2 hitPixelNDC = (hitPixel * 2.0 - 1.0);
	float maxDimension = min( 1.0, max( abs( hitPixelNDC.x), abs( hitPixelNDC.y)));
	alpha *= 1.0 - (max( 0.0, maxDimension - screenFade) / (1.0 - screenFade));
				
	// Fade ray hits base on how much they face the camera
    float eyeFadeStart = _EyeFadeStart;
    float eyeFadeEnd = _EyeFadeEnd;
	swapIfBigger( eyeFadeStart, eyeFadeEnd);
				
	float eyeDirection = clamp( vsRayDirection.z, eyeFadeStart, eyeFadeEnd);
	alpha *= 1.0 - ((eyeDirection - eyeFadeStart) / (eyeFadeEnd - eyeFadeStart));


	// Fade ray hits based on distance from ray origin
	alpha *= 1.0 - clamp( distance( vsRayOrigin, hitPoint) / _MaxRayDistance, 0.0, 1.0);

	alpha *= intersect ? 1.0 : 0.0;
		
	return alpha;
}

DECLARE_INPUT
    INPUT_SCREEN_POSITION
    INPUT( vec2, mTexCoordsVec2,  TEXCOORD0 )
    //INPUT( vec4, mCornerRayVec4, TEXCOORD5 )
DECLARE_END

vec3
RecreateViewPositionFromDepth(
    in float lfDepth,
    in vec2  lFragCoordsVec2,
    in mat4  lInverseProjectionMatrix )
{
    vec4 lPositionVec4;
    lPositionVec4.x = lFragCoordsVec2.x * 2.0 - 1.0;

#ifdef D_PLATFORM_ORBIS    
    lPositionVec4.y = ( 1.0f-lFragCoordsVec2.y ) * 2.0 - 1.0;
#else
    lPositionVec4.y = lFragCoordsVec2.y * 2.0 - 1.0;
#endif

    lPositionVec4.z = 0.0;
    lPositionVec4.w = 1.0;

    // Inverse projection
    lPositionVec4        = MUL( lInverseProjectionMatrix, lPositionVec4 );
    lPositionVec4        = lPositionVec4 / lPositionVec4.w;

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
    vec4 lBuffer1_Vec4 = texture2D( lBuffer1Map, lScreenPosVec2 );

    float lfDepth      = FastDenormaliseDepth( lClipPlanes, DecodeDepthFromColour( lBuffer1_Vec4 ) );
    vec3 lPositionVec3 = RecreateViewPositionFromDepth( lfDepth, lScreenPosVec2, lInverseProjectionMat4 );

    return lPositionVec3;
}

FRAGMENT_MAIN_COLOUR_SRT
{
    {
        _Iterations                             = 20.0;							// maximum ray iterations
        _BinarySearchIterations                 = 32.0;				// maximum binary search refinement iterations
        _PixelZSize                             = 0.1;							// Z size in camera space of a pixel in the depth buffer
        _PixelStride                            = 1.0;							// number of pixels per ray step close to camera
        _PixelStrideZCuttoff                    = 100.0;					// ray origin Z at this distance will have a pixel stride of 1.0
        _MaxRayDistance                         = 10.0;						// maximum distance of a ray
        _ScreenEdgeFadeStart                    = 0.75;					// distance to screen edge that ray hits will start to fade (0.0 -> 1.0)
        _EyeFadeStart                           = 0.0;					// ray direction's Z that ray hits will start to fade (0.0 -> 1.0)
        _EyeFadeEnd                             = 1.0;					// ray direction's Z that ray hits will be cut (0.0 -> 1.0)
        _RenderBufferSize                       = lUniforms.mpPerFrame->gFrameBufferSizeVec4.xy;
        _OneDividedByRenderBufferSize           = lUniforms.mpPerFrame->gFrameBufferSizeVec4.zw;
        _CameraProjectionMatrix                 = inverse(lUniforms.mpCommonPerMesh->gInverseProjectionMat4);			// projection matrix that maps to screen pixels (not NDC)

        // x is 1.0 ( or –1.0 if currently rendering with a flipped projection matrix ), y is the camera’s near plane, z is the camera’s far plane and w is 1 / FarPlane.
        _ProjectionParams = vec4( 1.0, 
                                  lUniforms.mpPerFrame->gClipPlanesVec4.x, 
                                  lUniforms.mpPerFrame->gClipPlanesVec4.y, 
                                  lUniforms.mpPerFrame->gClipPlanesRVec4.y );
    }

    vec2 lFragCoordsVec2 = IN( mTexCoordsVec2 );
	//vec4 specRoughPixel = texture2D( _CameraGBufferTexture1, lFragCoordsVec2 );
    vec4 specRoughPixel = vec4( 1.0, 1.0, 1.0, 1.0 );
    //float specularStrength = specRoughPixel.a;
    float specularStrength;

    {
        vec4 lBuffer3_Vec4;
        lBuffer3_Vec4    = texture2D( lUniforms.mpCustomPerMesh->gBuffer3Map, lFragCoordsVec2 );
        specularStrength = ( lBuffer3_Vec4.g * 16.0 );
        specularStrength = 1.0 - specularStrength;
    }

    vec3 vsRayOrigin = DecodeGBufferViewPosition(   lFragCoordsVec2,
                                                    lUniforms.mpPerFrame->gClipPlanesVec4,
                                                    lUniforms.mpCommonPerMesh->gInverseProjectionMat4,
                                                    SAMPLER2DPARAM( lUniforms.mpCustomPerMesh->gBuffer1Map ) );


	//vec3 decodedNormal = (texture2D( _CameraGBufferTexture2, lFragCoordsVec2 )).rgb * 2.0 - 1.0;
	//decodedNormal      = mul( (vec3x3)_NormalMatrix, decodedNormal); // worldToCameraMatrix
    vec3 lViewNormal;
    {
        vec4 lBuffer2_Vec4;
        lBuffer2_Vec4 = texture2D( lUniforms.mpCustomPerMesh->gBuffer2Map, lFragCoordsVec2 );
        vec3 lNormalVec3   = DecodeNormal( lBuffer2_Vec4.xyz );

        mat3 lCameraMatInverseRotMat3;
        mat4 lCameraMatInverseMat4;

        lCameraMatInverseMat4 = transpose( lUniforms.mpPerFrame->gCameraMat4 );
        lCameraMatInverseRotMat3[0] = lCameraMatInverseMat4[0].xyz;
        lCameraMatInverseRotMat3[1] = lCameraMatInverseMat4[1].xyz;
        lCameraMatInverseRotMat3[2] = lCameraMatInverseMat4[2].xyz;

        lViewNormal = MUL( lCameraMatInverseRotMat3, lNormalVec3 );
    }
				
	vec3 vsRayDirection = normalize( reflect( normalize( vsRayOrigin), normalize( lViewNormal )));
				
	vec2 hitPixel; 
	vec3 hitPoint;
	float iterationCount;
				
	vec2 lPixelCoordVec2 = lFragCoordsVec2 * lUniforms.mpPerFrame->gFrameBufferSizeVec4.xy;
	float c = (lPixelCoordVec2.x + lPixelCoordVec2.y) * 0.25;
	float jitter = mod( c, 1.0);
			
	bool intersect = TraceScreenSpaceRay( vsRayOrigin, vsRayDirection, jitter, hitPixel, hitPoint, iterationCount, lUniforms.mpPerFrame->gClipPlanesVec4, SAMPLER2DPARAM( lUniforms.mpCustomPerMesh->gBuffer1Map ) );
	float alpha    = CalculateAlphaForIntersection( intersect, iterationCount, specularStrength, hitPixel, hitPoint, vsRayOrigin, vsRayDirection);
	hitPixel       = mix( lFragCoordsVec2, hitPixel, intersect ? 1.0 : 0.0 );
				
	// Comment out the line below to get faked specular,
	// in no way physically correct but will tint based
	// on spec. Physically correct handling of spec is coming...
	specRoughPixel = vec4( 1.0, 1.0, 1.0, 1.0);
				
	FRAGMENT_COLOUR = vec4( texture2D( lUniforms.mpCustomPerMesh->gBufferMap, hitPixel ).rgb * specRoughPixel.rgb, alpha );
}

#if 0

// By Morgan McGuire and Michael Mara at Williams College 2014
// Released as open source under the BSD 2-Clause License
// http://opensource.org/licenses/BSD-2-Clause
#define point2 vec2
#define point3 vec3

float distanceSquared( vec2 a, vec2 b ) { a -= b; return dot( a, a ); }

// Returns true if the ray hit something
bool 
traceScreenSpaceRay1(
    point3 csOrig, // Camera-space ray origin, which must be within the view volume
    vec3 csDir, // Unit length camera-space ray direction
    mat4x4 proj, // A projection matrix that maps to pixel coordinates (not [-1, +1] normalized device coordinates)
    sampler2D csZBuffer,// The camera-space Z buffer (all negative values)
    vec2 csZBufferSize, // Dimensions of csZBuffer
    float zThickness, // Camera space thickness to ascribe to each pixel in the depth buffer
    float nearPlaneZ, // (Negative number)
    float stride, // Step in horizontal or vertical pixels between samples. This is a float because integer math is slow on GPUs, but should be set to an integer >= 1
    float jitter, // Number between 0 and 1 for how far to bump the ray in stride units to conceal banding artifacts
    const float maxSteps, // Maximum number of iterations. Higher gives better images but may be slow
    float maxDistance,  // Maximum camera-space distance to trace before returning a miss
    out point2 hitPixel, // Pixel coordinates of the first intersection with the scene
    out point3 hitPoint ) // Camera space location of the ray hit
{

    // Clip to the near plane    
    float rayLength = ( ( csOrig.z + csDir.z * maxDistance ) > nearPlaneZ ) ?
        ( nearPlaneZ - csOrig.z ) / csDir.z : maxDistance;
    point3 csEndPoint = csOrig + csDir * rayLength;

    // Project into homogeneous clip space
    vec4 H0 = proj * vec4( csOrig, 1.0 );
    vec4 H1 = proj * vec4( csEndPoint, 1.0 );
    float k0 = 1.0 / H0.w, k1 = 1.0 / H1.w;

    // The interpolated homogeneous version of the camera-space points  
    point3 Q0 = csOrig * k0, Q1 = csEndPoint * k1;

    // Screen-space endpoints
    point2 P0 = H0.xy * k0, P1 = H1.xy * k1;

    // If the line is degenerate, make it cover at least one pixel
    // to avoid handling zero-pixel extent as a special case later
    P1 += vec2( ( distanceSquared( P0, P1 ) < 0.0001 ) ? 0.01 : 0.0 );
    vec2 delta = P1 - P0;

    // Permute so that the primary iteration is in x to collapse
    // all quadrant-specific DDA cases later
    bool permute = false;
    if ( abs( delta.x ) < abs( delta.y ) )
    {
        // This is a more-vertical line
        permute = true; delta = delta.yx; P0 = P0.yx; P1 = P1.yx;
    }

    float stepDir = sign( delta.x );
    float invdx = stepDir / delta.x;

    // Track the derivatives of Q and k
    vec3  dQ = ( Q1 - Q0 ) * invdx;
    float dk = ( k1 - k0 ) * invdx;
    vec2  dP = vec2( stepDir, delta.y * invdx );

    // Scale derivatives by the desired pixel stride and then
    // offset the starting values by the jitter fraction
    dP *= stride; dQ *= stride; dk *= stride;
    P0 += dP * jitter; Q0 += dQ * jitter; k0 += dk * jitter;

    // Slide P from P0 to P1, (now-homogeneous) Q from Q0 to Q1, k from k0 to k1
    point3 Q = Q0;

    // Adjust end condition for iteration direction
    float  end = P1.x * stepDir;

    float k = k0, stepCount = 0.0, prevZMaxEstimate = csOrig.z;
    float rayZMin = prevZMaxEstimate, rayZMax = prevZMaxEstimate;
    float sceneZMax = rayZMax + 100;
    for ( point2 P = P0;
        ( ( P.x * stepDir ) <= end ) && ( stepCount < maxSteps ) &&
        ( ( rayZMax < sceneZMax - zThickness ) || ( rayZMin > sceneZMax ) ) &&
        ( sceneZMax != 0 );
    P += dP, Q.z += dQ.z, k += dk, ++stepCount )
    {

        rayZMin = prevZMaxEstimate;
        rayZMax = ( dQ.z * 0.5 + Q.z ) / ( dk * 0.5 + k );
        prevZMaxEstimate = rayZMax;
        if ( rayZMin > rayZMax )
        {
            float t = rayZMin; rayZMin = rayZMax; rayZMax = t;
        }

        hitPixel = permute ? P.yx : P;
        // You may need hitPixel.y = csZBufferSize.y - hitPixel.y; here if your vertical axis
        // is different than ours in screen space
        sceneZMax = texelFetch( csZBuffer, int2( hitPixel ), 0 );
    }

    // Advance Q based on the number of steps
    Q.xy += dQ.xy * stepCount;
    hitPoint = Q * ( 1.0 / k );
    return ( rayZMax >= sceneZMax - zThickness ) && ( rayZMin < sceneZMax );
}

#endif
