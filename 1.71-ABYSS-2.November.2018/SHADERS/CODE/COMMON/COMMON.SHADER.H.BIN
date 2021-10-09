////////////////////////////////////////////////////////////////////////////////
///
///     @file       Common.h
///     @author     User
///     @date       
///
///     @brief      Common
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef D_COMMON_H
#define D_COMMON_H

#define D_TERRAINCOLOURARRAY_SIZE 23


STATIC_CONST vec3 kGammaOutVec3 = vec3( 1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2 );
STATIC_CONST vec3 kGammaInVec3  = vec3( 2.2, 2.2, 2.2 );
STATIC_CONST vec4 RGBToHSV_K    = vec4( 0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0 );
STATIC_CONST vec4 HSVToRGB_K    = vec4( 1.0,  2.0 / 3.0, 1.0 / 3.0,  3.0 );
STATIC_CONST mat3 BT709_TO_BT2020 = mat3(   //ref: ARIB STD-B62 and BT.2087
	0.6274, 0.3293, 0.0433,
	0.0691, 0.9195, 0.0114,
	0.0164, 0.0880, 0.8956
    );
STATIC_CONST mat3 BT2020_TO_BT709 = mat3(
    1.6605, -0.5877, -0.0728,
    -0.1246, 1.1330, -0.0084,
    -0.0182, -0.1006, 1.1187
    );

//-----------------------------------------------------------------------------
///
///     GammaCorrect
///
//-----------------------------------------------------------------------------
vec3 
GammaCorrectInput(
    in vec3 lColourVec3 )
{
    vec3 lCorrectColourVec3;

    lCorrectColourVec3 = lColourVec3 * ( lColourVec3 * ( lColourVec3 * float2vec3( 0.305306011 ) + float2vec3( 0.682171111 ) ) + float2vec3( 0.012522878 ) );

    return lCorrectColourVec3;
}


//-----------------------------------------------------------------------------
///
///     GammaCorrect
///
//-----------------------------------------------------------------------------
vec3 
GammaCorrectOutput(
    in vec3 lColourVec3 )
{
    vec3 lCorrectColourVec3;

    lCorrectColourVec3 = pow( lColourVec3, kGammaOutVec3 );

    return lCorrectColourVec3;
}


//-----------------------------------------------------------------------------
///
///     RGBToHSV
///
//-----------------------------------------------------------------------------
vec3 
RGBToHSV(
    vec3 lRGB )
{
    //vec4 p = mix( vec4(lRGB.bg, RGBToHSV_K.wz), vec4(lRGB.gb, RGBToHSV_K.xy), step(lRGB.b, lRGB.g) );
    //vec4 q = mix( vec4(p.xyw, lRGB.r), vec4(lRGB.r, p.yzx), step(p.x, lRGB.r) );
    // This variant is faster, since it generates conditional moves
    vec4 p = lRGB.g < lRGB.b ? vec4(lRGB.bg, RGBToHSV_K.wz) : vec4(lRGB.gb, RGBToHSV_K.xy);
    vec4 q = lRGB.r < p.x ? vec4(p.xyw, lRGB.r) : vec4(lRGB.r, p.yzx);    
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}


//-----------------------------------------------------------------------------
///
///     HSVToRGB
///
///     @brief      http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
///
//-----------------------------------------------------------------------------
vec3 
HSVToRGB(
    vec3 lHSV )
{
    vec3 p = abs(fract(lHSV.xxx + HSVToRGB_K.xyz) * 6.0 - HSVToRGB_K.www);
    return lHSV.z * mix(HSVToRGB_K.xxx, saturate(p - HSVToRGB_K.xxx), lHSV.y);
}



//-----------------------------------------------------------------------------
///
///     HDR ( Perceptual Quantizer(PQ), Rec. 2020 color space. ) helpers
///
//------------------------------------------------------------------------------

#if !defined( D_PLATFORM_ORBIS)
float CndMask(bool cnd, float src0, float src1)
{
    return cnd ? src0 : src1;
}
#endif

STATIC_CONST float kRefWhiteLevel = 100.0;

vec4 SRGB_OETF(vec4 L)
{
	vec3 dark = L.xyz * 12.92;
	vec3 light = 1.055 * pow(L.xyz, float2vec3(1.0 / 2.4)) - 0.055;

	vec4 r;
    r.x = CndMask(L.x <= 0.0031308, dark.x, light.x);
    r.y = CndMask(L.y <= 0.0031308, dark.y, light.y);
    r.z = CndMask(L.z <= 0.0031308, dark.z, light.z);
	r.w = L.w;
	return r;
}

vec4 SRGB_EOTF(vec4 E)
{
	vec3 dark = E.xyz / 12.92;
	vec3 light = pow((E.xyz + 0.055) / (1 + 0.055), float2vec3(2.4));
	vec4 r;
    r.x = CndMask(E.x <= 0.04045, dark.x, light.x);
    r.y = CndMask(E.y <= 0.04045, dark.y, light.y);
    r.z = CndMask(E.z <= 0.04045, dark.z, light.z);
	r.w = E.w;
	return r;
}

//apply gamma adjustment to (minL, maxL).
vec4 GammaAdjOOTF(vec4 L, float minLNits, float maxLNits, float gamma, bool inverse)
{
	vec3 nits = L.xyz * kRefWhiteLevel;
	
	vec4 i = vec4((nits - minLNits) / (maxLNits - minLNits), 1.0);
	vec3 j;
	if (inverse){
		j = SRGB_EOTF(pow(i, float2vec4(1 / gamma))).xyz;
	}
	else{
		j = pow(SRGB_OETF(i).xyz,float2vec3(gamma));
	}
	vec3 adj = (minLNits + (maxLNits - minLNits) * j) / kRefWhiteLevel;
	vec4 ret;
    ret.x = CndMask(nits.x >= minLNits && nits.x < maxLNits, adj.x, L.x);
    ret.y = CndMask(nits.y >= minLNits && nits.y < maxLNits, adj.y, L.y);
    ret.z = CndMask(nits.z >= minLNits && nits.z < maxLNits, adj.z, L.z);
	ret.w = L.w;
	return ret;
}

//input: normalized L in units of RefWhite (1.0=100nits), output: normalized E
vec4 PQ_OETF(vec4 L, uint gamma_adj, float gamma)
{
	if (gamma_adj != 0)
		L = GammaAdjOOTF(L, 0.0, 300.0, gamma, false);
	const float c1 = 0.8359375;//3424.f/4096.f;
	const float c2 = 18.8515625;//2413.f/4096.f*32.f;
	const float c3 = 18.6875; //2392.f/4096.f*32.f;
	const float m1 = 0.159301758125; //2610.f / 4096.f / 4;
	const float m2 = 78.84375;// 2523.f / 4096.f * 128.f;
	L = L * kRefWhiteLevel / 10000.0;
	vec3 Lm1 = pow(L.xyz, float2vec3(m1));
	vec3 X = (c1 + c2 * Lm1) / (1 + c3 * Lm1);
	vec4 res = vec4(pow(X, float2vec3(m2)), L.w);
	return res;
}

//input: normalized E (0.0, 1.0), output: normalized L in units of RefWhite
vec4 PQ_EOTF(vec4 E, uint gamma_adj, float gamma)
{
	const float c1 = 0.8359375;//3424.f/4096.f;
	const float c2 = 18.8515625;//2413.f/4096.f*32.f;
	const float c3 = 18.6875; //2392.f/4096.f*32.f;
	const float m1 = 0.159301758125; //2610.f / 4096.f / 4;
	const float m2 = 78.84375;// 2523.f / 4096.f * 128.f;
	vec3 M = c2 - c3 * pow(E.xyz, float2vec3(1 / m2));
	vec3 N = max(pow(E.xyz, float2vec3(1 / m2)) - c1, 0);
	vec3 L = pow(N / M, float2vec3(1 / m1)); //normalized nits (1.0 = 10000nits)
	L = L * 10000.0 / kRefWhiteLevel; //convert to normalized L in units of RefWhite
	return (gamma_adj !=0) ? GammaAdjOOTF(vec4(L, E.w), 0.0, 300.0, gamma, true) : vec4(L, E.w);

}

// PQ OETF fast approximation
// http://www.glowybits.com/blog/2017/01/04/ifl_iss_hdr_2/

vec3 PQ_OETF_Fast(vec3 x)
{
    x = (x * (x * (x * (x * (x * 533095.76 + 47438306.2) + 29063622.1) + 575216.76) + 383.09104) + 0.000487781) /
        (x * (x * (x * (x * 66391357.4 + 81884528.2) + 4182885.1) + 10668.404) + 1.0);
    return x;
}

//-----------------------------------------------------------------------------
///
///     BrightnessVibranceContrast
///
//-----------------------------------------------------------------------------
vec3 BrightnessVibranceContrast(
    vec3  lInputColour,
    float lfBrightness,
    float lfVibrance,
    float lfContrast)
{
    vec3 lBrtResult     = lInputColour * lfBrightness;

    // get lum
    vec3 lLuma          = float2vec3( dot(lBrtResult, vec3( 0.2125, 0.7154, 0.0721 )) );

    // get saturation
    float lfMaxCol      = max( lBrtResult.r, max(lBrtResult.g, lBrtResult.b) );
    float lfMinCol      = min( lBrtResult.r, min(lBrtResult.g, lBrtResult.b) );
    float lfCurSatV     = lfMaxCol - lfMinCol;

    // lerp by 1 + (1 - vibrance) - current saturation
    float lfVibranceMix = (1.0 + (lfVibrance * (1.0 - (sign(lfVibrance) * lfCurSatV))));
    vec3 lVibResult     = mix( lLuma, lBrtResult, lfVibranceMix );

    // lerp from mid gray for contrast
    vec3 lContrastBase  = vec3( 0.5, 0.5, 0.5 );
    vec3 lConResult     = mix( lContrastBase , lVibResult, lfContrast );

    return lConResult;
}


//-----------------------------------------------------------------------------
///
///     Desaturate
///
//-----------------------------------------------------------------------------
vec3 Desaturate( vec3 color, float lfAmount )
{
    vec3 gray = float2vec3( dot( vec3( 0.299, 0.587, 0.114 ), color) );
    return mix( color, gray, lfAmount );
}


//-----------------------------------------------------------------------------
///
///     Inverse
///
//-----------------------------------------------------------------------------
mat4
Inverse(
    const mat4 lInMat4 )
{
#ifdef D_PLATFORM_OPENGL
    return inverse( lInMat4 );
#else
    float det = determinant( lInMat4 );
    det = 1.0f / det;

    mat4 _M = lInMat4;
    mat4 IM;
    IM[0][0] = det * ( _M[1][2]*_M[2][3]*_M[3][1] - _M[1][3]*_M[2][2]*_M[3][1] + _M[1][3]*_M[2][1]*_M[3][2] - _M[1][1]*_M[2][3]*_M[3][2] - _M[1][2]*_M[2][1]*_M[3][3] + _M[1][1]*_M[2][2]*_M[3][3] );
    IM[0][1] = det * ( _M[0][3]*_M[2][2]*_M[3][1] - _M[0][2]*_M[2][3]*_M[3][1] - _M[0][3]*_M[2][1]*_M[3][2] + _M[0][1]*_M[2][3]*_M[3][2] + _M[0][2]*_M[2][1]*_M[3][3] - _M[0][1]*_M[2][2]*_M[3][3] );
    IM[0][2] = det * ( _M[0][2]*_M[1][3]*_M[3][1] - _M[0][3]*_M[1][2]*_M[3][1] + _M[0][3]*_M[1][1]*_M[3][2] - _M[0][1]*_M[1][3]*_M[3][2] - _M[0][2]*_M[1][1]*_M[3][3] + _M[0][1]*_M[1][2]*_M[3][3] );
    IM[0][3] = det * ( _M[0][3]*_M[1][2]*_M[2][1] - _M[0][2]*_M[1][3]*_M[2][1] - _M[0][3]*_M[1][1]*_M[2][2] + _M[0][1]*_M[1][3]*_M[2][2] + _M[0][2]*_M[1][1]*_M[2][3] - _M[0][1]*_M[1][2]*_M[2][3] );
    IM[1][0] = det * ( _M[1][3]*_M[2][2]*_M[3][0] - _M[1][2]*_M[2][3]*_M[3][0] - _M[1][3]*_M[2][0]*_M[3][2] + _M[1][0]*_M[2][3]*_M[3][2] + _M[1][2]*_M[2][0]*_M[3][3] - _M[1][0]*_M[2][2]*_M[3][3] );
    IM[1][1] = det * ( _M[0][2]*_M[2][3]*_M[3][0] - _M[0][3]*_M[2][2]*_M[3][0] + _M[0][3]*_M[2][0]*_M[3][2] - _M[0][0]*_M[2][3]*_M[3][2] - _M[0][2]*_M[2][0]*_M[3][3] + _M[0][0]*_M[2][2]*_M[3][3] );
    IM[1][2] = det * ( _M[0][3]*_M[1][2]*_M[3][0] - _M[0][2]*_M[1][3]*_M[3][0] - _M[0][3]*_M[1][0]*_M[3][2] + _M[0][0]*_M[1][3]*_M[3][2] + _M[0][2]*_M[1][0]*_M[3][3] - _M[0][0]*_M[1][2]*_M[3][3] );
    IM[1][3] = det * ( _M[0][2]*_M[1][3]*_M[2][0] - _M[0][3]*_M[1][2]*_M[2][0] + _M[0][3]*_M[1][0]*_M[2][2] - _M[0][0]*_M[1][3]*_M[2][2] - _M[0][2]*_M[1][0]*_M[2][3] + _M[0][0]*_M[1][2]*_M[2][3] );
    IM[2][0] = det * ( _M[1][1]*_M[2][3]*_M[3][0] - _M[1][3]*_M[2][1]*_M[3][0] + _M[1][3]*_M[2][0]*_M[3][1] - _M[1][0]*_M[2][3]*_M[3][1] - _M[1][1]*_M[2][0]*_M[3][3] + _M[1][0]*_M[2][1]*_M[3][3] );
    IM[2][1] = det * ( _M[0][3]*_M[2][1]*_M[3][0] - _M[0][1]*_M[2][3]*_M[3][0] - _M[0][3]*_M[2][0]*_M[3][1] + _M[0][0]*_M[2][3]*_M[3][1] + _M[0][1]*_M[2][0]*_M[3][3] - _M[0][0]*_M[2][1]*_M[3][3] );
    IM[2][2] = det * ( _M[0][1]*_M[1][3]*_M[3][0] - _M[0][3]*_M[1][1]*_M[3][0] + _M[0][3]*_M[1][0]*_M[3][1] - _M[0][0]*_M[1][3]*_M[3][1] - _M[0][1]*_M[1][0]*_M[3][3] + _M[0][0]*_M[1][1]*_M[3][3] );
    IM[2][3] = det * ( _M[0][3]*_M[1][1]*_M[2][0] - _M[0][1]*_M[1][3]*_M[2][0] - _M[0][3]*_M[1][0]*_M[2][1] + _M[0][0]*_M[1][3]*_M[2][1] + _M[0][1]*_M[1][0]*_M[2][3] - _M[0][0]*_M[1][1]*_M[2][3] );
    IM[3][0] = det * ( _M[1][2]*_M[2][1]*_M[3][0] - _M[1][1]*_M[2][2]*_M[3][0] - _M[1][2]*_M[2][0]*_M[3][1] + _M[1][0]*_M[2][2]*_M[3][1] + _M[1][1]*_M[2][0]*_M[3][2] - _M[1][0]*_M[2][1]*_M[3][2] );
    IM[3][1] = det * ( _M[0][1]*_M[2][2]*_M[3][0] - _M[0][2]*_M[2][1]*_M[3][0] + _M[0][2]*_M[2][0]*_M[3][1] - _M[0][0]*_M[2][2]*_M[3][1] - _M[0][1]*_M[2][0]*_M[3][2] + _M[0][0]*_M[2][1]*_M[3][2] );
    IM[3][2] = det * ( _M[0][2]*_M[1][1]*_M[3][0] - _M[0][1]*_M[1][2]*_M[3][0] - _M[0][2]*_M[1][0]*_M[3][1] + _M[0][0]*_M[1][2]*_M[3][1] + _M[0][1]*_M[1][0]*_M[3][2] - _M[0][0]*_M[1][1]*_M[3][2] );
    IM[3][3] = det * ( _M[0][1]*_M[1][2]*_M[2][0] - _M[0][2]*_M[1][1]*_M[2][0] + _M[0][2]*_M[1][0]*_M[2][1] - _M[0][0]*_M[1][2]*_M[2][1] - _M[0][1]*_M[1][0]*_M[2][2] + _M[0][0]*_M[1][1]*_M[2][2] );

    return IM;
#endif
}

mat2
Inverse(
    const mat2 lInMat2 )
{
#ifdef D_PLATFORM_OPENGL
    return inverse( lInMat2 );
#else
    float det = determinant( lInMat2 );
    det = 1.0f / det;

    mat2 _M = lInMat2;
    mat2 IM;
    IM[0][0] = det * _M[1][1];
    IM[1][1] = det * _M[0][0];
    IM[0][1] = det * -_M[1][0];
    IM[1][0] = det * -_M[0][1];

    return IM;
#endif
}

float
lengthSquared( vec3 lInVec3 )
{
    return dot( lInVec3, lInVec3 );
}

#endif