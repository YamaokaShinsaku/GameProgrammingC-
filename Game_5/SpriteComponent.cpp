#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Shader.h"
#include "Texture.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	: Component(owner)
	, mTexture(nullptr)
	, mDrawOrder(drawOrder)
	, mTexWidth(0)
	, mTexHeight(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(Shader* shader)
{
	if (mTexture)
	{
		// テクスチャの幅と高さで短形をスケーリング
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(mTexWidth),
			static_cast<float>(mTexHeight),
			1.0f
		);
		Matrix4 world = scaleMat * mOwner->GetWorldTransform();

		// ワールド変換の設定
		shader->SetMatrixUniform("uWorldTransform", world);
		// 現在アクティブなテクスチャを設定
		mTexture->SetActive();
		// 短形を描画
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}

void SpriteComponent::SetTexture(Texture* texture)
{
	mTexture = texture;
	// テクスチャの幅と高さを求める
	mTexWidth = texture->GetWidth();
	mTexHeight = texture->GetHeight();
}
