#pragma once
#include <vector>
#include "Math.h"
#include "SpriteComponent.h"

class BGSpriteComponent :public SpriteComponent
{
public:
	//�w�i�ł��̂ŁA�`�揇�Ԃ̒l�����������āA��Ԍ��ɂ���
	BGSpriteComponent(class Actor* owner,int drawOrder = 10);
	//�p������I�o�[���C�h
	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer) override;
	//�e�L�X�`���[��ݒu
	void SetBGTextures(const std::vector<SDL_Texture*>& textures);
	//�Q�b�g�E�Z�b�g�E�B���h�E�Y�T�C�Y�ƃX�N���[�����x
	void SetScreenSize(const Vector2& size) { mScreenSize = size; }
	void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
	float GetScrollSpeed() const { return mScrollSpeed; }
private:
	//�\�z�̂ŃI�t�Z�b�g�ƃC���[�W���ݍ���
	struct BGTexture
	{
		SDL_Texture* mTexture;
		Vector2 mOffset;
	};
	std::vector<BGTexture> mBGTextures;
	Vector2 mScreenSize;
	float mScrollSpeed;
};