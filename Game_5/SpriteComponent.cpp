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
		// �e�N�X�`���̕��ƍ����ŒZ�`���X�P�[�����O
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(mTexWidth),
			static_cast<float>(mTexHeight),
			1.0f
		);
		Matrix4 world = scaleMat * mOwner->GetWorldTransform();

		// ���[���h�ϊ��̐ݒ�
		shader->SetMatrixUniform("uWorldTransform", world);
		// ���݃A�N�e�B�u�ȃe�N�X�`����ݒ�
		mTexture->SetActive();
		// �Z�`��`��
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}

void SpriteComponent::SetTexture(Texture* texture)
{
	mTexture = texture;
	// �e�N�X�`���̕��ƍ��������߂�
	mTexWidth = texture->GetWidth();
	mTexHeight = texture->GetHeight();
}
