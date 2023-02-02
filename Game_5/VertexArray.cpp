#include "VertexArray.h"
#include <GL/glew.h>

VertexArray::VertexArray(const float* verts, unsigned int numVerts,
	const unsigned int* indices, unsigned int numIndices)
	: mNumVerts(numVerts)
	, mNumIndices(numIndices)
{
	// ���_�z��I�u�W�F�N�g���쐬
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// ���_�o�b�t�@���쐬
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,		// �o�b�t�@�̎��
		numVerts * 5 * sizeof(float),	// �R�s�[����o�C�g��
		verts,							// �R�s�[���i�|�C���^�j
		GL_STATIC_DRAW					// ���̃f�[�^�̗��p���@
	);

	// �C���f�b�N�X�o�b�t�@���쐬
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,			// �C���f�b�N�X�o�b�t�@�̎w��
		numIndices * sizeof(unsigned int),	// �f�[�^�̃T�C�Y
		indices,							// �R�s�[���i�|�C���^�j
		GL_STATIC_DRAW						// ���̃f�[�^�̗��p���@
	);

	// ���_�������w��
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,						// �����C���f�b�N�X
		3,						// �v�f��
		GL_FLOAT,				// �v�f�̌^
		GL_FALSE,				// �����^�̂ݎg�p
		sizeof(float) * 5,		// �X�g���C�h�i�ʏ�͊e���_�̃T�C�Y�j
		0						// ���_�f�[�^�̊J�n�ʒu���炱�̑����܂ł̃I�t�Z�b�g
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5,
		reinterpret_cast<void*>(sizeof(float) * 3));
}

VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive()
{
	glBindVertexArray(mVertexArray);
}
