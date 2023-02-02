#include "VertexArray.h"
#include <GL/glew.h>

VertexArray::VertexArray(const float* verts, unsigned int numVerts,
	const unsigned int* indices, unsigned int numIndices)
	: mNumVerts(numVerts)
	, mNumIndices(numIndices)
{
	// 頂点配列オブジェクトを作成
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// 頂点バッファを作成
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,		// バッファの種類
		numVerts * 5 * sizeof(float),	// コピーするバイト数
		verts,							// コピー元（ポインタ）
		GL_STATIC_DRAW					// このデータの利用方法
	);

	// インデックスバッファを作成
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,			// インデックスバッファの指定
		numIndices * sizeof(unsigned int),	// データのサイズ
		indices,							// コピー元（ポインタ）
		GL_STATIC_DRAW						// このデータの利用方法
	);

	// 頂点属性を指定
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,						// 属性インデックス
		3,						// 要素数
		GL_FLOAT,				// 要素の型
		GL_FALSE,				// 整数型のみ使用
		sizeof(float) * 5,		// ストライド（通常は各頂点のサイズ）
		0						// 頂点データの開始位置からこの属性までのオフセット
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
