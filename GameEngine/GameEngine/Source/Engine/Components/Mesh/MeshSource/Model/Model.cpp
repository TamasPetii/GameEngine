#include "Model.h"

Model* Model::LoadObject(const std::string& path)
{
	std::ifstream file(path);
	if (!file.is_open()) throw std::runtime_error("No such .obj file can be parsed: " + path);

	Model* model = new Model();

	std::vector<glm::vec3> mPositions;
	std::vector<glm::vec3> mNormals;
	std::vector<glm::vec2> mTextures;

	std::istringstream ss;
	std::string line;
	int index = 0;

	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string type;
		ss >> type;

		if (type == "v")
		{
			int x, y, z;
			ss >> x >> y >> z;
			mPositions.push_back(glm::vec3(x,y,z));
		}
		else if (type == "vt")
		{
			int x, y;
			ss >> x >> y;
			mTextures.push_back(glm::vec2(x, y));
		}
		else if (type == "vn")
		{
			int x, y, z;
			ss >> x >> y >> z;
			mNormals.push_back(glm::vec3(x, y, z));
		}
		else if (type == "f")
		{
			for (int i = 0; i < 3; i++)
			{
				std::string x, y, z;
				std::getline(ss, x, '/');
				std::getline(ss, y, '/');
				ss >> z;

				int indX, indY, indZ;
				indX = stoi(x) - 1;
				indY = stoi(y) - 1;
				indZ = stoi(z) - 1;

				model->mIndices.push_back(index++);
				model->mVertices.push_back(Vertex(mPositions[indX], mNormals[indZ], mTextures[indY]));
			}
		}
	}

	model->UploadBuffersToGpu();

	return model;
}