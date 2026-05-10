#pragma once
#include "SceneComponent.h"
#include "Tile.h"

#include <vector>

//다른 액터들끼리-->인스턴싱 하고 싶다?
//할수는 있다~~~
//그러면 랜더를 하는걸 또 만들어야 합니다......
//지금 우리는 랜더매니저에서 레이어를 쓰고 있죠? --> 이걸 감안해서 만들어야 해용.

class TileComponent : public SceneComponent
{
public:
    TileComponent();
    virtual ~TileComponent();

protected:
    //타일들
    std::vector<Ptr<Tile>> _tiles;

    //타일 하나의 크기
    FVector2D _tileSize;

    //전체 크기
    FVector2D _mapSize;

    //타일의 총 X축 개수
    int32 _countX = -1;

    //타일의 총 Y축 개수
    int32 _countY = -1;

    //타일을 그릴때 필요한 메쉬
    Ptr<class Mesh> _tileMesh;

    //타일을 그릴때 사용할 쉐이더
    Ptr<class Shader> _tileShader;

    //타일 텍스쳐(아틀라스)
    Ptr<class Texture> _tileTexture;

    //각각의 타일이 텍스쳐에서 어디서 어디까지 그릴것인지
     std::vector<FAnimationFrame> _tileFrames;

    //타일 텍스쳐 사이즈
    FVector2D _tileTextureSize;

    //타일에서 사용할 상수버퍼
    Ptr<class TileMapCBuffer> _tileMapCBuffer;

    //타일의 외곽선
    bool _outLineRender = false;

    //타일의 외곽선 메쉬
    Ptr<class Mesh> _outLineMesh;

    //타일의 외곽선 쉐이더
    Ptr<class Shader> _outLineShader;

    //타일의 외곽선 색
    Ptr<class ColorCBuffer> _outLineCBuffer;

    //인스턴싱용 구조화버퍼
    Ptr<class TileSBuffer> _tileSBuffer;

    //인스턴싱용 쉐이더
    Ptr<class Shader> _tileInstanceShader;

    //외곽선 인스턴싱용 구조화 버퍼
    Ptr<class TileLineSBuffer> _tileLineSBuffer;

    //외곽선 인스턴싱용 쉐이더
    Ptr<class Shader> _tileLineInstanceShader;

    bool _useInstancing = true;
    bool _bTileInstRefresh = false;
    bool _bTileLineInstRefresh = false;

private:
    void RenderTile();
    void RenderOutLine();

public:
    virtual bool Init(int32 id, const std::string& name, Ptr<class Actor> owner) override;
    virtual void Tick(float deltaTime) override;
    virtual void Collision(float deltaTiem) override;
    virtual void Render(float deltaTime) override;

    virtual void Destroy() override;

public:
    void UseInstance() { _useInstancing = true; }
    void SetTileInstRefresh(bool refresh) { _bTileInstRefresh = refresh; }
    void SetTileLineInstRefresh(bool refresh) { _bTileLineInstRefresh = refresh; }

    int32 GetTileFrameCount() const { return static_cast<int32>(_tileFrames.size()); }
    const FVector2D& GetTileSize() const { return _tileSize; }
    int32 GetTileCountX() const { return _countX; }
    int32 GetTileCountY() const { return _countY; }

    void SetTexture(const std::string& name);
    void SetTexture(Ptr<class Texture> texture);
    void SetTexture(const std::string& name, const std::wstring& fileName);

    void AddTileFrame(const FVector2D& start, const FVector2D& size);
    void AddTileFrame(float startX, float startY, float sizeX, float sizeY);

    int32 GetTileIndexX(const FVector2D& pos) const;
    int32 GetTileIndexY(const FVector2D& pos) const;
    int32 GetTileIndex(const FVector2D& pos) const;

    Ptr<Tile> GetTile(const FVector2D& pos);
    Ptr<Tile> GetTile(int32 index);

    std::optional<FVector2D> GetTileWorldPos(int32 index);

    void CreateTile(int32 countX, int32 countY, const FVector2D& tileSize, int32 tileTextureFrame);
};

