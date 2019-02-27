#pragma once
#include <unordered_map>
#include<string>
#include<vector>
#include "SDL.h"
class Game
{
public:
	//�R���X�g���N�^�[
	Game();
	//������
	bool Initialize();
	//�Q�[�����[�v 
	void RunLoop();
	//Shutdown ����
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
	//�S�ẴA�N�^�[
	std::vector<class Actor*> mActors;
	//�����A�N�^�[
	std::vector<class Actor*> mPendingActors;
	//�X�v���C�g
	std::vector<class SpriteComponent*> mSprites;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
	//�A�N�^�[���X�V���Ă��邩�ǂ������m�F����
	bool mUpdatingActors;
	// Game-specific
	class Ship* mShip; // Player's ship
};