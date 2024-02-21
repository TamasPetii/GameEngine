#include "ShadowRenderer.h"

<<<<<<< HEAD
void ShadowRenderer::Render(std::shared_ptr<Registry> registry)
{

};

void ShadowRenderer::RenderDirLightShadows(std::shared_ptr<Registry> registry)
{
	/*
	auto dirlightPool = m_Registry->GetComponentPool<DirlightComponent>();
	std::for_each(std::execution::seq, dirlightPool->GetDenseEntitiesArray().begin(), dirlightPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entityLight) -> void {
			auto& dirlightComponent = dirlightPool->GetComponent(entityLight);
			auto dirlightIndex = dirlightPool->GetIndex(entityLight);

			if (dirlightComponent.useShadow)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, dirlightComponent.shadowFramebuffer);
				glViewport(0, 0, dirlightComponent.shadowSize, dirlightComponent.shadowSize);
				glClear(GL_DEPTH_BUFFER_BIT);
				m_ResourceManager->GetSsbo("TransformData")->BindBufferBase(0);
				m_ResourceManager->GetSsbo("DirLightData")->BindBufferBase(1);
				auto program = m_ResourceManager->GetProgram("ShadowDir");
				program->Bind();
				program->SetUniform("u_lightIndex", dirlightIndex);

				auto shapePool = m_Registry->GetComponentPool<ShapeComponent>();
				std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
					[&](const Entity& entityShape) -> void {
						if (m_Registry->HasComponent<TransformComponent>(entityShape))
						{
							auto& shapeComponent = shapePool->GetComponent(entityShape);
							auto transformIndex = m_Registry->GetIndex<TransformComponent>(entityShape);
							auto shapeIndex = shapePool->GetIndex(entityShape);

							program->SetUniform("u_transformIndex", transformIndex);
							shapeComponent.shape->Bind();
							glDrawElements(GL_TRIANGLES, shapeComponent.shape->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
							shapeComponent.shape->UnBind();
						}
					}
				);

				program->UnBind();
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}
	);
	*/
}

void ShadowRenderer::RenderPointLightShadows(std::shared_ptr<Registry> registry)
{
	/*
		{
		auto pointLightPool = m_Registry->GetComponentPool<PointLightComponent>();
		std::for_each(std::execution::seq, pointLightPool->GetDenseEntitiesArray().begin(), pointLightPool->GetDenseEntitiesArray().end(),
			[&](const Entity& entityLight) -> void {
				auto& pointLightComponent = pointLightPool->GetComponent(entityLight);
				auto pointLightIndex = pointLightPool->GetIndex(entityLight);

				if (pointLightComponent.useShadow)
				{
					glBindFramebuffer(GL_FRAMEBUFFER, pointLightComponent.shadowFramebuffer);
					glViewport(0, 0, pointLightComponent.shadowSize, pointLightComponent.shadowSize);
					glClear(GL_DEPTH_BUFFER_BIT);
					m_ResourceManager->GetSsbo("TransformData")->BindBufferBase(0);
					m_ResourceManager->GetSsbo("PointLightData")->BindBufferBase(1);
					auto program = m_ResourceManager->GetProgram("ShadowPoint");
					program->Bind();
					program->SetUniform("u_lightIndex", pointLightIndex);

					auto shapePool = m_Registry->GetComponentPool<ShapeComponent>();
					std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
						[&](const Entity& entityShape) -> void {
							if (m_Registry->HasComponent<TransformComponent>(entityShape))
							{
								auto& shapeComponent = shapePool->GetComponent(entityShape);
								auto transformIndex = m_Registry->GetIndex<TransformComponent>(entityShape);
								auto shapeIndex = shapePool->GetIndex(entityShape);

								program->SetUniform("u_transformIndex", transformIndex);
								shapeComponent.shape->Bind();
								glDrawElements(GL_TRIANGLES, shapeComponent.shape->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
								shapeComponent.shape->UnBind();
							}

						}
					);

					program->UnBind();
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
				}
			}
		);
	}
	*/
}

void ShadowRenderer::RenderSpotLightShadows(std::shared_ptr<Registry> registry)
=======
void ShadowRenderer::RenderDirLightShadows()
{
}

void ShadowRenderer::RenderPointLightShadows()
{
}

void ShadowRenderer::RenderSpotLightShadows()
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
{
}
