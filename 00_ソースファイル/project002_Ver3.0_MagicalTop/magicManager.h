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
class CLockCursor;		// ���b�N�I���\���N���X
class CObjectGauge2D;	// �I�u�W�F�N�g�Q�[�W2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// ���@�}�l�[�W���[�N���X
class CMagicManager
{
public:
	// ��ԗ�
	typedef enum
	{
		STATE_NORMAL = 0,	// �ʏ���
		STATE_ATTACK,		// �U�����
		STATE_HEAL,			// �񕜏��
		STATE_MAX,			// ���̗񋓌^�̑���
	}STATE;

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
	void Update(void);	// �X�V
	void LockOnMagic(const D3DXVECTOR3& rPos);	// ���@���b�N�I��
	bool ShotMagic(void);						// ���@����
	void DeleteLockOn(void);					// ���@���b�N�I���S�폜

	// �ÓI�����o�֐�
	static CMagicManager *Create(void);	// ����
	static HRESULT Release(CMagicManager *&prMagicManager);	// �j��

private:
	// �����o�֐�
	void SortLockOnMagic(LockInfo *pLock, const int nNumLock, CObject *pObject, const float fLength);	// ���b�N�I���\�[�g

	// �����o�ϐ�
	CLockCursor *m_apLockCursor[MAX_LOCK];	// ���b�N�I���\�����
	CObjectGauge2D *m_pMana;	// �}�i�̏��
	STATE m_state;				// ���
	int m_nCounterMagic;		// ���@�Ǘ��J�E���^�[
	int m_nCounterState;		// ��ԊǗ��J�E���^�[
	int m_nCounterHeal;			// �񕜊Ǘ��J�E���^�[
};

#endif	// _MAGICMANAGER_H_
