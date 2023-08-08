//============================================================
//
//	�e�w�b�_�[ [bullet.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _BULLET_H_
#define _BULLET_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectBillboard.h"

//************************************************************
//	�N���X��`
//************************************************************
// �e�N���X
class CBullet : public CObjectBillboard
{
public:
	// ��ޗ�
	typedef enum
	{
		TYPE_ENEMY = 0,	// �G�̒e
		TYPE_MAX		// ���̗񋓌^�̑���
	}TYPE;

	// �R���X�g���N�^
	CBullet(const int nDamage);

	// �f�X�g���N�^
	~CBullet();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CBullet *Create	// ����
	( // ����
		const TYPE type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rSize,	// �傫��
		const D3DXCOLOR& rCol,		// �F
		const D3DXVECTOR3& rVec,	// �ړ�����
		const float fMove,			// �ړ����x
		const int nLife,			// ����
		const int nDamage			// �U����
	);

	// �����o�֐�
	void SetMove(D3DXVECTOR3 vec, const float fMove);	// �ړ��ʐݒ�
	void SetType(const TYPE type);	// ��ސݒ�
	void SetLife(const int nLife);	// �����ݒ�

private:
	// �����o�֐�
	bool CollisionTarget(void);	// �^�[�Q�b�g�Ƃ̓����蔻��
	bool CollisionPlayer(void);	// �v���C���[�Ƃ̓����蔻��

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	D3DXVECTOR3 m_move;		// �ړ���
	TYPE m_type;			// ���
	int m_nLife;			// ����
	const int m_nDamage;	// �U���͒萔
};

#endif	// _BULLET_H_
