//============================================================
//
//	�o���l���� [expOrb.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "expOrb.h"
#include "manager.h"
#include "scene.h"
#include "sceneGame.h"
#include "renderer.h"
#include "texture.h"

#include "collision.h"
#include "player.h"
#include "target.h"
#include "field.h"
#include "stage.h"

//************************************************************
//	�}�N����`
//************************************************************
#define EXP_MOVE		(19.0f)	// �o���l�̈ړ���
#define EXP_REV_ROT		(0.25f)	// �o���l�z�[�~���O���̌����␳�W��
#define EXP_PLUS_POSY	(30.0f)	// �o���l�̒n�ʂ���̏㏸��
#define EXP_RADIUS		(80.0f)	// �o���l�̔��a

#define EXP_SPAWN_RADIUS	(250)	// �o���l�o�����̃����_���͈�
#define LIMIT_RADIUS	(3000.0f)	// �o���l�̃X�e�[�W�ړ����a

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CExpOrb::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\exp000.png",	// �o���l�e�N�X�`��
};

//************************************************************
//	�q�N���X [CExpOrb] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CExpOrb::CExpOrb() : CObjectBillboard(CObject::LABEL_EXP)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CExpOrb::~CExpOrb()
{

}

//============================================================
//	����������
//============================================================
HRESULT CExpOrb::Init(void)
{
	// �ϐ���錾
	int nTextureID;	// �e�N�X�`���C���f�b�N�X

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �I�u�W�F�N�g�r���{�[�h�̏�����
	if (FAILED(CObjectBillboard::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^
	nTextureID = pTexture->Regist(mc_apTextureFile[TEXTURE_NORMAL]);

	// �e�N�X�`��������
	BindTexture(nTextureID);

	// �傫����ݒ�
	SetScaling(D3DXVECTOR3(EXP_RADIUS, EXP_RADIUS, 0.0f));

	// Z�e�X�g��ݒ�
	SetFunc(D3DCMP_LESSEQUAL);

	// Z�o�b�t�@�̎g�p�󋵂�ݒ�
	SetZEnable(true);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CExpOrb::Uninit(void)
{
	// �I�u�W�F�N�g�r���{�[�h�̏I��
	CObjectBillboard::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CExpOrb::Update(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posPlayer = CScene::GetPlayer()->GetPosition();	// �v���C���[�ʒu
	D3DXVECTOR3 posTarget = CScene::GetTarget()->GetPosition();	// �^�[�Q�b�g�ʒu
	D3DXVECTOR3 posExp = CObjectBillboard::GetPosition();		// �o���l�ʒu
	D3DXVECTOR3 rotExp = CObjectBillboard::GetRotation();		// �o���l����
	float fDestRot = 0.0f;	// �ڕW����
	float fDiffRot = 0.0f;	// ����

	// �|�C���^��錾
	CTarget *pTarget = CScene::GetTarget();	// �^�[�Q�b�g���

	if (CScene::GetPlayer()->GetState() != CPlayer::STATE_DEATH && USED(CScene::GetTarget()))
	{ // �v���C���[�����S���Ă��Ȃ����A�^�[�Q�b�g���g�p����Ă���ꍇ

		// �v���C���[�̕�������
		fDestRot = atan2f(posExp.x - posPlayer.x, posExp.z - posPlayer.z);	// �ڕW����

		// �������������߂�
		fDiffRot = fDestRot - rotExp.y;
		useful::NormalizeRot(fDiffRot);	// ���������̐��K��

		// �����̍X�V
		rotExp.y += fDiffRot * EXP_REV_ROT;
		useful::NormalizeRot(rotExp.y);	// �����̐��K��

		// �ʒu��ݒ�
		posExp.x -= sinf(rotExp.y) * EXP_MOVE;
		posExp.y = 0.0f;
		posExp.z -= cosf(rotExp.y) * EXP_MOVE;

		// �n�ʂƂ̓����蔻��
		posExp.y = CScene::GetField()->GetPositionHeight(posExp);
		posExp.y += EXP_PLUS_POSY;

		// �~���̓�������
		collision::InCirclePillar(posExp, posTarget, EXP_RADIUS, LIMIT_RADIUS);

		// �ʒu��ݒ�
		CObjectBillboard::SetPosition(posExp);

		// ������ݒ�
		CObjectBillboard::SetRotation(rotExp);

		// �v���C���[�Ƃ̓����蔻��
		if (CollisionPlayer())
		{ // �v���C���[�ɓ������Ă���ꍇ

			// �I�u�W�F�N�g�̏I��
			Uninit();

			// �֐��𔲂���
			return;
		}
	}

	// �I�u�W�F�N�g�r���{�[�h�̍X�V
	CObjectBillboard::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CExpOrb::Draw(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// ���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// ���e�X�g�̗L�� / �����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 160);				// ���e�X�g�̎Q�ƒl�ݒ�

	// �I�u�W�F�N�g�r���{�[�h�̕`��
	CObjectBillboard::Draw();

	// ���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g�̗L�� / �����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// ���e�X�g�̎Q�ƒl�ݒ�
}

//============================================================
//	��������
//============================================================
CExpOrb *CExpOrb::Create(const D3DXVECTOR3& rPos)
{
	// �|�C���^��錾
	CExpOrb *pExpOrb = NULL;	// �o���l�����p

	if (UNUSED(pExpOrb))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pExpOrb = new CExpOrb;	// �o���l
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pExpOrb))
	{ // �m�ۂɐ������Ă���ꍇ

		// �o���l�̏�����
		if (FAILED(pExpOrb->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pExpOrb;
			pExpOrb = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pExpOrb->SetPosition(rPos);

		// �m�ۂ����A�h���X��Ԃ�
		return pExpOrb;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�����_����������
//============================================================
void CExpOrb::RandomSpawn(const int nNum, const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 pos;	// �ʒu�ݒ�p

	// �|�C���^��錾
	CTarget *pTarget = CScene::GetTarget();	// �^�[�Q�b�g���

	if (USED(CScene::GetTarget()))
	{ // �^�[�Q�b�g���g�p����Ă���ꍇ

		for (int nCntGrow = 0; nCntGrow < nNum; nCntGrow++)
		{ // ���������J��Ԃ�

			// �����ʒu��ݒ�
			pos.x = rPos.x + (float)(rand() % (EXP_SPAWN_RADIUS * 2) - EXP_SPAWN_RADIUS + 1);
			pos.y = rPos.y;
			pos.z = rPos.z + (float)(rand() % (EXP_SPAWN_RADIUS * 2) - EXP_SPAWN_RADIUS + 1);

			// �~���̓�������
			collision::InCirclePillar(pos, pTarget->GetPosition(), EXP_RADIUS, LIMIT_RADIUS);

			// �o���l�I�u�W�F�N�g�̐���
			CExpOrb::Create(pos);
		}
	}
}

//============================================================
//	�v���C���[�Ƃ̓����蔻��
//============================================================
bool CExpOrb::CollisionPlayer(void)
{
	// �ϐ���錾
	bool bHit = false;	// �����

	// �|�C���^��錾
	CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[���

	if (pPlayer->GetState() != CPlayer::STATE_DEATH)
	{ // �v���C���[���g�p����Ă���ꍇ

		// �v���C���[�Ƃ̓����蔻��
		bHit = collision::Circle2D
		( // ����
			GetPosition(),				// �o���l�ʒu
			pPlayer->GetPosition(),		// �v���C���[�ʒu
			EXP_RADIUS,					// �o���l���a
			pPlayer->GetRadius() * 0.5f	// �v���C���[���a
		);

		if (bHit)
		{ // �������Ă����ꍇ

			// �v���C���[�̌o���l�̉��Z
			CScene::GetPlayer()->AddExp(1);
		}
	}

	// ����󋵂�Ԃ�
	return bHit;
}
