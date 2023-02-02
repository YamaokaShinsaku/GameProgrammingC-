#include <string>

class Texture
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Texture();
	
	~Texture();

	/// <summary>
	/// �e�N�X�`���̓ǂݍ���
	/// </summary>
	/// <param name="fileName"> �t�@�C���� </param>
	/// <returns> ���� true / ���s false </returns>
	bool Load(const std::string& fileName);
	/// <summary>
	/// �폜
	/// </summary>
	void Unload();

	/// <summary>
	/// �A�N�e�B�u������
	/// </summary>
	void SetActive();

	// getter / setter
	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }

private:
	// ���̃e�N�X�`���� OpenGL ID
	unsigned int mTextureID;
	// �e�N�X�`���̕��ƍ���
	int mWidth;
	int mHeight;
};

