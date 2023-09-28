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
#include "sound.h"
#include "camera.h"
#include "titleManager.h"

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
//	�ÓI�����o�ϐ��錾
//************************************************************
CTitleManager *CSceneTitle::m_pTitleManager = NULL;	// �^�C�g���}�l�[�W���[

//************************************************************
//	�q�N���X [CSceneTitle] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSceneTitle::CSceneTitle(const MODE mode) : CScene(mode)
{

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
	CWaveManager::SEASON season = (CWaveManager::SEASON)(rand() % CWaveManager::SEASON_MAX);	// �G��

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^

	//--------------------------------------------------------
	//	�^�C�g���̏�����
	//--------------------------------------------------------
	// �^�C�g���}�l�[�W���[�̐���
	m_pTitleManager = CTitleManager::Create();
	if (UNUSED(m_pTitleManager))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �V�[���̏�����
	CScene::Init();

	//--------------------------------------------------------
	//	�I�u�W�F�N�g�����E������
	//--------------------------------------------------------
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

	//--------------------------------------------------------
	//	�����ݒ�
	//--------------------------------------------------------
	// �X�e�[�W�G���A�E�o���A�̕`����~
	GetStage()->SetEnableDrawArea(false);
	GetStage()->SetEnableDrawBarrier(false);

	// �^�[�Q�b�g�̗͂̕`����~
	GetTarget()->SetEnableDrawLife(false);

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
	CManager::GetCamera()->SetDestRotate();					// �ڕW�ʒu��ݒ�
	CManager::GetCamera()->SetEnableUpdate(false);			// �X�V���~

	// BGM�̍Đ�
	CManager::GetSound()->Play(CSound::LABEL_BGM_TITLE_RESULT);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CSceneTitle::Uninit(void)
{
	// �^�C�g���}�l�[�W���[�̔j��
	if (FAILED(CTitleManager::Release(m_pTitleManager)))
	{ // �j���Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �V�[���̏I��
	CScene::Uninit();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CSceneTitle::Update(void)
{
	if (USED(m_pTitleManager))
	{ // �g�p���̏ꍇ

		// �^�C�g���}�l�[�W���[�̍X�V
		m_pTitleManager->Update();
	}
	else { assert(false); }	// ��g�p��

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
//	�^�C�g���}�l�[�W���[�擾����
//============================================================
CTitleManager *CSceneTitle::GetTitleManager(void)
{
	// �^�C�g���}�l�[�W���[��Ԃ�
	return m_pTitleManager;
}
