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
#include "stage.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CShopManager::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\option000.png",	// �ʏ�e�N�X�`��
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
	m_pBg = NULL;	// �w�i���
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
	// �����o�ϐ���������
	m_pBg = NULL;	// �w�i���

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
void CShopManager::Uninit(void)
{
	// �w�i����j��
	m_pBg->Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CShopManager::Update(void)
{
	// �|�C���^��錾
	CTexture		*pTexture	= CManager::GetTexture();	// �e�N�X�`��
	CInputKeyboard	*pKeyboard	= CManager::GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetPad();		// �p�b�h

	// �w�i���̍X�V
	m_pBg->Update();
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CShopManager::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	m_pBg->SetEnableDraw(bDraw);	// �w�i���
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
