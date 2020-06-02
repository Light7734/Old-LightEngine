#include "PostProcessLayer.h"

PostProcessLayer::PostProcessLayer()
	: m_GrayscaleAttached(false), m_InverseAttached(false), m_KernelAttached(false)
{
	LT_PROFILE_FUNC();
	LT_TRACE("PostProcessLayer::PostProcessLayer");
	m_LayeDebugrName = "PostProcessLayer";

	// create framebuffers
	m_Grayscale = Light::Framebuffer::Create("res/FramebuffersVS.shader", "res/GrayscalePS.shader");
	m_Inverse = Light::Framebuffer::Create("res/FramebuffersVS.shader", "res/InversePS.shader");
	m_Kernel = Light::Framebuffer::Create("res/FramebuffersVS.shader", "res/KernelPS.shader");
	
	// create and set the kernel data
	m_KernelData = Light::ConstantBuffer::Create(Light::ConstantBufferIndex_ClientSlot0, sizeof(float) * 16);

	m_ConvolutionMatrix = glm::mat3(1.0f,  1.0f, 1.0f,
	                                1.0f, -8.0f, 1.0f,
	                                1.0f,  1.0f, 1.0f);

	float* map = (float*)m_KernelData->Map();

	memcpy(map, glm::value_ptr(m_ConvolutionMatrix), sizeof(glm::mat3));
	*(map + 12) = 1.0f / Light::GraphicsContext::GetResolution().width;
	*(map + 13) = 1.0f / Light::GraphicsContext::GetResolution().height;

	m_KernelData->UnMap();
}

void PostProcessLayer::OnAttach()
{
	LT_TRACE("PostProcessLayer::OnAttach");
}

void PostProcessLayer::OnDetatch()
{
	LT_TRACE("PostProcessLayer::OnDetatch");
}

void PostProcessLayer::ShowDebugWindow()
{
	ImGui::BulletText("**** order matters ****");

	// add or remove the specified framebuffers, order matters
	if (ImGui::Checkbox("grayscale", &m_GrayscaleAttached))
	{
		if (m_GrayscaleAttached)
			Light::Renderer::AddFramebuffer(m_Grayscale);

		if (!m_GrayscaleAttached)
			Light::Renderer::RemoveFramebuffer(m_Grayscale);
	}

	if (ImGui::Checkbox("inverse", &m_InverseAttached))
	{
		if (m_InverseAttached)
			Light::Renderer::AddFramebuffer(m_Inverse);

		if (!m_InverseAttached)
			Light::Renderer::RemoveFramebuffer(m_Inverse);
	}

	if (ImGui::Checkbox("kernel", &m_KernelAttached))
	{
		if (m_KernelAttached)
			Light::Renderer::AddFramebuffer(m_Kernel);

		if (!m_KernelAttached)
			Light::Renderer::RemoveFramebuffer(m_Kernel);
	}

	// change values of kernel effect's convolution matrix, for more information about kernel effects:
	//     https://setosa.io/ev/image-kernels/ and https://learnopengl.com/Advanced-OpenGL/Framebuffers.
	if (ImGui::TreeNode("kernel effect's convolution matrix"))
	{
		bool valueChanged = false;

		valueChanged += ImGui::DragFloat3(" top  ", &m_ConvolutionMatrix[0][0], 0.01f, NULL, NULL, "%.2f");
		valueChanged += ImGui::DragFloat3("center", &m_ConvolutionMatrix[1][0], 0.01f, NULL, NULL, "%.2f");
		valueChanged += ImGui::DragFloat3("bottom", &m_ConvolutionMatrix[2][0], 0.01f, NULL, NULL, "%.2f");

		if (valueChanged)
		{
			float* map = (float*)m_KernelData->Map();

			memcpy(map, glm::value_ptr(m_ConvolutionMatrix), sizeof(glm::mat3));
			*(map + 12) = 1.0f / Light::GraphicsContext::GetResolution().width;
			*(map + 13) = 1.0f / Light::GraphicsContext::GetResolution().height;

			m_KernelData->UnMap();
		}

		ImGui::TreePop();
	}
}

void PostProcessLayer::OnEvent(Light::Event& event)
{
	Light::Dispatcher dispatcher(event);
	dispatcher.Dispatch<Light::WindowResizedEvent>(LT_EVENT_FN(PostProcessLayer::OnWindowResize));
}

bool PostProcessLayer::OnWindowResize(Light::WindowResizedEvent& event)
{
	// resize framebuffers to the size of the window
	m_Grayscale->Resize(event.GetWidth(), event.GetHeight());
	m_Inverse->Resize(event.GetWidth(), event.GetHeight());
	m_Kernel->Resize(event.GetWidth(), event.GetHeight());

	return false;
}
