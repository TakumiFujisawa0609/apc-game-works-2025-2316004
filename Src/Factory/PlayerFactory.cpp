#include "../pch.h"
#include "../Object/Character/Player/player.h"
#include "PlayerFactory.h"

std::unique_ptr<CharacterBase> PlayerFactory::CreateObject(void)
{
	// プレイヤーオブジェクトの生成
	return std::make_unique<Player>();
}
