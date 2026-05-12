#pragma once
#include "Asset.h"

//=================================
// 도형을 그리기 위한 정점/인덱스 묶음
//=================================

struct FMeshSlot
{
	// 인덱스 버퍼 + 머테리얼 조합 -> 벡터로 여러개 가능.
	FIndexBuffer _indexBuffer;
	Ptr<class Material> _material;
};


class Mesh : public Asset
{
public:
	Mesh();
	virtual ~Mesh();
protected:
	//정점 데이터 정보
	FVertexBuffer _vertexBuffer;

	//인덱스 정보
	std::vector<FMeshSlot> _meshSlots;

	//정점들이 어떻게 도형을 그릴것인지 규칙
	D3D11_PRIMITIVE_TOPOLOGY _primitive;
public:
	// 정점 데이터랑 인덱스 데이터를 받아서 GPU버퍼를 생성
	bool CreateMesh(void* vertexData, int32 size, int32 count, D3D11_USAGE vertexUsahe,
		D3D11_PRIMITIVE_TOPOLOGY primitive, void* indexData, int32 indexSize, int32 indexCount,
		DXGI_FORMAT fmt, D3D11_USAGE indexUsage = D3D11_USAGE_DEFAULT);
	// 정점, 인덱스 버퍼 둘다 이 함수 호출 공통로직 -> flag 에서 구분함.
	bool CreateBuffer(ComPtr<ID3D11Buffer>& buffer, D3D11_BIND_FLAG flag, 
		void* data, int32 size, int32 count, D3D11_USAGE usage);
	//메쉬 그리기
	void Render();
	//같은 메쉬 여러 개 한 번에 그리기
	void RenderInstancing(int32 count);
	size_t GetSlotCount() { return _meshSlots.size(); }
	const FMeshSlot* GetSlot(int idx)
	{
		if (idx < 0 || idx >= _meshSlots.size())
			return nullptr;
		return &_meshSlots[idx];
	}
	virtual void Destroy() override; // Object -> Asset(구현x) -> Mesh 로 상속됨
};

