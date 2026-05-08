#pragma once
#include "MeshComponent.h"

//===================================
// 실제로 메쉬를 화면에 그리는 컴포넌트 
// 메쉬+쉐이더+재질을 조합해서 렌더링
//===================================



class StaticMeshComponent : public MeshComponent
{
public:
    StaticMeshComponent();
    virtual ~StaticMeshComponent();

protected:
    Weak<class Mesh> _mesh;     // 그릴 메쉬 MeshManager에서 찾아와서 연결
    Weak<class Shader> _shader; // 사용할 세이더 

public:
    void SetMesh(const std::string& name); //MeshManager에서 이름으로 메쉬를 찾아 저장
    void SetShader(const std::string& name); //ShaderManager에서 이름으로 쉐이더를 찾아 저장
    void SetMaterial(int32 slotIndex, const std::string& name); //MaterialManager에서 이름으로 재질을 찾아 저장. 
    void AddTexture(int32 slotIndex, const std::string& name, 
                    int32 registerNum, 
                    int32 shaderBufferType = SHADER_TYPE::PIXEL, 
                    int32 textureIndex = 0);

    // 특정 슬롯  색상/투명도 변경.
    void SetBaseColor(int32 slotIndex, float r, float g, float b, float a); 
    void SetOpacity(int slotIndex, float opacity);

public:
    virtual bool Init(int32 id, const std::string& name, Ptr<class Actor> owner) override;
    virtual void Tick(float deltaTime) override;
    virtual void Collision(float deltaTiem) override;
    virtual void Render(float deltaTime) override;

    virtual void Destroy() override;

};

