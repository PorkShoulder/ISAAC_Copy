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
#include "../Object/TileMap.h"

#include "../Component/SpriteComponent.h"
#include "../Component/StaticMeshComponent.h"
#include "../Component/TileComponent.h" 



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
	const char* categories[] = { "Obstacle", "Door", "Item", "Npc", "Monster" };
	eActorType categoryTypes[] = {
		eActorType::Obstacle, 
		eActorType::Door, 
		eActorType::Item,
		eActorType::Npc, 
		eActorType::Monster
	};

	// 현재 선택된 타입을 찾는다.
	int categoryIndex = 0;
	for (int i = 0; i < IM_ARRAYSIZE(categories); i++)
	{
		if (categoryTypes[i] == _placeType)
		{
			categoryIndex = i;
			break;
		}
	}

	if (ImGui::Combo("Category", &categoryIndex, categories, IM_ARRAYSIZE(categories))) 
	{
		_placeType = categoryTypes[categoryIndex];
		_currentIndex = 0;
	}

	// 오브젝트 타입 선택
	switch (_placeType)
	{
	case eActorType::Obstacle:
		ImGui::Combo("Type", &_currentIndex, ObstacleTypeName, (int)eObstacleType::END);
		break;
	case eActorType::Door:
		ImGui::Combo("Type", &_currentIndex, DoorTypeName, (int)eDoorType::END);
		break;
	case eActorType::Item:
		ImGui::Combo("Type", &_currentIndex, ItemTypeName, (int)eItemType::END);
		break;
	case eActorType::Npc:
		ImGui::Combo("Type", &_currentIndex, NpcTypeName, (int)eNpcType::END);
		break;
	case eActorType::Monster:
		break;
	}
	if (_placeType == eActorType::Monster || _placeType == eActorType::Npc)
		RenderAnimationSelect();


	// 스냅 기능
	ImGui::Separator();
	RenderSnapOption();
	ImGui::Separator();
	ImGui::Text("Click on tilemap to place object");

	// 배치하기 
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		if (InputSystem::Instance().GetMouseDown(MOUSE_BUTTON_TYPE::LButton) && _targetTileMap)
		{
			// 마우스 월드 좌표
			FVector2D worldPos = InputSystem::Instance().GetMouseWorldPos();
			
			if (_snapGrid)
			{
				Ptr<TileComponent> tileComp = _targetTileMap->GetTileComponent();
				if (tileComp)
				{
					int32 tileIndex = tileComp->GetTileIndex(worldPos);
					if (tileIndex >= 0)
					{
						auto tileWorldPos = tileComp->GetTileWorldPos(tileIndex);
						if (tileWorldPos.has_value())
							worldPos = tileWorldPos.value();
					}
				}
			}

			FVector3D pos(worldPos._x, worldPos._y, 2.f);
			FVector3D scale(50.f, 50.f, 1.f);
			FRotator rot(0, 0, 0);

			//Ptr<Actor> spawned = nullptr;
			// 현재 레벨
			Ptr<Level> level = GameEngine::Instance().GetWorld()->GetCurLevel();
			if (level)
			{
				std::string name = MakeObjectName(categories[categoryIndex]);

				switch (_placeType)
				{
				case eActorType::Obstacle:
				{
					auto obstacle = level->SpawnActor<Obstacle>(name, pos, scale, rot);
					if (obstacle)
					{
						obstacle->SetTexture(_selectedTextureName);
						obstacle->SetObstacleType((eObstacleType)_currentIndex);
					}
					
				}
					break;
				case eActorType::Item:
				{
					auto item = level->SpawnActor<Item>(name, pos, scale, rot);
					if (item) 
					{
						item->SetTexture(_selectedTextureName);
						item->SetItemType((eItemType)_currentIndex);
					}
				}
				break;
				case eActorType::Door:
				{
					auto door= level->SpawnActor<Door>(name, pos, scale, rot);
					if (door)
					{
						door->SetTexture(_selectedTextureName);
						door->SetDoorType((eDoorType)_currentIndex);
					}
				}
					break;
				
				case eActorType::Monster:
				{
					auto monster = level->SpawnActor<Monster>(name, pos, scale, rot);
					if (monster) 
					{
						monster->SetMonsterType((eMonsterType)_currentIndex);

						auto sprite = monster->FindSceneComponent<SpriteComponent>("Mesh");
						if (sprite && !_animSequences.empty())
						{
							for (auto& seq : _animSequences)
							{
								std::wstring wTex(seq.textureName.begin(), seq.textureName.end());
								sprite->AddAnimSequence(seq.name, wTex, seq.frames, seq.loop);
							}
						}
					}
				}
				break;

				case eActorType::Npc:
				{
					auto npc = level->SpawnActor<Npc>(name, pos, scale, rot);
					if (npc)
					{
						npc->SetNpcType((eNpcType)_currentIndex);

						auto sprite = npc->FindSceneComponent<SpriteComponent>("Mesh");
						if (sprite && !_animSequences.empty())
						{
							for (auto& seq : _animSequences)
							{
								std::wstring wTex(seq.textureName.begin(), seq.textureName.end());
								sprite->AddAnimSequence(seq.name, wTex, seq.frames, seq.loop);
							}
						}

					}
				}
				break;

				} // switch 끝 
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

bool RoomObjectUI::IsAnimatedObject(eActorType type) const
{
	if (type == eActorType::Monster)
		return true;

	if (type == eActorType::Npc)
		return true;

	return false;
}

void RoomObjectUI::RenderAnimationSelect()
{
	//const char* MonsterAnim[] = { "MONSTER_IDLE" };
	//const char* NpcAnim[]{ "NPC_IDLE" };
	//if (_placeType == eActorType::Monster)
	//{
	//	ImGui::Combo(
	//		"Animation",
	//		&_currentAnimIndex,
	//		MonsterAnim,
	//		IM_ARRAYSIZE(MonsterAnim)
	//	);
	//}
	//else if (_placeType == eActorType::Npc)
	//{
	//	ImGui::Combo(
	//		"Animation",
	//		&_currentAnimIndex,
	//		NpcAnim,
	//		IM_ARRAYSIZE(NpcAnim)
	//	);
	//} 보류

	ImGui::SeparatorText("Animation Sequence");
	ImGui::InputText("Anim Name", _seqName, sizeof(_seqName));

	if (_selectedFrameX >= 0 && _selectedFrameY >= 0)
		ImGui::Text("Frame: %d, %d (%dx%d)", _selectedFrameX, _selectedFrameY, _frameWidth, _frameHeight);
	
	// 드래그 범위 및 단일 프레임 추가
	if (ImGui::Button("Add Frame"))
	{	
		if (_selectedFrameX >= 0 && _selectedFrameY >= 0 && strlen(_seqName) > 0) // 1. 텍스처 프레임 선택 2. 시퀀스 이름 
		{
			FAnimSequenceData* seq = nullptr;
			// 겹치는 이름이 있는지 확인. 
			for (auto& s : _animSequences)
			{
				if (s.name == _seqName)
				{
					seq = &s;
					break;
				}
			}
			// 새로운 시퀀스 만들기
			if (!seq)
			{
				FAnimSequenceData data;
				_animSequences.push_back(data); // 벡터에 빈 기본값 하나 추가
				seq = &_animSequences.back();
				seq->name = _seqName;
				seq->textureName = _selectedTextureName;
				seq->loop = _seqLoop;
			}
			int startX = _selectedFrameX;
			int startY = _selectedFrameY;
			int endX = (int)_dragEndTex._x;
			int endY = (int)_dragEndTex._y;
			if (endX <= startX) 
				endX = startX + _frameWidth;
			if (endY <= startY) 
				endY = startY + _frameHeight;
			
			for (int y = startY; y < endY; y += _frameHeight)
			{
				for (int x = startX; x < endX; x += _frameWidth)
				{
					seq->frames.push_back(FVector4D(
						(float)x, 
						(float)y, 
						(float)_frameWidth, 
						(float)_frameHeight));
				}
			}
			
		}
		_hasDragSelection = false;

	}
	ImGui::SameLine();
	ImGui::Checkbox("Loop", &_seqLoop);

	for (int i = 0; i < (int)_animSequences.size(); i++)
	{
		auto seq = _animSequences[i];
		ImGui::Text("[%s] %d frames %s", seq.name.c_str(), (int)seq.frames.size(), seq.loop ? "(loop)" : "");
		ImGui::SameLine();
		ImGui::PushID(i);
		if (ImGui::SmallButton("X"))
		{
			_animSequences.erase(_animSequences.begin() + i);
			i--;
		}
		ImGui::PopID();
	}


}
