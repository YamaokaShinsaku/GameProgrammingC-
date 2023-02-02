#include <string>

class Texture
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Texture();
	
	~Texture();

	/// <summary>
	/// テクスチャの読み込み
	/// </summary>
	/// <param name="fileName"> ファイル名 </param>
	/// <returns> 成功 true / 失敗 false </returns>
	bool Load(const std::string& fileName);
	/// <summary>
	/// 削除
	/// </summary>
	void Unload();

	/// <summary>
	/// アクティブ化する
	/// </summary>
	void SetActive();

	// getter / setter
	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }

private:
	// このテクスチャの OpenGL ID
	unsigned int mTextureID;
	// テクスチャの幅と高さ
	int mWidth;
	int mHeight;
};

