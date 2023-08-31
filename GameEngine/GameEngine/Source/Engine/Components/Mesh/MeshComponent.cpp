#include "MeshComponent.h"
#include "MeshSource/Shapes/Cylinder.h"

MeshComponent::MeshComponent()
{
}

MeshComponent::MeshComponent(const json& object)
{
	LoadFromJson(object);
}

MeshComponent::~MeshComponent()
{
	if(mMesh != nullptr)
		delete mMesh;
}

void MeshComponent::Render()
{
	mMesh->Render();
}

json MeshComponent::SaveToJson() const 
{
	json data;
	data["MeshComponent"]["Mesh"] = dynamic_cast<MeshBase*>(mMesh)->name;
	data["MeshComponent"]["Material"]["Ambient"] = Vec3_ToJson(mMaterial.ambient);
	data["MeshComponent"]["Material"]["Diffuse"] = Vec3_ToJson(mMaterial.diffuse);
	data["MeshComponent"]["Material"]["Specular"] = Vec3_ToJson(mMaterial.specular);
	data["MeshComponent"]["Texture"]["Scale"] = mTextures.scale;

	if(mTextures.texture == nullptr)
		data["MeshComponent"]["Texture"]["Texture"] = nullptr;
	else
		data["MeshComponent"]["Texture"]["Texture"] = mTextures.texture->Get_Path();

	if (mTextures.normal == nullptr)
		data["MeshComponent"]["Texture"]["Normal"] = nullptr;
	else
		data["MeshComponent"]["Texture"]["Normal"] = mTextures.normal->Get_Path();

	if (mTextures.height == nullptr)
		data["MeshComponent"]["Texture"]["Height"] = nullptr;
	else
		data["MeshComponent"]["Texture"]["Height"] = mTextures.height->Get_Path();

	return data;
}

void MeshComponent::LoadFromJson(const json& object)
{
	std::string mesh = object["Mesh"];

	if (mesh == "Cube")
		mMesh = new Shape<Cube>;
	if (mesh == "Sphere")
		mMesh = new Shape<Sphere>;
	if (mesh == "Torus")
		mMesh = new Shape<Torus>;
	if (mesh == "Cylinder")
		mMesh = new Shape<Cylinder>;
	if (mesh == "Plane")
		mMesh = new Shape<Plane>;

	mMaterial.ambient = Vec3_FromJson(object["Material"]["Ambient"]);
	mMaterial.diffuse = Vec3_FromJson(object["Material"]["Diffuse"]);
	mMaterial.specular = Vec3_FromJson(object["Material"]["Specular"]);

	mTextures.scale = object["Texture"]["Scale"];
	mTextures.texture = object["Texture"]["Texture"].is_null() ? nullptr : ImageTexture::LoadImage(object["Texture"]["Texture"]);
	mTextures.normal = object["Texture"]["Normal"].is_null() ? nullptr : ImageTexture::LoadImage(object["Texture"]["Normal"]);
	mTextures.height = object["Texture"]["Height"].is_null() ? nullptr : ImageTexture::LoadImage(object["Texture"]["Height"]);
}