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
#include "fade.h"
#include "texture.h"
#include "model.h"
#include "gameManager.h"
#include "object.h"

#ifdef _DEBUG	// �f�o�b�O����

#include "debug.h"

#endif	// _DEBUG

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CRenderer		*CManager::m_pRenderer		= NULL;		// �����_���[�I�u�W�F�N�g
CInputKeyboard	*CManager::m_pKeyboard		= NULL;		// �L�[�{�[�h�I�u�W�F�N�g
CInputMouse		*CManager::m_pMouse			= NULL;		// �}�E�X�I�u�W�F�N�g
CInputPad		*CManager::m_pPad			= NULL;		// �p�b�h�I�u�W�F�N�g
CSound			*CManager::m_pSound			= NULL;		// �T�E���h�I�u�W�F�N�g
CCamera			*CManager::m_pCamera		= NULL;		// �J�����I�u�W�F�N�g
CLight			*CManager::m_pLight			= NULL;		// ���C�g�I�u�W�F�N�g
CTexture		*CManager::m_pTexture		= NULL;		// �e�N�X�`���I�u�W�F�N�g
CModel			*CManager::m_pModel			= NULL;		// ���f���I�u�W�F�N�g
CFade			*CManager::m_pFade			= NULL;		// �t�F�[�h�I�u�W�F�N�g
CScene			*CManager::m_pScene			= NULL;		// �V�[���I�u�W�F�N�g
CGameManager	*CManager::m_pGameManager	= NULL;		// �Q�[���}�l�[�W���[
CDebugProc		*CManager::m_pDebugProc		= NULL;		// �f�o�b�O�\���I�u�W�F�N�g

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

	// �Q�[���}�l�[�W���[�̐���
	m_pGameManager = CGameManager::Create();
	if (UNUSED(m_pGameManager))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	���̓Ǎ��E�ݒ�
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

	// �t�F�[�h�̐����E�V�[���̐ݒ�
	m_pFade = CFade::Create();
	if (UNUSED(m_pFade))
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
	// �Q�[���}�l�[�W���[�̔j��
	if (FAILED(CGameManager::Release(m_pGameManager)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �V�[���̔j��
	if (FAILED(CScene::Release(m_pScene)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �t�F�[�h�̔j��
	if (FAILED(CFade::Release(m_pFade)))
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

	// �I�u�W�F�N�g�̑S�j��
	CObject::ReleaseAll();

	// ��O����
	assert(CObject::GetNumAll() == 0);	// �j���̎��s

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

	if (USED(m_pFade))
	{ // �g�p���̏ꍇ

		// �t�F�[�h�̍X�V
		m_pFade->Update();
	}
	else { assert(false); }	// ��g�p��

	if (USED(m_pScene))
	{ // �g�p���̏ꍇ

		// �V�[���̍X�V
		m_pScene->Update();
	}
	else { assert(false); }	// ��g�p��

	if (USED(m_pGameManager))
	{ // �g�p���̏ꍇ

		// �Q�[���}�l�[�W���[�̍X�V
		m_pGameManager->Update();
	}
	else { assert(false); }	// ��g�p��

	//--------------------------------------------------------
	//	�f�o�b�O�p
	//--------------------------------------------------------
	if (USED(m_pDebugProc))
	{ // �g�p���̏ꍇ

		// �f�o�b�O�\���̍X�V
		m_pDebugProc->Update();
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
//	�V�[���̐ݒ菈��
//============================================================
void CManager::SetScene(const CScene::MODE mode, const int nWait)
{
	// ���̃V�[����ݒ�
	m_pFade->Set(mode, nWait);
}

//============================================================
//	���[�h�̐ݒ菈��
//============================================================
HRESULT CManager::SetMode(const CScene::MODE mode)
{
	// �T�E���h���~
	m_pSound->Stop();

	if (USED(m_pScene))
	{ // �V�[�����g�p���̏ꍇ

		// �V�[���̔j��
		if (FAILED(CScene::Release(m_pScene)))
		{ // �j���Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// �I�u�W�F�N�g�̑S�j��
	CObject::ReleaseAll();

	if (UNUSED(m_pScene))
	{ // �V�[������g�p���̏ꍇ

		// �V�[���̐���
		m_pScene = CScene::Create(mode);
		if (UNUSED(m_pScene))
		{ // ��g�p���̏ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}
	else { assert(false); return E_FAIL; }	// �g�p��

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	���[�h�̎擾����
//============================================================
CScene::MODE CManager::GetMode(void)
{
	// ���݂̃��[�h��Ԃ�
	return m_pScene->GetMode();
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
//	�t�F�[�h�擾����
//============================================================
CFade *CManager::GetFade(void)
{
	// �t�F�[�h�̃|�C���^��Ԃ�
	return m_pFade;
}

//============================================================
//	�V�[���擾����
//============================================================
CScene *CManager::GetScene(void)
{
	// �V�[���̃|�C���^��Ԃ�
	return m_pScene;
}

//============================================================
//	�Q�[���}�l�[�W���[�擾����
//============================================================
CGameManager *CManager::GetGameManager(void)
{
	// �Q�[���}�l�[�W���[��Ԃ�
	return m_pGameManager;
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
