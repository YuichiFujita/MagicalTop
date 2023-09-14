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
#include "texture.h"
#include "input.h"
#include "camera.h"
#include "object2D.h"

#include "stage.h"
#include "target.h"
#include "field.h"
#include "sea.h"
#include "wall.h"
#include "scenery.h"
#include "sky.h"
#include "flower.h"
#include "weed.h"

//************************************************************
//	�}�N����`
//************************************************************
#define CREATE_FLOWER	(30)	// �}�i�t�����[�̐�����
#define CREATE_WEED		(200)	// ���̐�����

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
	// �ϐ���錾
	CWaveManager::SEASON season = (CWaveManager::SEASON)(rand() % CWaveManager::SEASON_MAX);	// �G��

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^
	CTarget *pTarget = NULL;	// �^�[�Q�b�g�ݒ�p

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
	m_pObject2D->BindTexture(pTexture->Regist("data\\TEXTURE\\result000.png"));

	//--------------------------------------------------------
	//	�I�u�W�F�N�g�����E������
	//--------------------------------------------------------
	// �V�[���̏�����
	CScene::Init();

	// �C�I�u�W�F�N�g�̐���
	CSea::Create();

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

	// �^�[�Q�b�g�I�u�W�F�N�g�̐���
	pTarget = CTarget::Create(CTarget::MODEL_NORMAL, D3DXVECTOR3(0.0f, 400.0f, 0.0f), VEC3_ZERO);
	if (UNUSED(pTarget))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	�����ݒ�
	//--------------------------------------------------------
	// �X�e�[�W�G���A�E�o���A�̕`����~
	GetStage()->SetEnableDrawArea(false);
	GetStage()->SetEnableDrawBarrier(false);

	// �^�[�Q�b�g�̗͂̕`����~
	pTarget->SetEnableDrawLife(false);

	// �}�i�t�����[�Z�b�g�A�b�v�̓Ǎ�
	CFlower::LoadSetup();

	// ���Z�b�g�A�b�v�̓Ǎ�
	CWeed::LoadSetup();

	// �}�i�t�����[�����_������
	CFlower::RandomSpawn(CREATE_FLOWER, CFlower::TYPE_SPRING);

	// �������_������
	CWeed::RandomSpawn(CREATE_WEED, CWeed::TYPE_SPRING);

	// �G�߂�ݒ�
	GetField()->SetSeason(season);	// �n�ʂ�ύX
	CFlower::SetSeason(season);		// �Ԃ�ύX
	CWeed::SetSeason(season);		// ����ύX

	// �J������ݒ�
	CManager::GetCamera()->SetState(CCamera::STATE_ROTATE);	// �J��������]��Ԃɐݒ�
	CManager::GetCamera()->SetDestRotate();	// �ڕW�ʒu��ݒ�

	// BGM�̍Đ�
	//CManager::GetSound()->Play(CSound::LABEL_BGM_000);

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

	// �V�[���̏I��
	CScene::Uninit();

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
