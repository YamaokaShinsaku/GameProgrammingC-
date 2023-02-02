#pragma once
#include "Component.h"
#include "SDL/SDL.h"

/// <summary>
/// SpriteComponent�N���X
/// </summary>
class SpriteComponent : public Component
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner"> ���L��(Actor*) </param>
	/// <param name="drawOrder"> �`�揇�� </param>
	/// �`�揇��(drawOrder)���Ⴂ�قǉ����ɂ������
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SpriteComponent();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="renderer"> �����_���[ </param>
	virtual void Draw(SDL_Renderer* renderer);

	/// <summary>
	/// �e�N�X�`�����Z�b�g����
	/// </summary>
	/// <param name="texture"> �e�N�X�`�� </param>
	virtual void SetTexture(SDL_Texture* texture);

	// getter / setter
	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexWidth() const { return mTexWidth; }
	int GetTexHeight() const { return mTexHeight; }

protected:
	SDL_Texture* mTexture;	// �`�悷��e�N�X�`��
	int mDrawOrder;			// ��Ƃ̃A���S���Y���Ŏg���`�揇��

	// �e�N�X�`���̕��ƍ���
	int mTexWidth;
	int mTexHeight;

};