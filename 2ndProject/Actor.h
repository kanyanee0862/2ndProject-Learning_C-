#pragma once
#include<vector>
#include"Math.h"

class Actor
{
public:
	//アクターの状態をチェック
	enum State
	{
		EActive,
		EPaused,
		EDead
	};
	
	//コンストラクター・デストラクター
	Actor(class Game* game);
	virtual ~Actor();

	//ゲームからの更新
	void Update(float deltaTime);
	//アクターに追加されたコンポーネント
	void UpdateComponents(float deltaTime);
	//アクターの更新
	virtual void UpdateActor(float deltaTime);

	//Getter Setter
	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }
	
	//**
	//*Declarations in a nested class can use 
	//*any members of the enclosing class, 
	//*following the usual usage rules for the non-static members
	//*can access to all member in enclosing class
	//**
	class Game* GetGame() { return mGame; } //Declared nested class 


	//コンポネントを追加・削除
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);
private:
	//アクターの状態
	State mState;
	//Transition
	Vector2 mPosition;
	//極大
	float mScale;
	//回転
	float mRotation;

	std::vector<class Component*> mComponents;
	//Declared nested class
	class Game* mGame;
};