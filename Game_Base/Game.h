#pragma once
#include "SDL/SDL.h"


/// <summary>
/// Game �N���X
/// </summary>
class Game
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Game();
	/// <summary>
	/// �Q�[��������������
	/// </summary>
	/// <returns> �������ɐ��������� true/���s������ false </returns>
	bool Initialize();
	/// <summary>
	/// �Q�[���I�[�o�[�܂ŃQ�[�����[�v�����s����
	/// </summary>
	void RunLoop();
	/// <summary>
	/// �Q�[�����V���b�g�_�E������
	/// </summary>
	void Shutdown();

private:
	//�Q�[�����[�v�̂��߂̃w���p�[�֐��Q
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// SDL�����E�B���h�E
	SDL_Window* mWindow;
	// 2D�`��
	SDL_Renderer* mRenderer;
	// �Q�[���J�n����̌o�ߎ���
	Uint32 mTicksCount;
	// �Q�[���̑��s���w������
	bool mIsRunning;

};