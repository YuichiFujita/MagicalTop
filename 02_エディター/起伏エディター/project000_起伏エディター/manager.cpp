//============================================================
//
//	�}�l�[�W���[���� [manager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "model.h"
#include "debugproc.h"
#include "edit.h"

#include "player.h"
#include "field.h"

#ifdef _DEBUG	// �f�o�b�O����

#include "debug.h"

#endif	// _DEBUG

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CRenderer		*CManager::m_pRenderer	= NULL;		// �����_���[�I�u�W�F�N�g
CInputKeyboard	*CManager::m_pKeyboard	= NULL;		// �L�[�{�[�h�I�u�W�F�N�g
CInputMouse		*CManager::m_pMouse		= NULL;		// �}�E�X�I�u�W�F�N�g
CInputPad		*CManager::m_pPad		= NULL;		// �p�b�h�I�u�W�F�N�g
CSound			*CManager::m_pSound		= NULL;		// �T�E���h�I�u�W�F�N�g
CCamera			*CManager::m_pCamera	= NULL;		// �J�����I�u�W�F�N�g
CLight			*CManager::m_pLight		= NULL;		// ���C�g�I�u�W�F�N�g
CTexture		*CManager::m_pTexture	= NULL;		// �e�N�X�`���I�u�W�F�N�g
CModel			*CManager::m_pModel		= NULL;		// ���f���I�u�W�F�N�g
CPlayer			*CManager::m_pPlayer	= NULL;		// �v���C���[�I�u�W�F�N�g
CField			*CManager::m_pField		= NULL;		// �n�ʃI�u�W�F�N�g
CEdit			*CManager::m_pEdit		= NULL;		// �G�f�B�b�g�I�u�W�F�N�g
CDebugProc		*CManager::m_pDebugProc = NULL;		// �f�o�b�O�\���I�u�W�F�N�g

#ifdef _DEBUG	// �f�o�b�O����

CDebug *CManager::m_pDebug = NULL;	// �f�o�b�O�I�u�W�F�N�g

#endif	// _DEBUG

//************************************************************
//	�e�N���X [CManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CManager::CManager()
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CManager::~CManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//--------------------------------------------------------
	//	�V�X�e���̐���
	//--------------------------------------------------------
	// �����_���[�̐���
	m_pRenderer = CRenderer::Create(hWnd);
	if (UNUSED(m_pRenderer))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �L�[�{�[�h�̐���
	m_pKeyboard = CInputKeyboard::Create(hInstance, hWnd);
	if (UNUSED(m_pKeyboard))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �}�E�X�̐���
	m_pMouse = CInputMouse::Create(hInstance, hWnd);
	if (UNUSED(m_pMouse))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �p�b�h�̐���
	m_pPad = CInputPad::Create();
	if (UNUSED(m_pPad))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �T�E���h�̐���
	m_pSound = CSound::Create(hWnd);
	if (UNUSED(m_pSound))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �J�����̐���
	m_pCamera = CCamera::Create();
	if (UNUSED(m_pCamera))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���C�g�̐���
	m_pLight = CLight::Create();
	if (UNUSED(m_pLight))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �G�f�B�b�g�̐���
	m_pEdit = CEdit::Create();
	if (UNUSED(m_pEdit))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	���̓Ǎ�
	//--------------------------------------------------------
	// �e�N�X�`���̐����E�Ǎ�
	m_pTexture = CTexture::Create();
	if (UNUSED(m_pTexture))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���f���̐����E�Ǎ�
	m_pModel = CModel::Create();
	if (UNUSED(m_pModel))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	�Q�[���I�u�W�F�N�g�̐���
	//--------------------------------------------------------
	// �n�ʃI�u�W�F�N�g�̐���
	m_pField = CField::Create(CField::TEXTURE_NORMAL, D3DXVECTOR3(0.0f, 400.0f, 0.0f), VEC3_ZERO, D3DXVECTOR2(4000.0f, 4000.0f), XCOL_WHITE, POSGRID2(4, 4));
	if (UNUSED(m_pField))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �v���C���[�I�u�W�F�N�g�̐���
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, -100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	if (UNUSED(m_pPlayer))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	�f�o�b�O�p
	//--------------------------------------------------------
	// �f�o�b�O�\���̐���
	m_pDebugProc = CDebugProc::Create(hWnd);
	if (UNUSED(m_pDebugProc))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

#ifdef _DEBUG	// �f�o�b�O����

	// �f�o�b�O�̐���
	m_pDebug = CDebug::Create();
	if (UNUSED(m_pDebug))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

#endif	// _DEBUG

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CManager::Uninit(void)
{
	//--------------------------------------------------------
	//	�f�o�b�O�p
	//--------------------------------------------------------
	// �f�o�b�O�\���̔j��
	if (FAILED(CDebugProc::Release(m_pDebugProc)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

#ifdef _DEBUG	// �f�o�b�O����

	// �f�o�b�O�̔j��
	if (FAILED(CDebug::Release(m_pDebug)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

#endif	// _DEBUG

	//--------------------------------------------------------
	//	�T�E���h�̒�~
	//--------------------------------------------------------
	// BGM�̒�~
	m_pSound->Stop();

	//--------------------------------------------------------
	//	�Q�[���I�u�W�F�N�g�̔j��
	//--------------------------------------------------------
	// �I�u�W�F�N�g�̑S�j��
	CObject::ReleaseAll();

	// �I���ς݂̃I�u�W�F�N�g�|�C���^��NULL�ɂ���
	m_pPlayer = NULL;	// �v���C���[�I�u�W�F�N�g
	m_pField = NULL;	// �n�ʃI�u�W�F�N�g

	//--------------------------------------------------------
	//	���̔j��
	//--------------------------------------------------------
	// �e�N�X�`���̔j��
	if (FAILED(CTexture::Release(m_pTexture)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���f���̔j��
	if (FAILED(CModel::Release(m_pModel)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	�V�X�e���̔j��
	//--------------------------------------------------------
	// �G�f�B�b�g�̔j��
	if (FAILED(CEdit::Release(m_pEdit)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���C�g�̔j��
	if (FAILED(CLight::Release(m_pLight)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �J�����̔j��
	if (FAILED(CCamera::Release(m_pCamera)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �T�E���h�̔j��
	if (FAILED(CSound::Release(m_pSound)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �p�b�h�̔j��
	if (FAILED(CInputPad::Release(m_pPad)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �}�E�X�̔j��
	if (FAILED(CInputMouse::Release(m_pMouse)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �L�[�{�[�h�̔j��
	if (FAILED(CInputKeyboard::Release(m_pKeyboard)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����_���[�̔j��
	if (FAILED(CRenderer::Release(m_pRenderer)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CManager::Update(void)
{
#ifdef _DEBUG	// �f�o�b�O����

	// �f�o�b�O�\��
	m_pDebugProc->Print("FPS�F%d\n", m_pDebug->GetFps());

#endif	// _DEBUG

	if (USED(m_pPad))
	{ // �g�p���̏ꍇ

		// �p�b�h�̍X�V
		m_pPad->Update();
	}
	else { assert(false); }	// ��g�p��

	if (USED(m_pMouse))
	{ // �g�p���̏ꍇ

		// �}�E�X�̍X�V
		m_pMouse->Update();
	}
	else { assert(false); }	// ��g�p��

	if (USED(m_pKeyboard))
	{ // �g�p���̏ꍇ

		// �L�[�{�[�h�̍X�V
		m_pKeyboard->Update();
	}
	else { assert(false); }	// ��g�p��

	if (USED(m_pRenderer))
	{ // �g�p���̏ꍇ

		// �����_���[�̍X�V
		m_pRenderer->Update();
	}
	else { assert(false); }	// ��g�p��

	if (USED(m_pLight))
	{ // �g�p���̏ꍇ

		// ���C�g�̍X�V
		m_pLight->Update();
	}
	else { assert(false); }	// ��g�p��

	if (USED(m_pCamera))
	{ // �g�p���̏ꍇ

		// �J�����̍X�V
		m_pCamera->Update();
	}
	else { assert(false); }	// ��g�p��

	if (USED(m_pEdit))
	{ // �g�p���̏ꍇ

		// �G�f�B�b�g�̍X�V
		m_pEdit->Update();
	}
	else { assert(false); }	// ��g�p��
}

//============================================================
//	�`�揈��
//============================================================
void CManager::Draw(void)
{
	if (USED(m_pRenderer))
	{ // �g�p���̏ꍇ

		// �����_���[�̕`��
		m_pRenderer->Draw();
	}
	else { assert(false); }	// ��g�p��
}

//============================================================
//	��������
//============================================================
CManager *CManager::Create(HINSTANCE hInstance, HWND hWnd)
{
	// �|�C���^��錾
	CManager *pManager = NULL;		// �}�l�[�W���[�����p

	if (UNUSED(pManager))
	{ // �g�p����Ă��Ȃ��ꍇ

		// ���������m��
		pManager = new CManager;	// �}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pManager))
	{ // �m�ۂɐ������Ă���ꍇ

		// �}�l�[�W���[�̏�����
		if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
		{ // �������Ɏ��s�����ꍇ

			// �}�l�[�W���[�̏I��
			if (FAILED(pManager->Uninit()))
			{ // �}�l�[�W���[�̏I���Ɏ��s�����ꍇ

				// �������J��
				delete pManager;
				pManager = NULL;

				// ���s��Ԃ�
				assert(false);
				return NULL;
			}

			// �������J��
			delete pManager;
			pManager = NULL;

			// ���s��Ԃ�
			assert(false);
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CManager::Release(CManager *&prManager)
{
	if (USED(prManager))
	{ // �g�p���̏ꍇ

		// �}�l�[�W���[�̏I��
		if (FAILED(prManager->Uninit()))
		{ // �}�l�[�W���[�̏I���Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �������J��
		delete prManager;
		prManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�����_���[�擾����
//============================================================
CRenderer *CManager::GetRenderer(void)
{
	// �����_���[�̃|�C���^��Ԃ�
	return m_pRenderer;
}

//============================================================
//	�L�[�{�[�h�擾����
//============================================================
CInputKeyboard *CManager::GetKeyboard(void)
{
	// �L�[�{�[�h�̃|�C���^��Ԃ�
	return m_pKeyboard;
}

//============================================================
//	�}�E�X�擾����
//============================================================
CInputMouse *CManager::GetMouse(void)
{
	// �}�E�X�̃|�C���^��Ԃ�
	return m_pMouse;
}

//============================================================
//	�p�b�h�擾����
//============================================================
CInputPad *CManager::GetPad(void)
{
	// �p�b�h�̃|�C���^��Ԃ�
	return m_pPad;
}

//============================================================
//	�T�E���h�擾����
//============================================================
CSound *CManager::GetSound(void)
{
	// �T�E���h�̃|�C���^��Ԃ�
	return m_pSound;
}

//============================================================
//	�J�����擾����
//============================================================
CCamera *CManager::GetCamera(void)
{
	// �J�����̃|�C���^��Ԃ�
	return m_pCamera;
}

//============================================================
//	���C�g�擾����
//============================================================
CLight *CManager::GetLight(void)
{
	// ���C�g�̃|�C���^��Ԃ�
	return m_pLight;
}

//============================================================
//	�e�N�X�`���擾����
//============================================================
CTexture *CManager::GetTexture(void)
{
	// �e�N�X�`���̃|�C���^��Ԃ�
	return m_pTexture;
}

//============================================================
//	���f���擾����
//============================================================
CModel *CManager::GetModel(void)
{
	// ���f���̃|�C���^��Ԃ�
	return m_pModel;
}

//============================================================
//	�v���C���[�擾����
//============================================================
CPlayer *CManager::GetPlayer(void)
{
	// �v���C���[�̃|�C���^��Ԃ�
	return m_pPlayer;
}

//============================================================
//	�n�ʎ擾����
//============================================================
CField *CManager::GetField(void)
{
	// �n�ʂ̃|�C���^��Ԃ�
	return m_pField;
}

//============================================================
//	�G�f�B�b�g�擾����
//============================================================
CEdit *CManager::GetEdit(void)
{
	// �G�f�B�b�g�̃|�C���^��Ԃ�
	return m_pEdit;
}

//============================================================
//	�f�o�b�O�\���擾����
//============================================================
CDebugProc *CManager::GetDebugProc(void)
{
	// �f�o�b�O�\���̃|�C���^��Ԃ�
	return m_pDebugProc;
}

//============================================================
//	�f�o�b�O�擾����
//============================================================
#ifdef _DEBUG	// �f�o�b�O����

CDebug *CManager::GetDebug(void)
{
	// �f�o�b�O�̃|�C���^��Ԃ�
	return m_pDebug;
}

#endif	// _DEBUG
