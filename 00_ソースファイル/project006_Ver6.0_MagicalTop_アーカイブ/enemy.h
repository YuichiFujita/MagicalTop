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
#include "objectChara.h"

//************************************************************
//	�O���錾
//************************************************************
class CShadow;			// �e�N���X
class CObjectBillboard;	// �I�u�W�F�N�g�r���{�[�h�N���X
class CEnemyWay;		// �G�����\���N���X
class CBubble;			// �o�u���N���X

//************************************************************
//	�N���X��`
//************************************************************
// �G�N���X
class CEnemy : public CObjectChara
{
public:
	// �e�N�X�`����
	typedef enum
	{
		TEXTURE_WARNING = 0,	// �x���\��
		TEXTURE_MAX				// ���̗񋓌^�̑���
	}TEXTURE;

	// ��ޗ�
	typedef enum
	{
		TYPE_HUMAN = 0,	// ����
		TYPE_CAR,		// ���
		TYPE_MAX		// ���̗񋓌^�̑���
	}TYPE;

	// ��ԗ�
	typedef enum
	{
		STATE_SPAWN = 0,	// �X�|�[�����
		STATE_NORMAL,		// �ʏ���
		STATE_DAMAGE,		// �_���[�W���
		STATE_DEATH,		// ���S���
		STATE_VANISH,		// �������
		STATE_MAX			// ���̗񋓌^�̑���
	}STATE;

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
		D3DXVECTOR3 bullPos;		// �e�̔��ˈʒu
		D3DXVECTOR3 bubbleSize;		// �o�u���̑傫��
		int		nLife;				// �̗�
		int		nScore;				// �X�R�A���Z��
		int		nExp;				// �o���l������
		int		nCounterAttack;		// �U���p�x
		int		nBullParts;			// �e�̔��˃p�[�c
		int		nBullLife;			// �e�̎���
		int		nBullDamage;		// �e�̍U����
		float	fBullRadius;		// �e�̔��a
		float	fBullMove;			// �e�̈ړ���
		float	fShadowRadius;		// �e�̔��a
		float	fSpawnHeight;		// ��������Y���W
		float	fCollRadius;		// �����蔻��̔��a
		float	fRadius;			// ���a
		float	fHeight;			// �c��
		float	fKnockBackUp;		// �m�b�N�o�b�N��ړ���
		float	fKnockBackSide;		// �m�b�N�o�b�N���ړ���
		float	fKnockBackRevision;	// �m�b�N�o�b�N�����W��
		float	fForwardMove;		// �O�i�̈ړ���
		float	fLookRevision;		// �v���C���[�����������␳�W��
		float	fAttackRadius;		// �U���͈�
	}StatusInfo;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	void Hit(const int nDmg);	// �q�b�g
	void HitKnockBack(const int nDmg, const D3DXVECTOR3& vec);	// �m�b�N�o�b�N�q�b�g

	// �ÓI�����o�֐�
	static void LoadSetup(void);	// �Z�b�g�A�b�v
	static CEnemy *Create	// ����
	( // ����
		const TYPE type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot		// ����
	);
	static void RandomSpawn	// �����_������
	( // ����
		const int nNum,	// ������
		const TYPE type	// ���
	);
	static StatusInfo GetStatusInfo(const int nType);	// �X�e�[�^�X���擾
	static void SetAllVanish(void);	// �S����
	static int GetNumAll(void);		// �����擾

	// �����o�֐�
	void UpdateOldPosition(void);					// �ߋ��ʒu�X�V
	void UpdateKnockBack(D3DXVECTOR3& rPos);		// �m�b�N�o�b�N�X�V
	void SetMovePosition(const D3DXVECTOR3& rMove);	// �ʒu�ړ��ʐݒ�
	void SetMoveRotation(const D3DXVECTOR3& rMove);	// �����ύX�ʐݒ�
	void SetState(const int nState);				// ��Ԑݒ�
	D3DXMATRIX GetMtxWorld(void) const;				// �}�g���b�N�X�擾
	D3DXVECTOR3 GetOldPosition(void) const;			// �ߋ��ʒu�擾
	D3DXVECTOR3 GetMovePosition(void) const;		// �ʒu�ړ��ʎ擾
	D3DXVECTOR3 GetMoveRotation(void) const;		// �����ύX�ʎ擾
	int GetType(void) const;						// ��ގ擾
	int GetState(void) const;						// ��Ԏ擾
	float GetRadius(void) const;					// ���a�擾
	float GetHeight(void) const;					// �c���擾
	StatusInfo GetStatusInfo(void) const;			// �X�e�[�^�X���擾
	PartsInfo GetPartsInfo(void) const;				// �p�[�c���擾
	CMotion::Info GetMotionInfo(void) const;		// ���[�V�������擾

protected:
	// �������z�֐�
	virtual const char* GetModelFileName(const int nModel) const = 0;	// ���f���t�@�C���擾
	virtual void CollisionFind(void) = 0;	// ���m�͈͂̓����蔻��

	// ���z�֐�
	virtual void Spawn(void);	// �X�|�[������
	virtual void Damage(void);	// �_���[�W����
	virtual bool Death(void);	// ���S����
	virtual bool Vanish(void);	// ��������

	// �����o�֐�
	void Look	// �Ώێ��F
	( // ����
		const D3DXVECTOR3& rPosLook,	// ���F�Ώۈʒu
		const D3DXVECTOR3& rPosEnemy,	// �G�ʒu
		D3DXVECTOR3& rRotEnemy			// �G����
	);

	void Attack(const D3DXVECTOR3& rLookPos, const D3DXVECTOR3& rThisPos, const float fLookRadius);	// �U��
	void CollisionTarget(D3DXVECTOR3& rPos);			// �^�[�Q�b�g�Ƃ̓����蔻��
	void CollisionSpawnEnemy(D3DXVECTOR3& rPos);		// �X�|�[�����̓G�Ƃ̓����蔻��
	void CollisionNormalEnemy(D3DXVECTOR3& rPos);		// �ʏ펞�̓G�Ƃ̓����蔻��
	void SetDrawWarning(const bool bDraw);				// �x���̕`��󋵐ݒ�
	void SetPositionWarning(const D3DXVECTOR3& rPos);	// �x���̈ʒu�ݒ�
	void RandomRotation(void);	// �����_����]
	void SubBubble(void);		// �o�u���폜

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];			// �e�N�X�`���萔
	static StatusInfo m_aStatusInfo[TYPE_MAX];		// �X�e�[�^�X���
	static PartsInfo m_aPartsInfo[TYPE_MAX];		// �p�[�c���
	static CMotion::Info m_aMotionInfo[TYPE_MAX];	// ���[�V�������
	static int m_nNumAll;							// �G�̑���

	// �����o�ϐ�
	CShadow *m_pShadow;				// �e�̏��
	CObjectBillboard *m_pWarning;	// �x���̏��
	CEnemyWay *m_pWay;				// �����̏��
	CBubble *m_pBubble;				// �o�u���̏��
	D3DXVECTOR3	m_oldPos;			// �ߋ��ʒu
	D3DXVECTOR3	m_movePos;			// �ʒu�ړ���
	D3DXVECTOR3 m_moveKnock;		// �m�b�N�o�b�N�ړ���
	D3DXVECTOR3	m_moveRot;			// �����ύX��
	D3DXVECTOR3 m_deathMoveRot;		// ���S���̌����ύX��
	STATE m_state;					// ���
	int m_nCounterEffect;			// �G�t�F�N�g�Ǘ��J�E���^�[
	int m_nCounterBubble;			// �o�u���Ǘ��J�E���^�[
	int m_nCounterState;			// ��ԊǗ��J�E���^�[
	int m_nCounterAtk;				// �U���Ǘ��J�E���^�[
	const TYPE m_type;				// ��ޒ萔
	const StatusInfo m_status;		// �X�e�[�^�X�萔
	const PartsInfo m_parts;		// �p�[�c�萔
	const CMotion::Info m_motion;	// ���[�V�����萔
};

// �����G�N���X
class CEnemyHuman : public CEnemy
{
public:
	// ��ޗ�
	typedef enum
	{
		MODEL_WAIST,				// ��
		MODEL_BODY,					// ��
		MODEL_HEAD,					// ��
		MODEL_ARMUL,				// ����r
		MODEL_ARMUR,				// �E��r
		MODEL_ARMDL,				// �����r
		MODEL_ARMDR,				// �E���r
		MODEL_HANDL,				// ����
		MODEL_HANDR,				// �E��
		MODEL_LEGUL,				// ��������
		MODEL_LEGUR,				// �E������
		MODEL_LEGDL,				// ����
		MODEL_LEGDR,				// �E��
		MODEL_FOOTL,				// ����
		MODEL_FOOTR,				// �E��
		MODEL_WAISTPOUCHBACK,		// �E�G�X�g�|�[�`(��)
		MODEL_WAISTPOUCHRIGHT,		// �E�G�X�g�|�[�`(��)
		MODEL_WAISTPOUCHBACKRIGHT,	// �E�G�X�g�|�[�`(����)
		MODEL_LEGURPOUCH,			// �E�������|�[�`
		MODEL_ARMOR,				// �A�[�}�[
		MODEL_KNIFE,				// �i�C�t
		MODEL_ASSAULT,				// �e
		MODEL_MAX					// ���̗񋓌^�̑���
	}MODEL;

	// ���[�V������
	typedef enum
	{
		MOTION_MOVE = 0,	// ���s���[�V����
		MOTION_ACTION,		// �U�����[�V����
		MOTION_MAX			// ���̗񋓌^�̑���
	}MOTION;

	// �R���X�g���N�^
	CEnemyHuman(const TYPE type);

	// �f�X�g���N�^
	~CEnemyHuman();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	const char* GetModelFileName(const int nModel) const;	// ���f���t�@�C���擾

private:
	// �I�[�o�[���C�h�֐�
	void Spawn(void);			// �X�|�[������
	void CollisionFind(void);	// ���m�͈͂̓����蔻��

	// �ÓI�����o�ϐ�
	static const char *mc_apModelFile[];	// ���f���萔
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

	// �X�|�[����ԗ�
	typedef enum
	{
		SPAWN_WAIT = 0,	// �ҋ@���
		SPAWN_FALL,		// �������
		SPAWN_MAX		// ���̗񋓌^�̑���
	}SPAWN;

	// �R���X�g���N�^
	CEnemyCar(const TYPE type);

	// �f�X�g���N�^
	~CEnemyCar();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	const char* GetModelFileName(const int nModel) const;	// ���f���t�@�C���擾

private:
	// �I�[�o�[���C�h�֐�
	void Spawn(void);			// �X�|�[������
	void CollisionFind(void);	// ���m�͈͂̓����蔻��

	// �����o�֐�
	bool SetRotationCannon		// �L���m�������ݒ�
	( // ����
		const D3DXVECTOR3& rLookPos,	// ���F�Ώۈʒu
		const D3DXVECTOR3& rEnemyPos,	// �G�ʒu
		const D3DXVECTOR3& rEnemyRot	// �G����
	);

	// �ÓI�����o�ϐ�
	static const char *mc_apModelFile[];	// ���f���萔

	// �����o�ϐ�
	SPAWN m_spawn;			// �X�|�[�����
	int m_nCounterSpawn;	// �X�|�[���Ǘ��J�E���^�[
};

#endif	// _ENEMY_H_
