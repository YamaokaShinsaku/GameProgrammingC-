#pragma once
#include "Component.h"
#include <unordered_map>
#include <string>

class AIComponent : public Component
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner"> ���L�A�N�^�[(Actor*) </param>
	AIComponent(class Actor* owner);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="deltaTime"> �f���^�^�C�� </param>
	void Update(float deltaTime)override;
	/// <summary>
	/// �X�e�[�g�}�V�[���̑J��
	/// </summary>
	/// <param name="name"> �X�e�[�g�̖��O </param>
	void ChangeState(const std::string& name);

	/// <summary>
	/// �V���ȏ�Ԃ�A�z�z��ɓo�^����
	/// </summary>
	/// <param name="state"> ��� </param>
	void RegisterState(class AIState* state);

private:
	// ��Ԃ̖��O��AIState�̃C���X�^���X��Ή��t����
	std::unordered_map<std::string, class AIState*> mStateMap;
	// ���݂̏��
	class AIState* mCurrentState;

};

