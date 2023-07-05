//============================================================
//
//	�G�w�b�_�[ [enemy.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"
#include "motion.h"

//************************************************************
//	�O���錾
//************************************************************
class CMultiModel;	// �}���`���f���N���X

//************************************************************
//	�N���X��`
//************************************************************
// �G�N���X
class CEnemy : public CObject
{
public:
	// ��ޗ�
	typedef enum
	{
		TYPE_WALK = 0,	// ����
		TYPE_CAR,		// ���
		TYPE_FLY,		// �w��
		TYPE_MAX		// ���̗񋓌^�̑���
	}TYPE;

	// �R���X�g���N�^
	CEnemy(const TYPE type);

	// �f�X�g���N�^
	~CEnemy();

	// �p�[�c���\����
	typedef struct
	{
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 rot;	// ����
		int nParentID;		// �e�C���f�b�N�X
	}Parts;

	// �p�[�c�\����
	typedef struct
	{
		Parts aInfo[MAX_PARTS];	// �p�[�c���
		int nNumParts;			// �p�[�c��
	}PartsInfo;

	// �X�e�[�^�X�\����
	typedef struct
	{
		int		nLife;				// �̗�
		int		nScore;				// �X�R�A���Z��
		int		nCounterAttack;		// �U���p�x
		int		nBullLife;			// �e�̎���
		float	fBullRadius;		// �e�̔��a
		float	fBullMove;			// �e�̈ړ���
		float	fRadius;			// ���a
		float	fHeight;			// �c��
		float	fForwardMove;		// �O�i�̈ړ���
		float	fBackwardMove;		// ��ނ̈ړ���
		float	fLookRevision;		// �v���C���[�����������␳�W��
		float	fFindRadius;		// ���m�͈�
		float	fAttackRadius;		// �U���͈�
		float	fBackwardRadius;	// ��ޔ͈�
		bool	bBackward;			// ��ނ� ON/OFF
	}StatusInfo;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	void Hit(const int nDmg);	// �q�b�g

	// �ÓI�����o�֐�
	static void LoadSetup(void);	// �Z�b�g�A�b�v
	static CEnemy *Create	// ����
	( // ����
		const TYPE type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot		// ����
	);

	// �����o�֐�
	void SetPosition(const D3DXVECTOR3& rPos);		// �ʒu�ݒ�
	void SetMovePosition(const D3DXVECTOR3& rMove);	// �ʒu�ړ��ʐݒ�
	void SetRotation(const D3DXVECTOR3& rRot);		// �����ݒ�
	void SetMoveRotation(const D3DXVECTOR3& rMove);	// �����ύX�ʐݒ�

	D3DXMATRIX GetMtxWorld(void) const;			// �}�g���b�N�X�擾
	D3DXVECTOR3 GetPosition(void) const;		// �ʒu�擾
	D3DXVECTOR3 GetOldPosition(void) const;		// �ߋ��ʒu�擾
	D3DXVECTOR3 GetMovePosition(void) const;	// �ʒu�ړ��ʎ擾
	D3DXVECTOR3 GetRotation(void) const;		// �����擾
	D3DXVECTOR3 GetMoveRotation(void) const;	// �����ύX�ʎ擾
	float GetRadius(void) const;				// ���a�擾
	StatusInfo GetStatusInfo(void) const;		// �X�e�[�^�X���擾
	PartsInfo GetPartsInfo(void) const;			// �p�[�c���擾

protected:
	// �����o�֐�
	virtual void CollisionFind(void);	// ���m�͈͂̓����蔻��
	void Look(const D3DXVECTOR3& rPos);	// �Ώێ��F
	void Attack(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot);	// �U��
	void CollisionTarget(D3DXVECTOR3& rPos, D3DXVECTOR3& rPosOld);	// �^�[�Q�b�g�Ƃ̓����蔻��
	void CollisionEnemy(D3DXVECTOR3& rPos);							// �G�Ƃ̓����蔻��

private:
	// �ÓI�����o�ϐ�
	static StatusInfo m_aStatusInfo[TYPE_MAX];	// �X�e�[�^�X���
	static PartsInfo m_aPartsInfo[TYPE_MAX];	// �p�[�c���

	// �����o�ϐ�
	const StatusInfo m_status;	// �X�e�[�^�X�萔
	const PartsInfo m_parts;	// �p�[�c�萔
	D3DXMATRIX	m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3	m_pos;			// ���݈ʒu
	D3DXVECTOR3	m_oldPos;		// �ߋ��ʒu
	D3DXVECTOR3	m_movePos;		// �ʒu�ړ���
	D3DXVECTOR3	m_rot;			// ����
	D3DXVECTOR3	m_moveRot;		// �����ύX��
	int m_nLife;				// �̗�
	int m_nCounterAtk;			// �U���Ǘ��J�E���^�[
};

// ��ԓG�N���X
class CEnemyCar : public CEnemy
{
public:
	// ��ޗ�
	typedef enum
	{
		MODEL_CATERPILLAR = 0,	// �L���^�s��
		MODEL_CANNON,			// �L���m��
		MODEL_MAX				// ���̗񋓌^�̑���
	}MODEL;

	// �R���X�g���N�^
	CEnemyCar(const TYPE type);

	// �f�X�g���N�^
	~CEnemyCar();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

private:
	// �I�[�o�[���C�h�֐�
	void CollisionFind(void);	// ���m�͈͂̓����蔻��
	void SetRotationCannon(const D3DXVECTOR3& rLookPos, D3DXVECTOR3& rRotCannon);	// �L���m�������ݒ�

	// �ÓI�����o�ϐ�
	static const char *mc_apModelFile[];	// ���f���萔

	// �����o�ϐ�
	CMultiModel	*m_apMultiModel[MAX_PARTS];	// ���f���̏��
	int m_nNumModel;	// �p�[�c�̑���
};

#endif	// _ENEMY_H_
