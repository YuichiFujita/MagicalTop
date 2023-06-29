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
#include "object.h"
#include "motion.h"
#include "magic.h"

//************************************************************
//	�O���錾
//************************************************************
class CMultiModel;	// �}���`���f���N���X
class CMotion;		// ���[�V�����N���X

//************************************************************
//	�N���X��`
//************************************************************
// �v���C���[�N���X
class CPlayer : public CObject
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

	// �R���X�g���N�^
	CPlayer();

	// �f�X�g���N�^
	~CPlayer();

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

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CPlayer *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot		// ����
	);

	// �����o�֐�
	void SetPosition(const D3DXVECTOR3& rPos);	// �ʒu�ݒ�
	void SetRotation(const D3DXVECTOR3& rRot);	// �����ݒ�
	D3DXVECTOR3 GetPosition(void) const;		// �ʒu�擾
	D3DXVECTOR3 GetRotation(void) const;		// �����擾
	float GetRadius(void) const;				// ���a�擾

private:
	// �����o�֐�
	MOTION Move(MOTION motion);		// �ړ�
	MOTION Jump(MOTION motion);		// �W�����v
	MOTION Magic(MOTION motion);	// ���@
	MOTION Land(MOTION motion);		// ���n
	void Motion(MOTION motion);		// ���[�V����
	void Collision(void);			// �����蔻��
	void Rot(void);					// ����
	void Camera(void);				// �J����
	void LoadSetup(void);			// �Z�b�g�A�b�v

	// �ÓI�����o�ϐ�
	static const char *mc_apModelFile[];	// ���f���萔

	// �����o�ϐ�
	CMultiModel	*m_apMultiModel[MAX_PARTS];	// ���f���̏��
	CMotion		*m_pMotion;	// ���[�V�����̏��
	D3DXMATRIX	m_mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3	m_pos;		// ���݈ʒu
	D3DXVECTOR3	m_oldPos;	// �ߋ��ʒu
	D3DXVECTOR3	m_move;		// �ړ���
	D3DXVECTOR3	m_rot;		// ���݌���
	D3DXVECTOR3 m_destRot;	// �ڕW����
	CMagic::TYPE m_magic;	// ���@
	int m_nNumModel;		// �p�[�c�̑���
	bool m_bJump;			// �W�����v��
};

#endif	// _PLAYER_H_
