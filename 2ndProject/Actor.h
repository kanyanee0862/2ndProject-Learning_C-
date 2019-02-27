#pragma once
#include<vector>
#include"Math.h"

class Actor
{
public:
	//�A�N�^�[�̏�Ԃ��`�F�b�N
	enum State
	{
		EActive,
		EPaused,
		EDead
	};
	
	//�R���X�g���N�^�[�E�f�X�g���N�^�[
	Actor(class Game* game);
	virtual ~Actor();

	//�Q�[������̍X�V
	void Update(float deltaTime);
	//�A�N�^�[�ɒǉ����ꂽ�R���|�[�l���g
	void UpdateComponents(float deltaTime);
	//�A�N�^�[�̍X�V
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


	//�R���|�l���g��ǉ��E�폜
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);
private:
	//�A�N�^�[�̏��
	State mState;
	//Transition
	Vector2 mPosition;
	//�ɑ�
	float mScale;
	//��]
	float mRotation;

	std::vector<class Component*> mComponents;
	//Declared nested class
	class Game* mGame;
};