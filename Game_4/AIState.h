#pragma once

class AIState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner"> ���L�A�N�^�[(AIComponent*) </param>
	AIState(class AIComponent* owner)
		:mOwner(owner)
	{ }

	// ��Ԃ��Ƃ̐U�镑��	
	/// <summary>
	/// ��Ԃ̍X�V
	/// </summary>
	/// <param name="deltaTime"> �f���^�^�C�� </param>
	virtual void Update(float deltaTime) = 0;
	/// <summary>
	/// �J�ڂɓ���
	/// </summary>
	virtual void OnEnter() = 0;
	/// <summary>
	/// �J�ڂ���o��
	/// </summary>
	virtual void OnExit() = 0;

	// ��Ԗ��̎擾
	/// <summary>
	/// ��Ԃ̖��O��Ԃ�
	/// </summary>
	/// <returns> ��Ԃ̖��O </returns>
	virtual const char* GetName() const = 0;

protected:
	class AIComponent* mOwner;
};

class AIPatrol : public AIState
{
public:
	AIPatrol(class AIComponent* owner) 
		: AIState(owner)
	{ }

	// �U�镑�����I�[�o�[���C�h����
	void Update(float deltaTime)override;
	void OnEnter()override;
	void OnExit()override;

	const char* GetName() const override { return "Patrol"; }
};

class AIDeath : public AIState
{
public:
	AIDeath(class AIComponent* owner)
		: AIState(owner)
	{ }

	// �U�镑�����I�[�o�[���C�h����
	void Update(float deltaTime)override;
	void OnEnter()override;
	void OnExit()override;

	const char* GetName() const override { return "Death"; }
};

class AIAttack : public AIState
{
public:
	AIAttack(class AIComponent* owner)
		: AIState(owner)
	{ }

	// �U�镑�����I�[�o�[���C�h����
	void Update(float deltaTime)override;
	void OnEnter()override;
	void OnExit()override;

	const char* GetName() const override { return "Attack"; }
};