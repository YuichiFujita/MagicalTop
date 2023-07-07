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
//	�N���X��`
//************************************************************
// ���@�N���X
class CMagic : public CObject
{
public:
	// ��ޗ�
	typedef enum
	{
		TYPE_LV0_NORMAL = 0,	// �������@
		TYPE_LV1_FIRE,			// �����@
		TYPE_LV1_WATER,			// �����@
		TYPE_LV1_WIND,			// �����@
		TYPE_MAX				// ���̗񋓌^�̑���
	}TYPE;

	// �R���X�g���N�^
	CMagic(const TYPE type);

	// �f�X�g���N�^
	~CMagic();

	// �X�e�[�^�X�\����
	typedef struct
	{
		int		nLock;			// ���b�N�I����
		int		nLife;			// ����
		int		nDamage;		// �U����
		float	fRadius;		// ���a
		float	fHeight;		// �c��
		float	fMove;			// �ړ���
		float	fViewRadius;	// ���E�͈�
		float	fViewAngle;		// ����p
		bool	bHoming;		// �z�[�~���O�� ON/OFF
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
		const TYPE type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR3& rVec		// �ړ�����
	);
	static StatusInfo GetStatusInfo(const TYPE type);	// �X�e�[�^�X���擾

	// �����o�֐�
	void SetMove(D3DXVECTOR3 vec, const float fMove);	// �ړ��ʐݒ�
	void SetLife(const int nLife);						// �����ݒ�
	void SetPosition(const D3DXVECTOR3& rPos);			// �ʒu�ݒ�
	void SetRotation(const D3DXVECTOR3& rRot);			// �����ݒ�
	D3DXVECTOR3 GetPosition(void) const;				// �ʒu�擾
	D3DXVECTOR3 GetRotation(void) const;				// �����擾
	StatusInfo GetStatusInfo(void) const;				// �X�e�[�^�X���擾

	// ���z�֐�
	virtual bool Collision(CObject *pObject);	// ���@����

private:
	// �����o�֐�
	bool CollisionEnemy(void);	// �G�Ƃ̓����蔻��

	// �ÓI�����o�ϐ�
	static StatusInfo m_aStatusInfo[TYPE_MAX];	// �X�e�[�^�X���

	// �����o�ϐ�
	const StatusInfo m_status;	// �X�e�[�^�X�萔
	D3DXMATRIX	m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3	m_pos;			// ���݈ʒu
	D3DXVECTOR3	m_movePos;		// �ʒu�ړ���
	D3DXVECTOR3	m_rot;			// ����
	D3DXVECTOR3	m_moveRot;		// �����ύX��
	int m_nLife;				// ����
};

// �������@�N���X
class CNormalMagic : public CMagic
{
public:
	// �R���X�g���N�^
	CNormalMagic(const TYPE type);

	// �f�X�g���N�^
	~CNormalMagic();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
};

#endif	// _MAGIC_H_
