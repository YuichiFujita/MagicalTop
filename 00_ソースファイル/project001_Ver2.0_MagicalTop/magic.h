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
class CObjectTornado;	// �I�u�W�F�N�g�����N���X

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
		TYPE_LV1_FIRE,			// LV1�����@
		TYPE_LV1_WATER,			// LV1�����@
		TYPE_LV1_WIND,			// LV1�����@
		TYPE_LV2_FIRE,			// LV2�����@
		TYPE_LV2_WATER,			// LV2�����@
		TYPE_LV2_WIND,			// LV2�����@
		TYPE_MAX				// ���̗񋓌^�̑���
	}TYPE;

	// �R���X�g���N�^
	CMagic(const TYPE type);

	// �f�X�g���N�^
	~CMagic();

	// �X�e�[�^�X�\����
	typedef struct
	{
		D3DXVECTOR3 shotPos;	// ���ˈʒu
		int		nShotParts;		// ���˃p�[�c
		int		nLock;			// ���b�N�I����
		int		nLife;			// ����
		int		nDamage;		// �U����
		float	fRadius;		// ���a
		float	fHeight;		// �c��
		float	fMove;			// �ړ���
		float	fViewRadius;	// ���E�͈�
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

// LV2�����@�N���X
class CLv2WindMagic : public CMagic
{
public:
	// �R���X�g���N�^
	CLv2WindMagic(const TYPE type);

	// �f�X�g���N�^
	~CLv2WindMagic();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	bool Collision(CObject *pObject);	// ���@����

private:
	// �����o�ϐ�
	CObjectTornado *m_pTornado;	// �����I�u�W�F�N�g
};

#endif	// _MAGIC_H_
