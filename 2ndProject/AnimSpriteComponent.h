#pragma once
#include<vector>
#include"SpriteComponent.h"

class AnimSpriteComponent :public SpriteComponent
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	//毎フレームアニメーション更新
	virtual void Update(float deltaTime) override;
	//テキスチャーを設定
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	//アニメーションFPSをセットゲット
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
private:
	//全てのアニメーションのテキスチャー
	std::vector<SDL_Texture*> mAnimTextures;
	//現在のフレーム
	float mCurrFrame;
	//アニメーションFPS
	float mAnimFPS;
};