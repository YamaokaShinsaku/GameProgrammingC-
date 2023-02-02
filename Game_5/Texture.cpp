#include "Texture.h"
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <SDL/SDL.h>

Texture::Texture()
	: mTextureID(0)
	, mWidth(0)
	, mHeight(0)
{

}

Texture::~Texture()
{

}

bool Texture::Load(const std::string& fileName)
{
	// チャンネル数を格納
	int channels = 0;

	unsigned char* image = SOIL_load_image(
		fileName.c_str(),		// ファイル名
		&mWidth,				// 幅が記憶される
		&mHeight,				// 高さが記録される
		&channels,				// チャンネル数が記録される
		SOIL_LOAD_AUTO			// 画像ファイルの種類 (または auto)
	);

	// 画像ファイルの読み込みに失敗
	if (image == nullptr)
	{
		// ログ表示
		SDL_Log("SOIL failed to load image %s : %s", fileName.c_str(), SOIL_last_result());
		return false;
	}

	// 画像が RGB か RGBA かを判定
	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}

	// OpenGL テクスチャオブジェクトを作成,ID を mTextureID に保存
	glGenTextures(1, &mTextureID);
	// テクスチャをアクティブに
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	// 画像データをコピーする
	glTexImage2D(
		GL_TEXTURE_2D,			// テクスチャターゲット
		0,						// LoD(Level of Detail : 詳細レベル)
		format,					// OpenGL が使うべきカラーフォーマット
		mWidth,					// テクスチャの幅
		mHeight,				// テクスチャの高さ
		0,						// 境界色
		format,					// 入力データのカラーフォーマット
		GL_UNSIGNED_BYTE,		// 入力データのビット深度
								// unsigned byte で 8ビット / チャンネルを指定
		image					// 画像データへのポインタ
	);

	// 画像データをメモリから解放
	SOIL_free_image_data(image);

	// バイリニアフィルタを有効にする
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture::Unload()
{
	glDeleteTextures(1, &mTextureID);
}

void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}
