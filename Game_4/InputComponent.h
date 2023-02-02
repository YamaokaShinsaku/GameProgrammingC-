#pragma once
#include "MoveComponent.h"
#include <cstdint>

/// <summary>
/// �L�[���͂ŏ��L�A�N�^�[�̑O�i�A��ށA��]�𐧌䂷��
/// </summary>
class InputComponent : public MoveComponent
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner"> ���L�A�N�^�[(Actor*) </param>
	InputComponent(class Actor* owner);

	/// <summary>
	/// ���͗p�֐�
	/// </summary>
	/// <param name="keyState"> �L�[�{�[�h�̏�� </param>
	void ProcessInput(const uint8_t* keyState)override;

	// getter / setter
	float GetMaxForward() const { return mMaxForwardSpeed; }
	float GetMaxAngular() const { return mMaxAngularSpeed; }
	int GetForwardKey() const { return mForwardKey; }
	int GetBackKey() const { return mBackKey; }
	int GetClockwiseKey() { return mClockwiseKey; }
	int GetCounterClockwiseKey() { return mCounterClockwiseKey; }

	void SetMaxForwardSpeed(float speed) { mMaxForwardSpeed = speed; }
	void SetMaxAngularSpeed(float speed) { mMaxAngularSpeed = speed; }
	void SetForwardKey(int key) { mForwardKey = key; }
	void SetBackKey(int key) { mBackKey = key; }
	void SetClockwiseKey(int key) { mClockwiseKey = key; }
	void SetCounterClockwiseKey(int key) { mCounterClockwiseKey = key; }

private:
	// �O�i / ��]�̍ő呬�x
	float mMaxForwardSpeed;
	float mMaxAngularSpeed;

	// �O�i / ��ނ̂��߂̃L�[
	int mForwardKey;
	int mBackKey;

	// ��]�^���̂��߂̃L�[
	int mClockwiseKey;
	int mCounterClockwiseKey;
};