//============================================================
//
//	�`���[�g���A���v���C���[�w�b�_�[ [playerTutorial.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PLAYER_TUTORIAL_H_
#define _PLAYER_TUTORIAL_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "player.h"

//************************************************************
//	�N���X��`
//************************************************************
// �`���[�g���A���v���C���[�N���X
class CPlayerTutorial : public CPlayer
{
public:
	// �R���X�g���N�^
	CPlayerTutorial();

	// �f�X�g���N�^
	~CPlayerTutorial();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void HitBlowAway	// ������уq�b�g
	( // ����
		const D3DXVECTOR3& rPlayerPos,	// �v���C���[�ʒu
		const D3DXVECTOR3& rHitObjPos,	// �I�u�W�F�N�g�ʒu
		const int nDmg					// �_���[�W��
	) override;
	void HitVortex		// �Q�������܂�q�b�g
	( // ����
		D3DXVECTOR3& rPlayerPos,	// �v���C���[�ʒu
		const D3DXVECTOR3& rHitPos,	// �����蔻��ʒu
		const int nDmg				// �_���[�W��
	) override;
	void SetDisp(const bool bDisp) override;		// �\���ݒ�
	void SetEnableDraw(const bool bDraw) override;	// �`��󋵐ݒ�

private:
	// �����o�֐�
	MOTION UpdateNormal(void);	// �ʏ��Ԏ��̍X�V
	MOTION UpdateMove(void);	// �ړ��ʁE�ڕW�����̍X�V

	// �����o�ϐ�
	int m_nOldNumEnemy;	// �ߋ��̓G�̑���
};

#endif	// _PLAYER_TUTORIAL_H_
