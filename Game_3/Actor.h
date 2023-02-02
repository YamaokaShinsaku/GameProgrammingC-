#pragma once
#include <vector>
#include "Math.h"

/// <summary>
/// Actor�N���X(���N���X)
/// </summary>
class Actor
{
public:
	// �A�N�^�[�̏�ԊǗ��p
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="game"> Game�|�C���^ </param>
	Actor(class Game* game);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Actor();

	/// <summary>
	/// �X�V(�Q�[������Ăяo�����)...�I�[�o�[���C�h�s��
	/// </summary>
	/// <param name="deltaTime"> �f���^�^�C�� </param>
	void Update(float deltaTime);
	/// <summary>
	/// �A�N�^�[�����S�R���|�[�l���g���X�V...�I�[�o�[���C�h�s��
	/// </summary>
	/// <param name="deltaTime"> �f���^�^�C�� </param>
	void UpdateComponents(float deltaTime);
	/// <summary>
	/// �A�N�^�[�Ǝ��̍X�V����...�I�[�o�[���C�h�\
	/// </summary>
	/// <param name="deltaTime"> �f���^�^�C�� </param>
	virtual void UpdateActor(float deltaTime);

	/// <summary>
	/// ���͗p�֐�(Game����Ăяo�����)
	/// </summary>
	/// <param name="keyState"> �L�[�{�[�h�̏�� </param>
	/// �I�[�o�[���C�h�s��
	void ProcessInput(const uint8_t* keyState);
	/// <summary>
	/// �A�N�^�[�Ǝ��̓��͗p�R�[�h
	/// </summary>
	/// <param name="keyState"> �L�[�{�[�h�̏�� </param>
	/// �I�[�o�[���C�h�\
	virtual void ActorInput(const uint8_t* keyState);

	// getter / setter
	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }

	Vector2 GetForward() const { return Vector2(Math::Cos(mRotation), -Math::Sin(mRotation)); }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }


	/// <summary>
	/// �R���|�[�l���g�̒ǉ�
	/// </summary>
	/// <param name="component"> Component�|�C���^ </param>
	void AddComponent(class Component* component);
	/// <summary>
	/// �R���|�[�l���g�̍폜
	/// </summary>
	/// <param name="component"> Component�|�C���^ </param>
	void RemoveComponent(class Component* component);

private:
	// �A�N�^�[�̏��
	State mState;
	
	// ���W�ϊ�
	Vector2 mPosition;	// �A�N�^�[�̒��S�ʒu
	float mScale;		// �A�N�^�[�̃X�P�[��(1.0f = 100%)
	float mRotation;	// ��]�̊p�x(���W�A��)

	// �A�N�^�[�����R���|�[�l���g
	std::vector<class Component*> mComponents;
	class Game* mGame;
};