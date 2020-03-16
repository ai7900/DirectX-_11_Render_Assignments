
#pragma once
#ifndef MATRIXBUFFERS_H
#define MATRIXBUFFERS_H

#include "vec\vec.h"
#include "vec\mat.h"

using namespace linalg;

struct MatrixBuffer_t
{
	mat4f ModelToWorldMatrix;
	mat4f WorldToViewMatrix;
	mat4f ProjectionMatrix;
};

struct LightCameraBuffer_t
{
	float4 lightPos;
	float4 cameraPos;
};

struct MaterialBuffer_t
{
	float4 ka, kd, ks, alpha;
	int hasAmbient, hasDiffuse, hasSpecular, hasAlpha, hasNormalMap;
	float3 pad;
	//float3 pad;	// 4 + 12 = 16 byte
};

struct DisplayModeBuffer_t 
{
	int isPhong, isNormal, isTexture;
	int isAmbient, isDiffuse, isSpecular;
	float2 pad;
};


//Still not fully implemented in Geometry.cpp and .h
struct CubeMapBuffer_t 
{
	ID3D11Buffer* CubeMap_Buffer;
	ID3D11ShaderResourceView* map_Cube_TexSRV;
	ID3D11Resource* map_Cube_Tex;
	int isCubeMap = false;
	
	float3 pad;
};

#endif