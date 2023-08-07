//============================================================
//
//	���@���� [magic.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "magic.h"
#include "manager.h"
#include "sceneGame.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"
#include "effect3D.h"
#include "enemy.h"
#include "field.h"
#include "objectTornado.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAGIC_SETUP_TXT	"data\\TXT\\magic.txt"	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CMagic::StatusInfo CMagic::m_aStatusInfo[CMagic::TYPE_MAX] = {};	// �X�e�[�^�X���

//************************************************************
//	�q�N���X [CMagic] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CMagic::CMagic(const TYPE type) : CObject(CObject::LABEL_MAGIC), m_status(m_aStatusInfo[type])
{
	// �����o�ϐ����N���A
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ���[���h�}�g���b�N�X
	m_pos		= VEC3_ZERO;	// ���݈ʒu
	m_movePos	= VEC3_ZERO;	// �ʒu�ړ���
	m_rot		= VEC3_ZERO;	// ����
	m_moveRot	= VEC3_ZERO;	// �����ύX��
	m_nLife		= 0;			// ����
}

//============================================================
//	�f�X�g���N�^
//============================================================
CMagic::~CMagic()
{

}

//============================================================
//	����������
//============================================================
HRESULT CMagic::Init(void)
{
	// �����o�ϐ���������
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ���[���h�}�g���b�N�X
	m_pos		= VEC3_ZERO;	// ���݈ʒu
	m_movePos	= VEC3_ZERO;	// �ʒu�ړ���
	m_rot		= VEC3_ZERO;	// ����
	m_moveRot	= VEC3_ZERO;	// �����ύX��
	m_nLife		= 0;			// ����

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CMagic::Uninit(void)
{
	// ���@��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CMagic::Update(void)
{
	if (m_nLife > 0)
	{ // �������c���Ă���ꍇ

		// �J�E���^�[�����Z
		m_nLife--;
	}
	else
	{ // �����������ꍇ

		// �I�u�W�F�N�g�̏I��
		Uninit();

		// �֐��𔲂���
		return;
	}

	// �ړ��ʂ����Z
	m_pos += m_movePos;

	// �G�Ƃ̓����蔻��
	if (CollisionEnemy())
	{ // �G�Ɠ������Ă����ꍇ

		// �I�u�W�F�N�g�̏I��
		Uninit();

		// �֐��𔲂���
		return;
	}
}

//============================================================
//	�`�揈��
//============================================================
void CMagic::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//============================================================
//	��������
//============================================================
CMagic *CMagic::Create
(
	const TYPE type,			// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR3& rVec		// �ړ�����
)
{
	// �|�C���^��錾
	CMagic *pMagic = NULL;	// ���@�����p

	if (UNUSED(pMagic))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		switch (type)
		{ // ��ނ��Ƃ̏���
		case TYPE_LV0_NORMAL:	// �������@

			// �������@�𐶐�
			pMagic = new CNormalMagic(type);

			break;

		case TYPE_LV2_WIND:		// LV2�����@

			// LV2�����@�𐶐�
			pMagic = new CLv2WindMagic(type);

			break;

		default:	// ��O����
			assert(false);
			break;
		}
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pMagic))
	{ // �g�p����Ă���ꍇ
		
		// ���@�̏�����
		if (FAILED(pMagic->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pMagic;
			pMagic = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pMagic->SetPosition(rPos);

		// ������ݒ�
		pMagic->SetRotation(rRot);

		// �ړ��ʂ�ݒ�
		pMagic->SetMove(rVec, pMagic->GetStatusInfo().fMove);

		// �����̐ݒ�
		pMagic->SetLife(pMagic->GetStatusInfo().nLife);

		// �m�ۂ����A�h���X��Ԃ�
		return pMagic;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�X�e�[�^�X���擾����
//============================================================
CMagic::StatusInfo CMagic::GetStatusInfo(const TYPE type)
{
	if (type < TYPE_MAX)
	{ // �����̃C���f�b�N�X���g�p�\�ȏꍇ

		// �����̃X�e�[�^�X����Ԃ�
		return m_aStatusInfo[type];
	}
	else { assert(false); return m_aStatusInfo[0]; }	// �g�p�s��
}

//============================================================
//	�ړ��ʂ̐ݒ菈��
//============================================================
void CMagic::SetMove(D3DXVECTOR3 vec, const float fMove)
{
	// �x�N�g���̐��K��
	D3DXVec3Normalize(&vec, &vec);

	// �ړ��ʂ�ݒ�
	m_movePos = vec * fMove;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CMagic::SetLife(const int nLife)
{
	// �����̎�������
	m_nLife = nLife;
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CMagic::SetPosition(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CMagic::SetRotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_rot = rRot;

	// �����̐��K��
	useful::NormalizeRot(m_rot.x);
	useful::NormalizeRot(m_rot.y);
	useful::NormalizeRot(m_rot.z);
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CMagic::GetPosition(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CMagic::GetRotation(void) const
{
	// ������Ԃ�
	return m_rot;
}

//============================================================
//	�X�e�[�^�X���擾����
//============================================================
CMagic::StatusInfo CMagic::GetStatusInfo(void) const
{
	// �X�e�[�^�X����Ԃ�
	return m_status;
}

//============================================================
//	���@����
//============================================================
bool CMagic::Collision(CObject *pObject)
{
	// �ϐ���錾
	bool bHit = false;	// �����������̔���

	// �G�Ƃ̓����蔻��
	bHit = collision::Circle3D
	( // ����
		m_pos,					// ����ʒu
		pObject->GetPosition(),	// ����ڕW�ʒu
		m_status.fRadius,		// ���蔼�a
		pObject->GetRadius()	// ����ڕW���a
	);

	// �����������̔����Ԃ�
	return bHit;
}

//============================================================
//	�G�Ƃ̓����蔻��
//============================================================
bool CMagic::CollisionEnemy(void)
{
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

				if (pObjCheck->GetLabel() != CObject::LABEL_ENEMY)
				{ // �I�u�W�F�N�g���x�����G�ł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				if (pObjCheck->GetState() == CEnemy::STATE_SPAWN)
				{ // �G�̏�Ԃ��X�|�[����Ԃ̏ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// ���@����
				if (Collision(pObjCheck))
				{ // ���@�ɓ������Ă����ꍇ

					// �G�̃q�b�g����
					pObjCheck->Hit(m_status.nDamage);

					// �������������Ԃ�
					return true;
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}

	// �������Ă��Ȃ������Ԃ�
	return false;
}

//************************************************************
//	�q�N���X [CNormalMagic] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CNormalMagic::CNormalMagic(const TYPE type) : CMagic(type)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CNormalMagic::~CNormalMagic()
{

}

//============================================================
//	����������
//============================================================
HRESULT CNormalMagic::Init(void)
{
	// ���@�̏�����
	if (FAILED(CMagic::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CNormalMagic::Uninit(void)
{
	// ���@�̏I��
	CMagic::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CNormalMagic::Update(void)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = GetPosition();	// �ʒu

	if (pos.y <= CSceneGame::GetField()->GetPositionHeight(pos))
	{ // �n�ʂɓ������Ă���ꍇ

		// �I�u�W�F�N�g�̏I��
		Uninit();

		// �֐��𔲂���
		return;
	}

	// �G�t�F�N�g�𐶐�
	CEffect3D::Create(CEffect3D::TYPE_NORMAL, pos, VEC3_ZERO, VEC3_ZERO, D3DXCOLOR(0.6f, 0.65f, 0.0f, 1.0f), 38, 56.0f, 1.8f, 0.06f);
	CEffect3D::Create(CEffect3D::TYPE_NORMAL, pos, VEC3_ZERO, VEC3_ZERO, XCOL_WHITE, 24, 50.0f, 2.3f, 0.06f);

	// ���@�̍X�V
	CMagic::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CNormalMagic::Draw(void)
{
	// ���@�̕`��
	CMagic::Draw();
}

//************************************************************
//	�q�N���X [CLv2WindMagic] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CLv2WindMagic::CLv2WindMagic(const TYPE type) : CMagic(type)
{
	// �����o�ϐ����N���A
	m_pTornado = NULL;	// �����I�u�W�F�N�g
}

//============================================================
//	�f�X�g���N�^
//============================================================
CLv2WindMagic::~CLv2WindMagic()
{

}

//============================================================
//	����������
//============================================================
HRESULT CLv2WindMagic::Init(void)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetTexture();	// �e�N�X�`��

	// �����o�ϐ���������
	m_pTornado = NULL;	// �����I�u�W�F�N�g

	// ���@�̏�����
	if (FAILED(CMagic::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �����̐���
	m_pTornado = CObjectTornado::Create
	( // ����
		VEC3_ZERO,	// �ʒu
		VEC3_ZERO,	// ��������
		XCOL_WHITE,	// �F
		NULL,		// �e�̃}�g���b�N�X
		2,			// �Q�̎���
		32,			// �Q�̕�����
		0.3f,		// �����̕ύX��
		15.0f,		// �|���S���̑���
		2.0f,		// �|���S���O���� y���W���Z��
		15.0f,		// �������̉������
		1.0f,		// �������̓����x
		1.0f,		// ������̉��Z��
		1.5f,		// �c����̉��Z��
		0.0f,		// �����x�̌��Z��
		0.0f,		// ������̐�����
		0.0f,		// �c����̐�����
		0.0f		// �����x�̐�����
	);

	// �e�N�X�`���������E�ݒ�
	m_pTornado->BindTexture(pTexture->Regist("data\\TEXTURE\\tornado000.png"));

	// ���x����ݒ�
	m_pTornado->SetLabel(CObject::LABEL_MAGIC);

	// �D�揇�ʂ�ݒ�
	m_pTornado->SetPriority(5);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CLv2WindMagic::Uninit(void)
{
	// �����̏I��
	m_pTornado->SetGrow
	( // ����
		0.35f,	// �����̕ύX��
		0.005f,	// ������̐�����
		0.08f,	// �c����̐�����
		0.06f	// �����x�̐�����
	);

	// ���@�̏I��
	CMagic::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CLv2WindMagic::Update(void)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = GetPosition();	// �ʒu

	// �ʒu��n�ʂɕ␳
	pos.y = CSceneGame::GetField()->GetPositionHeight(pos);

	// �g���l�[�h�̈ʒu��ݒ�
	m_pTornado->SetPosition(pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f));

	// �ʒu��ݒ�
	SetPosition(pos);

	// �G�t�F�N�g�𐶐�
	CEffect3D::Create(CEffect3D::TYPE_NORMAL, pos, VEC3_ZERO, VEC3_ZERO, D3DXCOLOR(0.0f, 0.65f, 0.0f, 0.4f), 38, 56.0f, 3.8f, 0.06f);

	// �����̍X�V
	m_pTornado->Update();

	// ���@�̍X�V
	CMagic::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CLv2WindMagic::Draw(void)
{
	// ���@�̕`��
	CMagic::Draw();
}

//============================================================
//	���@����
//============================================================
bool CLv2WindMagic::Collision(CObject *pObject)
{
	// �ϐ���錾
	bool bHit = false;	// �����������̔���

	// �G�Ƃ̓����蔻��
	bHit = collision::CirclePillar
	( // ����
		GetPosition(),				// ����ʒu
		pObject->GetPosition(),		// ����ڕW�ʒu
		GetStatusInfo().fRadius,	// ���蔼�a
		pObject->GetRadius()		// ����ڕW���a
	);

	// �����������̔����Ԃ�
	return bHit;
}

//************************************************************
//	�q�N���X [CMagic] �̃Z�b�g�A�b�v�֐�
//************************************************************
//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CMagic::LoadSetup(void)
{
	// �ϐ���錾
	int nType	= 0;	// ��ނ̑���p
	int nHoming	= 0;	// �z�[�~���O��ON/OFF�̕ϊ��p
	int nEnd	= 0;	// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �ÓI�����o�ϐ��̏����N���A
	memset(&m_aStatusInfo[0], 0, sizeof(m_aStatusInfo));	// �X�e�[�^�X���

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(MAGIC_SETUP_TXT, "r");

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

					if (strcmp(&aString[0], "MAGICSET") == 0)
					{ // �ǂݍ��񂾕����� MAGICSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_MAGICSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "TYPE") == 0)
							{ // �ǂݍ��񂾕����� TYPE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nType);		// ��ނ�ǂݍ���
							}
							else if (strcmp(&aString[0], "LOCK") == 0)
							{ // �ǂݍ��񂾕����� LOCK �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nLock);	// ���b�N�I������ǂݍ���
							}
							else if (strcmp(&aString[0], "LIFE") == 0)
							{ // �ǂݍ��񂾕����� LIFE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nLife);	// ������ǂݍ���
							}
							else if (strcmp(&aString[0], "DAMAGE") == 0)
							{ // �ǂݍ��񂾕����� DAMAGE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nDamage);	// �U���͂�ǂݍ���
							}
							else if (strcmp(&aString[0], "RADIUS") == 0)
							{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fRadius);	// ���a��ǂݍ���
							}
							else if (strcmp(&aString[0], "HEIGHT") == 0)
							{ // �ǂݍ��񂾕����� HEIGHT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fHeight);	// �c����ǂݍ���
							}
							else if (strcmp(&aString[0], "MOVE") == 0)
							{ // �ǂݍ��񂾕����� MOVE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fMove);	// �ړ��ʂ�ǂݍ���
							}
							else if (strcmp(&aString[0], "VIEW_RADIUS") == 0)
							{ // �ǂݍ��񂾕����� VIEW_RADIUS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].fViewRadius);	// ���E�͈͂�ǂݍ���
							}
							else if (strcmp(&aString[0], "SHOT_PARTS") == 0)
							{ // �ǂݍ��񂾕����� SHOT_PARTS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nShotParts);	// ���˃p�[�c��ǂݍ���
							}
							else if (strcmp(&aString[0], "SHOT_POS") == 0)
							{ // �ǂݍ��񂾕����� SHOT_POS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_aStatusInfo[nType].shotPos.x);	// ���ˈʒuX��ǂݍ���
								fscanf(pFile, "%f", &m_aStatusInfo[nType].shotPos.y);	// ���ˈʒuY��ǂݍ���
								fscanf(pFile, "%f", &m_aStatusInfo[nType].shotPos.z);	// ���ˈʒuZ��ǂݍ���
							}
						} while (strcmp(&aString[0], "END_MAGICSET") != 0);	// �ǂݍ��񂾕����� END_MAGICSET �ł͂Ȃ��ꍇ���[�v
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
		MessageBox(NULL, "���@�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
