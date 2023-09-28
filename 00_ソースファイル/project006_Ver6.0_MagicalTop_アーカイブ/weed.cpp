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
#include "scene.h"
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
#define WEED_SETUP_TXT	"data\\TXT\\weed.txt"	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
#define WEED_PRIO	(2)	// ���̗D�揇��

#define SHADOW_SIZE	(D3DXVECTOR3(80.0f, 0.0f, 80.0f))	// �e�̑傫��
#define SHADOW_ALPHA	(0.2f)	// �e�̃��l

#define ADD_ROT		(0.2f)	// �Ȃт������̉��Z��
#define RANGE		(15.0f)	// �Ȃт��ʂ͈̔�
#define SUB_RANGE	(25.0f)	// �Ȃт������̌��Z��

#define PREC_PLUS_RADIUS	(80.0f)		// ���������̔��a���Z��
#define PREC_IN_RADIUS		(3000.0f)	// ���������̍ő唼�a

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
CWeed::StatusInfo CWeed::m_aStatusInfo[TYPE_MAX] = {};	// �X�e�[�^�X���

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

	// �Ȃт����������Z
	m_fSinRot += ADD_ROT;
	useful::NormalizeRot(m_fSinRot);	// �������K��

	// ���_�̂���ʂ�ݒ�
	SetGapPosition(0, D3DXVECTOR3(0.0f, 0.0f, sinf(m_fSinRot) * RANGE - SUB_RANGE));
	SetGapPosition(1, D3DXVECTOR3(0.0f, 0.0f, sinf(m_fSinRot) * RANGE - SUB_RANGE));

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

	// �|�C���^��錾
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

		// ���_��ݒ�
		pWeed->SetOrigin(ORIGIN_DOWN);

		// �ʒu��ݒ�
		pos.y = CScene::GetField()->GetPositionHeight(pos);	// ������n�ʂɐݒ�
		pWeed->SetPosition(pos);

		// ������ݒ�
		pWeed->SetRotation(rRot);

		// �傫����ݒ�
		pWeed->SetScaling(rSize);

		// ��ނ�ݒ�
		pWeed->SetType((int)type);

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
	const int nNum,	// ������
	const TYPE type	// ���
)
{
	// �ϐ���錾
	D3DXVECTOR3 posSet;	// �ʒu�ݒ�p
	D3DXVECTOR3 rotSet;	// �����ݒ�p
	D3DXVECTOR3 posCenter = CScene::GetStage()->GetStageLimit().center;	// ���S���W

	for (int nCntGrow = 0; nCntGrow < nNum; nCntGrow++)
	{ // ���������J��Ԃ�

		// �����ʒu��ݒ�
		posSet.x = (float)(rand() % ((int)PREC_IN_RADIUS * 2) - (int)PREC_IN_RADIUS + 1);
		posSet.y = 0.0f;
		posSet.z = (float)(rand() % ((int)PREC_IN_RADIUS * 2) - (int)PREC_IN_RADIUS + 1);

		// �����ʒu��␳
		collision::CirclePillar(posSet, posCenter, m_aStatusInfo[type].size.x, CScene::GetStage()->GetStageBarrier().fRadius + PREC_PLUS_RADIUS);	// �^�[�Q�b�g�����̐����h�~
		collision::InCirclePillar(posSet, posCenter, m_aStatusInfo[type].size.x, PREC_IN_RADIUS);	// �͈͊O�̐����h�~

		// ����������ݒ�
		rotSet = D3DXVECTOR3(0.0f, atan2f(posSet.x - posCenter.x, posSet.z - posCenter.z), 0.0f);

		// ���I�u�W�F�N�g�̐���
		CWeed::Create(type, posSet, rotSet, m_aStatusInfo[type].size);
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

				// �����̋G�߂�ݒ�
				pObjCheck->SetType((int)season);

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}
}

//============================================================
//	��ނ̐ݒ菈��
//============================================================
void CWeed::SetType(const int nType)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����̎�ނ�ݒ�
	m_type = (TYPE)nType;
	
	// �e�N�X�`����o�^�E����
	BindTexture(pTexture->Regist(mc_apTextureFile[m_type]));

	// �傫����ݒ�
	SetScaling(m_aStatusInfo[m_type].size);
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CWeed::LoadSetup(void)
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
	pFile = fopen(WEED_SETUP_TXT, "r");

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

					if (strcmp(&aString[0], "WEEDSET") == 0)
					{ // �ǂݍ��񂾕����� WEEDSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_WEEDSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "TYPE") == 0)
							{ // �ǂݍ��񂾕����� TYPE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nType);		// ��ނ�ǂݍ���
							}
							else if (strcmp(&aString[0], "SIZE") == 0)
							{ // �ǂݍ��񂾕����� SIZE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].size.x);	// �傫��X��ǂݍ���
								fscanf(pFile, "%f", &m_aStatusInfo[nType].size.y);	// �傫��Y��ǂݍ���
								fscanf(pFile, "%f", &m_aStatusInfo[nType].size.z);	// �傫��Z��ǂݍ���
							}
						} while (strcmp(&aString[0], "END_WEEDSET") != 0);	// �ǂݍ��񂾕����� END_WEEDSET �ł͂Ȃ��ꍇ���[�v
					}
				} while (strcmp(&aString[0], "END_STATUSSET") != 0);		// �ǂݍ��񂾕����� END_STATUSSET �ł͂Ȃ��ꍇ���[�v
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "���Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
