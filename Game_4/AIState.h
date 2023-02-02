#pragma once

class AIState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner"> 所有アクター(AIComponent*) </param>
	AIState(class AIComponent* owner)
		:mOwner(owner)
	{ }

	// 状態ごとの振る舞い	
	/// <summary>
	/// 状態の更新
	/// </summary>
	/// <param name="deltaTime"> デルタタイム </param>
	virtual void Update(float deltaTime) = 0;
	/// <summary>
	/// 遷移に入る
	/// </summary>
	virtual void OnEnter() = 0;
	/// <summary>
	/// 遷移から出る
	/// </summary>
	virtual void OnExit() = 0;

	// 状態名の取得
	/// <summary>
	/// 状態の名前を返す
	/// </summary>
	/// <returns> 状態の名前 </returns>
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

	// 振る舞いをオーバーライドする
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

	// 振る舞いをオーバーライドする
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

	// 振る舞いをオーバーライドする
	void Update(float deltaTime)override;
	void OnEnter()override;
	void OnExit()override;

	const char* GetName() const override { return "Attack"; }
};