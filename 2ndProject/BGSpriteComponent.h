#pragma once
#include <vector>
#include "Math.h"
#include "SpriteComponent.h"

class BGSpriteComponent :public SpriteComponent
{
public:
	//背景ですので、描画順番の値を小さくして、一番後ろにする
	BGSpriteComponent(class Actor* owner,int drawOrder = 10);
	//継承先をオバーライド
	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer) override;
	//テキスチャーを設置
	void SetBGTextures(const std::vector<SDL_Texture*>& textures);
	//ゲット・セットウィンドウズサイズとスクロール速度
	void SetScreenSize(const Vector2& size) { mScreenSize = size; }
	void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
	float GetScrollSpeed() const { return mScrollSpeed; }
private:
	//構想体でオフセットとイメージを包み込む
	struct BGTexture
	{
		SDL_Texture* mTexture;
		Vector2 mOffset;
	};
	std::vector<BGTexture> mBGTextures;
	Vector2 mScreenSize;
	float mScrollSpeed;
};