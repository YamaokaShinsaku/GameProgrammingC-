#pragma once

/// <summary>
/// Component�N���X
/// </summary>
class Component
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner"> ���L��(Actor*) </param>
	/// <param name="updateOrder"> �X�V���� </param>
	/// updateOrder���������R���|�[�l���g�قǑ����X�V�����
	Component(class Actor* owner, int updateOrder = 100);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Component();

	/// <summary>
	/// �R���|�[�l���g���f���^�^�C���ōX�V
	/// </summary>
	/// <param name="deltaTime"> �f���^�^�C�� </param>
	virtual void Update(float deltaTime);

	// getter / setter
	int GetUpdateOrder() const { return mUpdateOrder; }

protected:
	class Actor* mOwner;	// ���L�A�N�^�[
	int mUpdateOrder;		// �R���|�[�l���g�̍X�V����
};