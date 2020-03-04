//
//  Geometry.h
//
//  (c) CJG 2016, cjgribel@gmail.com
//

#pragma once
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "stdafx.h"
#include <vector>
#include "vec\vec.h"
#include "vec\mat.h"
#include "ShaderBuffers.h"
#include "drawcall.h"
#include "mesh.h"

using namespace linalg;

class Geometry_t
{
protected:
	// Pointers to the current device and device context
	ID3D11Device* const			dxdevice;
	ID3D11DeviceContext* const	dxdevice_context;

	// Pointers to the class' vertex & index arrays
	ID3D11Buffer* vertex_buffer = nullptr;
	ID3D11Buffer* index_buffer	= nullptr;

	// Pointers to material buffer and texture sampler
	ID3D11SamplerState* samplerState		= nullptr;
	ID3D11SamplerState* samplerTriLinear	= nullptr;
	ID3D11Buffer*		materialBuffer		= nullptr;

public:

	Geometry_t(
		ID3D11Device* dxdevice,
		ID3D11DeviceContext* dxdevice_context);

	//
	// Map and update the matrix buffer
	//
	virtual void MapMatrixBuffers(
		ID3D11Buffer* matrix_buffer,
		mat4f ModelToWorldMatrix,
		mat4f WorldToViewMatrix,
		mat4f ProjectionMatrix);

	virtual void MapLightCameraBuffers(
		ID3D11Buffer* lightCamera_buffer,
		float4 lightPos,
		float4 cameraPos);

	virtual void MapMaterialBuffer(
		ID3D11Buffer* material_buffer,
		float4 ka,
		float4 kd,
		float4 ks,
		float4 alpha,
		int hasAmbient,
		int hasDiffuse,
		int hasSpecular,
		int hasAlpha,
		int hasNormalMap);

	virtual void DisplayModeBuffer(
		ID3D11Buffer* displayMode_buffer,
		int isPhong,
		int isNormal,
		int isTexture,
		int isAmbient,
		int isDiffuse,
		int isSpecular);

	virtual void MapTextureMaterialBuffer(
		ID3D11Buffer* texture_material_buffer,
		material_t material
	);

	//
	// Abstract render method: must be implemented by derived classes
	//
	virtual void render() const = 0;

	void compute_tangentspace(vertex_t& v0, vertex_t& v1, vertex_t& v2);

	//
	// Destructor
	//
	virtual ~Geometry_t()
	{ 
		SAFE_RELEASE(vertex_buffer);
		SAFE_RELEASE(index_buffer);
	}
};

class Quad_t : public Geometry_t
{
	// our local vertex and index arrays
	std::vector<vertex_t> vertices;
	std::vector<unsigned> indices;
	unsigned nbr_indices = 0;

public:

	Quad_t(
		ID3D11Device* dx3ddevice,
		ID3D11DeviceContext* dx3ddevice_context);

	virtual void render() const;

	~Quad_t() { }
};

class Cube_t : public Geometry_t
{
	std::vector<vertex_t> vertices;
	std::vector<unsigned> indices;
	unsigned nbr_indices = 0;

public:

	Cube_t(ID3D11Device* dx3ddevice, ID3D11DeviceContext* dx3ddevice_context);

	virtual void render() const;

	~Cube_t() {}
};

class OBJModel_t : public Geometry_t
{
	// index ranges, representing drawcalls, within an index array
	struct index_range_t
	{
		size_t start;
		size_t size;
		unsigned ofs;
		int mtl_index;
	};

	std::vector<index_range_t> index_ranges;
	std::vector<material_t> materials;

	void append_materials(const std::vector<material_t>& mtl_vec)
	{
		materials.insert(materials.end(), mtl_vec.begin(), mtl_vec.end());
	}

public:

	OBJModel_t(
		const std::string& objfile,
		ID3D11Device* dxdevice,
		ID3D11DeviceContext* dxdevice_context);

	virtual void render() const;

	~OBJModel_t();
};

#endif