#pragma once
#include <GL/glew.h>
#include <string>
#include "Math.h"

class Shader
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Shader();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Shader();

	/// <summary>
	/// 指定された名前の 頂点/フラグメントシェーダーを読み込む
	/// </summary>
	/// <param name="vertName"> 頂点シェーダー </param>
	/// <param name="fragName"> フラグメントシェーダー </param>
	/// <returns> 読み込みが 成功 true/失敗 false </returns>
	bool Load(const std::string& vertName, const std::string& fragName);

	/// <summary>
	/// 削除
	/// </summary>
	void Unload();

	/// <summary>
	/// アクティブなシェーダープログラムとして設定
	/// </summary>
	void SetActive();

	/// <summary>
	/// アクティブなシェーダープログラムのuniform変数を設定
	/// </summary>
	/// <param name="name"> 文字列リテラルの名前 </param>
	/// <param name="matrix"> 行列 </param>
	void SetMatrixUniform(const char* name, const Matrix4& matrix);

private:
	/// <summary>
	/// シェーダーをコンパイルする
	/// </summary>
	/// <param name="fileName"> ファイル名 </param>
	/// <param name="shaderType"> シェーダーの種類 </param>
	/// <param name="outShader"> IDを保存 </param>
	/// <returns> 成功 true/ 失敗 </returns>
	bool CompileShader(const std::string& fileName,
					   GLenum shaderType,
					   GLuint& outShader);
	/// <summary>
	/// シェーダーのコンパイルが成功したかの判定
	/// </summary>
	/// <param name="shader"> シェーダー </param>
	/// <returns> 成功 true/ 失敗 false </returns>
	bool IsCompiled(GLuint shader);
	/// <summary>
	/// 頂点/フラグメントプログラムのリンクを確認
	/// </summary>
	/// <returns> 成功 true/ 失敗 false </returns>
	bool IsValidProgram();

	// シェーダーオブジェクトのIDを格納
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};

