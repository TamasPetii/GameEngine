#include "PreviewRenderer.h"

void PreviewRenderer::Render(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto fbo = resourceManager->GetFbo("Preview");
	fbo->Bind();
	fbo->ActivateTexture(GL_COLOR_ATTACHMENT0);

	auto program = resourceManager->GetProgram("Preview");
	program->Bind();

	RenderShapePreviews(registry);
	RenderModelPreviews(registry);
	RenderMaterialPreviews(registry);

	program->UnBind();
	fbo->UnBind();
}

void PreviewRenderer::RenderShapePreviews(std::shared_ptr<Registry> registry)
{
	auto textureManager = TextureManager::Instance();
	auto resourceManager = ResourceManager::Instance();
	auto previewManager = PreviewManager::Instance();
	auto fbo = resourceManager->GetFbo("Preview");
	auto program = resourceManager->GetProgram("Preview");

	std::for_each(std::execution::seq, previewManager->RefShapePreviews().begin(), previewManager->RefShapePreviews().end(),
		[&](auto& preview) -> void {
			if (preview.second == nullptr)
			{
				fbo->Clear();
				fbo->Bind();

				//Rendering background
				glDisable(GL_DEPTH_TEST);
				glDepthMask(GL_FALSE);

				program->SetUniform("u_renderMode", (GLuint)2);
				program->SetTexture("u_bgTexture", 0, textureManager->LoadImageTexture("../Assets/PreviewBG.png")->GetTextureID());

				resourceManager->GetGeometry("Cube")->Bind();
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				resourceManager->GetGeometry("Cube")->UnBind();

				glEnable(GL_DEPTH_TEST);
				glDepthMask(GL_TRUE);

				//Rendering each built in gemoetry
				auto geometry = resourceManager->GetGeometry(preview.first);
				auto& origin = geometry->GetObbOrigin();
				auto& extents = geometry->GetObbExtents();

				float maxExtent = glm::max(extents.x, extents.y);
				float padding = 2;

				// Setup orthographic projection matrix
				glm::mat4 projectionMatrix = glm::ortho(
					-maxExtent * padding,  // left
					maxExtent * padding,   // right
					-maxExtent * padding,  // bottom
					maxExtent * padding,   // top
					0.01f,                  // near
					3 * extents.z * padding // far
				);

				// Camera remains the same as before
				glm::vec3 cameraPosition = origin + glm::vec3(maxExtent, maxExtent, 2 * extents.z);
				glm::vec3 target = origin;
				glm::vec3 up = glm::vec3(0, 1, 0);
				glm::mat4 viewMatrix = glm::lookAt(cameraPosition, target, up);

				program->SetUniform("u_renderMode", (GLuint)0);
				program->SetUniform("u_viewProj", projectionMatrix * viewMatrix);
				program->SetUniform("u_eye", cameraPosition);

				geometry->Bind();
				glDrawElements(GL_TRIANGLES, geometry->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
				geometry->UnBind();

				//Creating a new texture and copying frambuffer texture
				TextureSpecGL spec;
				spec.textureType = GL_TEXTURE_2D;
				spec.format = GL_RGB;
				spec.internalFormat = GL_RGB8;
				spec.type = GL_UNSIGNED_BYTE;
				spec.width = 128;
				spec.height = 128;

				std::shared_ptr<TextureGL> previewTexture = std::make_shared<TextureGL>(spec);
				previewTexture->TextureCopy2D(fbo->GetTextureID("preview"));
				preview.second = previewTexture;
			}
		}
	);
}

void PreviewRenderer::RenderModelPreviews(std::shared_ptr<Registry> registry)
{
	auto modelManager = ModelManager::Instance();
	auto textureManager = TextureManager::Instance();
	auto resourceManager = ResourceManager::Instance();
	auto previewManager = PreviewManager::Instance();
	auto fbo = resourceManager->GetFbo("Preview");
	auto program = resourceManager->GetProgram("Preview");

	std::for_each(std::execution::seq, previewManager->RefModelPreviews().begin(), previewManager->RefModelPreviews().end(),
		[&](auto& preview) -> void {
			if (preview.second == nullptr)
			{
				fbo->Clear();
				fbo->Bind();

				//Rendering background
				glDisable(GL_DEPTH_TEST);
				glDepthMask(GL_FALSE);

				program->SetUniform("u_renderMode", (GLuint)2);
				program->SetTexture("u_bgTexture", 0, textureManager->LoadImageTexture("../Assets/PreviewBG.png")->GetTextureID());

				resourceManager->GetGeometry("Cube")->Bind();
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				resourceManager->GetGeometry("Cube")->UnBind();

				glEnable(GL_DEPTH_TEST);
				glDepthMask(GL_TRUE);

				//Rendering each built in gemoetry
				auto model = modelManager->GetModel(preview.first);
				auto& origin = model->GetObbOrigin();
				auto& extents = model->GetObbExtents();

				float maxExtent = glm::max(extents.x, extents.y);
				float padding = 1.1;

				// Setup orthographic projection matrix
				glm::mat4 projectionMatrix = glm::ortho(
					-maxExtent * padding,  // left
					maxExtent * padding,   // right
					-maxExtent * padding,  // bottom
					maxExtent * padding,   // top
					0.01f,                  // near
					3 * extents.z * padding // far
				);

				// Camera remains the same as before
				glm::vec3 cameraPosition = origin + glm::vec3(0, 0, 2 * extents.z);
				glm::vec3 target = origin;
				glm::vec3 up = glm::vec3(0, 1, 0);
				glm::mat4 viewMatrix = glm::lookAt(cameraPosition, target, up);

				program->SetUniform("u_renderMode", (GLuint)1);
				program->SetUniform("u_viewProj", projectionMatrix * viewMatrix);
				program->SetUniform("u_eye", cameraPosition);

				model->GetMaterialSsbo()->BindBufferBase(0);
				model->Bind();
				glDrawElements(GL_TRIANGLES, model->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
				model->UnBind();

				//Creating a new texture and copying frambuffer texture
				TextureSpecGL spec;
				spec.textureType = GL_TEXTURE_2D;
				spec.format = GL_RGB;
				spec.internalFormat = GL_RGB8;
				spec.type = GL_UNSIGNED_BYTE;
				spec.width = 128;
				spec.height = 128;

				std::shared_ptr<TextureGL> previewTexture = std::make_shared<TextureGL>(spec);
				previewTexture->TextureCopy2D(fbo->GetTextureID("preview"));
				preview.second = previewTexture;
			}
		}
	);
}

void PreviewRenderer::RenderMaterialPreviews(std::shared_ptr<Registry> registry)
{
	
}