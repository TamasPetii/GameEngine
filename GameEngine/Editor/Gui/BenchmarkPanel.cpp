#include "BenchmarkPanel.h"
#include <ImGui_Glfw/imgui.h>
#include <ImGui_Glfw/imgui_impl_glfw.h>
#include <ImGui_Glfw/imgui_impl_opengl3.h>
#include <ImGui_Glfw/imgui_internal.h>

#include "Manager/ResourceManager.h"
#include "Settings/GlobalSettings.h"
#include "Benchmark/BenchmarkManager.h"

#include "Registry/System/Systems.h"
#include "Render/Renderer.h"
#include "Render/WaterRenderer.h"
#include "Render/BloomRenderer.h"
#include "Render/SkyboxRenderer.h"
#include "Render/ShadowRenderer.h"
#include "Render/PreviewRenderer.h"
#include "Render/GeometryRenderer.h"
#include "Render/DeferredRenderer.h"
#include "Render/BillboardRenderer.h"
#include "Render/WireframeRenderer.h"

#include "FilesystemPanel.h"
#include "ConsolePanel.h"
#include "EntitiesPanel.h"
#include "ComponentPanel.h"
#include "ViewportPanel.h"

void BenchmarkPanel::Render(std::shared_ptr<Scene> scene, std::unordered_map<std::type_index, double>& averagePanelTime)
{
	if (GlobalSettings::GameViewActive)
		return;

	if (ImGui::Begin(TITLE_BP("BenchmarkPanel")))
	{
		RenderGeometryStats();
		RenderSystemTimes(scene);
		RenderRenderTimes();
		RenderGuiTimes(averagePanelTime);
	}

	ImGui::End();
}

void BenchmarkPanel::RenderGeometryStats()
{
	if (ImGui::CollapsingHeader(TITLE_BP("Geometry Stats")))
	{
		ImGui::Text("Entity: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%d", BenchmarkManager::GetRenderedEntityCount());

		ImGui::Text("Instance: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%d", BenchmarkManager::GetRenderedInstanceCount());

		ImGui::Text("Triangles: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%d", BenchmarkManager::GetRenderedTriangleCount());

		BenchmarkManager::ClearRenderedEntityCount();
		BenchmarkManager::ClearRenderedInstanceCount();
		BenchmarkManager::ClearRenderedTriangleCount();
	}
}

void BenchmarkPanel::RenderSystemTimes(std::shared_ptr<Scene> scene)
{
	if (ImGui::CollapsingHeader(TITLE_BP("System Time Stats")))
	{
		auto& systemTimes = scene->GetSystemTimes();

		double allTime = 0;
		for (auto& [tpyeID, time] : systemTimes)
		{
			allTime += time;
		}

		ImGui::Text("All System Time = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%f ms", allTime);

		ImGui::SeparatorText("Physics simulation");

		ImGui::Text("Physics System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<PhysicsSystem>()]);

		ImGui::Text("Dynamic Rigidbody System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<RigidbodyDynamicSystem>()]);

		ImGui::Text("Static Rigidbody System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<RigidbodyStaticSystem>()]);

		ImGui::Text("Default Collider System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<DefaultColliderSystem>()]);

		ImGui::Text("Box Collider System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<BoxColliderSystem>()]);

		ImGui::Text("Sphere Collider System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<SphereColliderSystem>()]);

		ImGui::Text("Convex Collider System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<ConvexColliderSystem>()]);

		ImGui::Text("Mesh Collider System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<MeshColliderSystem>()]);

		ImGui::SeparatorText("Light update");

		ImGui::Text("Dir Light System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<DirlightSystem>()]);

		ImGui::Text("Point Light System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<PointLightSystem>()]);

		ImGui::Text("Spot Light System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<SpotLightSystem>()]);

		ImGui::Text("Shadow Culling System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<ShadowCullingSystem>()]);

		ImGui::SeparatorText("Geometry update");

		ImGui::Text("Shape System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<ShapeSystem>()]);

		ImGui::Text("Model System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<ModelSystem>()]);

		ImGui::Text("Animation System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<AnimationSystem>()]);

		ImGui::Text("Material System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<MaterialSystem>()]);

		ImGui::SeparatorText("Render preparation");

		ImGui::Text("Transform System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<TransformSystem>()]);

		ImGui::Text("Frustum Culling System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<FrustumCullingSystem>()]);

		ImGui::Text("Instance System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<InstanceSystem>()]);

		ImGui::Text("Water System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<WaterSystem>()]);

		ImGui::SeparatorText("Others");

		ImGui::Text("Audio System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<AudioSystem>()]);

		ImGui::Text("Script System = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", systemTimes[Unique::typeIndex<ScriptSystem>()]);
	}
}

void BenchmarkPanel::RenderRenderTimes()
{
	if (ImGui::CollapsingHeader(TITLE_BP("Render Time Stats")))
	{
		auto& renderTimes = Renderer::m_AverageRenderTimes;

		double allTime = 0;
		for (auto& [tpyeID, time] : renderTimes)
		{
			allTime += time;
		}

		ImGui::Text("All Render Time = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%f ms", allTime);

		ImGui::Separator();

		ImGui::Text("Shadow Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<ShadowRenderer>()]);

		ImGui::Text("Geometry Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<GeometryRenderer>()]);

		ImGui::Text("Deferred Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<DeferredRenderer>()]);

		ImGui::Text("Billboard Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<BillboardRenderer>()]);

		ImGui::Text("Wireframe Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<WireframeRenderer>()]);

		ImGui::Text("Skybox Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<SkyboxRenderer>()]);

		ImGui::Text("Bloom Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<BloomRenderer>()]);

		ImGui::Text("Water Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<WaterRenderer>()]);

		ImGui::Text("Preview Renderer = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", renderTimes[Unique::typeIndex<PreviewRenderer>()]);
	}
}

void BenchmarkPanel::RenderGuiTimes(std::unordered_map<std::type_index, double>& averagePanelTime)
{
	if (ImGui::CollapsingHeader(TITLE_BP("Render Gui Stats")))
	{
		double allTime = 0;
		for (auto& [tpyeID, time] : averagePanelTime)
		{
			allTime += time;
		}

		ImGui::Text("All Panel Time = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%f ms", allTime);

		ImGui::Separator();

		ImGui::Text("FilesystemPanel = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", averagePanelTime[Unique::typeIndex<FilesystemPanel>()]);

		ImGui::Text("ConsolePanel = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", averagePanelTime[Unique::typeIndex<ConsolePanel>()]);

		ImGui::Text("EntitiesPanel = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", averagePanelTime[Unique::typeIndex<EntitiesPanel>()]);

		ImGui::Text("ComponentPanel = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", averagePanelTime[Unique::typeIndex<ComponentPanel>()]);

		ImGui::Text("ViewportPanel = ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.75, 0.75, 0.75, 1), "%f ms", averagePanelTime[Unique::typeIndex<ViewportPanel>()]);
	}
}