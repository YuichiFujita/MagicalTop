//============================================================
//
//	�u���b�N�w�b�_�[ [block.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _BLOCK_H_
#define _BLOCK_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectMeshCube.h"

//************************************************************
//	�N���X��`
//************************************************************
// �u���b�N�N���X
class CBlock : public CObjectMeshCube
{
public:
	// �e�N�X�`����
	typedef enum
	{
		TYPE_SOIL = 0,	// �y�e�N�X�`��
		TYPE_GRASS,		// ���e�N�X�`��
		TYPE_STONE,		// �΃e�N�X�`��
		TYPE_MAX		// ���̗񋓌^�̑���
	}TYPE;

	// �R���X�g���N�^
	CBlock();

	// �f�X�g���N�^
	~CBlock();

	// ��ލ\����
	typedef struct
	{
		FACETEX  texture;	// �e�N�X�`��
		TEXSTATE state;		// �e�N�X�`���g�p��
	}Type;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static void LoadSetup(void);	// �Z�b�g�A�b�v
	static CBlock *Create	// ����
	( // ����
		const TYPE type,		// ���
		const D3DXVECTOR3& rPos	// �ʒu
	);

private:
	// �ÓI�����o�ϐ�
	static MeshCube m_blockInfo;		// �u���b�N�X�e�[�^�X���
	static Type m_aTypeInfo[TYPE_MAX];	// �e�N�X�`����ޏ��
};

#endif	// _BLOCK_H_
