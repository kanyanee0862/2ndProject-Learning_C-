#include "AnimSpriteComponent.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(24.0f) //most game use 24 FPS for anim
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	if (mAnimTextures.size() > 0)
	{
		//FPSとdeltaTimeより現在のフレームを更新する
		mCurrFrame += mAnimFPS * deltaTime;
		
		while (mCurrFrame >= mAnimTextures.size())
		{
			mCurrFrame -= mAnimTextures.size();
		}

		//現在のテキスチャーを設置
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
	}
	
}
void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0)
	{
		mCurrFrame = 0.0f;
		SetTexture(mAnimTextures[0]);
	}
}