#pragma once
#include "SpriteComponent.h"
#include "Math.h"
#include <vector>

/// <summary>
/// BGSpriteComponentクラス
/// </summary>
class BGSpriteComponent : public SpriteComponent
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner"> 所有者(Actor*) </param>
	/// <param name="drawOrder"> 描画順序 </param>
	/// 描画順序の初期値は下げる（背景とするため）
	BGSpriteComponent(class Actor* owner, int drawOrder = 10);

	// 更新と描画は親からオーバーライドする	
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="deltaTime"> デルタタイム </param>
	void Update(float deltaTime)override;
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="renderer"> レンダラー </param>
	void Draw(SDL_Renderer* renderer)override;

	/// <summary>
	/// 背景用テクスチャを設定する
	/// </summary>
	/// <param name="textures"> 背景用テクスチャ </param>
	void SetBGTextures(const std::vector<SDL_Texture*>& textures);

	// getter / setter
	void SetScreenSize(const Vector2& size) { mScreenSize = size; }
	void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
	float GetScrollSpeed()const { return mScrollSpeed; }

private:
	// 個々の背景画像とオフセットをカプセル化する構造体
	struct BGTexture
	{
		SDL_Texture* mTexture;
		Vector2 mOffset;
	};

	// 背景画像テクスチャ
	std::vector<BGTexture> mBGTextures;

	Vector2 mScreenSize;		// 画面サイズ
	float mScrollSpeed;		// スクロール速度
};