#pragma once
#include "Actor.h"
#include <vector>

class Grid :
    public Actor
{
public:
    Grid(class Game* game);

    /// <summary>
    /// 画面のどこをクリックしたか
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    void ProcessClick(int x, int y);

    /// <summary>
    /// A*法を使って経路探索をする
    /// </summary>
    /// <param name="start"></param>
    /// <param name="goal"></param>
    bool FindPath(class Tile* start, class Tile* goal);

    /// <summary>
    /// 塔を建てる
    /// </summary>
    void BuildTower();

    // Tileの　開始／終了　を受け取る
    class Tile* GetStartTile();
    class Tile* GetEndTile();

    void UpdateActor(float deltaTime)override;
private:
    /// <summary>
    /// 特定のタイルを選択
    /// </summary>
    /// <param name="row"></param>
    /// <param name="col"></param>
    void SelectTile(size_t row, size_t col);

    void UpdatePathTiles(class Tile* start);

    class Tile* mSelectedTile;

    std::vector<std::vector<class Tile*>> mTiles;

    float mNextEnemy;

    const size_t NumRows = 7;
    const size_t NumCols = 16;

    const float StartY = 192.0f;
    const float TileSize = 64.0f;

    const float EnemyTime = 1.5f;

};

