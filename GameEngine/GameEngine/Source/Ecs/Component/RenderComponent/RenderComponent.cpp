#include "RenderComponent.h"

namespace Ecs
{
	RenderComponent::RenderComponent() :
		m_IsVisible(true),
		m_IsLightCaster(true),
	    m_IsShadowCaster(true)
	{

	}

	RenderComponent::RenderComponent(const json& data)
	{
		DeSerialize(data);
	}

	json RenderComponent::Serialize() const
	{
		json data;
		data["RenderComponent"]["IsVisible"] = m_IsVisible;
		data["RenderComponent"]["IsLightCaster"] = m_IsLightCaster;
		data["RenderComponent"]["IsShadowCaster"] = m_IsShadowCaster;

		data["RenderComponent"]["Material"]["Ambient"] = Vec3_ToJson(m_Material.ambient);
		data["RenderComponent"]["Material"]["Diffuse"] = Vec3_ToJson(m_Material.diffuse);
		data["RenderComponent"]["Material"]["Specular"] = Vec3_ToJson(m_Material.specular);
		data["RenderComponent"]["Texture"]["Scale"] = Vec3_ToJson(m_Texture.scale);

		if (m_Texture.diffuse == nullptr)
			data["RenderComponent"]["Texture"]["Diffuse"] = nullptr;
		else
			data["RenderComponent"]["Texture"]["Diffuse"] = m_Texture.diffuse->Get_Path();

		if (m_Texture.specular == nullptr)
			data["RenderComponent"]["Texture"]["Specular"] = nullptr;
		else
			data["RenderComponent"]["Texture"]["Specular"] = m_Texture.specular->Get_Path();

		if (m_Texture.normal == nullptr)
			data["RenderComponent"]["Texture"]["Normal"] = nullptr;
		else
			data["RenderComponent"]["Texture"]["Normal"] = m_Texture.normal->Get_Path();

		if (m_Texture.displacement == nullptr)
			data["RenderComponent"]["Texture"]["Displacement"] = nullptr;
		else
			data["RenderComponent"]["Texture"]["Displacement"] = m_Texture.displacement->Get_Path();

		return data;
	}

	void RenderComponent::DeSerialize(const json& data)
	{
		m_IsVisible = data["IsVisible"];
		m_IsLightCaster = data["IsLightCaster"];
		m_IsShadowCaster = data["IsShadowCaster"];
		m_Material.ambient = Vec3_FromJson(data["Material"]["Ambient"]);
		m_Material.diffuse = Vec3_FromJson(data["Material"]["Diffuse"]);
		m_Material.specular = Vec3_FromJson(data["Material"]["Specular"]);
		m_Texture.scale = Vec3_FromJson(data["Texture"]["Scale"]);
		m_Texture.diffuse = data["Texture"]["Diffuse"].is_null() ? nullptr : ImageTexture::LoadTexture(data["Texture"]["Diffuse"]);
		m_Texture.specular = data["Texture"]["Specular"].is_null() ? nullptr : ImageTexture::LoadTexture(data["Texture"]["Specular"]);
		m_Texture.normal = data["Texture"]["Normal"].is_null() ? nullptr : ImageTexture::LoadTexture(data["Texture"]["Normal"]);
		m_Texture.displacement = data["Texture"]["Displacement"].is_null() ? nullptr : ImageTexture::LoadTexture(data["Texture"]["Displacement"]);
	}

	RenderComponent* RenderComponent::Clone() const
	{
		json data = Serialize();
		return new RenderComponent(data["RenderComponent"]);
	}
}
