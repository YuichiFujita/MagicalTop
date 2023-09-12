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
#define FLOWER_PRIO	(2)	// �}�i�t�����[�̗D�揇��

#define SHADOW_SIZE	(D3DXVECTOR3(80.0f, 0.0f, 80.0f))	// �e�̑傫��
#define SHADOW_ALPHA	(0.2f)	// �e�̃��l

#define NORMAL_CNT	(20)	// �_���[�W��Ԃ���ʏ��Ԃɖ߂�܂ł̃t���[����

#define PREC_PLUS_RADIUS	(80.0f)	// ���������̔��a���Z��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CFlower::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\flower000.png",	// �t�}�i�t�����[�e�N�X�`��
	"data\\TEXTURE\\flower001.png",	// �ă}�i�t�����[�e�N�X�`��
	"data\\TEXTURE\\flower002.png",	// �H�}�i�t�����[�e�N�X�`��
	"data\\TEXTURE\\flower003.png",	// �~�}�i�t�����[�e�N�X�`��
};
int CFlower::m_nNumAll = 0;	// �}�i�t�����[�̑���

//************************************************************
//	�q�N���X [CFlower] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CFlower::CFlower(void) : CObject3D(CObject::LABEL_FLOWER, FLOWER_PRIO)
{
	// �����o�ϐ����N���A
	m_pShadow = NULL;		// �e�̏��
	m_state = STATE_NORMAL;	// ���
	m_nLife = 0;			// �̗�
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

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
	m_pShadow = NULL;		// �e�̏��
	m_state = STATE_NORMAL;	// ���
	m_nLife = 0;			// �̗�
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

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
void CFlower::Uninit(void)
{
	// �e��j��
	m_pShadow->Uninit();

	// �I�u�W�F�N�g3D�̏I��
	CObject3D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CFlower::Update(void)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = GetPosition();	// �ʒu

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NORMAL:

		// �v���C���[�Ƃ̓����蔻��
		if (CollisionPlayer(pos))
		{ // �̗͂��Ȃ��Ȃ����ꍇ

			// �I�u�W�F�N�g�̏I��
			Uninit();

			// �֐��𔲂���
			return;
		}

		break;

	case STATE_DAMAGE:

		if (m_nCounterState < NORMAL_CNT)
		{ // �J�E���^�[�����l��菬�����ꍇ

			// �ϐ���錾
			D3DXVECTOR3 size = GetScaling();	// �}�i�t�����[�傫��

			// �J�E���^�[�����Z
			m_nCounterState++;

			// �傫����ݒ�
			SetScaling(D3DXVECTOR3(size.x, (30.0f / NORMAL_CNT) * m_nCounterState + 20.0f, size.z));	// TODO�F�萔
		}
		else
		{ // �J�E���^�[�����l�ȏ�̏ꍇ

			// �J�E���^�[��������
			m_nCounterState = 0;

			// ��Ԃ�ݒ�
			m_state = STATE_NORMAL;	// �ʏ���
		}

		break;

	default:
		assert(false);
		break;
	}

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
void CFlower::Draw(void)
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
CFlower *CFlower::Create
(
	const TYPE type,			// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
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

		// ������ݒ�
		pFlower->SetRotation(rRot);

		// �傫����ݒ�
		pFlower->SetScaling(rSize);

		// �̗͂�ݒ�
		pFlower->SetLife(nLife);

		// �J�����O��ݒ�
		pFlower->SetCulling(D3DCULL_NONE);

		// ���C�e�B���O��ݒ�
		pFlower->SetLighting(false);

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
			collision::CirclePillar(posSet, posTarget, rSize.x, CSceneGame::GetStage()->GetStageBarrier().fRadius + PREC_PLUS_RADIUS);	// �^�[�Q�b�g�����̐����h�~
			CSceneGame::GetStage()->LimitPosition(posSet, rSize.x);	// �X�e�[�W�͈͊O�̐����h�~

			// ����������ݒ�
			rotSet = D3DXVECTOR3(0.0f, atan2f(posSet.x - posTarget.x, posSet.z - posTarget.z), 0.0f);

			// �}�i�t�����[�I�u�W�F�N�g�̐���
			CFlower::Create(type, posSet, rotSet, rSize, nLife);
		}
	}
}

//============================================================
//	�G�߂̐ݒ菈��
//============================================================
void CFlower::SetSeason(const CWaveManager::SEASON season)
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

				if (pObjCheck->GetLabel() != CObject::LABEL_FLOWER)
				{ // �I�u�W�F�N�g���x�����}�i�t�����[�ł͂Ȃ��ꍇ

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
bool CFlower::CollisionPlayer(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	bool bDeath = false;	// ���S��

	// �|�C���^��錾
	CPlayer *pPlayer = CSceneGame::GetPlayer();	// �v���C���[���

	// �ϐ���錾
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();	// �v���C���[�ʒu
	D3DXVECTOR3 sizeFlower = GetScaling();	// �}�i�t�����[�傫��

	if (pPlayer->GetState() == CPlayer::STATE_NORMAL)
	{ // �v���C���[���ʏ��Ԃ̏ꍇ

		if (collision::Circle2D(rPos, posPlayer, sizeFlower.x * 0.5f, pPlayer->GetRadius()))
		{ // �v���C���[�ɓ������Ă����ꍇ

			if (pPlayer->GetMotionType() != CPlayer::MOTION_ACCELE)
			{ // �v���C���[���������ł͂Ȃ��ꍇ

				// �̗͂����Z
				m_nLife--;
			}
			else
			{ // �v���C���[���������̏ꍇ

				// �̗͂�0�ɂ���
				m_nLife = 0;
			}

			if (m_nLife > 0)
			{ // �̗͂��c���Ă����ꍇ

				// �ϐ���錾
				float fCol = (0.5f / (float)10) * m_nLife + 0.5f;	// �}�i�t�����[�F	// TODO�F�萔

				// �}�i�t�����[�̐F��ݒ�
				SetColor(D3DXCOLOR(fCol, fCol, fCol, 1.0f));

				// ��Ԃ�ݒ�
				m_state = STATE_DAMAGE;	// �_���[�W���
			}
			else
			{ // �̗͂��Ȃ��Ȃ����ꍇ

				// ���S��Ԃɂ���
				bDeath = true;
			}
		}
	}

	// ���S�󋵂�Ԃ�
	return bDeath;
}
