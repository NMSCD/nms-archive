#ifndef D_COMPUTE_NOISE_COMMON_H
#define D_COMPUTE_NOISE_COMMON_H

#include "Common/Defines.shader.h"

//Simplex noise permutation lookup table (repeated to allow for indexing between 256-511)
STATIC_CONST uint gNoisePermutationTable[512] = 
{
	151, 160, 137, 91, 90, 15,
	131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
	190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
	88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
	77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
	102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
	135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
	5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
	223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
	129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
	251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
	49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
	138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,
	151, 160, 137, 91, 90, 15,
	131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
	190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
	88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
	77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
	102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
	135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
	5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
	223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
	129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
	251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
	49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
	138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};

//Macro to use permutation table to generate a random number from 3 integers (must be between 0 and 511)
#define SIMPLEX_NOISE_HASH(x,y,z) gNoisePermutationTable[(x) + gNoisePermutationTable[(y) + gNoisePermutationTable[(z)]]]

//Lookup table for grad3 vectors (12 edges of a cube)
STATIC_CONST vec3 gNoiseGrad3LUT[16] = 
{
	vec3(1.0f, 0.0f, 1.0f),
	vec3(0.0f, 1.0f, 1.0f), // 12 cube edges
	vec3(-1.0f, 0.0f, 1.0f),
	vec3(0.0f, -1.0f, 1.0f),
	vec3(1.0f, 0.0f, -1.0f),
	vec3(0.0f, 1.0f, -1.0f),
	vec3(-1.0f, 0.0f, -1.0f),
	vec3(0.0f, -1.0f, -1.0f),
	vec3(1.0f, -1.0f, 0.0f),
	vec3(1.0f, 1.0f, 0.0f),
	vec3(-1.0f, 1.0f, 0.0f),
	vec3(-1.0f, -1.0f, 0.0f),
	vec3(1.0f, 0.0f, 1.0f),
	vec3(-1.0f, 0.0f, 1.0f), // 4 repeats to make 16
	vec3(0.0f, 1.0f, -1.0f),
	vec3(0.0f, -1.0f, -1.0f)
};

//Uses lookup table to get a gradient for a given hash
vec3 NoiseGrad3(int hash)
{
	int h = hash & 15;
	return gNoiseGrad3LUT[h];
}
void NoiseGrad3(int hash, out float x, out float y, out float z)
{
	vec3 res = NoiseGrad3(hash);
	x = res.x;
	y = res.y;
	z = res.z;
}

//Constants for noise calculation
STATIC_CONST float F3 = 1.0f / 3.0f;
STATIC_CONST float G3 = 1.0f / 6.0f; // Very nice and simple unskew factor, too

//Main noise 3d function
float
Noise3d(float x, float y, float z)
{
	float noise;          /* Return value */
	float x1, y1, z1, x2, y2, z2, x3, y3, z3;
	float t0, t1, t2, t3, t20, t40, t21, t41, t22, t42, t23, t43;

	/* Skew the input space to determine which simplex cell we're in */
	float s = (x + y + z)*F3; /* Very nice and simple skew factor for 3D */
	float xs = x + s;
	float ys = y + s;
	float zs = z + s;
	int i = floor(xs);
	int j = floor(ys);
	int k = floor(zs);

	float t = (float)(i + j + k)*G3;
	float X0 = i - t; /* Unskew the cell origin back to (x,y,z) space */
	float Y0 = j - t;
	float Z0 = k - t;
	float x0 = x - X0; /* The x,y,z distances from the cell origin */
	float y0 = y - Y0;
	float z0 = z - Z0;

	/* For the 3D case, the simplex shape is a slightly irregular tetrahedron.
	* Determine which simplex we are in. */
	int i1, j1, k1; /* Offsets for second corner of simplex in (i,j,k) coords */
	int i2, j2, k2; /* Offsets for third corner of simplex in (i,j,k) coords */

	/* TODO: This code would benefit from a backport from the GLSL version! */
	if (x0 >= y0) {
		if (y0 >= z0)
		{
			i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
		} /* X Y Z order */
		else if (x0 >= z0) { i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1; } /* X Z Y order */
		else { i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1; } /* Z X Y order */
	}
	else { // x0<y0
		if (y0 < z0) { i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1; } /* Z Y X order */
		else if (x0 < z0) { i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1; } /* Y Z X order */
		else { i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0; } /* Y X Z order */
	}

	/* A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
	* a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
	* a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
	* c = 1/6.   */

	x1 = x0 - i1 + G3; /* Offsets for second corner in (x,y,z) coords */
	y1 = y0 - j1 + G3;
	z1 = z0 - k1 + G3;
	x2 = x0 - i2 + 2.0f * G3; /* Offsets for third corner in (x,y,z) coords */
	y2 = y0 - j2 + 2.0f * G3;
	z2 = z0 - k2 + 2.0f * G3;
	x3 = x0 - 1.0f + 3.0f * G3; /* Offsets for last corner in (x,y,z) coords */
	y3 = y0 - 1.0f + 3.0f * G3;
	z3 = z0 - 1.0f + 3.0f * G3;

	/* Wrap the integer indices at 256, to avoid indexing perm[] out of bounds */
	int ii = i & 255;
	int jj = j & 255;
	int kk = k & 255;


	/* Calculate the contribution from the four corners */
	float n = 0;
	t0 = 0.6f - x0*x0 - y0*y0 - z0*z0;
	if (t0 >= 0.0f)
	{
		vec3 g = NoiseGrad3(SIMPLEX_NOISE_HASH(ii, jj, kk));
		t20 = t0 * t0;
		t40 = t20 * t20;
		n += t40 * (g.x * x0 + g.y * y0 + g.z * z0);
	}

	t1 = 0.6f - x1*x1 - y1*y1 - z1*z1;
	if (t1 >= 0.0f)
	{
		vec3 g = NoiseGrad3(SIMPLEX_NOISE_HASH(ii + i1, jj + j1, kk + k1));
		t21 = t1 * t1;
		t41 = t21 * t21;
		n += t41 * (g.x * x1 + g.y * y1 + g.z * z1);
	}

	t2 = 0.6f - x2*x2 - y2*y2 - z2*z2;
	if (t2 >= 0.0f)
	{
		vec3 g = NoiseGrad3(SIMPLEX_NOISE_HASH(ii + i2, jj + j2, kk + k2));
		t22 = t2 * t2;
		t42 = t22 * t22;
		n += t42 * (g.x * x2 + g.y * y2 + g.z * z2);
	}

	t3 = 0.6f - x3*x3 - y3*y3 - z3*z3;
	if (t3 >= 0.0f)
	{
		vec3 g = NoiseGrad3(SIMPLEX_NOISE_HASH(ii + 1, jj + 1, kk + 1));
		t23 = t3 * t3;
		t43 = t23 * t23;
		n += t43 * (g.x * x3 + g.y * y3 + g.z * z3);
	}

	/*  Add contributions from each corner to get the final noise value.
	* The result is scaled to return values in the range [-1,1] */
	noise = 28.0f * n;
	return noise;
}

//Main noise 3d function with derivatives
float
Noise3d(float x, float y, float z, out float dnoise_dx, out float dnoise_dy, out float dnoise_dz)
{
	float n0, n1, n2, n3; /* Noise contributions from the four simplex corners */
	float noise;          /* Return value */
	float gx0, gy0, gz0, gx1, gy1, gz1; /* Gradients at simplex corners */
	float gx2, gy2, gz2, gx3, gy3, gz3;
	float x1, y1, z1, x2, y2, z2, x3, y3, z3;
	float t0, t1, t2, t3, t20, t40, t21, t41, t22, t42, t23, t43;
	float temp0, temp1, temp2, temp3;

	/* Skew the input space to determine which simplex cell we're in */
	float s = (x + y + z)*F3; /* Very nice and simple skew factor for 3D */
	float xs = x + s;
	float ys = y + s;
	float zs = z + s;
	int ii, i = floor(xs);
	int jj, j = floor(ys);
	int kk, k = floor(zs);

	float t = (float)(i + j + k)*G3;
	float X0 = i - t; /* Unskew the cell origin back to (x,y,z) space */
	float Y0 = j - t;
	float Z0 = k - t;
	float x0 = x - X0; /* The x,y,z distances from the cell origin */
	float y0 = y - Y0;
	float z0 = z - Z0;

	/* For the 3D case, the simplex shape is a slightly irregular tetrahedron.
	* Determine which simplex we are in. */
	int i1, j1, k1; /* Offsets for second corner of simplex in (i,j,k) coords */
	int i2, j2, k2; /* Offsets for third corner of simplex in (i,j,k) coords */

	/* TODO: This code would benefit from a backport from the GLSL version! */
	if (x0 >= y0) {
		if (y0 >= z0)
		{
			i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
		} /* X Y Z order */
		else if (x0 >= z0) { i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1; } /* X Z Y order */
		else { i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1; } /* Z X Y order */
	}
	else { // x0<y0
		if (y0<z0) { i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1; } /* Z Y X order */
		else if (x0<z0) { i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1; } /* Y Z X order */
		else { i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0; } /* Y X Z order */
	}

	/* A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
	* a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
	* a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
	* c = 1/6.   */

	x1 = x0 - i1 + G3; /* Offsets for second corner in (x,y,z) coords */
	y1 = y0 - j1 + G3;
	z1 = z0 - k1 + G3;
	x2 = x0 - i2 + 2.0f * G3; /* Offsets for third corner in (x,y,z) coords */
	y2 = y0 - j2 + 2.0f * G3;
	z2 = z0 - k2 + 2.0f * G3;
	x3 = x0 - 1.0f + 3.0f * G3; /* Offsets for last corner in (x,y,z) coords */
	y3 = y0 - 1.0f + 3.0f * G3;
	z3 = z0 - 1.0f + 3.0f * G3;

	/* Wrap the integer indices at 256, to avoid indexing perm[] out of bounds */
	//     ii = kTkMath.Wrap( i, 0, 256 );
	//     jj = kTkMath.Wrap( j, 0, 256 );
	//     kk = kTkMath.Wrap( k, 0, 256 );
	ii = i & 255;
	jj = j & 255;
	kk = k & 255;


	/* Calculate the contribution from the four corners */
	t0 = 0.6f - x0*x0 - y0*y0 - z0*z0;
	if (t0 < 0.0f)
	{
		n0 = t0 = t20 = t40 = gx0 = gy0 = gz0 = 0.0f;
	}
	else 
	{
		NoiseGrad3(SIMPLEX_NOISE_HASH(ii, jj, kk), gx0, gy0, gz0);
		t20 = t0 * t0;
		t40 = t20 * t20;
		n0 = t40 * (gx0 * x0 + gy0 * y0 + gz0 * z0);
	}

	t1 = 0.6f - x1*x1 - y1*y1 - z1*z1;
	if (t1 < 0.0f)
	{
		n1 = t1 = t21 = t41 = gx1 = gy1 = gz1 = 0.0f;
	}
	else 
	{
		NoiseGrad3(SIMPLEX_NOISE_HASH(ii + i1, jj + j1, kk + k1), gx1, gy1, gz1);
		t21 = t1 * t1;
		t41 = t21 * t21;
		n1 = t41 * (gx1 * x1 + gy1 * y1 + gz1 * z1);
	}

	t2 = 0.6f - x2*x2 - y2*y2 - z2*z2;
	if (t2 < 0.0f)
	{
		n2 = t2 = t22 = t42 = gx2 = gy2 = gz2 = 0.0f;
	}
	else 
	{
		NoiseGrad3(SIMPLEX_NOISE_HASH(ii + i2, jj + j2, kk + k2), gx2, gy2, gz2);
		t22 = t2 * t2;
		t42 = t22 * t22;
		n2 = t42 * (gx2 * x2 + gy2 * y2 + gz2 * z2);
	}

	t3 = 0.6f - x3*x3 - y3*y3 - z3*z3;
	if (t3 < 0.0f)
	{
		n3 = t3 = t23 = t43 = gx3 = gy3 = gz3 = 0.0f;
	}
	else 
	{
		NoiseGrad3(SIMPLEX_NOISE_HASH(ii + 1, jj + 1, kk + 1), gx3, gy3, gz3);
		t23 = t3 * t3;
		t43 = t23 * t23;
		n3 = t43 * (gx3 * x3 + gy3 * y3 + gz3 * z3);
	}

	/*  Add contributions from each corner to get the final noise value.
	* The result is scaled to return values in the range [-1,1] */
	noise = 28.0f * (n0 + n1 + n2 + n3);

	/* Compute derivative, if requested by supplying non-null pointers
	* for the last three arguments */
	{
		/*  A straight, unoptimised calculation would be like:
		*     *dnoise_dx = -8.0f * t20 * t0 * x0 * dot(gx0, gy0, gz0, x0, y0, z0) + t40 * gx0;
		*    *dnoise_dy = -8.0f * t20 * t0 * y0 * dot(gx0, gy0, gz0, x0, y0, z0) + t40 * gy0;
		*    *dnoise_dz = -8.0f * t20 * t0 * z0 * dot(gx0, gy0, gz0, x0, y0, z0) + t40 * gz0;
		*    *dnoise_dx += -8.0f * t21 * t1 * x1 * dot(gx1, gy1, gz1, x1, y1, z1) + t41 * gx1;
		*    *dnoise_dy += -8.0f * t21 * t1 * y1 * dot(gx1, gy1, gz1, x1, y1, z1) + t41 * gy1;
		*    *dnoise_dz += -8.0f * t21 * t1 * z1 * dot(gx1, gy1, gz1, x1, y1, z1) + t41 * gz1;
		*    *dnoise_dx += -8.0f * t22 * t2 * x2 * dot(gx2, gy2, gz2, x2, y2, z2) + t42 * gx2;
		*    *dnoise_dy += -8.0f * t22 * t2 * y2 * dot(gx2, gy2, gz2, x2, y2, z2) + t42 * gy2;
		*    *dnoise_dz += -8.0f * t22 * t2 * z2 * dot(gx2, gy2, gz2, x2, y2, z2) + t42 * gz2;
		*    *dnoise_dx += -8.0f * t23 * t3 * x3 * dot(gx3, gy3, gz3, x3, y3, z3) + t43 * gx3;
		*    *dnoise_dy += -8.0f * t23 * t3 * y3 * dot(gx3, gy3, gz3, x3, y3, z3) + t43 * gy3;
		*    *dnoise_dz += -8.0f * t23 * t3 * z3 * dot(gx3, gy3, gz3, x3, y3, z3) + t43 * gz3;
		*/
		temp0 = t20 * t0 * (gx0 * x0 + gy0 * y0 + gz0 * z0);
		dnoise_dx = temp0 * x0;
		dnoise_dy = temp0 * y0;
		dnoise_dz = temp0 * z0;
		temp1 = t21 * t1 * (gx1 * x1 + gy1 * y1 + gz1 * z1);
		dnoise_dx += temp1 * x1;
		dnoise_dy += temp1 * y1;
		dnoise_dz += temp1 * z1;
		temp2 = t22 * t2 * (gx2 * x2 + gy2 * y2 + gz2 * z2);
		dnoise_dx += temp2 * x2;
		dnoise_dy += temp2 * y2;
		dnoise_dz += temp2 * z2;
		temp3 = t23 * t3 * (gx3 * x3 + gy3 * y3 + gz3 * z3);
		dnoise_dx += temp3 * x3;
		dnoise_dy += temp3 * y3;
		dnoise_dz += temp3 * z3;
		dnoise_dx *= -8.0f;
		dnoise_dy *= -8.0f;
		dnoise_dz *= -8.0f;
		dnoise_dx += t40 * gx0 + t41 * gx1 + t42 * gx2 + t43 * gx3;
		dnoise_dy += t40 * gy0 + t41 * gy1 + t42 * gy2 + t43 * gy3;
		dnoise_dz += t40 * gz0 + t41 * gz1 + t42 * gz2 + t43 * gz3;
		dnoise_dx *= 28.0f; /* Scale derivative to match the noise scaling */
		dnoise_dy *= 28.0f;
		dnoise_dz *= 28.0f;
	}

	return noise;
}

struct GlobalUniforms
{
	RegularBuffer<float3>		mRequests;
	RW_RegularBuffer<float>		mNoiseResults;
	RW_RegularBuffer<float3>	mDerivResults;
};

struct PerDispatchUniforms
{
	int	  miFirstRequest;
	int	  miNumRequests;
	int   miOctaves;
	float mfAmplifyFeatures;		//! slider[0.0, 0.0, 0.5]
	float mfPerturbFeatures;		//! slider[-0.4, 0.0, 0.4]
	float mfSharpToRoundFeatures;	//! slider[-1.0, 0.0, 1.0]
	float mfAltitudeErosion;		//! slider[0.0, 0.0, 0.25]
	float mfRidgeErosion;			//! slider[0.0, 0.0, 1.0]
	float mfSlopeErosion;			//! slider[0.0, 0.0, 1.0]
	float mfLacunarity;				// = 2.0f
	float mfGain;					// = 0.5f

};
struct Srt
{
	GlobalUniforms* mpGlobalUniforms;
	PerDispatchUniforms* mpPerDispatchUniforms;
};

#endif