#include "MeshComponent.h"

MeshComponent::MeshComponent()
{
}

MeshComponent::MeshComponent(const json& object)
{
	LoadFromJson(object);
}

MeshComponent::~MeshComponent()
{
	if(m_RenderObject != nullptr)
		delete m_RenderObject;
}

void MeshComponent::Render()
{
	m_RenderObject->Render();
}

json MeshComponent::SaveToJson() const 
{
	json data;
	
	data["MeshComponent"]["Mesh"] = dynamic_cast<Shape*>(m_RenderObject)->Get_Name();
	data["MeshComponent"]["Material"]["Ambient"] = Vec3_ToJson(m_Material.ambient);
	data["MeshComponent"]["Material"]["Diffuse"] = Vec3_ToJson(m_Material.diffuse);
	data["MeshComponent"]["Material"]["Specular"] = Vec3_ToJson(m_Material.specular);
	data["MeshComponent"]["Texture"]["Scale"] = m_Textures.scale;

	if(m_Textures.texture == nullptr)
		data["MeshComponent"]["Texture"]["Texture"] = nullptr;
	else
		data["MeshComponent"]["Texture"]["Texture"] = m_Textures.texture->Get_Path();

	if (m_Textures.normal == nullptr)
		data["MeshComponent"]["Texture"]["Normal"] = nullptr;
	else
		data["MeshComponent"]["Texture"]["Normal"] = m_Textures.normal->Get_Path();

	if (m_Textures.height == nullptr)
		data["MeshComponent"]["Texture"]["Height"] = nullptr;
	else
		data["MeshComponent"]["Texture"]["Height"] = m_Textures.height->Get_Path();
	
	return data;
}

void MeshComponent::LoadFromJson(const json& object)
{
	
	std::string mesh = object["Mesh"];
	if (mesh == "Cube")
		m_RenderObject = new Cube();
	if (mesh == "Sphere")
		m_RenderObject = new Sphere();
	if (mesh == "Torus")
		m_RenderObject = new Torus();
	if (mesh == "Cylinder")
		m_RenderObject = new Cylinder();
	/*
	if (mesh == "Plane")
		m_RenderObject = new Shape<Plane>;
	*/

	m_Material.ambient = Vec3_FromJson(object["Material"]["Ambient"]);
	m_Material.diffuse = Vec3_FromJson(object["Material"]["Diffuse"]);
	m_Material.specular = Vec3_FromJson(object["Material"]["Specular"]);

	m_Textures.scale = object["Texture"]["Scale"];
	m_Textures.texture = object["Texture"]["Texture"].is_null() ? nullptr : ImageTexture::LoadImage(object["Texture"]["Texture"]);
	m_Textures.normal = object["Texture"]["Normal"].is_null() ? nullptr : ImageTexture::LoadImage(object["Texture"]["Normal"]);
	m_Textures.height = object["Texture"]["Height"].is_null() ? nullptr : ImageTexture::LoadImage(object["Texture"]["Height"]);
}