#include "PipelineStateObject.h"
#include "InitDirectX.h"
#include "RootSignature.h"

namespace {
    // ���C�h�����ɕϊ�
    std::wstring ConvertString(const std::string& text) {
        if (text.empty()) {
            return std::wstring();
        }

        /*
         * ref: https://docs.microsoft.com/ja-JP/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar
         * ref: http://www.t-net.ne.jp/~cyfis/win_api/sdk/MultiByteToWideChar.html
         * ref: http://chokuto.ifdef.jp/urawaza/api/MultiByteToWideChar.html
         */
        int wcharNum =
            MultiByteToWideChar(CP_ACP, 0, text.c_str(), static_cast<int>(text.size()), NULL, 0);
        if (wcharNum == 0) {
            return std::wstring();
        }
        std::wstring result(wcharNum, 0);
        MultiByteToWideChar(
            CP_ACP, 0, text.c_str(), static_cast<int>(text.size()), &result[0], wcharNum);
        return result;
    }
}

void PipelineStateObject::LoadVertShader(std::string _vertShader)
{
    HRESULT result = S_FALSE;
    Comptr<ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g

    std::wstring shaderFile = ConvertString(_vertShader);

    // ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    result = D3DCompileFromFile(
        shaderFile.c_str(), // �V�F�[�_�t�@�C����
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
        "main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
        0,
        &vsBlob, &errorBlob);

    // �G���[�Ȃ�
    if (FAILED(result)) {
        // errorBlob����G���[���e��string�^�ɃR�s�[
        std::string error;
        error.resize(errorBlob->GetBufferSize());
        std::copy_n((char*)errorBlob->GetBufferPointer(),
                    errorBlob->GetBufferSize(),
                    error.begin());
        error += "\n";
        // �G���[���e���o�̓E�B���h�E�ɕ\��
        OutputDebugStringA(error.c_str());
        assert(0);
    }
}

void PipelineStateObject::LoadPixelShader(std::string _pixelShader)
{
    HRESULT result = S_FALSE;
    Comptr<ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g

    std::wstring shaderFile = ConvertString(_pixelShader);

    // �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
    result = D3DCompileFromFile(
        shaderFile.c_str(), // �V�F�[�_�t�@�C����
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
        "main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
        0,
        &psBlob, &errorBlob);

    // �G���[�Ȃ�
    if (FAILED(result)) {
        // errorBlob����G���[���e��string�^�ɃR�s�[
        std::string error;
        error.resize(errorBlob->GetBufferSize());
        std::copy_n((char*)errorBlob->GetBufferPointer(),
                    errorBlob->GetBufferSize(),
                    error.begin());
        error += "\n";
        // �G���[���e���o�̓E�B���h�E�ɕ\��
        OutputDebugStringA(error.c_str());
        assert(0);
    }
}

void PipelineStateObject::Initialize(std::string _vertShader, std::string _pixelShader)
{
    HRESULT result = S_FALSE;

    LoadVertShader(_vertShader);
    LoadPixelShader(_pixelShader);

    // �O���t�B�b�N�X�p�C�v���C���ݒ�
    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

    // �V�F�[�_�[�̐ݒ�
    pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
    pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
    pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
    pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

    // �T���v���}�X�N�̐ݒ�
    pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

    // ���X�^���C�U�̐ݒ�
    pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;   // �w�ʂ��J�����O
    pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;  // �|���S�����h��Ԃ�
    pipelineDesc.RasterizerState.DepthClipEnable = true;            // �[�x�N���b�s���O��L����

    // �u�����h�X�e�[�g
    /*pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;*/
    D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
    blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��

    // ���ʐݒ�(�A���t�@�l)
    blenddesc.BlendEnable = true;					// �u�����h��L���ɂ���
    blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	// ���Z
    blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		// �\�[�X�̒l��100%�g��
    blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	// �f�X�g�̒l��0%�g��

    // ���Z����(RGB�l)
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;		// ���Z
    blenddesc.SrcBlend = D3D12_BLEND_ONE;		// �\�[�X�̒l��100%�g��
    blenddesc.DestBlend = D3D12_BLEND_ONE;		// �f�X�g�̒l��100%�g��

    // ���Z����(RGB�l)
    blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;		// �f�X�g�J���\�[�X�������Z
    blenddesc.SrcBlend = D3D12_BLEND_ONE;					// �\�[�X�̒l��100%�g��
    blenddesc.DestBlend = D3D12_BLEND_ONE;					// �f�X�g�̒l��100%�g��

    // �F���](RGB�l)
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;					// ���Z
    blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;		// 1.0f - �f�X�g�J���[�̒l
    blenddesc.DestBlend = D3D12_BLEND_ZERO;					// �g��Ȃ�

    // ����������
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;					// ���Z
    blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;				// �\�[�X�̃A���t�@�l
    blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		// 1.0f - �\�[�X�̃A���t�@�l

    // ���_���C�A�E�g�̐ݒ�
    pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
    pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

    // �}�`�̌`��ݒ�
    pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    // ���̑��̐ݒ�
    pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
    pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
    pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

    // ���[�g�V�O�l�`����������
    GetInstanceRS()->Initialize();

    // �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
    pipelineDesc.pRootSignature = GetInstanceRS()->GetRootSignature();

    // �f�v�X�X�e���V���X�e�[�g�̐ݒ�
    pipelineDesc.DepthStencilState.DepthEnable = true;  // �[�x�e�X�g���s��
    pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;     // �������݋���
    pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;          // ��������΍��i
    pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

    // �p�C�v�����X�e�[�g�̐���
    result = GetInstanceIDX()->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
    assert(SUCCEEDED(result));
}

PipelineStateObject pipelineStateObject;
PipelineStateObject* GetInstancePSO()
{
    return &pipelineStateObject;
}