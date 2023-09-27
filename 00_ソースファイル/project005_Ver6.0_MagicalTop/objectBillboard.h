//============================================================
//
//	�I�u�W�F�N�g�r���{�[�h�w�b�_�[ [objectBillboard.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECTBILLBOARD_H_
#define _OBJECTBILLBOARD_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g�r���{�[�h�N���X
class CObjectBillboard : public CObject
{
public:
	// ���_��
	typedef enum
	{
		ORIGIN_CENTER = 0,	// ���S���_
		ORIGIN_DOWN,		// �����_
		ORIGIN_MAX,			// ���̗񋓌^�̑���
	}ORIGIN;

	// ��]��
	typedef enum
	{
		ROTATE_NORMAL = 0,	// �ʏ��]
		ROTATE_LATERAL,		// ����]
		ROTATE_MAX,			// ���̗񋓌^�̑���
	}ROTATE;

	// �R���X�g���N�^
	CObjectBillboard();
	CObjectBillboard(const CObject::LABEL label, const int nPriority = DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObjectBillboard();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CObjectBillboard *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rSize,	// �傫��
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXCOLOR& rCol = XCOL_WHITE,		// �F
		const ORIGIN origin = ORIGIN_CENTER,	// ���_
		const ROTATE rotate = ROTATE_NORMAL,	// ��]
		const D3DCMPFUNC func = D3DCMP_ALWAYS,	// Z�e�X�g�ݒ�
		const bool bZEnable = false				// Z�o�b�t�@�̎g�p��
	);

	// �����o�֐�
	void BindTexture(const int nTextureID);		// �e�N�X�`������
	void SetPosition(const D3DXVECTOR3& rPos);	// �ʒu�ݒ�
	void SetRotation(const D3DXVECTOR3& rRot);	// �����ݒ�
	void SetScaling(const D3DXVECTOR3& rSize);	// �傫���ݒ�
	void SetColor(const D3DXCOLOR& rCol);		// �F�ݒ�
	void SetOrigin(const ORIGIN origin);		// ���_�ݒ�
	void SetRotate(const ROTATE rotate);		// ��]�ݒ�
	void SetFunc(const D3DCMPFUNC func);		// Z�e�X�g�ݒ�
	void SetZEnable(const bool bEnable);		// Z�o�b�t�@�̎g�p�󋵐ݒ�

	D3DXMATRIX *GetPtrMtxWorld(void);		// �}�g���b�N�X�|�C���^�擾
	D3DXMATRIX GetMtxWorld(void) const;		// �}�g���b�N�X�擾
	D3DXVECTOR3 GetPosition(void) const;	// �ʒu�擾
	D3DXVECTOR3 GetRotation(void) const;	// �����擾
	D3DXVECTOR3 GetScaling(void) const;		// �傫���擾
	D3DXCOLOR GetColor(void) const;			// �F�擾
	ORIGIN GetOrigin(void) const;			// ���_�擾
	ROTATE GetRotate(void) const;			// ��]�擾
	D3DCMPFUNC GetFunc(void) const;			// Z�e�X�g�擾
	bool GetZEnable(void) const;			// Z�o�b�t�@�̎g�p�󋵎擾

protected:
	// �����o�֐�
	void SetVtx(void);	// ���_���̐ݒ�

private:
	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX  m_mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3	m_pos;		// �ʒu
	D3DXVECTOR3	m_rot;		// ����
	D3DXVECTOR3	m_size;		// �傫��
	D3DXCOLOR	m_col;		// �F
	ORIGIN		m_origin;	// ���_
	ROTATE		m_rotate;	// ��]
	D3DCMPFUNC	m_func;		// Z�e�X�g�ݒ�
	bool m_bZEnable;		// Z�o�b�t�@�̎g�p��
	float m_fAngle;			// �Ίp���̊p�x
	float m_fLength;		// �Ίp���̒���
	int m_nTextureID;		// �e�N�X�`���C���f�b�N�X
};

#endif	// _OBJECTBILLBOARD_H_
