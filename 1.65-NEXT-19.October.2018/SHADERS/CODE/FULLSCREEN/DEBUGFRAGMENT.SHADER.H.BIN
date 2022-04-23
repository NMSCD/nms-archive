////////////////////////////////////////////////////////////////////////////////
///
///     @file       DebugFragment.h
///     @author     User
///     @date       
///
///     @brief      DebugFragment
///
///     Copyright (c) 2008 Hello Games Ltd. All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
//      Compilation defines 


//-----------------------------------------------------------------------------
//      Include files

#ifndef D_FRAGMENT
#define D_FRAGMENT
#endif
#include "Common/Defines.shader.h"
#include "Common/Common.shader.h"
#include "Common/CommonUniforms.shader.h"

#include "Fullscreen/PostCommon.h"

#include "Common/CommonPostProcess.shader.h"





// =================================================================================================

#ifdef D_DEBUG_SHADOW


//-----------------------------------------------------------------------------
//      Global Data



//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2,     TEXCOORD0 )
DECLARE_END


//-----------------------------------------------------------------------------
//      Functions

FRAGMENT_MAIN_COLOUR_SRT
{
   vec2 newCoords = IN(mTexCoordsVec2);
   float lFragCol = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), newCoords ).x;
   FRAGMENT_COLOUR = vec4( lFragCol, lFragCol, lFragCol, 1.0 );
}

#endif



// =================================================================================================

#ifdef D_DEBUG_FLATCOLOUR

//-----------------------------------------------------------------------------
//      Global Data

//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2,     TEXCOORD0 )
    // INPUT( vec4, mWorldPositionVec4, TEXCOORD4 )
DECLARE_END


//-----------------------------------------------------------------------------
//      Functions

FRAGMENT_MAIN_COLOUR_SRT
{
    FRAGMENT_COLOUR = vec4( 1.0, 1.0, 0.0, 1.0 );
}

#endif



// =================================================================================================
//
// D_DEBUG_QUAD_SPLIT
//
// =================================================================================================

#ifdef D_DEBUG_HEX_SPLIT

//-----------------------------------------------------------------------------
//      Global Data

//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2,     TEXCOORD0 )
DECLARE_END

//-----------------------------------------------------------------------------
//      Functions

float blendScreen( float base, float blend )
{
    return 1.0 - ( ( 1.0 - base )*( 1.0 - blend ) );
}

vec3 blendScreen( vec3 base, vec3 blend )
{
    //return vec3( blendScreen( base.r, blend.r ), blendScreen( base.g, blend.g ), blendScreen( base.b, blend.b ) );
    return 1.0 - ( ( 1.0 - base )*( 1.0 - blend ) );
}

FRAGMENT_MAIN_COLOUR_SRT
{
    //Debug

    vec3 lFragCol = vec3( 1.0, 1.0, 1.0 );
    vec2 newCoords;

    newCoords = IN(mTexCoordsVec2);
    
    // Left Quadrant
    if ( newCoords.x < 0.25 )
    {
        newCoords.x *= 4.0;

        // Top Quadrant
        if ( newCoords.y > 0.75 )
        {
            newCoords.y = (newCoords.y - 0.75) * 4.0;

            //lFragCol = GammaCorrectOutput( texture2D( lUniforms.mpCustomPerMesh->gBuffer11Map, newCoords ).xyz );
            lFragCol = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBuffer11Map ), newCoords ).xyz;
        }
        // Mid Top Quadrant
        else
        if ( newCoords.y > 0.5 )
        {
            newCoords.y = (newCoords.y - 0.5) * 4.0;
            
            lFragCol = GammaCorrectOutput( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer12Map ), newCoords ).xyz );
            lFragCol *= texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer12Map ), newCoords ).a;
            
//             float lfGlow = texture2D( lUniforms.mpCustomPerMesh->gBuffer9Map, newCoords ).a;
// 
//             if ( lfGlow > 0.68 )
//             {
//                 lFragCol = vec3(1.0,0.0,0.0);
//             }
//             else
//             if ( lfGlow > 0.35 )
//             {
//                 lFragCol = vec3(0.0,1.0,0.0);
//             }
//             else
//             if ( lfGlow > 0.0 )
//             {
//                 lFragCol = vec3(0.0,0.0,1.0);
//             }
        }
        // Mid Bottom Quadrant
        else
        if ( newCoords.y > 0.25 )
        {
            newCoords.y = (newCoords.y - 0.25) * 4.0;
            
            lFragCol = GammaCorrectOutput( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer13Map ), newCoords ).xyz );
            //lFragCol = ( texture2D( lUniforms.mpCustomPerMesh->gBuffer13Map, newCoords ).xyz );
            
//             float lfGlow = texture2D( lUniforms.mpCustomPerMesh->gBuffer10Map, newCoords ).a;
// 
//             if ( lfGlow > 0.68 )
//             {
//                 lFragCol = vec3(1.0,0.0,0.0);
//             }
//             else
//             if ( lfGlow > 0.35 )
//             {
//                 lFragCol = vec3(0.0,1.0,0.0);
//             }
//             else
//             if ( lfGlow > 0.0 )
//             {
//                 lFragCol = vec3(0.0,0.0,1.0);
//             }
        }
        // Bottom Quadrant
        else
        {
            newCoords.y *= 4.0;

            //lFragCol = GammaCorrectOutput( texture2D( lUniforms.mpCustomPerMesh->gBuffer14Map, newCoords ).xyz );
            lFragCol = ( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer14Map ), newCoords ).xyz );
        }
    }
    // Bottom Quadrant
    else
    if ( newCoords.y < 0.25 )
    {
        newCoords.y *= 4.0;

        // Left Mid Quadrant
        if ( newCoords.x < 0.5 )
        {
            newCoords.x = ( newCoords.x - 0.25 ) * 4.0;

            // Bottom Left
            if ( newCoords.x < 0.5 && newCoords.y < 0.5 )
            {
                newCoords = newCoords * 2.0;

                lFragCol = GammaCorrectOutput( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBuffer7Map ), newCoords ).xyz );
            }
            else
            // Bottom Right
            if ( newCoords.x > 0.5 && newCoords.y < 0.5 )
            {
                newCoords.x = (newCoords.x - 0.5) * 2.0;
                newCoords.y = newCoords.y * 2.0;

                lFragCol = GammaCorrectOutput( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer8Map ), newCoords ).xyz );
            }
            else
            // Top Left
            if ( newCoords.x < 0.5 && newCoords.y > 0.5 )
            {
                newCoords.y = (newCoords.y - 0.5) * 2.0;
                newCoords.x = newCoords.x * 2.0;

                lFragCol = GammaCorrectOutput( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer1Map ), newCoords ).xyz );
            }
            // Top Right
            else
            {
                newCoords.x = ( newCoords.x - 0.5 ) * 2.0;
                newCoords.y = ( newCoords.y - 0.5 ) * 2.0;

                lFragCol = GammaCorrectOutput( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer2Map ), newCoords ).xyz );
            }
        }
        // Right Mid Quadrant
        else
        if ( newCoords.x < 0.75 )
        {
            newCoords.x = ( newCoords.x - 0.5 ) * 4.0;

            // Bottom Left
            if ( newCoords.x < 0.5 && newCoords.y < 0.5 )
            {
                newCoords = newCoords * 2.0;

                lFragCol = GammaCorrectOutput( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer9Map ), newCoords ).xyz );
            }
            else
            // Bottom Right
            if ( newCoords.x > 0.5 && newCoords.y < 0.5 )
            {
                newCoords.x = (newCoords.x - 0.5) * 2.0;
                newCoords.y = newCoords.y * 2.0;

                //lFragCol = GammaCorrectOutput( texture2D( lUniforms.mpCustomPerMesh->gBuffer10Map, newCoords ).xyz );
                lFragCol = float2vec3( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer10Map ), newCoords ).a );
            }
            else
            // Top Left
            if ( newCoords.x < 0.5 && newCoords.y > 0.5 )
            {
                newCoords.y = (newCoords.y - 0.5) * 2.0;
                newCoords.x = newCoords.x * 2.0;

                lFragCol = GammaCorrectOutput( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer3Map ), newCoords ).xyz );
            }
            // Top Right
            else
            {
                newCoords.x = ( newCoords.x - 0.5 ) * 2.0;
                newCoords.y = ( newCoords.y - 0.5 ) * 2.0;

                lFragCol = GammaCorrectOutput( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBuffer4Map ), newCoords ).xyz );
            }
        }
        // Right Quadrant
        else
        {
            newCoords.x = ( newCoords.x - 0.75 ) * 4.0;

            // Bottom Left
            if ( newCoords.x < 0.5 && newCoords.y < 0.5 )
            {
                newCoords = newCoords * 2.0;

                lFragCol = GammaCorrectOutput( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer11Map ), newCoords ).xyz );
            }
            else
            // Bottom Right
            if ( newCoords.x > 0.5 && newCoords.y < 0.5 )
            {
                newCoords.x = (newCoords.x - 0.5) * 2.0;
                newCoords.y = newCoords.y * 2.0;

                lFragCol = GammaCorrectOutput( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer12Map ), newCoords ).xyz );
            }
            else
            // Top Left
            if ( newCoords.x < 0.5 && newCoords.y > 0.5 )
            {
                newCoords.y = (newCoords.y - 0.5) * 2.0;
                newCoords.x = newCoords.x * 2.0;

                lFragCol = GammaCorrectOutput( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer5Map ), newCoords ).xyz );
            }
            // Top Right
            else
            {
                newCoords.x = ( newCoords.x - 0.5 ) * 2.0;
                newCoords.y = ( newCoords.y - 0.5 ) * 2.0;

                lFragCol = GammaCorrectOutput( texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer6Map ), newCoords ).xyz );
                //lFragCol = GammaCorrectOutput( CombineBloom( DEREF_PTR( lUniforms.mpCustomPerMesh ), newCoords ) );
            }
        }
    }
    // Top Right 3/4
    else
    {
        newCoords = (newCoords - vec2(0.25,0.25)) / 0.75;

        //lFragCol = texture2D( lUniforms.mpCustomPerMesh->gBufferMap, newCoords.xy ).rgb;
        lFragCol = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), newCoords ).xyz;

        //vec4 lCloudCol = texture2D( lUniforms.mpCustomPerMesh->gBuffer14Map, newCoords );

        //lFragCol = (lFragCol * (1.0-lCloudCol.a)) + vec3(lCloudCol.a);
        //lFragCol = mix(lFragCol, vec3(lCloudCol), lCloudCol);
        //lFragCol = (lFragCol * (1.0-lCloudCol.a)) + (vec3(lCloudCol.xyz) );

        lFragCol = TonemapKodak( lFragCol ) / TonemapKodak( vec3(1.0, 1.0, 1.0) );
        lFragCol = GammaCorrectOutput( lFragCol );

        /*
        //if ( newCoords.x > 0.25 )
        {
            lFragCol = texture3DLod( lUniforms.mpCustomPerMesh->gColourLUT, lFragCol, 0.0 ).rgb;
        }
        */

        //lFragCol += GammaCorrectOutput( texture2D( lUniforms.mpCustomPerMesh->gBuffer14Map, newCoords.xy ).rgb );
        
        //lFragCol = blendScreen( lFragCol, texture2D( lUniforms.mpCustomPerMesh->gBuffer11Map, newCoords.xy ).rgb );
        /*
        lFragCol = blendScreen( lFragCol, GammaCorrectOutput( texture2D( lUniforms.mpCustomPerMesh->gBuffer11Map, newCoords.xy ).rgb ) );
        lFragCol = blendScreen( lFragCol, GammaCorrectOutput( texture2D( lUniforms.mpCustomPerMesh->gBuffer8Map, newCoords.xy ).rgb ) );
        lFragCol = blendScreen( lFragCol, GammaCorrectOutput( texture2D( lUniforms.mpCustomPerMesh->gBuffer12Map, newCoords.xy ).rgb ) * texture2D( lUniforms.mpCustomPerMesh->gBuffer12Map, newCoords.xy ).a );
        */
        //lFragCol = blendScreen( lFragCol, GammaCorrectOutput( texture2D( lUniforms.mpCustomPerMesh->gBuffer12Map, newCoords.xy ).rgb ) );

        /*
        if ( newCoords.x > 0.5 )
        {
            lFragCol = texture2D( lUniforms.mpCustomPerMesh->gBufferMap, newCoords.xy ).rgb;
        }
        */
    } 

    FRAGMENT_COLOUR = vec4( lFragCol.rgb, 1.0 );
}

#endif

#ifdef D_DEBUG_QUAD_SPLIT

//-----------------------------------------------------------------------------
//      Global Data

//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2,     TEXCOORD0 )
DECLARE_END

//-----------------------------------------------------------------------------
//      Functions


FRAGMENT_MAIN_COLOUR_SRT
{
    //Debug

    vec3 lFragCol = vec3( 1.0, 1.0, 1.0 );
    vec2 newCoords;

    // Bottom Left
    if ( IN( mTexCoordsVec2 ).x < 0.5 && IN( mTexCoordsVec2 ).y < 0.5 )
    {
        newCoords = IN(mTexCoordsVec2) * 2.0;

        lFragCol = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer2Map ), newCoords ).xyz;
    }
    else
    // Bottom Right
    if ( IN( mTexCoordsVec2 ).x > 0.5 && IN( mTexCoordsVec2 ).y < 0.5 )
    {
        newCoords.x = (IN(mTexCoordsVec2).x - 0.5) * 2.0;
        newCoords.y = IN(mTexCoordsVec2).y * 2.0;

        lFragCol = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer3Map ), newCoords ).xyz;
    }
    else
    // Top Left
    if ( IN( mTexCoordsVec2 ).x < 0.5 && IN( mTexCoordsVec2 ).y > 0.5 )
    {
        newCoords.y = (IN(mTexCoordsVec2).y - 0.5) * 2.0;
        newCoords.x = IN(mTexCoordsVec2).x * 2.0;

        lFragCol = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBufferMap ), newCoords ).xyz;
    }
    // Top Right
    else
    {
        newCoords.x = (IN(mTexCoordsVec2).x - 0.5) * 2.0;
        newCoords.y = (IN(mTexCoordsVec2).y - 0.5) * 2.0;

        //lFragCol = texture2D( lUniforms.mpCustomPerMesh->gBuffer1Map, newCoords ).xyz;
        lFragCol = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh, gBuffer1Map ), newCoords ).xyz;
    }

    FRAGMENT_COLOUR = vec4( lFragCol.rgb, 1.0 );
}

#endif

// =================================================================================================
//
// D_DEBUG_QUAD_COMBINE
//
// =================================================================================================

#ifdef D_DEBUG_QUAD_COMBINE

//-----------------------------------------------------------------------------
//      Global Data

//-----------------------------------------------------------------------------
//      Typedefs and Classes 

DECLARE_INPUT
    INPUT_SCREEN_POSITION

    INPUT( vec2, mTexCoordsVec2,     TEXCOORD0 )
DECLARE_END

//-----------------------------------------------------------------------------
//      Functions


FRAGMENT_MAIN_COLOUR_SRT
{
    //Debug

    vec3 lFragCol = vec3( 1.0, 1.0, 1.0 );

    // Bottom Left
    if ( IN( mTexCoordsVec2 ).x < 0.5 && IN( mTexCoordsVec2 ).y < 0.5 )
    {
        lFragCol = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBufferMap ), IN( mTexCoordsVec2 ).xy ).rgb;
    }
    else
    // Bottom Right
    if ( IN( mTexCoordsVec2 ).x > 0.5 && IN( mTexCoordsVec2 ).y < 0.5 )
    {
        lFragCol = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBuffer1Map ), IN( mTexCoordsVec2 ).xy ).rgb;
    }
    else
    // Top Left
    if ( IN( mTexCoordsVec2 ).x < 0.5 && IN( mTexCoordsVec2 ).y > 0.5 )
    {
        lFragCol = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBuffer2Map ), IN( mTexCoordsVec2 ).xy ).rgb;
    }
    // Top Right
    else
    {
        lFragCol = texture2D( SAMPLER_GETMAP( lUniforms.mpCustomPerMesh,gBuffer3Map ), IN( mTexCoordsVec2 ).xy ).rgb;
    }

    FRAGMENT_COLOUR = vec4( lFragCol.rgb, 1.0 );
}

#endif

