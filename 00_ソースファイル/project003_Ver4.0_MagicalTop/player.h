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
#include "magic.h"

//************************************************************
//	�}�N����`
//************************************************************
#define PLAY_MAGIC_POS_PLUS_Y	(40.0f)	// �v���C���[�̖��@���ˎ��̍������Z��

//************************************************************
//	�O���錾
//************************************************************
class CMagicManager;	// ���@�}�l�[�W���[�N���X
class CExpManager;		// �o���l�}�l�[�W���[�N���X
class CLevelupManager;	// �����}�l�[�W���[�N���X
class CObjectGauge2D;	// �I�u�W�F�N�g�Q�[�W2D�N���X
class CShadow;			// �e�N���X
class CObjectOrbit;		// �I�u�W�F�N�g�O�ՃN���X

//************************************************************
//	�N���X��`
//************************************************************
// �v���C���[�N���X
class CPlayer : public CObjectChara
{
public:
	// ��ޗ�
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
		MOTION_MOVE = 0,	// ���s���[�V����
		MOTION_ACTION,		// �A�N�V�������[�V����
		MOTION_BLOW_AWAY,	// ������у��[�V����
		MOTION_LANDING,		// ���n���[�V����
		MOTION_MAX			// ���̗񋓌^�̑���
	}MOTION;

	// ��]��
	typedef enum
	{
		ROTATION_LEFT = 0,	// ����]
		ROTATION_RIGHT,		// �E��]
		ROTATION_MAX		// ���̗񋓌^�̑���
	}ROTATION;

	// ��ԗ�
	typedef enum
	{
		STATE_NONE = 0,		// �������Ȃ����
		STATE_FADEOUT,		// �t�F�[�h�A�E�g���
		STATE_NORMAL,		// �ʏ���
		STATE_DAMAGE,		// �_���[�W���
		STATE_BLOW_AWAY,	// ������я��
		STATE_FADEIN,		// �t�F�[�h�C�����
		STATE_DEATH,		// ���S���
		STATE_MAX			// ���̗񋓌^�̑���
	}STATE;

	// �R���X�g���N�^
	CPlayer();

	// �f�X�g���N�^
	~CPlayer();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	void Hit(const int nDmg);	// �q�b�g

	// �ÓI�����o�֐�
	static CPlayer *Create	// ����
	( // ����
		D3DXVECTOR3& rPos,		// �ʒu
		const D3DXVECTOR3& rRot	// ����
	);

	// �����o�֐�
	void AddExp(const int nAdd);				// �o���l���Z
	void SetRespawn(D3DXVECTOR3& rPos);			// �ďo���ݒ�
	void SetDisp(const bool bDisp);				// �\���ݒ�
	void SetEnableUpdate(const bool bUpdate);	// �X�V�󋵐ݒ�
	void SetEnableDraw(const bool bDraw);		// �`��󋵐ݒ�
	D3DXMATRIX GetMtxWorld(void) const;			// �}�g���b�N�X�擾
	float GetDistanceTarget(void) const;		// �^�[�Q�b�g�Ƃ̋����擾
	int GetLevel(void) const;					// ���x���擾
	int GetState(void) const;					// ��Ԏ擾
	float GetRadius(void) const;				// ���a�擾

private:
	// �����o�֐�
	MOTION UpdateNormal(void);	// �ʏ��Ԏ��̍X�V
	MOTION UpdateDamage(void);	// �_���[�W��Ԏ��̍X�V
	void UpdateBlowAway(void);	// ������я�Ԏ��̍X�V
	void UpdateFadeOut(void);	// �t�F�[�h�A�E�g��Ԏ��̍X�V
	void UpdateFadeIn(void);	// �t�F�[�h�C����Ԏ��̍X�V

	MOTION Move(void);								// �ړ�
	MOTION Magic(MOTION motion, D3DXVECTOR3& rPos);	// ���@
	bool Land(D3DXVECTOR3& rPos);					// ���n
	void Motion(int nMotion);						// ���[�V����
	void Pos(D3DXVECTOR3& rPos);					// �ʒu
	void Rot(D3DXVECTOR3& rRot);					// ����
	void CollisionTarget(D3DXVECTOR3& rPos);		// �^�[�Q�b�g�Ƃ̓����蔻��
	void CollisionEnemy(D3DXVECTOR3& rPos);			// �G�Ƃ̓����蔻��
	void LoadSetup(void);							// �Z�b�g�A�b�v

	// �ÓI�����o�ϐ�
	static const char *mc_apModelFile[];	// ���f���萔

	// �����o�ϐ�
	CMagicManager	*m_pMagic;		// ���@�}�l�[�W���[�̏��
	CExpManager		*m_pExp;		// �o���l�}�l�[�W���[�̏��
	CLevelupManager	*m_pLevelup;	// �����}�l�[�W���[�̏��
	CObjectGauge2D	*m_pLife;		// �̗͂̏��
	CShadow			*m_pShadow;		// �e�̏��
	CObjectOrbit	*m_pOrbit;		// �O�Ղ̏��

	D3DXVECTOR3	m_oldPos;	// �ߋ��ʒu
	D3DXVECTOR3	m_move;		// �ړ���
	D3DXVECTOR3	m_destRot;	// �ڕW����
	ROTATION m_rotation;	// ��]����
	STATE m_state;			// ���
	int   m_nCounterState;	// ��ԊǗ��J�E���^�[
	int   m_nNumModel;		// �p�[�c�̑���
	float m_fDisTarget;		// �^�[�Q�b�g�Ƃ̋���
	bool  m_bJump;			// �W�����v��
};

#endif	// _PLAYER_H_
