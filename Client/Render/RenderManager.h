#pragma once
#include "../Common/Singleton.h"

#include <vector>
#include <map>
#include <unordered_map>


//랜더레이어
//각 랜더링 해야 되는 씬컴포넌트들을 계층 별로 모아서 출력을 할꺼에요.
struct FRenderLayer
{
    std::string _name;
    int32 _order;

    //씬컴포넌트 랜더링 용
    std::vector<Weak<class SceneComponent>> _renders;

    //씬컴포넌트 탐색용
    std::map<std::pair<int32, int32>, Weak<class SceneComponent>> _finder;

    //_renders 해당 변수가 갱신되어야 하는지 여부
    bool _refreshRenders;
};


class RenderManager : public Singleton<RenderManager>
{
    SINGLETON(RenderManager)

private:
    std::map<int32, FRenderLayer> _layers;
    std::unordered_map<std::string, int32> _layerFinder;
    std::vector<std::tuple<std::string, int32, int32>> _removeRenders;
    bool _refreshLayer;
    bool _sortY = true;

    Ptr<class RenderStateManager> _renderStateManager;
    Ptr<class BlendState> _alphaBlend;
    Ptr<class DepthStencilState> _depthStencilState;

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

    Ptr<class BlendState> GetAlphaBlend();
    Ptr<class DepthStencilState> GetDepthStencilState();



private:
    void CreateRenderLayer(const std::string& name, int32 order);
};

