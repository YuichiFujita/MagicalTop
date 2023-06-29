//============================================================
//
//	�֗��֐��w�b�_�[ [useful.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _USEFUL_H_
#define _USEFUL_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�}�N����`
//************************************************************
// D3DXVECTOR2�֌W
#define VEC2_ZERO	(D3DXVECTOR2(0.0f, 0.0f))	// 0�N���A
#define VEC2_ONE	(D3DXVECTOR2(1.0f, 1.0f))	// 1�N���A
#define VEC2_ALL(f)	(D3DXVECTOR2(f, f))	// ���l�N���A

// D3DXVECTOR3�֌W
#define VEC3_ZERO	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// 0�N���A
#define VEC3_ONE	(D3DXVECTOR3(1.0f, 1.0f, 1.0f))	// 1�N���A
#define VEC3_ALL(f)	(D3DXVECTOR3(f, f, f))	// ���l�N���A

// POSGRID2�֌W
#define GRID2_ZERO	(POSGRID2(0, 0))	// 0�N���A
#define GRID2_ONE	(POSGRID2(1, 1))	// 1�N���A

// POSGRID3�֌W
#define GRID3_ZERO	(D3DXVECTOR3(0, 0, 0))	// 0�N���A
#define GRID3_ONE	(D3DXVECTOR3(1, 1, 1))	// 1�N���A

// D3DXCOLOR�֌W
#define XCOL_ABLACK	(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))	// ��(����)
#define XCOL_BLACK	(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))	// ��(�s����)
#define XCOL_AWHITE	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f))	// ��(����)
#define XCOL_WHITE	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// ��(�s����)

// �|�C���^�g�p����
#define USED(p)		((p) != NULL)	// �g�p��
#define UNUSED(p)	((p) == NULL)	// ���g�p

// ���̂ق��֗��}�N��
#define MAX_STRING		(128)	// ������̍ő啶����
#define MAX_FILENAME	(128)	// �t�@�C�����̍ő啶����
#define NONE_IDX		(-1)	// �C���f�b�N�X��g�p

//************************************************************
//	�\���̒�`
//************************************************************
// 2�������W
struct POSGRID2
{
	// �R���X�g���N�^
	POSGRID2() {}
	POSGRID2(const int nx, const int ny) { x = nx; y = ny; }	// ���

	// �f�X�g���N�^
	~POSGRID2() {}

	// �����o�ϐ�
	int x;	// ������W (x)
	int y;	// ������W (y)
};

// 3�������W
struct POSGRID3
{
	// �R���X�g���N�^
	POSGRID3() {}
	POSGRID3(const int nx, const int ny, const int nz) { x = nx; y = ny; z = nz; }	// ���

	// �f�X�g���N�^
	~POSGRID3() {}

	// �����o�ϐ�
	int x;	// ������W (x)
	int y;	// ������W (y)
	int z;	// ������W (z)
};

//************************************************************
//	���O��Ԑ錾
//************************************************************
// �֗��֐����
namespace useful
{
	// �ʏ�֐�
	void DivideDigitNum		// ���l�̌������Ƃ̕���
	( // ����
		int *pNumDivide,	// �������ʂ̊i�[�z��
		const int nNum,		// �������鐔�l
		const int nMaxDigit	// �������鐔���̌���
	);

	void NormalizeNormal	// �@���̐��K��
	( // ����
		const D3DXVECTOR3& rPosLeft,	// ���ʒu
		const D3DXVECTOR3& rPosCenter,	// ���S�ʒu
		const D3DXVECTOR3& rPosRight,	// �E�ʒu
		D3DXVECTOR3& rNor				// �@��
	);

	void NormalizeRot(float& rRot);	// �����̐��K��

	// �e���v���[�g�֐�
	template<class T> bool LimitNum	// �l�͈͓̔�����
	( // ����
		T& rNum,		// �������l
		const T min,	// �ŏ��͈�
		const T max		// �ő�͈�
	);
}

//************************************************************
//	�e���v���[�g�֐�
//************************************************************
//============================================================
//	�l�͈͓̔�����
//============================================================
template<class T> bool useful::LimitNum
( // ����
	T& rNum,		// �������l
	const T min,	// �ŏ��͈�
	const T max		// �ő�͈�
)
{
	if (rNum < min)
	{ // �������l���ŏ��͈͂𒴂��Ă����ꍇ

		// �͈͓��ɕ␳
		rNum = min;

		// �^��Ԃ�
		return true;
	}
	else if (rNum > max)
	{ // �������l���ő�͈͂𒴂��Ă����ꍇ

		// �͈͓��ɕ␳
		rNum = max;

		// �^��Ԃ�
		return true;
	}

	// �U��Ԃ�
	return false;
}

#endif	// _USEFUL_H_
