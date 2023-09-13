//============================================================
//
//	�}���`���f���w�b�_�[ [multiModel.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _MULTIMODEL_H_
#define _MULTIMODEL_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "model.h"

//************************************************************
//	�N���X��`
//************************************************************
// �}���`���f���N���X
class CMultiModel
{
public:
	// �R���X�g���N�^
	CMultiModel();

	// �f�X�g���N�^
	~CMultiModel();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	void BindModel(CModel::Model *pModel);	// ���f������
	void SetParent(CMultiModel *pModel);	// �e���f���ݒ�

	void SetMtxWorld(const D3DXMATRIX& rMtxWorld);	// �}�g���b�N�X�ݒ�
	void SetPosition(const D3DXVECTOR3& rPos);		// �ʒu�ݒ�
	void SetRotation(const D3DXVECTOR3& rRot);		// �����ݒ�
	void SetScaling(const D3DXVECTOR3& rScale);		// �傫���ݒ�
	void SetMaterial(const D3DXMATERIAL& rMat);		// �}�e���A���ݒ�
	void ResetMaterial(void);						// �}�e���A���Đݒ�
	void SetAlpha(const float fAlpha);				// �����x�ݒ�
	void SetModelData(const CModel::Model& rModel);	// ���f�����ݒ�
	HRESULT SetMaterial(const LPD3DXBUFFER pBuffMat, const int nNumMat);	// �}�e���A���ݒ�

	D3DXMATRIX *GetPtrMtxWorld(void);		// �}�g���b�N�X�|�C���^�擾
	D3DXMATRIX GetMtxWorld(void) const;		// �}�g���b�N�X�擾
	D3DXVECTOR3 GetPosition(void) const;	// �ʒu�擾
	D3DXVECTOR3 GetRotation(void) const;	// �����擾
	D3DXVECTOR3 GetScaling(void) const;		// �傫���擾

	D3DXMATERIAL GetMaterial(const int nID) const;	// �}�e���A���擾
	float GetAlpha(void) const;						// �����x�擾
	float GetMaxAlpha(void) const;					// �ő哧���x�擾
	CMultiModel GetParent(void) const;				// �e�擾
	CModel::Model GetModelData(void) const;			// ���f�����擾

	// �ÓI�����o�֐�
	static CMultiModel *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR3& rScale = VEC3_ONE	// �傫��
	);
	static HRESULT Release(CMultiModel *&prMultiModel);	// �j��

private:
	// �����o�ϐ�
	CModel::Model	m_modelData;	// ���f�����
	CMultiModel		*m_pParent;		// �e���f���ւ̃|�C���^
	D3DXMATERIAL	*m_pMat;		// �}�e���A���ւ̃|�C���^
	D3DXMATRIX		m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;			// �ʒu
	D3DXVECTOR3		m_rot;			// ����
	D3DXVECTOR3		m_scale;		// �傫��
};

#endif	// _MULTIMODEL_H_
