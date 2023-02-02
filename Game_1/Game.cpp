#include "Game.h"

const int thickness = 15;	// �����`�`��p
const float paddleH = 100.0f;

Game::Game()
	:mWindow(nullptr)
	,mIsRunning(true)
	,mRenderer(nullptr)
	,mTicksCount(0)
	,mPaddleDir(0)
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

	// 
	mPaddlePos.x = 10.0f;
	mPaddlePos.y = 768.0f / 2.0f;
	mBallPos.x = 1024.0f / 2.0f;
	mBallPos.y = 768.0f / 2.0f;
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;

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

	// �p�h���̈ړ������X�V W/S�L�[
	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddleDir += 1;
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

	// �p�h�����W���f���^�^�C���Ɋ�Â��čX�V
	if ( mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
		// �p�h������ʂ���o�Ȃ��悤�ɂ���
		if (mPaddlePos.y < (paddleH / 2.0f + thickness))
		{
			mPaddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	// �{�[�����W�𑬓x�Ɋ�Â��čX�V
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	// �o�E���h
	// �{�[�����p�h���Ɠ���������
	float diff = mPaddlePos.y - mBallPos.y;
	//
	diff = (diff > 0.0f) ? diff : -diff;
	if (
		// ���̍����\���ɏ�����
		diff <= paddleH / 2.0f &&
		// �{�[�������������ʒu�ɂ���
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		// �{�[�����������ɓ����Ă����
		mBallVel.x < 0.0f
		)
	{
		mBallVel.x *= -1.0f;
	}

	// �{�[�����ǂɏՓ˂�����
	// �㕔
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
	}
	// ����
	else if (mBallPos.y >= 768 - thickness && mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1;
	}
	// �E
	else if (mBallPos.x >= 1024.0f - thickness && mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1;
	}
	
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

	// �����`�̕`��
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	// �ǂ�`��
	// �㕔
	SDL_Rect wall{
		0,			// ������̂�
		0,			// ������̂�
		1024,		// ��
		thickness	// ����
	};
	SDL_RenderFillRect(mRenderer, &wall);

	// ��
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	// �E
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;
	SDL_RenderFillRect(mRenderer, &wall);


	// �p�h���`��
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH / 2),
		thickness,
		paddleH
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	// �{�[���`��
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

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
