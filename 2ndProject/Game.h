#pragma once
#include <unordered_map>
#include<string>
#include<vector>
#include "SDL.h"
class Game
{
public:
	//コンストラクター
	Game();
	//初期化
	bool Initialize();
	//ゲームループ 
	void RunLoop();
	//Shutdown 処理
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	SDL_Texture* GetTexture(const std::string& fileName);
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, SDL_Texture*> mTextures;
	//全てのアクター
	std::vector<class Actor*> mActors;
	//未決アクター
	std::vector<class Actor*> mPendingActors;
	//スプライト
	std::vector<class SpriteComponent*> mSprites;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
	//アクターを更新しているかどうかを確認する
	bool mUpdatingActors;
	// Game-specific
	class Ship* mShip; // Player's ship
};