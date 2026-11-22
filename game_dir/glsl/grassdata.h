/*
grassdata.h - implicit grass data
Copyright (C) 2019 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#ifndef GRASSDATA_H
#define GRASSDATA_H

vec2 GetTexCoordsForVertex( int vertexNum )
{
	if (vertexNum >= 16)
	{
		int corner = vertexNum - 4 * int(floor(float(vertexNum) * 0.25));

		if (corner == 0)
			return vec2(0.0, 1.0);
		if (corner == 1)
			return vec2(0.0, 0.0);
		if (corner == 2)
			return vec2(1.0, 0.0);

		return vec2(1.0, 1.0);
	}
	if( vertexNum == 0 || vertexNum == 4 || vertexNum == 8 || vertexNum == 12 )
		return vec2( 0.0, 1.0 );
	if( vertexNum == 1 || vertexNum == 5 || vertexNum == 9 || vertexNum == 13 )
		return vec2( 0.0, 0.0 );
	if( vertexNum == 2 || vertexNum == 6 || vertexNum == 10 || vertexNum == 14 )
		return vec2( 1.0, 0.0 );
	return vec2( 1.0, 1.0 );
}

vec3 GetGrassBillboardOffset(
    int vertexNum,
    vec3 localOffset,
    vec3 viewDirection)
{
    int corner = vertexNum -
        4 * int(floor(float(vertexNum) * 0.25));

    float side = (corner < 2) ? -1.0 : 1.0;

    float width = length(localOffset.xy);
    float height = localOffset.z;

    // Y=1: vertical billboard, rotation only around world Z.
    if (vertexNum >= 52 && vertexNum < 56)
    {
        vec3 facing = vec3(viewDirection.xy, 0.0);

        if (dot(facing, facing) < 0.000001)
            facing = vec3(0.0, 1.0, 0.0);

        facing = normalize(facing);

        vec3 right = vec3(-facing.y, facing.x, 0.0);

        return right * (side * width) + vec3(0.0, 0.0, height);
    }

    // Y=2: full billboard.
    if (vertexNum >= 56 && vertexNum < 60)
    {
        vec3 facing = viewDirection;

        if (dot(facing, facing) < 0.000001)
            facing = vec3(0.0, 1.0, 0.0);

        facing = normalize(facing);

        vec3 right = cross(vec3(0.0, 0.0, 1.0), facing);

        if (dot(right, right) < 0.000001)
            right = vec3(1.0, 0.0, 0.0);
        else
            right = normalize(right);

        vec3 up = normalize(cross(facing, right));

        if (up.z < 0.0)
        {
            right = -right;
            up = -up;
        }

        return right * (side * width) + up * height;
    }

    return localOffset;
}

vec3 GetNormalForVertex(
    int vertexNum,
    vec3 vertexDirection,
    vec3 viewDirection)
{
    // Y=3: the CPU supplies the actual surface normal.
    if (vertexNum >= 60 && vertexNum < 64)
        return normalize(vertexDirection);

    // Y=2: normal points towards the viewer.
    if (vertexNum >= 56 && vertexNum < 60)
        return normalize(viewDirection);

    // Y=1: normal points towards the viewer horizontally.
    if (vertexNum >= 52 && vertexNum < 56)
    {
        vec3 facing = vec3(viewDirection.xy, 0.0);

        if (dot(facing, facing) < 0.000001)
            facing = vec3(0.0, 1.0, 0.0);

        return normalize(facing);
    }

    // X=1..3: vertical planes with individual orientation.
    if (vertexNum >= 16 && vertexNum < 52)
    {
        int corner = vertexNum -
            4 * int(floor(float(vertexNum) * 0.25));

        float side = (corner < 2) ? -1.0 : 1.0;

        vec2 axis = vertexDirection.xy / side;

        return normalize(vec3(-axis.y, axis.x, 0.0));
    }

    // Original vanilla normals for X=4.
    if (vertexNum == 0 || vertexNum == 1 || vertexNum == 2 || vertexNum == 3)
        return vec3(0.0, 1.0, 0.0);

    if (vertexNum == 4 || vertexNum == 5 || vertexNum == 6 || vertexNum == 7)
        return vec3(-1.0, 0.0, 0.0);

    if (vertexNum == 8 || vertexNum == 9 || vertexNum == 10 || vertexNum == 11)
        return vec3(-0.707107, 0.707107, 0.0);

    return vec3(0.707107, 0.707107, 0.0);
}

#endif//GRASSDATA_H