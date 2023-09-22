//============================================================
//
//	�莆�}�l�[�W���[���� [letterManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "letterManager.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "object2D.h"

//************************************************************
//	�}�N����`
//************************************************************
#define LETTER_PRIO	(6)	// �莆�̗D�揇��

#define SIZE_LETTER		(D3DXVECTOR3(1150.0f, 687.0f, 0.0f))		// �莆�̑傫��
#define COL_LETTER		(XCOL_AWHITE)								// �莆�̐F
#define COL_FADE		(XCOL_ABLACK)								// �t�F�[�h�̐F
#define POS_PAPER		(D3DXVECTOR3(SCREEN_CENT.x, 1020.0f, 0.0f))	// ��Ⳃ̏������W
#define SIZE_PAPER		(D3DXVECTOR3(1000.0f, 597.5f, 0.0f))		// ��Ⳃ̑傫��
#define POS_CONTROL		(D3DXVECTOR3(SCREEN_CENT.x, 665.0f, 0.0f))	// ����̏������W
#define SIZE_CONTROL	(D3DXVECTOR3(SCREEN_WIDTH, 100.0f, 0.0f))	// ����̑傫��
#define COL_CONTROL		(XCOL_AWHITE)								// ����̐F

#define LETTER_CHAN	(0.04f)		// �莆�̃��l�ύX��
#define LETTER_STOP	(1.0f)		// �莆�̍ő僿�l
#define FADE_CHAN	(0.02f)		// �t�F�[�h�̃��l�ύX��
#define FADE_STOP	(0.6f)		// �t�F�[�h�̍ő僿�l
#define PAPER_MOVE	(0.5f)		// ��Ⳃ̈ʒu�̍X�V��
#define PAPER_STOP	(315.0f)	// ��Ⳃ̒�~Y�ʒu

#define CONTROL_ADD_ROT		(0.02f)	// ��������̓_�Ō����̉��Z��
#define CONTROL_SUB_ALPHA	(0.06f)	// ��������̂��܂����̃��l�̕ύX��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CLetterManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\letter000.png",	// �莆�̃e�N�X�`�����΃p�X
	"data\\TEXTURE\\letter001.png",	// ����\���̃e�N�X�`�����΃p�X
};

//************************************************************
//	�e�N���X [CLetterManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CLetterManager::CLetterManager()
{
	// �����o�ϐ����N���A
	m_pLetter	= NULL;			// �莆�̏��
	m_pFade		= NULL;			// �t�F�[�h�̏��
	m_pPaper	= NULL;			// ��Ⳃ̏��
	m_pControl	= NULL;			// ������@�̏��
	m_state		= STATE_NONE;	// ���
	m_fMove		= 0.0f;			// ��Ⳃ̈ړ���
	m_fSinRot	= 0.0f;			// ������@�̓_�Ō���
	m_nCounterState	= 0;		// ��ԊǗ��J�E���^�[
}

//============================================================
//	�f�X�g���N�^
//============================================================
CLetterManager::~CLetterManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CLetterManager::Init(void)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����o�ϐ���������
	m_pLetter	= NULL;			// �莆�̏��
	m_pFade		= NULL;			// �t�F�[�h�̏��
	m_pPaper	= NULL;			// ��Ⳃ̏��
	m_pControl	= NULL;			// ������@�̏��
	m_state		= STATE_NONE;	// ���
	m_fMove		= 0.0f;			// ��Ⳃ̈ړ���
	m_fSinRot	= -HALF_PI;		// ������@�̓_�Ō���
	m_nCounterState	= 0;		// ��ԊǗ��J�E���^�[

	//--------------------------------------------------------
	//	�莆�̐����E�ݒ�
	//--------------------------------------------------------
	// �莆�̐���
	m_pLetter = CObject2D::Create
	( // ����
		SCREEN_CENT,	// �ʒu
		SIZE_LETTER,	// �傫��
		VEC3_ZERO,		// ����
		COL_LETTER		// �F
	);
	if (UNUSED(m_pLetter))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pLetter->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_LETTER]));

	// �D�揇�ʂ�ݒ�
	m_pLetter->SetPriority(LETTER_PRIO);

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
	m_pFade->SetPriority(LETTER_PRIO);

	//--------------------------------------------------------
	//	��Ⳃ̐����E�ݒ�
	//--------------------------------------------------------
	// ��Ⳃ̐���
	m_pPaper = CObject2D::Create
	( // ����
		POS_PAPER,	// �ʒu
		SIZE_PAPER	// �傫��
	);
	if (UNUSED(m_pPaper))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pPaper->SetPriority(LETTER_PRIO);

	//--------------------------------------------------------
	//	������@�̐����E�ݒ�
	//--------------------------------------------------------
	// ������@�̐���
	m_pControl = CObject2D::Create
	( // ����
		POS_CONTROL,	// �ʒu
		SIZE_CONTROL,	// �傫��
		VEC3_ZERO,		// ����
		COL_CONTROL		// �F
	);
	if (UNUSED(m_pControl))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pControl->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_CONTROL]));

	// �D�揇�ʂ�ݒ�
	m_pControl->SetPriority(LETTER_PRIO);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CLetterManager::Uninit(void)
{
	// �莆�̏I��
	m_pLetter->Uninit();

	// �t�F�[�h�̏I��
	m_pFade->Uninit();

	// ��Ⳃ̏I��
	m_pPaper->Uninit();

	// ������@�̏I��
	m_pControl->Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CLetterManager::Update(void)
{
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:			// �������Ȃ����

		// ����

		// �����𔲂���
		break;

	case STATE_LETTER:			// �莆�̕\�����

		// �莆�\���̍X�V
		UpdateLetter();

		// �����𔲂���
		break;

	case STATE_FADEIN:			// �t�F�[�h�C�����

		// �t�F�[�h�C���̍X�V
		UpdateFade();

		// �����𔲂���
		break;

	case STATE_PAPER_TAKE:		// ��Ⳃ̎��o�����

		// ��Ⳏ�o�̍X�V
		UpdatePaperTake();

		// �����𔲂���
		break;

	case STATE_WAIT:			// �ҋ@���

		// �ҋ@�̍X�V
		UpdateWait();

		// �����𔲂���
		break;

	case STATE_PAPER_RETURN:	// ��Ⳃ̂��܂����

		// ��Ⳏ��[�̍X�V
		UpdatePaperReturn();

		// �����𔲂���
		break;

	case STATE_END:				// �I�����

		// ����

		// �����𔲂���
		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �莆�̍X�V
	m_pLetter->Update();

	// �t�F�[�h�̍X�V
	m_pFade->Update();

	// ��Ⳃ̍X�V
	m_pPaper->Update();

	// ������@�̍X�V
	m_pControl->Update();
}

//============================================================
//	�莆�\���̊J�n����
//============================================================
void CLetterManager::SetLook(const char *pPassTex)
{
	// ��Ⳃ̐ݒ�
	SetPaper(pPassTex);

	// ��Ⳃ̈ʒu��������
	m_pPaper->SetPosition(POS_PAPER);

	// �|���S���̐F����������
	m_pLetter->SetColor(COL_LETTER);	// �莆
	m_pFade->SetColor(COL_FADE);		// �t�F�[�h
	m_pControl->SetColor(COL_CONTROL);	// ������@

	// ����������
	m_fMove = 0.0f;			// ��Ⳃ̈ړ���
	m_fSinRot = -HALF_PI;	// ������@�̓_�Ō���
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

	// ��Ԃ�ݒ�
	m_state = STATE_LETTER;	// �莆�̕\�����
}

//============================================================
//	��Ⳃ̐ݒ菈��
//============================================================
void CLetterManager::SetPaper(const char *pPassTex)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// ��Ⳃ̃e�N�X�`����o�^�E����
	m_pPaper->BindTexture(pTexture->Regist(pPassTex));
}

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
void CLetterManager::SetState(const STATE state)
{
	// �����̏�Ԃ�ݒ�
	m_state = state;
}

//============================================================
//	��Ԏ擾����
//============================================================
CLetterManager::STATE CLetterManager::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	��������
//============================================================
CLetterManager *CLetterManager::Create(void)
{
	// �|�C���^��錾
	CLetterManager *pLetterManager = NULL;	// �莆�}�l�[�W���[�����p

	if (UNUSED(pLetterManager))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pLetterManager = new CLetterManager;	// �莆�}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pLetterManager))
	{ // �g�p����Ă���ꍇ
		
		// �莆�}�l�[�W���[�̏�����
		if (FAILED(pLetterManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pLetterManager;
			pLetterManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pLetterManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CLetterManager::Release(CLetterManager *&prLetterManager)
{
	if (USED(prLetterManager))
	{ // �g�p���̏ꍇ

		// �莆�}�l�[�W���[�̏I��
		prLetterManager->Uninit();

		// �������J��
		delete prLetterManager;
		prLetterManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�莆�\���̍X�V����
//============================================================
void CLetterManager::UpdateLetter(void)
{
	// �ϐ���錾
	D3DXCOLOR colLetter = m_pLetter->GetColor();	// �莆�̐F

	// �莆�̃��l�����Z
	colLetter.a += LETTER_CHAN;

	if (colLetter.a >= LETTER_STOP)
	{ // �莆�̃��l�����l�ȏ�̏ꍇ

		// �莆�̃��l��␳
		colLetter.a = LETTER_STOP;

		// �t�F�[�h�C����Ԃɂ���
		m_state = STATE_FADEIN;
	}

	// �莆�̐F�𔽉f
	m_pLetter->SetColor(colLetter);
}

//============================================================
//	�t�F�[�h�C���̍X�V����
//============================================================
void CLetterManager::UpdateFade(void)
{
	// �ϐ���錾
	D3DXCOLOR colFade = m_pFade->GetColor();	// �t�F�[�h�̐F

	// �t�F�[�h�̃��l�����Z
	colFade.a += FADE_CHAN;
	
	if (colFade.a >= FADE_STOP)
	{ // �t�F�[�h�̃��l�����l�ȏ�̏ꍇ

		// �t�F�[�h�̃��l��␳
		colFade.a = FADE_STOP;

		// ��Ⳃ̎��o����Ԃɂ���
		m_state = STATE_PAPER_TAKE;

		// �T�E���h�̍Đ�
		CManager::GetSound()->Play(CSound::LABEL_SE_PAPER);	// ��Ⳃ߂��艹
	}

	// �t�F�[�h�̐F�𔽉f
	m_pFade->SetColor(colFade);
}

//============================================================
//	��Ⳏ�o�̍X�V����
//============================================================
void CLetterManager::UpdatePaperTake(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posPaper = m_pPaper->GetPosition();	// ��Ⳃ̈ʒu

	// ��Ⳃ̈ʒu���Z�ʂ�ݒ�
	m_fMove += PAPER_MOVE;

	// ��Ⳃ̈ʒu�����Z
	posPaper.y -= m_fMove;

	if (posPaper.y <= PAPER_STOP)
	{ // ��Ⳃ̈ʒu�����l�ȉ��̏ꍇ

		// ��Ⳃ̈ʒu��␳
		posPaper.y = PAPER_STOP;

		// ��Ⳃ̈ړ��ʂ�������
		m_fMove = 0.0f;

		// �ҋ@��Ԃɂ���
		m_state = STATE_WAIT;
	}

	// ��Ⳃ̈ʒu�𔽉f
	m_pPaper->SetPosition(posPaper);
}

//============================================================
//	�ҋ@�̍X�V����
//============================================================
void CLetterManager::UpdateWait(void)
{
	// �ϐ���錾
	D3DXCOLOR colControl = m_pControl->GetColor();	// ������@�̐F

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
		// ������@�̓_�Ō�����������
		m_fSinRot = -HALF_PI;

		// ��Ⳃ̂��܂���Ԃɂ���
		m_state = STATE_PAPER_RETURN;

		// �T�E���h�̍Đ�
		CManager::GetSound()->Play(CSound::LABEL_SE_PAPER);	// ��Ⳃ߂��艹
	}

	// ������@�̓_�Ō��������Z
	m_fSinRot += CONTROL_ADD_ROT;
	useful::NormalizeRot(m_fSinRot);	// ������␳

	// ��������̃��l��ύX
	colControl.a = (1.0f / 2.0f) * (sinf(m_fSinRot) + 1.0f);

	// ������@�̐F�𔽉f
	m_pControl->SetColor(colControl);
}

//============================================================
//	��Ⳏ��[�̍X�V����
//============================================================
void CLetterManager::UpdatePaperReturn(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posPaper = m_pPaper->GetPosition();	// ��Ⳃ̈ʒu
	D3DXCOLOR colLetter = m_pLetter->GetColor();	// �莆�̐F
	D3DXCOLOR colFade = m_pFade->GetColor();		// �t�F�[�h�̐F
	D3DXCOLOR colControl = m_pControl->GetColor();	// ������@�̐F

	//--------------------------------------------------------
	//	�莆�̓�����
	//--------------------------------------------------------
	if (colLetter.a > 0.0f)
	{ // �莆�̃��l�����l���傫���ꍇ

		// �莆�̃��l�����Z
		colLetter.a -= LETTER_CHAN;

		if (colLetter.a <= 0.0f)
		{ // �莆�̃��l�����l�ȉ��̏ꍇ

			// �莆�̃��l��␳
			colLetter.a = 0.0f;
		}

		// �莆�̐F�𔽉f
		m_pLetter->SetColor(colLetter);
	}

	//--------------------------------------------------------
	//	�t�F�[�h�̓�����
	//--------------------------------------------------------
	if (colFade.a > 0.0f)
	{ // �t�F�[�h�̃��l�����l���傫���ꍇ

		// �t�F�[�h�̃��l�����Z
		colFade.a -= FADE_CHAN;

		if (colFade.a <= 0.0f)
		{ // �t�F�[�h�̃��l�����l�ȉ��̏ꍇ

			// �t�F�[�h�̃��l��␳
			colFade.a = 0.0f;
		}

		// �t�F�[�h�̐F�𔽉f
		m_pFade->SetColor(colFade);
	}

	//--------------------------------------------------------
	//	��������̓�����
	//--------------------------------------------------------
	if (colControl.a > 0.0f)
	{ // ��������̃��l�����l���傫���ꍇ

		// ��������̃��l�����Z
		colControl.a -= CONTROL_SUB_ALPHA;

		if (colControl.a <= 0.0f)
		{ // ��������̃��l�����l�ȉ��̏ꍇ

			// ��������̃��l��␳
			colControl.a = 0.0f;
		}

		// ������@�̐F�𔽉f
		m_pControl->SetColor(colControl);
	}

	//--------------------------------------------------------
	//	��Ⳃ̈ړ�
	//--------------------------------------------------------
	if (posPaper.y < POS_PAPER.y)
	{ // ��Ⳃ̈ʒu�����l��菬�����ꍇ

		// ��Ⳃ̈ړ��ʂ����Z
		m_fMove += PAPER_MOVE;

		// ��Ⳃ̈ʒu�����Z
		posPaper.y += m_fMove;
	}
	else if (posPaper.y >= POS_PAPER.y)
	{ // ��Ⳃ̈ʒu�����l�ȏ�̏ꍇ

		// ��Ⳃ̈ʒu��␳
		posPaper.y = POS_PAPER.y;

		// ��Ⳃ̈ړ��ʂ�������
		m_fMove = 0.0f;

		if (colLetter.a	 <= 0.0f
		&&  colFade.a	 <= 0.0f
		&&  colControl.a <= 0.0f)
		{ // �S�Ẵ��l��������؂��Ă���ꍇ

			// �I����Ԃɂ���
			m_state = STATE_END;
		}
	}

	// ��Ⳃ̈ʒu�𔽉f
	m_pPaper->SetPosition(posPaper);
}
