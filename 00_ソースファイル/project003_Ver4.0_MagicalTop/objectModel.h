//============================================================
//
//	�I�u�W�F�N�g���f���w�b�_�[ [objectModel.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECTMODEL_H_
#define _OBJECTMODEL_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"
#include "model.h"

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g���f���N���X
class CObjectModel : public CObject
{
public:
	// �R���X�g���N�^
	CObjectModel();
	CObjectModel(const CObject::LABEL label, const int nPriority = DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObjectModel();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CObjectModel *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR3& rScale = VEC3_ONE	// �傫��
	);

	// �����o�֐�
	void BindModel(CModel::Model *pModel);			// ���f������
	void SetMtxWorld(const D3DXMATRIX& rMtxWorld);	// �}�g���b�N�X�ݒ�
	void SetPosition(const D3DXVECTOR3& rPos);		// �ʒu�ݒ�
	void SetRotation(const D3DXVECTOR3& rRot);		// �����ݒ�
	void SetScaling(const D3DXVECTOR3& rScale);		// �傫���ݒ�
	void SetModelData(const CModel::Model& rModel);	// ���f�����ݒ�
	D3DXMATRIX *GetPtrMtxWorld(void);				// �}�g���b�N�X�|�C���^�擾
	D3DXMATRIX GetMtxWorld(void) const;				// �}�g���b�N�X�擾
	D3DXVECTOR3 GetPosition(void) const;			// �ʒu�擾
	D3DXVECTOR3 GetRotation(void) const;			// �����擾
	D3DXVECTOR3 GetScaling(void) const;				// �傫���擾
	CModel::Model GetModelData(void) const;			// ���f�����擾

private:
	// �����o�ϐ�
	CModel::Model	m_modelData;	// ���f�����
	D3DXMATRIX		m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;			// �ʒu
	D3DXVECTOR3		m_rot;			// ����
	D3DXVECTOR3		m_scale;		// �傫��
};

#endif	// _OBJECTMODEL_H_
