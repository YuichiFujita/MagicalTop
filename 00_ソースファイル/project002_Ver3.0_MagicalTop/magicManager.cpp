//============================================================
//
//	���@�}�l�[�W���[���� [magicManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "magicManager.h"
#include "manager.h"
#include "sceneGame.h"
#include "object.h"
#include "lockCursor.h"
#include "objectGauge2D.h"
#include "multiModel.h"
#include "player.h"
#include "target.h"
#include "enemy.h"
#include "stage.h"
#include "collision.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_MANA	(100)	// �ő�}�i
#define MANA_FRAME	(10)	// �}�i�ϓ��t���[��

#define GAUGE_POS		(D3DXVECTOR3(260.0f, 540.0f, 0.0f))	// �ʒu
#define GAUGE_GAUGESIZE	(D3DXVECTOR3(200.0f, 30.0f, 0.0f))	// �Q�[�W�傫��
#define GAUGE_FRONTCOL	(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f))	// �\�Q�[�W�F
#define GAUGE_BACKCOL	(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	// ���Q�[�W�F

#define HEAL_CNT	(120)	// �񕜏�ԂɈڍs����܂ł̃J�E���^�[
#define NORMAL_CNT	(40)	// �ʏ��ԂɈڍs����܂ł̃J�E���^�[

#define HEAL_SAFE_PLUS	(2)		// �񕜏�Ԉڍs�J�E���^�[�̃Z�[�t�G���A���̉��Z��
#define HEALCNT_AREAMUL	(10)	// �Z�[�t�G���A�O�ł̉񕜃J�E���^�[�ݒ�p�W��
#define SUB_TARGLIFE	(-2)	// �Z�[�t�G���A�Ń}�i�񕜎��̃^�[�Q�b�g�ւ̃_���[�W��

//************************************************************
//	�e�N���X [CMagicManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CMagicManager::CMagicManager()
{
	// �����o�ϐ����N���A
	memset(&m_apLockCursor[0], 0, sizeof(m_apLockCursor));	// ���b�N�I���\�����
	m_pMana = NULL;				// �}�i�̏��
	m_magic = CMagic::TYPE_FLY;	// ���@
	m_state = STATE_NORMAL;		// ���
	m_nCounterMagic = 0;		// ���@�Ǘ��J�E���^�[
	m_nCounterState = 0;		// ��ԊǗ��J�E���^�[
	m_nCounterHeal = 0;			// �񕜊Ǘ��J�E���^�[
}

//============================================================
//	�f�X�g���N�^
//============================================================
CMagicManager::~CMagicManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CMagicManager::Init(void)
{
	// �����o�ϐ���������
	memset(&m_apLockCursor[0], 0, sizeof(m_apLockCursor));	// ���b�N�I���\�����
	m_pMana = NULL;				// �}�i�̏��
	m_magic = CMagic::TYPE_FLY;	// ���@
	m_state = STATE_NORMAL;		// ���
	m_nCounterMagic = 0;		// ���@�Ǘ��J�E���^�[
	m_nCounterState = 0;		// ��ԊǗ��J�E���^�[
	m_nCounterHeal = 0;			// �񕜊Ǘ��J�E���^�[

	for (int nCntLock = 0; nCntLock < MAX_LOCK; nCntLock++)
	{ // ���b�N�I���̍ő吔���J��Ԃ�

		// ���b�N�I���\���̐���
		m_apLockCursor[nCntLock] = CLockCursor::Create(VEC3_ZERO);
		if (UNUSED(m_apLockCursor[nCntLock]))
		{ // ��g�p���̏ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// �}�i�̐���
	m_pMana = CObjectGauge2D::Create
	( // ����
		CObject::LABEL_GAUGE,	// �I�u�W�F�N�g���x��
		MAX_MANA,				// �ő�}�i
		MANA_FRAME,				// �}�i�ϓ��t���[��
		GAUGE_POS,				// �ʒu
		GAUGE_GAUGESIZE,		// �Q�[�W�傫��
		GAUGE_FRONTCOL,			// �\�Q�[�W�F
		GAUGE_BACKCOL			// ���Q�[�W�F
	);
	if (UNUSED(m_pMana))
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
void CMagicManager::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CMagicManager::Update(void)
{
	if (m_nCounterMagic > 0)
	{ // �J�E���^�[�� 0���傫���ꍇ

		// �J�E���^�[�����Z
		m_nCounterMagic--;
	}

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NORMAL:	// �ʏ���

		if (m_nCounterState < HEAL_CNT)
		{ // �J�E���^�[�����l��菬�����ꍇ

			if (CSceneGame::GetStage()->GetAreaPlayer() == CStage::AREA_SAFE)
			{ // �Z�[�t�G���A�ɂ���ꍇ

				// �J�E���^�[�����Z
				m_nCounterState += HEAL_SAFE_PLUS;
			}
			else
			{ // �Z�[�t�G���A�ɂ��Ȃ��ꍇ

				// �J�E���^�[�����Z
				m_nCounterState++;
			}
		}
		else
		{ // �J�E���^�[�����l�ȏ�̏ꍇ

			// �J�E���^�[��������
			m_nCounterState = 0;

			// ��Ԃ�ύX
			m_state = STATE_HEAL;	// �񕜏��
		}

		break;

	case STATE_ATTACK:	// �U�����

		if (m_nCounterState < NORMAL_CNT)
		{ // �J�E���^�[�����l��菬�����ꍇ

			// �J�E���^�[�����Z
			m_nCounterState++;
		}
		else
		{ // �J�E���^�[�����l�ȏ�̏ꍇ

			// �J�E���^�[��������
			m_nCounterState = 0;

			// ��Ԃ�ύX
			m_state = STATE_NORMAL;	// �ʏ���
		}

		break;

	case STATE_HEAL:	// �񕜏��

		if (CSceneGame::GetStage()->GetAreaPlayer() == CStage::AREA_SAFE)
		{ // �Z�[�t�G���A�ɂ���ꍇ

			if (m_pMana->GetNum() < MAX_MANA)
			{ // �}�i�������Ă���ꍇ

				// �}�i����
				m_pMana->AddNum(1);

				// �^�[�Q�b�g�̗̑͂����Z
				CSceneGame::GetTarget()->AddLife(SUB_TARGLIFE);
			}
		}
		else
		{ // �Z�[�t�G���A�ɂ��Ȃ��ꍇ

			if (m_nCounterHeal < HEALCNT_AREAMUL * CSceneGame::GetStage()->GetAreaPlayer())
			{ // �J�E���^�[�����l��菬�����ꍇ

				// �J�E���^�[�����Z
				m_nCounterHeal++;
			}
			else
			{ // �J�E���^�[�����l�ȏ�̏ꍇ

				// �J�E���^�[��������
				m_nCounterHeal = 0;

				// �}�i����
				m_pMana->AddNum(1);
			}
		}

		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	���@�̃��b�N�I������
//============================================================
void CMagicManager::LockOnMagic(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	CMagic::StatusInfo status = CMagic::GetStatusInfo(m_magic);	// ���@�X�e�[�^�X
	int nCurrentLock = 0;	// ���݂̃��b�N�I����

	// �ϐ��z���錾
	LockInfo aLockData[MAX_LOCK] = {};	// ���b�N�I�����

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		// �|�C���^��錾
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// �擪�I�u�W�F�N�g

		if (USED(pObjectTop))
		{ // �擪�����݂���ꍇ

			// �ϐ���錾
			bool  bHit    = false;	// ���茋��
			float fLength = 0.0f;	// �G�Ƃ̋���

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
				{ // �G�̏�Ԃ��X�|�[����Ԃ����S��Ԃ̏ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// ���E������
				bHit = collision::Circle2D
				( // ����
					rPos,						// ����ʒu
					pObjCheck->GetPosition(),	// ����ڕW�ʒu
					status.fViewRadius,			// ���蔼�a
					pObjCheck->GetRadius(),		// ����ڕW���a
					&fLength					// ����ڕW�Ƃ̋���
				);

				if (bHit)
				{ // ���E���̏ꍇ

					if (nCurrentLock < status.nLock)
					{ // ���݂̖��@�̍ő働�b�N�I������菭�Ȃ��ꍇ

						// ���݂̃��b�N�I���������Z
						nCurrentLock++;

						// ����ݒ�
						aLockData[nCurrentLock - 1].pObject = pObjCheck;	// ���b�N�I�������I�u�W�F�N�g
						aLockData[nCurrentLock - 1].fLength = fLength;		// ���b�N�I���I�u�W�F�N�g�Ƃ̋���

						if (nCurrentLock >= status.nLock)
						{ // ���b�N�I���̍ő吔�ɓ��B�����ꍇ

							// ���b�N�I���̃\�[�g
							SortLockOnMagic(&aLockData[0], nCurrentLock, pObjCheck, fLength);
						}
					}
					else
					{ // ���b�N�I�������ő�̏ꍇ

						// ���b�N�I���̃\�[�g
						SortLockOnMagic(&aLockData[0], nCurrentLock, pObjCheck, fLength);
					}
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}

	// �O��̃��b�N�I���̍폜
	for (int nCntLock = 0; nCntLock < MAX_LOCK; nCntLock++)
	{ // ���b�N�I���̍ő吔���J��Ԃ�

		// �ϐ���錾
		bool bSame = false;	// ���ꔻ��

		if (USED(m_apLockCursor[nCntLock]->GetLockObject()))
		{ // �I�u�W�F�N�g�����b�N�I�����Ă����ꍇ

			for (int nCntCheck = 0; nCntCheck < MAX_LOCK; nCntCheck++)
			{ // ���b�N�I���̍ő吔���J��Ԃ�

				if (m_apLockCursor[nCntLock]->GetLockObject() == aLockData[nCntCheck].pObject)
				{ // ���b�N���̃I�u�W�F�N�g������̃��b�N�I�u�W�F�N�g�����ꍇ

					// ����I�u�W�F�N�g�����݂�����Ԃɂ���
					bSame = true;

					// �����𔲂���
					break;
				}
			}
		}

		if (bSame == false)
		{ // ����I�u�W�F�N�g�����݂��Ȃ������ꍇ

			// �\�����Ȃ��ݒ�ɂ���
			m_apLockCursor[nCntLock]->SetEnableDraw(false);
		}
	}

	// ����̃��b�N�I���̐ݒ�
	for (int nCntLock = 0; nCntLock < nCurrentLock; nCntLock++)
	{ // ����̃��b�N�I�������J��Ԃ�

		// �\������ݒ�ɂ���
		m_apLockCursor[nCntLock]->SetEnableDraw(true);

		// ���b�N����I�u�W�F�N�g��ݒ�
		m_apLockCursor[nCntLock]->SetLockObject(aLockData[nCntLock].pObject);
	}
}

//============================================================
//	���@�̔��ˏ���
//============================================================
bool CMagicManager::ShotMagic(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posPlayer = CSceneGame::GetPlayer()->GetPosition();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = CSceneGame::GetPlayer()->GetRotation();	// �v���C���[����
	CMagic::StatusInfo status = CMagic::GetStatusInfo(m_magic);		// ���@�X�e�[�^�X
	int nNumShot = 0;	// ���ː�
	bool bLock = false;	// ���b�N�I����

	// �|�C���^��錾
	CPlayer *pPlayer = CSceneGame::GetPlayer();	// �v���C���[�̏��

	if (m_pMana->GetNum() > 0
	&&  m_nCounterMagic <= 0)
	{ // �}�i�����銎�A�N�[���^�C�����I�������ꍇ

		for (int nCntLock = 0; nCntLock < MAX_LOCK; nCntLock++)
		{ // ���b�N�I���̍ő吔���J��Ԃ�

			if (m_apLockCursor[nCntLock]->IsDraw())
			{ // ���b�N�I�����Ă����ꍇ

				// �ϐ���錾
				D3DXVECTOR3 magicPos, vecMove;	// ���ˈʒu�E�ړ�����
				D3DXMATRIX  mtxTrans, mtxWorld;	// �v�Z�p�}�g���b�N�X

				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&mtxWorld);

				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, status.shotPos.x, status.shotPos.y, status.shotPos.z);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

				// �}�g���b�N�X���|�����킹��
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &pPlayer->GetMultiModel(status.nShotParts)->GetMtxWorld());

				// �}�g���b�N�X���甭�ˈʒu�����߂�
				magicPos = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);

				// �ړ����������߂�
				vecMove = m_apLockCursor[nCntLock]->GetLockObject()->GetPosition() - magicPos;

				// ���@�I�u�W�F�N�g�̐���
				CMagic::Create
				( // ����
					m_magic,	// ���
					magicPos,	// �ʒu
					VEC3_ZERO,	// ����
					vecMove		// �ړ�����
				);

				// �J�E���^�[��ݒ�
				m_nCounterMagic = status.nCoolTime;

				// ���ː������Z
				nNumShot++;

				// ���b�N�I�������Ă����Ԃɂ���
				bLock = true;
			}
		}

		if (bLock == false)
		{ // ���b�N�I������Ă��Ȃ������ꍇ

			// �ϐ���錾
			D3DXVECTOR3 magicPos, vecMove;			// ���ˈʒu�E�ړ�����
			D3DXMATRIX  mtxTrans, mtxWorld;			// �v�Z�p�}�g���b�N�X
			float fRotVec = rotPlayer.y + D3DX_PI;	// ���˕���

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorld);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, status.shotPos.x, status.shotPos.y, status.shotPos.z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

			// �}�g���b�N�X���|�����킹��
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &pPlayer->GetMultiModel(status.nShotParts)->GetMtxWorld());

			// �}�g���b�N�X���甭�ˈʒu�����߂�
			magicPos = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);

			// �����𐳋K��
			useful::NormalizeRot(fRotVec);

			// �ړ�������ݒ�
			vecMove = D3DXVECTOR3(sinf(fRotVec), 0.0f, cosf(fRotVec));

			// ���@�I�u�W�F�N�g�̐���
			CMagic::Create
			( // ����
				m_magic,	// ���
				magicPos,	// �ʒu
				VEC3_ZERO,	// ����
				vecMove		// �ړ�����
			);

			// �J�E���^�[��ݒ�
			m_nCounterMagic = status.nCoolTime;

			// ���ː������Z
			nNumShot++;
		}

		// �}�i������
		m_pMana->AddNum(-nNumShot);

		// �J�E���^�[��������
		m_nCounterState = 0;

		// ��Ԃ�ݒ�
		m_state = STATE_ATTACK;	// �U�����

		// ���˂�����Ԃ�Ԃ�
		return true;
	}

	// ���˂��Ă��Ȃ���Ԃ�Ԃ�
	return false;
}

//============================================================
//	���@�̐ݒ菈��
//============================================================
void CMagicManager::SetMagic(const CMagic::TYPE type)
{
	// �����̖��@��ݒ�
	m_magic = type;
}

//============================================================
//	���@�擾����
//============================================================
CMagic::TYPE CMagicManager::GetMagic(void) const
{
	// ���@��Ԃ�
	return m_magic;
}

//============================================================
//	���@���b�N�I���S�폜����
//============================================================
void CMagicManager::DeleteLockOn(void)
{
	// ���b�N�I���̍폜
	for (int nCntLock = 0; nCntLock < MAX_LOCK; nCntLock++)
	{ // ���b�N�I���̍ő吔���J��Ԃ�

		// �\�����Ȃ��ݒ�ɂ���
		m_apLockCursor[nCntLock]->SetEnableDraw(false);
	}
}

//============================================================
//	��������
//============================================================
CMagicManager *CMagicManager::Create()
{
	// �|�C���^��錾
	CMagicManager *pMagicManager = NULL;	// ���@�}�l�[�W���[�����p

	if (UNUSED(pMagicManager))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pMagicManager = new CMagicManager;	// ���@�}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (USED(pMagicManager))
	{ // �g�p����Ă���ꍇ
		
		// ���@�}�l�[�W���[�̏�����
		if (FAILED(pMagicManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pMagicManager;
			pMagicManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pMagicManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CMagicManager::Release(CMagicManager *&prMagicManager)
{
	if (USED(prMagicManager))
	{ // �g�p���̏ꍇ

		// ���@�}�l�[�W���[�̏I��
		prMagicManager->Uninit();

		// �������J��
		delete prMagicManager;
		prMagicManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	���b�N�I���̃\�[�g����
//============================================================
void CMagicManager::SortLockOnMagic(LockInfo *pLock, const int nNumLock, CObject *pObject, const float fLength)
{
	// �ϐ���錾
	LockInfo keepLock;	// �\�[�g�p
	int nCurrentMaxID;	// �ő�l�̃C���f�b�N�X

	// ��O����
	assert(nNumLock > 0);	// ���b�N�I�������ŏ��l�ȉ�

	if (fLength <= pLock[nNumLock - 1].fLength)
	{ // �ŉ��̃��b�N�I�������߂����b�N�I���̏ꍇ

		// ����ݒ�
		pLock[nNumLock - 1].pObject = pObject;	// ���b�N�I�������I�u�W�F�N�g
		pLock[nNumLock - 1].fLength = fLength;	// ���b�N�I���I�u�W�F�N�g�Ƃ̋���

		for (int nCntKeep = 0; nCntKeep < (nNumLock - 1); nCntKeep++)
		{ // ����ւ���l�̑��� -1�񕪌J��Ԃ�

			// ���݂̌J��Ԃ�������
			nCurrentMaxID = nCntKeep;

			for (int nCntSort = nCntKeep + 1; nCntSort < nNumLock; nCntSort++)
			{ // ����ւ���l�̑��� - �\�[�g�񐔕��J��Ԃ�

				if (pLock[nCurrentMaxID].fLength > pLock[nCntSort].fLength)
				{ // �ő�l�ɐݒ肳��Ă��鋗�����A���݂̋����̂ق��������ꍇ

					// ���݂̃C���f�b�N�X���ő�l�ɐݒ�
					nCurrentMaxID = nCntSort;
				}
			}

			if (nCntKeep != nCurrentMaxID)
			{ // �ő�l�̃C���f�b�N�X�ɕϓ����������ꍇ

				// �v�f�̓���ւ�
				keepLock             = pLock[nCntKeep];
				pLock[nCntKeep]      = pLock[nCurrentMaxID];
				pLock[nCurrentMaxID] = keepLock;
			}
		}
	}
}
