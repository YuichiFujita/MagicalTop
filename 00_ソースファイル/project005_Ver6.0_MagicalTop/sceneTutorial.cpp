//============================================================
//
//	�`���[�g���A����ʏ��� [sceneTutorial.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sceneTutorial.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"

#include "stage.h"
#include "score.h"

#include "enemy.h"
#include "magic.h"
#include "sea.h"
#include "wall.h"
#include "scenery.h"
#include "sky.h"
#include "flower.h"
#include "weed.h"

//************************************************************
//	�}�N����`
//************************************************************
#define SCO_POS		(D3DXVECTOR3(825.0f, 50.0f, 0.0f))		// �X�R�A�ʒu
#define SCO_SIZE	(D3DXVECTOR3(60.0f, 80.0f, 0.0f))		// �X�R�A�傫��
#define SCO_SPACE	(D3DXVECTOR3(SCO_SIZE.x, 0.0f, 0.0f))	// �X�R�A��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CScore *CSceneTutorial::m_pScore = NULL;	// �X�R�A�I�u�W�F�N�g

//************************************************************
//	�q�N���X [CSceneTutorial] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSceneTutorial::CSceneTutorial(const MODE mode) : CScene(mode)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSceneTutorial::~CSceneTutorial()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSceneTutorial::Init(void)
{
	//--------------------------------------------------------
	//	�`���[�g���A���̏�����
	//--------------------------------------------------------
	//// ���U���g�}�l�[�W���[�̐���
	//m_pResultManager = CResultManager::Create();
	//if (UNUSED(m_pResultManager))
	//{ // ��g�p���̏ꍇ

	//	// ���s��Ԃ�
	//	assert(false);
	//	return E_FAIL;
	//}

	// �V�[���̏�����
	CScene::Init();

	//--------------------------------------------------------
	//	�I�u�W�F�N�g����
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

	// �X�R�A�I�u�W�F�N�g�̐���
	m_pScore = CScore::Create
	( // ����
		SCO_POS,	// �ʒu
		SCO_SIZE,	// �傫��
		SCO_SPACE	// ��
	);
	if (UNUSED(m_pScore))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	�����ݒ�
	//--------------------------------------------------------
	// �G�Z�b�g�A�b�v�̓Ǎ�
	CEnemy::LoadSetup();

	// ���@�Z�b�g�A�b�v�̓Ǎ�
	CMagic::LoadSetup();

	// �}�i�t�����[�Z�b�g�A�b�v�̓Ǎ�
	CFlower::LoadSetup();

	// ���Z�b�g�A�b�v�̓Ǎ�
	CWeed::LoadSetup();

	// �}�i�t�����[�����_������
	CFlower::RandomSpawn(30, CFlower::TYPE_SPRING);	// TODO�F�萔�ύX

	// �������_������
	CWeed::RandomSpawn(200, CWeed::TYPE_SPRING);	// TODO�F�萔�ύX

	// �J������ݒ�
	CManager::GetCamera()->SetState(CCamera::STATE_BARGAINING);	// �J��������������Ԃɐݒ�
	CManager::GetCamera()->SetDestBargaining();	// �ڕW�ʒu��ݒ�

	// BGM�̍Đ�
	//CManager::GetSound()->Play(CSound::LABEL_BGM_000);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CSceneTutorial::Uninit(void)
{
	// �I���ς݂̃I�u�W�F�N�g�|�C���^��NULL�ɂ���
	m_pScore = NULL;	// �X�R�A�I�u�W�F�N�g

	// �V�[���̏I��
	CScene::Uninit();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CSceneTutorial::Update(void)
{
	// �V�[���̍X�V
	CScene::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CSceneTutorial::Draw(void)
{

}

//============================================================
//	�X�R�A�擾����
//============================================================
CScore *CSceneTutorial::GetScore(void)
{
	// �X�R�A�̃|�C���^��Ԃ�
	return m_pScore;
}
