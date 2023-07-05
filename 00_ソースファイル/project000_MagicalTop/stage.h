//============================================================
//
//	�X�e�[�W�w�b�_�[ [stage.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _STAGE_H_
#define _STAGE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_GRIDX	(128)	// ������WX�̍ő吔
#define MAX_GRIDY	(128)	// ������WY�̍ő吔
#define MAX_GRIDZ	(128)	// ������WZ�̍ő吔

//************************************************************
//	�O���錾
//************************************************************
class CBlock;	// �u���b�N�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �X�e�[�W�N���X
class CStage
{
public:
	// �R���X�g���N�^
	CStage();

	// �f�X�g���N�^
	~CStage();

	// ������ Z�\����
	typedef struct
	{
		CBlock *pBlock;	// �u���b�N�̃|�C���^
	}GridZ;

	// ������ Y�\����
	typedef struct
	{
		GridZ aZ[MAX_GRIDZ];	// ������WZ
	}GridY;

	// ������ X�\����
	typedef struct
	{
		GridY aY[MAX_GRIDY];	// ������WY
	}GridX;

	// �X�e�[�W������W�\����
	typedef struct
	{
		// �����o�֐�
		void In(const POSGRID3& rGrid, CBlock *pBlock)
		{
			aX[rGrid.x].aY[rGrid.y].aZ[rGrid.z].pBlock = pBlock;
		}
		GridZ Out(const POSGRID3& rGrid)
		{
			return aX[rGrid.x].aY[rGrid.y].aZ[rGrid.z];
		}

		// �����o�ϐ�
		GridX aX[MAX_GRIDX];	// ������WX
		D3DXVECTOR3 centerPos;	// ������W�̒��S���W
		D3DXVECTOR3 revPos;		// ���W�ϊ��̕␳�l
		POSGRID3 maxGrid;		// ������W��
	}StageGrid;

	// �X�e�[�W�͈͍\����
	typedef struct
	{
		float fNear;	// �����ʒu (�O)
		float fFar;		// �����ʒu (��)
		float fRight;	// �����ʒu (�E)
		float fLeft;	// �����ʒu (��)
		float fField;	// �����ʒu (�n��)
	}StageLimit;

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void LimitPosition(D3DXVECTOR3& rPos, const float fRadius);						// �ʒu�␳
	bool LandPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, const float fHeight);	// �͈͊O���n
	D3DXVECTOR3 ChangeGridPosition(const POSGRID3& rGrid);							// ������W�̍��W�ϊ�

	void SetStageLimit(const StageLimit& rLimit);			// �X�e�[�W�͈͐ݒ�
	void SetStageGridCenter(const D3DXVECTOR3& rPos);		// �X�e�[�W������W�̒��S�ʒu�ݒ�
	void SetStageGridRevision(const D3DXVECTOR3& rRev);		// �X�e�[�W������W�̈ʒu�␳�ʐݒ�
	void SetStageGridMax(const POSGRID3& rGrid);			// �X�e�[�W������W�̍ő���ᐔ�ݒ�
	void SetInGrid(const POSGRID3& rGrid, CBlock *pBlock);	// �X�e�[�W������W���ݒ�

	StageLimit GetStageLimit(void) const;	// �X�e�[�W�͈͎擾
	StageGrid GetStageGrid(void) const;		// �X�e�[�W������W�擾
	GridZ GetInGrid(const POSGRID3& rGrid);	// �X�e�[�W������W���擾

	// �ÓI�����o�֐�
	static CStage *Create(void);				// ����
	static HRESULT Release(CStage *&prStage);	// �j��

private:
	// �ÓI�����o�֐�
	static void LoadSetup(CStage *pStage);	// �Z�b�g�A�b�v

	// �����o�ϐ�
	StageLimit m_stageLimit;	// �X�e�[�W�͈�
	StageGrid m_stageGrid;		// �X�e�[�W������W
};

#endif	// _STAGE_H_
