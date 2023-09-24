//============================================================
//
//	�I�u�W�F�N�g�w�b�_�[ [object.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECT_H_
#define _OBJECT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�}�N����`
//************************************************************
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)				// ���_�t�H�[�}�b�g [2D]
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// ���_�t�H�[�}�b�g [3D]

#define MAX_PRIO		(8)	// �D�揇�ʂ̑���
#define DEFAULT_PRIO	(3)	// �f�t�H���g�̗D�揇��

//************************************************************
//	�\���̒�`
//************************************************************
// ���_��� [2D]
struct VERTEX_2D
{
	D3DXVECTOR3	pos;	// ���_���W
	float		rhw;	// ���W�ϊ��p�W�� (1.0f�ŌŒ�)
	D3DCOLOR	col;	// ���_�J���[
	D3DXVECTOR2	tex;	// �e�N�X�`�����W
};

// ���_��� [3D]
struct VERTEX_3D
{
	D3DXVECTOR3	pos;	// ���_���W
	D3DXVECTOR3	nor;	// �@���x�N�g��
	D3DCOLOR	col;	// ���_�J���[
	D3DXVECTOR2	tex;	// �e�N�X�`�����W
};

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g�N���X
class CObject
{
public:
	// �I�u�W�F�N�g���x����
	typedef enum
	{
		LABEL_NONE = 0,		// �Ȃ�
		LABEL_UI,			// UI
		LABEL_PLAYER,		// �v���C���[
		LABEL_ENEMY,		// �G
		LABEL_TARGET,		// �^�[�Q�b�g
		LABEL_MAGIC,		// ���@
		LABEL_LOCK,			// ���b�N�I��
		LABEL_GAUGE,		// �Q�[�W
		LABEL_NUMBER,		// ����
		LABEL_EXP,			// �o���l
		LABEL_BULLET,		// �e
		LABEL_FLOWER,		// �}�i�t�����[
		LABEL_WAY,			// �����\��
		LABEL_WEED,			// ��
		LABEL_WIND,			// ��
		LABEL_FIELD,		// �n��
		LABEL_WALL,			// ��
		LABEL_SCENERY,		// �i�F
		LABEL_SKY,			// ��
		LABEL_SEA,			// �C
		LABEL_EFFECT2D,		// �G�t�F�N�g
		LABEL_PARTICLE2D,	// �p�[�e�B�N��
		LABEL_EFFECT3D,		// �G�t�F�N�g
		LABEL_PARTICLE3D,	// �p�[�e�B�N��
		LABEL_MAX			// ���̗񋓌^�̑���
	}LABEL;

	// �R���X�g���N�^
	CObject();
	CObject(const LABEL label, const int nPriority = DEFAULT_PRIO);

	// �f�X�g���N�^
	virtual ~CObject();

	// �������z�֐�
	virtual HRESULT Init(void)	= 0;	// ������
	virtual void Uninit(void)	= 0;	// �I��
	virtual void Update(void)	= 0;	// �X�V
	virtual void Draw(void)		= 0;	// �`��

	// ���z�֐�
	virtual void Hit(const int nDmg);	// �q�b�g
	virtual void HitKnockBack(const int nDmg, const D3DXVECTOR3& vec);	// �m�b�N�o�b�N�q�b�g

	virtual void BindTexture(const int nTextureID);		// �e�N�X�`������
	virtual void SetPosition(const D3DXVECTOR3& rPos);	// �ʒu�ݒ�
	virtual void SetRotation(const D3DXVECTOR3& rRot);	// �����ݒ�
	virtual void SetState(const int nState);			// ��Ԑݒ�
	virtual void SetType(const int nType);				// ��ސݒ�
	virtual void SetEnableUpdate(const bool bUpdate);	// �X�V�󋵐ݒ�
	virtual void SetEnableDraw(const bool bDraw);		// �`��󋵐ݒ�
	virtual void SetPriority(const int nPriority);		// �D�揇�ʐݒ�
	virtual D3DXMATRIX *GetPtrMtxWorld(void);			// �}�g���b�N�X�|�C���^�擾
	virtual D3DXMATRIX GetMtxWorld(void) const;			// �}�g���b�N�X�擾
	virtual D3DXVECTOR3 GetPosition(void) const;		// �ʒu�擾
	virtual D3DXVECTOR3 GetRotation(void) const;		// �����擾
	virtual D3DXVECTOR3 GetScaling(void) const;			// �傫���擾
	virtual int GetState(void) const;					// ��Ԏ擾
	virtual int GetType(void) const;					// ��ގ擾
	virtual float GetRadius(void) const;				// ���a�擾
	virtual float GetHeight(void) const;				// �c���擾

	// �ÓI�����o�֐�
	static void ReleaseAll(void);	// �S�j��
	static void UpdateAll(void);	// �S�X�V
	static void DrawAll(void);		// �S�`��
	static int GetNumAll(void);		// �����擾
	static bool CheckUse(const CObject *pObject);	// �g�p�m�F
	static CObject *GetTop(const int nPriority);	// �擪�I�u�W�F�N�g�擾
	static CObject *GetCur(const int nPriority);	// �Ō���I�u�W�F�N�g�擾

	// �����o�֐�
	void	SetLabel(const LABEL label);	// ���x���ݒ�
	LABEL	GetLabel(void) const;			// ���x���擾
	int		GetPriority(void) const;		// �D�揇�ʎ擾

	DWORD	GetID(void) const;		// ���j�[�NID�擾
	bool	IsUpdate(void) const;	// �X�V�󋵎擾
	bool	IsDraw(void) const;		// �`��󋵎擾
	bool	IsDeath(void) const;	// ���S�t���O�擾
	CObject	*GetObject(void);		// �I�u�W�F�N�g�擾
	CObject	*GetPrev(void) const;	// �O�I�u�W�F�N�g�擾
	CObject	*GetNext(void) const;	// ���I�u�W�F�N�g�擾

protected:
	// �����o�֐�
	void Release(void);	// �j��

private:
	// �ÓI�����o�֐�
	static void DeathAll(void);	// �S���S

	// �ÓI�����o�ϐ�
	static CObject *m_apTop[MAX_PRIO];	// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CObject *m_apCur[MAX_PRIO];	// �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
	static DWORD m_dwNextID;			// ���̃��j�[�NID
	static int m_nNumAll;				// �I�u�W�F�N�g�̑���

	// �����o�ϐ�
	LABEL m_label;		// ���g�̃I�u�W�F�N�g���x��
	DWORD m_dwID;		// ���g�̃��j�[�NID
	int m_nPriority;	// ���g�̗D�揇��
	bool m_bUpdate;		// ���g�̍X�V��
	bool m_bDraw;		// ���g�̕`���
	bool m_bDeath;		// ���g�̎��S�t���O
	CObject *m_pPrev;	// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject *m_pNext;	// ���̃I�u�W�F�N�g�ւ̃|�C���^
};

#endif	// _OBJECT_H_
