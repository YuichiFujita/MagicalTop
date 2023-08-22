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
//	�O���錾
//************************************************************
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

	// �����o�֐�
	HRESULT Init(void);		// ������
	void Uninit(void);		// �I��
	void Update(void);		// �X�V
	bool ShotMagic(void);	// ���@����

	// �ÓI�����o�֐�
	static CMagicManager *Create(void);	// ����
	static HRESULT Release(CMagicManager *&prMagicManager);	// �j��

private:
	// �����o�ϐ�
	CObjectGauge2D *m_pMana;	// �}�i�̏��
	STATE m_state;				// ���
	int m_nCounterMagic;		// ���@�Ǘ��J�E���^�[
	int m_nCounterState;		// ��ԊǗ��J�E���^�[
	int m_nCounterHeal;			// �񕜊Ǘ��J�E���^�[
};

#endif	// _MAGICMANAGER_H_
