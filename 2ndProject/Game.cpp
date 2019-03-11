#include<algorithm>
#include "SDL_image.h"
#include "Game.h"
#include "Actor.h"
#include "Ship.h"
#include "SpriteComponent.h"
#include "BGSpriteComponent.h"

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mIsRunning(true)
,mUpdatingActors(false)
{
}
bool Game::Initialize()
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO )!= 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow(
		"Shooting Game", //title
		100, //x
		100, //y
		1024, //w
		768, //h
		0); //flags
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	mRenderer = SDL_CreateRenderer(mWindow, 
									-1, 
									SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
								  ); //�E�B���h�E��2D�����_�����O�R���e�L�X�g�𐶐�����
	//2D�����_�����O�R���e�L�X�g���m�F����
	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	LoadData();

	mTicksCount = SDL_GetTicks();

	return true;
}
void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
 		GenerateOutput();
	}
}
void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}
	//ESC�L�[�����ꂽ��AWindows�߂�
	const uint8_t* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	//Keys�C���v�b�g
	mShip->ProcessKeyboard(state);
}
void Game::UpdateGame()
{
	//���Ԃ��J�n
	//16�~���b�o�߂�����n�܂�
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	//�o��l����S�čX�V
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;
	
	//�����o��l�����ړ�����
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	//���񂾓o��l�������x�N�g���ɒǉ�
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}
	//���񂾓o��l�����폜����
	for (auto actor : deadActors)
	{
		delete actor;
	}
}
void Game::GenerateOutput()
{
	//windows�̐F
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	//�X�v���C�g��`�悷��
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}
	
	SDL_RenderPresent(mRenderer);
}
void Game::LoadData()
{
	// Create player's ship
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(100.0f, 384.0f));
	mShip->SetScale(1.5f);

	// Create actor for the background (this doesn't need a subclass)
	Actor* temp = new Actor(this);
	temp->SetPosition(Vector2(512.0f, 384.0f));
	// Create the "far back" background
	BGSpriteComponent* bg = new BGSpriteComponent(temp);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	std::vector<SDL_Texture*> bgtexs = {
		GetTexture("Assets/NebulaBlue.png"),
		GetTexture("Assets/NebulaBlue.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-100.0f);
	// Create the closer background
	bg = new BGSpriteComponent(temp, 20);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	bgtexs = {
		GetTexture("Assets/Stars-Big1.png"),
		GetTexture("Assets/Stars-Big1.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-200.0f);
}
void Game::UnloadData()
{
	// �o��l�����폜
	// ~Actor�֐��ł�RemoveActor���Ăяo���Ă���̂ŁA�ʂ̃��[�v�ɂ���
	while (!mActors.empty())
	{
		//�����v�f�ւ̎Q�Ƃ��擾���āA�폜����
		delete mActors.back();
	}
	//�e�L�X�`���[���폜
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second); //second store value
	}
	mTextures.clear();
}
SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	// Is the texture already in the map?
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// Load from file
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log(SDL_GetError());
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		//�T�[�t�F�C�X����e�N�X�`���𐶐�����
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);//�����_�����O�R���e�L�X�g,
															//�e�N�X�`���Ŏg���s�N�Z���f�[�^������SDL_Surface
		//RGB�T�[�t�F�C�X���������
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log(SDL_GetError());
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

		mTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}
void Game::Shutdown()
{
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}
void Game::AddActor(Actor* actor)
{
	//�X�V������A�����ɒǉ�����
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
	mPendingActors.clear();
	
}
void Game::RemoveActor(Actor* actor)
{
	//****
	//*[first,last) ���̃C�e���[�^ i �ɂ��āA
	//*i == value �ł���悤�ȍŏ��̃C�e���[�^��Ԃ��B
	//*���̂悤�ȃC�e���[�^��������Ȃ������ꍇ�� last ��Ԃ��B
	//****

	//�����o��l���H
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if(iter != mPendingActors.end())
	{
		// �Ō�ɓ���ւ��Apop back������i�R�s�[���폜�����Ȃ����߁j
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	//�o��l���ł����H
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// �Ō�ɓ���ւ��Apop back������i�R�s�[���폜���Ȃ��悤�j
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}
void Game::AddSprite(class SpriteComponent* sprite)
{
	//���Ԃɕ`�悷��
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	//�C�e���[�^�ɓ���
	mSprites.insert(iter, sprite);
}
void Game::RemoveSprite(class SpriteComponent* sprite)
{
	// (can't swap because it ruins ordering)
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}