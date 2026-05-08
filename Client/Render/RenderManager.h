#pragma once
#include "../Common/Singleton.h"

#include <vector>
#include <map>
#include <unordered_map>



struct FRenderLayer
{
	std::string _name;
	int32 _oreder;

	// 씬컴포넌트 렌더링용
	std::vector<Weak<class SceneComponent>> _renders;
	// 씬컴폰너트 탐색용
	std::map<std::pair<int32, int32>, Weak<class SceneComponent>> _finder;
	//_renders 해당 변수가 갱신되어야 하는지 여부
	bool _refreshRenders;
};



class RenderManager : public Singleton<RenderManager>
{
	SINGLETON(RenderManager)
public:
	bool Init();
	void Tick(float deltaTime);
	void Render(float deltaTime);

	void RefreshLayer();
	void SetYSort(bool sort);

	void AddRenderComponent(int32 actorID, Ptr<class SceneComponent> comp);
	void RemoveRenderComponent(const std::string& layerName, int32 actorID, int32 compID);
	FRenderLayer* FindLayer(const std::string& name);

	virtual void Destroy() override;
};

