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
#include "shop.h"

//************************************************************
//	�O���錾
//************************************************************
class COmitShop;	// �V���b�v�Ȃ����N���X
class CObject2D;	// �I�u�W�F�N�g2D�N���X
class CValueUI;		// ����UI�N���X
class CShop;		// �V���b�v�N���X

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
		TEXTURE_PLAYER = 0,	// �v���C���[�A�C�R���e�N�X�`��
		TEXTURE_EXP,		// �o���l�A�C�R���e�N�X�`��
		TEXTURE_LIFE,		// �̗̓e�N�X�`��
		TEXTURE_SLASH,		// �ő�̗͋�؂�e�N�X�`��
		TEXTURE_LV,			// ���x���e�N�X�`��
		TEXTURE_MAX,		// ���̗񋓌^�̑���
	};

	// �I���
	enum SELECT
	{
		SELECT_LEFT = 0,	// ���I��
		SELECT_CENTER,		// �����I��
		SELECT_RIGHT,		// �E�I��
		SELECT_MAX,			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CShopManager();

	// �f�X�g���N�^
	~CShopManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void SetEnableOpen(const bool bOpen);	// �J�X�󋵐ݒ�

	// �ÓI�����o�֐�
	static CShopManager *Create(void);	// ����
	static HRESULT Release(CShopManager *&prShopManager);	// �j��

private:
	// �����o�֐�
	void UpdateSelect(void);	// �w���i�I��
	void UpdateBuy(void);		// �w��
	void CheckHealOmit(void);	// �񕜏Ȃ��m�F
	void SetEnableDraw(const bool bDraw);	// �`��󋵐ݒ�

	int RandomShop	// �V���b�v�P�ύX
	( // ����
		const int nShop,		// �ύX����V���b�v�C���f�b�N�X
		int *pOmit = NULL,		// �����_�����Ȃ��w���i�̐擪�A�h���X
		const int nNumOmit = 0	// �����_�����Ȃ��w���i��
	);
	void AllRandomShop(void);	// �V���b�v�S�ύX

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	COmitShop *m_pOmitShop;			// �V���b�v�Ȃ����
	CObject2D *m_pBg;				// �w�i���
	CObject2D *m_pIconPlayer;		// �v���C���[�A�C�R�����
	CObject2D *m_pIconExp;			// �o���l�A�C�R�����
	CValueUI *m_pLife;				// �v���C���[�̗͏��
	CValueUI *m_pMaxLife;			// �v���C���[�ő�̗͏��
	CValueUI *m_pLv;				// �v���C���[���x�����
	CShop *m_apShop[SELECT_MAX];	// �V���b�v���
	int m_nSelect;					// ���݂̑I��ԍ�
	int m_nOldSelect;				// �ߋ��̑I��ԍ�
};

#endif	// _SHOPMANAGER_H_
