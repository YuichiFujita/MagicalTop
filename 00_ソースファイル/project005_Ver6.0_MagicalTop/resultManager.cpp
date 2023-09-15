//============================================================
//
//	���U���g�}�l�[�W���[���� [resultManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "resultManager.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "texture.h"
#include "model.h"
#include "object2D.h"
#include "score.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CResultManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\result000.png",	// MISSION�e�N�X�`��
	"data\\TEXTURE\\result001.png",	// CLEAR�e�N�X�`��
	"data\\TEXTURE\\result002.png",	// FAILED�e�N�X�`��
	"data\\TEXTURE\\result003.png",	// �X�R�A�\���e�N�X�`��
	"data\\TEXTURE\\result004.png",	// �^�C���\���e�N�X�`��
};

//************************************************************
//	�}�N����`
//************************************************************
#define RESULT_PRIO	(6)		// ���U���g�̗D�揇��
#define WAIT_CNT	(10)	// ��ԕύX�ҋ@�t���[����

#define SIZE_FADE	(SCREEN_SIZE * 0.95f)	// �t�F�[�h�̑傫��
#define ADD_ALPHA	(0.008f)	// ���l�̉��Z��
#define SET_ALPHA	(0.6f)		// ���l�̒�~�l

#define POS_RESULT_MISSION	(D3DXVECTOR3(360.0f, 160.0f, 0.0f))	// ���U���g�\����MISSION�ʒu
#define POS_RESULT_RESULT	(D3DXVECTOR3(920.0f, 160.0f, 0.0f))	// ���U���g�\����RESULT�ʒu
#define SIZE_RESULT			(D3DXVECTOR3(632.7f, 203.5f, 0.0f))	// ���U���g�\���̑傫��
#define SET_RESULT_SCALE	(15.0f)	// ���U���g�\���̏����g�嗦
#define SUB_RESULT_SCALE	(0.65f)	// ���U���g�\���g�嗦�̌��Z��

#define POS_SCORE_LOGO	(D3DXVECTOR3(250.0f, 340.0f, 0.0f))		// �X�R�A���S�ʒu
#define SIZE_SCORE_LOGO	(D3DXVECTOR3(487.5f, 154.7f, 0.0f))		// �X�R�A���S�傫��
#define POS_SCORE		(D3DXVECTOR3(490.0f, 340.0f, 0.0f))		// �X�R�A�ʒu
#define SIZE_SCORE		(D3DXVECTOR3(94.0f, 112.0f, 0.0f))		// �X�R�A�傫��
#define SPACE_SCORE		(D3DXVECTOR3(SIZE_SCORE.x, 0.0f, 0.0f))	// �X�R�A��
#define SET_SCORE_SCALE	(8.0f)	// �X�R�A�\���̏����g�嗦
#define SUB_SCORE_SCALE	(0.95f)	// �X�R�A�\���g�嗦�̌��Z��

//************************************************************
//	�e�N���X [CResultManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CResultManager::CResultManager()
{
	// �����o�ϐ����N���A
	memset(&m_apResult[0], 0, sizeof(m_apResult));	// ���U���g�\���̏��
	m_pScoreLogo	= NULL;			// �X�R�A���S�̏��
	m_pFade			= NULL;			// �t�F�[�h�̏��
	m_pScore		= NULL;			// �X�R�A�̏��
	m_state			= STATE_NONE;	// ���
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[
	m_fScale		= 0.0f;			// �|���S���g�嗦
}

//============================================================
//	�f�X�g���N�^
//============================================================
CResultManager::~CResultManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CResultManager::Init(void)
{
	// �ϐ��z���錾
	D3DXVECTOR3 aPosResult[] =	// ���U���g�̈ʒu
	{ // �����l
		POS_RESULT_MISSION,	// MISSION�ʒu
		POS_RESULT_RESULT,	// RESULT�ʒu
	};

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����o�ϐ���������
	memset(&m_apResult[0], 0, sizeof(m_apResult));	// ���U���g�\���̏��
	m_pScoreLogo	= NULL;			// �X�R�A���S�̏��
	m_pFade			= NULL;			// �t�F�[�h�̏��
	m_pScore		= NULL;			// �X�R�A�̏��
	m_state			= STATE_FADEIN;	// ���
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[
	m_fScale		= 0.0f;			// �|���S���g�嗦

	//--------------------------------------------------------
	//	�t�F�[�h�̐����E�ݒ�
	//--------------------------------------------------------
	// �t�F�[�h�̐���
	m_pFade = CObject2D::Create
	( // ����
		SCREEN_CENT,	// �ʒu
		SIZE_FADE,		// �傫��
		VEC3_ZERO,		// ����
		XCOL_AWHITE		// �F
	);
	if (UNUSED(m_pFade))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pFade->SetPriority(RESULT_PRIO);

	//--------------------------------------------------------
	//	���U���g�\���̐����E�ݒ�
	//--------------------------------------------------------
	for (int nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
	{ // ���U���g�\���̑������J��Ԃ�

		// ���U���g�\���̐���
		m_apResult[nCntResult] = CObject2D::Create
		( // ����
			aPosResult[nCntResult],			// �ʒu
			SIZE_RESULT * SET_RESULT_SCALE	// �傫��
		);
		if (UNUSED(m_apResult[nCntResult]))
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �D�揇�ʂ�ݒ�
		m_apResult[nCntResult]->SetPriority(RESULT_PRIO);

		// �`������Ȃ��ݒ�ɂ���
		m_apResult[nCntResult]->SetEnableDraw(false);
	}

	// ���U���g�\���̃e�N�X�`����ݒ�
	SetTexResult();

	//--------------------------------------------------------
	//	�X�R�A���S�\���̐����E�ݒ�
	//--------------------------------------------------------
	// �X�R�A���S�\���̐���
	m_pScoreLogo = CObject2D::Create
	( // ����
		POS_SCORE_LOGO,						// �ʒu
		SIZE_SCORE_LOGO * SET_SCORE_SCALE	// �傫��
	);
	if (UNUSED(m_pScoreLogo))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pScoreLogo->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_SCORE]));

	// �D�揇�ʂ�ݒ�
	m_pScoreLogo->SetPriority(RESULT_PRIO);

	// �`������Ȃ��ݒ�ɂ���
	m_pScoreLogo->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�X�R�A�\���̐����E�ݒ�
	//--------------------------------------------------------
	// �X�R�A�I�u�W�F�N�g�̐���
	m_pScore = CScore::Create
	( // ����
		POS_SCORE,						// �ʒu
		SIZE_SCORE * SET_SCORE_SCALE,	// �傫��
		SPACE_SCORE						// ��
	);
	if (UNUSED(m_pScore))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pScore->SetPriority(RESULT_PRIO);

	// �`������Ȃ��ݒ�ɂ���
	m_pScore->SetEnableDraw(false);

	// �X�R�A��ݒ�
	m_pScore->Set(12345678);	// TODO�F�l���X�R�A�̕ێ�

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CResultManager::Uninit(void)
{
	for (int nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
	{ // ���U���g�\���̑������J��Ԃ�

		// ���U���g�\���̏I��
		m_apResult[nCntResult]->Uninit();
	}

	// �X�R�A���S�\���̏I��
	m_pScoreLogo->Uninit();

	// �t�F�[�h�̏I��
	m_pFade->Uninit();

	// �I���ς݂̃I�u�W�F�N�g�|�C���^��NULL�ɂ���
	m_pScore = NULL;	// �X�R�A�I�u�W�F�N�g
}

//============================================================
//	�X�V����
//============================================================
void CResultManager::Update(void)
{
	// �J�ڌ���̍X�V
	UpdateBack();

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:	// �������Ȃ����

		// ����

		break;

	case STATE_FADEIN:	// �t�F�[�h�C�����

		// �t�F�[�h�C���̍X�V
		UpdateFade();

		break;

	case STATE_RESULT:	// ���U���g�\�����

		// ���U���g�\���̍X�V
		UpdateResult();

		break;

	case STATE_SCORE_WAIT:	// �X�R�A�\���ҋ@���

		// �\���ҋ@�̍X�V
		if (UpdateDrawWait())
		{ // �ҋ@�����̏ꍇ

			// �X�R�A�\���̊g�嗦��ݒ�
			m_fScale = SET_SCORE_SCALE;

			// �X�R�A�\���̕`��J�n
			m_pScoreLogo->SetEnableDraw(true);
			m_pScore->SetEnableDraw(true);

			// ��Ԃ�ύX
			m_state = STATE_SCORE;	// �X�R�A�\�����
		}

		break;

	case STATE_SCORE:	// �X�R�A�\�����

		// �X�R�A�\���̍X�V
		UpdateScore();

		break;

	case STATE_WAIT:	// �J�ڑҋ@���

		// ����

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	for (int nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
	{ // ���U���g�\���̑������J��Ԃ�

		// ���U���g�\���̍X�V
		m_apResult[nCntResult]->Update();
	}

	// �X�R�A���S�\���̍X�V
	m_pScoreLogo->Update();

	// �t�F�[�h�̍X�V
	m_pFade->Update();
}

//============================================================
//	��������
//============================================================
CResultManager *CResultManager::Create(void)
{
	// �|�C���^��錾
	CResultManager *pResultManager = NULL;	// ���U���g�}�l�[�W���[�����p

	if (UNUSED(pResultManager))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pResultManager = new CResultManager;	// ���U���g�}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pResultManager))
	{ // �g�p����Ă���ꍇ
		
		// ���U���g�}�l�[�W���[�̏�����
		if (FAILED(pResultManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pResultManager;
			pResultManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pResultManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CResultManager::Release(CResultManager *&prResultManager)
{
	if (USED(prResultManager))
	{ // �g�p���̏ꍇ

		// ���U���g�}�l�[�W���[�̏I��
		prResultManager->Uninit();

		// �������J��
		delete prResultManager;
		prResultManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�t�F�[�h�C���̍X�V����
//============================================================
void CResultManager::UpdateFade(void)
{
	// �ϐ���錾
	D3DXCOLOR colFade = m_pFade->GetColor();	// �t�F�[�h�̐F

	if (colFade.a < SET_ALPHA)
	{ // �����ʂ��ݒ�l�����̏ꍇ

		// �����x�����Z
		colFade.a += ADD_ALPHA;
	}
	else
	{ // �����ʂ��ݒ�l�ȏ�̏ꍇ

		// �����x��␳
		colFade.a = SET_ALPHA;

		for (int nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
		{ // ���U���g�\���̑������J��Ԃ�

			// ���U���g�\���̕`��J�n
			m_apResult[nCntResult]->SetEnableDraw(true);
		}

		// ���U���g�\���̊g�嗦��ݒ�
		m_fScale = SET_RESULT_SCALE;

		// ��Ԃ�ύX
		m_state = STATE_RESULT;	// ���U���g�\�����
	}

	// �����x�𔽉f
	m_pFade->SetColor(colFade);
}

//============================================================
//	���U���g�\���̍X�V����
//============================================================
void CResultManager::UpdateResult(void)
{
	if (m_fScale > 1.0f)
	{ // �g�嗦���ŏ��l���傫���ꍇ

		// �g�嗦�����Z
		m_fScale -= SUB_RESULT_SCALE;

		for (int nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
		{ // ���U���g�\���̑������J��Ԃ�

			// ���U���g�\���̑傫����ݒ�
			m_apResult[nCntResult]->SetScaling(SIZE_RESULT * m_fScale);
		}
	}
	else
	{ // �g�嗦���ŏ��l�ȉ��̏ꍇ

		for (int nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
		{ // ���U���g�\���̑������J��Ԃ�

			// ���U���g�\���̑傫����ݒ�
			m_apResult[nCntResult]->SetScaling(SIZE_RESULT);
		}

		// ��Ԃ�ύX
		m_state = STATE_SCORE_WAIT;	// �X�R�A�\���ҋ@���
	}
}

//============================================================
//	�X�R�A�\���̍X�V����
//============================================================
void CResultManager::UpdateScore(void)
{
	if (m_fScale > 1.0f)
	{ // �g�嗦���ŏ��l���傫���ꍇ

		// �g�嗦�����Z
		m_fScale -= SUB_SCORE_SCALE;

		// �X�R�A�\���̑傫����ݒ�
		m_pScoreLogo->SetScaling(SIZE_SCORE_LOGO * m_fScale);
		m_pScore->SetScaling(SIZE_SCORE * m_fScale);
	}
	else
	{ // �g�嗦���ŏ��l�ȉ��̏ꍇ

		// �g�嗦��␳
		m_fScale = 1.0f;

		// �X�R�A�\���̑傫����ݒ�
		m_pScoreLogo->SetScaling(SIZE_SCORE_LOGO);
		m_pScore->SetScaling(SIZE_SCORE);

		// ��Ԃ�ύX
		//m_state = STATE_SCORE;	// �X�R�A�\�����	// TODO
	}
}

//============================================================
//	�\���ҋ@����
//============================================================
bool CResultManager::UpdateDrawWait(void)
{
	if (m_nCounterState < WAIT_CNT)
	{ // �J�E���^�[���ҋ@�J�E���g�܂ŒB���Ă��Ȃ��ꍇ

		// �J�E���^�[�����Z
		m_nCounterState++;

		// �ҋ@��������Ԃ�
		return false;
	}
	else
	{ // �J�E���^�[���ҋ@���������ꍇ

		// �J�E���^�[��������
		m_nCounterState = 0;

		// �ҋ@������Ԃ�
		return true;
	}
}

//============================================================
//	�J�ڌ���̍X�V����
//============================================================
void CResultManager::UpdateBack(void)
{
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
		if (m_state != STATE_WAIT)
		{ // �J�ڑҋ@��Ԃł͂Ȃ��ꍇ

			// ���o�X�L�b�v
			SkipStaging();
		}
		else
		{ // �J�ڑҋ@��Ԃ̏ꍇ

			// �V�[���̐ݒ�
			CManager::SetScene(CScene::MODE_TITLE);	// �^�C�g�����
		}
	}
}

//============================================================
//	���o�X�L�b�v����
//============================================================
void CResultManager::SkipStaging(void)
{
#if 0

	// ���U���g���S��\����Ԃɐݒ�E�傫���𐳋K��
	for (int nCntResult = 0; nCntResult < LOGO_MAX; nCntResult++)
	{ // ���U���g���S�̑������J��Ԃ�

		// ���U���g���S�̑傫����ݒ�
		m_apLogo[nCntResult]->SetScaling(SIZE_RESULT);

		// �`�������ݒ�ɂ���
		m_apLogo[nCntResult]->SetEnableDraw(true);
	}

	// �I��\����`�悷��ݒ�ɂ���
	for (int nCntResult = 0; nCntResult < SELECT_MAX; nCntResult++)
	{ // �I�����ڂ̑������J��Ԃ�

		m_apSelect[nCntResult]->SetEnableDraw(true);
	}

	// �t�F�[�h�𓧖��ɂ���
	m_pFade->SetColor(XCOL_ABLACK);

	// �I��w�i��`�悷��ݒ�ɂ���
	m_pSelectBG->SetEnableDraw(true);

	// �J�����̍X�V���ĊJ
	CManager::GetCamera()->SetEnableUpdate(true);

#endif

	// ��Ԃ�ύX
	m_state = STATE_WAIT;	// �J�ڑҋ@���
}

//============================================================
//	���U���g�\���̃e�N�X�`���̐ݒ菈��
//============================================================
void CResultManager::SetTexResult(void)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// MISSION�e�N�X�`����o�^�E����
	m_apResult[0]->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_MISSION]));

	// RESULT�e�N�X�`����o�^�E����
	m_apResult[1]->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_FAILED]));
}
