#pragma once

/// <summary>
/// 頂点配列クラス
/// </summary>
class VertexArray
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="verts"> 頂点 </param>
	/// <param name="numVerts"> 頂点の数 </param>
	/// <param name="indices"> インデックス </param>
	/// <param name="numIndices"> インデックスの数 </param>
	VertexArray(const float* verts, unsigned int numVerts,
		const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	/// <summary>
	/// 頂点配列をアクティブにする(描画できるようにする)
	/// </summary>
	void SetActive();

	// getter / setter
	unsigned int GetNumIndices() const { return mNumIndices; }
	unsigned int GetNumVerts() const { return mNumVerts; }

private:
	// 頂点バッファにある頂点の数
	unsigned int mNumVerts;
	// インデックスバッファにあるインデックスの数
	unsigned int mNumIndices;
	// 頂点バッファの OpenGL ID
	unsigned int mVertexBuffer;
	// インデックスバッファの OpenGL ID
	unsigned int mIndexBuffer;
	// 頂点配列オブジェクトの OpenGL ID
	unsigned int mVertexArray;
};

