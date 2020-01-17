#include "ltpch.h"
#include "dxBufferLayout.h"

#include "dxGraphicsContext.h"
#include "dxShader.h"
#include "dxVertexLayout.h"

namespace Light {

	dxBufferLayout::dxBufferLayout(std::shared_ptr<Shader> shader, std::shared_ptr<VertexLayout> layout)
	{
		std::shared_ptr<dxVertexLayout> dxpLayout = std::dynamic_pointer_cast<dxVertexLayout>(layout);
		std::shared_ptr<dxShader> dxpShader = std::dynamic_pointer_cast<dxShader>(shader);

		LT_CORE_ASSERT(dxpLayout, "Failed to cast VertexLayout to dxVertexLayout");
		LT_CORE_ASSERT(dxpShader, "Failed to cast Shader to dxShader");

		dxGraphicsContext::GetDevice()->CreateInputLayout(dxpLayout->GetElements(), dxpLayout->GetElementsCount(),
		                                                  dxpShader->GetVertexBlob().Get()->GetBufferPointer(),
		                                                  dxpShader->GetVertexBlob().Get()->GetBufferSize(), &m_InputLayout);
	}

	void dxBufferLayout::Bind()
	{
		dxGraphicsContext::GetDeviceContext()->IASetInputLayout(m_InputLayout.Get());
	}

}