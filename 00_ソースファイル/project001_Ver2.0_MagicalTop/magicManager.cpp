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
#include "object.h"
#include "lockCursor.h"
#include "player.h"
#include "collision.h"

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
	m_magic = CMagic::TYPE_LV0_NORMAL;	// ���@
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
	m_magic = CMagic::TYPE_LV0_NORMAL;	// ���@

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
//	���@�̃��b�N�I������
//============================================================
void CMagicManager::LockOnMagic(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	CMagic::StatusInfo status = CMagic::GetStatusInfo(m_magic);	// ���@�X�e�[�^�X
	int nCurrentLock = 0;	// ���݂̃��b�N�I����

	// �ϐ��z���錾
	LockInfo aLockData[MAX_LOCK] = {};	// ���b�N�I�����

	// TODO�F�����蔻��
#if 0
	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
		{ // �I�u�W�F�N�g�̑������J��Ԃ�

			// �ϐ���錾
			bool bHit = false;		// ���茋��
			float fLength = 0.0f;	// �G�Ƃ̋���

			// �|�C���^��錾
			CObject *pObject = CObject::GetObject(nCntPri, nCntObject);	// �I�u�W�F�N�g

			if (UNUSED(pObject)
			||  pObject->GetLabel() != CObject::LABEL_ENEMY)
			{ // �I�u�W�F�N�g����g�p���E���x�����G�ł͂Ȃ��ꍇ

				// ���̌J��Ԃ��Ɉڍs
				continue;
			}

			// ���E������
			bHit = collision::Circle2D
			( // ����
				rPos,					// ����ʒu
				pObject->GetPosition(),	// ����ڕW�ʒu
				status.fViewRadius,		// ���蔼�a
				pObject->GetRadius(),	// ����ڕW���a
				&fLength				// ����ڕW�Ƃ̋���
			);

			if (bHit)
			{ // ���E���̏ꍇ

				if (nCurrentLock < status.nLock)
				{ // ���݂̖��@�̍ő働�b�N�I������菭�Ȃ��ꍇ

					// ���݂̃��b�N�I���������Z
					nCurrentLock++;

					// ����ݒ�
					aLockData[nCurrentLock - 1].pObject = pObject;	// ���b�N�I�������I�u�W�F�N�g
					aLockData[nCurrentLock - 1].fLength = fLength;	// ���b�N�I���I�u�W�F�N�g�Ƃ̋���

					if (nCurrentLock >= status.nLock)
					{ // ���b�N�I���̍ő吔�ɓ��B�����ꍇ

						// ���b�N�I���̃\�[�g
						SortLockOnMagic(&aLockData[0], nCurrentLock, pObject, fLength);
					}
				}
				else
				{ // ���b�N�I�������ő�̏ꍇ

					// ���b�N�I���̃\�[�g
					SortLockOnMagic(&aLockData[0], nCurrentLock, pObject, fLength);
				}
			}
		}
	}
#endif

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

			// �`�悵�Ȃ��ݒ�ɂ���
			m_apLockCursor[nCntLock]->SetEnableDraw(false);
		}
	}

	// ����̃��b�N�I���̐ݒ�
	for (int nCntLock = 0; nCntLock < nCurrentLock; nCntLock++)
	{ // ����̃��b�N�I�������J��Ԃ�

		// �`�悷��ݒ�ɂ���
		m_apLockCursor[nCntLock]->SetEnableDraw(true);

		// ���b�N����I�u�W�F�N�g��ݒ�
		m_apLockCursor[nCntLock]->SetLockObject(aLockData[nCntLock].pObject);
	}
}

//============================================================
//	���@�̔��ˏ���
//============================================================
void CMagicManager::ShotMagic(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posPlayer = CManager::GetPlayer()->GetPosition();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = CManager::GetPlayer()->GetRotation();	// �v���C���[����
	bool bLock = false;	// ���b�N�I����

	for (int nCntLock = 0; nCntLock < MAX_LOCK; nCntLock++)
	{ // ���b�N�I���̍ő吔���J��Ԃ�

		if (m_apLockCursor[nCntLock]->IsDraw())
		{ // ���b�N�I�����Ă����ꍇ

			// �ϐ���錾
			D3DXVECTOR3 magicPos = D3DXVECTOR3(posPlayer.x, posPlayer.y + PLAY_MAGIC_POS_PLUS_Y, posPlayer.z);	// ���ˈʒu
			D3DXVECTOR3 vecMove = m_apLockCursor[nCntLock]->GetLockObject()->GetPosition() - posPlayer;			// �ړ�����

			// ���@�I�u�W�F�N�g�̐���
			CMagic::Create
			( // ����
				m_magic,	// ���
				magicPos,	// �ʒu
				VEC3_ZERO,	// ����
				vecMove		// �ړ�����
			);

			// ���b�N�I�������Ă����Ԃɂ���
			bLock = true;
		}
	}

	if (bLock == false)
	{ // ���b�N�I������Ă��Ȃ������ꍇ

		// �ϐ���錾
		D3DXVECTOR3 magicPos = D3DXVECTOR3(posPlayer.x, posPlayer.y + PLAY_MAGIC_POS_PLUS_Y, posPlayer.z);	// ���ˈʒu
		D3DXVECTOR3 vecMove;					// �ړ�����
		float fRotVec = rotPlayer.y + D3DX_PI;	// ���˕���

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
	}
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
//	��������
//============================================================
CMagicManager *CMagicManager::Create()
{
	// �|�C���^��錾
	CMagicManager *pMagicManager = NULL;	// ���@�}�l�[�W���[�����p

	if (UNUSED(pMagicManager))
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pMagicManager = new CMagicManager();	// ���@�}�l�[�W���[
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
