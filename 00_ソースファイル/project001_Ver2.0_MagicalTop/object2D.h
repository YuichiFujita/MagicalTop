//============================================================
//
//	�I�u�W�F�N�g2D�w�b�_�[ [object2D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g2D�N���X
class CObject2D : public CObject
{
public:
	// �R���X�g���N�^
	CObject2D();
	CObject2D(const CObject::LABEL label, const int nPriority = DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObject2D();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CObject2D *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rSize = VEC3_ONE,	// �傫��
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXCOLOR& rCol = XCOL_WHITE		// �F
	);

	// �����o�֐�
	void BindTexture(const int nTextureID);		// �e�N�X�`������
	void SetPosition(const D3DXVECTOR3& rPos);	// �ʒu�ݒ�
	void SetRotation(const D3DXVECTOR3& rRot);	// �����ݒ�
	void SetScaling(const D3DXVECTOR3& rSize);	// �傫���ݒ�
	void SetColor(const D3DXCOLOR& rCol);		// �F�ݒ�
	D3DXVECTOR3 GetPosition(void) const;		// �ʒu�擾
	D3DXVECTOR3 GetRotation(void) const;		// �����擾
	D3DXVECTOR3 GetScaling(void) const;			// �傫���擾
	D3DXCOLOR GetColor(void) const;				// �F�擾

protected:
	// �����o�֐�
	void SetVtx(void);	// ���_���̐ݒ�

	void SetAnimTex		// �A�j���[�V�����̃e�N�X�`�����W�̐ݒ�
	( // ����
		const int nPattern,		// �A�j���[�V�����p�^�[��
		const int nWidthPtrn,	// �e�N�X�`���̉��̕�����
		const int nHeightPtrn	// �e�N�X�`���̏c�̕�����
	);

	void SetScrollTex	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	( // ����
		const float fTexU,	// �e�N�X�`���̉����W�̊J�n�ʒu
		const float fTexV	// �e�N�X�`���̏c���W�̊J�n�ʒu
	);

private:
	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3	m_pos;	// �ʒu
	D3DXVECTOR3	m_rot;	// ����
	D3DXVECTOR3	m_size;	// �傫��
	D3DXCOLOR	m_col;	// �F
	float m_fAngle;		// �Ίp���̊p�x
	float m_fLength;	// �Ίp���̒���
	int m_nTextureID;	// �e�N�X�`���C���f�b�N�X
};

#endif	// _OBJECT2D_H_
