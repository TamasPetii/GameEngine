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

	auto wtDataSsbo = resourceManager->GetSsbo("WaterData");
	if (wtDataSsbo->GetBufferHandler() == nullptr)
		wtDataSsbo->MapBufferRange();
	WaterGLSL* wtDataSsboHandler = static_cast<WaterGLSL*>(wtDataSsbo->GetBufferHandler());

	if (!wtDataSsboHandler || waterPool->GetSize() > resourceManager->GetComponentSsboSize<WaterComponent>())
		return;

	std::for_each(std::execution::seq, waterPool->GetDenseEntitiesArray().begin(), waterPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			auto& waterComponent = waterPool->GetComponent(entity);
			auto waterIndex = waterPool->GetIndex(entity);

			if (transformPool->HasComponent(entity) && (waterPool->IsFlagSet(entity, UPDATE_FLAG) || transformPool->IsFlagSet(entity, CHANGED_FLAG)))
			{
				auto& transformComponent = transformPool->GetComponent(entity);
				waterComponent.plane.w = transformComponent.translate.y;
			}

			waterComponent.dudvMoveFactor += waterComponent.dudvMoveSpeed * deltaTime;
			waterComponent.dudvMoveFactor = fmod(waterComponent.dudvMoveFactor, 1.0f);
			wtDataSsboHandler[waterIndex] = WaterGLSL(waterComponent);
			waterPool->ResFlag(entity, UPDATE_FLAG);

			if (waterPool->IsFlagSet(entity, REGENERATE_FLAG))
			{
				TextureSpecGL textureSpec;
				textureSpec.attachment = GL_COLOR_ATTACHMENT0;
				textureSpec.textureType = GL_TEXTURE_2D;
				textureSpec.internalFormat = GL_RGBA16F;
				textureSpec.format = GL_RGBA;
				textureSpec.type = GL_FLOAT;
				textureSpec.generateHandler = false;
				textureSpec.generateMipMap = false;
				textureSpec.paramTextureFunction = defaultFboParamTextureFunction;

				TextureSpecGL depthTextureSpec;
				depthTextureSpec.attachment = GL_DEPTH_STENCIL_ATTACHMENT;
				depthTextureSpec.textureType = GL_TEXTURE_2D;
				depthTextureSpec.internalFormat = GL_DEPTH24_STENCIL8;
				depthTextureSpec.format = GL_DEPTH_STENCIL;
				depthTextureSpec.type = GL_UNSIGNED_INT_24_8;
				depthTextureSpec.generateHandler = false;
				depthTextureSpec.generateMipMap = false;
				depthTextureSpec.paramTextureFunction = defaultFboParamTextureFunction;

				waterComponent.reflectionFbo = std::make_shared<FramebufferGL>(waterComponent.size, waterComponent.size);
				waterComponent.reflectionFbo->AttachTexture("reflection", textureSpec);
				waterComponent.reflectionFbo->AttachTexture("depth", depthTextureSpec);
				waterComponent.reflectionFbo->CheckCompleteness();

				waterComponent.refractionFbo = std::make_shared<FramebufferGL>(waterComponent.size, waterComponent.size);
				waterComponent.refractionFbo->AttachTexture("refraction", textureSpec);
				waterComponent.refractionFbo->AttachTexture("depth", depthTextureSpec);
				waterComponent.refractionFbo->CheckCompleteness();

				waterPool->ResFlag(entity, REGENERATE_FLAG);
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

	std::string dudvPath = "none";
	if (waterComponent.dudv)
	{
		if (waterComponent.dudv->GetPath().substr(0, GlobalSettings::ProjectPath.size()) == GlobalSettings::ProjectPath)
			dudvPath = waterComponent.dudv->GetPath().substr(GlobalSettings::ProjectPath.size() + 1); //The +1 for deleting / from the start of the path
		else
			dudvPath = waterComponent.dudv->GetPath();
	}
	data["dudv"] = dudvPath;

	data["size"] = waterComponent.size;
	data["dudvScale"]["x"] = waterComponent.dudvScale.x;
	data["dudvScale"]["y"] = waterComponent.dudvScale.y;
	data["dudvWaveStrength"]["x"] = waterComponent.dudvWaveStrength.x;
	data["dudvWaveStrength"]["y"] = waterComponent.dudvWaveStrength.y;
	data["dudvMoveSpeed"] = waterComponent.dudvMoveSpeed;
	data["updateFrequency"] = waterComponent.updateFrequency;
	return data;
}

void WaterSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto textureManager = TextureManager::Instance();

	auto& waterComponent = registry->GetComponent<WaterComponent>(entity);
	waterComponent.dudv = data["dudv"] == "none" ? nullptr : textureManager->LoadImageTexture(static_cast<std::string>(data["dudv"]));
	waterComponent.size = data["size"];
	waterComponent.dudvScale.x = data["dudvScale"]["x"];
	waterComponent.dudvScale.y = data["dudvScale"]["y"];
	waterComponent.dudvWaveStrength.x = data["dudvWaveStrength"]["x"];
	waterComponent.dudvWaveStrength.y = data["dudvWaveStrength"]["y"];
	waterComponent.dudvMoveSpeed = data["dudvMoveSpeed"];

	if(data.find("updateFrequency") != data.end())
		waterComponent.updateFrequency = data["updateFrequency"];

	registry->SetFlag<WaterComponent>(entity, UPDATE_FLAG);
	registry->SetFlag<WaterComponent>(entity, REGENERATE_FLAG);
}