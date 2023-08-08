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
#include "sceneGame.h"
#include "sceneResult.h"

//************************************************************
//	�e�N���X [CScene] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CScene::CScene()
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CScene::~CScene()
{

}

//============================================================
//	�X�V����
//============================================================
void CScene::Update(void)
{
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
			pScene = new CSceneTitle;

			break;

		case MODE_GAME:

			// �Q�[����ʂ𐶐�
			pScene = new CSceneGame;

			break;

		case MODE_RESULT:

			// ���U���g��ʂ𐶐�
			pScene = new CSceneResult;

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

		// ���[�h�̐ݒ�
		pScene->SetMode(mode);

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
