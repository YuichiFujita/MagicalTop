//============================================================
//
//	�V���b�v�w�b�_�[ [shop.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SHOP_H_
#define _SHOP_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�O���錾
//************************************************************
class CAnim2D;	// �A�j���[�V����2D�N���X
class CValueUI;	// ����UI�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �V���b�v�N���X
class CShop : public CObject
{
public:
	// �w����
	enum BUY
	{
		BUY_LV1_FIRE = 0,	// LV1�����@
		BUY_LV1_WATER,		// LV1�����@
		BUY_LV1_WIND,		// LV1�����@
		BUY_LV2_FIRE,		// LV2�����@
		BUY_LV2_WATER,		// LV2�����@
		BUY_LV2_WIND,		// LV2�����@
		BUY_MAX				// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CShop();

	// �f�X�g���N�^
	~CShop();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CShop *Create	// ����
	( // ����
		const BUY buy,			// �w���i
		const D3DXVECTOR3& rPos	// �ʒu
	);

	// �����o�֐�
	void Set(const BUY buy);	// �ݒ�
	BUY  Get(void);				// �擾

private:
	// �����o�ϐ�
	CAnim2D *m_pIcon;	// �w���i�A�C�R��
	CValueUI *m_pLife;	// �K�v�̗�
	CValueUI *m_pLv;	// �K�v���x��
	BUY m_buy;			// �w���i
};

#endif	// _SHOP_H_
