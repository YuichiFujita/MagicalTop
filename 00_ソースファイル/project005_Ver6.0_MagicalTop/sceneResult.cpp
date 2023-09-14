//============================================================
//
//	���U���g��ʏ��� [sceneResult.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sceneResult.h"
#include "manager.h"
#include "input.h"
#include "texture.h"
#include "object2D.h"

//************************************************************
//	�q�N���X [CSceneResult] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSceneResult::CSceneResult(const MODE mode) : CScene(mode)
{
	// �����o�ϐ����N���A
	m_pObject2D = NULL;	// �^�C�g���\���p
}

//============================================================
//	�f�X�g���N�^
//============================================================
CSceneResult::~CSceneResult()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSceneResult::Init(void)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����o�ϐ���������
	m_pObject2D = NULL;	// �^�C�g���\���p

	// �I�u�W�F�N�g2D�̐���
	m_pObject2D = CObject2D::Create
	( // ����
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),	// �ʒu
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f)					// �傫��
	);
	if (UNUSED(m_pObject2D))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pObject2D->BindTexture(pTexture->Regist("data\\TEXTURE\\result000.png"));

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CSceneResult::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I��
	m_pObject2D->Uninit();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CSceneResult::Update(void)
{
	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN)
	||  CManager::GetKeyboard()->GetTrigger(DIK_SPACE)
	||  CManager::GetPad()->GetTrigger(CInputPad::KEY_A)
	||  CManager::GetPad()->GetTrigger(CInputPad::KEY_B)
	||  CManager::GetPad()->GetTrigger(CInputPad::KEY_X)
	||  CManager::GetPad()->GetTrigger(CInputPad::KEY_Y)
	||  CManager::GetPad()->GetTrigger(CInputPad::KEY_START))
	{
		// �V�[���̐ݒ�
		CManager::SetScene(MODE_TITLE);	// �^�C�g�����
	}

	// �V�[���̍X�V
	CScene::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CSceneResult::Draw(void)
{

}
