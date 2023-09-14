//============================================================
//
//	�^�C�g���}�l�[�W���[���� [titleManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "titleManager.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "texture.h"
#include "model.h"
#include "object2D.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CTitleManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\title000.png",	// �^�C�g���\���e�N�X�`��
};
const char *CTitleManager::mc_apLogoTextureFile[] =	// ���S�e�N�X�`���萔
{
	"data\\TEXTURE\\title000.png",	// MAGICAL�e�N�X�`��
	"data\\TEXTURE\\title001.png",	// TOP�e�N�X�`��
};

//************************************************************
//	�}�N����`
//************************************************************
#define TITLE_PRIO	(6)	// �^�C�g���̗D�揇��

#define POS_LOGO_MAGICAL	(D3DXVECTOR3(440.0f, 160.0f, 0.0f))	// �^�C�g�����S�̈ʒu (MAGICAL)
#define POS_LOGO_TOP		(D3DXVECTOR3(930.0f, 160.0f, 0.0f))	// �^�C�g�����S�̈ʒu (TOP)

#define SIZE_TITLE	(D3DXVECTOR3(666.0f, 214.2f, 0.0f))	// �^�C�g�����S�̑傫��
#define INIT_SCALE	(15.0f)	// �^�C�g�����S�̏����g�嗦
#define SUB_SCALE	(0.65f)	// �^�C�g�����S�g�嗦�̌��Z��

#define COL_FADE	(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f))	// �^�C�g���t�F�[�h�̐F
#define SUB_ALPHA	(0.008f)	// ���l�̌��Z��

//************************************************************
//	�e�N���X [CTitleManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTitleManager::CTitleManager()
{
	// �����o�ϐ����N���A
	memset(&m_apLogo[0], 0, sizeof(m_apLogo));	// �^�C�g���\���̏��
	m_pFade = NULL;			// �t�F�[�h�̏��
	m_state = STATE_NONE;	// ���
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[
	m_fScale = 0.0f;		// �^�C�g���g�嗦
}

//============================================================
//	�f�X�g���N�^
//============================================================
CTitleManager::~CTitleManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTitleManager::Init(void)
{
	// �ϐ��z���錾
	D3DXVECTOR3 aPosLogo[] =	// ���S�̈ʒu
	{ // �����l
		POS_LOGO_MAGICAL,	// MAGICAL�ʒu
		POS_LOGO_TOP,		// TOP�ʒu
	};

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^
	CModel *pModel = CManager::GetModel();			// ���f���ւ̃|�C���^

	// �����o�ϐ���������
	memset(&m_apLogo[0], 0, sizeof(m_apLogo));	// �^�C�g���\���̏��
	m_pFade = NULL;				// �t�F�[�h�̏��
	m_state = STATE_FADEOUT;	// ���
	m_nCounterState = 0;		// ��ԊǗ��J�E���^�[
	m_fScale = INIT_SCALE;		// �^�C�g���g�嗦

	//--------------------------------------------------------
	//	�t�F�[�h�̐����E�ݒ�
	//--------------------------------------------------------
	// �t�F�[�h�̐���
	m_pFade = CObject2D::Create
	( // ����
		SCREEN_CENT,	// �ʒu
		SCREEN_SIZE,	// �傫��
		VEC3_ZERO,		// ����
		COL_FADE		// �F
	);
	if (UNUSED(m_pFade))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pFade->SetPriority(TITLE_PRIO);

	//--------------------------------------------------------
	//	�^�C�g�����S�̐����E�ݒ�
	//--------------------------------------------------------
	for (int nCntTitle = 0; nCntTitle < NUM_LOGO; nCntTitle++)
	{ // �^�C�g�����S�̐����J��Ԃ�

		// �^�C�g�����S�̐���
		m_apLogo[nCntTitle] = CObject2D::Create
		( // ����
			aPosLogo[nCntTitle],	// �ʒu
			SIZE_TITLE	// �傫��
		);
		if (UNUSED(m_apLogo[nCntTitle]))
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^�E����
		m_apLogo[nCntTitle]->BindTexture(pTexture->Regist(mc_apLogoTextureFile[nCntTitle]));

		// �D�揇�ʂ�ݒ�
		m_apLogo[nCntTitle]->SetPriority(TITLE_PRIO);

		// �`������Ȃ��ݒ�ɂ���
		m_apLogo[nCntTitle]->SetEnableDraw(false);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTitleManager::Uninit(void)
{
	// �t�F�[�h�̏I��
	m_pFade->Uninit();

	for (int nCntTitle = 0; nCntTitle < NUM_LOGO; nCntTitle++)
	{ // �^�C�g�����S�̐����J��Ԃ�

		// �^�C�g�����S�̏I��
		m_apLogo[nCntTitle]->Uninit();
	}
}

//============================================================
//	�X�V����
//============================================================
void CTitleManager::Update(void)
{
	// �J�ڌ���̍X�V
	UpdateStart();

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:		// �������Ȃ����

		// ����

		break;

	case STATE_FADEOUT:		// �t�F�[�h�A�E�g���

		// �t�F�[�h�A�E�g�̍X�V
		UpdateFade();

		break;

	case STATE_MOVE:		// �^�C�g���k�����

		// �^�C�g���ړ��̍X�V
		UpdateMove();

		break;

	case STATE_WAIT:		// �J�ڑҋ@���

		// ����

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �t�F�[�h�̍X�V
	m_pFade->Update();

	for (int nCntTitle = 0; nCntTitle < NUM_LOGO; nCntTitle++)
	{ // �^�C�g�����S�̐����J��Ԃ�

		// �^�C�g�����S�̍X�V
		m_apLogo[nCntTitle]->Update();
	}
}

//============================================================
//	��������
//============================================================
CTitleManager *CTitleManager::Create(void)
{
	// �|�C���^��錾
	CTitleManager *pTitleManager = NULL;	// �^�C�g���}�l�[�W���[�����p

	if (UNUSED(pTitleManager))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pTitleManager = new CTitleManager;	// �^�C�g���}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pTitleManager))
	{ // �g�p����Ă���ꍇ
		
		// �^�C�g���}�l�[�W���[�̏�����
		if (FAILED(pTitleManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pTitleManager;
			pTitleManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pTitleManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CTitleManager::Release(CTitleManager *&prTitleManager)
{
	if (USED(prTitleManager))
	{ // �g�p���̏ꍇ

		// �^�C�g���}�l�[�W���[�̏I��
		prTitleManager->Uninit();

		// �������J��
		delete prTitleManager;
		prTitleManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�t�F�[�h�A�E�g�̍X�V����
//============================================================
void CTitleManager::UpdateFade(void)
{
	// �ϐ���錾
	float fAlpha = m_pFade->GetColor().a;	// �t�F�[�h�����x

	if (fAlpha > 0.0f)
	{ // �����ɂȂ��Ă��Ȃ��ꍇ

		// �����x�����Z
		fAlpha -= SUB_ALPHA;
	}
	else
	{ // �����ɂȂ����ꍇ

		// �����x��␳
		fAlpha = 0.0f;

		for (int nCntTitle = 0; nCntTitle < NUM_LOGO; nCntTitle++)
		{ // �^�C�g�����S�̐����J��Ԃ�

			// �`�������ݒ�ɂ���
			m_apLogo[nCntTitle]->SetEnableDraw(true);
		}

		// ��Ԃ�ύX
		m_state = STATE_MOVE;	// �^�C�g���ړ����
	}

	// �����x�𔽉f
	m_pFade->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, fAlpha));
}

//============================================================
//	�^�C�g���ړ��̍X�V����
//============================================================
void CTitleManager::UpdateMove(void)
{
	if (m_fScale > 1.0f)
	{ // �g�嗦���ŏ��l���傫���ꍇ

		// �g�嗦�����Z
		m_fScale -= SUB_SCALE;

		for (int nCntTitle = 0; nCntTitle < NUM_LOGO; nCntTitle++)
		{ // �^�C�g�����S�̐����J��Ԃ�

			// �^�C�g�����S�̑傫����ݒ�
			m_apLogo[nCntTitle]->SetScaling(SIZE_TITLE * m_fScale);
		}
	}
	else
	{ // �g�嗦���ŏ��l�ȉ��̏ꍇ

		// �g�嗦��␳
		m_fScale = 1.0f;

		for (int nCntTitle = 0; nCntTitle < NUM_LOGO; nCntTitle++)
		{ // �^�C�g�����S�̐����J��Ԃ�

			// �^�C�g�����S�̑傫����ݒ�
			m_apLogo[nCntTitle]->SetScaling(SIZE_TITLE);
		}

		// ��Ԃ�ύX
		m_state = STATE_WAIT;	// �J�ڑҋ@���

		// �J�����̍X�V���ĊJ
		CManager::GetCamera()->SetEnableUpdate(true);
	}
}

//============================================================
//	�J�ڌ���̍X�V����
//============================================================
void CTitleManager::UpdateStart(void)
{
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard = CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad = CManager::GetPad();	// �p�b�h

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
			CManager::SetScene(CScene::MODE_GAME);	// �Q�[�����
		}
	}
}

//============================================================
//	���o�X�L�b�v����
//============================================================
void CTitleManager::SkipStaging(void)
{
	// �t�F�[�h�𓧖��ɂ���
	m_pFade->SetColor(XCOL_ABLACK);

	// �^�C�g�����S��\����Ԃɐݒ�E�傫���𐳋K��
	for (int nCntTitle = 0; nCntTitle < NUM_LOGO; nCntTitle++)
	{ // �^�C�g�����S�̐����J��Ԃ�

		// �^�C�g�����S�̑傫����ݒ�
		m_apLogo[nCntTitle]->SetScaling(SIZE_TITLE);

		// �`�������ݒ�ɂ���
		m_apLogo[nCntTitle]->SetEnableDraw(true);
	}

	// �J�����̍X�V���ĊJ
	CManager::GetCamera()->SetEnableUpdate(true);

	// ��Ԃ�ύX
	m_state = STATE_WAIT;	// �J�ڑҋ@���
}
