#pragma once
#include "SDL/SDL.h"
#include <vector>
#include <string>
#include <unordered_map>
#include "Math.h"

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

	/// <summary>
	/// �Ώۂ̃A�N�^�[��ǉ�����
	/// </summary>
	/// <param name="actor"> Actor�|�C���^ </param>
	/// �󂯂Ƃ����A�N�^�[���AmPendingActors �� mActors�ɒǉ�
	/// �ǂ���ɒǉ����邩��mUpdatingActors�Ɉˑ�����
	void AddActor(class Actor* actor);
	/// <summary>
	/// �Ώۂ̃A�N�^�[���폜����
	/// </summary>
	/// <param name="actor"> Actor�|�C���^ </param>
	/// �Ώۂ́AmActors �� mPendingActors �̂ǂ��炩�ɂ���
	void RemoveActor(class Actor* actor);

	/// <summary>
	/// �X�v���C�g�̒ǉ�
	/// </summary>
	/// <param name="sprite"> Sprite�|�C���^ </param>
	void AddSprite(class SpriteComponent* sprite);
	/// <summary>
	/// �X�v���C�g�̍폜
	/// </summary>
	/// <param name="sprite"> Sprite�|�C���^ </param>
	void RemoveSprite(class SpriteComponent* sprite);

	/// <summary>
	/// �e�N�X�`���t�@�C���p�X���󂯎��A�Ή�����|�C���^�[��Ԃ�
	/// </summary>
	/// <param name="fileName">	�t�@�C���� </param>
	/// <returns> SDL_Texture�̃|�C���^��Ԃ� </returns>
	class Texture* GetTexture(const std::string& fileName);


	// -------------------------------------------
	void AddAsteroid(class Asteroid* ast);
	void RemoveAsteroid(class Asteroid* ast);
	std::vector<class Asteroid*>& GetAsteroids() { return mAsteroids; }

private:
	//�Q�[�����[�v�̂��߂̃w���p�[�֐��Q
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	bool LoadShader();
	void CreateSpriteVerts();
	void LoadData();
	void UnloadData();

	//Actor�ւ̃|�C���^���i�[
	std::vector<class Actor*> mActors;	// �Q�[�����̂��ׂẴA�N�^�[
	std::vector<class Actor*> mPendingActors;	// �҂���Ԃ̃A�N�^�[�Q

	// ���[�h���ꂽ�e�N�X�`���̃}�b�v
	std::unordered_map<std::string, class Texture*> mTextures;

	// ���ׂĂ�SpriteComponents��`��
	std::vector<class SpriteComponent*> mSprites;

	// �V�F�[�_�[�|�C���^
	class Shader* mSpriteShader;
	// ���_�o�b�t�@�|�C���^
	class VertexArray* mSpriteVerts;

	// SDL�����E�B���h�E
	SDL_Window* mWindow;
	// �R���e�N�X�g�쐬
	SDL_GLContext mContext;
	// �Q�[���J�n����̌o�ߎ���
	Uint32 mTicksCount;
	// �Q�[���̑��s���w������
	bool mIsRunning;
	// mActors���X�V���Ă��邩�ǂ���
	bool mUpdatingActors;

	//------------------------------------
	class Ship* mShip;
	std::vector<class Asteroid*> mAsteroids;

};