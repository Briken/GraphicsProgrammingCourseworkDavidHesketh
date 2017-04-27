#include "stdafx.h"
#include "ModelLoader.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>




void IndexedModel::CalculateNormals()
{
}

GameModel::GameModel(const std::string& name)
{
	hasUVs = false;
	hasNormals = false;
	std::ifstream file;
	file.open(name.c_str());

	std::string line;
	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);

			unsigned int lineLength = line.length();

			if (lineLength < 2)
				continue;

			const char* lineCStr = line.c_str();

			switch (lineCStr[0])
			{
			case 'v':
				if (lineCStr[1] == 't')
					this->uvs.push_back(ParseModelVec2(line));
				else if (lineCStr[1] == 'n')
					this->normal.push_back(ParseModelVec3(line));
				else if (lineCStr[1] == ' ' || lineCStr[1] == '\t')
					this->verts.push_back(ParseModelVec3(line));
				break;
			case 'f':
				CreateFace(line);
				break;
			default: break;
			};
		}
	}
	else
	{
		std::cerr << "Unable to load mesh: " << name << std::endl;
	}
}