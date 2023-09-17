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

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************


//************************************************************
//	�}�N����`
//************************************************************


//************************************************************
//	�e�N���X [CTutorialManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTutorialManager::CTutorialManager()
{
	// �����o�ϐ����N���A
	m_state = STATE_NONE;	// ���
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
	m_state = STATE_NONE;	// ���

#if 0
	//--------------------------------------------------------
	//	�I��w�i�̐����E�ݒ�
	//--------------------------------------------------------
	// �I��w�i�̐���
	m_pSelectBG = CObject2D::Create
	( // ����
		POS_SELECTBG,	// �ʒu
		SIZE_SELECTBG,	// �傫��
		VEC3_ZERO,		// ����
		COL_SELECTBG	// �F
	);
	if (UNUSED(m_pSelectBG))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pSelectBG->SetPriority(TUTORIAL_PRIO);

	// �`������Ȃ��ݒ�ɂ���
	m_pSelectBG->SetEnableDraw(false);
#endif

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTutorialManager::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CTutorialManager::Update(void)
{
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:	// �������Ȃ����

		// ����

		break;

	default:	// ��O����
		assert(false);
		break;
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
