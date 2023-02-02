#pragma once
#include "MoveComponent.h"
#include "Math.h"

class NavComponent : public MoveComponent
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner"> ���L�A�N�^�[(Acotr*) </param>
	/// <param name="updateOrder"> �X�V���� </param>
	/// �X�V�������������قǑ����X�V�����
	NavComponent(class Actor* owner, int updateOrder = 10);

	void Update(float deltaTime)override;
	void StartPath(const class Tile* start);
	void TurnTo(const Vector2& pos);

private:
	const class Tile* mNextNode;
};
