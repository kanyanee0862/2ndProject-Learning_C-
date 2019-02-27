#include <algorithm>
#include "Actor.h"
#include "Game.h"
#include "Component.h"
//---------------------------------------------------------------------------------
Actor::Actor(class Game* game)
	:mState(EActive),
	mPosition(Vector2::Zero),
	mScale(1.0f),
	mRotation(0.0f),
	mGame(game)
{
	mGame->AddActor(this);
}
Actor::~Actor()
{
	mGame->RemoveActor(this);
	//new‚ðŽg‚Á‚½Œãdelete‚µ‚È‚¢‚Æƒƒ‚ƒŠ[ã‚ÉŽc‚³‚ê‚Ä‚µ‚Ü‚¤
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}
void Actor::Update(float deltaTime)
{
	//¶‚«‚Ä‚¢‚é‚Æ‚«‚¾‚¯
	if (mState == EActive)
	{
		UpdateActor(deltaTime);
		UpdateComponents(deltaTime);
	}
}
void Actor::UpdateComponents(float deltaTime)
{
	for (auto comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}
void Actor::UpdateActor(float deltaTime)
{
}
void Actor::AddComponent(Component* component)
{
	// Find the insertion point in the sorted vector
	// (The first element with a order higher than me)
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (;
		iter != mComponents.end();
		++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	// Inserts component before position of iterator
	mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component)
{
	//****
	//*	template <class InputIterator, class T>
	//*	InputIterator find(InputIterator first, InputIterator last, const T& val);
	//*	Find value in range
	//*	Returns an iterator to the first element in the range[first, last) 
	//*	that compares equal to val.If no such element is found, the function returns last.
	//****

	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}