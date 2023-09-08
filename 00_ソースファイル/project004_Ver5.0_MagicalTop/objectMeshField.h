//============================================================
//
//	�I�u�W�F�N�g���b�V���t�B�[���h�w�b�_�[ [objectMeshField.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECT_MESHFIELD_H_
#define _OBJECT_MESHFIELD_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g���b�V���t�B�[���h�N���X
class CObjectMeshField : public CObject
{
public:
	// �R���X�g���N�^
	CObjectMeshField();
	CObjectMeshField(const CObject::LABEL label, const int nPriority = DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObjectMeshField();

	// ���b�V���t�B�[���h�\����
	typedef struct
	{
		D3DXVECTOR3	pos;		// �ʒu
		D3DXVECTOR3	rot;		// ����
		D3DXVECTOR2	size;		// �傫��
		D3DXCOLOR	col;		// �F
		D3DXMATRIX	mtxWorld;	// ���[���h�}�g���b�N�X
		D3DCULL cull;	// �J�����O��
		bool bLight;	// ���C�e�B���O��
	}MeshField;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CObjectMeshField *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR2& rSize,	// �傫��
		const D3DXCOLOR& rCol,		// �F
		const POSGRID2& rPart,		// ������
		const D3DCULL cull = D3DCULL_CCW,	// �J�����O��
		const bool bLight = true			// ���C�e�B���O��
	);

	// �����o�֐�
	void NormalizeNormal(void);		// �@���̐��K��
	bool LandPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove);	// ���b�V�����n

	void BindTexture(const int nTextureID);				// �e�N�X�`������
	void SetPosition(const D3DXVECTOR3& rPos);			// �ʒu�ݒ�
	void SetRotation(const D3DXVECTOR3& rRot);			// �����ݒ�
	void SetScalingBiaxial(const D3DXVECTOR2& rSize);	// �傫���ݒ�
	void SetColor(const D3DXCOLOR& rCol);				// �F�ݒ�
	void SetCulling(const D3DCULL cull);				// �J�����O�ݒ�
	void SetLighting(const bool bLight);				// ���C�e�B���O�ݒ�
	HRESULT SetPattern(const POSGRID2& rPart);			// �������ݒ�
	void SetGapPosition(const int nID, const D3DXVECTOR3& rPos);		// ���W�̂���ݒ�
	void SetMeshVertexPosition(const int nID, const D3DXVECTOR3& rPos);	// ���b�V���̒��_�ʒu�ݒ�
	void SetTerrain(const POSGRID2& rPart, D3DXVECTOR3 *pPosGap);		// �n�`�ݒ�

	D3DXVECTOR3 GetPosition(void) const;		// �ʒu�擾
	D3DXVECTOR3 GetRotation(void) const;		// �����擾
	D3DXVECTOR2 GetScalingBiaxial(void) const;	// �傫���擾
	D3DXCOLOR GetColor(void) const;				// �F�擾
	D3DCULL GetCulling(void) const;				// �J�����O�擾
	bool GetLighting(void) const;				// ���C�e�B���O�擾
	POSGRID2 GetPattern(void) const;			// �������擾
	int GetNumVertex(void) const;				// ���_���擾
	D3DXVECTOR3 GetGapPosition(const int nID);				// ���W�̂���擾
	D3DXVECTOR3 GetMeshVertexPosition(const int nID);		// ���b�V���̒��_�ʒu�擾
	bool IsPositionRange(const D3DXVECTOR3&rPos);			// ���b�V���͈͓̔��擾 (��]�l��)
	float GetPositionHeight(const D3DXVECTOR3&rPos);		// ���b�V���̒��n�ʒu�擾 (��]��l��)
	float GetPositionRotateHeight(const D3DXVECTOR3&rPos);	// ���b�V���̒��n�ʒu�擾 (��]�l��)

protected:
	// �����o�֐�
	void SetVtx(bool bNor);	// ���_���̐ݒ�
	void SetIdx(void);		// �C���f�b�N�X���̐ݒ�

	void SetScrollTex		// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	( // ����
		const float fTexU,	// �e�N�X�`���̉����W�̊J�n�ʒu
		const float fTexV	// �e�N�X�`���̏c���W�̊J�n�ʒu
	);

private:
	// �����o�֐�
	D3DXVECTOR3 GetNormalLeft(VERTEX_3D *pVtx);			// �@���̎擾 (��)
	D3DXVECTOR3 GetNormalLeftTop(VERTEX_3D *pVtx);		// �@���̎擾 (����)
	D3DXVECTOR3 GetNormalLeftBottom(VERTEX_3D *pVtx);	// �@���̎擾 (����)
	D3DXVECTOR3 GetNormalRight(VERTEX_3D *pVtx);		// �@���̎擾 (�E)
	D3DXVECTOR3 GetNormalRightTop(VERTEX_3D *pVtx);		// �@���̎擾 (�E��)
	D3DXVECTOR3 GetNormalRightBottom(VERTEX_3D *pVtx);	// �@���̎擾 (�E��)

	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 *m_pPosGapBuff;	// ���W�̂���o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 *m_pNorBuff;	// �@���o�b�t�@�ւ̃|�C���^
	int *m_pNumNorBuff;			// �@���̎g�p���o�b�t�@�ւ̃|�C���^

	MeshField m_meshField;	// ���b�V���t�B�[���h�̏��
	POSGRID2 m_part;		// ������
	int m_nNumVtx;			// �K�v���_��
	int m_nNumIdx;			// �K�v�C���f�b�N�X��
	int m_nTextureID;		// �e�N�X�`���C���f�b�N�X
};

#endif	// _OBJECT_MESHFIELD_H_
