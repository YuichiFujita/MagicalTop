//============================================================
//
//	�V���b�v�}�l�[�W���[�w�b�_�[ [shopManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SHOPMANAGER_H_
#define _SHOPMANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;	// �I�u�W�F�N�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �V���b�v�}�l�[�W���[�N���X
class CShopManager
{
public:
	// �e�N�X�`����
	enum TEXTURE
	{
		TEXTURE_TARGET = 0,	// �^�[�Q�b�g�A�C�R���e�N�X�`��
		TEXTURE_EXP,		// �o���l�A�C�R���e�N�X�`��
		TEXTURE_MAX,		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CShopManager();

	// �f�X�g���N�^
	~CShopManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void SetEnableDraw(const bool bDraw);	// �`��󋵐ݒ�

	// �ÓI�����o�֐�
	static CShopManager *Create(void);	// ����
	static HRESULT Release(CShopManager *&prShopManager);	// �j��

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	CObject2D *m_pBg;			// �w�i���
	CObject2D *m_pIconTarget;	// �^�[�Q�b�g�A�C�R�����
	CObject2D *m_pIconExp;		// �o���l�A�C�R�����
};

#endif	// _SHOPMANAGER_H_
