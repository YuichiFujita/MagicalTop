//============================================================
//
//	�`���[�g���A���}�l�[�W���[�w�b�_�[ [tutorialManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TUTORIAL_MANAGER_H_
#define _TUTORIAL_MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�O���錾
//************************************************************
class CLetterManager;	// �莆�}�l�[�W���[�N���X
class CObject2D;		// �I�u�W�F�N�g2D�N���X
class CObjectGauge2D;	// �I�u�W�F�N�g�Q�[�W2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �`���[�g���A���}�l�[�W���[�N���X
class CTutorialManager
{
public:
	// �e�N�X�`����
	enum TEXTURE
	{
		TEXTURE_CONTROL = 0,	// ������@�\���̃e�N�X�`��
		TEXTURE_START,			// �J�n���̕�ⳃe�N�X�`��
		TEXTURE_END,			// �I�����̕�ⳃe�N�X�`��
		TEXTURE_MAX				// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum STATE
	{
		STATE_NONE = 0,		// �������Ȃ����
		STATE_WAIT,			// �ҋ@���
		STATE_LETTER,		// �莆�\�����
		STATE_FADEIN,		// �t�F�[�h�C�����
		STATE_EXPLAIN,		// �����\�����
		STATE_FADEWAIT,		// �t�F�[�h�ҋ@���
		STATE_FADEOUT,		// �t�F�[�h�A�E�g���
		STATE_PROGRESSION,	// �i�s���
		STATE_NEXTWAIT,		// �����b�X���ҋ@���
		STATE_END,			// �I�����
		STATE_MAX			// ���̗񋓌^�̑���
	};

	// ���b�X����
	enum LESSON
	{
		LESSON_NONE = 0,	// ���b�X������
		LESSON_01,			// ���b�X��01�F�z�����܂��
		LESSON_02,			// ���b�X��02�F�O�����
		LESSON_03,			// ���b�X��03�F���E����
		LESSON_04,			// ���b�X��04�F�U��
		LESSON_05,			// ���b�X��05�F�}�i��
		LESSON_06,			// ���b�X��06�F�G�ւ̍U��
		LESSON_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CTutorialManager();

	// �f�X�g���N�^
	~CTutorialManager();

	// �����o�֐�
	HRESULT Init(void);		// ������
	HRESULT Uninit(void);	// �I��
	void Update(void);		// �X�V
	void AddLessonCounter(void);	// ���b�X���J�E���^�[���Z
	int GetLesson(void) const;		// ���b�X���擾
	STATE GetState(void) const;		// ��Ԏ擾

	// �ÓI�����o�֐�
	static CTutorialManager *Create(void);	// ����
	static HRESULT Release(CTutorialManager *&prTutorialManager);	// �j��
	static int GetNextLessonCounter(const int nID);	// ���b�X���ڍs�J�E���g�擾

private:
	// �����o�֐�
	void SetEnableProgressionDraw(const bool bDraw);	// �i�s��Ԏ��ɕ\������UI�̕`��󋵐ݒ�

	void UpdateWait(void);		// �ҋ@
	void UpdateLetter(void);	// �莆�\��
	void UpdateFadeIn(void);	// �t�F�[�h�C��
	void UpdateExplain(void);	// �����\��
	void UpdateFadeWait(void);	// �t�F�[�h�ҋ@
	void UpdateFadeOut(void);	// �t�F�[�h�A�E�g
	void UpdateNextWait(void);	// �����b�X���ҋ@
	void NextLesson(void);		// �����b�X���ւ̈ڍs

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];			// �e�N�X�`���萔
	static const char *mc_apLessonTextureFile[];	// ���b�X���e�N�X�`���萔
	static const char *mc_apGuideTextureFile[];		// �������e�N�X�`���萔
	static const char *mc_apControlTextureFile[];	// ������@�e�N�X�`���萔
	static const int mc_aNextLesson[];				// ���b�X���ڍs�J�E���g
	static const int mc_aNextLessonWait[];			// �����b�X���]�C�J�E���g

	// �����o�ϐ�
	CLetterManager *m_pLetterManager;	// �莆�}�l�[�W���[�̏��
	CObjectGauge2D *m_pCounterLesson;	// ���b�X���Ǘ��J�E���^�[�̏��
	CObject2D *m_pFade;			// �t�F�[�h�̏��
	CObject2D *m_pExplain;		// �����\���̏��
	CObject2D *m_pClose;		// �����I���\���̏��
	CObject2D *m_pGuide;		// �������\���̏��
	CObject2D *m_pGuideBG;		// �������\���̔w�i���
	CObject2D *m_pControl;		// ������@�\���̏��
	CObject2D *m_pControlBG;	// ������@�\���̔w�i���
	STATE m_state;				// ���
	int m_nCounterState;		// ��ԊǗ��J�E���^�[
	int m_nLesson;				// ���b�X��
	float m_fScale;				// �|���S���g�嗦
	float m_fSinRot;			// �|���S���_�Ō���
};

#endif	// _TUTORIAL_MANAGER_H_
