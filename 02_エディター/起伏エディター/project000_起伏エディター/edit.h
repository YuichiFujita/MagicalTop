//============================================================
//
//	�G�f�B�b�g�w�b�_�[ [edit.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_H_
#define _EDIT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�N���X
class CEdit
{
public:
	// �R���X�g���N�^
	CEdit();

	// �f�X�g���N�^
	~CEdit();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V

	// �ÓI�����o�֐�
	static CEdit *Create(void);				// ����
	static HRESULT Release(CEdit *&prEdit);	// �j��

private:
	// �����o�֐�
	void SetBursh(void);	// �u���V�ݒ�
	void SetVtxUp(void);	// �㏸�ʐݒ�
	void SetVtx(void);		// ���_�ݒ�
	void SetNor(void);		// �@���ݒ�
	void DrawManual(void);	// ��������\��
	void Save(void);		// �ۑ�
	void Load(void);		// �Ǎ�

	// �����o�ϐ�
	float m_fBrushRadius;	// �M�̑���
	float m_fVtxUp;			// ���_�̏㏸��
	bool m_bAutoNor;		// �@���̎����ݒ��
};

#endif	// _EDIT_H_
