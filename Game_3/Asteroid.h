#pragma once
#include "Actor.h"

/// <summary>
/// ���f���N���X
/// </summary>
class Asteroid : public Actor
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="game"> Game* </param>
	Asteroid(class Game* game);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Asteroid();

	// getter / setter
	class CircleComponent* GetCircle() { return mCircle; }

private:
	class CircleComponent* mCircle;
};
