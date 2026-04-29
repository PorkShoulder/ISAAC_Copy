#include "pch.h"
#include "Mesh.h"
#include "Device.h"



Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}
// 메쉬 만들기 -> 정점, 인덱스 정보를 받아서 처리함.
bool Mesh::CreateMesh(void* vertexData, int32 size, int32 count, D3D11_USAGE vertexUsage, D3D11_PRIMITIVE_TOPOLOGY primitive, void* indexData, int32 indexSize, int32 indexCount, DXGI_FORMAT fmt, D3D11_USAGE indexUsage)
{
	_vertexBuffer._size = size;
	_vertexBuffer._count = count;
	_vertexBuffer._data.resize(size * count); // 32byte 공간 확보 // resize -> 배열의 크기를 재할당 하는 함수.
	memcpy(_vertexBuffer._data.data(), vertexData, size * count); // memcpy -> 메모리의 값을 복사하는 함수. (복사받을, 복사할, 길이)
	
	if (!CreateBuffer(_vertexBuffer._buffer, D3D11_BIND_VERTEX_BUFFER, vertexData, size, count, vertexUsage)) // 정점 버퍼를 만듬.
		return false;

	_primitive = primitive;
	
	if (indexData)
	{
		FMeshSlot slot;
		slot._indexBuffer._size = indexSize;
		slot._indexBuffer._count = indexCount;
		slot._indexBuffer._data.resize(indexSize * indexCount);
		slot._indexBuffer._fmt = fmt;
		memcpy(slot._indexBuffer._data.data(), indexData, indexSize * indexCount);

		if (!CreateBuffer(slot._indexBuffer._buffer, D3D11_BIND_INDEX_BUFFER, indexData, indexSize, indexCount, indexUsage))
			return false;
		_meshSlots.push_back(slot);
	}

	return true;
}

bool Mesh::CreateBuffer(ComPtr<ID3D11Buffer>& buffer, D3D11_BIND_FLAG flag, void* data, int32 size, int32 count, D3D11_USAGE usage)
{
	// 버퍼 만들기
	//D3D11_BUFFER_DESC : 생성하려는 버퍼의 정볼르 담은 구조체.
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = size * count;	// 버퍼의 총 크기
	bufferDesc.Usage = usage;				// 버퍼의 사용방식
	bufferDesc.BindFlags = flag;			// 버퍼의 용도-> 이 버퍼는 구조화 버퍼라고 알려줌.
	bufferDesc.StructureByteStride = size;	// 구조체 하나의 크기 -> GPU가 몇 바이트씩 건너뛸지.
	
	// CPUAccessFlags : 버퍼의 CPU 접근권한
	if (usage == D3D11_USAGE_DYNAMIC)		
		bufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	else if (usage == D3D11_USAGE_STAGING)
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

	//버퍼 생성
	D3D11_SUBRESOURCE_DATA bufferData = {};
	bufferData.pSysMem = data;				//gpu버퍼를 만들때 초기에 넣을 데이터.

	if(FAILED(Device::Instance().GetDevice()->CreateBuffer(&bufferDesc, &bufferData, buffer.GetAddressOf())))
		return false;

	return false;
}

void Mesh::Render()
{
}

void Mesh::RenderInstacing(int32 count)
{
}

void Mesh::Destroy()
{
}
