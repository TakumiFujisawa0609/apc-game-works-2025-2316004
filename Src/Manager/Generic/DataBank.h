#pragma once

#include <string>

class DataBank
{
public:	
	/// <summary>
	/// 明示的にインステンスを生成する
	/// </summary>
	/// <param name=""></param>
	static void CreateInstance(void);

	/// <summary>
	/// 静的インスタンスの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンスを返す</returns>
	static DataBank& GetInstance(void);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	/// <summary>
	/// 解放処理
	/// </summary>
	/// <param name=""></param>
	void Destroy(void);

	/// <summary>
	/// プレイヤー人数を設定
	/// </summary>
	/// <param name="num">プレイヤー人数</param>
	/// <returns>正しく設定できればtrue,問題が起きればfalse</returns>
	bool SetPlayerNum(int _num);
	

	/// <summary>
	/// プレイヤー人数を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>プレイヤー人数</returns>
	int GetPlayerNum(void);






private:


	static DataBank* instance_;
	DataBank(void);
	DataBank(const DataBank& instance_) = default;
	~DataBank(void);
	int playerNum_;		//プレイヤーの人数
	int maxPlayerNum_;	//コントローラー数よりプレイヤー人数が多くならないようにする
};

