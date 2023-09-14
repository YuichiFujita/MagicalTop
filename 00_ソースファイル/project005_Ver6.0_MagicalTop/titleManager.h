//============================================================
//
//	�^�C�g���}�l�[�W���[�w�b�_�[ [titleManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TITLEMANAGER_H_
#define _TITLEMANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "enemy.h"

//************************************************************
//	�}�N����`
//************************************************************
#define NUM_LOGO	(2)	// ���S�̎g�p��

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;	// �I�u�W�F�N�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �^�C�g���}�l�[�W���[�N���X
class CTitleManager
{
public:
	// �e�N�X�`����
	typedef enum
	{
		TEXTURE_NORMAL = 0,	// �^�C�g��
		TEXTURE_MAX			// ���̗񋓌^�̑���
	}TEXTURE;

	// ���S��
	typedef enum
	{
		LOGO_MAGICAL = 0,	// MAGICAL
		LOGO_TOP,			// TOP
		LOGO_MAX			// ���̗񋓌^�̑���
	}LOGO;

	// ��ԗ�
	typedef enum
	{
		STATE_NONE = 0,		// �������Ȃ����
		STATE_FADEOUT,		// �t�F�[�h�A�E�g���
		STATE_MOVE,			// �^�C�g���k�����
		STATE_WAIT,			// �J�ڑҋ@���
		STATE_MAX			// ���̗񋓌^�̑���
	}STATE;

	// �R���X�g���N�^
	CTitleManager();

	// �f�X�g���N�^
	~CTitleManager();

	// �����o�֐�
	HRESULT Init(void);		// ������
	void Uninit(void);		// �I��
	void Update(void);		// �X�V

	// �ÓI�����o�֐�
	static CTitleManager *Create(void);	// ����
	static HRESULT Release(CTitleManager *&prTitleManager);	// �j��

private:
	// �����o�֐�
	void UpdateFade(void);	// �t�F�[�h�A�E�g
	void UpdateMove(void);	// �^�C�g���ړ�
	void UpdateStart(void);	// �J�ڌ���
	void SkipStaging(void);	// ���o�X�L�b�v

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];		// �e�N�X�`���萔
	static const char *mc_apLogoTextureFile[];	// ���S�e�N�X�`���萔

	// �����o�ϐ�
	CObject2D *m_apLogo[NUM_LOGO];	// �^�C�g���\���̏��
	CObject2D *m_pFade;		// �t�F�[�h�̏��
	STATE m_state;			// ���
	int m_nCounterState;	// ��ԊǗ��J�E���^�[
	float m_fScale;			// �^�C�g���g�嗦
};

#endif	// _TITLEMANAGER_H_
