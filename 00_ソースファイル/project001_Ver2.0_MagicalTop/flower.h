//============================================================
//
//	�}�i�t�����[�w�b�_�[ [flower.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _FLOWER_H_
#define _FLOWER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectBillboard.h"

//************************************************************
//	�O���錾
//************************************************************
class CShadow;	// �e�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �}�i�t�����[�N���X
class CFlower : public CObjectBillboard
{
public:
	// ��ޗ�
	typedef enum
	{
		TYPE_NORMAL = 0,	// �ʏ�
		TYPE_MAX			// ���̗񋓌^�̑���
	}TYPE;

	// �R���X�g���N�^
	CFlower();

	// �f�X�g���N�^
	~CFlower();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CFlower *Create	// ����
	( // ����
		const TYPE type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rSize,	// �傫��
		const int nLife				// �̗�
	);

	// �����o�֐�
	void SetLife(const int nLife);	// �̗͐ݒ�

private:
	// �����o�֐�
	bool CollisionPlayer(void);	// �v���C���[�Ƃ̓����蔻��

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	CShadow *m_pShadow;	// �e�̏��
	TYPE m_type;		// ���
	int m_nLife;		// �̗�
};

#endif	// _FLOWER_H_
