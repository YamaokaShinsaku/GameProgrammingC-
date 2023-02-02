#include "Game.h"

int main(int argc, char** argv)
{
	Game game;	// Gameクラスのインスタンス作成
	bool success = game.Initialize();	// 初期化
	// 初期化が成功したら
	if (success)
	{
		// ゲームループ開始
		game.RunLoop();
	}
	// ゲーム終了
	game.Shutdown();

	return 0;
}