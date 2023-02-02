#pragma once
#include "Actor.h"
#include <vector>

class Grid :
    public Actor
{
public:
    Grid(class Game* game);

    /// <summary>
    /// ��ʂ̂ǂ����N���b�N������
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    void ProcessClick(int x, int y);

    /// <summary>
    /// A*�@���g���Čo�H�T��������
    /// </summary>
    /// <param name="start"></param>
    /// <param name="goal"></param>
    bool FindPath(class Tile* start, class Tile* goal);

    /// <summary>
    /// �������Ă�
    /// </summary>
    void BuildTower();

    // Tile�́@�J�n�^�I���@���󂯎��
    class Tile* GetStartTile();
    class Tile* GetEndTile();

    void UpdateActor(float deltaTime)override;
private:
    /// <summary>
    /// ����̃^�C����I��
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

