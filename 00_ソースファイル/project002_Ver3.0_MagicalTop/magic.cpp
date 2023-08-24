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
#include "target.h"
#include "enemy.h"
#include "field.h"
#include "stage.h"
#include "bubble.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAGIC_SETUP_TXT	"data\\TXT\\magic.txt"	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

#define PLUS_HIT_RADIUS	(10.0f)	// �����蔻��̊g���l
#define MOVE_INHALE		(10.0f)	// �z�����܂ꎞ�̖��@�̈ړ���
#define MOVE_DELETE		(3.0f)	// �������̖��@�̈ړ���

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CMagic::StatusInfo CMagic::m_statusInfo = {};	// �X�e�[�^�X���

//************************************************************
//	�q�N���X [CMagic] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CMagic::CMagic() : CObject(CObject::LABEL_MAGIC)
{
	// �����o�ϐ����N���A
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ���[���h�}�g���b�N�X
	m_pBubble	= NULL;			// �o�u�����
	m_pos		= VEC3_ZERO;	// ���݈ʒu
	m_movePos	= VEC3_ZERO;	// �ʒu�ړ���
	m_rot		= VEC3_ZERO;	// ����
	m_moveRot	= VEC3_ZERO;	// �����ύX��
	m_state		= STATE_NORMAL;	// ���
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
	m_pBubble	= NULL;			// �o�u�����
	m_pos		= VEC3_ZERO;	// ���݈ʒu
	m_movePos	= VEC3_ZERO;	// �ʒu�ړ���
	m_rot		= VEC3_ZERO;	// ����
	m_moveRot	= VEC3_ZERO;	// �����ύX��
	m_state		= STATE_NORMAL;	// ���

	// �o�u�����̐���
	m_pBubble = CBubble::Create(this, m_statusInfo.nLife, VEC3_ALL(m_statusInfo.bubbleRadius), VEC3_ZERO, 0.0f);
	if (UNUSED(m_pBubble))
	{ // ��g�p���̏ꍇ

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
void CMagic::Uninit(void)
{
	// �o�u���̏I��
	m_pBubble->Uninit();

	// ���@��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CMagic::Update(void)
{
	// �ϐ���錾
	D3DXVECTOR3 vecTarg;

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NORMAL:	// �ʏ���

		// �ړ��ʂ����Z
		m_pos += m_movePos;

		// �ʒu�ɕ��������Z
		m_pos += CSceneGame::GetStage()->GetVecWind();

		// �o�u�����x�������Z
		m_pBubble->AddLevel(1);

		if (m_pBubble->GetLevel() >= m_statusInfo.nLife)
		{ // �����������ꍇ

			// ��Ԃ�ݒ�
			m_state = STATE_INHALE;	// �z�����܂���
		}

		break;

	case STATE_INHALE:	// �z�����܂���

		// �^�[�Q�b�g�����̃x�N�g�����v�Z
		vecTarg = CSceneGame::GetTarget()->GetPosition() - m_pos;
		vecTarg.y = 0.0f;						// �x�N�g���̏c�����𖳎�
		D3DXVec3Normalize(&vecTarg, &vecTarg);	// �x�N�g�����K��
		
		// �ړ��ʂ�ݒ�
		m_movePos = vecTarg * MOVE_INHALE;

		// �x�N�g����90�x��]
		vecTarg = D3DXVECTOR3(-vecTarg.z, 0.0f, vecTarg.x);

		// �ړ��ʂ����Z
		m_movePos += vecTarg * MOVE_INHALE;

		// �ړ��ʂ����Z
		m_pos += m_movePos;

		break;

	case STATE_DELETE:	// �������

		// �^�[�Q�b�g�����̃x�N�g�����v�Z
		vecTarg = CSceneGame::GetTarget()->GetPosition() - m_pos;
		vecTarg.y = 0.0f;						// �x�N�g���̏c�����𖳎�
		D3DXVec3Normalize(&vecTarg, &vecTarg);	// �x�N�g�����K��
		
		// �ړ��ʂ�ݒ�
		m_movePos = vecTarg * MOVE_DELETE;

		// �x�N�g����90�x��]
		vecTarg = D3DXVECTOR3(-vecTarg.z, 0.0f, vecTarg.x);

		// �ړ��ʂ����Z
		m_movePos += vecTarg * MOVE_DELETE;

		// �ړ��ʂ����Z
		m_pos += m_movePos;

		// �o�u�����x�������Z
		m_pBubble->AddLevel(-1);

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	if (m_state != STATE_DELETE)
	{ // ������Ԃł͂Ȃ��ꍇ

		// �o���A�Ƃ̓����蔻��
		if (CSceneGame::GetStage()->CollisionBarrier(m_pos, m_pBubble->GetRadius()))
		{ // �������Ă����ꍇ

			// ��Ԃ�ݒ�
			m_state = STATE_DELETE;	// �������
		}
	}

	// �c�ʒu��␳
	CSceneGame::GetField()->LandPosition(m_pos, VEC3_ZERO);

	// �o�u���̍X�V
	m_pBubble->Update();

	if (CollisionEnemy())
	{ // �G�ɓ������Ă����ꍇ

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
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR3& rVec		// �ړ�����
)
{
	// �|�C���^��錾
	CMagic *pMagic = NULL;		// ���@�����p

	if (UNUSED(pMagic))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pMagic = new CMagic;	// ���@
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

		// �m�ۂ����A�h���X��Ԃ�
		return pMagic;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�X�e�[�^�X���擾����
//============================================================
CMagic::StatusInfo CMagic::GetStatusInfo(void)
{
	// �X�e�[�^�X����Ԃ�
	return m_statusInfo;
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
//	�^�[�Q�b�g�Ƃ̓����蔻��
//============================================================
bool CMagic::CollisionTarget(void)
{
	// �|�C���^��錾
	CTarget *pTarget = CSceneGame::GetTarget();	// �^�[�Q�b�g���

	if (pTarget->GetState() != CTarget::STATE_DESTROY)
	{ // �^�[�Q�b�g���j�󂳂�Ă��Ȃ��ꍇ

		// �^�[�Q�b�g�Ƃ̓����蔻��
		if (collision::Circle2D
		( // ����
			m_pos,										// ����ʒu
			pTarget->GetPosition(),						// ����ڕW�ʒu
			m_pBubble->GetRadius() + PLUS_HIT_RADIUS,	// ���蔼�a
			pTarget->GetRadius()						// ����ڕW���a
		))
		{ // ���@�ɓ������Ă����ꍇ

			// �������������Ԃ�
			return true;
		}
	}

	// �������Ă��Ȃ������Ԃ�
	return false;
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

				if (pObjCheck->GetState() == CEnemy::STATE_SPAWN
				||  pObjCheck->GetState() == CEnemy::STATE_DEATH)
				{ // �G�̏�Ԃ��X�|�[���܂��͎��S��Ԃ̏ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// ���@����
				if (collision::Circle3D
				( // ����
					m_pos,										// ����ʒu
					pObjCheck->GetPosition(),					// ����ڕW�ʒu
					m_pBubble->GetRadius() + PLUS_HIT_RADIUS,	// ���蔼�a
					pObjCheck->GetRadius()						// ����ڕW���a
				))
				{ // ���@�ɓ������Ă����ꍇ

					// �G�̃q�b�g����
					pObjCheck->Hit((m_pBubble->GetLevel() / (m_pBubble->GetMaxLevel() / 2)) + 1);

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
	memset(&m_statusInfo, 0, sizeof(m_statusInfo));	// �X�e�[�^�X���

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
							else if (strcmp(&aString[0], "LIFE") == 0)
							{ // �ǂݍ��񂾕����� LIFE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_statusInfo.nLife);	// ������ǂݍ���
							}
							else if (strcmp(&aString[0], "COOLTIME") == 0)
							{ // �ǂݍ��񂾕����� COOLTIME �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_statusInfo.nCoolTime);	// �N�[���^�C����ǂݍ���
							}
							else if (strcmp(&aString[0], "MOVE") == 0)
							{ // �ǂݍ��񂾕����� MOVE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_statusInfo.fMove);		// �ړ��ʂ�ǂݍ���
							}
							else if (strcmp(&aString[0], "BUBBLE_RADIUS") == 0)
							{ // �ǂݍ��񂾕����� BUBBLE_RADIUS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_statusInfo.bubbleRadius);	// �o�u�����a��ǂݍ���
							}
							else if (strcmp(&aString[0], "SHOT_PARTS") == 0)
							{ // �ǂݍ��񂾕����� SHOT_PARTS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_statusInfo.nShotParts);	// ���˃p�[�c��ǂݍ���
							}
							else if (strcmp(&aString[0], "SHOT_POS") == 0)
							{ // �ǂݍ��񂾕����� SHOT_POS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &m_statusInfo.shotPos.x);	// ���ˈʒuX��ǂݍ���
								fscanf(pFile, "%f", &m_statusInfo.shotPos.y);	// ���ˈʒuY��ǂݍ���
								fscanf(pFile, "%f", &m_statusInfo.shotPos.z);	// ���ˈʒuZ��ǂݍ���
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
