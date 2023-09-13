//============================================================
//
//	�I�u�W�F�N�g���b�V���L���[�u�w�b�_�[ [objectMeshCube.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECT_MESHCUBE_H_
#define _OBJECT_MESHCUBE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g���b�V���L���[�u�N���X
class CObjectMeshCube : public CObject
{
public:
	// �R���X�g���N�^
	CObjectMeshCube();
	CObjectMeshCube(const CObject::LABEL label, const int nPriority = DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObjectMeshCube();

	// ������
	typedef enum
	{
		BORDERSTATE_OFF = 0,	// �����Ȃ�
		BORDERSTATE_ON,			// ����肠��
		BORDERSTATE_MAX,		// ���̗񋓌^�̑���
	}BORDERSTATE;

	// �e�N�X�`���g�p��
	typedef enum
	{
		TEXSTATE_ONE = 0,	// ����e�N�X�`��
		TEXSTATE_SELECT,	// �S�I���e�N�X�`��
		TEXSTATE_MAX,		// ���̗񋓌^�̑���
	}TEXSTATE;

	// �L���[�u�F��
	typedef enum
	{
		CUBECOL_CUBE = 0,	// �L���[�u�F
		CUBECOL_BORDER,		// �����F
		CUBECOL_MAX			// ���̗񋓌^�̑���
	}CUBECOL;

	// �L���[�u��������
	typedef enum
	{
		CUBEPART_X = 0,	// �e�N�X�`�������� x
		CUBEPART_Y,		// �e�N�X�`�������� y
		CUBEPART_Z,		// �e�N�X�`�������� z
		CUBEPART_MAX	// ���̗񋓌^�̑���
	}CUBEPART;

	// �e�N�X�`���\����
	struct FACETEX
	{
	public:
		// �R���X�g���N�^
		FACETEX() {}

		FACETEX(const int nAll)
		{ All = nAll; Left = 0; Right = 0; Bottom = 0; Top = 0; Near = 0; Far = 0; }

		FACETEX(const int nLeft, const int nRight, const int nBottom, const int nTop, const int nNear, const int nFar)
		{ All = 0; Left = nLeft; Right = nRight; Bottom = nBottom; Top = nTop; Near = nNear; Far = nFar; }

		FACETEX(const int nAll, const int nLeft, const int nRight, const int nBottom, const int nTop, const int nNear, const int nFar)
		{ All = nAll; Left = nLeft; Right = nRight; Bottom = nBottom; Top = nTop; Near = nNear; Far = nFar; }

		// �f�X�g���N�^
		~FACETEX() {}

		// �����o�ϐ�
		int All;	// �S�ʂ̃e�N�X�`���C���f�b�N�X
		int Left;	// ���̃e�N�X�`���C���f�b�N�X
		int Right;	// �E�̃e�N�X�`���C���f�b�N�X
		int Bottom;	// ���̃e�N�X�`���C���f�b�N�X
		int Top;	// ��̃e�N�X�`���C���f�b�N�X
		int Near;	// �O�̃e�N�X�`���C���f�b�N�X
		int Far;	// ��̃e�N�X�`���C���f�b�N�X
	};

	// ���b�V���L���[�u�\����
	typedef struct
	{
		D3DXVECTOR3	pos;			// �ʒu
		D3DXVECTOR3	rot;			// ����
		D3DXVECTOR3	size;			// �傫��
		D3DXCOLOR	aCol[2];		// �F
		D3DXMATRIX	mtxWorld;		// ���[���h�}�g���b�N�X
		BORDERSTATE	bordState;		// �����g�p���
		float		fBordThick;		// ����葾��
		TEXSTATE	texState;		// �e�N�X�`���g�p���
		FACETEX		texID;			// �e�N�X�`�����
		D3DXVECTOR2	aTexPart[3];	// �e�N�X�`��������
		D3DCULL		cull;			// �J�����O��
		bool		bLight;			// ���C�e�B���O��
	}MeshCube;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CObjectMeshCube *Create		// ����
	( // ����
		const D3DXVECTOR3& rPos,		// �ʒu
		const D3DXVECTOR3& rRot,		// ����
		const D3DXVECTOR3& rSize,		// �傫��
		const D3DXCOLOR& rCubeCol,		// �L���[�u�F
		const D3DXCOLOR& rBorderCol,	// �����F
		const BORDERSTATE bordState,	// �������
		const float fBordThick,			// ����葾��
		const TEXSTATE texState,		// �e�N�X�`�����
		const FACETEX& rTexID,			// �e�N�X�`�����
		const D3DXVECTOR2& rTexPartX = VEC2_ONE,	// �e�N�X�`�������� x
		const D3DXVECTOR2& rTexPartY = VEC2_ONE,	// �e�N�X�`�������� y
		const D3DXVECTOR2& rTexPartZ = VEC2_ONE,	// �e�N�X�`�������� z
		const D3DCULL cull = D3DCULL_CCW,			// �J�����O��
		const bool bLight = true					// ���C�e�B���O��
	);

	// �����o�֐�
	void BindTexture(const FACETEX textureID);				// �e�N�X�`������
	void SetPosition(const D3DXVECTOR3& rPos);				// �ʒu�ݒ�
	void SetRotation(const D3DXVECTOR3& rRot);				// �����ݒ�
	void SetScaling(const D3DXVECTOR3& rSize);				// �傫���ݒ�
	void SetCubeColor(const D3DXCOLOR& rCol);				// �L���[�u�F�ݒ�
	void SetBorderColor(const D3DXCOLOR& rCol);				// �����F�ݒ�
	HRESULT SetBorderState(const BORDERSTATE bordState);	// ������Ԑݒ�
	void SetBorderThick(const float fBordThick);			// ����葾���ݒ�
	void SetTextureState(const TEXSTATE texState);			// �e�N�X�`����Ԑݒ�
	void SetTexturePatternX(const D3DXVECTOR2& rTexPart);	// �e�N�X�`�������� x�ݒ�
	void SetTexturePatternY(const D3DXVECTOR2& rTexPart);	// �e�N�X�`�������� y�ݒ�
	void SetTexturePatternZ(const D3DXVECTOR2& rTexPart);	// �e�N�X�`�������� z�ݒ�
	void SetCulling(const D3DCULL cull);					// �J�����O�ݒ�
	void SetLighting(const bool bLight);					// ���C�e�B���O�ݒ�

	D3DXVECTOR3 GetPosition(void) const;		// �ʒu�擾
	D3DXVECTOR3 GetRotation(void) const;		// �����擾
	D3DXVECTOR3 GetScaling(void) const;			// �傫���擾
	D3DXCOLOR GetCubeColor(void) const;			// �L���[�u�F�擾
	D3DXCOLOR GetBorderColor(void) const;		// �����F�擾
	BORDERSTATE GetBorderState(void) const;		// ������Ԏ擾
	float GetBorderThick(void) const;			// ����葾���擾
	TEXSTATE GetTextureState(void) const;		// �e�N�X�`����Ԏ擾
	D3DXVECTOR2 GetTexturePatternX(void) const;	// �e�N�X�`�������� x�擾
	D3DXVECTOR2 GetTexturePatternY(void) const;	// �e�N�X�`�������� y�擾
	D3DXVECTOR2 GetTexturePatternZ(void) const;	// �e�N�X�`�������� z�擾
	D3DCULL GetCulling(void) const;				// �J�����O�擾
	bool GetLighting(void) const;				// ���C�e�B���O�擾

protected:
	// �����o�֐�
	void SetVtx(void);	// ���_���̐ݒ�
	void SetIdx(void);	// �C���f�b�N�X���̐ݒ�

private:
	// �ÓI�����o�ϐ�
	static const D3DXVECTOR3 aPosMeshCube[];	// ���_���W�̐ݒ�p�萔
	static const D3DXVECTOR3 aNorMeshCube[];	// �@���x�N�g���̐ݒ�p�萔
	static const D3DXCOLOR aColMeshCube[];		// ���_�J���[�̐ݒ�p�萔
	static const D3DXVECTOR2 aTexMeshCube[];	// �e�N�X�`�����W�̐ݒ�p�萔
	static const int aIdxMeshCube[];			// �C���f�b�N�X�̐ݒ�p�萔

	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	MeshCube m_meshCube;	// ���b�V���L���[�u�̏��
	int m_nNumVtx;			// �K�v���_��
	int m_nNumIdx;			// �K�v�C���f�b�N�X��
};

#endif	// _OBJECT_MESHCUBE_H_
