#version 330 core
layout(points) in;
layout(triangle_strip, max_vertices = 16) out;

precision highp float;

out vec3 normal;

uniform sampler3D chunkData;
uniform isampler2DRect trisLUT;

uniform float isoLevel;

uniform mat4 modelViewProjectionMatrix;

uniform vec3 cubeCorners[8];

vec3 lightPos = vec3(0, 1, 0);

vec3 worldCornerPos(int i)
{
    return gl_in[0].gl_Position.xyz + cubeCorners[i];
}

float isoValue(int i)
{
    return texture(chunkData, (worldCornerPos(i)/float(16))).r;
}

int triIdx(int x, int y)
{
    return texture(trisLUT, vec2(x, y)).r;
}

vec3 interpolate(float iLevel, vec3 v0, float iso0, vec3 v1, float iso1)
{
    return mix(v0, v1, (iLevel-iso0)/(iso1-iso0));
}

void main()
{
    int cubeIdx = 0;

    //Get the iso value at each corner of the cube
    float bottomFrontLeft = isoValue(0);
    float bottomFrontRight = isoValue(1);
    float bottomBackRight = isoValue(2);
    float bottomBackLeft = isoValue(3);
    float topFrontLeft = isoValue(4);
    float topFrontRight = isoValue(5);
    float topBackRight = isoValue(6);
    float topBackLeft = isoValue(7);

    //Convert to binary values and compute lut index
    cubeIdx = int(bottomFrontLeft < isoLevel);
    cubeIdx += int(bottomFrontRight < isoLevel)*2;
    cubeIdx += int(bottomBackRight < isoLevel)*4;
    cubeIdx += int(bottomBackLeft < isoLevel)*8;
    cubeIdx += int(topFrontLeft < isoLevel)*16;
    cubeIdx += int(topFrontRight < isoLevel)*32;
    cubeIdx += int(topBackRight < isoLevel)*64;
    cubeIdx += int(topBackLeft < isoLevel)*128;

    //Return immediately no geometry is produced
    if(cubeIdx == 0 || cubeIdx == 255)
    {
     return;
    }

    vec3 vert[12];

    //Find the vertices where the surface intersects the cube
    vert[0] = interpolate(isoLevel, worldCornerPos(0), bottomFrontLeft, worldCornerPos(1), bottomFrontRight);
    vert[1] = interpolate(isoLevel, worldCornerPos(1), bottomFrontRight, worldCornerPos(2), bottomBackRight);
    vert[2] = interpolate(isoLevel, worldCornerPos(2), bottomBackRight, worldCornerPos(3), bottomBackLeft);
    vert[3] = interpolate(isoLevel, worldCornerPos(3), bottomBackLeft, worldCornerPos(0), bottomFrontLeft);
    vert[4] = interpolate(isoLevel, worldCornerPos(4), topFrontLeft, worldCornerPos(5), topFrontRight);
    vert[5] = interpolate(isoLevel, worldCornerPos(5), topFrontRight, worldCornerPos(6), topBackRight);
    vert[6] = interpolate(isoLevel, worldCornerPos(6), topBackRight, worldCornerPos(7), topBackLeft);
    vert[7] = interpolate(isoLevel, worldCornerPos(7), topBackLeft, worldCornerPos(4), topFrontLeft);
    vert[8] = interpolate(isoLevel, worldCornerPos(0), bottomFrontLeft, worldCornerPos(4), topFrontLeft);
    vert[9] = interpolate(isoLevel, worldCornerPos(1), bottomFrontRight, worldCornerPos(5), topFrontRight);
    vert[10] = interpolate(isoLevel, worldCornerPos(2), bottomBackRight, worldCornerPos(6), topBackRight);
    vert[11] = interpolate(isoLevel, worldCornerPos(3), bottomBackLeft, worldCornerPos(7), topBackLeft);

    int i = 0;

    while(true)
    {
        if(triIdx(i, cubeIdx) != -1)
        {
            vec4 pos1 = vec4(vert[triIdx(i, cubeIdx)], 1.0);
            vec4 pos2 = vec4(vert[triIdx(i+1, cubeIdx)], 1.0);
            vec4 pos3 = vec4(vert[triIdx(i+2, cubeIdx)], 1.0);

            float d = 1.0 / 16.0;
            vec3 grad;
            vec3 textureNormalizedWorldSpace = pos1.xyz / 16.0;
            grad.x = texture(chunkData, textureNormalizedWorldSpace + vec3(d, 0, 0)).r - texture(chunkData, textureNormalizedWorldSpace + vec3(-d, 0, 0)).r;
            grad.y = texture(chunkData, textureNormalizedWorldSpace + vec3(0, d, 0)).r - texture(chunkData, textureNormalizedWorldSpace + vec3(0, -d, 0)).r;
            grad.z = texture(chunkData, textureNormalizedWorldSpace + vec3(0, 0, d)).r - texture(chunkData, textureNormalizedWorldSpace + vec3(0, 0, -d)).r;
            normal = -normalize(grad);
            gl_Position = modelViewProjectionMatrix * pos1;
            EmitVertex();

            textureNormalizedWorldSpace = pos2.xyz / 16.0;
            grad.x = texture(chunkData, textureNormalizedWorldSpace + vec3(d, 0, 0)).r - texture(chunkData, textureNormalizedWorldSpace + vec3(-d, 0, 0)).r;
            grad.y = texture(chunkData, textureNormalizedWorldSpace + vec3(0, d, 0)).r - texture(chunkData, textureNormalizedWorldSpace + vec3(0, -d, 0)).r;
            grad.z = texture(chunkData, textureNormalizedWorldSpace + vec3(0, 0, d)).r - texture(chunkData, textureNormalizedWorldSpace + vec3(0, 0, -d)).r;
            normal = -normalize(grad);
            gl_Position = modelViewProjectionMatrix * pos2;
            EmitVertex();

            textureNormalizedWorldSpace = pos3.xyz / 16.0;
            grad.x = texture(chunkData, textureNormalizedWorldSpace + vec3(d, 0, 0)).r - texture(chunkData, textureNormalizedWorldSpace + vec3(-d, 0, 0)).r;
            grad.y = texture(chunkData, textureNormalizedWorldSpace + vec3(0, d, 0)).r - texture(chunkData, textureNormalizedWorldSpace + vec3(0, -d, 0)).r;
            grad.z = texture(chunkData, textureNormalizedWorldSpace + vec3(0, 0, d)).r - texture(chunkData, textureNormalizedWorldSpace + vec3(0, 0, -d)).r;
            normal = -normalize(grad);
            gl_Position = modelViewProjectionMatrix * pos3;
            EmitVertex();
            EndPrimitive();
        }
        else
        {
            break;
        }
        i+=3;

        if(i > 16)
            break;
    }
}
