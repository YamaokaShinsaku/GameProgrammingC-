#pragma once

/// <summary>
/// ���_�z��N���X
/// </summary>
class VertexArray
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="verts"> ���_ </param>
	/// <param name="numVerts"> ���_�̐� </param>
	/// <param name="indices"> �C���f�b�N�X </param>
	/// <param name="numIndices"> �C���f�b�N�X�̐� </param>
	VertexArray(const float* verts, unsigned int numVerts,
		const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	/// <summary>
	/// ���_�z����A�N�e�B�u�ɂ���(�`��ł���悤�ɂ���)
	/// </summary>
	void SetActive();

	// getter / setter
	unsigned int GetNumIndices() const { return mNumIndices; }
	unsigned int GetNumVerts() const { return mNumVerts; }

private:
	// ���_�o�b�t�@�ɂ��钸�_�̐�
	unsigned int mNumVerts;
	// �C���f�b�N�X�o�b�t�@�ɂ���C���f�b�N�X�̐�
	unsigned int mNumIndices;
	// ���_�o�b�t�@�� OpenGL ID
	unsigned int mVertexBuffer;
	// �C���f�b�N�X�o�b�t�@�� OpenGL ID
	unsigned int mIndexBuffer;
	// ���_�z��I�u�W�F�N�g�� OpenGL ID
	unsigned int mVertexArray;
};

