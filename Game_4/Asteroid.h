#pragma once
#include "Actor.h"

/// <summary>
/// 小惑星クラス
/// </summary>
class Asteroid : public Actor
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="game"> Game* </param>
	Asteroid(class Game* game);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Asteroid();

	// getter / setter
	class CircleComponent* GetCircle() { return mCircle; }

private:
	class CircleComponent* mCircle;
};
