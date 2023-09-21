//============================================================
//
//	�����}�l�[�W���[�w�b�_�[ [levelupManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _LEVELUPMANAGER_H_
#define _LEVELUPMANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�O���錾
//************************************************************
class CShopManager;	// �V���b�v�}�l�[�W���[�N���X
class CObject2D;	// �I�u�W�F�N�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �����}�l�[�W���[�N���X
class CLevelupManager
{
public:
	// �e�N�X�`����
	enum TEXTURE
	{
		TEXTURE_CONTROL = 0,	// ����e�N�X�`��
		TEXTURE_MAX,			// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum STATE
	{
		STATE_NORMAL = 0,	// �ʏ���
		STATE_SELECT,		// �����I�����
		STATE_MAX,			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CLevelupManager();

	// �f�X�g���N�^
	~CLevelupManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V

	// �ÓI�����o�֐�
	static CLevelupManager *Create(void);	// ����
	static HRESULT Release(CLevelupManager *&prLevelupManager);	// �j��

private:
	// �����o�֐�
	void UpdateBlink(void);	// �_�ōX�V

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	CShopManager *m_pShopManager;	// �V���b�v�}�l�[�W���[�̏��
	CObject2D *m_pControl;	// ������
	STATE m_state;			// ���
	float m_fSinRot;		// �|���S���_�Ō���
};

#endif	// _LEVELUPMANAGER_H_
