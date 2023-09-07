//============================================================
//
//	�}�i�t�����[���� [flower.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "flower.h"
#include "manager.h"
#include "sceneGame.h"
#include "renderer.h"
#include "texture.h"

#include "collision.h"
#include "player.h"
#include "target.h"
#include "shadow.h"
#include "field.h"
#include "stage.h"

//************************************************************
//	�}�N����`
//************************************************************
#define FLOWER_PRIO	(3)	// �}�i�t�����[�̗D�揇��

#define SHADOW_SIZE	(D3DXVECTOR3(80.0f, 0.0f, 80.0f))	// �e�̑傫��
#define SHADOW_ALPHA	(0.2f)	// �e�̃��l

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CFlower::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\magicFlower000.png",	// �}�i�t�����[�e�N�X�`��
};
int CFlower::m_nNumAll = 0;	// �}�i�t�����[�̑���

//************************************************************
//	�q�N���X [CFlower] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CFlower::CFlower(void) : CObjectBillboard(CObject::LABEL_FLOWER, FLOWER_PRIO)
{
	// �����o�ϐ����N���A
	m_pShadow = NULL;		// �e�̏��
	m_type = TYPE_NORMAL;	// ���
	m_nLife = 0;			// ����

	// �}�i�t�����[�̑��������Z
	m_nNumAll++;
}

//============================================================
//	�f�X�g���N�^
//============================================================
CFlower::~CFlower()
{
	// �}�i�t�����[�̑��������Z
	m_nNumAll--;
}

//============================================================
//	����������
//============================================================
HRESULT CFlower::Init(void)
{
	// �����o�ϐ���������
	m_type = TYPE_NORMAL;	// ���
	m_nLife = 0;			// ����

	// �e�̐���
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, SHADOW_SIZE, this, SHADOW_ALPHA, SHADOW_ALPHA);
	if (UNUSED(m_pShadow))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �I�u�W�F�N�g�r���{�[�h�̏�����
	if (FAILED(CObjectBillboard::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CFlower::Uninit(void)
{
	// �e��j��
	m_pShadow->Uninit();

	// �I�u�W�F�N�g�r���{�[�h�̏I��
	CObjectBillboard::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CFlower::Update(void)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = GetPosition();	// �ʒu

	// TODO�F�Ԃƃv���C���[�̓����蔻��
#if 0
	// �v���C���[�Ƃ̓����蔻��
	if (CollisionPlayer())
	{ // �v���C���[�ɓ������Ă���ꍇ

		// �I�u�W�F�N�g�̏I��
		Uninit();

		// �֐��𔲂���
		return;
	}
#endif

	// �ʒu�����߂�
	pos.y = CSceneGame::GetField()->GetPositionHeight(pos);	// ������n�ʂɐݒ�

	// �ʒu���X�V
	SetPosition(pos);

	// �e�̍X�V
	m_pShadow->Update();

	// �I�u�W�F�N�g�r���{�[�h�̍X�V
	CObjectBillboard::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CFlower::Draw(void)
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
CFlower *CFlower::Create
(
	const TYPE type,			// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rSize,	// �傫��
	const int nLife				// �̗�
)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = rPos;	// ���W�ݒ�p
	int nTextureID;	// �e�N�X�`���C���f�b�N�X

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^
	CFlower *pFlower = NULL;	// �}�i�t�����[�����p

	if (UNUSED(pFlower))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pFlower = new CFlower;	// �}�i�t�����[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pFlower))
	{ // �m�ۂɐ������Ă���ꍇ

		// �}�i�t�����[�̏�����
		if (FAILED(pFlower->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pFlower;
			pFlower = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �e�N�X�`����o�^
		nTextureID = pTexture->Regist(mc_apTextureFile[type]);

		// �e�N�X�`��������
		pFlower->BindTexture(nTextureID);

		// ���_��ݒ�
		pFlower->SetOrigin(ORIGIN_DOWN);

		// �ʒu��ݒ�
		pos.y = CSceneGame::GetField()->GetPositionHeight(pos);	// ������n�ʂɐݒ�
		pFlower->SetPosition(pos);

		// �傫����ݒ�
		pFlower->SetScaling(rSize);

		// �̗͂�ݒ�
		pFlower->SetLife(nLife);

		// ��]��ݒ�
		pFlower->SetRotate(ROTATE_LATERAL);

		// Z�e�X�g��ݒ�
		pFlower->SetFunc(D3DCMP_LESSEQUAL);

		// Z�o�b�t�@�̎g�p�󋵂�ݒ�
		pFlower->SetZEnable(true);

		// �m�ۂ����A�h���X��Ԃ�
		return pFlower;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�����_����������
//============================================================
void CFlower::RandomSpawn
(
	const int nNum,				// ������
	const TYPE type,			// ���
	const D3DXVECTOR3& rSize,	// �傫��
	const int nLife				// �̗�
)
{
	// �ϐ���錾
	D3DXVECTOR3 pos;	// �ʒu�ݒ�p
	int nLimit = (int)CSceneGame::GetStage()->GetStageLimit().fRadius;	// �X�e�[�W�͈�

	// �|�C���^��錾
	CTarget *pTarget = CSceneGame::GetTarget();	// �^�[�Q�b�g���

	if (USED(CSceneGame::GetTarget()))
	{ // �^�[�Q�b�g���g�p����Ă���ꍇ

		for (int nCntGrow = 0; nCntGrow < nNum; nCntGrow++)
		{ // ���������J��Ԃ�

			// �����ʒu��ݒ�
			pos.x = (float)(rand() % (nLimit * 2) - nLimit + 1);
			pos.y = 0.0f;
			pos.z = (float)(rand() % (nLimit * 2) - nLimit + 1);

			// �����ʒu��␳
			collision::CirclePillar(pos, pTarget->GetPosition(), rSize.x, pTarget->GetRadius());	// �^�[�Q�b�g�����̐����h�~
			CSceneGame::GetStage()->LimitPosition(pos, rSize.x);									// �X�e�[�W�͈͊O�̐����h�~

			// �}�i�t�����[�I�u�W�F�N�g�̐���
			CFlower::Create(type, pos, rSize, nLife);
		}
	}
}

//============================================================
//	�����擾����
//============================================================
int CFlower::GetNumAll(void)
{
	// ���݂̃}�i�t�����[�̑�����Ԃ�
	return m_nNumAll;
}

//============================================================
//	�̗͂̐ݒ菈��
//============================================================
void CFlower::SetLife(const int nLife)
{
	// �����̗̑͂�ݒ�
	m_nLife = nLife;
}

//============================================================
//	�v���C���[�Ƃ̓����蔻��
//============================================================
bool CFlower::CollisionPlayer(void)
{
	// �ϐ���錾
	bool bHit = false;	// �����

	// �|�C���^��錾
	CPlayer *pPlayer = CSceneGame::GetPlayer();	// �v���C���[���

	if (pPlayer->GetState() != CPlayer::STATE_DEATH)
	{ // �v���C���[���g�p����Ă���ꍇ

		// TODO
	}

	// ����󋵂�Ԃ�
	return bHit;
}
