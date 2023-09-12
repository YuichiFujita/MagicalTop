//============================================================
//
//	������ [weed.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "weed.h"
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
#define WEED_PRIO	(2)	// ���̗D�揇��

#define SHADOW_SIZE	(D3DXVECTOR3(80.0f, 0.0f, 80.0f))	// �e�̑傫��
#define SHADOW_ALPHA	(0.2f)	// �e�̃��l

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CWeed::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\weed000.png",	// �t���e�N�X�`��
	"data\\TEXTURE\\weed001.png",	// �đ��e�N�X�`��
	"data\\TEXTURE\\weed002.png",	// �H���e�N�X�`��
	"data\\TEXTURE\\weed003.png",	// �~���e�N�X�`��
};

//************************************************************
//	�q�N���X [CWeed] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CWeed::CWeed(void) : CObject3D(CObject::LABEL_WEED, WEED_PRIO)
{
	// �����o�ϐ����N���A
	m_pShadow = NULL;		// �e�̏��
	m_type = TYPE_SPRING;	// ���
	m_fSinRot = 0.0f;		// �Ȃт�����
}

//============================================================
//	�f�X�g���N�^
//============================================================
CWeed::~CWeed()
{

}

//============================================================
//	����������
//============================================================
HRESULT CWeed::Init(void)
{
	// �����o�ϐ���������
	m_pShadow = NULL;		// �e�̏��
	m_type = TYPE_SPRING;	// ���
	m_fSinRot = (float)(rand() % 629 - 314) * 0.01f;	// �Ȃт�����

	// �e�̐���
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, SHADOW_SIZE, this, SHADOW_ALPHA, SHADOW_ALPHA);
	if (UNUSED(m_pShadow))
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CObject3D::Init()))
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
void CWeed::Uninit(void)
{
	// �e��j��
	m_pShadow->Uninit();

	// �I�u�W�F�N�g3D�̏I��
	CObject3D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CWeed::Update(void)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = GetPosition();	// �ʒu

	// TODO�F���ƃv���C���[�̓����蔻��
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



	// �Ȃт����������Z
	m_fSinRot += 0.2f;
	useful::NormalizeRot(m_fSinRot);	// �������K��

	// ���_�̂���ʂ�ݒ�
	SetGapPosition(0, D3DXVECTOR3(0.0f, 0.0f, sinf(m_fSinRot) * 15.0f - 25.0f));
	SetGapPosition(1, D3DXVECTOR3(0.0f, 0.0f, sinf(m_fSinRot) * 15.0f - 25.0f));



	// �ʒu�����߂�
	pos.y = CSceneGame::GetField()->GetPositionHeight(pos);	// ������n�ʂɐݒ�

	// �ʒu���X�V
	SetPosition(pos);

	// �e�̍X�V
	m_pShadow->Update();

	// �I�u�W�F�N�g3D�̍X�V
	CObject3D::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CWeed::Draw(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// ���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// ���e�X�g�̗L�� / �����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 160);				// ���e�X�g�̎Q�ƒl�ݒ�

	// �I�u�W�F�N�g3D�̕`��
	CObject3D::Draw();

	// ���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g�̗L�� / �����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// ���e�X�g�̎Q�ƒl�ݒ�
}

//============================================================
//	��������
//============================================================
CWeed *CWeed::Create
(
	const TYPE type,			// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR3& rSize	// �傫��
)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = rPos;	// ���W�ݒ�p
	int nTextureID;	// �e�N�X�`���C���f�b�N�X

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^
	CWeed *pWeed = NULL;	// �������p

	if (UNUSED(pWeed))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pWeed = new CWeed;	// ��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pWeed))
	{ // �m�ۂɐ������Ă���ꍇ

		// ���̏�����
		if (FAILED(pWeed->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pWeed;
			pWeed = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �e�N�X�`����o�^
		nTextureID = pTexture->Regist(mc_apTextureFile[type]);

		// �e�N�X�`��������
		pWeed->BindTexture(nTextureID);

		// ���_��ݒ�
		pWeed->SetOrigin(ORIGIN_DOWN);

		// �ʒu��ݒ�
		pos.y = CSceneGame::GetField()->GetPositionHeight(pos);	// ������n�ʂɐݒ�
		pWeed->SetPosition(pos);

		// ������ݒ�
		pWeed->SetRotation(rRot);

		// �傫����ݒ�
		pWeed->SetScaling(rSize);

		// �J�����O��ݒ�
		pWeed->SetCulling(D3DCULL_NONE);

		// ���C�e�B���O��ݒ�
		pWeed->SetLighting(false);

		// Z�e�X�g��ݒ�
		pWeed->SetFunc(D3DCMP_LESSEQUAL);

		// Z�o�b�t�@�̎g�p�󋵂�ݒ�
		pWeed->SetZEnable(true);

		// �m�ۂ����A�h���X��Ԃ�
		return pWeed;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�����_����������
//============================================================
void CWeed::RandomSpawn
(
	const int nNum,				// ������
	const TYPE type,			// ���
	const D3DXVECTOR3& rSize	// �傫��
)
{
	// �ϐ���錾
	D3DXVECTOR3 posSet;	// �ʒu�ݒ�p
	D3DXVECTOR3 rotSet;	// �����ݒ�p

	D3DXVECTOR3 posTarget = CSceneGame::GetTarget()->GetPosition();		// �^�[�Q�b�g�ʒu
	int nLimit = (int)CSceneGame::GetStage()->GetStageLimit().fRadius;	// �X�e�[�W�͈�

	if (USED(CSceneGame::GetTarget()))
	{ // �^�[�Q�b�g���g�p����Ă���ꍇ

		for (int nCntGrow = 0; nCntGrow < nNum; nCntGrow++)
		{ // ���������J��Ԃ�

			// �����ʒu��ݒ�
			posSet.x = (float)(rand() % (nLimit * 2) - nLimit + 1);
			posSet.y = 0.0f;
			posSet.z = (float)(rand() % (nLimit * 2) - nLimit + 1);

			// �����ʒu��␳
			collision::CirclePillar(posSet, posTarget, rSize.x, CSceneGame::GetStage()->GetStageBarrier().fRadius);	// �^�[�Q�b�g�����̐����h�~
			CSceneGame::GetStage()->LimitPosition(posSet, rSize.x);	// �X�e�[�W�͈͊O�̐����h�~

			// ����������ݒ�
			rotSet = D3DXVECTOR3(0.0f, atan2f(posSet.x - posTarget.x, posSet.z - posTarget.z), 0.0f);

			// ���I�u�W�F�N�g�̐���
			CWeed::Create(type, posSet, rotSet, rSize);
		}
	}
}

//============================================================
//	�G�߂̐ݒ菈��
//============================================================
void CWeed::SetSeason(const CWaveManager::SEASON season)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		// �|�C���^��錾
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// �擪�I�u�W�F�N�g

		if (USED(pObjectTop))
		{ // �擪�����݂���ꍇ

			// �|�C���^��錾
			CObject *pObjCheck = pObjectTop;	// �I�u�W�F�N�g�m�F�p

			while (USED(pObjCheck))
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_WEED)
				{ // �I�u�W�F�N�g���x�������ł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// �����̋G�߂̃e�N�X�`����o�^�E����
				pObjCheck->BindTexture(pTexture->Regist(mc_apTextureFile[season]));

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}
}

//============================================================
//	�v���C���[�Ƃ̓����蔻��
//============================================================
bool CWeed::CollisionPlayer(void)
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
