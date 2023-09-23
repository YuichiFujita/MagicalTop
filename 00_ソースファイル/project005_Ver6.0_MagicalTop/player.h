//============================================================
//
//	�v���C���[�w�b�_�[ [player.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectChara.h"
#include "scene.h"
#include "magic.h"

//************************************************************
//	�}�N����`
//************************************************************
#define PLAY_SPAWN_POS	(D3DXVECTOR3(0.0f, 0.0f, -1000.0f))	// �v���C���[�����ʒu

//************************************************************
//	�O���錾
//************************************************************
class CMagicManager;	// ���@�}�l�[�W���[�N���X
class CExpManager;		// �o���l�}�l�[�W���[�N���X
class CObjectGauge3D;	// �I�u�W�F�N�g�Q�[�W3D�N���X
class CGaugeStar;		// ���Q�[�W�N���X
class CShadow;			// �e�N���X
class CObjectOrbit;		// �I�u�W�F�N�g�O�ՃN���X

//************************************************************
//	�N���X��`
//************************************************************
// �v���C���[�N���X
class CPlayer : public CObjectChara
{
public:
	// �e�N�X�`����
	typedef enum
	{
		TEXTURE_ORBIT = 0,	// �O�Ճe�N�X�`��
		TEXTURE_MAX,		// ���̗񋓌^�̑���
	}TEXTURE;

	// ���f����
	typedef enum
	{
		MODEL_WAIST,	// ��
		MODEL_BODY,		// ��
		MODEL_CLOAK,	// �}���g
		MODEL_HEAD,		// ��
		MODEL_HAT,		// �X�q
		MODEL_ARMUL,	// ����r
		MODEL_ARMUR,	// �E��r
		MODEL_ARMDL,	// �����r
		MODEL_ARMDR,	// �E���r
		MODEL_HANDL,	// ����
		MODEL_HANDR,	// �E��
		MODEL_LEGUL,	// ��������
		MODEL_LEGUR,	// �E������
		MODEL_LEGDL,	// ����
		MODEL_LEGDR,	// �E��
		MODEL_FOOTL,	// ����
		MODEL_FOOTR,	// �E��
		MODEL_ROD,		// ��
		MODEL_MAX		// ���̗񋓌^�̑���
	}MODEL;

	// ���[�V������
	typedef enum
	{
		MOTION_MOVE = 0,		// ���V���[�V����
		MOTION_ACCELE,			// �������[�V����
		MOTION_DECELE,			// �������[�V����
		MOTION_ACTION_LEFT,		// ���A�N�V�������[�V����
		MOTION_ACTION_RIGHT,	// �E�A�N�V�������[�V����
		MOTION_ACTION_FAR,		// ���A�N�V�������[�V����
		MOTION_ACTION_NEAR,		// ��O�A�N�V�������[�V����
		MOTION_BLOW_AWAY,		// ������у��[�V����
		MOTION_DEATH,			// ���S���[�V����
		MOTION_MAX				// ���̗񋓌^�̑���
	}MOTION;

	// ��ԗ�
	typedef enum
	{
		STATE_NONE = 0,		// �������Ȃ����
		STATE_FADEOUT,		// �t�F�[�h�A�E�g���
		STATE_NORMAL,		// �ʏ���
		STATE_DAMAGE,		// �_���[�W���
		STATE_INVULN,		// ���G���
		STATE_BLOW_AWAY,	// ������я��
		STATE_VORTEX,		// �Q�������܂���
		STATE_FADEIN,		// �t�F�[�h�C�����
		STATE_DEATH,		// ���S���
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
		LEVELINFO_LIFE = 0,	// �̗�
		LEVELINFO_MANA,		// �}�i
		LEVELINFO_DASH,		// �_�b�V��
		LEVELINFO_DEFENSE,	// �h���
		LEVELINFO_SPEED,	// �f����
		LEVELINFO_EXP_UP,	// �o���l�{�[�i�X
		LEVELINFO_MAX		// ���̗񋓌^�̑���
	}LEVELINFO;

	// �R���X�g���N�^
	CPlayer();

	// �f�X�g���N�^
	~CPlayer();

	// �f�����\����
	typedef struct
	{
		float fLeftside;	// �����ւ̉�����
		float fOutside;		// �����ւ̉�����
		float fInside;		// �O���ւ̉�����
	}Speed;

	// �X�e�[�^�X�\����
	typedef struct
	{
		int nLife;		// �̗�
		int nMana;		// �}�i
		int nDash;		// �_�b�V��
		int nDefense;	// �h���
		Speed speed;	// �f����
		float fMulExp;	// �o���l�{�[�i�X
	}StatusInfo;

	// ���x���\����
	typedef struct
	{
		int nLife;		// �̗�
		int nMana;		// �}�i
		int nDash;		// �_�b�V��
		int nDefense;	// �h���
		int nSpeed;		// �f����
		int nMulExp;	// �o���l�{�[�i�X
	}LevelInfo;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	void Hit(const int nDmg);	// �q�b�g

	// �ÓI�����o�֐�
	static CPlayer *Create	// ����
	( // ����
		const CScene::MODE mode,	// ���[�h
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot		// ����
	);
	static StatusInfo GetStatusInfo(const LEVEL level);	// �X�e�[�^�X���擾

	// �������z�֐�
	virtual void HitBlowAway	// ������уq�b�g
	( // ����
		const D3DXVECTOR3& rPlayerPos,	// �v���C���[�ʒu
		const D3DXVECTOR3& rHitObjPos,	// �I�u�W�F�N�g�ʒu
		const int nDmg					// �_���[�W��
	) = 0;
	virtual void HitVortex	// �Q�������܂�q�b�g
	( // ����
		D3DXVECTOR3& rPlayerPos,	// �v���C���[�ʒu
		const D3DXVECTOR3& rHitPos,	// �����蔻��ʒu
		const int nDmg				// �_���[�W��
	) = 0;
	virtual void SetDisp(const bool bDisp) = 0;	// �\���ݒ�

	// �����o�֐�
	void AddLevelStatus(const LEVELINFO level);			// �X�e�[�^�X���x�����Z
	void AddLevel(const int nAdd);						// ���x�����Z
	void AddExp(const int nAdd);						// �o���l���Z
	void AddLife(const int nAdd);						// �̗͉��Z
	void SetRespawn(D3DXVECTOR3& rPos);					// �ďo���ݒ�
	void SetMana(const int nMana);						// �}�i�ݒ�
	void SetState(const int nState);					// ��Ԑݒ�
	void SetEnableUpdate(const bool bUpdate);			// �X�V�󋵐ݒ�
	void SetEnableDraw(const bool bDraw);				// �`��󋵐ݒ�
	void SetEnableDrawMana(const bool bDraw);			// �}�i�̕`��󋵐ݒ�
	void SetEnableDrawExp(const bool bDraw);			// �o���l�̕`��󋵐ݒ�
	void SetEnableDrawLife(const bool bDraw);			// �̗͂̕`��󋵐ݒ�
	void SetEnableDrawDash(const bool bDraw);			// �_�b�V���̕`��󋵐ݒ�
	void SetEnableHealMana(const bool bHeal);			// �}�i�񕜏󋵐ݒ�
	D3DXMATRIX GetMtxWorld(void) const;					// �}�g���b�N�X�擾
	float GetDistanceTarget(void) const;				// �^�[�Q�b�g�Ƃ̋����擾
	int GetLevelStatus(const LEVELINFO level) const;	// �X�e�[�^�X���x���擾
	int GetLevel(void) const;		// ���x���擾
	int GetLife(void) const;		// �̗͎擾
	int GetMaxLife(void) const;		// �ő�̗͎擾
	int GetMana(void) const;		// �}�i�擾
	int GetMaxMana(void) const;		// �ő�}�i�擾
	int GetState(void) const;		// ��Ԏ擾
	float GetRadius(void) const;	// ���a�擾

protected:
	// �������z�֐�
	virtual MOTION UpdateNormal(void) = 0;	// �ʏ��Ԏ��̍X�V

	// �����o�֐�
	void SetBlowAway(const D3DXVECTOR3& rVecAway);	// ������ѐݒ�
	void SetVortex(const D3DXVECTOR3& rPlayerPos, const D3DXVECTOR3& rHitPos);	// �Q�������܂�ݒ�

	void UpdateOldPosition(void);	// �ߋ��ʒu�̍X�V
	void UpdateDisTarget(void);		// �^�[�Q�b�g�����̍X�V
	void UpdateMotion(int nMotion);	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V

	void UpdateAbsorb(D3DXVECTOR3& rVecTarg, D3DXVECTOR3& rVecSide);			// �z�����݂̍X�V
	MOTION ControlTargAccel(const D3DXVECTOR3& rVecTarg, bool *pMove = NULL);	// �^�[�Q�b�g�t�����ւ̉������̑���
	MOTION ControlSideAccel(const D3DXVECTOR3& rVecSide, bool *pMove = NULL);	// �^�[�Q�b�g�������ւ̉������̑���
	MOTION ControlShotMagic(MOTION motion, bool *pShot = NULL);					// ���@�U���̑���
	bool UpdateLanding(D3DXVECTOR3& rPos);		// ���n�󋵂̍X�V
	void UpdatePosition(D3DXVECTOR3& rPos);		// �ʒu�̍X�V
	void UpdateRotation(D3DXVECTOR3& rRot);		// �����̍X�V
	void CollisionTarget(D3DXVECTOR3& rPos);	// �^�[�Q�b�g�Ƃ̓����蔻��
	void CollisionEnemy(D3DXVECTOR3& rPos);		// �G�Ƃ̓����蔻��

	MOTION UpdateDamage(void);	// �_���[�W��Ԏ��̍X�V
	MOTION UpdateInvuln(void);	// ���G��Ԏ��̍X�V
	void UpdateBlowAway(void);	// ������я�Ԏ��̍X�V
	void UpdateVortex(void);	// �Q�������܂��Ԏ��̍X�V
	void UpdateFadeOut(void);	// �t�F�[�h�A�E�g��Ԏ��̍X�V
	void UpdateFadeIn(void);	// �t�F�[�h�C����Ԏ��̍X�V

private:
	// �����o�֐�
	void LoadSetup(void);	// �Z�b�g�A�b�v

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];		// �e�N�X�`���萔
	static const char *mc_apModelFile[];		// ���f���萔
	static StatusInfo m_aStatusInfo[LEVEL_MAX];	// �X�e�[�^�X���

	// �����o�ϐ�
	CMagicManager	*m_pMagic;		// ���@�}�l�[�W���[�̏��
	CExpManager		*m_pExp;		// �o���l�}�l�[�W���[�̏��
	CObjectGauge3D	*m_pLife;		// �̗͂̏��
	CGaugeStar		*m_pDash;		// �_�b�V���̏��
	CShadow			*m_pShadow;		// �e�̏��
	CObjectOrbit	*m_pOrbit;		// �O�Ղ̏��

	D3DXVECTOR3	m_oldPos;	// �ߋ��ʒu
	D3DXVECTOR3	m_move;		// �ړ���
	D3DXVECTOR3	m_destRot;	// �ڕW����
	LevelInfo m_level;		// ���x��
	STATE m_state;			// ���
	int   m_nCounterState;	// ��ԊǗ��J�E���^�[
	int   m_nNumModel;		// �p�[�c�̑���
	float m_fDisTarget;		// �^�[�Q�b�g�Ƃ̋���
	float m_fVortexRot;		// �Q��������
	float m_fVortexDis;		// �Q���Ƃ̋���
	bool  m_bJump;			// �W�����v��
};

#endif	// _PLAYER_H_
