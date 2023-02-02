#pragma once
#include "MoveComponent.h"
#include "Math.h"

class NavComponent : public MoveComponent
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner"> 所有アクター(Acotr*) </param>
	/// <param name="updateOrder"> 更新順序 </param>
	/// 更新順序が小さいほど早く更新される
	NavComponent(class Actor* owner, int updateOrder = 10);

	void Update(float deltaTime)override;
	void StartPath(const class Tile* start);
	void TurnTo(const Vector2& pos);

private:
	const class Tile* mNextNode;
};

