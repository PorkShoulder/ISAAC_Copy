#include "pch.h"
#include "EditorEngine.h"
#include "RoomObjectUI.h"

#include "../Input/InputSystem.h"

#include "../World/Level.h"
#include "../World/World.h"

#include "../Core/GameEngine.h"

#include "../Object/Monster.h"
#include "../Object/Door.h"
#include "../Object/Item.h"
#include "../Object/Npc.h"
#include "../Object/Obstacle.h"

#include "../Component/SpriteComponent.h"
#include "../Component/StaticMeshComponent.h"


namespace
{
	std::vector<std::string> CategoryNames{ "Obstacle", "Door", "Item", "Monster" };

	std::vector<std::string> ObstacleType{ "Rock", "Pit", "Spike" };
	std::vector<std::string> DoorType{ "NextDoor", "LockDoor", "HiddenDoor" };
	std::vector<std::string> ItemType{ "Heart", "Key", "Bomb", "Passive", "Active" };
	std::vector<std::string> MonsterType{ "Fly", "Gaper", "Spider" };

	eActorType CategoryToType[] = {
		eActorType::Obstacle,
		eActorType::Door,
		eActorType::Item,
		eActorType::Monster
	};

	std::vector<std::string>* SubTypeTable[] = {
		&ObstacleType,
		&DoorType,
		&ItemType,
		&MonsterType
	};

	static bool ComboFromVector(const char* label, int* index, const std::vector<std::string>& items)
	{
		std::vector<const char*> ptrs;
		for (auto& s : items)
			ptrs.push_back(s.c_str());
		return ImGui::Combo(label, index, ptrs.data(), (int)ptrs.size());
	}
}

RoomObjectUI::RoomObjectUI()
{
}

RoomObjectUI::~RoomObjectUI()
{
}

bool RoomObjectUI::Init(const std::string& name)
{
	RoomEditor::Init(name);
	_placeType = eActorType::Obstacle;
	_currentIndex = 0;
	return true;
}

void RoomObjectUI::Render(float deltaTime)
{
	// 텍스처 선택 + 미리보기

	RenderTextureSelect();
	RenderTexturePreview();

	ImGui::Separator();

	// 카테고리 선택
	int categoryIndex = 0;
	for (int i = 0; i < (int)CategoryNames.size(); i++)
	{
		if (CategoryToType[i] == _placeType)
		{
			categoryIndex = i;
			break;
		}
	}

	if (ComboFromVector("Category", &categoryIndex, CategoryNames))
	{
		_placeType = CategoryToType[categoryIndex];
		_currentIndex = 0;
	}

	// 서브타입 선택
	ComboFromVector("Type", &_currentIndex, *SubTypeTable[categoryIndex]);

	ImGui::Separator();

	// 스냅 기능
	RenderSnapOption();
	ImGui::Separator();
	ImGui::Text("Click on tilemap to place object");

	// 마우스가 없을 때 클릭
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		if (InputSystem::Instance().GetMouseDown(MOUSE_BUTTON_TYPE::LButton) && _targetTileMap)
		{
			// 마우스 월드 좌표
			FVector2D worldPos = InputSystem::Instance().GetMouseWorldPos();

			// 스냅 적용
			if (_snapGrid)
			{
				worldPos._x = floor(worldPos._x / _gridSize) * _gridSize; // 소수점 아래는 버리는 함수.
				worldPos._y = floor(worldPos._y / _gridSize) * _gridSize;
			}

			Ptr<Actor> spawned = nullptr;
			// 현재 레벨
			Ptr<Level> level = GameEngine::Instance().GetWorld()->GetCurLevel();
			if (level)
			{


				FVector3D pos(worldPos._x, worldPos._y, 2.f);
				FVector3D scale(52.f, 52.f, 1.f);
				FRotator rot(0, 0, 0);
				std::string name = MakeObjectName((*SubTypeTable[categoryIndex])[_currentIndex]);

				switch (_placeType)
				{
				case eActorType::Obstacle:
					spawned = level->SpawnActor<Obstacle>(name, pos, scale, rot);
					if (spawned)
					{
						//Ptr<StaticMeshComponent> mesh = spawned->FindSceneComponent<StaticMeshComponent>("Mesh");
						//if (!mesh)
						//	mesh = Cast<SceneComponent, StaticMeshComponent>(spawned->GetRoot());
						//if (mesh)
						//{
						//	mesh->SetMesh("TexRect");
						//	mesh->AddTexture(0, _selectedTextureName, 0);
						//}

						auto mesh = spawned->FindSceneComponent<SpriteComponent>("Mesh");
						if (!mesh)
							mesh = Cast<SceneComponent, SpriteComponent>(spawned->GetRoot());
						if (mesh)
						{
							size_t retLength = 0;
							std::wstring wstr;
							wstr.resize(_selectedTextureName.size()+1);
							mbstowcs_s(&retLength, wstr.data(), wstr.size(), _selectedTextureName.c_str(), _selectedTextureName.size());
							
							mesh->SetTexture(_selectedTextureName);
							//mesh->AddAnimSequence(_selectedTextureName, {wstr}); 파일이름 여러개 

							//std::string name = std::format("{}_{}_{}_{}_{}", _selectedTextureName, _selectedFrameX, _selectedFrameY, _frameWidth, _frameHeight);
							//mesh->AddAnimSequence(name, wstr, {
							//	FVector4D(_selectedFrameX, _selectedFrameY, _frameWidth, _frameHeight)
							//	});
						}
					}
					break;
				case eActorType::Door:
					spawned = level->SpawnActor<Door>(name, pos, scale, rot);
					break;
				case eActorType::Item:
					spawned = level->SpawnActor<Item>(name, pos, scale, rot);
					break;
				case eActorType::Monster:
					spawned = level->SpawnActor<Monster>(name, pos, scale, rot);
					break;
				}

			}
			// 생성된 Actor에 텍스처 적용
			if (spawned && _selectedTexture)
			{
				Ptr<SpriteComponent> sprite = spawned->FindSceneComponent<SpriteComponent>("Mesh");
				if (sprite)
					sprite->SetRelativeScale(52.f, 52.f, 1.f);
			}

		}
	}


}

void RoomObjectUI::Destroy()
{
}

std::string RoomObjectUI::MakeObjectName(const std::string& typeName)
{
	int& counter = _objectNameCounter[typeName];
	if (counter == 0)
		counter = 1;

	char buffer[64] = {};
	snprintf(buffer, sizeof(buffer), "%s_%02d", typeName.c_str(), counter++);
	return buffer;



}
