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
#include "texture.h"
#include "sceneGame.h"
#include "object2D.h"
#include "stage.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_EXP		(100)	// �ő勭��
#define EXP_FRAME	(10)	// �����ϓ��t���[��

#define GAUGE_POS		(D3DXVECTOR3(260.0f, 440.0f, 0.0f))	// �ʒu
#define GAUGE_GAUGESIZE	(D3DXVECTOR3(200.0f, 30.0f, 0.0f))	// �Q�[�W�傫��
#define GAUGE_FRONTCOL	(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))	// �\�Q�[�W�F
#define GAUGE_BACKCOL	(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	// ���Q�[�W�F

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CLevelupManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\option000.png",	// �����e�N�X�`��
	"data\\TEXTURE\\option001.png",	// �I���e�N�X�`��
};

//************************************************************
//	�e�N���X [CLevelupManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CLevelupManager::CLevelupManager()
{
	// �����o�ϐ����N���A
	m_pOption	= NULL;		// ������
	m_pBg		= NULL;		// �w�i���
	m_state = STATE_NORMAL;	// ���
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
	// �����o�ϐ���������
	m_pOption	= NULL;		// ������
	m_pBg		= NULL;		// �w�i���
	m_state = STATE_NORMAL;	// ���

	// ������̐���
	m_pOption = CObject2D::Create	// TODO�F�萔
	( // ����
		D3DXVECTOR3(1160.5f, 680.0f, 0.0f),
		D3DXVECTOR3(220.0f, 65.0f, 0.0f)
	);
	if (UNUSED(m_pOption))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pOption->SetPriority(6);

	// �`������Ȃ���Ԃɂ���
	m_pOption->SetEnableDraw(false);

	// �w�i���̐���
	m_pBg = CObject2D::Create	// TODO�F�萔
	( // ����
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.85f, SCREEN_HEIGHT * 0.78f, 0.0f),
		VEC3_ZERO,
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.55f)
	);
	if (UNUSED(m_pBg))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pBg->SetPriority(6);

	// �`������Ȃ���Ԃɂ���
	m_pBg->SetEnableDraw(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CLevelupManager::Uninit(void)
{
	// �������j��
	m_pOption->Uninit();

	// �w�i����j��
	m_pBg->Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CLevelupManager::Update(void)
{
	// �|�C���^��錾
	CTexture		*pTexture	= CManager::GetTexture();	// �e�N�X�`��
	CInputKeyboard	*pKeyboard	= CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetPad();		// �p�b�h

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NORMAL:		// �ʏ���

		if (CSceneGame::GetStage()->GetAreaPlayer() == CStage::AREA_SAFE)
		{ // �Z�[�t�G���A���̏ꍇ

			// �������\��
			m_pOption->SetEnableDraw(true);

			// ��Ԃ�ݒ�
			m_state = STATE_INIT_INSAFE;	// �Z�[�t�G���A�����(������)
		}

		break;

	case STATE_INIT_INSAFE:	// �Z�[�t�G���A�����(������)

		// �����\���̃e�N�X�`����ݒ�
		m_pOption->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_STRENGTHEN]));

		// ��Ԃ�ݒ�
		m_state = STATE_INSAFE;	// �Z�[�t�G���A�����

		// �����𔲂����ɉ��ɑ���

	case STATE_INSAFE:		// �Z�[�t�G���A�����

		if (pKeyboard->GetTrigger(DIK_1))
		{
			// �w�i����\��
			m_pBg->SetEnableDraw(true);

			// ��Ԃ�ݒ�
			m_state = STATE_INIT_SELECT;	// �����I�����(������)
		}

		break;

	case STATE_INIT_SELECT:	// �����I�����(������)

		// �I���\���̃e�N�X�`����ݒ�
		m_pOption->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_END]));

		// ��Ԃ�ݒ�
		m_state = STATE_SELECT;	// �����I�����(������)

		// �����𔲂����ɉ��ɑ���

	case STATE_SELECT:		// �����I�����

		if (pKeyboard->GetTrigger(DIK_1))
		{
			// �w�i����\��
			m_pBg->SetEnableDraw(false);

			// ��Ԃ�ݒ�
			m_state = STATE_INIT_INSAFE;	// �Z�[�t�G���A�����(������)
		}

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	if (m_state != STATE_NORMAL)
	{ // ��Ԃ��ʏ��ԈȊO�̏ꍇ

		if (CSceneGame::GetStage()->GetAreaPlayer() != CStage::AREA_SAFE)
		{ // �Z�[�t�G���A�O�̏ꍇ

			// �`������Ȃ���Ԃɂ���
			m_pBg->SetEnableDraw(false);
			m_pOption->SetEnableDraw(false);

			// ��Ԃ�ݒ�
			m_state = STATE_NORMAL;	// �ʏ���
		}
	}

	// ������̍X�V
	m_pOption->Update();

	// �w�i���̍X�V
	m_pBg->Update();
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
