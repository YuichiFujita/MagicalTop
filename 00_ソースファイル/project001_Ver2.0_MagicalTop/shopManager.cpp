//============================================================
//
//	�V���b�v�}�l�[�W���[���� [shopManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "shopManager.h"
#include "manager.h"
#include "input.h"
#include "texture.h"
#include "sceneGame.h"
#include "object2D.h"
#include "valueUI.h"
#include "multiValue.h"
#include "target.h"
#include "player.h"
#include "stage.h"

//************************************************************
//	�}�N����`
//************************************************************
#define SHOP_PRIO	(6)	// �V���b�v�\���̗D�揇��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CShopManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\icon000.png",	// �^�[�Q�b�g�A�C�R���e�N�X�`��
	"data\\TEXTURE\\icon001.png",	// �o���l�A�C�R���e�N�X�`��
};

//************************************************************
//	�e�N���X [CShopManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CShopManager::CShopManager()
{
	// �����o�ϐ����N���A
	m_pBg = NULL;			// �w�i���
	m_pIconTarget = NULL;	// �^�[�Q�b�g�A�C�R�����
	m_pIconExp = NULL;		// �o���l�A�C�R�����
	m_pLife = NULL;			// �^�[�Q�b�g�̗͏��
	m_pLv = NULL;			// �v���C���[���x�����
}

//============================================================
//	�f�X�g���N�^
//============================================================
CShopManager::~CShopManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CShopManager::Init(void)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`��

	// �����o�ϐ���������
	m_pBg = NULL;			// �w�i���
	m_pIconTarget = NULL;	// �^�[�Q�b�g�A�C�R�����
	m_pIconExp = NULL;		// �o���l�A�C�R�����
	m_pLife = NULL;			// �^�[�Q�b�g�̗͏��
	m_pLv = NULL;			// �v���C���[���x�����

	//--------------------------------------------------------
	//	�w�i�̐����E�ݒ�
	//--------------------------------------------------------
	// �w�i���̐���
	m_pBg = CObject2D::Create	// TODO�F�萔
	( // ����
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.85f, SCREEN_HEIGHT * 0.78f, 0.0f),
		VEC3_ZERO,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.55f)
	);
	if (UNUSED(m_pBg))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pBg->SetPriority(SHOP_PRIO);

	// �`������Ȃ���Ԃɂ���
	m_pBg->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�^�[�Q�b�g�A�C�R���̐����E�ݒ�
	//--------------------------------------------------------
	// �^�[�Q�b�g�A�C�R�����̐���
	m_pIconTarget = CObject2D::Create	// TODO�F�萔
	( // ����
		D3DXVECTOR3(180.0f, 190.0f, 0.0f),
		D3DXVECTOR3(120.0f, 120.0f, 0.0f)
	);
	if (UNUSED(m_pIconTarget))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`���������E�ݒ�
	m_pIconTarget->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_TARGET]));

	// �D�揇�ʂ�ݒ�
	m_pIconTarget->SetPriority(SHOP_PRIO);

	// �`������Ȃ���Ԃɂ���
	m_pIconTarget->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�o���l�A�C�R���̐����E�ݒ�
	//--------------------------------------------------------
	// �o���l�A�C�R�����̐���
	m_pIconExp = CObject2D::Create	// TODO�F�萔
	( // ����
		D3DXVECTOR3(800.0f, 190.0f, 0.0f),
		D3DXVECTOR3(120.0f, 120.0f, 0.0f)
	);
	if (UNUSED(m_pIconExp))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`���������E�ݒ�
	m_pIconExp->BindTexture(pTexture->Regist(mc_apTextureFile[TEXTURE_EXP]));

	// �D�揇�ʂ�ݒ�
	m_pIconExp->SetPriority(SHOP_PRIO);

	// �`������Ȃ���Ԃɂ���
	m_pIconExp->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�^�[�Q�b�g�̗͂̐����E�ݒ�
	//--------------------------------------------------------
	// �^�[�Q�b�g�̗̑͏��
	m_pLife = CValueUI::Create
	( // ����
		D3DXVECTOR3(380.0f, 140.0f, 0.0f),	// 
		D3DXVECTOR3(0.0f, 100.0f, 0.0f),	// 
		D3DXVECTOR3(280.0f, 90.0f, 0.0f),	// 
		D3DXVECTOR3(80.0f, 90.0f, 0.0f),	// 
		pTexture->Regist("data\\TEXTURE\\area000.png")
	);

	// �����̍s�Ԃ�ݒ�
	m_pLife->GetMultiValue()->SetSpace(D3DXVECTOR3(80.0f, 0.0f, 0.0f));

	// �����̌�����ݒ�
	m_pLife->GetMultiValue()->SetDigit(4);

	// �����̐��l��ݒ�
	//m_pLife->GetMultiValue()->SetNum(CSceneGame::GetTarget()->GetLife());

	// �D�揇�ʂ�ݒ�
	m_pLife->SetPriority(SHOP_PRIO);

	// �`������Ȃ���Ԃɂ���
	m_pLife->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�v���C���[���x���̐����E�ݒ�
	//--------------------------------------------------------
	// �v���C���[�̃��x�����
	m_pLv = CValueUI::Create
	( // ����
		D3DXVECTOR3(980.0f, 140.0f, 0.0f),	// 
		D3DXVECTOR3(0.0f, 100.0f, 0.0f),	// 
		D3DXVECTOR3(280.0f, 90.0f, 0.0f),	// 
		D3DXVECTOR3(80.0f, 90.0f, 0.0f),	// 
		pTexture->Regist("data\\TEXTURE\\area000.png")
	);

	// �����̍s�Ԃ�ݒ�
	m_pLv->GetMultiValue()->SetSpace(D3DXVECTOR3(80.0f, 0.0f, 0.0f));

	// �����̌�����ݒ�
	m_pLv->GetMultiValue()->SetDigit(2);

	// �����̐��l��ݒ�
	//m_pLv->GetMultiValue()->SetNum(CSceneGame::GetPlayer()->GetLevel());

	// �D�揇�ʂ�ݒ�
	m_pLv->SetPriority(SHOP_PRIO);

	// �`������Ȃ���Ԃɂ���
	m_pLv->SetEnableDraw(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CShopManager::Uninit(void)
{
	// �I�u�W�F�N�g2D����j��
	m_pBg->Uninit();
	m_pIconTarget->Uninit();
	m_pIconExp->Uninit();

	// ����UI����j��
	m_pLife->Uninit();
	m_pLv->Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CShopManager::Update(void)
{
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetPad();		// �p�b�h

	// ���l�ɑ̗͂�ݒ�
	m_pLife->GetMultiValue()->SetNum(CSceneGame::GetTarget()->GetLife());

	// ���l�Ƀ��x����ݒ�
	m_pLv->GetMultiValue()->SetNum(CSceneGame::GetPlayer()->GetLevel());

	// �I�u�W�F�N�g2D���̍X�V
	m_pBg->Update();
	m_pIconTarget->Update();
	m_pIconExp->Update();

	// ����UI���̍X�V
	m_pLife->Update();
	m_pLv->Update();
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CShopManager::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	m_pBg->SetEnableDraw(bDraw);			// �w�i���
	m_pIconTarget->SetEnableDraw(bDraw);	// �^�[�Q�b�g�A�C�R�����
	m_pIconExp->SetEnableDraw(bDraw);		// �o���l�A�C�R�����
	m_pLife->SetEnableDraw(bDraw);			// �^�[�Q�b�g�̗͏��
	m_pLv->SetEnableDraw(bDraw);			// �v���C���[���x�����
}

//============================================================
//	��������
//============================================================
CShopManager *CShopManager::Create(void)
{
	// �|�C���^��錾
	CShopManager *pShopManager = NULL;	// �V���b�v�}�l�[�W���[�����p

	if (UNUSED(pShopManager))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pShopManager = new CShopManager;	// �V���b�v�}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pShopManager))
	{ // �g�p����Ă���ꍇ
		
		// �V���b�v�}�l�[�W���[�̏�����
		if (FAILED(pShopManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pShopManager;
			pShopManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pShopManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CShopManager::Release(CShopManager *&prShopManager)
{
	if (USED(prShopManager))
	{ // �g�p���̏ꍇ

		// �V���b�v�}�l�[�W���[�̏I��
		prShopManager->Uninit();

		// �������J��
		delete prShopManager;
		prShopManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}
