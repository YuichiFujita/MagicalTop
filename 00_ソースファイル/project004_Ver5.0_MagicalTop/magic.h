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
		STATE_INHALE,		// �z�����܂���
		STATE_DELETE,		// �������
		STATE_MAX			// ���̗񋓌^�̑���
	}STATE;

	// �R���X�g���N�^
	CMagic();

	// �f�X�g���N�^
	~CMagic();

	// �X�e�[�^�X�\����
	typedef struct
	{
		D3DXVECTOR3 shotPos;	// ���ˈʒu
		int		nShotParts;		// ���˃p�[�c
		int		nLife;			// ����
		int		nCoolTime;		// �N�[���^�C��
		float	fMove;			// �ړ���
		float	bubbleRadius;	// �o�u�����a
	}StatusInfo;

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
	static StatusInfo GetStatusInfo(void);	// �X�e�[�^�X���擾

	// �����o�֐�
	void SetMove(D3DXVECTOR3 vec, const float fMove);	// �ړ��ʐݒ�
	void SetPosition(const D3DXVECTOR3& rPos);	// �ʒu�ݒ�
	void SetRotation(const D3DXVECTOR3& rRot);	// �����ݒ�
	D3DXVECTOR3 GetPosition(void) const;	// �ʒu�擾
	D3DXVECTOR3 GetRotation(void) const;	// �����擾

private:
	// �����o�֐�
	bool CollisionEnemy(void);	// �G�Ƃ̓����蔻��

	// �ÓI�����o�ϐ�
	static StatusInfo m_statusInfo;	// �X�e�[�^�X���

	// �����o�ϐ�
	CBubble *m_pBubble;		// �o�u�����
	CShadow *m_pShadow;		// �e�̏��
	D3DXMATRIX	m_mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3	m_pos;		// ���݈ʒu
	D3DXVECTOR3	m_movePos;	// �ʒu�ړ���
	D3DXVECTOR3	m_rot;		// ����
	D3DXVECTOR3	m_moveRot;	// �����ύX��
	STATE m_state;			// ���
};

#endif	// _MAGIC_H_
