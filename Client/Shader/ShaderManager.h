#pragma once
#include "../Common/Singleton.h"
#include "../Shader/Shader.h"

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
	//std::unordered_map<std::string, Ptr<CBuffer>> _cBuffers;	// 상수 버퍼 관리
	//std::unordered_map<std::string, Ptr<SBuffer>> _sBuffers;	// 구조화 버퍼 관리.
	//ComPtr<ID3D11SampleState> _samplers[TEXURE_SAMPLE_END];
private:
	void CreateSampler();

public:
	bool Init();
	virtual void Destroy() override;
	Ptr<Shader> FindShader(const std::string& name);

	//template<typename T>
	//Ptr<T> FindCBuffer(const std::string& name)
	//{
	//	// auto it = _cBu
	//}

	template<typename T>		// 여러 종류의 쉐이더를 만들 수 있음.
	bool CreateShader(const std:: string & name)
	{
		Ptr<Shader> shader = FindShader(name);	 
		if (shader)					// 이미 있는지 확인 있다면 안 만듦.
			return false;

		shader = New<T>();			// 새로 만듦
		if (false == shader->Init())
		{
			Delete(shader);			// 초기화 실패시 삭제
			return false;

		}
		_shaders[name] = shader;	// 맵에 등록
		return true;
	}



};

