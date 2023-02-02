#pragma once
#include "Actor.h"
#include <vector>

class Tile :
    public Actor
{
public:
    /// <summary>
    /// フレンドクラス作成
    /// </summary>
    friend class Grid;

    enum TileState
    {
        EDefault,
        EPath,
        EStart,
        EBase
    };

    Tile(class Game* game);

    void SetTileState(TileState state);
    TileState GetTileState() const { return mTileState; }
    void ToggleSelect();
    const Tile* GetParent() const { return mParent; }

private:
    // 経路探索用
    std::vector<Tile*> mAdjacent;
    Tile* mParent;
    float f;
    float g;
    float h;
    bool mInOpenSet;
    bool mInCloseSet;
    bool mBlocked;

    void UpdateTexture();
    class SpriteComponent* mSprite;
    TileState mTileState;
    bool mSelected;
};

