#include "Game.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "SDL/SDL_image.h"
#include <algorithm>
#include "Random.h"
#include "AIState.h"
#include "AIComponent.h"
#include "Grid.h"
#include "Enemy.h"

Game::Game()
	: mWindow(nullptr)
	, mIsRunning(true)
	, mRenderer(nullptr)
	, mUpdatingActors(false)
	, mTicksCount(0)
{
}

bool Game::Initialize()
{
	// SDL���C�u����������������
	// SDL_Init �̕Ԃ�l���O�łȂ���Ώ������Ɏ��s
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	// �������Ɏ��s�����Ƃ�
	if (sdlResult != 0)
	{
		// ���O��\��
		SDL_Log("SDL���������ł��܂��� : %s", SDL_GetError());

		return false;
	}

	// �E�B���h�E�쐬
	mWindow = SDL_CreateWindow(
		"OpenGL_Game",	// �E�B���h�E�̃^�C�g��
		100,			// �E�B���h�E�������x���W
		100,			// �E�B���h�E�������y���W
		1024,			// �E�B���h�E�̕�(width)
		768,			// �E�B���h�E�̍���(height)
		0				// �t���O(�ݒ肵�Ȃ��Ƃ��͂O)
	);
	// �E�B���h�E�̍쐬�Ɏ��s�����Ƃ�
	if (!mWindow)
	{
		// ���O��\��
		SDL_Log("�E�B���h�E�̍쐬�Ɏ��s���܂��� : %s", SDL_GetError());
		return false;
	}

	// SDL�����_���[�쐬
	mRenderer = SDL_CreateRenderer(
		mWindow,	// �쐬���郌���_���[�̕`��ΏۂƂȂ�E�B���h�E
		-1,			// �ʏ�� -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	// SDL�����_���[�̍쐬�Ɏ��s�����Ƃ�
	if (!mRenderer)
	{
		// ���O��\��
		SDL_Log("�����_���[�̏������Ɏ��s���܂��� : %s", SDL_GetError());
		return false;
	}

	// �摜(png)�̐����Ɏ��s�����Ƃ�
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		// ���O��\��
		SDL_Log("�摜�̏������Ɏ��s���܂��� : %s", SDL_GetError());
		return false;
	}

	Random::Init();
	LoadData();
	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::Shutdown()
{
	UnloadData();
	IMG_Quit();
	// SDL_Window��j��
	SDL_DestroyWindow(mWindow);
	// SDL_Renderer��j��
	SDL_DestroyRenderer(mRenderer);
	// SDL�I��
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	// �A�N�^�[�̍X�V���Ȃ� mPendingActors �ɒǉ�
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// mPendingActors ���ǂ���
	auto iter = std::find(mPendingActors.begin(), 
						  mPendingActors.end(),
						  actor);
	if (iter != mPendingActors.end())
	{
		// vector�̍Ō�ɃX���b�v���č폜����
		// ���R�s�[���폜���Ȃ��悤�ɒ���
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// mActors ���ǂ���
	iter = std::find(mActors.begin(),
					 mActors.end(),
					 actor);
	if (iter != mActors.end())
	{
		// vector�̍Ō�ɃX���b�v���č폜����
		// ���R�s�[���폜���Ȃ��悤�ɒ���
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}

}

void Game::AddSprite(SpriteComponent* sprite)
{
	// �\�[�g�ς݂̔z��ő}���_��������
	// (������菇���̍����ŏ��̗v�f�̈ʒu)
	int mDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		iter != mSprites.end();
		++iter)
	{
		if (mDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// �C�e���[�^�[�̈ʒu�̑O�ɗv�f��}������
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	// �����������̂ŁA�X���b�v�͂ł��Ȃ�
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	// �e�N�X�`�����}�b�v�ɏ�������Ă��邩�ǂ���
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// �t�@�C�����烍�[�h
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		// �e�N�X�`���t�@�C���̃��[�h�Ɏ��s�����Ƃ�
		if (!surf)
		{
			// ���O�\��
			SDL_Log("�e�N�X�`���̃��[�h�Ɏ��s���܂��� : %s", fileName.c_str());
			return nullptr;
		}

		// �T�[�t�F�X����e�N�X�`�����쐬
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		// �e�N�X�`���ւ̕ϊ��Ɏ��s
		if (!tex)
		{
			// ���O��\��
			SDL_Log("�e�N�X�`���ւ̕ϊ��Ɏ��s���܂��� : %s", fileName.c_str());
			return nullptr;
		}

		mTextures.emplace(fileName.c_str(), tex);
	}

	return tex;
}

Enemy* Game::GetNearestEnemy(const Vector2& pos)
{
	Enemy* best = nullptr;

	if (mEnemies.size() > 0)
	{
		best = mEnemies[0];

		float bestDistSq = (pos - mEnemies[0]->GetPosition()).LengthSq();
		for (size_t i = 1; i < mEnemies.size(); i++)
		{
			float newDistSq = (pos - mEnemies[i]->GetPosition()).LengthSq();
			if (newDistSq < bestDistSq)
			{
				bestDistSq = newDistSq;
				best = mEnemies[i];
			}
		}
	}

	return best;
}

void Game::ProcessInput()
{
	SDL_Event event;
	// �L���[�ɃC�x���g������ΌJ��Ԃ�
	while (SDL_PollEvent(&event))
	{
		// �C�x���g����
		switch (event.type)
		{
			// SDL_QUIT�C�x���g�Ȃ�A���[�v�I��
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// �L�[�{�[�h�̏�Ԃ��擾����
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	// Esc�L�[��������Ă��Ă��A���[�v���I��
	if (keyState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	//--------------------------------
	if (keyState[SDL_SCANCODE_B])
	{
		mGrid->BuildTower();
	}

	int x, y;
	Uint32 buttons = SDL_GetMouseState(&x, &y);
	if (SDL_BUTTON(buttons) & SDL_BUTTON_LEFT)
	{
		mGrid->ProcessClick(x, y);
	}
	//---------------------------------

	// �S�A�N�^�[��ProcessInput���Ăяo��
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(keyState);
	}
	mUpdatingActors = false;
}

void Game::UpdateGame()
{
	// ���̃t���[���܂�16ms�o�߂���܂ő҂�
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// deltaTime...�O�t���[���Ƃ̎��Ԃ̍���b�ɕϊ������l
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	// �f���^�^�C�����ő�l�Ő���
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	// ���Ԃ��X�V(���̃t���[���̂���)
	mTicksCount = SDL_GetTicks();

	// �S�ẴA�N�^�[���X�V
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// �҂��ɂȂ��Ă����A�N�^�[�� mActors �Ɉړ�
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// ���񂾃A�N�^�[���ꎞ�z��ɒǉ�
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// ���񂾃A�N�^�[���폜����(mActors����폜�����)
	for (auto actor : deadActors)
	{
		delete actor;
	}

}

void Game::GenerateOutput()
{
	// �`��F���w��
	SDL_SetRenderDrawColor(
		mRenderer,
		34,		// R
		139,		// G
		34,		// B
		255		// A(�A���t�@�l)
	);
	// �o�b�N�o�b�t�@�����݂̕`��F�ŃN���A
	SDL_RenderClear(mRenderer);

	// �S�Ă�SpriteComponents��`�悷��
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}

	// �t�����g�o�b�t�@�ƃo�b�N�o�b�t�@����������
	SDL_RenderPresent(mRenderer);
}

void Game::LoadData()
{
	mGrid = new Grid(this);

	//// test
	//Actor* a = new Actor(this);
	//// AIComponent���쐬
	//AIComponent* aic = new AIComponent(a);
	//// ��Ԃ�AIComponent�ɓo�^
	//aic->RegisterState(new AIPatrol(aic));
	//aic->RegisterState(new AIDeath(aic));
	//aic->RegisterState(new AIAttack(aic));
	//// AIComponent��Patrol�ŏ�����
	//aic->ChangeState("Patrol");
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy textures
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		// �w���p�[�֐����Ăяo��
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}
