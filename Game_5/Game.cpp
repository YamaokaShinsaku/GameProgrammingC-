#include "Game.h"
#include <GL/glew.h>
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Shader.h"
#include "Texture.h"

//--------------------------
#include "Asteroid.h"
#include "Ship.h"
#include "Random.h"
#include "VertexArray.h"

Game::Game()
	: mWindow(nullptr)
	, mIsRunning(true)
	, mSpriteShader(nullptr)
	, mUpdatingActors(false)
	, mTicksCount(0)
{
}

bool Game::Initialize()
{
	// SDL������������
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		// ���O�\��
		SDL_Log("Unable to Initialize SDL : %s", SDL_GetError());

		return false;
	}

	// OpenGL�E�B���h�E�̑������w��@�����@�O/ ���s�@-�̒l
	// �R�A OpenGL �v���t�@�C�����g��
	SDL_GL_SetAttribute(
		SDL_GL_CONTEXT_PROFILE_MASK,	// �ݒ肷�鑮��
		SDL_GL_CONTEXT_PROFILE_CORE		// ���̑����̒l
	);

	// �o�[�W����3.3���w��
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// RGBA �e�`�����l��8�r�b�g�̃J���[�o�b�t�@���g��
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	// �_�u���o�b�t�@��L���ɂ���
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// �n�[�h�E�F�A�A�N�Z�����[�V�������g��
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// �E�B���h�E�쐬
	mWindow = SDL_CreateWindow(
		"OpenGL_Game",		// �E�B���h�E�̃^�C�g��
		100,				// �E�B���h�E�������x���W
		100,				// �E�B���h�E�������y���W
		1024,				// �E�B���h�E�̕�(width)
		768,				// �E�B���h�E�̍���(height)
		SDL_WINDOW_OPENGL	// �t���O(�ݒ肵�Ȃ��Ƃ��͂O)
	);
	// �E�B���h�E�̍쐬�Ɏ��s�����Ƃ�
	if (!mWindow)
	{
		// ���O��\��
		SDL_Log("Failed to create window : %s", SDL_GetError());
		return false;
	}

	// OpenGL �R���e�N�X�g���쐬
	mContext = SDL_GL_CreateContext(mWindow);

	// GLEW ��������
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		// ���O�\��
		SDL_Log("Failed to Initialize GLEW ");
		return false;
	}

	// �ꕔ�̃v���b�g�t�H�[���ł� GLEW �����Q�ȃG���[�R�[�h���o�̂�
	// ������N���A����
	glGetError();

	// �V�F�[�_�[���Ăяo��
	if (!LoadShader())
	{
		// ���O�\��
		SDL_Log("Failed to load shaders");
		return false;
	}

	Random::Init();
	CreateSpriteVerts();
	LoadData();
	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::Shutdown()
{
	UnloadData();
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	// SDL_GLContext ��j��
	SDL_GL_DeleteContext(mContext);
	// SDL_Window��j��
	SDL_DestroyWindow(mWindow);
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

Texture* Game::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	// �e�N�X�`�����}�b�v�ɏ�������Ă��邩�ǂ���
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// �t�@�C�����烍�[�h
		tex = new Texture();
		if (tex->Load(fileName))
		{
			mTextures.emplace(fileName.c_str(), tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}

	}

	return tex;
}

//----------------------------------------------
void Game::AddAsteroid(Asteroid* ast)
{
	mAsteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast)
{
	auto iter = std::find(mAsteroids.begin(),
		mAsteroids.end(), ast);
	if (iter != mAsteroids.end())
	{
		mAsteroids.erase(iter);
	}
}
//----------------------------------------------

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
		pending->ComputeWorldTransform();
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
	// �N���A�J���[��ݒ�
	glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
	// �J���[�o�b�t�@���N���A
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// �X�v���C�g�̃V�F�[�_�[�ƒ��_�z��̃I�u�W�F�N�g���A�N�e�B�u��
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();

	// �S�ẴX�v���C�g��`��
	for (auto sprite : mSprites)
	{
		sprite->Draw(mSpriteShader);
	}

	// �o�b�t�@������(�V�[����\��)
	SDL_GL_SwapWindow(mWindow);
}

bool Game::LoadShader()
{
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}

	mSpriteShader->SetActive();

	// �r���[�ˉe�s����쐬���A�ݒ肷��
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(1024.f, 768.f);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	return true;
}

void Game::CreateSpriteVerts()
{
	float vertices[] = {
		-0.5f,  0.5f, 0.f, 0.f, 0.f, // ���_0  ����
		 0.5f,  0.5f, 0.f, 1.f, 0.f, // ���_1  �E��
		 0.5f, -0.5f, 0.f, 1.f, 1.f, // ���_2  �E��
		-0.5f, -0.5f, 0.f, 0.f, 1.f  // ���_3  ����
	};

	unsigned int indices[] = {
		0,1,2, // ��ڂ̎O�p�` (���_1,2,3)
		2,3,0  // ��ڂ̎O�p�` (���_2,3,0)
	};

	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

void Game::LoadData()
{
	mShip = new Ship(this);
	//mShip->SetPosition(Vector2(512.0f, 384.0f));
	mShip->SetRotation(Math::PiOver2);

	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; i++)
	{
		new Asteroid(this);
	}
}

void Game::UnloadData()
{
	// �A�N�^�[���폜
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// �e�N�X�`���\���폜
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
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
