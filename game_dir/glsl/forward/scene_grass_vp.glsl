/*
scene_grass_vp.glsl - vertex uber shader for grass meshes
Copyright (C) 2015 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "const.h"
#include "mathlib.h"
#include "grassdata.h"

attribute vec4		attr_Position;
attribute vec4		attr_Normal;
attribute vec4		attr_LightColor;
attribute vec4		attr_LightVecs;
attribute vec4		attr_LightStyles;
attribute vec4 attr_GrassAnim;

uniform mat4		u_ModelMatrix;
uniform float		u_LightStyleValues[MAX_LIGHTSTYLES];
uniform vec3		u_GrassParams;
uniform vec3		u_ViewOrigin;
uniform float		u_RealTime;
	
varying vec2		var_TexDiffuse;
varying vec3		var_FrontLight;
varying vec3		var_BackLight;
varying vec3		var_Position;

vec3 ApplyLightStyleGrass( float scale, float packedLight, float packedDelux, vec3 N )
{
	vec3	lightmap = UnpackVector( packedLight );
	float	factor = 1.0;

#if defined( HAS_DELUXEMAP )
	vec3 deluxmap = UnpackVector( packedDelux );
	vec3 L = normalize(( deluxmap - 0.5 ) * 2.0 ); // get lightvector
	factor = max( dot( N, L ), GRASS_FLATSHADE );
#endif

#if defined( HAS_DELUXEMAP ) && defined( LIGHTVEC_DEBUG )
	return (deluxmap + N) * scale;
#else
	return lightmap * factor * scale;
#endif
}

vec3 RotateAroundAxis(vec3 value, vec3 axis, float angle)
{
    float s = sin(angle);
    float c = cos(angle);

    return value * c
         + cross(axis, value) * s
         + axis * dot(axis, value) * (1.0 - c);
}

float GetLeafWindAngle(vec3 worldPosition, float time, float phase)
{
    float spatialPhase = dot(worldPosition.xy, vec2(0.035, 0.027));

    float sharedPhase = time * 3.6 + spatialPhase;
    float gustPhase = time * 1.25 + spatialPhase * 0.45;

    float gust = 0.18 + 0.82 * pow(max(sin(gustPhase), 0.0), 2.0);

    float sway = sin(sharedPhase + phase)
               + 0.22 * sin((sharedPhase + phase) * 2.17 + 1.1);

    return sway * gust * 0.12;
}

void main( void )
{
#if defined( APPLY_FADE_DIST )
	float dist = distance( u_ViewOrigin, ( u_ModelMatrix * vec4( attr_Position.xyz, 1.0 )).xyz );
	float scale = clamp(( u_GrassParams.z - dist ) / u_GrassParams.y, 0.0, 1.0 );
#else
	float dist = GRASS_ANIM_DIST + 1.0;	// disable animation
	float scale = 1.0;			// keep constant size
#endif
	int grassVertex = int( attr_Normal.w );

int vertexID = grassVertex -
    int( 4.0 * floor( attr_Normal.w * 0.25 ));

vec3 localOffset =
    normalize( attr_Normal.xyz ) * ( attr_Position.w * scale );

vec4 worldCenter = u_ModelMatrix * vec4( attr_Position.xyz, 1.0 );
vec4 worldpos;

if( grassVertex >= 52 && grassVertex < 60 )
{
    vec3 viewDirection = u_ViewOrigin - worldCenter.xyz;

    vec3 billboardOffset = GetGrassBillboardOffset(
        grassVertex, localOffset, viewDirection );

    worldpos = vec4( worldCenter.xyz + billboardOffset, 1.0 );
}
else
{
    vec4 position = vec4( attr_Position.xyz + localOffset, 1.0 );

    if( bool( dist < GRASS_ANIM_DIST ) &&
        bool( vertexID == 1 || vertexID == 2 ) &&
        grassVertex < 60 )
    {
        position.xy += GetGrassWindOffset(worldCenter.xyz, u_RealTime);
    }

    worldpos = u_ModelMatrix * position;
}
	gl_Position = gl_ModelViewProjectionMatrix * worldpos;
	var_TexDiffuse = GetTexCoordsForVertex( int( attr_Normal.w ));
	gl_ClipVertex = gl_ModelViewMatrix * worldpos;
	var_Position = worldpos.xyz;
	
#if !defined( LIGHTING_FULLBRIGHT )
	vec3 N = GetNormalForVertex(
    grassVertex,
    attr_Normal.xyz,
    u_ViewOrigin - worldCenter.xyz );
	var_FrontLight = var_BackLight = vec3( 0.0 );

#if defined( APPLY_STYLE0 )
	scale = u_LightStyleValues[int( attr_LightStyles[0] )];
	var_FrontLight += ApplyLightStyleGrass( scale, attr_LightColor.x, attr_LightVecs.x, N );
	var_BackLight += ApplyLightStyleGrass( scale, attr_LightColor.x, attr_LightVecs.x, -N );
#endif

#if defined( APPLY_STYLE1 )
	scale = u_LightStyleValues[int( attr_LightStyles[1] )];
	var_FrontLight += ApplyLightStyleGrass( scale, attr_LightColor.y, attr_LightVecs.y, N );
	var_BackLight += ApplyLightStyleGrass( scale, attr_LightColor.y, attr_LightVecs.y, -N );
#endif

#if defined( APPLY_STYLE2 )
	scale = u_LightStyleValues[int( attr_LightStyles[2] )];
	var_FrontLight += ApplyLightStyleGrass( scale, attr_LightColor.z, attr_LightVecs.z, N );
	var_BackLight += ApplyLightStyleGrass( scale, attr_LightColor.z, attr_LightVecs.z, -N );
#endif

#if defined( APPLY_STYLE3 )
	scale = u_LightStyleValues[int( attr_LightStyles[3] )];
	var_FrontLight += ApplyLightStyleGrass( scale, attr_LightColor.w, attr_LightVecs.w, N );
	var_BackLight += ApplyLightStyleGrass( scale, attr_LightColor.w, attr_LightVecs.w, -N );
#endif
	// convert values back to normal range and clamp it
	var_FrontLight = min(( var_FrontLight * LIGHTMAP_SHIFT ), 1.0 );
	var_BackLight = min(( var_BackLight * LIGHTMAP_SHIFT ), 1.0 );

#if defined( LIGHTVEC_DEBUG )
	var_FrontLight = ( normalize( var_FrontLight ) + 1.0 ) * 0.5;
	var_BackLight = ( normalize( var_BackLight ) + 1.0 ) * 0.5;
#endif
#endif//LIGHTING_FULLBRIGHT
}