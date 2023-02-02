#pragma once
#include "Component.h"
#include "Math.h"

/// <summary>
/// 衝突検知をする
/// </summary>
class CircleComponent : public Component
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner"> 所有アクター(Actor*) </param>
	CircleComponent(class Actor* owner);

	// getter / setter
	void SetRadius(float radius) { mRadius = radius; }
	float GetRadius() const;

	const Vector2& GetCenter() const;

private:
	float mRadius;
};

// CircleComponentを参照の形で受け取る
bool Intersect(const CircleComponent& a, const CircleComponent& b);