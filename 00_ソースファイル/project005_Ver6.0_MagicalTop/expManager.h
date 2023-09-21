//============================================================
//
//	�o���l�}�l�[�W���[�w�b�_�[ [expManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EXPMANAGER_H_
#define _EXPMANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�O���錾
//************************************************************
class CObjectGauge2D;	// �I�u�W�F�N�g�Q�[�W2D�N���X
class CMultiValue;		// �}���`�����N���X

//************************************************************
//	�N���X��`
//************************************************************
// �o���l�}�l�[�W���[�N���X
class CExpManager
{
public:
	// �R���X�g���N�^
	CExpManager();

	// �f�X�g���N�^
	~CExpManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void AddLevel(const int nAdd);	// ���x�����Z
	void AddExp(const int nAdd);	// �o���l���Z
	int  GetLevel(void) const;		// ���x���擾
	void SetEnableExpDraw(const bool bDraw);	// �o���l�`��󋵂̐ݒ�

	// �ÓI�����o�֐�
	static CExpManager *Create(void);	// ����
	static HRESULT Release(CExpManager *&prExpManager);	// �j��

private:
	// �����o�ϐ�
	CObjectGauge2D *m_pExp;	// �o���l�̏��
	CMultiValue *m_pLevel;	// ���x���̏��
};

#endif	// _EXPMANAGER_H_
