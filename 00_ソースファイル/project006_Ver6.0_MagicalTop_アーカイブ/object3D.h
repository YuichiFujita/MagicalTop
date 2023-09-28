//============================================================
//
//	�I�u�W�F�N�g3D�w�b�_�[ [object3D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g3D�N���X
class CObject3D : public CObject
{
public:
	// ���_��
	typedef enum
	{
		ORIGIN_CENTER = 0,	// ���S���_
		ORIGIN_DOWN,		// �����_
		ORIGIN_MAX,			// ���̗񋓌^�̑���
	}ORIGIN;

	// �R���X�g���N�^
	CObject3D();
	CObject3D(const CObject::LABEL label, const int nPriority = DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObject3D();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CObject3D *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rSize,	// �傫��
		const D3DXVECTOR3& rRot = VEC3_ZERO,		// ����
		const D3DXCOLOR& rCol = XCOL_WHITE,			// �F
		const ORIGIN origin = ORIGIN_CENTER,		// ���_
		const D3DCULL cull = D3DCULL_CCW,			// �J�����O��
		const bool bLight = true,					// ���C�e�B���O��
		const D3DCMPFUNC func = D3DCMP_LESSEQUAL,	// Z�e�X�g�ݒ�
		const bool bZEnable = true					// Z�o�b�t�@�̎g�p��
	);

	// �����o�֐�
	void BindTexture(const int nTextureID);		// �e�N�X�`������
	void SetPosition(const D3DXVECTOR3& rPos);	// �ʒu�ݒ�
	void SetRotation(const D3DXVECTOR3& rRot);	// �����ݒ�
	void SetScaling(const D3DXVECTOR3& rSize);	// �傫���ݒ�
	void SetColor(const D3DXCOLOR& rCol);		// �F�ݒ�
	void SetOrigin(const ORIGIN origin);		// ���_�ݒ�
	void SetCulling(const D3DCULL cull);		// �J�����O�ݒ�
	void SetLighting(const bool bLight);		// ���C�e�B���O�ݒ�
	void SetFunc(const D3DCMPFUNC func);		// Z�e�X�g�ݒ�
	void SetZEnable(const bool bEnable);		// Z�o�b�t�@�̎g�p�󋵐ݒ�
	void SetVertexPosition(const int nID, const D3DXVECTOR3& rPos);	// ���_�ʒu�ݒ�
	void SetGapPosition(const int nID, const D3DXVECTOR3& rPos);	// ���W�̂���ݒ�

	D3DXVECTOR3 GetPosition(void) const;	// �ʒu�擾
	D3DXVECTOR3 GetRotation(void) const;	// �����擾
	D3DXVECTOR3 GetScaling(void) const;		// �傫���擾
	D3DXCOLOR GetColor(void) const;			// �F�擾
	ORIGIN GetOrigin(void) const;			// ���_�擾
	D3DCULL GetCulling(void) const;			// �J�����O�擾
	bool GetLighting(void) const;			// ���C�e�B���O�擾
	D3DCMPFUNC GetFunc(void) const;			// Z�e�X�g�擾
	bool GetZEnable(void) const;			// Z�o�b�t�@�̎g�p�󋵎擾
	D3DXVECTOR3 GetVertexPosition(const int nID);		// ���_�ʒu�擾
	D3DXVECTOR3 GetGapPosition(const int nID);			// ���W�̂���擾
	float GetPositionHeight(const D3DXVECTOR3&rPos);	// �|���S���̒��n�擾

protected:
	// �����o�֐�
	void SetVtx(void);	// ���_���̐ݒ�

private:
	// �����o�֐�
	void NormalizeNormal(void);	// �@���̐��K��

	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 *m_pPosGapBuff;			// ���W�̂���o�b�t�@�ւ̃|�C���^
	D3DXMATRIX  m_mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3	m_pos;		// �ʒu
	D3DXVECTOR3	m_rot;		// ����
	D3DXVECTOR3	m_size;		// �傫��
	D3DXCOLOR	m_col;		// �F
	ORIGIN		m_origin;	// ���_
	D3DCMPFUNC	m_func;		// Z�e�X�g�ݒ�
	bool m_bZEnable;		// Z�o�b�t�@�̎g�p��
	D3DCULL m_cull;			// �J�����O��
	bool m_bLight;			// ���C�e�B���O��
	int m_nTextureID;		// �e�N�X�`���C���f�b�N�X
};

#endif	// _OBJECT3D_H_
