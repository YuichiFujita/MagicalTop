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
#include "camera.h"
#include "texture.h"
#include "object2D.h"
#include "objectGauge2D.h"
#include "player.h"

//************************************************************
//	�}�N����`
//************************************************************
#define TUTORIAL_PRIO	(5)	// �`���[�g���A���̗D�揇��

#define WAIT_CNT	(90)	// �ҋ@��Ԃ̑ҋ@�t���[��

#define COL_FADE_MIN	(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))	// �t�F�[�h�̃��l�̍ŏ�
#define COL_FADE_MAX	(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f))	// �t�F�[�h�̃��l�̍ő�
#define FADE_LEVEL		(0.025f)	// �t�F�[�h�̃��l�̉�����

#define SIZE_EXPLAIN	(D3DXVECTOR3(1000.0f, 580.0f, 0.0f))	// �����\���̑傫��
#define SET_SCALE		(0.05f)		// �g�嗦�̏����l
#define ADD_SCALE		(0.1f)		// �g�嗦�̉��Z��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CTutorialManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\tutorial000.png",	// �e�N�X�`��
};

const char *CTutorialManager::mc_apLessonTextureFile[] =	// ���b�X���e�N�X�`���萔
{
	NULL,	// ���b�X��00�F�e�N�X�`���Ȃ�
	"data\\TEXTURE\\lesson000.png",	// ���b�X��01�F�z�����܂������̃e�N�X�`��
	"data\\TEXTURE\\lesson001.png",	// ���b�X��02�F�O������̐����̃e�N�X�`��
	"data\\TEXTURE\\lesson002.png",	// ���b�X��03�F���E�����̐����̃e�N�X�`��
};

const int CTutorialManager::mc_aNextLesson[] =	// ���b�X���ڍs�J�E���g
{
	0,		// ���b�X���Ȃ�
	1,		// ���b�X��01�F�z�����܂��I���J�E���g
	60,		// ���b�X��02�F�O������̏I���J�E���g
	60,		// ���b�X��03�F���E�����̏I���J�E���g
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
	m_pConterLesson	= NULL;		// ���b�X���Ǘ��J�E���^�[�̏��
	m_pFade		= NULL;			// �t�F�[�h�̏��
	m_pExplain	= NULL;			// �����\���̏��
	m_state		= STATE_NONE;	// ���
	m_nLesson	= 0;			// ���b�X��
	m_fScale	= 0.0f;			// �|���S���g�嗦
	m_nCounterState	= 0;		// ��ԊǗ��J�E���^�[
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
	m_pConterLesson	= NULL;		// ���b�X���Ǘ��J�E���^�[�̏��
	m_pFade		= NULL;			// �t�F�[�h�̏��
	m_pExplain	= NULL;			// �����\���̏��
	m_state		= STATE_WAIT;	// ���
	m_nLesson	= LESSON_NONE;	// ���b�X��
	m_fScale	= 0.0f;			// �|���S���g�嗦
	m_nCounterState	= 0;		// ��ԊǗ��J�E���^�[

	//--------------------------------------------------------
	//	���b�X���Ǘ��J�E���^�[�̐����E�ݒ�
	//--------------------------------------------------------
	// ���b�X���Ǘ��J�E���^�[�̐���
	m_pConterLesson = CObjectGauge2D::Create	// TODO:�萔
	( // ����
		CObject::LABEL_GAUGE,
		mc_aNextLesson[0],
		20,
		D3DXVECTOR3(200.0f, 200.0f, 0.0f),
		D3DXVECTOR3(100.0f, 20.0f, 0.0f)
	);
	if (UNUSED(m_pConterLesson))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

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
		SCREEN_CENT,	// �ʒu
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

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTutorialManager::Uninit(void)
{
	// �t�F�[�h�̏I��
	m_pFade->Uninit();

	// �I��w�i�̏I��
	m_pExplain->Uninit();
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

	case STATE_PROGRESSION:	// �i�s���

		// ����

		break;

	case STATE_FADEOUT:		// �t�F�[�h�A�E�g���

		// �t�F�[�h�A�E�g�̍X�V
		UpdateFadeOut();

		break;

	case STATE_END:			// �I�����

		// ����

		break;

	default:	// ��O����
		assert(false);
		break;
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
//	�����b�X���ւ̈ڍs����
//============================================================
void CTutorialManager::NextLesson(void)
{
	if (m_nLesson < LESSON_MAX - 1)
	{ // ���b�X�����܂�����ꍇ

		// ���b�X���J�E���^�[�����Z
		m_pConterLesson->AddNum(1);

		if (m_pConterLesson->GetNum() >= mc_aNextLesson[m_nLesson])
		{ // ���b�X�������ɐi�߂�J�E���g�܂œ��B�����ꍇ

			// ���b�X�������ɐi�߂�
			m_nLesson++;

			switch (m_nLesson)
			{ // ���b�X�����Ƃ̏���
			case LESSON_01:	// ���b�X��01�F�z�����܂��

				// ����

				// �����𔲂���
				break;

			case LESSON_02:	// ���b�X��02�F�O�����

				// ����

				// �����𔲂���
				break;

			case LESSON_03:	// ���b�X��03�F���E����

				// ����

				// �����𔲂���
				break;

			default:	// ��O����
				assert(false);
				break;
			}

			// ���b�X���J�E���^�[�̍ő�l��ݒ�
			m_pConterLesson->SetMaxNum(mc_aNextLesson[m_nLesson]);

			// ���b�X���J�E���^�[��������
			m_pConterLesson->SetNum(0);

			// �v���C���[�������Ȃ�����
			CScene::GetPlayer()->SetDisp(false);

			// ��Ԃ�ݒ�
			m_state = STATE_FADEIN;	// �t�F�[�h�C�����

			//// �T�E���h�̍Đ�
			//PlaySound(SOUND_LABEL_SE_SCORE_000);	// SE (����00)
		}
	}
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
		prTutorialManager->Uninit();

		// �������J��
		delete prTutorialManager;
		prTutorialManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�ҋ@�̍X�V����
//============================================================
void CTutorialManager::UpdateWait(void)
{
	if (m_nCounterState < WAIT_CNT)
	{ // �J�E���^�[���ҋ@���Ԗ����̏ꍇ

		// �J�E���^�[�����Z
		m_nCounterState++;
	}
	else
	{ // �ҋ@���Ԃ��I�������ꍇ

		// �J�E���^�[��������
		m_nCounterState = 0;

		// ��Ԃ�ݒ�
		m_state = STATE_FADEIN;	// �t�F�[�h�C�����
	}
}

//============================================================
//	�莆�\���̍X�V����
//============================================================
void CTutorialManager::UpdateLetter(void)
{
	// �����b�X���ւ̈ڍs
	NextLesson();
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

		if (m_nLesson == LESSON_NONE)
		{ // ���b�X��������̏ꍇ

			// ��Ԃ�ݒ�
			m_state = STATE_LETTER;		// �莆�\�����
		}
		else
		{ // ���b�X��������ł͂Ȃ��ꍇ

			// �����\���̕`����ĊJ
			m_pExplain->SetEnableDraw(true);

			// �����\���̏����|���S���g�嗦��ݒ�
			m_fScale = SET_SCALE;

			// ��Ԃ�ݒ�
			m_state = STATE_EXPLAIN;	// �����\�����
		}
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

		if (CManager::GetKeyboard()->GetTrigger(DIK_0))
		{ // TODO�F����

			// �����\���̏����|���S���g�嗦��ݒ�
			m_fScale = SET_SCALE;

			// �����\���̑傫����ݒ�
			m_pExplain->SetScaling(SIZE_EXPLAIN * m_fScale);

			// �����\���̕`����~
			m_pExplain->SetEnableDraw(false);

			// ��Ԃ�ݒ�
			m_state = STATE_FADEOUT;	// �t�F�[�h�A�E�g���
		}
	}
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

		// ��Ԃ�ݒ�
		m_state = STATE_PROGRESSION;	// �i�s���
	}

	// �����x�𔽉f
	m_pFade->SetColor(colFade);
}
