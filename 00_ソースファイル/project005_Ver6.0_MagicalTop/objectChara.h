//============================================================
//
//	�I�u�W�F�N�g�L�����N�^�[�w�b�_�[ [objectChara.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECTCHARA_H_
#define _OBJECTCHARA_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"
#include "motion.h"

//************************************************************
//	�O���錾
//************************************************************
class CMultiModel;	// �}���`���f���N���X
class CMotion;		// ���[�V�����N���X

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g�L�����N�^�[�N���X
class CObjectChara : public CObject
{
public:
	// �R���X�g���N�^
	CObjectChara();
	CObjectChara(const CObject::LABEL label, const int nPriority = DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObjectChara();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	D3DXMATRIX *GetPtrMtxWorld(void);	// �}�g���b�N�X�|�C���^�擾

	// �����o�֐�
	void SetPartsInfo	// �p�[�c���ݒ�
	( // ����
		const int nID,				// �p�[�c�C���f�b�N�X
		const int nParentID,		// �e�C���f�b�N�X
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const char *pFileName		// �t�@�C����
	);
	void SetModelInfo(void);	// ���f�����ݒ�
	void SetMotionInfo(CMotion::MotionInfo info);	// ���[�V�������ݒ�
	void SetMotion(const int nType);				// ���[�V�����ݒ�
	void SetEnableMotionUpdate(const bool bUpdate);	// �X�V�󋵐ݒ�
	void SetPosition(const D3DXVECTOR3& rPos);		// �ʒu�ݒ�
	void SetRotation(const D3DXVECTOR3& rRot);		// �����ݒ�
	void SetMaterial(const D3DXMATERIAL& rMat);		// �}�e���A���ݒ�
	void ResetMaterial(void);						// �}�e���A���Đݒ�
	void SetAlpha(const float fAlpha);				// �����x�ݒ�
	void SetPartsPosition(const int nID, const D3DXVECTOR3& rPos);	// �p�[�c�ʒu�ݒ�
	void SetPartsRotation(const int nID, const D3DXVECTOR3& rRot);	// �p�[�c�����ݒ�

	int  GetMotionType(void) const;				// ���[�V������ގ擾
	bool IsMotionFinish(void) const;			// ���[�V�����I���擾
	bool IsMotionLoop(const int nType) const;	// ���[�V�������[�v�擾
	D3DXVECTOR3 GetPosition(void) const;		// �ʒu�擾
	D3DXVECTOR3 GetRotation(void) const;		// �����擾
	float GetAlpha(void) const;					// �����x�擾
	float GetMaxAlpha(void) const;				// �ő哧���x�擾
	D3DXVECTOR3 GetPartsPosition(const int nID) const;	// �p�[�c�ʒu�擾
	D3DXVECTOR3 GetPartsRotation(const int nID) const;	// �p�[�c�����擾
	CMultiModel *GetMultiModel(const int nID) const;	// �}���`���f���擾

private:
	// �����o�ϐ�
	CMultiModel	*m_apMultiModel[MAX_PARTS];	// ���f���̏��
	CMotion		*m_pMotion;	// ���[�V�����̏��
	D3DXMATRIX	m_mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3	m_pos;		// �ʒu
	D3DXVECTOR3	m_rot;		// ����
	int m_nNumModel;		// �p�[�c�̑���
};

#endif	// _OBJECTCHARA_H_
