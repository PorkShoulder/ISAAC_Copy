# ISAAC_Copy 프로젝트 — 남은 작업 정리

> 다음 세션이 이 문서를 읽고 이어서 작업하기 위한 현황/TODO 문서.
> 코드 직접 스캔 기준 (2026-06-06), 전투 시스템 진행 추가 (2026-06-07).

---

## 0. 다음 세션 이어서 할 것 (전투 시스템 — 작업 중) ★최우선

### 완료
- **Pawn에 HP/사망** — `_hp/_maxHp`, `TakeDamage(int)`, 가상 `OnDeath()`(기본=액터 제거), `IsDead/GetHp`
- **Bullet 데미지 + 피격** — `_damage`, `OnHit`(맞은 Pawn에 TakeDamage), 충돌 프로파일 라우팅(PlayerBullet↔Monster, MonsterBullet↔Player) 정상. 몬스터 피격→제거 동작 확인됨
- **Bullet 애니** — `IsaacAnimationLoader::Register`에 "Tears" 등록 (`ISAAC_Tears\bulletatlas.png`, 64px 셀, 시작 (320,256), 4x3=12프레임). Init에서 `AddAnimSequence("Tears")`, 비행 중 `SetPlay(false)`(첫 프레임 정지), 충돌 시 `BulletImpact`에서 `SetPlay(true)`
- **Bullet 크기** — `_mesh->SetRelativeScale(64,64,1)`
- **타일 BLOCK 감지** — `Bullet::Tick`에서 `level->GetTileMap()->GetTileComponent()->IsBlocked(pos)` → `BulletImpact`. 플레이어/총알 둘 다 BLOCK에서 막힘 확인
- **RoomManager** — `ActivateStartRoom`에서 `_level->SetTileMap(_currentRoom->tileMap)` 추가(현재 방 타일맵 등록)

### 진행 중 버그 (여기서부터 시작)
1. **★ 충돌체 디버그 박스 잔존 (몬스터+총알 공통)**
   - 증상: 액터는 제거됨(OutLiner에서 사라짐, 애니 끝남)인데 **충돌체 박스만 화면에 남음**
   - 원인 추정: 박스는 `RenderManager`에 등록돼 그려짐(collider `_isRender=true`). `SceneComponent::Destroy`에서 `Ptr<Actor> owner = Lock<Actor>(_owner); if(!owner) return;` — **owner Lock 실패 시 `RemoveRenderComponent` 스킵** → 박스 잔존. (`CollisionComponent::Destroy`의 `RemoveCollision`도 같은 식으로 `_level` null이면 스킵)
   - 다음 할 일: `SceneComponent::Destroy`의 `if(!owner)` 자리에 로그 넣어 **owner가 정말 null인지 확인**. null이면 → 컴포넌트에 `int32 _ownerActorID`를 Init 때 저장해두고, Destroy에서 owner Lock 없이 그 ID로 `RemoveRenderComponent`/`RemoveCollision` 호출
   - 참고: `DESTROY(ptr)` = `ptr->Destroy(); Delete(ptr)` (Destroy는 호출됨). `_root->Destroy()`가 `_childs` 순회하며 자식(충돌체) Destroy함. AttachToComponent는 자식 등록 정상.

2. **타일 BLOCK 시 스플래시 애니 안 보임** (몬스터는 보임)
   - 추정: 총알이 벽 타일 안쪽에 박혀 z-order로 가려짐. 충돌 위치 살짝 뒤로 빼거나 렌더 레이어 올리기. (박스 버그 해결 후)

### 미구현 (전투 마무리용)
- **Player 발사 검증** — `Player::Tick`에 쿨다운 발사(`_fireTimer -= dt; if(_headKeyActive && _fireTimer<=0){Fire(); _fireTimer=_fireRate;}`), `_headDir`은 Head함수 4개에서 세팅, 기본값 `(0,-1,0)`(주의: `-0`이면 0됨). **화살표 키로 발사**. 마우스(`mouseDown`)는 로그만 찍음
- **Player/Monster OnDeath 분기** — Player→게임오버 훅, Monster→제거/드롭. `Monster::BlockCallback`(충돌 즉시제거 조잡 코드)은 삭제

---

## 1. 이미 구현된 것 (프레임워크/기반)

- **적 AI FSM** — `AIController`, `AIComponent`, `MachineBase`, `AIState`, `AITransition`, `AIBoard`(블랙보드) 골격 존재
- **사운드** — `SoundComponent`, `SoundManager`, `Sound`
- **총알/발사** — `Bullet`, `RotBullet` + `Monster::Fire()`, `RotBulletFire()` 구현
- **방 생성** — `RoomManager` + `Level::GenerateRandomMap()`
- **문** — `Door` 클래스, `eDoorType`
- **타일 BLOCK 충돌** — `TileComponent::IsBlocked` + `MovementComponent` 연결 (플레이어 막힘 동작)
- **카메라 모드** — `CameraManager`에 상태/로직, `MainMenuUI::Camera()` 메뉴 (동작)
- **오브젝트 배치 스냅** — `RoomObjectUI`에 그리드 스냅 구현
- 충돌 시스템 / 타일 / 에디터(EditorTool 탭) / 카메라 / 인풋 컨텍스트

---

## 2. 없는 것 — 새로 만들어야 함 (게임 완성 핵심)

| 우선 | 작업 | 상태 | 시작점 / 관련 |
|------|------|------|--------------|
| 🔴 1 | **체력/피격/사망** | 전무 (`hp`/`TakeDamage`/`Die` 없음) | `Player`/`IsaacPlayer`/`Monster`에 HP·피격·사망 추가 |
| 🔴 2 | **데미지 처리** | 총알은 나가나 피해 없음 | `Bullet` 충돌 시 대상 HP 감소 (1번과 연결) |
| 🔴 3 | **아이템 픽업/효과** | `_isPickedUp` 플래그만 존재 | `Item`에 픽업 콜백 + 효과 적용, `Player` 스탯 |
| 🟠 4 | **HUD** | 전무 | 체력/아이템/맵 표시 UI 클래스 신규 |
| 🟠 5 | **게임 흐름** | 전무 | 시작/게임오버/승리/재시작 (`GameState` 등) |
| 🟡 6 | **보스** | Boss 클래스 없음 (충돌 프로파일만) | `Boss` 클래스 + 패턴 |
| 🟡 7 | **미니맵** | 전무 | 방 레이아웃 기반 미니맵 UI |

---

## 3. 있지만 동작 확인/연결 필요

- **방 전환(문 기능)** — 미구현. ① `Door::Init`에 **충돌체 없음**(`_col` 선언만, 생성 X) → 추가 필요(프로파일 "Door", OVERLAP 콜백). ② Door에 방향 멤버(`eDoorDir UP/DOWN/LEFT/RIGHT`) 필요. ③ 플레이어 OVERLAP → RoomManager 인접방 이동. ④ `RoomManager::Tick` 비어있음 — `FindCellAtWorldPos(플레이어pos)`로 현재 방 갱신 + `_level->SetTileMap(새 방)` 하는 전환 로직 추가
- **AI 상태/사운드** — 프레임워크는 있으나 실제 게임플레이 연결 여부 확인
- **오브젝트 Save/Load** — 미착수. `Level::Save/Load`가 `//todo`, `it.second->Save(file)` 주석 처리됨
  - 필요: `Actor::Save/Load` 가상함수 + 각 타입(Obstacle/Item/Door/Monster) override + Level 연동(타입 헤더 기록 후 Load 시 SpawnActor 복원)

---

## 4. 보류 / 선택

- SetTexture 공통화 리팩터링 (컴포넌트 다형성, 논의만)
- 캐릭터 AABB 기준 타일 막기 (현재 중심점 기준으로 충분)
- 카메라 입력 컨텍스트 전환 — 플레이어 정지 + WASD 카메라 이동 (설계 완료, 보류)
  - 방식: `InputComponent`의 `_activeContext`를 DEFAULT↔CAMERA 스왑
- 타일 아웃라인 on/off (`_showTileCollider` → `RenderOutLine` 연결) — 코드 준비됨, 미연결
- 오브젝트 충돌체 표시 (`_showObjectCollider` → `CollisionComponent::Render` 연결, `RoomEditor::_showCollider` 정리)

---

## 5. 추천 진행 순서

1. **체력/데미지/사망 (1·2)** — 전투의 기반. 이미 있는 총알·AI가 의미를 갖게 됨
2. **아이템 픽업/효과 (3)**
3. **HUD (4)** — 체력 생기면 표시 필요
4. **게임 흐름 (5)** — 사망 생기면 게임오버 연결
5. 방 전환 동작 확인 → 보스(6) → 미니맵(7)
6. 틈틈이: Save/Load, 작은 토글들(아웃라인/충돌체 표시)

> 메모: 대화가 길어지면 1M 컨텍스트 한계에 걸리므로, 큰 작업(체력/Save/Load 등)은 새 대화에서 이 문서를 먼저 읽고 시작할 것.
