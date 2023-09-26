//============================================================
//
//	���@�w�b�_�[ [magic.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _MAGIC_H_
#define _MAGIC_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"
#include "motion.h"

//************************************************************
//	�O���錾
//************************************************************
class CBubble;	// �o�u���N���X
class CShadow;	// �e�N���X

//************************************************************
//	�N���X��`
//************************************************************
// ���@�N���X
class CMagic : public CObject
{
public:
	// ��ԗ�
	typedef enum
	{
		STATE_NORMAL = 0,	// �ʏ���
		STATE_DELETE,		// �������
		STATE_MAX			// ���̗񋓌^�̑���
	}STATE;

	// ���x����
	typedef enum
	{
		LEVEL_00 = 0,	// ���x��00
		LEVEL_01,		// ���x��01
		LEVEL_02,		// ���x��02
		LEVEL_MAX		// ���̗񋓌^�̑���
	}LEVEL;

	// ���x������
	typedef enum
	{
		LEVELINFO_NUM = 0,	// ���ː�
		LEVELINFO_SPEED,	// �e��
		LEVELINFO_RAPID,	// �A�ˑ��x
		LEVELINFO_MAX		// ���̗񋓌^�̑���
	}LEVELINFO;

	// �R���X�g���N�^
	CMagic();

	// �f�X�g���N�^
	~CMagic();

	// ���x���X�e�[�^�X�\����
	typedef struct
	{
		int		nNumBunnle;	// �o�u����
		float	fMove;		// �ړ���
		int		nCoolTime;	// �N�[���^�C��
	}LevelStatusInfo;

	// �X�e�[�^�X�\����
	typedef struct
	{
		LevelStatusInfo aLevel[LEVEL_MAX];	// ���x���X�e�[�^�X
		D3DXVECTOR3 shotPos;	// ���ˈʒu
		int		nShotParts;		// ���˃p�[�c
		int		nLife;			// ����
		float	fBubbleRadius;	// �o�u�����a
		float	fShotAngle;		// �ˌ��p�x
	}StatusInfo;

	// ���x���\����
	typedef struct
	{
		int nNumBunnle;	// �o�u����
		int nMove;		// �ړ���
		int nCoolTime;	// �N�[���^�C��
	}LevelInfo;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static void LoadSetup(void);	// �Z�b�g�A�b�v
	static CMagic *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR3& rVec		// �ړ�����
	);
	static StatusInfo GetStatusInfo(void);				// �X�e�[�^�X���擾
	static void AddLevelStatus(const LEVELINFO level);	// �X�e�[�^�X���x�����Z
	static int GetLevelStatus(const LEVELINFO level);	// �X�e�[�^�X���x���擾

	// �����o�֐�
	void SetMove(D3DXVECTOR3 vec, const float fMove);	// �ړ��ʐݒ�
	void SetPosition(const D3DXVECTOR3& rPos);			// �ʒu�ݒ�
	void SetRotation(const D3DXVECTOR3& rRot);			// �����ݒ�
	D3DXVECTOR3 GetPosition(void) const;				// �ʒu�擾
	D3DXVECTOR3 GetRotation(void) const;				// �����擾

private:
	// �����o�֐�
	bool CollisionEnemy(void);	// �G�Ƃ̓����蔻��

	// �ÓI�����o�ϐ�
	static StatusInfo m_statusInfo;	// �X�e�[�^�X���
	static LevelInfo m_level;		// ���x�����

	// �����o�ϐ�
	CBubble *m_pBubble;		// �o�u�����
	CShadow *m_pShadow;		// �e�̏��
	D3DXMATRIX	m_mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3	m_pos;		// ���݈ʒu
	D3DXVECTOR3	m_movePos;	// �ʒu�ړ���
	D3DXVECTOR3	m_rot;		// ����
	D3DXVECTOR3	m_moveRot;	// �����ύX��
	STATE m_state;			// ���
	int m_nCounterEffect;	// �G�t�F�N�g�Ǘ��J�E���^�[
};

#endif	// _MAGIC_H_
