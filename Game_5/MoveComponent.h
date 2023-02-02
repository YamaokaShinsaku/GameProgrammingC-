#pragma once
#include "Component.h"

/// <summary>
/// �A�N�^�[�����̃X�s�[�h�őO�i������
/// </summary>
class MoveComponent : Component
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner"> ���L��(Actor*) </param>
	/// <param name="updateOrder"> �`�揇�� </param>
	/// updateOrder�̒l���������قǁA��ɍX�V�����
	MoveComponent(class Actor* owner, int updateOrder = 10);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="deltaTime"> �f���^�^�C�� </param>
	void Update(float deltaTime)override;

	// getter / setter
	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }

private:
	// ��]�𐧌䂷��i���W�A�� / �b�j
	float mAngularSpeed;
	// �O�i�^���𐧌䂷��i�P�ʒ� / �b�j
	float mForwardSpeed;
};