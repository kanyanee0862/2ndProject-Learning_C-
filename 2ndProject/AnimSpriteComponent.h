#pragma once
#include<vector>
#include"SpriteComponent.h"

class AnimSpriteComponent :public SpriteComponent
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	//���t���[���A�j���[�V�����X�V
	virtual void Update(float deltaTime) override;
	//�e�L�X�`���[��ݒ�
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	//�A�j���[�V����FPS���Z�b�g�Q�b�g
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
private:
	//�S�ẴA�j���[�V�����̃e�L�X�`���[
	std::vector<SDL_Texture*> mAnimTextures;
	//���݂̃t���[��
	float mCurrFrame;
	//�A�j���[�V����FPS
	float mAnimFPS;
};