//============================================================
//
//	���@�}�l�[�W���[�w�b�_�[ [magicManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _MAGICMANAGER_H_
#define _MAGICMANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "magic.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_LOCK	(6)	// ���b�N�I���̍ő吔

//************************************************************
//	�O���錾
//************************************************************
class CLockCursor;	// ���b�N�I���\��

//************************************************************
//	�N���X��`
//************************************************************
// ���@�}�l�[�W���[�N���X
class CMagicManager
{
public:
	// �R���X�g���N�^
	CMagicManager();

	// �f�X�g���N�^
	~CMagicManager();

	// ���b�N�I�����\����
	typedef struct
	{
		CObject *pObject;	// ���b�N�I�������I�u�W�F�N�g
		float fLength;		// ���b�N�I���I�u�W�F�N�g�Ƃ̋���
	}LockInfo;

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void LockOnMagic(const D3DXVECTOR3& rPos);	// ���@���b�N�I��
	void ShotMagic(void);						// ���@����
	void SetMagic(const CMagic::TYPE type);		// ���@�ݒ�
	CMagic::TYPE GetMagic(void) const;			// ���@�擾
	void DeleteLockOn(void);					// ���@���b�N�I���S�폜

	// �ÓI�����o�֐�
	static CMagicManager *Create(void);	// ����
	static HRESULT Release(CMagicManager *&prMagicManager);	// �j��

private:
	// �����o�֐�
	void SortLockOnMagic(LockInfo *pLock, const int nNumLock, CObject *pObject, const float fLength);	// ���b�N�I���\�[�g

	// �����o�ϐ�
	CLockCursor *m_apLockCursor[MAX_LOCK];	// ���b�N�I���\�����
	CMagic::TYPE m_magic;	// ���@
};

#endif	// _MAGICMANAGER_H_
