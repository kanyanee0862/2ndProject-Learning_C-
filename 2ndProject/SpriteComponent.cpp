#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
SpriteComponent::SpriteComponent(class Actor* owner, int drawOrder)
	:Component(owner)
	, mTexture(nullptr) //Texture�̏�����
	, mDrawOrder(drawOrder) //Draw����
	, mTexWidth(0) 
	, mTexHeight(0)
{
	mOwner->GetGame()->AddSprite(this);
}
//�f�X�g���N�^�[
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
		//���W�A������x�ɕϊ����āA�����v���ɂ���
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
	//���E�������擾����
	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}