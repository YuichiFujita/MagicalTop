//============================================================
//
//	�莆�}�l�[�W���[�w�b�_�[ [letterManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _LETTER_MANAGER_H_
#define _LETTER_MANAGER_H_

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
// �莆�}�l�[�W���[�N���X
class CLetterManager
{
public:
	// �e�N�X�`����
	enum TEXTURE
	{
		TEXTURE_LETTER = 0,	// �莆�̃e�N�X�`��
		TEXTURE_CONTROL,	// ����\���̃e�N�X�`��
		TEXTURE_MAX			// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum STATE
	{
		STATE_NONE = 0,		// �������Ȃ����
		STATE_LETTER,		// �莆�̕\�����
		STATE_FADEIN,		// �t�F�[�h�C�����
		STATE_PAPER_TAKE,	// ��Ⳃ̎��o�����
		STATE_WAIT,			// �ҋ@���
		STATE_PAPER_RETURN,	// ��Ⳃ̂��܂����
		STATE_END,			// �I�����
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CLetterManager();

	// �f�X�g���N�^
	~CLetterManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void SetPaper(const char *pPassTex);	// ��Ⳑݒ�
	void SetLook(const char *pPassTex);		// �莆�\���J�n
	void SetState(const STATE state);		// ��Ԑݒ�
	STATE GetState(void) const;				// ��Ԏ擾

	// �ÓI�����o�֐�
	static CLetterManager *Create(void);	// ����
	static HRESULT Release(CLetterManager *&prLetterManager);	// �j��

private:
	// �����o�֐�
	void UpdateLetter(void);		// �莆�\��
	void UpdateFade(void);			// �t�F�[�h�C��
	void UpdatePaperTake(void);		// ��Ⳏ�o
	void UpdateWait(void);			// �ҋ@
	void UpdatePaperReturn(void);	// ��Ⳏ��[

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	CObject2D *m_pLetter;	// �莆�̏��
	CObject2D *m_pFade;		// �t�F�[�h�̏��
	CObject2D *m_pPaper;	// ��Ⳃ̏��
	CObject2D *m_pControl;	// ������@�̏��
	STATE m_state;			// ���
	int m_nCounterState;	// ��ԊǗ��J�E���^�[
	float m_fMove;			// ��Ⳃ̈ړ���
	float m_fSinRot;		// ������@�̓_�Ō���
};

#endif	// _LETTER_MANAGER_H_
