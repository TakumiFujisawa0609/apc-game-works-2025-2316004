#pragma once

#include <string>

class DataBank
{
public:	
	/// @brief 明示的にインステンスを生成する
	/// @param  
	static void CreateInstance(void);
	
	/// @brief 静的インスタンスの取得
	/// @param  
	/// @return インスタンスを返す
	static DataBank& GetInstance(void);

	/// @brief 初期化
	/// @param  
	void Init(void);

	/// @brief 解放処理
	/// @param  
	void Destroy(void);
	
	/// @brief プレイヤー人数を設定
	/// @param _num プレイヤー人数を設定
	/// @return 正しく設定できればtrue,問題が起きればfalse
	bool SetPlayerNum(int _num);
	
	/// @brief  プレイヤー人数を返す
	/// @param  
	/// @return プレイヤー人数
	int GetPlayerNum(void);






private:


	static DataBank* instance_;
	DataBank(void);
	DataBank(const DataBank& instance_) = default;
	~DataBank(void);
	int playerNum_;		//プレイヤーの人数
	int maxPlayerNum_;	//コントローラー数よりプレイヤー人数が多くならないようにする
};

