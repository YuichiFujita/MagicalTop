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
class CObjectGauge3D;	// �I�u�W�F�N�g�Q�[�W3D�N���X

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

	// �ˌ��󋵍\����
	typedef struct
	{
		float fRot;		// ���ˌ���
		bool bControl;	// �ˌ����삪�s���Ă��邩
		bool bShot;		// ���ۂɎˌ��ł��Ă��邩
	}Shot;

	// �����o�֐�
	HRESULT Init(void);		// ������
	void Uninit(void);		// �I��
	void Update(void);		// �X�V
	Shot ShotMagic(void);	// ���@����
	void SetEnableHealMana(const bool bHeal);	// �}�i�񕜏󋵐ݒ�
	void SetEnableManaDraw(const bool bDraw);	// �}�i�`��󋵐ݒ�
	void AddMana(const int nAdd);		// �}�i���Z
	void SetMana(const int nMana);		// �}�i�ݒ�
	void SetMaxMana(const int nMana);	// �ő�}�i�ݒ�
	int GetMana(void) const;			// �}�i�c�ʎ擾
	int GetMaxMana(void) const;			// �ő�}�i�擾

	// �ÓI�����o�֐�
	static CMagicManager *Create(CObject *pPlayer, const int nMaxMana);	// ����
	static HRESULT Release(CMagicManager *&prMagicManager);	// �j��

private:
	// �����o�ϐ�
	CObjectGauge3D *m_pMana;	// �}�i�̏��
	STATE m_state;				// ���
	int m_nCounterEffect;		// �G�t�F�N�g�Ǘ��J�E���^�[
	int m_nCounterMagic;		// ���@�Ǘ��J�E���^�[
	int m_nCounterState;		// ��ԊǗ��J�E���^�[
	int m_nCounterHeal;			// �񕜊Ǘ��J�E���^�[
	bool m_bHeal;				// �񕜏�
};

#endif	// _MAGICMANAGER_H_
