//============================================================
//
//	�I�u�W�F�N�g���b�V���V�����_�[�w�b�_�[ [objectMeshCylinder.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECT_MESHCYLINDER_H_
#define _OBJECT_MESHCYLINDER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g���b�V���V�����_�[�N���X
class CObjectMeshCylinder : public CObject
{
public:
	// �R���X�g���N�^
	CObjectMeshCylinder();
	CObjectMeshCylinder(const CObject::LABEL label, const int nPriority = DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObjectMeshCylinder();

	// ���b�V���V�����_�[�\����
	typedef struct
	{
		D3DXVECTOR3	pos;		// �ʒu
		D3DXVECTOR3	rot;		// ����
		D3DXCOLOR	col;		// �F
		D3DXMATRIX	mtxWorld;	// ���[���h�}�g���b�N�X
		float fRadius;	// ���a
		float fHeight;	// �c��
		D3DCULL cull;	// �J�����O��
		bool bLight;	// ���C�e�B���O��
	}MeshCylinder;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CObjectMeshCylinder *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXCOLOR& rCol,		// �F
		const POSGRID2& rPart,		// ������
		const float fRadius,		// ���a
		const float fHeight,		// �c��
		const D3DCULL cull = D3DCULL_CCW,	// �J�����O��
		const bool bLight = true			// ���C�e�B���O��
	);

	// �����o�֐�
	void BindTexture(const int nTextureID);		// �e�N�X�`������
	void SetPosition(const D3DXVECTOR3& rPos);	// �ʒu�ݒ�
	void SetRotation(const D3DXVECTOR3& rRot);	// �����ݒ�
	void SetColor(const D3DXCOLOR& rCol);		// �F�ݒ�
	void SetRadius(const float fRadius);		// ���a�ݒ�
	void SetHeight(const float fHeight);		// �c���ݒ�
	void SetCulling(const D3DCULL cull);		// �J�����O�ݒ�
	void SetLighting(const bool bLight);		// ���C�e�B���O�ݒ�
	HRESULT SetPattern(const POSGRID2& rPart);	// �������ݒ�
	D3DXVECTOR3 GetPosition(void) const;		// �ʒu�擾
	D3DXVECTOR3 GetRotation(void) const;		// �����擾
	D3DXCOLOR GetColor(void) const;				// �F�擾
	float GetRadius(void) const;				// ���a�擾
	float GetHeight(void) const;				// �c���擾
	D3DCULL GetCulling(void) const;				// �J�����O�擾
	bool GetLighting(void) const;				// ���C�e�B���O�擾
	POSGRID2 GetPattern(void) const;			// �������擾

protected:
	// �����o�֐�
	void SetVtx(void);	// ���_���̐ݒ�
	void SetIdx(void);	// �C���f�b�N�X���̐ݒ�

	void SetScrollTex	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	( // ����
		const float fTexU,	// �e�N�X�`���̉����W�̊J�n�ʒu
		const float fTexV	// �e�N�X�`���̏c���W�̊J�n�ʒu
	);

private:
	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	MeshCylinder m_meshCylinder;	// ���b�V���V�����_�[�̏��
	POSGRID2 m_part;	// ������
	int m_nNumVtx;		// �K�v���_��
	int m_nNumIdx;		// �K�v�C���f�b�N�X��
	int m_nTextureID;	// �e�N�X�`���C���f�b�N�X
};

#endif	// _OBJECT_MESHCYLINDER_H_
