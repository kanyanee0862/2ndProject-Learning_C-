#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
SpriteComponent::SpriteComponent(class Actor* owner, int drawOrder)
	:Component(owner)
	, mTexture(nullptr) //Textureの初期化
	, mDrawOrder(drawOrder) //Draw順位
	, mTexWidth(0) 
	, mTexHeight(0)
{
	mOwner->GetGame()->AddSprite(this);
}
//デストラクター
SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->RemoveSprite(this);
}
void SpriteComponent::Draw(SDL_Renderer* renderer){
	if (mTexture)
	{
		SDL_Rect r;
		r.w = static_cast<int>(mTexWidth*mOwner->GetScale());
		r.h = static_cast<int>(mTexHeight*mOwner->GetScale());
		r.x = static_cast<int>(mOwner->GetPosition().x-r.w/2);
		r.y = static_cast<int>(mOwner->GetPosition().y-r.h/2);
		//ラジアンから度に変換して、反時計回りにする
		(
			renderer,
			mTexture,
			nullptr,
			&r,
			-Math::ToDegrees(mOwner->GetRotation()),
			nullptr,
			SDL_FLIP_NONE
		);
	}
}
void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	mTexture = texture;
	//幅・高さを取得する
	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}