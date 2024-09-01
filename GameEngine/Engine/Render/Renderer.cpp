#include "Renderer.h"

std::unordered_map<std::type_index, double> Renderer::m_RenderTimes;
std::unordered_map<std::type_index, double> Renderer::m_AverageRenderTimes;

void Renderer::RenderScene(std::shared_ptr<Scene> scene, float deltaTime)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	auto resourceManager = ResourceManager::Instance();

	auto fbo = resourceManager->GetFbo("Main");
	fbo->Clear();
	fbo->Bind();

	{ // Shadow Renderer
		auto start = std::chrono::high_resolution_clock::now();
		ShadowRenderer::Render(scene->GetRegistry());
		auto end = std::chrono::high_resolution_clock::now();
		m_RenderTimes[Unique::typeIndex<ShadowRenderer>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Geometry Renderer
		auto start = std::chrono::high_resolution_clock::now();
		GeometryRenderer::Render(scene->GetRegistry());
		auto end = std::chrono::high_resolution_clock::now();
		m_RenderTimes[Unique::typeIndex<GeometryRenderer>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Deferred Renderer
		auto start = std::chrono::high_resolution_clock::now();
		DeferredRenderer::Render(scene->GetRegistry());
		auto end = std::chrono::high_resolution_clock::now();
		m_RenderTimes[Unique::typeIndex<DeferredRenderer>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Billboard Renderer
		auto start = std::chrono::high_resolution_clock::now();
		BillboardRenderer::Render(scene->GetRegistry());
		auto end = std::chrono::high_resolution_clock::now();
		m_RenderTimes[Unique::typeIndex<BillboardRenderer>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Wireframe Renderer
		auto start = std::chrono::high_resolution_clock::now();
		WireframeRenderer::Render(scene->GetRegistry());
		auto end = std::chrono::high_resolution_clock::now();
		m_RenderTimes[Unique::typeIndex<WireframeRenderer>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ //WaterRenderer
		auto start = std::chrono::high_resolution_clock::now();
		WaterRenderer::Render(scene->GetRegistry(), scene->GetMainCamera());
		auto end = std::chrono::high_resolution_clock::now();
		m_RenderTimes[Unique::typeIndex<WaterRenderer>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Skybox Renderer
		auto start = std::chrono::high_resolution_clock::now();
		SkyboxRenderer::Render(scene->GetRegistry(), scene->GetMainCamera());
		auto end = std::chrono::high_resolution_clock::now();
		m_RenderTimes[Unique::typeIndex<SkyboxRenderer>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Bloom Renderer
		auto start = std::chrono::high_resolution_clock::now();
		BloomRenderer::Render(scene->GetRegistry());
		auto end = std::chrono::high_resolution_clock::now();
		m_RenderTimes[Unique::typeIndex<BloomRenderer>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Preview Renderer
		auto start = std::chrono::high_resolution_clock::now();
		PreviewRenderer::Render(scene->GetRegistry(), deltaTime);
		auto end = std::chrono::high_resolution_clock::now();
		m_RenderTimes[Unique::typeIndex<PreviewRenderer>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (GlobalSettings::GameViewActive && !GlobalSettings::UseDockingSpace)
	{
		//glClearColor(0, 0, 0, 1);
		//glClear(GL_COLOR_BUFFER_BIT);
		//TODO
	}


	static float time = 0;
	static int counter = 0;
	time += deltaTime;
	counter++;

	if (time > 1)
	{
		for (auto& timeData : m_RenderTimes)
		{
			m_AverageRenderTimes[timeData.first] = timeData.second / counter;
			timeData.second = 0;
		}

		time = 0;
		counter = 0;
	}
}