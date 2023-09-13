//============================================================
//
//	�o���l�w�b�_�[ [expOrb.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EXPORB_H_
#define _EXPORB_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectBillboard.h"

//************************************************************
//	�N���X��`
//************************************************************
// �o���l�N���X
class CExpOrb : public CObjectBillboard
{
public:
	// �e�N�X�`����
	typedef enum
	{
		TEXTURE_NORMAL = 0,	// �o���l
		TEXTURE_MAX			// ���̗񋓌^�̑���
	}TEXTURE;

	// �R���X�g���N�^
	CExpOrb();

	// �f�X�g���N�^
	~CExpOrb();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CExpOrb *Create(const D3DXVECTOR3& rPos);	// ����
	static void RandomSpawn(const int nNum, const D3DXVECTOR3& rPos);	// �����_������

private:
	// �����o�֐�
	bool CollisionPlayer(void);	// �v���C���[�Ƃ̓����蔻��

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔
};

#endif	// _EXPORB_H_
