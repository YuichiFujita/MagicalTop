//============================================================
//
//	�v���C���[�w�b�_�[ [player.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectModel.h"

//************************************************************
//	�N���X��`
//************************************************************
// �v���C���[�N���X
class CPlayer : public CObjectModel
{
public:
	// ��ޗ�
	typedef enum
	{
		MODEL_ESCAPEKUN = 0,	// �G�X�P�[�v�N
		MODEL_MAX				// ���̗񋓌^�̑���
	}MODEL;

	// �R���X�g���N�^
	CPlayer();

	// �f�X�g���N�^
	~CPlayer();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CPlayer *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot		// ����
	);

private:
	// �����o�֐�
	void Move(void);	// �ړ�
	void Rot(void);		// ����
	void Jump(void);	// �W�����v
	void Land(D3DXVECTOR3& rPos);	// ���n

	// �ÓI�����o�ϐ�
	static const char *mc_apModelFile[];	// ���f���萔

	// �����o�ϐ�
	D3DXMATRIX	m_mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3	m_move;		// �ړ���
	D3DXVECTOR3	m_destRot;	// �ڕW����
	bool m_bJump;			// �W�����v��
};

#endif	// _PLAYER_H_
