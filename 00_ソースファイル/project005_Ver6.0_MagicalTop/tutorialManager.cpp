//============================================================
//
//	�`���[�g���A���}�l�[�W���[���� [tutorialManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "tutorialManager.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "letterManager.h"
#include "object2D.h"
#include "objectGauge2D.h"
#include "player.h"
#include "enemy.h"

//************************************************************
//	�}�N����`
//************************************************************
#define TUTORIAL_PRIO	(6)	// �`���[�g���A���̗D�揇��

#define LETTER_WAIT_CNT	(90)	// �ҋ@��Ԃ̑ҋ@�t���[��
#define TITLE_WAIT_CNT	(30)	// �^�C�g���J�ڂ̗]�C�t���[��

#define POS_GUIDE		(D3DXVECTOR3(1020.0f, 585.0f, 0.0f))	// �������\���̈ʒu
#define POS_CONTROL		(D3DXVECTOR3(260.0f, 585.0f, 0.0f))		// ������@�\���̈ʒu

#define SIZE_PROGRESSION_UI		(D3DXVECTOR3(500.0f, 250.0f, 0.0f))	// �`���[�g���A���i�s����UI�\���̑傫��
#define COL_PROGRESSION_UI_BG	(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f))	// �`���[�g���A���i�s����UI�\���̔w�i�F

#define FRAME_COUNTER		(10)	// �Q�[�W�̕ϓ��t���[��
#define SIZE_COUNTER		(D3DXVECTOR3(SIZE_PROGRESSION_UI.x * 0.5f, 8.0f, 0.0f))	// �Q�[�W�̑傫��
#define POS_COUNTER			(D3DXVECTOR3(POS_GUIDE.x, POS_GUIDE.y - (SIZE_PROGRESSION_UI.y * 0.5f) - SIZE_COUNTER.y, 0.0f))	// �Q�[�W�̈ʒu
#define COL_FRONT_COUNTER	(XCOL_WHITE)	// �Q�[�W�̕\�Q�[�W�F
#define COL_BACK_COUNTER	(XCOL_AWHITE)	// �Q�[�W�̗��Q�[�W�F

#define COL_FADE_MIN	(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))	// �t�F�[�h�̃��l�̍ŏ�
#define COL_FADE_MAX	(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f))	// �t�F�[�h�̃��l�̍ő�
#define FADE_LEVEL		(0.025f)	// �t�F�[�h�̃��l�̉�����

#define POS_EXPLAIN		(D3DXVECTOR3(SCREEN_CENT.x, 315.0f, 0.0f))	// �����\���̈ʒu
#define SIZE_EXPLAIN	(D3DXVECTOR3(1000.0f, 597.5f, 0.0f))		// �����\���̑傫��
#define SET_SCALE		(0.05f)		// �g�嗦�̏����l
#define ADD_SCALE		(0.1f)		// �g�嗦�̉��Z��

#define POS_CLOSE	(D3DXVECTOR3(SCREEN_CENT.x, 665.0f, 0.0f))	// ������@�\���̈ʒu
#define SIZE_CLOSE	(D3DXVECTOR3(SCREEN_WIDTH, 100.0f, 0.0f))	// ������@�\���̑傫��
#define ADD_ROT		(0.02f)		// �_�Ō����̉��Z��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CTutorialManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\letter001.png",	// ������@�\���̃e�N�X�`��
	"data\\TEXTURE\\letter002.png",	// �J�n���̕�ⳃe�N�X�`��
	"data\\TEXTURE\\letter003.png",	// �I�����̕�ⳃe�N�X�`��
};

const char *CTutorialManager::mc_apLessonTextureFile[] =	// ���b�X���e�N�X�`���萔
{
	NULL,	// ���b�X��00�F�e�N�X�`���Ȃ�
	"data\\TEXTURE\\lesson000.png",	// ���b�X��01�F�z�����܂��	�����e�N�X�`��
	"data\\TEXTURE\\lesson001.png",	// ���b�X��02�F�O�����		�����e�N�X�`��
	"data\\TEXTURE\\lesson002.png",	// ���b�X��03�F���E����		�����e�N�X�`��
	"data\\TEXTURE\\lesson003.png",	// ���b�X��04�F�U��			�����e�N�X�`��
	"data\\TEXTURE\\lesson004.png",	// ���b�X��05�F�}�i��		�����e�N�X�`��
	"data\\TEXTURE\\lesson005.png",	// ���b�X��06�F�G�ւ̍U��	�����e�N�X�`��
};

const char *CTutorialManager::mc_apGuideTextureFile[] =	// �������e�N�X�`���萔
{
	NULL,	// ���b�X��00�F�e�N�X�`���Ȃ�
	"data\\TEXTURE\\guide000.png",	// ���b�X��01�F�z�����܂��	�������̃e�N�X�`��
	"data\\TEXTURE\\guide001.png",	// ���b�X��02�F�O�����		�������̃e�N�X�`��
	"data\\TEXTURE\\guide002.png",	// ���b�X��03�F���E����		�������̃e�N�X�`��
	"data\\TEXTURE\\guide003.png",	// ���b�X��04�F�U��			�������̃e�N�X�`��
	"data\\TEXTURE\\guide004.png",	// ���b�X��05�F�}�i��		�������̃e�N�X�`��
	"data\\TEXTURE\\guide005.png",	// ���b�X��06�F�G�ւ̍U��	�������̃e�N�X�`��
};

const char *CTutorialManager::mc_apControlTextureFile[] =	// ������@�e�N�X�`���萔
{
	NULL,	// ���b�X��00�F�e�N�X�`���Ȃ�
	NULL,	// ���b�X��01�F�e�N�X�`���Ȃ�
	"data\\TEXTURE\\control000.png",	// ���b�X��02�F�O�����	������@�̃e�N�X�`��
	"data\\TEXTURE\\control001.png",	// ���b�X��03�F���E����	������@�̃e�N�X�`��
	"data\\TEXTURE\\control002.png",	// ���b�X��04�F�U��		������@�̃e�N�X�`��
	NULL,	// ���b�X��05�F�e�N�X�`���Ȃ�
	NULL,	// ���b�X��06�F�e�N�X�`���Ȃ�
};

const int CTutorialManager::mc_aNextLesson[] =	// ���b�X���ڍs�J�E���g
{
	0,		// ���b�X���Ȃ�
	30,		// ���b�X��01�F�z�����܂��	�I���J�E���g
	320,	// ���b�X��02�F�O�����		�I���J�E���g
	320,	// ���b�X��03�F���E����		�I���J�E���g
	240,	// ���b�X��04�F�U��			�I���J�E���g
	70,		// ���b�X��05�F�}�i��		�I���J�E���g
	3,		// ���b�X��06�F�G�ւ̍U��	�I���J�E���g
};

const int CTutorialManager::mc_aNextLessonWait[] =	// �����b�X���]�C�J�E���g
{
	0,		// ���b�X���Ȃ�
	60,		// ���b�X��01�F�z�����܂��	�I�����̗]�C�J�E���g
	90,		// ���b�X��02�F�O�����		�I�����̗]�C�J�E���g
	90,		// ���b�X��03�F���E����		�I�����̗]�C�J�E���g
	90,		// ���b�X��04�F�U��			�I�����̗]�C�J�E���g
	120,	// ���b�X��05�F�}�i��		�I�����̗]�C�J�E���g
	90,		// ���b�X��06�F�G�ւ̍U��	�I�����̗]�C�J�E���g
};

//************************************************************
//	�e�N���X [CTutorialManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTutorialManager::CTutorialManager()
{
	// �����o�ϐ����N���A
	m_pLetterManager	= NULL;			// �莆�}�l�[�W���[�̏��
	m_pCounterLesson	= NULL;			// ���b�X���Ǘ��J�E���^�[�̏��
	m_pFade				= NULL;			// �t�F�[�h�̏��
	m_pExplain			= NULL;			// �����\���̏��
	m_pClose			= NULL;			// �����I���\���̏��
	m_pGuide			= NULL;			// �������\���̏��
	m_pGuideBG			= NULL;			// �������\���̔w�i���
	m_pControl			= NULL;			// ������@�\���̏��
	m_pControlBG		= NULL;			// ������@�\���̔w�i���
	m_state				= STATE_NONE;	// ���
	m_nLesson			= 0;			// ���b�X��
	m_fScale			= 0.0f;			// �|���S���g�嗦
	m_fSinRot			= 0.0f;			// �|���S���_�Ō���
	m_nCounterState		= 0;			// ��ԊǗ��J�E���^�[
}

//============================================================
//	�f�X�g���N�^
//============================================================
CTutorialManager::~CTutorialManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTutorialManager::Init(void)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����o�ϐ���������
	m_pLetterManager	= NULL;			// �莆�}�l�[�W���[�̏��
	m_pCounterLesson	= NULL;			// ���b�X���Ǘ��J�E���^�[�̏��
	m_pFade				= NULL;			// �t�F�[�h�̏��
	m_pExplain			= NULL;			// �����\���̏��
	m_pClose			= NULL;			// �����I���\���̏��
	m_pGuide			= NULL;			// �������\���̏��
	m_pGuideBG			= NULL;			// �������\���̔w�i���
	m_pControl			= NULL;			// ������@�\���̏��
	m_pControlBG		= NULL;			// ������@�\���̔w�i���
	m_state				= STATE_WAIT;	// ���
	m_nLesson			= LESSON_NONE;	// ���b�X��
	m_fScale			= 0.0f;			// �|���S���g�嗦
	m_fSinRot			= -HALF_PI;		// �|���S���_�Ō���
	m_nCounterState		= 0;			// ��ԊǗ��J�E���^�[

	//--------------------------------------------------------
	//	���b�X���Ǘ��J�E���^�[�̐����E�ݒ�
	//--------------------------------------------------------
	// ���b�X���Ǘ��J�E���^�[�̐���
	m_pCounterLesson = CObjectGauge2D::Create
	( // ����
		CObject::LABEL_GAUGE,	// �I�u�W�F�N�g���x��
		mc_aNextLesson[0],		// �ő�\���l
		FRAME_COUNTER,			// �\���l�ϓ��t���[��
		POS_COUNTER,			// �ʒu
		SIZE_COUNTER,			// �Q�[�W�傫��
		COL_FRONT_COUNTER,		// �\�Q�[�W�F
		COL_BACK_COUNTER		// ���Q�[�W�F
	);
	if (UNUSED(m_pCounterLesson))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pCounterLesson->SetPriority(TUTORIAL_PRIO);

	//--------------------------------------------------------
	//	�������\���̔w�i�����E�ݒ�
	//--------------------------------------------------------
	// �������\���̔w�i����
	m_pGuideBG = CObject2D::Create
	( // ����
		POS_GUIDE,				// �ʒu
		SIZE_PROGRESSION_UI,	// �傫��
		VEC3_ZERO,				// ����
		COL_PROGRESSION_UI_BG	// �F
	);
	if (UNUSED(m_pGuideBG))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pGuideBG->SetPriority(TUTORIAL_PRIO);

	//--------------------------------------------------------
	//	�������\���̐����E�ݒ�
	//--------------------------------------------------------
	// �������\���̐���
	m_pGuide = CObject2D::Create
	( // ����
		POS_GUIDE,			// �ʒu
		SIZE_PROGRESSION_UI	// �傫��
	);
	if (UNUSED(m_pGuide))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pGuide->SetPriority(TUTORIAL_PRIO);

	//--------------------------------------------------------
	//	������@�\���̔w�i�����E�ݒ�
	//--------------------------------------------------------
	// ������@�\���̔w�i����
	m_pControlBG = CObject2D::Create
	( // ����
		POS_CONTROL,			// �ʒu
		SIZE_PROGRESSION_UI,	// �傫��
		VEC3_ZERO,				// ����
		COL_PROGRESSION_UI_BG	// �F
	);
	if (UNUSED(m_pControlBG))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pControlBG->SetPriority(TUTORIAL_PRIO);

	//--------------------------------------------------------
	//	������@�\���̐����E�ݒ�
	//--------------------------------------------------------
	// ������@�\���̐���
	m_pControl = CObject2D::Create
	( // ����
		POS_CONTROL,		// �ʒu
		SIZE_PROGRESSION_UI	// �傫��
	);
	if (UNUSED(m_pControl))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pControl->SetPriority(TUTORIAL_PRIO);

	//--------------------------------------------------------
	//	�t�F�[�h�̐����E�ݒ�
	//--------------------------------------------------------
	// �t�F�[�h�̐���
	m_pFade = CObject2D::Create
	( // ����
		SCREEN_CENT,	// �ʒu
		SCREEN_SIZE,	// �傫��
		VEC3_ZERO,		// ����
		COL_FADE_MIN	// �F
	);
	if (UNUSED(m_pFade))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pFade->SetPriority(TUTORIAL_PRIO);

	//--------------------------------------------------------
	//	�I��w�i�̐����E�ݒ�
	//--------------------------------------------------------
	// �I��w�i�̐���
	m_pExplain = CObject2D::Create
	( // ����
		POS_EXPLAIN,	// �ʒu
		SIZE_EXPLAIN * SET_SCALE	// �傫��
	);
	if (UNUSED(m_pExplain))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pExplain->SetPriority(TUTORIAL_PRIO);

	// �`����~
	m_pExplain->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�����I���\���̐����E�ݒ�
	//--------------------------------------------------------
	// �����I���\���̐���
	m_pClose = CObject2D::Create
	( // ����
		POS_CLOSE,	// �ʒu
		SIZE_CLOSE,	// �傫��
		VEC3_ZERO,	// ����
		XCOL_AWHITE	// �F
	);
	if (UNUSED(m_pClose))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pClose->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_CONTROL]));

	// �D�揇�ʂ�ݒ�
	m_pClose->SetPriority(TUTORIAL_PRIO);

	//--------------------------------------------------------
	//	�莆�}�l�[�W���[�̐����E�ݒ�
	//--------------------------------------------------------
	// �莆�}�l�[�W���[�̐���
	m_pLetterManager = CLetterManager::Create();
	if (UNUSED(m_pLetterManager))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �i�s��Ԏ��ɕ\������UI�̕`����~
	SetEnableProgressionDraw(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CTutorialManager::Uninit(void)
{
	// �莆�}�l�[�W���[�̔j��
	if (FAILED(CLetterManager::Release(m_pLetterManager)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �������\���̔w�i�I��
	m_pGuideBG->Uninit();

	// �������\���̏I��
	m_pGuide->Uninit();

	// ������@�\���̔w�i�I��
	m_pControlBG->Uninit();

	// ������@�\���̏I��
	m_pControl->Uninit();

	// �t�F�[�h�̏I��
	m_pFade->Uninit();

	// �I��w�i�̏I��
	m_pExplain->Uninit();

	// �����I���̏I��
	m_pClose->Uninit();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CTutorialManager::Update(void)
{
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:		// �������Ȃ����

		// ����

		break;

	case STATE_WAIT:		// �ҋ@���

		// �ҋ@�̍X�V
		UpdateWait();

		break;

	case STATE_LETTER:		// �莆�\�����

		// �莆�\���̍X�V
		UpdateLetter();

		break;

	case STATE_FADEIN:		// �t�F�[�h�C�����

		// �t�F�[�h�C���̍X�V
		UpdateFadeIn();

		break;

	case STATE_EXPLAIN:		// �����\�����

		// �����\���̍X�V
		UpdateExplain();

		break;

	case STATE_FADEWAIT:	// �t�F�[�h�ҋ@���

		// �t�F�[�h�ҋ@�̍X�V
		UpdateFadeWait();

		break;

	case STATE_FADEOUT:		// �t�F�[�h�A�E�g���

		// �t�F�[�h�A�E�g�̍X�V
		UpdateFadeOut();

		break;

	case STATE_PROGRESSION:	// �i�s���

		// ����

		break;

	case STATE_NEXTWAIT:	// �����b�X���ҋ@���

		// �����b�X���ҋ@�̍X�V
		UpdateNextWait();

		break;

	case STATE_END:			// �I�����

		// �V�[���̐ݒ�
		CManager::SetScene(CScene::MODE_TITLE, TITLE_WAIT_CNT);	// �^�C�g�����

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	if (USED(m_pLetterManager))
	{ // �g�p���̏ꍇ

		// �莆�}�l�[�W���[�̍X�V
		m_pLetterManager->Update();
	}
	else { assert(false); }	// ��g�p��

	// �������\���̔w�i�X�V
	m_pGuideBG->Update();

	// �������\���̍X�V
	m_pGuide->Update();

	// ������@�\���̔w�i�X�V
	m_pControlBG->Update();

	// ������@�\���̍X�V
	m_pControl->Update();

	// �t�F�[�h�̍X�V
	m_pFade->Update();

	// �I��w�i�̍X�V
	m_pExplain->Update();

	// �����I���̍X�V
	m_pClose->Update();
}

//============================================================
//	���b�X���J�E���^�[�̉��Z����
//============================================================
void CTutorialManager::AddLessonCounter(void)
{
	if (m_nLesson < LESSON_MAX && m_state != STATE_NEXTWAIT)
	{ // ���b�X�����܂����銎�A�����b�X���̑ҋ@���ł͂Ȃ��ꍇ

		// ���b�X���J�E���^�[�����Z
		m_pCounterLesson->AddNum(1);

		if (m_pCounterLesson->GetNum() >= mc_aNextLesson[m_nLesson])
		{ // ���b�X�������ɐi�߂�J�E���g�܂œ��B�����ꍇ

			// ��Ԃ�ݒ�
			m_state = STATE_NEXTWAIT;	// �����b�X���ҋ@���

			// �T�E���h�̍Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// ���艹01
		}
	}
}

//============================================================
//	���b�X���擾����
//============================================================
int CTutorialManager::GetLesson(void) const
{
	// ���݂̃��b�X����Ԃ�
	return m_nLesson;
}

//============================================================
//	��Ԏ擾����
//============================================================
CTutorialManager::STATE CTutorialManager::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	��������
//============================================================
CTutorialManager *CTutorialManager::Create(void)
{
	// �|�C���^��錾
	CTutorialManager *pTutorialManager = NULL;	// �`���[�g���A���}�l�[�W���[�����p

	if (UNUSED(pTutorialManager))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pTutorialManager = new CTutorialManager;	// �`���[�g���A���}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pTutorialManager))
	{ // �g�p����Ă���ꍇ
		
		// �`���[�g���A���}�l�[�W���[�̏�����
		if (FAILED(pTutorialManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pTutorialManager;
			pTutorialManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pTutorialManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CTutorialManager::Release(CTutorialManager *&prTutorialManager)
{
	if (USED(prTutorialManager))
	{ // �g�p���̏ꍇ

		// �`���[�g���A���}�l�[�W���[�̏I��
		if (FAILED(prTutorialManager->Uninit()))
		{ // �j���Ɏ��s�����ꍇ

			// �������J��
			delete prTutorialManager;
			prTutorialManager = NULL;

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �������J��
		delete prTutorialManager;
		prTutorialManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	���b�X���ڍs�J�E���g�擾����
//============================================================
int CTutorialManager::GetNextLessonCounter(const int nID)
{
	if (nID < LESSON_MAX)
	{ // �C���f�b�N�X���͈͓��̏ꍇ

		// �����̃C���f�b�N�X�̃��b�X���ڍs�J�E���g��Ԃ�
		return mc_aNextLesson[nID];
	}
	else { assert(false); return NONE_IDX; }	// �͈͊O
}

//============================================================
//	�i�s��Ԏ��ɕ\������UI�̕`��󋵂̐ݒ菈��
//============================================================
void CTutorialManager::SetEnableProgressionDraw(const bool bDraw)
{
	//--------------------------------------------------------
	//	�����̕`��󋵂�ݒ�
	//--------------------------------------------------------
	// ���b�X���Ǘ��J�E���^�[�̕`��󋵂�ݒ�
	m_pCounterLesson->SetEnableDraw(bDraw);

	// �������\���̔w�i�`��󋵂�ݒ�
	m_pGuideBG->SetEnableDraw(bDraw);

	// �������\���̕`��󋵂�ݒ�
	m_pGuide->SetEnableDraw(bDraw);

	//--------------------------------------------------------
	//	�`��󋵂��󋵂ɉ����ĕύX���Đݒ�
	//--------------------------------------------------------
	// �ϐ���錾
	bool bDrawControl = bDraw;	// ������@�̕\����

	if (m_nLesson != LESSON_02	// ���b�X��02�F�O�����
	&&  m_nLesson != LESSON_03	// ���b�X��03�F���E����
	&&  m_nLesson != LESSON_04)	// ���b�X��04�F�U��
	{ // ��L�̃��b�X���̂�����ł��Ȃ��ꍇ

		// ������@��\�����Ȃ���Ԃɂ���
		bDrawControl = false;
	}

	// ������@�\���̔w�i�`��󋵂�ݒ�
	m_pControlBG->SetEnableDraw(bDrawControl);

	// ������@�\���̕`��󋵂�ݒ�
	m_pControl->SetEnableDraw(bDrawControl);
}

//============================================================
//	�ҋ@�̍X�V����
//============================================================
void CTutorialManager::UpdateWait(void)
{
	if (m_nCounterState < LETTER_WAIT_CNT)
	{ // �J�E���^�[���ҋ@���Ԗ����̏ꍇ

		// �J�E���^�[�����Z
		m_nCounterState++;
	}
	else
	{ // �ҋ@���Ԃ��I�������ꍇ

		// �J�E���^�[��������
		m_nCounterState = 0;

		// �莆�\���̊J�n
		m_pLetterManager->SetLook(mc_apTextureFile[TEXTURE_START]);

		// ��Ԃ�ݒ�
		m_state = STATE_LETTER;	// �莆�\�����
	}
}

//============================================================
//	�莆�\���̍X�V����
//============================================================
void CTutorialManager::UpdateLetter(void)
{
	if (m_pLetterManager->GetState() == CLetterManager::STATE_END)
	{ // �莆��ǂݏI�����ꍇ

		if (m_nLesson == LESSON_NONE)
		{ // ���b�X���J�n�O�̏ꍇ

			// �����b�X���ւ̈ڍs
			NextLesson();
		}
		else
		{ // ���b�X���I����̏ꍇ

			// ��Ԃ�ݒ�
			m_state = STATE_END;	// �I�����
		}
	}
}

//============================================================
//	�t�F�[�h�C���̍X�V����
//============================================================
void CTutorialManager::UpdateFadeIn(void)
{
	// �ϐ���錾
	D3DXCOLOR colFade = m_pFade->GetColor();	// �t�F�[�h�̐F

	if (colFade.a < COL_FADE_MAX.a)
	{ // �����ʂ��ݒ�l�����̏ꍇ

		// �����x�����Z
		colFade.a += FADE_LEVEL;
	}
	else
	{ // �����ʂ��ݒ�l�ȏ�̏ꍇ

		// �����x��␳
		colFade.a = COL_FADE_MAX.a;

		// �����\���̕`����ĊJ
		m_pExplain->SetEnableDraw(true);

		// �����\���̏����|���S���g�嗦��ݒ�
		m_fScale = SET_SCALE;

		// ��Ԃ�ݒ�
		m_state = STATE_EXPLAIN;	// �����\�����

		// �T�E���h�̍Đ�
		CManager::GetSound()->Play(CSound::LABEL_SE_PAPER);	// ��Ⳃ߂��艹
	}

	// �����x�𔽉f
	m_pFade->SetColor(colFade);
}

//============================================================
//	�����\���̍X�V����
//============================================================
void CTutorialManager::UpdateExplain(void)
{
	if (m_fScale < 1.0f)
	{ // �g�嗦���ŏ��l�����̏ꍇ

		// �g�嗦�����Z
		m_fScale += ADD_SCALE;

		// �����\���̑傫����ݒ�
		m_pExplain->SetScaling(SIZE_EXPLAIN * m_fScale);
	}
	else
	{ // �g�嗦���ŏ��l�ȉ��̏ꍇ

		// �����\���̑傫����ݒ�
		m_pExplain->SetScaling(SIZE_EXPLAIN);

		// ��Ԃ�ݒ�
		m_state = STATE_FADEWAIT;	// �t�F�[�h�ҋ@���
	}
}

//============================================================
//	�t�F�[�h�ҋ@�̍X�V����
//============================================================
void CTutorialManager::UpdateFadeWait(void)
{
	// �ϐ���錾
	D3DXCOLOR colControl = m_pClose->GetColor();	// �����I���\���̐F

	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetPad();		// �p�b�h

	if (pKeyboard->GetTrigger(DIK_RETURN)
	||  pKeyboard->GetTrigger(DIK_SPACE)
	||  pPad->GetTrigger(CInputPad::KEY_A)
	||  pPad->GetTrigger(CInputPad::KEY_B)
	||  pPad->GetTrigger(CInputPad::KEY_X)
	||  pPad->GetTrigger(CInputPad::KEY_Y)
	||  pPad->GetTrigger(CInputPad::KEY_START))
	{
		// �|���S���_�Ō�����������
		m_fSinRot = -HALF_PI;

		// �����\���̏����|���S���g�嗦��ݒ�
		m_fScale = SET_SCALE;

		// �����\���̑傫����ݒ�
		m_pExplain->SetScaling(SIZE_EXPLAIN * m_fScale);

		// �����\���̕`����~
		m_pExplain->SetEnableDraw(false);

		// ��Ԃ�ݒ�
		m_state = STATE_FADEOUT;	// �t�F�[�h�A�E�g���
	}

	// �����I���\���̓_�Ō��������Z
	m_fSinRot += ADD_ROT;
	useful::NormalizeRot(m_fSinRot);	// ������␳

	// �����I���\���̃��l��ύX
	colControl.a = (1.0f / 2.0f) * (sinf(m_fSinRot) + 1.0f);

	// �����I���\���̐F�𔽉f
	m_pClose->SetColor(colControl);
}

//============================================================
//	�t�F�[�h�A�E�g�̍X�V����
//============================================================
void CTutorialManager::UpdateFadeOut(void)
{
	// �ϐ���錾
	D3DXCOLOR colFade = m_pFade->GetColor();	// �t�F�[�h�̐F

	if (colFade.a > COL_FADE_MIN.a)
	{ // �����ʂ��ݒ�l���傫���ꍇ

		// �����x�����Z
		colFade.a -= FADE_LEVEL;
	}
	else
	{ // �����ʂ��ݒ�l�ȉ��̏ꍇ

		// �����x��␳
		colFade.a = COL_FADE_MIN.a;

		// �v���C���[���ďo��������
		CScene::GetPlayer()->SetRespawn(PLAY_SPAWN_POS);

		switch (m_nLesson)
		{ // ���b�X�����Ƃ̏���
		case LESSON_05:	// ���b�X��05�F�}�i��

			// �}�i�̉񕜂��ł���悤�ɕύX
			CScene::GetPlayer()->SetEnableHealMana(true);

			// �}�i����ɂ���
			CScene::GetPlayer()->SetMana(0);

			break;

		case LESSON_06:	// ���b�X��06�F�G�ւ̍U��

			// �G�����_������
			CEnemy::RandomSpawn(mc_aNextLesson[LESSON_06], CEnemy::TYPE_HUMAN);

			break;
		}

		// �i�s��Ԏ��ɕ\������UI�̕`����ĊJ
		SetEnableProgressionDraw(true);

		// ��Ԃ�ݒ�
		m_state = STATE_PROGRESSION;	// �i�s���
	}

	// �����x�𔽉f
	m_pFade->SetColor(colFade);
}

//============================================================
//	�����b�X���ҋ@�̍X�V����
//============================================================
void CTutorialManager::UpdateNextWait(void)
{
	if (m_nCounterState < mc_aNextLessonWait[m_nLesson])
	{ // �J�E���^�[���]�C���Ԗ����̏ꍇ

		// �J�E���^�[�����Z
		m_nCounterState++;
	}
	else
	{ // �]�C���Ԃ��I�������ꍇ

		// �J�E���^�[��������
		m_nCounterState = 0;

		// �����b�X���ւ̈ڍs
		NextLesson();

		// �i�s��Ԏ��ɕ\������UI�̕`����~
		SetEnableProgressionDraw(false);
	}
}

//============================================================
//	�����b�X���ւ̈ڍs����
//============================================================
void CTutorialManager::NextLesson(void)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^

	//--------------------------------------------------------
	//	���b�X���E�J�E���^�[���X�V
	//--------------------------------------------------------
	// ���b�X�������ɐi�߂�
	m_nLesson++;

	// ���b�X���J�E���^�[�̍ő�l��ݒ�
	m_pCounterLesson->SetMaxNum(mc_aNextLesson[m_nLesson]);

	// ���b�X���J�E���^�[��������
	m_pCounterLesson->SetNum(0);

	//--------------------------------------------------------
	//	���b�X���\�����X�V
	//--------------------------------------------------------
	// ���b�X�������e�N�X�`����o�^�E����
	m_pExplain->BindTexture(pTexture->Regist(mc_apLessonTextureFile[m_nLesson]));

	// ���b�X���������e�N�X�`����o�^�E����
	m_pGuide->BindTexture(pTexture->Regist(mc_apGuideTextureFile[m_nLesson]));

	// ���b�X��������@�e�N�X�`����o�^�E����
	m_pControl->BindTexture(pTexture->Regist(mc_apControlTextureFile[m_nLesson]));

	//--------------------------------------------------------
	//	��Ԃ��X�V
	//--------------------------------------------------------
	// �v���C���[�������Ȃ�����
	CScene::GetPlayer()->SetDisp(false);

	if (m_nLesson < LESSON_MAX)
	{ // ���b�X�����܂�����ꍇ

		// ��Ԃ�ݒ�
		m_state = STATE_FADEIN;	// �t�F�[�h�C�����
	}
	else
	{ // ���b�X���������Ȃ��ꍇ

		// �莆�\���̊J�n
		m_pLetterManager->SetLook(mc_apTextureFile[TEXTURE_END]);

		// ��Ԃ�ݒ�
		m_state = STATE_LETTER;	// �莆�\�����
	}
}
