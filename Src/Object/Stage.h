#pragma once
#include "ObjectBase.h"

class ModelRenderer;
class ModelMaterial;

class Stage :
    public ObjectBase
{

public:

	Stage(void);
	~Stage(void)override;

    /// @brief ロード
    /// @param  
    void Load(void)override;

    /// @brief 初期化
    /// @param  
    void Init(void)override;

    /// @brief 更新
    /// @param  
    void Update(void)override;

    /// @brief 描画
    /// @param  
    void Draw(void)override;

    /// @brief 当たった時の処理
    /// @param _hitCol 
    void OnHit(const std::weak_ptr<Collider> _hitCol)override;
private:

    static constexpr float STAGE_SCL = 30.0f;

    //シェーダー関連
    std::unique_ptr<ModelMaterial> material_;
    std::unique_ptr<ModelRenderer> renderer_;
};

