#include "pch.h"
#include "Mesh.h"
#include "Device.h"
#include "Material.h"
#include "AssetManager.h"




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

		slot._material = MATERIAL_MANAGER->CreateMaterialInstance("DefaultMaterial");

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

	return true;
}

void Mesh::Render()
{
	uint32 stride = _vertexBuffer._size;	// 정점 하나의 크기
	uint32 offset = 0;						// 버퍼의 어디부터 읽을 지 0은 처음부터 읽겠다는 뜻임.

	// GPu에게 정점을 어떤 도형 규칙으로 해석할지.
	Device::Instance().GetContext()->IASetPrimitiveTopology(_primitive);
	// GPU에게 정점 버퍼 세팅
	Device::Instance().GetContext()->IASetVertexBuffers(0, 1, _vertexBuffer._buffer.GetAddressOf(), &stride, &offset);

	// 실제 그리는 명령
	size_t size = _meshSlots.size();	// 인덱스 버퍼와 머테리얼을 묶음
	if (size > 0)						// 인덱스 버퍼가 있는 경우
	{
		for (size_t i = 0; i < size; ++i)
		{
			Device::Instance().GetContext()->IASetIndexBuffer(_meshSlots[i]._indexBuffer._buffer.Get(), _meshSlots[i]._indexBuffer._fmt, 0);
			Device::Instance().GetContext()->DrawIndexed(_meshSlots[i]._indexBuffer._count, 0, 0);
		}
	}
	else
	{
		Device::Instance().GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		Device::Instance().GetContext()->Draw(_vertexBuffer._count, 0);
	

	}

}

void Mesh::RenderInstancing(int32 count) // _meshSlots[0]._indexBuffer._count 개의 인덱스를 사용하는 메쉬 1개를 count개 만큼 반복해서 그려라.
{
	uint32 stride = _vertexBuffer._size;	// 정점 하나의 크기
	uint32 offset = 0;						// 시작 오프셋 설정.
	// IA : Input Assembler -> 조립한다.
	// 어떤 도형 규칙으로 해석할지
	Device::Instance().GetContext()->IASetPrimitiveTopology(_primitive); 
	// 정점 버퍼 조립 단계에서 바인딩.
	Device::Instance().GetContext()->IASetVertexBuffers(0, 1, _vertexBuffer._buffer.GetAddressOf(), &stride, &offset);
	// 인덱스 버퍼 조립 단계에서 바인딩.
	Device::Instance().GetContext()->IASetIndexBuffer(_meshSlots[0]._indexBuffer._buffer.Get(), _meshSlots[0]._indexBuffer._fmt, 0);
	// count 개 만큼 그리기.
	Device::Instance().GetContext()->DrawIndexedInstanced(_meshSlots[0]._indexBuffer._count, count, 0, 0, 0);
}

void Mesh::Destroy()
{
}
