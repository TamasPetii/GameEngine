#include "WaterSystem.h"

void WaterSystem::OnStart(std::shared_ptr<Registry> registry)
{
}

void WaterSystem::OnUpdate(std::shared_ptr<Registry> registry, float deltaTime)
{
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto waterPool = registry->GetComponentPool<WaterComponent>();

	if (!transformPool || !waterPool)
		return;

	static bool init = true;
	static WaterGLSL* wtDataSsboHandler = nullptr;
	if (init)
	{
		init = false;
		auto wtDataSsbo = resourceManager->GetSsbo("WaterData");
		wtDataSsboHandler = static_cast<WaterGLSL*>(wtDataSsbo->MapBuffer(GL_WRITE_ONLY));
	}

	std::for_each(std::execution::seq, waterPool->GetDenseEntitiesArray().begin(), waterPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if(!transformPool->HasComponent(entity))
				return;

			auto& waterComponent = waterPool->GetComponent(entity);
			auto waterIndex = waterPool->GetIndex(entity);

			if (waterPool->IsFlagSet(entity, REGENERATE_FLAG))
			{

				TextureFboSpecGL textureSpec;
				textureSpec.attachment = GL_COLOR_ATTACHMENT0;
				textureSpec.textureType = GL_TEXTURE_2D;
				textureSpec.internalFormat = GL_RGBA16F;
				textureSpec.format = GL_RGBA;
				textureSpec.type = GL_FLOAT;

				TextureFboSpecGL depthTextureSpec;
				depthTextureSpec.attachment = GL_DEPTH_STENCIL_ATTACHMENT;
				depthTextureSpec.textureType = GL_TEXTURE_2D;
				depthTextureSpec.internalFormat = GL_DEPTH24_STENCIL8;
				depthTextureSpec.format = GL_DEPTH_STENCIL;
				depthTextureSpec.type = GL_UNSIGNED_INT_24_8;

				waterComponent.reflectionFbo = std::make_shared<FramebufferGL>(waterComponent.size.x, waterComponent.size.y);
				waterComponent.reflectionFbo->AttachTexture("reflection", textureSpec);
				waterComponent.reflectionFbo->AttachTexture("depth", depthTextureSpec);
				waterComponent.reflectionFbo->CheckCompleteness();

				waterComponent.refractionFbo = std::make_shared<FramebufferGL>(waterComponent.size.x, waterComponent.size.y);
				waterComponent.refractionFbo->AttachTexture("refraction", textureSpec);
				waterComponent.refractionFbo->AttachTexture("depth", depthTextureSpec);
				waterComponent.refractionFbo->CheckCompleteness();

				waterPool->ResFlag(entity, REGENERATE_FLAG);
			}

			if (true || waterPool->IsFlagSet(entity, UPDATE_FLAG) || transformPool->IsFlagSet(entity, CHANGED_FLAG))
			{
				auto& transformComponent = transformPool->GetComponent(entity);

				waterComponent.plane.w = transformComponent.translate.y;
				waterComponent.dudvMoveFactor += waterComponent.dudvMoveSpeed * deltaTime;
				waterComponent.dudvMoveFactor = fmod(waterComponent.dudvMoveFactor, 1.0f);

				wtDataSsboHandler[waterIndex] = WaterGLSL(waterComponent);
				waterPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);

	//wtDataSsbo->UnMapBuffer();
}

void WaterSystem::OnEnd(std::shared_ptr<Registry> registry)
{

}

nlohmann::json WaterSystem::Serialize(Registry* registry, Entity entity)
{
	auto& waterComponent = registry->GetComponent<WaterComponent>(entity);

	nlohmann::json data;
	data["dudv"] = waterComponent.dudv ? waterComponent.dudv->GetPath() : "none";
	data["size"]["x"] = waterComponent.size.x;
	data["size"]["y"] = waterComponent.size.y;
	data["dudvScale"]["x"] = waterComponent.dudvScale.x;
	data["dudvScale"]["y"] = waterComponent.dudvScale.y;
	data["dudvWaveStrength"]["x"] = waterComponent.dudvWaveStrength.x;
	data["dudvWaveStrength"]["y"] = waterComponent.dudvWaveStrength.y;
	data["dudvMoveSpeed"] = waterComponent.dudvMoveSpeed;
	return data;
}

void WaterSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto textureManager = TextureManager::Instance();

	auto& waterComponent = registry->GetComponent<WaterComponent>(entity);
	waterComponent.dudv = data["dudv"] == "none" ? nullptr : textureManager->LoadImageTexture(static_cast<std::string>(data["dudv"]));
	waterComponent.size.x = data["size"]["x"];
	waterComponent.size.y = data["size"]["y"];
	waterComponent.dudvScale.x = data["dudvScale"]["x"];
	waterComponent.dudvScale.y = data["dudvScale"]["y"];
	waterComponent.dudvWaveStrength.x = data["dudvWaveStrength"]["x"];
	waterComponent.dudvWaveStrength.y = data["dudvWaveStrength"]["y"];
	waterComponent.dudvMoveSpeed = data["dudvMoveSpeed"];

	registry->SetFlag<WaterComponent>(entity, UPDATE_FLAG);
	registry->SetFlag<WaterComponent>(entity, REGENERATE_FLAG);
}