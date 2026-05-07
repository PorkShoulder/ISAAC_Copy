#pragma once
#include "../Common/Singleton.h"
#include "Shader.h"
#include "SBuffer.h"
#include "CBuffer.h"

#include <unordered_map>

//===========================
// 버퍼를 이름으로 관리함.
//===========================


class ShaderManager : public Singleton<ShaderManager>
{
	SINGLETON(ShaderManager)
protected:
	// 
	std::unordered_map<std::string, Ptr<Shader>> _shaders;		// 쉐이더들 관리
	std::unordered_map<std::string, Ptr<CBuffer>> _cBuffers;	// 상수 버퍼 관리
	std::unordered_map<std::string, Ptr<SBuffer>> _sBuffers;	// 구조화 버퍼 관리.
	ComPtr<ID3D11SamplerState> _samplers[TEXURE_SAMPLE_END];
private:
	void CreateSampler();

public:
    bool Init();
    virtual void Destroy() override;
    Ptr<Shader> FindShader(const std::string& name);

    void SetSample(eTextureSampleType type);

    template<typename T>
    Ptr<T> FindCBuffer(const std::string& name)
    {
        auto it = _cBuffers.find(name);
        if (_cBuffers.end() == it)
            return nullptr;

        return Cast<CBuffer, T>(it->second);
    }

    template<typename T>
    Ptr<T> FindSBuffer(const std::string& name)
    {
        auto it = _sBuffers.find(name);
        if (_sBuffers.end() == it)
            return nullptr;

        return Cast<SBuffer, T>(it->second);
    }

private:
    template<typename T>
    bool CreateShader(const std::string& name)
    {
        Ptr<Shader> shader = FindShader(name);
        if (shader)
            return false;

        shader = New<T>();
        if (false == shader->Init())
        {
            Delete(shader);
            return false;
        }

        _shaders[name] = shader;

        return true;
    }

    template<typename T>
    bool CreateCBuffer(const std::string& name, int size, int regi, int type)
    {
        Ptr<T> cbuffer = FindCBuffer<T>(name);
        if (cbuffer)
            return false;

        cbuffer = New<T>();
        if (false == cbuffer->Create(size, regi, type))
        {
            Delete(cbuffer);
            return false;
        }

        _cBuffers[name] = cbuffer;

        return true;
    }

    template<typename T>
    bool CreateSBuffer(const std::string& name, int32 size, int32 eleCnt, int32 regi, int32 type)
    {
        Ptr<T> sBuffer = FindSBuffer<T>(name);
        if (sBuffer)
            return false;

        sBuffer = New<T>();
        if (false == sBuffer->Create(size, eleCnt, regi, type))
        {
            DESTROY(sBuffer);
            return false;
        }

        _sBuffers[name] = sBuffer;

        return true;
    }

};

#define SHADER_MANAGER ShaderManager::Instance()
#define FIND_SHADER(x) ShaderManager::Instance().FindShader(x)
#define FIND_CBUFFER(x, T) ShaderManager::Instance().FindCBuffer<T>(x)
#define FIND_SBUFFER(x, T) ShaderManager::Instance().FindSBuffer<T>(x)