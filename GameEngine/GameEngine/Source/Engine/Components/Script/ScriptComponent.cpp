#include "ScriptComponent.h"

void ScriptComponent::GenerateScript(const std::string& name)
{
	GenerateHeader(name);
	GenerateBody(name);
}

void ScriptComponent::GenerateHeader(const std::string& name)
{
	std::string fullPath = "Scripts/" + name + ".h";
	std::ofstream file(fullPath);
	if (!file.is_open()) throw std::runtime_error("Couldn't open script file: " + fullPath);
	file << "#pragma once" << std::endl;
	file << "#include \"../Source/Engine/Components/Script/ScriptComponent.h\"" << std::endl << std::endl;
	file << "class " << name << " : public ScriptComponent" << std::endl;
	file << "{" << std::endl;
	file << "public:" << std::endl;
	file << "\t" << name << "(Entity* entity) : ScriptComponent(entity) {}" << std::endl;
	file << "\t" << "void OnCreate() override;" << std::endl;
	file << "\t" << "void OnUpdate() override;" << std::endl;
	file << "\t" << "void OnDelete() override;" << std::endl;
	file << "};" << std::endl;
	file.close();
}

void ScriptComponent::GenerateBody(const std::string& name)
{
	std::string fullPath = "Scripts/" + name + ".cpp";
	std::ofstream file(fullPath);
	if (!file.is_open()) throw std::runtime_error("Couldn't open script file: " + fullPath);
	file << "#include \"" << name << ".h\"" << std::endl << std::endl;

	file << "void " << name << "::OnCreate()" << std::endl;
	file << "{" << std::endl;
	file << "}" << std::endl << std::endl;

	file << "void " << name << "::OnUpdate()" << std::endl;
	file << "{" << std::endl;
	file << "}" << std::endl << std::endl;

	file << "void " << name << "::OnDelete()" << std::endl;
	file << "{" << std::endl;
	file << "}" << std::endl << std::endl;

	file.close();
}