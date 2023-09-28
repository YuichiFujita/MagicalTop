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
	// �e�N�X�`����
	enum TEXTURE
	{
		TEXTURE_LV = 0,				// �K�v���x���e�N�X�`��
		TEXTURE_ICON,				// �A�C�R���e�N�X�`��
		TEXTURE_EXPLAIN,			// �����e�N�X�`��
		TEXTURE_SOLDOUT_ICON,		// ����؂�A�C�R���e�N�X�`��
		TEXTURE_SOLDOUT_EXPLAIN,	// ����؂�����e�N�X�`��
		TEXTURE_MAX,				// ���̗񋓌^�̑���
	};

	// �w����
	enum BUY
	{
		BUY_HEAL = 0,			// ��
		BUY_LVUP_LIFE,			// �̗̓��x���A�b�v
		BUY_LVUP_MANA,			// �}�i���x���A�b�v
		BUY_LVUP_DASH,			// �_�b�V�����x���A�b�v
		BUY_LVUP_DEFENSE,		// �h��̓��x���A�b�v
		BUY_LVUP_SPEED,			// �f�������x���A�b�v
		BUY_LVUP_MAGIC_NUM,		// ���@���ː����x���A�b�v
		BUY_LVUP_MAGIC_SPEED,	// ���@�e�����x���A�b�v
		BUY_LVUP_MAGIC_RAPID,	// ���@�A�ˑ��x���x���A�b�v
		BUY_EXP_UP,				// �l���o���l����
		BUY_MAX					// ���̗񋓌^�̑���
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
	void SetEnableDraw(const bool bDraw);	// �`��󋵐ݒ�
	void SetPriority(const int nPriority);	// �D�揇�ʐݒ�

	// �ÓI�����o�֐�
	static CShop *Create	// ����
	( // ����
		const BUY buy,			// �w���i
		const D3DXVECTOR3& rPos	// �ʒu
	);
	static int GetNeedLevel(const BUY buy);	// �w���K�v���x���擾

	// �����o�֐�
	void SetBuy(const BUY buy);	// �ݒ�
	BUY  GetBuy(void) const;	// �擾
	void SetPosition(const D3DXVECTOR3& rPos);	// �ʒu�ݒ�
	void SetColor(const D3DXCOLOR& rCol);		// �F�ݒ�
	D3DXVECTOR3 GetPosition(void) const;		// �ʒu�擾
	D3DXCOLOR GetColor(void) const;				// �F�擾

private:
	// �����o�֐�
	void SetPositionRelative(void);	// ���Έʒu�ݒ�

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔
	static const int mc_aNeedLevel[];		// �w���K�v���x���萔

	// �����o�ϐ�
	CAnim2D *m_pIcon;		// �w���i�A�C�R��
	CAnim2D *m_pExplain;	// �w���i����
	CValueUI *m_pLv;		// �K�v���x��
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXCOLOR m_col;		// �F
	BUY m_buy;				// �w���i
};

#endif	// _SHOP_H_
