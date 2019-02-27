#pragma once
#include "SDL.h"
#include "Component.h"
class SpriteComponent :public Component //コンポーネントを継承する
{
public:
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(SDL_Renderer* renderer);
	virtual void SetTexture(SDL_Texture* texture);
	
	int GetDrawOrder()const { return mDrawOrder; }
	int GetTexHeight()const { return mTexHeight; }
	int GetTexWidth()const { return mTexWidth; }
protected:
	//テキスチャー
	SDL_Texture* mTexture;
	//painter's algorithmのための描画順番
	int mDrawOrder;
	//テキスチャーの幅と高さ
	int mTexWidth;
	int mTexHeight;
};