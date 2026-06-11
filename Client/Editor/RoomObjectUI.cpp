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
		RenderObstacleUI();
		break;
	case eActorType::Door:
		RenderDoorUI();
		break;
	case eActorType::Item:
		RenderItemUI();
		break;
	case eActorType::Npc:
		RenderNpcUI();
		break;
	case eActorType::Monster:
		RenderMonsterUI();
		break;
	}
	
	//배치된 액터 목록
	ImGui::Separator();
	RenderActorList();
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
					scale = FVector3D(_editObstacleData.renderSize._x, _editObstacleData.renderSize._y, 1.f);

					auto obstacle = level->SpawnActor<Obstacle>(name, pos, scale, rot);
					if (obstacle)
					{
						_editObstacleData.textureName = _selectedTextureName;
						_editObstacleData.texturePath = _selectedTexturePath;
						obstacle->SetObstacleData(_editObstacleData);
						
					}

				}
				break;
				case eActorType::Item:
				{
					scale = FVector3D(_editItemData.renderSize._x, _editItemData.renderSize._y, 1.f);
					auto item = level->SpawnActor<Item>(name, pos, scale, rot);
					if (item)
					{
						_editItemData.textureName = _selectedTextureName;
						_editItemData.texturePath = _selectedTexturePath;
						item->SetItemData(_editItemData);
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
						
						// RoomManager에 등록 (전투시 몬스터 생존 체크용.) -> 이를 알아야 door활성화 여부 가능.
						Ptr<RoomManager> roomMgr = level->GetRoomManager();
						if (roomMgr)
							roomMgr->RegisterMonster(monster); 

					}
				}
				break;

				case eActorType::Npc:
				{
					scale = FVector3D(_editNpcData.renderSize._x, _editNpcData.renderSize._y, 1.f);
					auto npc = level->SpawnActor<Npc>(name, pos, scale, rot);
					if (npc)
					{
						_editNpcData.textureName = _selectedTextureName;
						_editNpcData.texturePath = _selectedTexturePath;
						_editNpcData.rewardItem.textureName = _selectedTextureName;
						_editNpcData.rewardItem.texturePath = _selectedTexturePath;
						npc->SetNpcData(_editNpcData);
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
void RoomObjectUI::RenderActorList()
{
	if (!ImGui::CollapsingHeader("Placed Actors"))
		return;

	Ptr<Level> level = GameEngine::Instance().GetWorld()->GetCurLevel();
	if (!level) return;

	std::vector<int32> deleteList;

	for (auto& [id, actor] : level->GetActors())
	{
		if (!actor || !actor->IsActive()) continue;

		eActorType type = actor->GetActorType();

		// 편집 대상만 표시 (타일, 플레이어, 카메라 제외)
		if (type != eActorType::Monster &&
			type != eActorType::Obstacle &&
			type != eActorType::Door &&
			type != eActorType::Item &&
			type != eActorType::Npc)
			continue;

		ImGui::PushID(id);

		FVector3D pos = actor->GetWorldPosition();
		ImGui::Text("[%s] %s (%.0f, %.0f)",
			actor->GetName().c_str(),
			// 타입 표시
			type == eActorType::Monster ? "Monster" :
			type == eActorType::Obstacle ? "Obstacle" :
			type == eActorType::Door ? "Door" :
			type == eActorType::Item ? "Item" :
			type == eActorType::Npc ? "Npc" : "?",
			pos._x, pos._y);

		ImGui::SameLine();
		if (ImGui::Button("Delete"))
			deleteList.push_back(id);

		ImGui::PopID();
	}

	// 삭제 처리
	for (int32 id : deleteList)
		level->RemoveActor(id);
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
	// 방이동 로직
	const char* dirNames[] = { "UP", "DOWN", "LEFT", "RIGHT" };
	int dirIdx = (int)_editDoorData.exitDir;
	if (ImGui::Combo("Exit Direction", &dirIdx, dirNames, 4))
		_editDoorData.exitDir = (eRoomDir)dirIdx;
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

void RoomObjectUI::RenderObstacleUI()
{
	ImGui::SeparatorText("Obsstacle Setting");

	//타입 선택 -> 프리셋 자동 적용
	int typeIdx = (int)_editObstacleData.obstacleType;
	if (ImGui::Combo("Obstacle Type", &typeIdx, ObstacleTypeName, (int)eObstacleType::END))
	{
		std::string texName = _editObstacleData.textureName;
		std::wstring texPath = _editObstacleData.texturePath;
		_editObstacleData = ObstaclePresets[typeIdx];
		_editObstacleData.textureName = texName;
		_editObstacleData.texturePath = texPath;
	}

	// 렌더 충돌 크기
	ImGui::DragFloat2("Render Size", &_editObstacleData.renderSize._x, 1.f, 1.f, 512.f);
	ImGui::DragFloat2("Collision Size", &_editObstacleData.collisionSize._x, 1.f, 1.f, 512.f);

	// 프리셋 능력치 읽기
	ImGui::SeparatorText("Obstacle Behavior");
	ImGui::Text("Fly: %s / Walk: &s",
		_editObstacleData.canFlyPass ? "0" : "X",
		_editObstacleData.canWalkPass ? "0" : "X");
	ImGui::Text("BulletBreak: %s / BulletPass: %s",
		_editObstacleData.bulletBreakable ? "O" : "X",
		_editObstacleData.bulletPassThrough ? "O" : "X");
	ImGui::Text("BombBreak: %s / Damage: %s (%.1f)",
		_editObstacleData.bombBreakable ? "O" : "X",
		_editObstacleData.damage ? "O" : "X",
		_editObstacleData.damageAmount);
	ImGui::Text("HP: %d", _editObstacleData.hp);

	ImGui::SeparatorText("Animation Frames");
	RenderFrameList("ANIM", _editObstacleData.animFrames);


}

void RoomObjectUI::RenderItemUI()
{

	// 타입 선택
	int typeIdx = (int)_editItemData.itemType;
	if (ImGui::Combo("ItemType", &typeIdx, ItemTypeName, (int)eItemType::END))
		_editItemData.itemType = (eItemType)typeIdx;

	// 렌더/콜리전 크기
	ImGui::DragFloat2("RenderSize", &_editItemData.renderSize._x, 1.f, 1.f, 512.f);
	ImGui::DragFloat2("CollisionSize", &_editItemData.collisionSize._x, 1.f, 1.f, 512.f);

	//아이템 자체 프레임
	RenderFrameList("ItemFrame", _editItemData.itemFrames);
	//습득시 머리 프레임
	RenderFrameList("EquipHead", _editItemData.equipHeadFrames);
	//습득시 몸통 프레임
	RenderFrameList("EquipBody", _editItemData.equipBodyFrames);
	//총알 프레임
	RenderFrameList("BulletFrame", _editItemData.bulletFrames);

	ImGui::SeparatorText("Item Stats");
	ImGui::DragFloat("BonusDamage", &_editItemData.bonusDamage, 0.1f, 0.f, 100.f);
	ImGui::DragFloat("BonusSpeed", &_editItemData.bonusSpeed, 1.f, 0.f, 500.f);
	ImGui::DragInt("BonusHp", &_editItemData.bonusHp, 1, 0, 20);
	ImGui::CheckboxFlags("Homing", &_editItemData.effect, EFFECT_HOMING);

	ImGui::DragInt("Amount", &_editItemData.consumableAmount, 1, 1, 10);

	ImGui::Text("--- Cost ---");
	ImGui::SliderInt("Coin##item", &_editItemData.cost.coin, 0, 5);
	ImGui::SliderInt("Key##item", &_editItemData.cost.key, 0, 5);
	ImGui::SliderInt("Bomb##item", &_editItemData.cost.bomb, 0, 5);
	
}

void RoomObjectUI::RenderNpcUI()
{
	// NPC 타입 선택
	int npcType = (int)_editNpcData.npcType;
	if (ImGui::Combo("NpcType", &npcType, NpcTypeName, (int)eNpcType::END))
		_editNpcData.npcType = (eNpcType)npcType;

	// 렌더/충돌 크기
	ImGui::DragFloat2("RenderSize##npc", &_editNpcData.renderSize._x, 1.f, 1.f, 512.f);
	ImGui::DragFloat2("CollisionSize##npc", &_editNpcData.collisionSize._x, 1.f, 1.f, 512.f);

	// 비용 (0~5 제한)
	ImGui::Text("--- Cost ---");
	ImGui::SliderInt("Coin Cost", &_editNpcData.cost.coin, 0, 5);
	ImGui::SliderInt("Key Cost", &_editNpcData.cost.key, 0, 5);
	ImGui::SliderInt("Bomb Cost", &_editNpcData.cost.bomb, 0, 5);

	// 보상 (npcType에 따라)
	ImGui::Text("--- Reward ---");
	if (_editNpcData.npcType == eNpcType::CONSUMABLE)
	{
		ImGui::SliderInt("Coin Reward", &_editNpcData.rewardConsumable.coin, 0, 5);
		ImGui::SliderInt("Key Reward", &_editNpcData.rewardConsumable.key, 0, 5);
		ImGui::SliderInt("Bomb Reward", &_editNpcData.rewardConsumable.bomb, 0, 5);
	}
	else if (_editNpcData.npcType == eNpcType::EFFECT_ITEM)
	{
		ImGui::Text("--- Reward Item ---");

		// 보상 아이템 타입
		int riType = (int)_editNpcData.rewardItem.itemType;
		if (ImGui::Combo("RewardItemType", &riType, ItemTypeName, (int)eItemType::END))
			_editNpcData.rewardItem.itemType = (eItemType)riType;

		// 소모품이면 수량만
		if (_editNpcData.rewardItem.itemType == eItemType::COIN ||
			_editNpcData.rewardItem.itemType == eItemType::KEY ||
			_editNpcData.rewardItem.itemType == eItemType::BOMB ||
			_editNpcData.rewardItem.itemType == eItemType::HEART)
		{
			ImGui::DragInt("Amount##npc", &_editNpcData.rewardItem.consumableAmount, 1, 1, 10);
		}
		else
		{
			// 패시브/액티브면 프레임 + 스탯
			RenderFrameList("ItemFrame##npc", _editNpcData.rewardItem.itemFrames);
			RenderFrameList("EquipHead##npc", _editNpcData.rewardItem.equipHeadFrames);
			RenderFrameList("EquipBody##npc", _editNpcData.rewardItem.equipBodyFrames);
			RenderFrameList("BulletFrame##npc", _editNpcData.rewardItem.bulletFrames);

			ImGui::DragFloat("BonusDamage##npc", &_editNpcData.rewardItem.bonusDamage, 0.1f);
			ImGui::DragFloat("BonusSpeed##npc", &_editNpcData.rewardItem.bonusSpeed, 0.1f);
			ImGui::DragInt("BonusHp##npc", &_editNpcData.rewardItem.bonusHp, 1, 0, 10);
			ImGui::CheckboxFlags("HOMING##npc", &_editNpcData.rewardItem.effect, EFFECT_HOMING);
		}

		// 보상 아이템 자체 프레임 (바닥에 뿌려질 때 보이는 모습)
		ImGui::DragFloat2("RewardRenderSize", &_editNpcData.rewardItem.renderSize._x, 1.f, 1.f, 512.f);
		
	}


	// Idle 프레임
	RenderFrameList("NpcIdle", _editNpcData.idleFrames);
}
