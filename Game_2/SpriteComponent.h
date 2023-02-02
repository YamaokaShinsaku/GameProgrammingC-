#pragma once
#include "Component.h"
#include "SDL/SDL.h"

/// <summary>
/// SpriteComponentクラス
/// </summary>
class SpriteComponent : public Component
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner"> 所有者(Actor*) </param>
	/// <param name="drawOrder"> 描画順序 </param>
	/// 描画順序(drawOrder)が低いほど遠くにおかれる
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~SpriteComponent();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="renderer"> レンダラー </param>
	virtual void Draw(SDL_Renderer* renderer);

	/// <summary>
	/// テクスチャをセットする
	/// </summary>
	/// <param name="texture"> テクスチャ </param>
	virtual void SetTexture(SDL_Texture* texture);

	// getter / setter
	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexWidth() const { return mTexWidth; }
	int GetTexHeight() const { return mTexHeight; }

protected:
	SDL_Texture* mTexture;	// 描画するテクスチャ
	int mDrawOrder;			// 画家のアルゴリズムで使う描画順序

	// テクスチャの幅と高さ
	int mTexWidth;
	int mTexHeight;

};