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
#include "scene.h"
#include "sceneGame.h"
#include "renderer.h"
#include "sound.h"
#include "texture.h"

#include "collision.h"
#include "player.h"
#include "target.h"
#include "shadow.h"
#include "field.h"
#include "stage.h"
#include "particle3D.h"

//************************************************************
//	�}�N����`
//************************************************************
#define FLOWER_SETUP_TXT	"data\\TXT\\flower.txt"	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

#define FLOWER_PRIO	(2)	// �}�i�t�����[�̗D�揇��

#define SHADOW_SIZE	(D3DXVECTOR3(80.0f, 0.0f, 80.0f))	// �e�̑傫��
#define SHADOW_ALPHA	(0.2f)	// �e�̃��l

#define NORMAL_CNT	(20)	// �_���[�W��Ԃ���ʏ��Ԃɖ߂�܂ł̃t���[����
#define MIN_COL		(0.5f)	// �F�̍ŏ��l

#define REV_MIN_SIZE		(0.6f)	// �Œ���̑傫���̕␳�W��
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
CFlower::StatusInfo CFlower::m_aStatusInfo[TYPE_MAX] = {};	// �X�e�[�^�X���
int CFlower::m_nNumAll = 0;	// �}�i�t�����[�̑���

//************************************************************
//	�q�N���X [CFlower] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CFlower::CFlower() : CObject3D(CObject::LABEL_FLOWER, FLOWER_PRIO)
{
	// �����o�ϐ����N���A
	m_pShadow = NULL;		// �e�̏��
	m_state = STATE_NORMAL;	// ���
	m_type = TYPE_SPRING;	// ���
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
	m_type = TYPE_SPRING;	// ���
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

			// �J�E���^�[�����Z
			m_nCounterState++;

			// �ϐ���錾
			D3DXVECTOR3 size = GetScaling();	// �}�i�t�����[�傫��
			float fSizeY = ((m_aStatusInfo[m_type].size.y - (m_aStatusInfo[m_type].size.y * REV_MIN_SIZE)) / NORMAL_CNT) * m_nCounterState + (m_aStatusInfo[m_type].size.y * REV_MIN_SIZE);	// �c�̑傫��

			// �傫����ݒ�
			SetScaling(D3DXVECTOR3(size.x, fSizeY, size.z));
		}
		else
		{ // �J�E���^�[�����l�ȏ�̏ꍇ

			// �J�E���^�[��������
			m_nCounterState = 0;

			// �傫����������
			SetScaling(m_aStatusInfo[m_type].size);

			// ��Ԃ�ݒ�
			m_state = STATE_NORMAL;	// �ʏ���
		}

		break;

	default:
		assert(false);
		break;
	}

	// �ʒu�����߂�
	pos.y = CScene::GetField()->GetPositionHeight(pos);	// ������n�ʂɐݒ�

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
	D3DXVECTOR3 pos = rPos;		// ���W�ݒ�p

	// �|�C���^��錾
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

		// ���_��ݒ�
		pFlower->SetOrigin(ORIGIN_DOWN);

		// �ʒu��ݒ�
		pos.y = CScene::GetField()->GetPositionHeight(pos);	// ������n�ʂɐݒ�
		pFlower->SetPosition(pos);

		// ������ݒ�
		pFlower->SetRotation(rRot);

		// �傫����ݒ�
		pFlower->SetScaling(rSize);

		// ��ނ�ݒ�
		pFlower->SetType((int)type);

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
	const int nNum,	// ������
	const TYPE type	// ���
)
{
	// �ϐ���錾
	D3DXVECTOR3 posSet;	// �ʒu�ݒ�p
	D3DXVECTOR3 rotSet;	// �����ݒ�p
	D3DXVECTOR3 posTarget = CScene::GetStage()->GetStageLimit().center;	// ���S���W
	int nLimit = (int)CScene::GetStage()->GetStageLimit().fRadius;		// �X�e�[�W�͈�

	for (int nCntGrow = 0; nCntGrow < nNum; nCntGrow++)
	{ // ���������J��Ԃ�

		// �����ʒu��ݒ�
		posSet.x = (float)(rand() % (nLimit * 2) - nLimit + 1);
		posSet.y = 0.0f;
		posSet.z = (float)(rand() % (nLimit * 2) - nLimit + 1);

		// �����ʒu��␳
		collision::CirclePillar(posSet, posTarget, m_aStatusInfo[type].size.x, CScene::GetStage()->GetStageBarrier().fRadius + PREC_PLUS_RADIUS);	// �^�[�Q�b�g�����̐����h�~
		CScene::GetStage()->LimitPosition(posSet, m_aStatusInfo[type].size.x);	// �X�e�[�W�͈͊O�̐����h�~

		// ����������ݒ�
		rotSet = D3DXVECTOR3(0.0f, atan2f(posSet.x - posTarget.x, posSet.z - posTarget.z), 0.0f);

		// �}�i�t�����[�I�u�W�F�N�g�̐���
		CFlower::Create(type, posSet, rotSet, m_aStatusInfo[type].size, m_aStatusInfo[type].nLife);
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

				// �����̋G�߂�ݒ�
				pObjCheck->SetType((int)season);

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
//	��ނ̐ݒ菈��
//============================================================
void CFlower::SetType(const int nType)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����̎�ނ�ݒ�
	m_type = (TYPE)nType;
	
	// �e�N�X�`����o�^�E����
	BindTexture(pTexture->Regist(mc_apTextureFile[m_type]));

	// �傫����ݒ�
	SetScaling(m_aStatusInfo[m_type].size);

	// �̗͂�ݒ�
	SetLife(m_aStatusInfo[m_type].nLife);
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

	if (USED(CScene::GetPlayer()))
	{ // �v���C���[���g�p����Ă���ꍇ

		// �|�C���^��錾
		CPlayer *pPlayer = CScene::GetPlayer();	// �v���C���[���

		// �ϐ���錾
		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();	// �v���C���[�ʒu
		D3DXVECTOR3 sizeFlower = GetScaling();	// �}�i�t�����[�傫��

		if (pPlayer->GetState() == CPlayer::STATE_NORMAL)
		{ // �v���C���[���ʏ��Ԃ̏ꍇ

			if (collision::Circle2D(rPos, posPlayer, sizeFlower.x * 0.5f, pPlayer->GetRadius()))
			{ // �v���C���[�ɓ������Ă����ꍇ

				// �ϐ���錾
				float fCol = 0.0f;	// �}�i�t�����[�F

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

				// �}�i�t�����[�̐F��ݒ�
				fCol = (MIN_COL / (float)m_aStatusInfo[m_type].nLife) * m_nLife + fabsf(MIN_COL - 1.0f);

				if (m_nLife > 0)
				{ // �̗͂��c���Ă����ꍇ

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

				// �p�[�e�B�N��3D�I�u�W�F�N�g�𐶐�
				CParticle3D::Create(CParticle3D::TYPE_STOMP_PLANT, GetPosition(), D3DXCOLOR(fCol, fCol, fCol, 1.0f));

				// �T�E���h�̍Đ�
				CManager::GetSound()->Play(CSound::LABEL_SE_STOMP);	// �A�����݂���
			}
		}
	}

	// ���S�󋵂�Ԃ�
	return bDeath;
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CFlower::LoadSetup(void)
{
	// �ϐ���錾
	int nType	= 0;	// ��ނ̑���p
	int nEnd	= 0;	// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �ÓI�����o�ϐ��̏����N���A
	memset(&m_aStatusInfo[0], 0, sizeof(m_aStatusInfo));	// �X�e�[�^�X���

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(FLOWER_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			// �X�e�[�^�X�̐ݒ�
			if (strcmp(&aString[0], "STATUSSET") == 0)
			{ // �ǂݍ��񂾕����� STATUSSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_STATUSSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "FLOWERSET") == 0)
					{ // �ǂݍ��񂾕����� FLOWERSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_FLOWERSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "TYPE") == 0)
							{ // �ǂݍ��񂾕����� TYPE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nType);		// ��ނ�ǂݍ���
							}
							else if (strcmp(&aString[0], "LIFE") == 0)
							{ // �ǂݍ��񂾕����� LIFE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nLife);	// �̗͂�ǂݍ���
							}
							else if (strcmp(&aString[0], "SIZE") == 0)
							{ // �ǂݍ��񂾕����� SIZE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].size.x);	// �傫��X��ǂݍ���
								fscanf(pFile, "%f", &m_aStatusInfo[nType].size.y);	// �傫��Y��ǂݍ���
								fscanf(pFile, "%f", &m_aStatusInfo[nType].size.z);	// �傫��Z��ǂݍ���
							}
						} while (strcmp(&aString[0], "END_FLOWERSET") != 0);	// �ǂݍ��񂾕����� END_FLOWERSET �ł͂Ȃ��ꍇ���[�v
					}
				} while (strcmp(&aString[0], "END_STATUSSET") != 0);			// �ǂݍ��񂾕����� END_STATUSSET �ł͂Ȃ��ꍇ���[�v
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�}�i�t�����[�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
