//============================================================
//
//	�I�u�W�F�N�g���� [object.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"
#include "manager.h"
#include "debugproc.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CObject *CObject::m_apTop[MAX_PRIO] = {};	// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
CObject *CObject::m_apCur[MAX_PRIO] = {};	// �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
int CObject::m_nNumAll = 0;					// �I�u�W�F�N�g�̑���

//************************************************************
//	�e�N���X [CObject] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObject::CObject()
{
	if (USED(m_apCur[DEFAULT_PRIO]))
	{ // �Ō�������݂���ꍇ

		// ���݂̍Ō���I�u�W�F�N�g�̎��I�u�W�F�N�g�����g�ɐݒ�
		m_apCur[DEFAULT_PRIO]->m_pNext = this;

		// �O�I�u�W�F�N�g��ݒ�
		m_pPrev = m_apCur[DEFAULT_PRIO];	// ���݂̍Ō���I�u�W�F�N�g

		// ���I�u�W�F�N�g���N���A
		m_pNext = NULL;

		// ���g�̏��A�h���X���Ō���ɐݒ�
		m_apCur[DEFAULT_PRIO] = this;
	}
	else
	{ // �Ō�������݂��Ȃ��ꍇ

		// ���g�̏��A�h���X��擪�ɐݒ�
		m_apTop[DEFAULT_PRIO] = this;

		// ���g�̏��A�h���X���Ō���ɐݒ�
		m_apCur[DEFAULT_PRIO] = this;

		// �O�I�u�W�F�N�g�̃N���A
		m_pPrev = NULL;

		// ���I�u�W�F�N�g�̃N���A
		m_pNext = NULL;
	}

	// ���g�̏����N���A
	m_label		= LABEL_NONE;	// �I�u�W�F�N�g���x��
	m_nPriority	= DEFAULT_PRIO;	// �D�揇��

	// �I�u�W�F�N�g�̑��������Z
	m_nNumAll++;
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CObject::CObject(const LABEL label, const int nPriority)
{
	if (USED(m_apCur[nPriority]))
	{ // �Ō�������݂���ꍇ

		// ���݂̍Ō���I�u�W�F�N�g�̎��I�u�W�F�N�g�����g�ɐݒ�
		m_apCur[nPriority]->m_pNext = this;

		// �O�I�u�W�F�N�g��ݒ�
		m_pPrev = m_apCur[nPriority];	// ���݂̍Ō���I�u�W�F�N�g

		// ���I�u�W�F�N�g���N���A
		m_pNext = NULL;

		// ���g�̏��A�h���X���Ō���ɐݒ�
		m_apCur[nPriority] = this;
	}
	else
	{ // �Ō�������݂��Ȃ��ꍇ

		// ���g�̏��A�h���X��擪�ɐݒ�
		m_apTop[nPriority] = this;

		// ���g�̏��A�h���X���Ō���ɐݒ�
		m_apCur[nPriority] = this;

		// �O�I�u�W�F�N�g�̃N���A
		m_pPrev = NULL;

		// ���I�u�W�F�N�g�̃N���A
		m_pNext = NULL;
	}

	// ���g�̏���ݒ�
	m_label		= label;		// �I�u�W�F�N�g���x��
	m_nPriority	= nPriority;	// �D�揇��

	// �I�u�W�F�N�g�̑��������Z
	m_nNumAll++;
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObject::~CObject()
{
	// �I�u�W�F�N�g�̑��������Z
	m_nNumAll--;
}

//============================================================
//	�q�b�g����
//============================================================
void CObject::Hit(const int nDmg)
{
	// ��O����
	assert(false);
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObject::SetPosition(const D3DXVECTOR3& rPos)
{
	// ��O����
	assert(false);
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObject::SetRotation(const D3DXVECTOR3& rRot)
{
	// ��O����
	assert(false);
}

//============================================================
//	�}�g���b�N�X�擾����
//============================================================
D3DXMATRIX CObject::GetMtxWorld(void) const
{
	// �ϐ���錾
	D3DXMATRIX mtxNone;	// �Ԃ�l�p

	// �}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxNone);

	// ��O����
	assert(false);

	// �P�ʃ}�g���b�N�X��Ԃ�
	return mtxNone;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObject::GetPosition(void) const
{
	// ��O����
	assert(false);

	// �����l��Ԃ�
	return VEC3_ZERO;
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObject::GetRotation(void) const
{
	// ��O����
	assert(false);

	// �����l��Ԃ�
	return VEC3_ZERO;
}

//============================================================
//	�傫���擾����
//============================================================
D3DXVECTOR3 CObject::GetScaling(void) const
{
	// ��O����
	assert(false);

	// �����l��Ԃ�
	return VEC3_ZERO;
}

//============================================================
//	���a�擾����
//============================================================
float CObject::GetRadius(void) const
{
	// ��O����
	assert(false);

	// �����l��Ԃ�
	return 0.0f;
}

//============================================================
//	�S�j������
//============================================================
void CObject::ReleaseAll(void)
{
	// �|�C���^��錾
	CObject *pObject;	// �I�u�W�F�N�g����p

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		if (USED(m_apTop[nCntPri]))
		{ // �擪�����݂���ꍇ

			// �I�u�W�F�N�g�̐擪����
			pObject = m_apTop[nCntPri];

			while (USED(pObject))
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �|�C���^��錾
				CObject *pObjectNext = pObject->m_pNext;	// ���̃I�u�W�F�N�g�ւ̃|�C���^

				// TODO�F�I�u�W�F�N�g�̑��ҊǗ�����Ŗ{���ɑ��v�H
				if (USED(pObjectNext))
				{ // ���̃I�u�W�F�N�g�����݂���ꍇ

					while (pObjectNext->m_label == LABEL_NONE)
					{ // ���̃I�u�W�F�N�g�̃��x�����ݒ肳��Ă��Ȃ��ꍇ�J��Ԃ�

						// ����Ɏ��̃I�u�W�F�N�g�ւ̃|�C���^���w��
						pObjectNext = pObjectNext->m_pNext;

						if (UNUSED(pObjectNext))
						{ // ����Ɏ��̃I�u�W�F�N�g�����݂��Ȃ��ꍇ

							// �����𔲂���
							break;
						}
					}
				}

				if (pObject->m_label != LABEL_NONE)
				{ // �I�u�W�F�N�g���x�����ݒ肳��Ă���ꍇ

					// �I�u�W�F�N�g�̏I��
					pObject->Uninit();
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObject = pObjectNext;
			}
		}
	}

	// ��O����
	if (m_nNumAll != 0) { assert(false); }	// �j���̎��s
}

//============================================================
//	�S�X�V����
//============================================================
void CObject::UpdateAll(void)
{
	// �|�C���^��錾
	CObject *pObject;	// �I�u�W�F�N�g����p

	// �f�o�b�O�\��
	CManager::GetDebugProc()->Print("�I�u�W�F�N�g���F%d\n", m_nNumAll);

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		if (USED(m_apTop[nCntPri]))
		{ // �擪�����݂���ꍇ

			// �I�u�W�F�N�g�̐擪����
			pObject = m_apTop[nCntPri];

			while (USED(pObject))
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �|�C���^��錾
				CObject *pObjectNext = pObject->m_pNext;	// ���̃I�u�W�F�N�g�ւ̃|�C���^

				// TODO�F�I�u�W�F�N�g�̑��ҊǗ�����Ŗ{���ɑ��v�H
				if (USED(pObjectNext))
				{ // ���̃I�u�W�F�N�g�����݂���ꍇ

					while (pObjectNext->m_label == LABEL_NONE)
					{ // ���̃I�u�W�F�N�g�̃��x�����ݒ肳��Ă��Ȃ��ꍇ�J��Ԃ�

						// ����Ɏ��̃I�u�W�F�N�g�ւ̃|�C���^���w��
						pObjectNext = pObjectNext->m_pNext;

						if (UNUSED(pObjectNext))
						{ // ����Ɏ��̃I�u�W�F�N�g�����݂��Ȃ��ꍇ

							// �����𔲂���
							break;
						}
					}
				}

				if (pObject->m_label != LABEL_NONE)
				{ // �I�u�W�F�N�g���x�����ݒ肳��Ă���ꍇ

					// �I�u�W�F�N�g�̍X�V
					pObject->Update();
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObject = pObjectNext;
			}
		}
	}
}

//============================================================
//	�S�`�揈��
//============================================================
void CObject::DrawAll(void)
{
	// �|�C���^��錾
	CObject *pObject;	// �I�u�W�F�N�g����p

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		if (USED(m_apTop[nCntPri]))
		{ // �擪�����݂���ꍇ

			// �I�u�W�F�N�g�̐擪����
			pObject = m_apTop[nCntPri];

			while (USED(pObject))
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �|�C���^��錾
				CObject *pObjectNext = pObject->m_pNext;	// ���̃I�u�W�F�N�g�ւ̃|�C���^

				// TODO�F�I�u�W�F�N�g�̑��ҊǗ�����Ŗ{���ɑ��v�H
				if (USED(pObjectNext))
				{ // ���̃I�u�W�F�N�g�����݂���ꍇ

					while (pObjectNext->m_label == LABEL_NONE)
					{ // ���̃I�u�W�F�N�g�̃��x�����ݒ肳��Ă��Ȃ��ꍇ�J��Ԃ�

						// ����Ɏ��̃I�u�W�F�N�g�ւ̃|�C���^���w��
						pObjectNext = pObjectNext->m_pNext;

						if (UNUSED(pObjectNext))
						{ // ����Ɏ��̃I�u�W�F�N�g�����݂��Ȃ��ꍇ

							// �����𔲂���
							break;
						}
					}
				}

				if (pObject->m_label != LABEL_NONE)
				{ // �I�u�W�F�N�g���x�����ݒ肳��Ă���ꍇ

					// �I�u�W�F�N�g�̕`��
					pObject->Draw();
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObject = pObjectNext;
			}
		}
	}
}

//============================================================
//	�g�p�m�F����
//============================================================
bool CObject::CheckUse(const CObject *pObject)
{
	// �|�C���^��錾
	CObject *pObjCheck;	// �I�u�W�F�N�g����p

	if (USED(pObject))
	{ // �|�C���^���g�p���̏ꍇ

		for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
		{ // �D�揇�ʂ̑������J��Ԃ�

			if (USED(m_apTop[nCntPri]))
			{ // �擪�����݂���ꍇ

				// �I�u�W�F�N�g�̐擪����
				pObjCheck = m_apTop[nCntPri];

				while (USED(pObjCheck))
				{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

					// �|�C���^��錾
					CObject *pObjectNext = pObjCheck->m_pNext;	// ���̃I�u�W�F�N�g�ւ̃|�C���^

					if (pObjCheck == pObject)
					{ // ���A�h���X�̏ꍇ

						// �^��Ԃ�
						return true;	// TODO�F�����A�h���X���m�ۂ���Ă��邾���̉\��������
					}

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;
				}
			}
		}

		// �U��Ԃ�
		return false;
	}
	else
	{ // �|�C���^����g�p���̏ꍇ

		// �U��Ԃ�
		return false;
	}
}

//============================================================
//	�擪�I�u�W�F�N�g�擾����
//============================================================
CObject *CObject::GetTop(const int nPriority)
{
	// �����̗D�揇�ʂ̐擪�I�u�W�F�N�g��Ԃ�
	return m_apTop[nPriority];
}

//============================================================
//	�Ō���I�u�W�F�N�g�擾����
//============================================================
CObject *CObject::GetCur(const int nPriority)
{
	// �����̗D�揇�ʂ̍Ō���I�u�W�F�N�g��Ԃ�
	return m_apCur[nPriority];
}

//============================================================
//	���x���̐ݒ菈��
//============================================================
void CObject::SetLabel(const LABEL label)
{
	if (label < LABEL_MAX)
	{ // �����̃��x�����g�p�\�ȏꍇ

		// �����̃��x����ݒ�
		m_label = label;
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	���x���̎擾����
//============================================================
CObject::LABEL CObject::GetLabel(void) const
{
	// ���x����Ԃ�
	return m_label;
}

//============================================================
//	�D�揇�ʂ̎擾����
//============================================================
int CObject::GetPriority(void) const
{
	// �D�揇�ʂ�Ԃ�
	return m_nPriority;
}

//============================================================
//	�I�u�W�F�N�g�擾����
//============================================================
CObject *CObject::GetObject(void)
{
	// ���g�̃|�C���^��Ԃ�
	return this;
}

//============================================================
//	�O�I�u�W�F�N�g�擾����
//============================================================
CObject *CObject::GetPrev(void) const
{
	// �O�I�u�W�F�N�g��Ԃ�
	return m_pPrev;
}

//============================================================
//	���I�u�W�F�N�g�擾����
//============================================================
CObject *CObject::GetNext(void) const
{
	// ���I�u�W�F�N�g��Ԃ�
	return m_pNext;
}

//============================================================
//	�j������
//============================================================
void CObject::Release(void)
{
	// �O�̃I�u�W�F�N�g���Ȃ��Ȃ���
	if (USED(m_pNext))
	{ // ���̃I�u�W�F�N�g�����݂���ꍇ

		// �O�̃I�u�W�F�N�g��ύX
		m_pNext->m_pPrev = m_pPrev;
	}

	// ���̃I�u�W�F�N�g���Ȃ��Ȃ���
	if (USED(m_pPrev))
	{ // �O�̃I�u�W�F�N�g�����݂���ꍇ

		// ���̃I�u�W�F�N�g��ύX
		m_pPrev->m_pNext = m_pNext;
	}

	// �擪�I�u�W�F�N�g�̕ύX
	if (m_apTop[m_nPriority] == this)
	{ // �擪�I�u�W�F�N�g���j������I�u�W�F�N�g�������ꍇ

		// ���̃I�u�W�F�N�g��擪�Ɏw��
		m_apTop[m_nPriority] = m_pNext;
	}

	// �Ō���I�u�W�F�N�g�̕ύX
	if (m_apCur[m_nPriority] == this)
	{ // �Ō���I�u�W�F�N�g���j������I�u�W�F�N�g�������ꍇ

		// �O�̃I�u�W�F�N�g���Ō���Ɏw��
		m_apCur[m_nPriority] = m_pPrev;
	}

	if (USED(this))
	{ // �g�p����Ă���ꍇ

		// �������J��
		delete this;
	}
}
