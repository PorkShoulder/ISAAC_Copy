#include "pch.h"
#include "EditorEngine.h"
#include "RoomObjectUI.h"

#include "../Input/InputSystem.h"

#include "../World/Level.h"
#include "../World/World.h"

#include "../World/RoomManager.h"
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
	const char* categories[] = { "Tile", "Obstacle", "Door", "Item", "Npc", "Monster" };
	eActorType categoryTypes[] = {
		eActorType::Tile,
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
	
	// 초기 리소스 프레임 나누기 값 지정.
	if (ImGui::Combo("Category", &categoryIndex, categories, IM_ARRAYSIZE(categories)))
	{
		_placeType = categoryTypes[categoryIndex];
		_currentIndex = 0;

		switch (_placeType)
		{
		case eActorType::Tile:
			_frameWidth = 52;
			_frameHeight = 52;
			break;

		case eActorType::Obstacle:
			_frameWidth = 52;
			_frameHeight = 52;
			break;

		case eActorType::Door:
			_frameWidth = 64;
			_frameHeight = 48;
			break;

		case eActorType::Item:
			_frameWidth = 52;
			_frameHeight = 52;
			break;
		case eActorType::Npc:
			_frameWidth = 52;
			_frameHeight = 52;
			break;
		case eActorType::Monster:
			_frameWidth = 52;
			_frameHeight = 52;
			break;
		}
		

	}

	// 오브젝트 타입 선택
	switch (_placeType)
	{
	case eActorType::Obstacle:
		ImGui::Combo("Type", &_currentIndex, ObstacleTypeName, (int)eObstacleType::END);
		break;
	case eActorType::Door:
		RenderDoorUI();
		break;
	case eActorType::Item:
		ImGui::Combo("Type", &_currentIndex, ItemTypeName, (int)eItemType::END);
		break;
	case eActorType::Npc:
		ImGui::Combo("Type", &_currentIndex, NpcTypeName, (int)eNpcType::END);
		break;
	case eActorType::Monster:
		RenderMonsterUI();
		break;
	}
	if (_placeType == eActorType::Npc)
	{
		RenderAnimationSelect();
	}


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
			FVector3D scale(52.f, 52.f, 1.f); // 타일 크기입니다.
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
					
					scale = FVector3D(_editDoorData.renderSize._x, _editDoorData.renderSize._y, 1.f);
					auto door = level->SpawnActor<Door>(name, pos, scale, rot);
					if (door)
					{
						_editDoorData.textureName = _selectedTextureName;
						_editDoorData.texturePath = _selectedTexturePath;
						door->SetDoorData(_editDoorData);

						Ptr<RoomManager> roomMgr = level->GetRoomManager();
						if (roomMgr)
							roomMgr->RegisterDoor(door);
					}
				}
				break;

				case eActorType::Monster:
				{
					scale = FVector3D(_editMonsterData.renderSize._x, _editMonsterData.renderSize._y, 1.f);
					
					// 몬스터 액터 생성.
					auto monster = level->SpawnActor<Monster>(name, pos, scale, rot);
					if (monster)
					{
						// 현재 에디터에서 선택한 텍스처 정보를 몬스터 데이터에 저장
						_editMonsterData.textureName = _selectedTextureName;
						_editMonsterData.texturePath = _selectedTexturePath;
						// 프리셋 능력치 + 텍스처+크기 정보를 몬스터에 전달
						monster->SetMonsterData(_editMonsterData);
						
						//// 애니메이션 등록.
						//auto sprite = monster->FindSceneComponent<SpriteComponent>("Mesh");
						//if (sprite && !_animSequences.empty())
						//{
						//	for (auto& seq : _animSequences)
						//	{
						//		sprite->AddAnimSequence(seq.name, seq.texturePath, seq.frames, seq.loop);
						//	}
						//}
						
						// RoomManager에 등록 (전투시 몬스터 생존 체크용.) -> 이를 알아야 door활성화 여부 가능.
						Ptr<RoomManager> roomMgr = level->GetRoomManager();
						if (roomMgr)
							roomMgr->RegisterMonster(monster); 

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

void RoomObjectUI::RenderAnimationSelect() // 애니메이션 프레임 등록
{
	

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
				seq->texturePath = _selectedTexturePath;
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
void RoomObjectUI::RenderDoorUI()
{
	ImGui::SeparatorText("Door Function");
	ImGui::DragFloat2("Render Size", &_editDoorData.renderSize._x, 1.f, 1.f, 512.f);
	ImGui::DragFloat2("Collision Size", &_editDoorData.collisionSize._x, 1.f, 1.f, 512.f);

	ImGui::SeparatorText("Door Image Parts");

	if (_selectedFrameX >= 0 && _selectedFrameY >= 0)
		ImGui::Text("Selected: %d,%d (%dx%d)",
			_selectedFrameX, _selectedFrameY, _frameWidth, _frameHeight);

	// Frame
	if (ImGui::Button("Set Frame"))
	{
		_editDoorData.frame = FVector4D(
			(float)_selectedFrameX, (float)_selectedFrameY,
			(float)_frameWidth, (float)_frameHeight);

		_editDoorData.left = _editDoorData.frame;
		_editDoorData.right = _editDoorData.frame;
		_editDoorData.openImage = _editDoorData.frame;
	}
	ImGui::SameLine();
	ImGui::Text("(%d,%d %dx%d)", (int)_editDoorData.frame._x, (int)_editDoorData.frame._y,
		(int)_editDoorData.frame._z, (int)_editDoorData.frame._w);

	// Left
	if (ImGui::Button("Set Left"))
	{
		_editDoorData.left = FVector4D(
			(float)_selectedFrameX, (float)_selectedFrameY,
			(float)_frameWidth, (float)_frameHeight);
	}
	ImGui::SameLine();
	ImGui::Text("(%d,%d %dx%d)", (int)_editDoorData.left._x, (int)_editDoorData.left._y,
		(int)_editDoorData.left._z, (int)_editDoorData.left._w);

	// Right
	if (ImGui::Button("Set Right"))
	{
		_editDoorData.right = FVector4D(
			(float)_selectedFrameX, (float)_selectedFrameY,
			(float)_frameWidth, (float)_frameHeight);
	}
	ImGui::SameLine();
	ImGui::Text("(%d,%d %dx%d)", (int)_editDoorData.right._x, (int)_editDoorData.right._y,
		(int)_editDoorData.right._z, (int)_editDoorData.right._w);

	// Open Image
	if (ImGui::Button("Set Open Image"))
	{
		_editDoorData.openImage = FVector4D(
			(float)_selectedFrameX, (float)_selectedFrameY,
			(float)_frameWidth, (float)_frameHeight);
	}
	ImGui::SameLine();
	ImGui::Text("(%d,%d %dx%d)", (int)_editDoorData.openImage._x, (int)_editDoorData.openImage._y,
		(int)_editDoorData.openImage._z, (int)_editDoorData.openImage._w);

	ImGui::SeparatorText("Door Function");

	int doorIdx = (int)_editDoorData.doorType;
	if (ImGui::Combo("Door Type", &doorIdx, DoorTypeName, (int)eDoorType::END))
		_editDoorData.doorType = (eDoorType)doorIdx;

	ImGui::Checkbox("Open (Initial)", &_editDoorData.bOpen);
	ImGui::Checkbox("Battle Control", &_editDoorData.bBattle);
}

void RoomObjectUI::RenderMonsterUI()
{
	ImGui::SeparatorText("Monster Setting");

	// 몬스터 타입 선택 -> 프리셋 능력치 자동 적용
	int typeIdx = (int)_editMonsterData.monsterType;
	if (ImGui::Combo("Monster Type", &typeIdx, MonsterTypeName, (int)eMonsterType::End))
	{
		std::string texName = _editMonsterData.textureName;
		std::wstring texPath = _editMonsterData.texturePath;
		_editMonsterData = MonsterInfo[typeIdx];
		_editMonsterData.textureName = texName;
		_editMonsterData.texturePath = texPath;
	}
	// 렌더/충돌 크기 - 에디터에서 직접 조절.
	ImGui::DragFloat2("Render Size", &_editMonsterData.renderSize._x, 1.f, 1.f, 512.f);
	ImGui::DragFloat2("Collision Size", &_editMonsterData.collisionSize._x, 1.f, 1.f, 512.f);

	//프리셋 능력치 읽기
	ImGui::SeparatorText("Monster stats");
	ImGui::Text("Speed: %.0f / Charge: %.0f", _editMonsterData.moveSpeed, _editMonsterData.chargeSpeed);
	ImGui::Text("Range: %.0f / ATK: %.1f / HP: %.0f", _editMonsterData.detectRange, _editMonsterData.attackPower, _editMonsterData.hp);

	ImGui::SeparatorText("Animation Frames");
	RenderFrameList("IDLE", _editMonsterData.idleFrames);
	RenderFrameList("MOVE_FRONT", _editMonsterData.moveFrontFrames);
	RenderFrameList("MOVE_BACK", _editMonsterData.moveBackFrames);
	RenderFrameList("MOVE_SIDE", _editMonsterData.moveSideFrames);
	RenderFrameList("DEATH", _editMonsterData.deathFrames);
}

void RoomObjectUI::RenderFrameList(const char* label, std::vector<FVector4D>& frames)
{
	if (ImGui::TreeNode(label))
	{
		for (int i = 0; i < (int)frames.size(); ++i)
		{
			ImGui::PushID(i);
			ImGui::DragFloat4("Frame", &frames[i]._x, 1.f, 0.f, 1024.f);
			ImGui::SameLine();
			if (ImGui::Button("X")) { frames.erase(frames.begin() + i); --i; }
			ImGui::PopID();
		}

		if (ImGui::Button("+ Add Frame"))
		{
			float sx = (_selectedFrameX >= 0) ? (float)_selectedFrameX : 0.f;
			float sy = (_selectedFrameY >= 0) ? (float)_selectedFrameY : 0.f;
			frames.push_back(FVector4D(sx, sy, (float)_frameWidth, (float)_frameHeight));
		}
		ImGui::TreePop();
	}
}
