//============================================================
//
//	�V�[������ [scene.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "scene.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"
#include "camera.h"
#include "sceneTitle.h"
#include "sceneTutorial.h"
#include "sceneGame.h"
#include "sceneResult.h"

#include "stage.h"
#include "player.h"
#include "target.h"
#include "field.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CStage *CScene::m_pStage = NULL;	// �X�e�[�W
CPlayer	*CScene::m_pPlayer = NULL;	// �v���C���[�I�u�W�F�N�g
CTarget *CScene::m_pTarget = NULL;	// �^�[�Q�b�g�I�u�W�F�N�g
CField *CScene::m_pField = NULL;	// �n�ʃI�u�W�F�N�g

//************************************************************
//	�e�N���X [CScene] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CScene::CScene(const MODE mode)
{
	// �����o�ϐ����N���A
	m_mode = mode;	// ���[�h
}

//============================================================
//	�f�X�g���N�^
//============================================================
CScene::~CScene()
{

}

//============================================================
//	����������
//============================================================
HRESULT CScene::Init(void)
{
	// �X�e�[�W�̐���
	m_pStage = CStage::Create();
	if (UNUSED(m_pStage))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �n�ʃI�u�W�F�N�g�̐���
	if (FAILED(CreateField(m_mode)))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �^�[�Q�b�g�I�u�W�F�N�g�̐���
	m_pTarget = CTarget::Create(CTarget::MODEL_NORMAL, GetStage()->GetStageLimit().center, VEC3_ZERO);
	if (UNUSED(m_pTarget))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �v���C���[�I�u�W�F�N�g�̐���
	m_pPlayer = CPlayer::Create(m_mode, VEC3_ZERO, VEC3_ZERO);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CScene::Uninit(void)
{
	// �X�e�[�W�̔j��
	if (FAILED(CStage::Release(m_pStage)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �I���ς݂̃I�u�W�F�N�g�|�C���^��NULL�ɂ���
	m_pPlayer = NULL;	// �v���C���[�I�u�W�F�N�g
	m_pTarget = NULL;	// �^�[�Q�b�g�I�u�W�F�N�g
	m_pField = NULL;	// �n�ʃI�u�W�F�N�g

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CScene::Update(void)
{
	if (USED(m_pStage))
	{ // �g�p���̏ꍇ

		// �X�e�[�W�̍X�V
		m_pStage->Update();
	}
	else { assert(false); }	// ��g�p��

	if (USED(CManager::GetLight()))
	{ // �g�p���̏ꍇ

		// ���C�g�̍X�V
		CManager::GetLight()->Update();
	}
	else { assert(false); }	// ��g�p��

	if (USED(CManager::GetCamera()))
	{ // �g�p���̏ꍇ

		// �J�����̍X�V
		CManager::GetCamera()->Update();
	}
	else { assert(false); }	// ��g�p��

	if (USED(CManager::GetRenderer()))
	{ // �g�p���̏ꍇ

		// �����_���[�̍X�V
		CManager::GetRenderer()->Update();
	}
	else { assert(false); }	// ��g�p��
}

//============================================================
//	��������
//============================================================
CScene *CScene::Create(MODE mode)
{
	// �|�C���^��錾
	CScene *pScene = NULL;	// �V�[�������p

	if (UNUSED(pScene))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		switch (mode)
		{ // ���[�h���Ƃ̏���
		case MODE_TITLE:

			// �^�C�g����ʂ𐶐�
			pScene = new CSceneTitle(mode);

			break;

		case MODE_TUTORIAL:

			// �`���[�g���A����ʂ𐶐�
			pScene = new CSceneTutorial(mode);

			break;

		case MODE_GAME:

			// �Q�[����ʂ𐶐�
			pScene = new CSceneGame(mode);

			break;

		case MODE_RESULT:

			// ���U���g��ʂ𐶐�
			pScene = new CSceneResult(mode);

			break;

		default:	// ��O����
			assert(false);
			break;
		}
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pScene))
	{ // �m�ۂɐ������Ă���ꍇ

		// �V�[���̏�����
		if (FAILED(pScene->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �V�[���̏I��
			if (FAILED(pScene->Uninit()))
			{ // �V�[���̏I���Ɏ��s�����ꍇ

				// �������J��
				delete pScene;
				pScene = NULL;

				// ���s��Ԃ�
				assert(false);
				return NULL;
			}

			// �������J��
			delete pScene;
			pScene = NULL;

			// ���s��Ԃ�
			assert(false);
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pScene;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CScene::Release(CScene *&prScene)
{
	if (USED(prScene))
	{ // �g�p���̏ꍇ

		// �V�[���̏I��
		if (FAILED(prScene->Uninit()))
		{ // �V�[���̏I���Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �������J��
		delete prScene;
		prScene = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�X�e�[�W�擾����
//============================================================
CStage *CScene::GetStage(void)
{
	// �X�e�[�W�̃|�C���^��Ԃ�
	return m_pStage;
}

//============================================================
//	�v���C���[�擾����
//============================================================
CPlayer *CScene::GetPlayer(void)
{
	// �v���C���[�̃|�C���^��Ԃ�
	return m_pPlayer;
}

//============================================================
//	�^�[�Q�b�g�擾����
//============================================================
CTarget *CScene::GetTarget(void)
{
	// �^�[�Q�b�g�̃|�C���^��Ԃ�
	return m_pTarget;
}

//============================================================
//	�n�ʐ�������
//============================================================
HRESULT CScene::CreateField(MODE mode)
{
	switch (mode)
	{ // ���[�h���Ƃ̏���
	case MODE_TITLE:

		// �n�ʃI�u�W�F�N�g�̐���
		m_pField = CField::Create(CField::TEXTURE_SPRING, D3DXVECTOR3(0.0f, 400.0f, 0.0f), VEC3_ZERO, D3DXVECTOR2(6000.0f, 6000.0f), XCOL_WHITE, POSGRID2(120, 120));
		if (UNUSED(m_pField))
		{ // ��g�p���̏ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �n�`��ݒ�
		m_pField->SetTerrain(CField::TERRAIN_120x120);

		// ������Ԃ�
		return S_OK;

	case MODE_TUTORIAL:

		// �n�ʃI�u�W�F�N�g�̐���
		m_pField = CField::Create(CField::TEXTURE_SPRING, D3DXVECTOR3(0.0f, 400.0f, 0.0f), VEC3_ZERO, D3DXVECTOR2(6000.0f, 6000.0f), XCOL_WHITE, POSGRID2(120, 120));
		if (UNUSED(m_pField))
		{ // ��g�p���̏ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �n�`��ݒ�
		m_pField->SetTerrain(CField::TERRAIN_120x120);

		// ������Ԃ�
		return S_OK;

	case MODE_GAME:

		// �n�ʃI�u�W�F�N�g�̐���
		m_pField = CField::Create(CField::TEXTURE_SPRING, D3DXVECTOR3(0.0f, 400.0f, 0.0f), VEC3_ZERO, D3DXVECTOR2(6000.0f, 6000.0f), XCOL_WHITE, POSGRID2(120, 120));
		if (UNUSED(m_pField))
		{ // ��g�p���̏ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �n�`��ݒ�
		m_pField->SetTerrain(CField::TERRAIN_120x120);

		// ������Ԃ�
		return S_OK;

	case MODE_RESULT:

		// �n�ʃI�u�W�F�N�g�̐���
		m_pField = CField::Create(CField::TEXTURE_SPRING, D3DXVECTOR3(0.0f, 400.0f, 0.0f), VEC3_ZERO, D3DXVECTOR2(6000.0f, 6000.0f), XCOL_WHITE, POSGRID2(120, 120));
		if (UNUSED(m_pField))
		{ // ��g�p���̏ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �n�`��ݒ�
		m_pField->SetTerrain(CField::TERRAIN_120x120);

		// ������Ԃ�
		return S_OK;

	default:	// ��O����
		assert(false);
		return E_FAIL;
	}
}

//============================================================
//	�n�ʎ擾����
//============================================================
CField *CScene::GetField(void)
{
	// �n�ʂ̃|�C���^��Ԃ�
	return m_pField;
}

//============================================================
//	���[�h�̐ݒ菈��
//============================================================
void CScene::SetMode(const MODE mode)
{
	// �����̃��[�h��ݒ�
	m_mode = mode;
}

//============================================================
//	���[�h�擾����
//============================================================
CScene::MODE CScene::GetMode(void) const
{
	// ���݂̃��[�h��Ԃ�
	return m_mode;
}
