#include "Shader.h"
//#include "Texture.h"
#include <SDL/SDL.h>
#include <fstream>
#include <sstream>

Shader::Shader()
	: mShaderProgram(0)
	, mVertexShader(0)
	, mFragShader(0)
{

}

Shader::~Shader()
{

}

bool Shader::Load(const std::string& vertName, const std::string& fragName)
{
	// 頂点シェーダーとフラグメントシェーダーをコンパイルする
	if (!CompileShader(vertName, GL_VERTEX_SHADER, mVertexShader)
		|| !CompileShader(fragName, GL_FRAGMENT_SHADER, mFragShader))
	{
		return false;
	}

	// 頂点/フラグメントシェーダーをリンクして
	// シェーダープログラムを作る
	mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, mVertexShader);
	glAttachShader(mShaderProgram, mFragShader);
	glLinkProgram(mShaderProgram);

	// プログラムが正しくリンクされたことを確認
	if (!IsValidProgram())
	{
		return false;
	}

	return true;
}

void Shader::Unload()
{
	// シェーダープログラム/頂点シェーダー/フラグメントシェーダーを削除
	glDeleteProgram(mShaderProgram);
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragShader);
}

void Shader::SetActive()
{
	// シェーダープログラムをアクティブにする
	glUseProgram(mShaderProgram);
}

void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix)
{
	// この名前の uniform を探す
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// 行列データを uniform に送る
	glUniformMatrix4fv(
		loc,						// Uniform ID
		1,							// 行列の数
		GL_TRUE,					// 行列ベクトルを使うなら TRUE
		matrix.GetAsFloatPtr()		// 行列へのポインタ
	);
}

bool Shader::CompileShader(const std::string& fileName, 
	GLenum shaderType,
	GLuint& outShader)
{
	// ファイルを開く
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open())
	{
		// すべてのテキストを1つの文字列に読み込む
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		// 指定されたタイプのシェーダーを作成
		outShader = glCreateShader(shaderType);
		// 読み込んだ文字列でのコンパイルを試みる
		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);

		if (!IsCompiled(outShader))
		{
			// ログ表示
			SDL_Log("Failed to compile shader : %s", fileName.c_str());

			return false;
		}
	}
	else
	{
		// ログ表示
		SDL_Log("Shader file not found : %s", fileName.c_str());

		return false;
	}

	return true;
}

bool Shader::IsCompiled(GLuint shader)
{
	GLint status;
	// コンパイル状態を問い合わせる
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);

		// ログ表示
		SDL_Log("GLSL Compile Failed : \n %s", buffer);
		return false;
	}

	return true;
}

bool Shader::IsValidProgram()
{
	GLint status;
	// コンパイル状態を問い合わせる
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(mShaderProgram, 511, nullptr, buffer);

		// ログ表示
		SDL_Log("GLSL Link Status : \n %s", buffer);
		return false;
	}

	return true;
}
