#include "Game.h"


Game::Game()
	:mWindow(nullptr)
	, mIsRunning(true)
	, mRenderer(nullptr)
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

	return true;
}

void Game::Shutdown()
{
	// SDL_Window��j��
	SDL_DestroyWindow(mWindow);
	// SDL_Renderer��j��
	SDL_DestroyRenderer(mRenderer);
	// SDL�I��
	SDL_Quit();
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
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// Esc�L�[��������Ă��Ă��A���[�v���I��
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

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

}

void Game::GenerateOutput()
{
	// �`��F���w��
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G
		255,	// B
		255		// A(�A���t�@�l)
	);
	// �o�b�N�o�b�t�@�����݂̕`��F�ŃN���A
	SDL_RenderClear(mRenderer);


	// �t�����g�o�b�t�@�ƃo�b�N�o�b�t�@����������
	SDL_RenderPresent(mRenderer);
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
