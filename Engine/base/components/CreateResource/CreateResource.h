#pragma once
#include"DirectXCommon.h"
#include"../math/Vector.h"
#include"../math/Matrix.h"

using namespace Microsoft::WRL;

struct  VertexData
{
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

struct ParticleData {
	Matrix4x4 WVP;
	Matrix4x4 world;
	Vector4 color;
	Matrix4x4 uvTransform;
};





struct  Material
{
	Vector4 color = { 1,1,1,1 };
	Matrix4x4 uvTransform;

	float shininess = 70.0f;
	float specular_ = 1.0f;
	float metalness_ = 1.0f;
	float roughness_ = 1.0f;
	
	float scatterCoefficient = 0.5f;
	float absorptionCoefficient = 0.5f;
	float scatterDistance = 0.5f;
	float grayFactor = 0.0f;
	
	float dissolveMask = 0.0f;
	Vector2 dissolveEdgeMinMax = {};
	float pad1;
	Vector4 dissolveEdgeColor = { 1.0f,1.0f,1.0f,1.0f };
};



class CreateResources
{
public:

	/// <summary>
	/// Resource作成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="sizeInbyte"></param>
	/// <returns></returns>
	static ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInbyte);


	static void CreateBufferResource(size_t sizeInbyte, ComPtr<ID3D12Resource>& Resource);


	/// <summary>
	/// BufferView��쐬
	/// </summary>
	/// <param name="sizeInbyte"></param>
	/// <param name="Resource"></param>
	/// <returns></returns>
	static D3D12_VERTEX_BUFFER_VIEW VertexCreateBufferView(size_t sizeInbyte, ID3D12Resource* Resource, int size);
	/// <summary>
	/// index��BufferView
	/// </summary>
	/// <param name="NumVertex"></param>
	/// <returns></returns>
	static D3D12_INDEX_BUFFER_VIEW IndexCreateBufferView(size_t sizeInbyte, ComPtr<ID3D12Resource> Resource);

private:

	ID3D12Resource* resource_ = nullptr;
};
