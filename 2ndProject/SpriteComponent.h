#pragma once
#include "SDL.h"
#include "Component.h"
class SpriteComponent :public Component //�R���|�[�l���g���p������
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
	//�e�L�X�`���[
	SDL_Texture* mTexture;
	//painter's algorithm�̂��߂̕`�揇��
	int mDrawOrder;
	//�e�L�X�`���[�̕��ƍ���
	int mTexWidth;
	int mTexHeight;
};