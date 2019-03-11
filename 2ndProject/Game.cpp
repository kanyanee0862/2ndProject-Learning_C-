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
								  ); //ウィンドウの2Dレンダリングコンテキストを生成する
	//2Dレンダリングコンテキストを確認する
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
	//ESCキー押されたら、Windows閉める
	const uint8_t* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	//Keysインプット
	mShip->ProcessKeyboard(state);
}
void Game::UpdateGame()
{
	//時間を開始
	//16ミリ秒経過したら始まる
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	//登場人物を全て更新
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;
	
	//未決登場人物を移動する
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	//死んだ登場人物を仮ベクトルに追加
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}
	//死んだ登場人物を削除する
	for (auto actor : deadActors)
	{
		delete actor;
	}
}
void Game::GenerateOutput()
{
	//windowsの色
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	//スプライトを描画する
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
	// 登場人物を削除
	// ~Actor関数ではRemoveActorを呼び出しているので、別のループにする
	while (!mActors.empty())
	{
		//末尾要素への参照を取得して、削除する
		delete mActors.back();
	}
	//テキスチャーを削除
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

		//サーフェイスからテクスチャを生成する
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);//レンダリングコンテキスト,
															//テクスチャで使うピクセルデータを持つSDL_Surface
		//RGBサーフェイスを解放する
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
	//更新したら、未決に追加する
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
	//*[first,last) 内のイテレータ i について、
	//*i == value であるような最初のイテレータを返す。
	//*そのようなイテレータが見つからなかった場合は last を返す。
	//****

	//未決登場人物？
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if(iter != mPendingActors.end())
	{
		// 最後に入れ替え、pop backをする（コピーを削除させないため）
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	//登場人物ですか？
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// 最後に入れ替え、pop backをする（コピーを削除しないよう）
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}
void Game::AddSprite(class SpriteComponent* sprite)
{
	//順番に描画する
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	//イテレータに入力
	mSprites.insert(iter, sprite);
}
void Game::RemoveSprite(class SpriteComponent* sprite)
{
	// (can't swap because it ruins ordering)
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}