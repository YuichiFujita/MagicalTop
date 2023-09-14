//============================================================
//
//	�^�C�g����ʏ��� [sceneTitle.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sceneTitle.h"
#include "manager.h"
#include "texture.h"
#include "input.h"
#include "camera.h"
#include "object2D.h"

#include "stage.h"
#include "target.h"
#include "player.h"

#include "sea.h"
#include "field.h"
#include "wall.h"
#include "scenery.h"
#include "sky.h"
#include "flower.h"
#include "weed.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CStage	*CSceneTitle::m_pStage = NULL;	// �X�e�[�W
CField	*CSceneTitle::m_pField = NULL;	// �n�ʃI�u�W�F�N�g

//************************************************************
//	�q�N���X [CSceneTitle] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSceneTitle::CSceneTitle()
{
	// �����o�ϐ����N���A
	m_pObject2D = NULL;	// �^�C�g���\���p
}

//============================================================
//	�f�X�g���N�^
//============================================================
CSceneTitle::~CSceneTitle()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSceneTitle::Init(void)
{
	// �ϐ���錾
	CWaveManager::SEASON season = (CWaveManager::SEASON)(rand() % CWaveManager::SEASON_MAX + 1);	// �G��

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����o�ϐ���������
	m_pObject2D = NULL;	// �^�C�g���\���p

	// �I�u�W�F�N�g2D�̐���
	m_pObject2D = CObject2D::Create
	( // ����
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),	// �ʒu
		//D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f)					// �傫��
		D3DXVECTOR3(100.0f, 100.0f, 0.0f)
	);
	if (UNUSED(m_pObject2D))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pObject2D->BindTexture(pTexture->Regist("data\\TEXTURE\\title000.png"));

	//--------------------------------------------------------
	//	�I�u�W�F�N�g����
	//--------------------------------------------------------
	// �X�e�[�W�̐���
	m_pStage = CStage::Create();
	if (UNUSED(m_pStage))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �C�I�u�W�F�N�g�̐���
	CSea::Create();

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

	// �ǃI�u�W�F�N�g�̐���
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3( 0.0f,    0.0f, -3000.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),   D3DXVECTOR2(6000.0f, 400.0f), XCOL_WHITE, POSGRID2(18, 1));
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3(-3000.0f, 0.0f,  0.0f),    D3DXToRadian(D3DXVECTOR3(0.0f, 90.0f, 0.0f)),  D3DXVECTOR2(6000.0f, 400.0f), XCOL_WHITE, POSGRID2(18, 1));
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3( 0.0f,    0.0f,  3000.0f), D3DXToRadian(D3DXVECTOR3(0.0f, 180.0f, 0.0f)), D3DXVECTOR2(6000.0f, 400.0f), XCOL_WHITE, POSGRID2(18, 1));
	CWall::Create(CWall::TEXTURE_NORMAL, D3DXVECTOR3( 3000.0f, 0.0f,  0.0f),    D3DXToRadian(D3DXVECTOR3(0.0f, 270.0f, 0.0f)), D3DXVECTOR2(6000.0f, 400.0f), XCOL_WHITE, POSGRID2(18, 1));

	// �i�F�I�u�W�F�N�g�̐���
	CScenery::Create(CScenery::TEXTURE_NORMAL, VEC3_ZERO, VEC3_ZERO,                                    XCOL_WHITE,                        POSGRID2(32, 1), 12000.0f, 1000.0f, D3DCULL_CW, false);
	CScenery::Create(CScenery::TEXTURE_NORMAL, VEC3_ZERO, D3DXToRadian(D3DXVECTOR3(0.0f, 85.0f, 0.0f)), D3DXCOLOR(0.7f, 1.0f, 1.0f, 1.0f), POSGRID2(32, 1), 14000.0f, 1600.0f, D3DCULL_CW, false);
	CScenery::Create(CScenery::TEXTURE_NORMAL, VEC3_ZERO, D3DXToRadian(D3DXVECTOR3(0.0f, 35.0f, 0.0f)), D3DXCOLOR(0.4f, 1.0f, 0.7f, 1.0f), POSGRID2(32, 1), 16000.0f, 2200.0f, D3DCULL_CW, false);

	// ��I�u�W�F�N�g�̐���
	CSky::Create(CSky::TEXTURE_NORMAL, VEC3_ZERO, VEC3_ZERO, XCOL_WHITE, POSGRID2(32, 6), 18000.0f, D3DCULL_CW, false);

#if 0
	// �^�[�Q�b�g�I�u�W�F�N�g�̐���
	m_pTarget = CTarget::Create(CTarget::MODEL_NORMAL, D3DXVECTOR3(0.0f, 400.0f, 0.0f), VEC3_ZERO);
	if (UNUSED(m_pTarget))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �v���C���[�I�u�W�F�N�g�̐���
	m_pPlayer = CPlayer::Create(VEC3_ZERO, VEC3_ZERO);
	if (UNUSED(m_pPlayer))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}
#endif

	//--------------------------------------------------------
	//	�����ݒ�
	//--------------------------------------------------------
#if 0
	// �}�i�t�����[�Z�b�g�A�b�v�̓Ǎ�
	CFlower::LoadSetup();

	// ���Z�b�g�A�b�v�̓Ǎ�
	CWeed::LoadSetup();

	// �}�i�t�����[�����_������
	CFlower::RandomSpawn(30, CFlower::TYPE_SPRING);	// TODO�F�萔�ύX

	// �������_������
	CWeed::RandomSpawn(200, CWeed::TYPE_SPRING);	// TODO�F�萔�ύX
#endif

	// �G�߂�ݒ�
	m_pField->SetSeason(season);	// �n�ʂ�ύX
	CFlower::SetSeason(season);		// �Ԃ�ύX
	CWeed::SetSeason(season);		// ����ύX

	// �J������ݒ�
	CManager::GetCamera()->SetState(CCamera::STATE_ROTATE);	// �J��������]��Ԃɐݒ�
	CManager::GetCamera()->SetDestRotate();	// �ڕW�ʒu��ݒ�

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CSceneTitle::Uninit(void)
{
	// �X�e�[�W�̔j��
	if (FAILED(CStage::Release(m_pStage)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �I�u�W�F�N�g2D�̏I��
	m_pObject2D->Uninit();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CSceneTitle::Update(void)
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
		CManager::SetScene(MODE_GAME);	// �Q�[�����
	}

#if 0
	if (USED(m_pStage))
	{ // �g�p���̏ꍇ

		// �X�e�[�W�̍X�V
		m_pStage->Update();
	}
	else { assert(false); }	// ��g�p��
#endif

	// �V�[���̍X�V
	CScene::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CSceneTitle::Draw(void)
{

}

//============================================================
//	�X�e�[�W�擾����
//============================================================
CStage *CSceneTitle::GetStage(void)
{
	// �X�e�[�W�̃|�C���^��Ԃ�
	return m_pStage;
}

//============================================================
//	�n�ʎ擾����
//============================================================
CField *CSceneTitle::GetField(void)
{
	// �n�ʂ̃|�C���^��Ԃ�
	return m_pField;
}
