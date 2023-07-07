//============================================================
//
//	�n�ʃw�b�_�[ [field.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _FIELD_H_
#define _FIELD_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectMeshField.h"

//************************************************************
//	�N���X��`
//************************************************************
// �n�ʃN���X
class CField : public CObjectMeshField
{
public:
	// �e�N�X�`����
	typedef enum
	{
		TEXTURE_NORMAL = 0,	// �ʏ�e�N�X�`��
		TEXTURE_MAX			// ���̗񋓌^�̑���
	}TEXTURE;

	// �n�`��
	typedef enum
	{
		TERRAIN_80x80 = 0,	// 80x80�������̒n�`
		TERRAIN_MAX			// ���̗񋓌^�̑���
	}TERRAIN;

	// �R���X�g���N�^
	CField();

	// �f�X�g���N�^
	~CField();

	// �n�`���\����
	typedef struct
	{
		D3DXVECTOR3 *pPosGap;	// ���_���W�̂����
		POSGRID2 part;			// ������
	}TerrainInfo;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static void LoadSetup(void);	// �Z�b�g�A�b�v
	static CField *Create	// ����
	( // ����
		const TEXTURE texture,		// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR2& rSize,	// �傫��
		const D3DXCOLOR& rCol,		// �F
		const POSGRID2& rPart,		// ������
		const D3DCULL cull = D3DCULL_CCW,	// �J�����O��
		const bool bLight = true			// ���C�e�B���O��
	);

	// �����o�֐�
	void SetTerrain(const TERRAIN terrain);	// �n�`�ݒ�

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔
	static TerrainInfo m_aTerrainInfo[TERRAIN_MAX];	// �n�`���
};

#endif	// _FIELD_H_
