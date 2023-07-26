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
class CObjectGauge2D;	// �I�u�W�F�N�g�Q�[�W2D�N���X
class CShadow;			// �e�N���X

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
		MODEL_BODY = 0,	// ��
		MODEL_HEAD,		// ��
		MODEL_SLEEVE_L,	// ���r
		MODEL_SLEEVE_R,	// �E�r
		MODEL_WRIST_L,	// �����
		MODEL_WRIST_R,	// �E���
		MODEL_HAND_L,	// ����
		MODEL_HAND_R,	// �E��
		MODEL_PANTS_L,	// ����
		MODEL_PANTS_R,	// �E��
		MODEL_LEG_L,	// ����
		MODEL_LEG_R,	// �E��
		MODEL_SHOES_L,	// ���C
		MODEL_SHOES_R,	// �E�C
		MODEL_KNIFE,	// �i�C�t
		MODEL_MAX		// ���̗񋓌^�̑���
	}MODEL;

	// ���[�V������
	typedef enum
	{
		MOTION_NEUTRAL = 0,	// �ҋ@���[�V����
		MOTION_MOVE,		// ���s���[�V����
		MOTION_ACTION,		// �A�N�V�������[�V����
		MOTION_JUMP,		// �W�����v���[�V����
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
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot		// ����
	);

	// �����o�֐�
	void SetEnableUpdate(const bool bUpdate);	// �X�V�󋵐ݒ�
	void SetEnableDraw(const bool bDraw);		// �`��󋵐ݒ�
	D3DXMATRIX GetMtxWorld(void) const;			// �}�g���b�N�X�擾
	float GetDistanceTarget(void) const;		// �^�[�Q�b�g�Ƃ̋����擾
	float GetRadius(void) const;				// ���a�擾

private:
	// �����o�֐�
	MOTION Move(MOTION motion);						// �ړ�
	MOTION Jump(MOTION motion);						// �W�����v
	MOTION Magic(MOTION motion, D3DXVECTOR3& rPos);	// ���@
	MOTION Land(MOTION motion, D3DXVECTOR3& rPos);	// ���n
	void Motion(MOTION motion);						// ���[�V����
	void Pos(D3DXVECTOR3& rPos);					// �ʒu
	void Rot(D3DXVECTOR3& rRot);					// ����
	void CollisionTarget(D3DXVECTOR3& rPos);		// �^�[�Q�b�g�Ƃ̓����蔻��
	void CollisionEnemy(D3DXVECTOR3& rPos);			// �G�Ƃ̓����蔻��
	void LoadSetup(void);							// �Z�b�g�A�b�v

	// �ÓI�����o�ϐ�
	static const char *mc_apModelFile[];	// ���f���萔

	// �����o�ϐ�
	CMagicManager *m_pMagic;	// ���@�}�l�[�W���[�̏��
	CObjectGauge2D *m_pLife;	// �̗͂̏��
	CShadow		*m_pShadow;		// �e�̏��
	D3DXVECTOR3	m_oldPos;		// �ߋ��ʒu
	D3DXVECTOR3	m_move;			// �ړ���
	D3DXVECTOR3	m_destRot;		// �ڕW����
	ROTATION m_rotation;		// ��]����
	int   m_nNumModel;			// �p�[�c�̑���
	float m_fDisTarget;			// �^�[�Q�b�g�Ƃ̋���
	bool  m_bJump;				// �W�����v��
};

#endif	// _PLAYER_H_
