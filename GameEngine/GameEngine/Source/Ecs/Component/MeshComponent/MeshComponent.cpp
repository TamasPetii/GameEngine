#include "MeshComponent.h"

namespace Ecs
{
	MeshComponent::MeshComponent() :
		m_MeshSource(nullptr)
	{
	}

	MeshComponent::MeshComponent(const json& data)
	{
		DeSerialize(data);
	}

	MeshComponent::~MeshComponent()
	{
		if(m_MeshSource != nullptr)
			delete m_MeshSource;
	}

	json MeshComponent::Serialize() const
	{
		json data;

		if (auto model = dynamic_cast<Model*>(m_MeshSource))
		{
			data["MeshComponent"]["MeshSource"] = model->Serialize();
		}
		else if (auto shape = dynamic_cast<Shape*>(m_MeshSource))
		{
			data["MeshComponent"]["MeshSource"] = shape->Serialize();
		}
		else
		{
			data["MeshComponent"]["MeshSource"] = nullptr;
		}

		return data;
	}

	void MeshComponent::DeSerialize(const json& data)
	{
		auto meshSource = data["MeshSource"];

		if (meshSource.find("Model") != meshSource.end())
		{
			//...
		}
		else if (meshSource.find("Cube") != meshSource.end())
		{
			m_MeshSource = new Cube(meshSource["Cube"]);
		}
		else if (meshSource.find("Cylinder") != meshSource.end())
		{
			m_MeshSource = new Cylinder(meshSource["Cylinder"]);
		}
		else if (meshSource.find("Pyramid") != meshSource.end())
		{
			m_MeshSource = new Pyramid(meshSource["Pyramid"]);
		}
		else if (meshSource.find("Sphere") != meshSource.end())
		{
			m_MeshSource = new Sphere(meshSource["Sphere"]);
		}
		else if (meshSource.find("Torus") != meshSource.end())
		{
			m_MeshSource = new Torus(meshSource["Torus"]);
		}
		else if (meshSource.find("Plane") != meshSource.end())
		{
			m_MeshSource = new Plane(meshSource["Plane"]);
		}
		else if (meshSource.find("Canvas") != meshSource.end())
		{
			m_MeshSource = new Canvas(meshSource["Canvas"]);
		}
		else
		{
			m_MeshSource = nullptr;
		}
	}

	MeshComponent* MeshComponent::Clone() const
	{
		auto data = Serialize();
		return new MeshComponent(data["MeshComponent"]);
	}
}