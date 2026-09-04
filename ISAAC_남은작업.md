# ISAAC_Copy 프로젝트 — 남은 작업 정리

> 다음 세션이 이 문서를 읽고 이어서 작업하기 위한 현황/TODO 문서.
> **갱신: 2026-08-28** (코드 기준: 최종 커밋 `작업끝` 2026-06-12 + 현재 워킹트리).
> 이전 버전(2026-06-06/07) 이후 전투·아이템·Save/Load·방전환·타이틀이 대거 완료됨. 아래는 코드 재스캔 기준 갱신.

---

## 0. 지금 바로 할 것 ★최우선

### 🐛 A. 워킹트리 미커밋 버그 (커밋 전 반드시 처리)
1. **총알 null 역참조 크래시** — `Client/Object/Bullet.cpp:66`
   - `Ptr<TileMap> tileMap = level->GetTileMap();` 뒤의 null 가드가 **주석 처리됨**(63~64줄).
   - `GetTileMap()`은 `Lock<TileMap>(_tileMap)`(weak.lock) 결과라 **null 가능** → 바로 다음 줄 `tileMap->GetTileComponent()`에서 크래시.
   - 방/레벨 전환 순간 총알이 살아있으면 재현. **해결:** `if(!tileMap) return;` 가드 복구.
2. **방 배정 시작 셀 이중 처리** — `Client/World/RoomManager.cpp` `AssignRooms`
   - "임시 보완용" 블록(210~228)이 시작 셀을 먼저 배정하는데, `_roomPos`(std::map) 메인 루프가 시작 셀 `{0,0}`을 **다시 순회** → 이중 처리.
   - 대개 무해하나, 시작방 형태가 `{0,0}`을 emptyCell로 두면 시작방 파일이 덮어써지고 유령 방이 생김.
   - **해결:** 임시 블록 제거하고 단일 루프에서 시작 셀을 먼저 배정(공유 로직 위 특수케이스 지양).

### 🐛 B. 충돌체 디버그 박스 잔존 (이전 문서 0-1번, **여전히 미해결**)
- 증상: 액터는 제거됐는데 **충돌체 박스만 화면에 남음**.
- 원인: `SceneComponent::Destroy` / `CollisionComponent::Destroy` **둘 다** `Ptr owner = Lock<...>(...); if(!owner) return;` 패턴 → 소멸 시점에 owner/level Lock 실패 시 `RemoveRenderComponent`/`RemoveCollision`이 **스킵**됨.
- **해결(제안 유지):** 컴포넌트 `Init` 때 `int32 _ownerActorID`(+ 필요한 레이어/콜라이더 ID)를 저장 → `Destroy`에서 Lock 없이 그 ID로 `RemoveRenderComponent`/`RemoveCollision` 호출. `CollisionComponent::Destroy`의 `//todo`(리액션 목록에서 자기 제거)도 함께 정리.

### C. 게임 부팅 레벨 확인
- 현재 `World::Init`이 **`EditorLevel`로 부팅**(`World.cpp:18`), `TitleLevel`은 주석(`:19`).
- 실제 게임 플레이로 시작하려면 `TitleLevel`(또는 `GameLevel`)로 스위칭 필요. 게임/에디터 진입 분기 정리할 것.

---

## 1. 완료된 것 (이전 문서 이후 새로 끝난 항목 포함)

### 전투 (완료)
- **HP/피격/사망** — `Pawn` `_hp/_maxHp/TakeDamage/OnDeath/IsDead`. `Player::OnDeath`(사망 애니 `IASSC_DEATH` + `ChangeLevel<TitleLevel>` 타이틀 복귀), `Monster::OnDeath`(추격·이동 정지, 충돌 비활성).
- **데미지 처리** — `Bullet` `_damage` + `OnHit`으로 피격 Pawn `TakeDamage`. 프로파일 라우팅(PlayerBullet↔Monster / MonsterBullet↔Player).
- **플레이어 발사** — `Player::Tick`에 쿨다운 발사(`_fireTimer/_fireRate=0.8`), 화살표 키 방향 발사.
- **총알 애니** — `Tears` 아틀라스(64px, 4x3), 비행 중 정지 / 충돌 시 스플래시 재생.
- **방 전투 잠금** — `RoomManager::Tick`에 `isBattleActive` → 방 몬스터 전멸 시 `EndBattle`, 전투 중 방 이동 차단.

### 아이템 (완료)
- **픽업/효과** — `Item::OnOverlap`에서 비용(코인/키/폭탄) 체크·차감 후 `ApplyItemStats`/`ApplyItemVisual`/`AddEffect`. PASSIVE/ACTIVE 분기.

### 방 생성 / 전환 (완료)
- **랜덤 맵** — `RoomManager` 랜덤워크 레이아웃 + `set` 점유그리드 겹침검사(`TryPlaceRoom`) + Fisher–Yates 셔플 배정 + 4방향 인접 연결.
- **문/방 전환** — `Door` AABB 충돌체 + "Door" 프로파일 생성, `RoomManager::Tick`의 위치 감지 + `MoveToRoom`/`FindCellAtWorldPos`로 방 전환.
- **문 스프라이트** — `Door`를 SpriteComponent(아틀라스)로, 렌더 레이어 분리.

### Save / Load (완료)
- `Level::Save/Load` 구현. 타입 태그 기록 후 로드 시 `SpawnActor`로 복원.
- 전 타입 override: `TileMap/Monster/Obstacle/Door/Item/Npc` `Save/Load`.

### AI (연결됨)
- `Monster::Init`에서 `AIController` 스폰 → `AIComponent` 자동 생성/획득. FSM 골격(`MachineBase/AIState/AITransition/AIBoard`)과 실제 몬스터 연결.

### 타이틀 (신규 추가)
- `TitleLevel` — Menu_Title 배경/애니 + "Press Start" 위젯. (문서 이전 범위 밖 신규 작업)

### 기반 (기존)
- Actor–Component, 스마트포인터 정책(Ptr/Weak/New/Cast/Lock), 매니저 싱글톤, D3D11 렌더, 충돌 채널/프로파일, 카메라 모드, ImGui 에디터(타일맵/오브젝트 배치·그리드 스냅).

---

## 2. 아직 없는 것 — 새로 만들어야 함 (게임 완성 핵심)

| 우선 | 작업 | 상태 | 시작점 / 관련 |
|------|------|------|--------------|
| 🔴 1 | **HUD** | ❌ 미구현 | UI 프리미티브(`ProgressBar/Image/TextBlock/Widget/UIManager`)는 있음. `GameLevel`에 체력/코인/키/폭탄·아이템 표시 위젯 신규 |
| 🔴 2 | **게임 흐름(GameState)** | 🟡 부분 | 타이틀 존재 + 사망→타이틀 복귀는 됨. **명시적 GameOver/Victory/Restart 상태 관리 없음**. 층 클리어/승리 조건 정의 필요 |
| 🟠 3 | **보스** | ❌ 미구현 | `BossRoom.room`·`room_Boss.png` 에셋만 존재. **Boss 클래스 + 패턴 없음** |
| 🟠 4 | **미니맵** | ❌ 미구현 | `_roomPos` 방 레이아웃 기반 미니맵 UI 신규 |
| 🟡 5 | **상점(Shop)** | 🟡 부분 | `ShopRoom.room`·아이템 비용 로직은 있음. 상점 방 진입/구매 UX 연결 확인 필요 |

---

## 3. 있지만 동작 확인 / 폴리시 필요

- **타일 BLOCK 시 스플래시 애니** — 벽 안쪽 z-order로 가려질 수 있음(이전 문서 0-2). 충돌 지점 뒤로 빼거나 렌더 레이어 상향.
- **게임/에디터 진입 분기** — `World.cpp`가 EditorLevel로 부팅 중(위 0-C).
- **AI 행동 검증** — FSM 연결됨. 실제 추격/발사/사망 전이가 의도대로 도는지 플레이 검증.
- **작은 토글** — 타일 아웃라인 on/off(`_showTileCollider`→`RenderOutLine`), 오브젝트 충돌체 표시(`_showObjectCollider`) 연결.

---

## 4. 보류 / 선택

- SetTexture 공통화 리팩터링(컴포넌트 다형성).
- 캐릭터 AABB 기준 타일 막기(현재 중심점 기준).
- 카메라 입력 컨텍스트 전환(플레이어 정지 + WASD 카메라). 설계 완료, 보류.
- 충돌 브로드페이즈(공간 분할) 도입 — 확장성.

---

## 5. 추천 진행 순서

1. **워킹트리 버그(0-A) + 박스 잔존(0-B) 정리** — 크래시/잔존 제거가 최우선. 커밋 전 필수.
2. **게임 부팅 레벨(0-C)** — 실제 플레이 진입 경로 확정.
3. **HUD(2-1)** — 체력·재화 표시(전투/아이템이 이미 동작하므로 바로 의미 있음).
4. **게임 흐름(2-2)** — GameOver/Victory/Restart 상태 정리.
5. **보스(2-3) → 미니맵(2-4) → 상점 UX(2-5)** — 콘텐츠 확장.
6. 틈틈이: AI 행동 검증, 작은 토글, 스플래시 z-order.

> 메모: 큰 작업(보스/게임흐름)은 새 대화에서 이 문서를 먼저 읽고 시작할 것.
