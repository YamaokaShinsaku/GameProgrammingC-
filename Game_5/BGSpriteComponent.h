#pragma once
#include "SpriteComponent.h"
#include "Math.h"
#include <vector>

/// <summary>
/// BGSpriteComponent�N���X
/// </summary>
class BGSpriteComponent : public SpriteComponent
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner"> ���L��(Actor*) </param>
	/// <param name="drawOrder"> �`�揇�� </param>
	/// �`�揇���̏����l�͉�����i�w�i�Ƃ��邽�߁j
	BGSpriteComponent(class Actor* owner, int drawOrder = 10);

	// �X�V�ƕ`��͐e����I�[�o�[���C�h����	
	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="deltaTime"> �f���^�^�C�� </param>
	void Update(float deltaTime)override;
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="renderer"> �����_���[ </param>
	void Draw(SDL_Renderer* renderer)override;

	/// <summary>
	/// �w�i�p�e�N�X�`����ݒ肷��
	/// </summary>
	/// <param name="textures"> �w�i�p�e�N�X�`�� </param>
	void SetBGTextures(const std::vector<SDL_Texture*>& textures);

	// getter / setter
	void SetScreenSize(const Vector2& size) { mScreenSize = size; }
	void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
	float GetScrollSpeed()const { return mScrollSpeed; }

private:
	// �X�̔w�i�摜�ƃI�t�Z�b�g���J�v�Z��������\����
	struct BGTexture
	{
		SDL_Texture* mTexture;
		Vector2 mOffset;
	};

	// �w�i�摜�e�N�X�`��
	std::vector<BGTexture> mBGTextures;

	Vector2 mScreenSize;		// ��ʃT�C�Y
	float mScrollSpeed;		// �X�N���[�����x
};