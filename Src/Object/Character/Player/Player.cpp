#include<algorithm>
#include<iostream>
#include<fstream>
#include "../../Src/Lib/nlohmann/json.hpp"
#include "../../../Utility/Utility3D.h"
#include "../../../Utility/UtilityCommon.h"
#include "../Application.h"
//#include "../../Manager/Game/GravityManager.h"
#include "../../../Manager/Game/PlayerManager.h"
#include "../../../Manager/Resource/ResourceManager.h"
//#include "../../Manager/System/SoundManager.h"
#include "../../../Manager/Generic/SceneManager.h"
//#include"../../Manager/System/DateBank.h"

#include "../../../Manager/Generic/Camera.h"

//#include "../../Renderer/ModelMaterial.h"
//#include "../../Renderer/ModelRenderer.h"

//#include "../../Object/Common/Geometry/Sphere.h"
//#include "../../Object/Common/Geometry/Line.h"
//#include"../../Object/Common/Geometry/Model.h"
//#include"../../Object/Common/EffectController.h"
#include"../../Common/Capsule.h"
#include"../Object/Card/CardDeck.h"
#include "../../../Object/Common/AnimationController.h"
#include"./ActionController.h"
//#include"./PlayerOnHit.h"
#include "./PlayerLogic.h"

using json = nlohmann::json;

#include "Player.h"
Player::Player(void)
	:playerNum_()
	, cntl_()
	, padNum_()
	, state_()
	, stateUpdate_()
{
	trans_ = Transform();
	playerNum_ = 0;
	cntl_ = InputManager::CONTROLL_TYPE::ALL;
	//初めのJOYPADがkey_padなのでパッドの番号に合わせる
	//パッド番号を設定
	padNum_ = static_cast<InputManager::JOYPAD_NO>(playerNum_ + 1);

	
}

Player::~Player(void)
{
	SavePos(L"PlayerPos.json");
}

void Player::Load(void)
{
	trans_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal =
		Quaternion::Euler({ 0.0f, UtilityCommon::Deg2RadF(0.0f), 0.0f });

	animationController_ = std::make_unique<AnimationController>(trans_.modelId);
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE),ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::IDLE));
	animationController_->Add(static_cast<int>(ANIM_TYPE::RUN), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::RUN));
	animationController_->Add(static_cast<int>(ANIM_TYPE::REACT), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::REACT));
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK_1), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::P_ATTACK_1));
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK_2), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::P_ATTACK_2));
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK_3), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::P_ATTACK_3));

	//animType_.emplace(
	//	{ ANIM_TYPE::IDLE,static_cast<int>(ANIM_TYPE::IDLE) }
	//	, { ANIM_TYPE::RUN,static_cast<int>(ANIM_TYPE::RUN) }
	//)


	//プレイヤー入力
	logic_ = std::make_unique<PlayerLogic>(padNum_, InputManager::CONTROLL_TYPE::ALL);
	logic_->Init();

	//カードデッキ
	cardCenterPos_ = { 140,140 };//カードの中心位置
	deck_ = std::make_shared<CardDeck>(cardCenterPos_, PLAYER_NUM);
	//デッキに山札追加
	for (int i = 0; i < CARD_NUM_MAX; i++)
	{
		deck_->AddDrawPile(CARD_POWS[i]);
	}
	deck_->Init();

	//アクション
	action_ = std::make_unique<ActionController>(*logic_,trans_,*deck_,*animationController_,padNum_);
	action_->Load();
}

void Player::Init(void)
{
	//Transformの設定
	trans_.quaRot = Quaternion();
	trans_.scl = MODEL_SCL;
	trans_.quaRotLocal = 
		Quaternion::Euler({ 0.0f, UtilityCommon::Deg2RadF(MODEL_LOCAL_DEG), 0.0f });

	float posX = PLAYER_ONE_POS_X + DISTANCE_POS * playerNum_;
	trans_.pos={ posX,0.0f,0.0f };
	trans_.localPos = { 0.0f,-Player::RADIUS,0.0f };

	cap_ = std::make_unique<Capsule>(trans_);
	cap_->SetLocalPosTop({ 0.0f,150.0f,0.0f });
	cap_->SetLocalPosDown({ 0.0f,0.0f,0.0f });
	cap_->SetRadius(25.0f);

	//プレイヤー状態
	changeStates_.emplace(PLAYER_STATE::ALIVE, [this]() {ChangeAlive(); });
	changeStates_.emplace(PLAYER_STATE::DEATH, [this]() {ChangeDeath(); });
	changeStates_.emplace(PLAYER_STATE::GOAL, [this]() {ChangeGoal(); });

	//生存状態
	ChangeState(PLAYER_STATE::ALIVE);

	action_->Init();

	//更新
	trans_.Update();

	LoadPos(L"PlayerPos.json");
}

void Player::Update(void)
{
	//animationController_->Update();
#ifdef DEBUG_ON
	//CubeMove();
#endif // DEBUG_ON

	animationController_->Update();

	//プレイヤー状態更新
	stateUpdate_();

	//回転の同期
	trans_.quaRot = action_->GetPlayerRotY();
	
	trans_.pos = VAdd(trans_.pos, action_->GetMovePow());

	trans_.Update();
}

void Player::Draw(void)
{
	//通常描画
	MV1DrawModel(trans_.modelId);

	action_->DrawDebug();
	
	////影の描画
	//shadow_->Draw();

#ifdef DEBUG_ON
	DrawDebug();
#endif // DEBUG_ON
}
#ifdef DEBUG_ON
void Player::DrawDebug(void)
{
	unsigned int color = 0xffffff;
	const int HIGH = 10;
	const int WIDTH = 200;
	//DrawSphere3D(trans_.pos, RADIUS, 4, 0xff0000, 0xff0000, true);
	cap_->Draw();
}

#endif // DEBUG_ON

void Player::ChangeState(PLAYER_STATE _state)
{
	state_ = _state;
	changeStates_[state_]();
}
void Player::ChangeAlive(void)
{
	stateUpdate_ = std::bind(&Player::AliveUpdate, this);
}
void Player::AliveUpdate(void)
{
	//アクション関係更新
	Action();
}
void Player::ChangeDeath(void)
{
	stateUpdate_ = std::bind(&Player::DeathUpdate, this);
}
void Player::DeathUpdate(void)
{

}
void Player::ChangeGoal(void)
{

}
void Player::GoalUpdate(void)
{

}

void Player::Action(void)
{
	//アクション関係の更新
	logic_->Update();
	action_->Update();

}

void Player::SavePos(const std::wstring _fileName)
{
	json playerData;
	json posArray;
	json rotArray;
	json quaRotAttay;

	playerData["Position"]["x"] = trans_.pos.x;
	playerData["Position"]["y"] = trans_.pos.y;
	playerData["Position"]["z"] = trans_.pos.z;

	//	{	//格納
	//		{"x",trans_.pos.x},
	//		{"y",trans_.pos.y},
	//		{"z",trans_.pos.z}
	//	}
	//);
	//角度情報
	playerData["Rotation"]["x"] = trans_.rot.x;
	playerData["Rotation"]["y"] = trans_.rot.y;
	playerData["Rotation"]["z"] = trans_.rot.z;
	//rotArray.push_back(
	//	{	//格納
	//		{"x",trans_.rot.x},
	//		{"y",trans_.rot.y},
	//		{"z",trans_.rot.z}
	//	}
	//);
	//クォータニオン回転
	playerData["QuaRot"]["x"] = trans_.quaRot.x;
	playerData["QuaRot"]["y"] = trans_.quaRot.y;
	playerData["QuaRot"]["z"] = trans_.quaRot.z;
	playerData["QuaRot"]["w"] = trans_.quaRot.w;
	//quaRotAttay.push_back(
	//	{
	//		{"x",trans_.quaRot.x},
	//		{"y",trans_.quaRot.y},
	//		{"z",trans_.quaRot.z},
	//		{"w",trans_.quaRot.w}
	//	}
	//);

	//JSONファイルに書き込み
	std::ofstream outFile(Application::PATH_JSON+_fileName.c_str());
	//outFile.open(Application::PATH_JSON + _fileName.c_str());
	if(outFile.is_open())
	{
		//playerData["Position"]=posArray;
		//playerData["Rotation"].push_back(rotArray);
		//playerData["QuaRot"].push_back(quaRotAttay);
		auto dmp = playerData.dump(4);
		outFile.write(dmp.c_str(), dmp.size());
		outFile.close();
	}
	else
	{
		std::cout << "ファイルが開けません" << std::endl;
	}
}

void Player::LoadPos(const std::wstring _fileName)
{
	std::ifstream inFile(Application::PATH_JSON + _fileName.c_str());
	//inFile.open(Application::PATH_JSON + _fileName.c_str());
	if (!inFile.is_open())
	{
		//std::cerr << "ファイルが開けません" << std::endl;
		return;
	}

	json j;
	inFile >> j;
	//JSONファイルの読み込み
	if (j.contains("Position"))
	{
		//std::vector<VECTOR>positions;
		//for (const auto& pos : j["Position"])
		//{
			VECTOR position;
			position.x = j["Position"]["x"].get<float>();
			position.y = j["Position"]["y"].get<float>();
			position.z = j["Position"]["z"].get<float>();
		//}
		trans_.pos = position;
	}
	
	

	if (j.contains("Rotation"))
	{
		//std::vector<VECTOR>rotations;
		//for (const auto& rot : j["Rotation"])
		//{
			VECTOR rotation;
			rotation.x = j["Rotation"]["x"].get<float>();
			rotation.y = j["Rotation"]["y"].get<float>();
			rotation.z = j["Rotation"]["z"].get<float>();
		//}
		trans_.rot = rotation;
	}

	if (j.contains("QuaRot"))
	{

		Quaternion q;
		q.x = j["QuaRot"]["x"].get<float>();
		q.y = j["QuaRot"]["y"].get<float>();
		q.z = j["QuaRot"]["z"].get<float>();
		q.w = j["QuaRot"]["w"].get<float>();

		trans_.quaRot = q;

	}
}

