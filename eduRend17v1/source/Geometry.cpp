#include "Geometry.h"

Geometry_t::Geometry_t(ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context) : dxdevice(dxdevice), dxdevice_context(dxdevice_context)
{
	HRESULT hr;

	// Create sampler
	D3D11_SAMPLER_DESC sd =
	{
		D3D11_FILTER_ANISOTROPIC,               //Filter
		D3D11_TEXTURE_ADDRESS_WRAP,             //AdressU
		D3D11_TEXTURE_ADDRESS_WRAP,             //AdressV
		D3D11_TEXTURE_ADDRESS_WRAP,             //AdressW
		0.0f,                                   //MipLOBBias
		4,                                      //MaxAnisotropy
		D3D11_COMPARISON_NEVER,                 //ComparisonFunc
		{1.0f, 1.0f, 1.0f, 1.0f},               //BorderColor
		-FLT_MAX,                               //MinLOD
		FLT_MAX,                                //MaxLOD


	};

	ASSERT(hr = dxdevice->CreateSamplerState(&sd, &samplerState));

	// Create Tri-linear sampler which is used for CubeMap
	//D3D11_SAMPLER_DESC tlsd =
	//{
	//	D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,		// Filter
	//	D3D11_TEXTURE_ADDRESS_WRAP,					// Address U
	//	D3D11_TEXTURE_ADDRESS_WRAP					// Address V
	//};

	//ASSERT(hr = dxdevice->CreateSamplerState(&tlsd, &samplerTriLinear));

}


void Geometry_t::MapMatrixBuffers(
	ID3D11Buffer* matrix_buffer,
	mat4f ModelToWorldMatrix,
	mat4f WorldToViewMatrix,
	mat4f ProjectionMatrix)
{
	// Map the resource buffer, obtain a pointer and then write our matrices to it
	D3D11_MAPPED_SUBRESOURCE resource;
	dxdevice_context->Map(matrix_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	MatrixBuffer_t* matrix_buffer_ = (MatrixBuffer_t*)resource.pData;
	matrix_buffer_->ModelToWorldMatrix = ModelToWorldMatrix;
	matrix_buffer_->WorldToViewMatrix = WorldToViewMatrix;
	matrix_buffer_->ProjectionMatrix = ProjectionMatrix;
	dxdevice_context->Unmap(matrix_buffer, 0);
}

void Geometry_t::MapLightCameraBuffers(
	ID3D11Buffer* lightCameraBuffer,
	float4 lightPos,
	float4 cameraPos)
{
	// Map the resource buffer, obtain a pointer and then write our matrices to it
	D3D11_MAPPED_SUBRESOURCE resource;
	dxdevice_context->Map(lightCameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	LightCameraBuffer_t* lightCamera_buffer_ = (LightCameraBuffer_t*)resource.pData;
	lightCamera_buffer_->lightPos = lightPos;
	lightCamera_buffer_->cameraPos = cameraPos;
	dxdevice_context->Unmap(lightCameraBuffer, 0);
}

void Geometry_t::MapMaterialBuffer(
	ID3D11Buffer* material_buffer,
	float4 ka,
	float4 kd,
	float4 ks,
	float4 alpha,
	int hasAmbient,
	int hasDiffuse,
	int hasSpecular,
	int hasAlpha,
	int hasNormalMap)
{
	// Map the resource buffer, obtain a pointer and then write our matrices to it
	D3D11_MAPPED_SUBRESOURCE resource;
	dxdevice_context->Map(material_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	MaterialBuffer_t* material_buffer_ = (MaterialBuffer_t*)resource.pData;
	material_buffer_->ka = ka;
	material_buffer_->kd = kd;
	material_buffer_->ks = ks;
	material_buffer_->alpha = alpha;
	material_buffer_->hasAmbient = hasAmbient;
	material_buffer_->hasDiffuse = hasDiffuse;
	material_buffer_->hasSpecular = hasSpecular;
	material_buffer_->hasAlpha = hasAlpha;
	material_buffer_->hasNormalMap = hasNormalMap;
	dxdevice_context->Unmap(material_buffer, 0);
}

void Geometry_t::DisplayModeBuffer(
	ID3D11Buffer* displayMode_buffer,
	int isPhong,
	int isNormal,
	int isTexture,
	int isAmbient,
	int isDiffuse,
	int isSpecular)
{
	D3D11_MAPPED_SUBRESOURCE resource;
	dxdevice_context->Map(displayMode_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	DisplayModeBuffer_t* displayMode_buffer_ = (DisplayModeBuffer_t*)resource.pData;
	displayMode_buffer_->isPhong = isPhong;
	displayMode_buffer_->isNormal = isNormal;
	displayMode_buffer_->isTexture = isTexture;
	displayMode_buffer_->isAmbient = isAmbient;
	displayMode_buffer_->isDiffuse = isDiffuse;
	displayMode_buffer_->isSpecular = isSpecular;
	dxdevice_context->Unmap(displayMode_buffer, 0);
}

void Geometry_t::MapTextureMaterialBuffer(
	ID3D11Buffer* texture_material_buffer,
	material_t material)
{

}

void Geometry_t::MapCubeMapBuffer(
	ID3D11Buffer* CubeMap_Buffer,
	ID3D11ShaderResourceView* map_Cube_TexSRV,
	ID3D11Resource* map_Cube_Tex,
	int isCubeMap)
{
	D3D11_MAPPED_SUBRESOURCE resource;
	dxdevice_context->Map(CubeMap_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	CubeMapBuffer_t* cube_buffer_ = (CubeMapBuffer_t*)resource.pData;
	dxdevice_context->PSSetShaderResources(5, 1, &map_Cube_TexSRV);
	cube_buffer_->isCubeMap = isCubeMap;
	dxdevice_context->Unmap(CubeMap_Buffer, 0);
}

Quad_t::Quad_t(
	ID3D11Device* dxdevice,
	ID3D11DeviceContext* dxdevice_context)
	: Geometry_t(dxdevice, dxdevice_context)
{
	//Populate the vertex array with 4 vertices
	vertex_t v0, v1, v2, v3;
	v0.Pos = { -0.5, -0.5f, 0.0f };
	v1.Pos = { 0.5, -0.5f, 0.0f };
	v2.Pos = { 0.5, 0.5f, 0.0f };
	v3.Pos = { -0.5, 0.5f, 0.0f };

	v0.Normal = v1.Normal = v2.Normal = v3.Normal = { 0, 0, 1 };

	v0.TexCoord = { 0, 0 };
	v1.TexCoord = { 0, 1 };
	v2.TexCoord = { 1, 1 };
	v3.TexCoord = { 1, 0 };

	vertices.push_back(v0);
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);

	// Populate the index array with two triangles
	// Triangle #1
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);
	// Triangle #2
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	// Vertex array descriptor
	D3D11_BUFFER_DESC vbufferDesc = { 0.0f };
	vbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbufferDesc.CPUAccessFlags = 0;
	vbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vbufferDesc.MiscFlags = 0;
	vbufferDesc.ByteWidth = vertices.size()*sizeof(vertex_t);
	// Data resource
	D3D11_SUBRESOURCE_DATA vdata;
	vdata.pSysMem = &vertices[0];
	// Create vertex buffer on device using descriptor & data
	HRESULT vhr = dxdevice->CreateBuffer(&vbufferDesc, &vdata, &vertex_buffer);

	//  Index array descriptor
	D3D11_BUFFER_DESC ibufferDesc = { 0.0f };
	ibufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibufferDesc.CPUAccessFlags = 0;
	ibufferDesc.Usage = D3D11_USAGE_DEFAULT;
	ibufferDesc.MiscFlags = 0;
	ibufferDesc.ByteWidth = indices.size()*sizeof(unsigned);
	// Data resource
	D3D11_SUBRESOURCE_DATA idata;
	idata.pSysMem = &indices[0];
	// Create index buffer on device using descriptor & data
	HRESULT ihr = dxdevice->CreateBuffer(&ibufferDesc, &idata, &index_buffer);

	// Local data is now loaded to device so it can be released
	vertices.clear();
	nbr_indices = indices.size();
	indices.clear();
}


void Quad_t::render() const
{
	//set topology
	dxdevice_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// bind our vertex buffer
	UINT32 stride = sizeof(vertex_t); //  sizeof(float) * 8;
	UINT32 offset = 0;
	dxdevice_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);

	// bind our index buffer
	dxdevice_context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);

	// make the drawcall
	dxdevice_context->DrawIndexed(nbr_indices, 0, 0);
}

Cube_t::Cube_t(ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context) : Geometry_t(dxdevice, dxdevice_context)
{

	// Populate the vertex array with 24 vertices, 3 for each vertice
	vertex_t v0_0, v1_0, v2_0, v3_0, v4_0, v5_0, v6_0, v7_0,	// 8*0 offset
			 v0_1, v1_1, v2_1, v3_1, v4_1, v5_1, v6_1, v7_1,	// 8*1 offset
			 v0_2, v1_2, v2_2, v3_2, v4_2, v5_2, v6_2, v7_2;	// 8*2 offset

	v0_0.Pos = v0_1.Pos = v0_2.Pos = { -1, 1, 1 };
	v1_0.Pos = v1_1.Pos = v1_2.Pos = { 1, 1, 1 };
	v2_0.Pos = v2_1.Pos = v2_2.Pos = { -1, -1, 1 };
	v3_0.Pos = v3_1.Pos = v3_2.Pos = { 1, -1, 1 };
	v4_0.Pos = v4_1.Pos = v4_2.Pos = { -1, 1, -1 };
	v5_0.Pos = v5_1.Pos = v5_2.Pos = { 1, 1, -1 };
	v6_0.Pos = v6_1.Pos = v6_2.Pos = { -1, -1, -1 };
	v7_0.Pos = v7_1.Pos = v7_2.Pos = { 1, -1, -1 };

	// Note to self: Normalerna är inverterade, trianglarna vill inte rendera rätt om de är rätt orienterade?
	v0_0.Normal = v1_0.Normal = v4_0.Normal = v5_0.Normal = { 0, 1, 0 };	// Normals pointing up				x.0
	v2_0.Normal = v3_0.Normal = v6_0.Normal = v7_0.Normal = { 0, -1, 0 };	// Normals pointing down			x.0
	v0_1.Normal = v2_1.Normal = v4_1.Normal = v6_1.Normal = { -1, 0, 0 };	// Normals pointing left			x.1
	v1_1.Normal = v3_1.Normal = v5_1.Normal = v7_1.Normal = { 1, 0, 0 };	// Normals pointing right			x.1
	v0_2.Normal = v1_2.Normal = v2_2.Normal = v3_2.Normal = { 0, 0 , 1 };	// Normals poiting towards you		x.2
	v4_2.Normal = v5_2.Normal = v6_2.Normal = v7_2.Normal = { 0, 0, -1 };	// Normals poiting away from you	x.2

	v1_0.TexCoord = v7_0.TexCoord = v4_1.TexCoord = v1_1.TexCoord = v5_2.TexCoord = v0_2.TexCoord = { 0, 0 };
	v0_0.TexCoord = v6_0.TexCoord = v0_1.TexCoord = v5_1.TexCoord = v4_2.TexCoord = v1_2.TexCoord = { 1, 0 };
	v5_0.TexCoord = v3_0.TexCoord = v6_1.TexCoord = v3_1.TexCoord = v7_2.TexCoord = v2_2.TexCoord = { 0, 1 };
	v4_0.TexCoord = v2_0.TexCoord = v2_1.TexCoord = v7_1.TexCoord = v6_2.TexCoord = v3_2.TexCoord = { 1, 1 };

	vertices.push_back(v0_0);
	vertices.push_back(v1_0);
	vertices.push_back(v2_0);
	vertices.push_back(v3_0);
	vertices.push_back(v4_0);
	vertices.push_back(v5_0);
	vertices.push_back(v6_0);
	vertices.push_back(v7_0);

	vertices.push_back(v0_1);
	vertices.push_back(v1_1);
	vertices.push_back(v2_1);
	vertices.push_back(v3_1);
	vertices.push_back(v4_1);
	vertices.push_back(v5_1);
	vertices.push_back(v6_1);
	vertices.push_back(v7_1);

	vertices.push_back(v0_2);
	vertices.push_back(v1_2);
	vertices.push_back(v2_2);
	vertices.push_back(v3_2);
	vertices.push_back(v4_2);
	vertices.push_back(v5_2);
	vertices.push_back(v6_2);
	vertices.push_back(v7_2);

	// Populate the index array with 12 triangles
	// Triangle #1 (Up side)
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(4);

	//Triangle #2 (Up side)
	indices.push_back(1);
	indices.push_back(5);
	indices.push_back(4);

	//Triangle #3 (Down side)
	indices.push_back(6);
	indices.push_back(7);
	indices.push_back(2);

	//Triangle #4 (Down side)
	indices.push_back(7);
	indices.push_back(3);
	indices.push_back(2);

	//Triangle (Left side)
	indices.push_back(8 * 1 + 6);
	indices.push_back(8 * 1 + 2);
	indices.push_back(8 * 1 + 4);

	//Triangle (Left side)
	indices.push_back(8 * 1 + 2);
	indices.push_back(8 * 1 + 0);
	indices.push_back(8 * 1 + 4);

	//Triangle (Right side)
	indices.push_back(8 * 1 + 3);
	indices.push_back(8 * 1 + 7);
	indices.push_back(8 * 1 + 1);

	//Triangle (Right side)
	indices.push_back(8 * 1 + 7);
	indices.push_back(8 * 1 + 5);
	indices.push_back(8 * 1 + 1);

	//Triangle # Towards You
	indices.push_back(8 * 2 + 2);
	indices.push_back(8 * 2 + 3);
	indices.push_back(8 * 2 + 0);

	//Triangle # Towards You
	indices.push_back(8 * 2 + 3);
	indices.push_back(8 * 2 + 1);
	indices.push_back(8 * 2 + 0);

	//Triangle # Away from you
	indices.push_back(8 * 2 + 7);
	indices.push_back(8 * 2 + 6);
	indices.push_back(8 * 2 + 5);

	//Triangle # Away from you
	indices.push_back(8 * 2 + 6);
	indices.push_back(8 * 2 + 4);
	indices.push_back(8 * 2 + 5);

	// Vertex array descriptor
	D3D11_BUFFER_DESC vbufferDesc = { 0.0f };
	vbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbufferDesc.CPUAccessFlags = 0;
	vbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vbufferDesc.MiscFlags = 0;
	vbufferDesc.ByteWidth = vertices.size() * sizeof(vertex_t);
	// Data resource
	D3D11_SUBRESOURCE_DATA vdata;
	vdata.pSysMem = &vertices[0];
	// Create vertex buffer on device using descriptor & data
	HRESULT vhr = dxdevice->CreateBuffer(&vbufferDesc, &vdata, &vertex_buffer);

	//  Index array descriptor
	D3D11_BUFFER_DESC ibufferDesc = { 0.0f };
	ibufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibufferDesc.CPUAccessFlags = 0;
	ibufferDesc.Usage = D3D11_USAGE_DEFAULT;
	ibufferDesc.MiscFlags = 0;
	ibufferDesc.ByteWidth = indices.size() * sizeof(unsigned);
	// Data resource
	D3D11_SUBRESOURCE_DATA idata;
	idata.pSysMem = &indices[0];
	// Create index buffer on device using descriptor & data
	HRESULT ihr = dxdevice->CreateBuffer(&ibufferDesc, &idata, &index_buffer);

	// Local data is now loaded to device so it can be released
	vertices.clear();
	nbr_indices = indices.size();
	indices.clear();
}

void Cube_t::render() const
{
	//set topology
	dxdevice_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//bind our vertex buffer
	UINT32 stride = sizeof(vertex_t); // sizeof(float) * 8;
	UINT32 offset = 0;
	dxdevice_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);

	//bind our index buffer
	dxdevice_context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);

	//make the drawcall
	dxdevice_context->DrawIndexed(nbr_indices, 0, 0);
}


OBJModel_t::OBJModel_t(
	const std::string& objfile,
	ID3D11Device* dxdevice,
	ID3D11DeviceContext* dxdevice_context)
	: Geometry_t(dxdevice, dxdevice_context)
{
	// Load the OBJ
	mesh_t* mesh = new mesh_t();
	mesh->load_obj(objfile);

	// Load and organize indices in ranges per drawcall (material)

	std::vector<unsigned> indices;
	size_t i_ofs = 0;

	for (auto& dc : mesh->drawcalls)
	{
		// Append the drawcall indices
		for (auto& tri : dc.tris)
		{
			indices.insert(indices.end(), tri.vi, tri.vi + 3);

			compute_tangentspace(mesh->vertices[tri.vi[0]], mesh->vertices[tri.vi[1]], mesh->vertices[tri.vi[2]]);
		}
			

		// Create a range
		size_t i_size = dc.tris.size() * 3;
		int mtl_index = dc.mtl_index > -1 ? dc.mtl_index : -1;
		index_ranges.push_back({ i_ofs, i_size, 0, mtl_index });

		i_ofs = indices.size();
	}

	// Vertex array descriptor
	D3D11_BUFFER_DESC vbufferDesc = { 0.0f };
	vbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbufferDesc.CPUAccessFlags = 0;
	vbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vbufferDesc.MiscFlags = 0;
	vbufferDesc.ByteWidth = mesh->vertices.size()*sizeof(vertex_t);
	// Data resource
	D3D11_SUBRESOURCE_DATA vdata;
	vdata.pSysMem = &(mesh->vertices)[0];
	// Create vertex buffer on device using descriptor & data
	HRESULT vhr = dxdevice->CreateBuffer(&vbufferDesc, &vdata, &vertex_buffer);

	// Index array descriptor
	D3D11_BUFFER_DESC ibufferDesc = { 0.0f };
	ibufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibufferDesc.CPUAccessFlags = 0;
	ibufferDesc.Usage = D3D11_USAGE_DEFAULT;
	ibufferDesc.MiscFlags = 0;
	ibufferDesc.ByteWidth = indices.size()*sizeof(unsigned);
	// Data resource
	D3D11_SUBRESOURCE_DATA idata;
	idata.pSysMem = &indices[0];
	// Create index buffer on device using descriptor & data
	HRESULT ihr = dxdevice->CreateBuffer(&ibufferDesc, &idata, &index_buffer);

	// Copy materials from mesh
	append_materials(mesh->materials);

	// Go through materials and load textures (if any) to device

	for (auto& mtl : materials)
	{
		HRESULT hr;
		std::wstring wstr; // for conversion from string to wstring

		// map_Kd (diffuse texture)
		//
		if (mtl.map_Kd.size()) {
			// Convert the file path string to wstring
			wstr = std::wstring(mtl.map_Kd.begin(), mtl.map_Kd.end());
			// Load texture to device and obtain pointers to it
			hr = DirectX::CreateWICTextureFromFile(dxdevice, dxdevice_context, wstr.c_str(), &mtl.map_Kd_Tex, &mtl.map_Kd_TexSRV);
			// Say how it went
			printf("loading texture %s - %s\n", mtl.map_Kd.c_str(), SUCCEEDED(hr) ? "OK" : "FAILED");
		}

		// map_Ks (specular texture)
		//
		if (mtl.map_Ks.size())
		{
			// Convert the file path string to wstring
			wstr = std::wstring(mtl.map_Ks.begin(), mtl.map_Ks.end());

			// Load texture to device and obtain pointers to it
			hr = DirectX::CreateWICTextureFromFile(dxdevice, dxdevice_context, wstr.c_str(), &mtl.map_Ks_Tex, &mtl.map_Ks_TexSRV);

			// Say how it wen
			printf("loading texture %s - %s\n", mtl.map_Ks.c_str(), SUCCEEDED(hr) ? "OK" : "FAILED");
		}

		// map_Ka (ambient texture)
		//
		if (mtl.map_Ka.size())
		{
			wstr = std::wstring(mtl.map_Ka.begin(), mtl.map_Ka.end());
			hr = DirectX::CreateWICTextureFromFile(dxdevice, dxdevice_context, wstr.c_str(), &mtl.map_Ka_Tex, &mtl.map_Ka_TexSRV);
			printf("loading texture %s - %s\n", mtl.map_Ka.c_str(), SUCCEEDED(hr) ? "OK" : "FAILED");
		}

		// map_d (mask texture)
		//
		if (mtl.map_d.size())
		{
			wstr = std::wstring(mtl.map_d.begin(), mtl.map_d.end());
			hr = DirectX::CreateWICTextureFromFile(dxdevice, dxdevice_context, wstr.c_str(), &mtl.map_d_Tex, &mtl.map_d_TexSRV);
			printf("loading texture %s - %s\n", mtl.map_d.c_str(), SUCCEEDED(hr) ? "OK" : "FAILED");
		}

		// map_bump (bump texture)
		if (mtl.map_bump.size())
		{
			wstr = std::wstring(mtl.map_bump.begin(), mtl.map_bump.end());
			hr = DirectX::CreateWICTextureFromFile(dxdevice, dxdevice_context, wstr.c_str(), &mtl.map_bump_Tex, &mtl.map_bump_TexSRV);
			printf("loading texture %s - %s\n", mtl.map_bump.c_str(), SUCCEEDED(hr) ? "OK" : "FAILED");
		}

		//// map_normal (normal texture)
		//if (mtl.map_normal.size())
		//{
		//	wstr = std::wstring(mtl.map_normal.begin(), mtl.map_normal.end());
		//	hr = DirectX::CreateWICTextureFromFile(dxdevice, dxdevice_context, wstr.c_str(), &mtl.map_normal_Tex, &mtl.map_normal_TexSRV);
		//	printf("loading texture %s - %s\n", mtl.map_normal.c_str(), SUCCEEDED(hr) ? "OK" : "FAILED");
		//}

		//// map_alpha (alpha texture)
		//if (mtl.map_alpha.size())
		//{
		//	wstr = std::wstring(mtl.map_alpha.begin(), mtl.map_alpha.end());
		//	hr = DirectX::CreateWICTextureFromFile(dxdevice, dxdevice_context, wstr.c_str(), &mtl.map_alpha_Tex, &mtl.map_alpha_TexSRV);
		//	printf("loading texture %s - %s\n", mtl.map_alpha.c_str(), SUCCEEDED(hr) ? "OK" : "FAILED");
		//}

		// Same thing with other textres here such as mtl.map_bump (Bump/Normal texture) etc
		//
		// ...
	}

	SAFE_DELETE(mesh);
}


void OBJModel_t::render() const
{
	// Set topology
	dxdevice_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Bind vertex buffer
	UINT32 stride = sizeof(vertex_t);
	UINT32 offset = 0;
	dxdevice_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);

	// Bind index buffer
	dxdevice_context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);

	// Iterate drawcalls
	for (auto& irange : index_ranges)
	{
		// Fetch material
		const material_t& mtl = materials[irange.mtl_index];

		// Bind textures
		dxdevice_context->PSSetShaderResources(0, 1, &mtl.map_Ka_TexSRV);		// Ambient
		dxdevice_context->PSSetShaderResources(1, 1, &mtl.map_Kd_TexSRV);		// Diffuse
		dxdevice_context->PSSetShaderResources(2, 1, &mtl.map_Ks_TexSRV);		// Specular
		dxdevice_context->PSSetShaderResources(3, 1, &mtl.map_d_TexSRV);		// mask - transparency
		dxdevice_context->PSSetShaderResources(4, 1, &mtl.map_bump_TexSRV);		// Bump
		// ...other textures here (see material_t)

		// Bind Sampler
		dxdevice_context->PSSetSamplers(0, 1, &samplerState);
		/*dxdevice_context->PSSetSamplers(1, 1, &samplerTriLinear);*/	// For Cube-map

		// Make the drawcall
		dxdevice_context->DrawIndexed(irange.size, irange.start, 0);
	}
}

void Geometry_t::compute_tangentspace(vertex_t& v0, vertex_t& v1, vertex_t& v2)
{
	// todo: compute tangent and binormal

	vec3f tangent;	// Tangent vector
	vec3f binormal;	// Binormal vector

	// Euclidean space - 3D
	vec3f D = vec3f(v1.Pos - v0.Pos);
	vec3f E = vec3f(v2.Pos - v0.Pos);

	// Texture Coordinates - 2D
	vec2f F = v1.TexCoord - v0.TexCoord;
	vec2f G = v2.TexCoord - v0.TexCoord;

	// Lengyel's Method
	// | Tx Ty Tz |				  1			| Gv	-Fv	|	| Dx Dy Dz	|
	// |		  |		=	-------------	|			| *	|			|   
	// | Bx By Bz |			Fu*Gv - Fv*Gu	| -Gu	Fu	|	| Ex Ey Ez	|

	float constant = (1 / ((F.x * G.y) - (F.y * G.x)));

	tangent.x = constant * ((G.y * D.x) - (F.y * E.x));
	tangent.y = constant * ((G.y * D.y) - (F.y * E.y));
	tangent.z = constant * ((G.y * D.z) - (F.y * E.z));

	binormal.x = constant * (((-G.x) * D.x) + (F.x * E.x));
	binormal.y = constant * (((-G.x) * D.y) + (F.x * E.y));
	binormal.z = constant * (((-G.x) * D.z) + (F.x * E.z));

	tangent.normalize();
	binormal.normalize();

	v0.Tangent = v1.Tangent = v2.Tangent = tangent;
	v0.Binormal = v1.Binormal = v2.Binormal = binormal;
}

OBJModel_t::~OBJModel_t()
{
	for (auto& mtl : materials)
	{
		SAFE_RELEASE(mtl.map_Kd_Tex);
		SAFE_RELEASE(mtl.map_Kd_TexSRV);
		SAFE_RELEASE(mtl.map_Ks_Tex);
		SAFE_RELEASE(mtl.map_Ks_TexSRV);
		SAFE_RELEASE(mtl.map_Ka_Tex);
		SAFE_RELEASE(mtl.map_Ka_TexSRV);
		SAFE_RELEASE(mtl.map_alpha_Tex);
		SAFE_RELEASE(mtl.map_alpha_TexSRV);
		SAFE_RELEASE(mtl.map_normal_Tex);
		SAFE_RELEASE(mtl.map_normal_TexSRV);
		SAFE_RELEASE(mtl.map_d_Tex);
		SAFE_RELEASE(mtl.map_d_TexSRV);
		SAFE_RELEASE(mtl.map_bump_Tex);
		SAFE_RELEASE(mtl.map_bump_TexSRV);
	}
	SAFE_RELEASE(samplerState);
}