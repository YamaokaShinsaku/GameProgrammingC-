#include "Game.h"

int main(int argc, char** argv)
{
	Game game;	// Game�N���X�̃C���X�^���X�쐬
	bool success = game.Initialize();

	if (success)
	{
		game.RunLoop();
	}

	game.Shutdown();

	return 0;
}