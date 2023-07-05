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
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXCOLOR& rCol = XCOL_WHITE		// �F
	);

	// �����o�֐�
	void BindTexture(const int nTextureID);		// �e�N�X�`������
	void SetPosition(const D3DXVECTOR3& rPos);	// �ʒu�ݒ�
	void SetRotation(const D3DXVECTOR3& rRot);	// �����ݒ�
	void SetScaling(const D3DXVECTOR3& rSize);	// �傫���ݒ�
	void SetColor(const D3DXCOLOR& rCol);		// �F�ݒ�
	void SetVertexPosition(const int nID, const D3DXVECTOR3& rPos);	// ���_�ʒu�ݒ�

	D3DXVECTOR3 GetPosition(void) const;	// �ʒu�擾
	D3DXVECTOR3 GetRotation(void) const;	// �����擾
	D3DXVECTOR3 GetScaling(void) const;		// �傫���擾
	D3DXCOLOR GetColor(void) const;			// �F�擾
	D3DXVECTOR3 GetVertexPosition(const int nID);		// ���_�ʒu�擾
	float GetPositionHeight(const D3DXVECTOR3&rPos);	// �|���S���̒��n�擾

protected:
	// �����o�֐�
	void SetVtx(void);	// ���_���̐ݒ�

private:
	// �����o�֐�
	void NormalizeNormal(void);	// �@���̐��K��

	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX  m_mtxWorld;				// ���[���h�}�g���b�N�X
	D3DXVECTOR3	m_pos;	// �ʒu
	D3DXVECTOR3	m_rot;	// ����
	D3DXVECTOR3	m_size;	// �傫��
	D3DXCOLOR	m_col;	// �F
	int m_nTextureID;	// �e�N�X�`���C���f�b�N�X
};

#endif	// _OBJECT3D_H_
