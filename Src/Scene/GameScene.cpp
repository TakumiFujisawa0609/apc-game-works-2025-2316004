#include "GameScene.h"
#include <DxLib.h>
#include "../Application.h"
#include "../Common/Fader.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/UIManager.h"
#include "../Manager/Game/CollisionManager.h"
#include "../Manager/Game/CharacterManager.h"
#include "../Manager/Game/GravityManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Generic/DataBank.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include "../Object/Character/Player/Player.h"
#include "../Object/SkyDome/SkyDome.h"
#include "../Object/Character/Enemy/Enemy.h"
#include "../Object/Card/CardSystem.h"	
#include "../Object/Stage.h"	

#include "PauseScene.h"

GameScene::GameScene(void)
{
	//更新関数のセット
	updateFunc_ = std::bind(&GameScene::LoadingUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&GameScene::LoadingDraw, this);

	postEffectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	CharacterManager::CreateInstance();
	CollisionManager::CreateInstance();
	CardSystem::CreateInstance();
	UIManager::CreateInstance();

	//カードデータの開放
	DataBank::GetInstance().ReleaseCardData();
}

GameScene::~GameScene(void)
{
	//インスタンスの削除
	InitFontToHandle();
	CardSystem::GetInstance().Destroy();
	CollisionManager::GetInstance().Destroy();
	CharacterManager::GetInstance().Destroy();
	SoundManager::GetInstance().Release();
	UIManager::GetInstance().Destroy();



}

void GameScene::Load(void)
{
	//フォントの登録
	buttonFontHandle_ = CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE, 0);

	//何回かフォントを使うと一定の大きさだけ大きくなる不具合が発生しているので別で作る
	fontHandle_ = CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE, 0);

	//ポーズ画面のリソース
	pauseScene_ = std::make_shared<PauseScene>();
	pauseScene_->Load();

	UIManager::GetInstance().Load();

	//集中線画像のロード
	intensiveLineImg_1 = resMng_.Load(ResourceManager::SRC::INTENSIVE_LINE_1).handleId_;
	intensiveLineImg_2 = resMng_.Load(ResourceManager::SRC::INTENSIVE_LINE_2).handleId_;

	imgSkipButtom_ = resMng_.Load(ResourceManager::SRC::SKIP_BUTTOM).handleId_;
	imgSkipButtomMask_ = resMng_.Load(ResourceManager::SRC::SKIP_BUTTOM_MASK).handleId_;
	skipArcGaugeMaterial_ = std::make_unique<PixelMaterial>(L"ArcHpBarPS.cso", 2);
	skipArcGaugeRenderer_ = std::make_unique<PixelRenderer>(*skipArcGaugeMaterial_);
	skipArcGaugeMaterial_->AddTextureBuf(imgSkipButtomMask_);
	skipArcGaugeMaterial_->AddConstBuf({ 1.0f,0.0f,0.0f,1.0f });
	skipArcGaugeMaterial_->AddConstBuf({ 0.0f,0.0f,0.0f,0.0f });
	skipArcGaugeRenderer_->MakeSquareVertexFromCenter(SKIP_BTN_POS, SKIP_BTN_SIZE);

	stage_ = std::make_unique<Stage>();

	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Load();

	CharacterManager::GetInstance().Load();

	UIManager::GetInstance().Load();
}

void GameScene::Init(void)
{
	changeUpdate_ = {
		{UPDATE_PHASE::NONE,[this]() {ChangeNone(); }},
		{UPDATE_PHASE::FADE,[this]() {ChangeFade(); }},
		{UPDATE_PHASE::DIRECTION,[this]() {ChangeDirection(); }},
		{UPDATE_PHASE::NORMAL,[this]() {ChangeNormal(); }},
		{UPDATE_PHASE::SLOW,[this]() {ChangeSlow(); }}
	};
	updatePhase_ = UPDATE_PHASE::NONE;
	ChangeUpdatePhase(UPDATE_PHASE::DIRECTION);

	CharacterManager::GetInstance().Init();
	UIManager::GetInstance().Init();

	//シェイク状態を初期化
	scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::NONE);
	//カメラの当たり判定作成
	scnMng_.GetCamera().lock()->MakeColliderGeometry();

	//重力管理クラスを生成
	GravityManager::CreateInstance();

	stage_->Init();
	skyDome_->Init();
	SoundManager::GetInstance().LoadResource(SoundManager::SRC::GAME_BGM);
	SoundManager::GetInstance().Play(SoundManager::SRC::GAME_BGM, SoundManager::PLAYTYPE::LOOP);
	SoundManager::GetInstance().SetSystemVolume(BGM_GAME_VOL, static_cast<int>(SoundManager::TYPE::BGM));


}

void GameScene::UpdateIntensiveLineAnim(void)
{

	if (!CharacterManager::GetInstance().GetIsEnemyRoar())return;
	intensiveLineAnimFrame_++;
	if (intensiveLineAnimFrame_ >= INTENSIVE_LINE_ANIM_SPEED)
	{
		intensiveLineAnimFrame_ = 0;
	}
	intensiveLineAnimImg_ = (intensiveLineAnimFrame_ % INTENSIVE_LINE_ANIM_SPEED / 2 == 0) ? intensiveLineImg_1 : intensiveLineImg_2;
}

void GameScene::CheckSkip(void)
{


	if (InputManager::GetInstance().IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHTBUTTON_TOP))
	{
		if (skipKeepCnt_ > SKIP_BTN_TIME)
		{
			Skip();
		}
		skipKeepCnt_ += scnMng_.GetDeltaTime();
	}
	else
	{
		skipKeepCnt_ = 0.0f;
	}

	float per = skipKeepCnt_ / SKIP_BTN_TIME;
	//割合、どこから始めるかをセット
	skipArcGaugeMaterial_->SetConstBuf(1, { 0.0f,per,1.0f,0.0f });

	if (isSkippingDirection_)
	{
		scnMng_.Fade();
		if (scnMng_.GetIsEndFade())
		{
			ChangeUpdatePhase(UPDATE_PHASE::FADE);
			return;
		}
	}


}

void GameScene::NoneUpdate(void)
{
	//何もしない
}

void GameScene::FadeUpdate(void)
{
	scnMng_.Fade();
	if (scnMng_.GetIsEndFade())
	{
		ChangeUpdatePhase(UPDATE_PHASE::NORMAL);
	}
}

void GameScene::NormalUpdate(void)
{
	//ポーズ画面へ遷移
	if (inputMng_.IsTrgDown(KEY_INPUT_P))
	{
		scnMng_.PushScene(pauseScene_);
		return;
	}
	//とりあえず敵が倒れたら
	if (CharacterManager::GetInstance().IsSceneChageClearCondition())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME_CLEAR);
	}
	else if (CharacterManager::GetInstance().IsSceneChangeGameOverCondition())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME_OVER);
	}
	//ステージ
	stage_->Update();

	//キャラクターの更新
	CharacterManager::GetInstance().Update();

	//カード勝敗状態の監視
	CardSystem::GetInstance().CompareCards();

	UIManager::GetInstance().Update();

	//更新はアクション中のみ
	CollisionManager::GetInstance().Update();

	//終了した当たり判定の消去
	CollisionManager::GetInstance().Sweep();



#ifdef _DEBUG
	//デバッグ処理
	DebagUpdate();
#endif // _DEBUG


}

void GameScene::NormalDraw(void)
{
	//プレイヤーの描画
	skyDome_->Draw();
	stage_->Draw();
	CharacterManager::GetInstance().Draw();

	//UIなどの描画
	CharacterManager::GetInstance().Draw2D();

	UIManager::GetInstance().Draw();

	//UI2DManager::GetInstance().Draw();
#ifdef _DEBUG
	//デバッグ処理
	DebagDraw();
	//CardSystem::GetInstance().DrawDebug();
#endif // _DEBUG

}

void GameScene::DirectionDraw(void)
{
	//プレイヤーの描画
	skyDome_->Draw();
	stage_->Draw();
	CharacterManager::GetInstance().Draw();
	skipArcGaugeRenderer_->Draw();
	const Vector2F leftTop = SKIP_BTN_POS - (SKIP_BTN_SIZE / 2.0f);
	const Vector2F rightDown= SKIP_BTN_POS+ (SKIP_BTN_SIZE / 2.0f);
	DrawExtendGraph(leftTop.x, leftTop.y, rightDown.x, rightDown.y, imgSkipButtom_,true);

	//int 
	//GetString

	DrawStringFToHandle(leftTop.x+ SKIP_BTN_SIZE.x, leftTop.y+ SKIP_BTN_STR_OFFSET_Y, L"ボタン長押しでスキップ", 0x000000, fontHandle_);
	if (CharacterManager::GetInstance().GetIsEnemyRoar())
	{
		//集中線描画
		DrawGraph(0, 0, intensiveLineAnimImg_, true);
	}

}


void GameScene::ChangeUpdatePhase(const UPDATE_PHASE _phase)
{
	if (updatePhase_ == _phase)return;
	updatePhase_ = _phase;
	changeUpdate_[updatePhase_]();
}

void GameScene::ChangeNone(void)
{
}

void GameScene::ChangeFade(void)
{
	scnMng_.StartFadeIn();
	isSkippingDirection_ = true;
	scnMng_.GetCamera().lock()->ChangeMode(Camera::MODE::FOLLOW);
	updateFunc_ = [this]() {FadeUpdate(); };
}

void GameScene::ChangeDirection(void)
{
	isSkippingDirection_ = false;
	CharacterManager::GetInstance().ChangeCharacterDirectionUpdate();
	updateFunc_ = [this]() {DirectionUpdate(); };
	drawFunc_ = [this]() {DirectionDraw(); };
}

void GameScene::ChangeNormal(void)
{
	CharacterManager::GetInstance().ChangeCharacterNormalUpdate();
	scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::NONE);
	updateFunc_ = [this]() {NormalUpdate(); };
	drawFunc_ = [this]() {NormalDraw(); };
}

void GameScene::ChangeSlow(void)
{
	CharacterManager::GetInstance().ChangeCharacterNormalUpdate();
	updateFunc_ = [this]() {SlowUpdate(); };
}

void GameScene::DirectionUpdate(void)
{

	CheckSkip();

	if (scnMng_.GetFader().GetState()==Fader::STATE::FADE_OUT)return;

	if (scnMng_.GetCamera().lock()->IsEndDirectionMode())
	{
		ChangeUpdatePhase(UPDATE_PHASE::NORMAL);
		return;
	}

	CharacterManager::GetInstance().Update();
	UpdateIntensiveLineAnim();
}

void GameScene::SlowUpdate(void)
{
	if (--slowFrame_ > 0)return;
	slowFrame_ = FRAME_PER_UPDATE;
	NormalUpdate();
}

void GameScene::OnSceneEnter(void)
{
	//演出状態へ移行
	ChangeDirection();
}
void GameScene::Skip(void)
{
	if (isSkippingDirection_)return;
	scnMng_.StartFadeOut();
	isSkippingDirection_ = true;
}
#ifdef _DEBUG
void GameScene::DebagUpdate(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		scnMng_.ChangeScene(SceneManager::SCENE_ID::GAME_CLEAR);
	}
	frame_++;
}

void GameScene::DebagDraw(void)
{
	//DrawBox(
	//	0,
	//	0,
	//	Application::SCREEN_SIZE_X,
	//	Application::SCREEN_SIZE_Y,
	//	0x00ff00,
	//	true
	//);

	//DrawFormatString(
	//	0, 0,
	//	0x000000,
	//	L"GameScene"
	//);

	//constexpr float r = 40.0f;
	//float angle = DX_PI_F * 2.0f * static_cast<float>(frame_ % 360) / 60.0f;

	CardSystem::GetInstance().DrawDebug();

	////円運動を描画
	//DrawCircleAA(
	//	320+cos(angle) * r, 
	//	240+sin(angle) * r,
	//	r, 
	//	32, 
	//	0xff8888, 
	//	true);
}
#endif // _DEBUG