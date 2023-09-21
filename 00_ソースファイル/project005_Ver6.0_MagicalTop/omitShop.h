//============================================================
//
//	�V���b�v�Ȃ����w�b�_�[ [omitShop.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OMITSHOP_H_
#define _OMITSHOP_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "shop.h"

//************************************************************
//	�N���X��`
//************************************************************
// �V���b�v�Ȃ����N���X
class COmitShop
{
public:
	// �R���X�g���N�^
	COmitShop();

	// �f�X�g���N�^
	~COmitShop();

	// �����o�֐�
	void Init(void);	// ������
	void Uninit(void);	// �I��
	int *CreateOmit(void);		// �Ȃ��w���i����
	HRESULT ReleaseOmit(void);	// �Ȃ��w���i�j��
	int GetNumOmit(void) const;	// �Ȃ��w���i���擾
	bool IsOmit(const CShop::BUY buy) const;	// �Ȃ��󋵎擾
	void SetEnableOmit(const CShop::BUY buy, const bool bOmit);	// �Ȃ��󋵐ݒ�

	// �ÓI�����o�֐�
	static COmitShop *Create(void);	// ����
	static HRESULT Release(COmitShop *&prOmitShop);	// �j��

private:
	int *m_pOmit;		// �Ȃ��w���i���
	int  m_nNumOmit;	// �Ȃ��w���i��
	bool m_aOmitShop[CShop::BUY_MAX];	// �w���i�̏Ȃ���
};

#endif	// _OMITSHOP_H_
