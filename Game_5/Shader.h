#pragma once
#include <GL/glew.h>
#include <string>
#include "Math.h"

class Shader
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Shader();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Shader();

	/// <summary>
	/// �w�肳�ꂽ���O�� ���_/�t���O�����g�V�F�[�_�[��ǂݍ���
	/// </summary>
	/// <param name="vertName"> ���_�V�F�[�_�[ </param>
	/// <param name="fragName"> �t���O�����g�V�F�[�_�[ </param>
	/// <returns> �ǂݍ��݂� ���� true/���s false </returns>
	bool Load(const std::string& vertName, const std::string& fragName);

	/// <summary>
	/// �폜
	/// </summary>
	void Unload();

	/// <summary>
	/// �A�N�e�B�u�ȃV�F�[�_�[�v���O�����Ƃ��Đݒ�
	/// </summary>
	void SetActive();

	/// <summary>
	/// �A�N�e�B�u�ȃV�F�[�_�[�v���O������uniform�ϐ���ݒ�
	/// </summary>
	/// <param name="name"> �����񃊃e�����̖��O </param>
	/// <param name="matrix"> �s�� </param>
	void SetMatrixUniform(const char* name, const Matrix4& matrix);

private:
	/// <summary>
	/// �V�F�[�_�[���R���p�C������
	/// </summary>
	/// <param name="fileName"> �t�@�C���� </param>
	/// <param name="shaderType"> �V�F�[�_�[�̎�� </param>
	/// <param name="outShader"> ID��ۑ� </param>
	/// <returns> ���� true/ ���s </returns>
	bool CompileShader(const std::string& fileName,
					   GLenum shaderType,
					   GLuint& outShader);
	/// <summary>
	/// �V�F�[�_�[�̃R���p�C���������������̔���
	/// </summary>
	/// <param name="shader"> �V�F�[�_�[ </param>
	/// <returns> ���� true/ ���s false </returns>
	bool IsCompiled(GLuint shader);
	/// <summary>
	/// ���_/�t���O�����g�v���O�����̃����N���m�F
	/// </summary>
	/// <returns> ���� true/ ���s false </returns>
	bool IsValidProgram();

	// �V�F�[�_�[�I�u�W�F�N�g��ID���i�[
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};

