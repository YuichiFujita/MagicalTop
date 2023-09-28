//============================================================
//
//	�����}�l�[�W���[���� [levelupManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "levelupManager.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "texture.h"
#include "sceneGame.h"
#include "waveManager.h"
#include "shopManager.h"
#include "object2D.h"
#include "stage.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CLevelupManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\shop008.png",	// ����e�N�X�`��
};

//************************************************************
//	�}�N����`
//************************************************************
#define CONTROL_PRIO	(6)		// ����\���̗D�揇��
#define ADD_ROT			(0.05f)	// �_�Ō����̉��Z��
#define POS_CONTROL		(D3DXVECTOR3(SCREEN_CENT.x, 665.0f, 0.0f))	// ������@�\���̈ʒu
#define SIZE_CONTROL	(D3DXVECTOR3(SCREEN_WIDTH, 100.0f, 0.0f))	// ������@�\���̑傫��
#define COL_CONTROL		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f))			// ������@�\���̐F

//************************************************************
//	�e�N���X [CLevelupManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CLevelupManager::CLevelupManager()
{
	// �����o�ϐ����N���A
	m_pShopManager	= NULL;			// �V���b�v�}�l�[�W���[�̏��
	m_pControl		= NULL;			// ������
	m_state			= STATE_NORMAL;	// ���
	m_fSinRot		= 0.0f;			// �|���S���_�Ō���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CLevelupManager::~CLevelupManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CLevelupManager::Init(void)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`��

	// �����o�ϐ���������
	m_pShopManager	= NULL;			// �V���b�v�}�l�[�W���[�̏��
	m_pControl		= NULL;			// ������
	m_state			= STATE_NORMAL;	// ���
	m_fSinRot		= -HALF_PI;		// �|���S���_�Ō���

	// �V���b�v�}�l�[�W���[�̐���
	m_pShopManager = CShopManager::Create();
	if (UNUSED(m_pShopManager))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �V���b�v��X����
	m_pShopManager->SetEnableOpen(false);

	// ������̐���
	m_pControl = CObject2D::Create
	( // ����
		POS_CONTROL,	// �ʒu
		SIZE_CONTROL,	// �傫��
		VEC3_ZERO,		// ����
		COL_CONTROL		// �F
	);
	if (UNUSED(m_pControl))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����Ǎ��E����
	m_pControl->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_CONTROL]));

	// �D�揇�ʂ�ݒ�
	m_pControl->SetPriority(CONTROL_PRIO);

	// �`������Ȃ���Ԃɂ���
	m_pControl->SetEnableDraw(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CLevelupManager::Uninit(void)
{
	// �V���b�v�}�l�[�W���[��j��
	if (FAILED(m_pShopManager->Release(m_pShopManager)))
	{ // �j���Ɏ��s�����ꍇ

		// ��O����
		assert(false);
	}

	// �������j��
	m_pControl->Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CLevelupManager::Update(void)
{
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetPad();		// �p�b�h

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NORMAL:	// �ʏ���

		if (CSceneGame::GetWaveManager()->GetState() == CWaveManager::STATE_WAIT)
		{ // ���̋G�߂ֈڍs�҂��̏ꍇ

			// �V���b�v���J�X����
			m_pShopManager->SetEnableOpen(true);

			// �������\��
			m_pControl->SetEnableDraw(true);

			// ��Ԃ�ݒ�
			m_state = STATE_SELECT;	// �����I�����

			// �T�E���h�̍Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// ���艹01
		}

		break;

	case STATE_SELECT:	// �����I�����

		if (pKeyboard->GetTrigger(DIK_SPACE) || pPad->GetTrigger(CInputPad::KEY_B))
		{ // �����I���̑��삪�s��ꂽ�ꍇ

			// �V���b�v��X����
			m_pShopManager->SetEnableOpen(false);

			// ��������\��
			m_pControl->SetEnableDraw(false);

			// �|���S���_�Ō�����������
			m_fSinRot = -HALF_PI;

			// ������̐F��������
			m_pControl->SetColor(COL_CONTROL);

			// ��Ԃ�ݒ�
			m_state = STATE_NORMAL;	// �ʏ���

			// ���G�߂ֈڍs
			CSceneGame::GetWaveManager()->NextSeason();
		}

		// �_�ł̍X�V
		UpdateBlink();

		// �V���b�v�}�l�[�W���[�̍X�V
		m_pShopManager->Update();

		// ������̍X�V
		m_pControl->Update();

		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	��������
//============================================================
CLevelupManager *CLevelupManager::Create(void)
{
	// �|�C���^��錾
	CLevelupManager *pLevelupManager = NULL;	// �����}�l�[�W���[�����p

	if (UNUSED(pLevelupManager))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pLevelupManager = new CLevelupManager;	// �����}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pLevelupManager))
	{ // �g�p����Ă���ꍇ
		
		// �����}�l�[�W���[�̏�����
		if (FAILED(pLevelupManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pLevelupManager;
			pLevelupManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pLevelupManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CLevelupManager::Release(CLevelupManager *&prLevelupManager)
{
	if (USED(prLevelupManager))
	{ // �g�p���̏ꍇ

		// �����}�l�[�W���[�̏I��
		prLevelupManager->Uninit();

		// �������J��
		delete prLevelupManager;
		prLevelupManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�_�ł̍X�V����
//============================================================
void CLevelupManager::UpdateBlink(void)
{
	// �ϐ���錾
	D3DXCOLOR colControl = m_pControl->GetColor();	// ����\���̐F

	// ����\���̓_�Ō��������Z
	m_fSinRot += ADD_ROT;
	useful::NormalizeRot(m_fSinRot);	// ������␳

	// ����\���̃��l��ύX
	colControl.a = (1.0f / 2.0f) * (sinf(m_fSinRot) + 1.0f);

	// ����\���̐F�𔽉f
	m_pControl->SetColor(colControl);
}
