#pragma once
#include "Component.h"
#include "Math.h"

/// <summary>
/// �Փˌ��m������
/// </summary>
class CircleComponent : public Component
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner"> ���L�A�N�^�[(Actor*) </param>
	CircleComponent(class Actor* owner);

	// getter / setter
	void SetRadius(float radius) { mRadius = radius; }
	float GetRadius() const;

	const Vector2& GetCenter() const;

private:
	float mRadius;
};

// CircleComponent���Q�Ƃ̌`�Ŏ󂯎��
bool Intersect(const CircleComponent& a, const CircleComponent& b);