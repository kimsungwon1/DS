# DS Project

## 프로젝트 경로
- 프로젝트 루트: `D:\ueg\DS`
- 엔진: `D:\Program Files\UE_5.7`

## 빌드
```powershell
& "D:\Program Files\UE_5.7\Engine\Build\BatchFiles\Build.bat" DSEditor Win64 Development -Project="D:\ueg\DS\DS.uproject" -WaitMutex -architecture=x64
```

## 설치된 플러그인
- **UnrealMCP** — 기존 설치
- **VibeUE** — AI 채팅으로 Blueprint/에셋 편집. API 키: vibeue.com/login → Tools > VibeUE > AI Chat Settings
- **UnrealClaude** — Claude Code CLI를 UE 에디터에 직접 통합. MCP bridge: `D:\ueg\DS\Plugins\UnrealClaude\UnrealClaude\Resources\mcp-bridge`

## UnrealClaude MCP 서버
- 포트: `http://127.0.0.1:8088/mcp` (SSE 방식)
- 확인: `curl -H "Accept: text/event-stream" http://127.0.0.1:8088/mcp`

## Claude Code CLI
- 설치됨 (`npm install -g @anthropic-ai/claude-code`)
- 로그인: kimsunguen@gmail.com (Pro 플랜)

## DSEditor.Target.cs 수정 이력
- `DefaultBuildSettings`: V5 → V6
- `IncludeOrderVersion`: Unreal5_6 → Latest

## 진행 중인 작업 — D&D 파티 RPG UI

### 목표
사진(Wizardry 스타일) 참고한 D&D 풍 파티 RPG UI. 캐릭터 창은 화면 **하단 가로 정렬**.

### C++ 부모 클래스 (이미 빌드 완료)
| 파일 | 설명 |
|---|---|
| `Source/DS/Public/UI/DSPartyMemberWidget.h` | 파티 멤버 1칸 (초상화+HP/SP바) |
| `Source/DS/Public/UI/DSActionButtonWidget.h` | 행동 버튼 1개 |
| `Source/DS/Public/UI/DSActionPanelWidget.h` | 행동 목록 패널 |
| `Source/DS/Public/UI/DSEnemyInfoWidget.h` | 적 정보 상단 바 |
| `Source/DS/Public/UI/DSBattleHUDWidget.h` | 메인 HUD (DSMainWidget 상속) |

### 다음 할 일 — UE 에디터에서 Blueprint 위젯 제작
에디터를 직접 제어해서 아래 5개 Blueprint 위젯 생성:

1. **WBP_PartyMember** (부모: DSPartyMemberWidget)
   - Border(SelectionBorder) > VerticalBox
   - Image(PortraitImage), TextBlock(CharacterNameText), TextBlock(ProfessionText)
   - ProgressBar(HPBar), TextBlock(HPText)
   - ProgressBar(SPBar), TextBlock(SPText)
   - Overlay(DeadOverlay) > TextBlock "DEAD"

2. **WBP_ActionButton** (부모: DSActionButtonWidget)
   - Button(ActionButton) > HorizontalBox
   - TextBlock(ActionNameText), TextBlock(ActionCostText)

3. **WBP_ActionPanel** (부모: DSActionPanelWidget)
   - VerticalBox > TextBlock(ActiveCharacterNameText), ScrollBox(ActionListBox)
   - Details: ActionButtonClass = WBP_ActionButton

4. **WBP_EnemyInfo** (부모: DSEnemyInfoWidget)
   - HorizontalBox > TextBlock(EnemyNameText), ProgressBar(EnemyHPBar), TextBlock(EnemyHPText)

5. **WBP_BattleHUD** (부모: DSBattleHUDWidget)
   - CanvasPanel 전체
   - WBP_EnemyInfo(EnemyInfoWidget) 상단
   - TextBlock(CombatStatusText) 중앙
   - WBP_ActionPanel(ActionPanelWidget) 우측
   - Border(CombatLogBorder) > TextBlock(CombatLogText) 하단
   - HorizontalBox(PartyMemberBox) 맨 하단 가로 전체
   - Details: PartyMemberWidgetClass = WBP_PartyMember

### 주의사항
- BindWidget 이름 대소문자 정확히 일치해야 함
- Unreal Editor 시작 메뉴 바로가기: `%APPDATA%\Microsoft\Windows\Start Menu\Programs\Unreal Editor.lnk`
- 다음 세션에서 computer-use로 직접 제어 가능
