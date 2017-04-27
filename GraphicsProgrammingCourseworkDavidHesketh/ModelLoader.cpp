#include "stdafx.h"
#include "ModelLoader.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

static bool CompareModelIndexPtr(const ModelIndex* a, const ModelIndex* b);
static inline unsigned int FindNextChar(unsigned int start, const char* str, unsigned int length, char token);
static inline unsigned int ParseModelIndexValue(const std::string& token, unsigned int start, unsigned int end);
static inline float ParseModelFloatValue(const std::string& token, unsigned int start, unsigned int end);
static inline std::vector<std::string> SplitString(const std::string &s, char delim);



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
void IndexedModel::CalculateNormals()
{
	for (unsigned int i = 0; i < indices.size(); i += 3)
	{
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];

		glm::vec3 v1 = positions[i1] - positions[i0];
		glm::vec3 v2 = positions[i2] - positions[i0];

		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

		normals[i0] += normal;
		normals[i1] += normal;
		normals[i2] += normal;
	}

	for (unsigned int i = 0; i < positions.size(); i++)
		normals[i] = glm::normalize(normals[i]);
}

IndexedModel GameModel::ToIndexedModel()
{
	IndexedModel result;
	IndexedModel normalModel;

	unsigned int numIndices = modelIndices.size();

	std::vector<ModelIndex*> indexLookup;

	for (unsigned int i = 0; i < numIndices; i++)
		indexLookup.push_back(&modelIndices[i]);

	std::sort(indexLookup.begin(), indexLookup.end(), CompareModelIndexPtr);

	std::map<ModelIndex, unsigned int> normalModelIndexMap;
	std::map<unsigned int, unsigned int> indexMap;

	for (unsigned int i = 0; i < numIndices; i++)
	{
		ModelIndex* currentIndex = &modelIndices[i];

		glm::vec3 currentPosition = verts[currentIndex->vertexIndex];
		glm::vec2 currentTexCoord;
		glm::vec3 currentNormal;

		if (hasUVs)
			currentTexCoord = uvs[currentIndex->uvIndex];
		else
			currentTexCoord = glm::vec2(0, 0);

		if (hasNormals)
			currentNormal = normal[currentIndex->normalIndex];
		else
			currentNormal = glm::vec3(0, 0, 0);

		unsigned int normalModelIndex;
		unsigned int resultModelIndex;

		//Create model to properly generate normals on
		std::map<ModelIndex, unsigned int>::iterator it = normalModelIndexMap.find(*currentIndex);
		if (it == normalModelIndexMap.end())
		{
			normalModelIndex = normalModel.positions.size();

			normalModelIndexMap.insert(std::pair<ModelIndex, unsigned int>(*currentIndex, normalModelIndex));
			normalModel.positions.push_back(currentPosition);
			normalModel.texCoords.push_back(currentTexCoord);
			normalModel.normals.push_back(currentNormal);
		}
		else
			normalModelIndex = it->second;

		//Create model which properly separates texture coordinates
		unsigned int previousVertexLocation = FindLastVertex(indexLookup, currentIndex, result);

		if (previousVertexLocation == (unsigned int)-1)
		{
			resultModelIndex = result.positions.size();

			result.positions.push_back(currentPosition);
			result.texCoords.push_back(currentTexCoord);
			result.normals.push_back(currentNormal);
		}
		else
			resultModelIndex = previousVertexLocation;

		normalModel.indices.push_back(normalModelIndex);
		result.indices.push_back(resultModelIndex);
		indexMap.insert(std::pair<unsigned int, unsigned int>(resultModelIndex, normalModelIndex));
	}

	if (!hasNormals)
	{
		normalModel.CalculateNormals();

		for (unsigned int i = 0; i < result.positions.size(); i++)
			result.normals[i] = normalModel.normals[indexMap[i]];
	}

	return result;
};

unsigned int GameModel::FindLastVertex(const std::vector<ModelIndex*>& indexLookup, const ModelIndex* currentIndex, const IndexedModel& result)
{
	unsigned int start = 0;
	unsigned int end = indexLookup.size();
	unsigned int current = (end - start) / 2 + start;
	unsigned int previous = start;

	while (current != previous)
	{
		ModelIndex* testIndex = indexLookup[current];

		if (testIndex->vertexIndex == currentIndex->vertexIndex)
		{
			unsigned int countStart = current;

			for (unsigned int i = 0; i < current; i++)
			{
				ModelIndex* possibleIndex = indexLookup[current - i];

				if (possibleIndex == currentIndex)
					continue;

				if (possibleIndex->vertexIndex != currentIndex->vertexIndex)
					break;

				countStart--;
			}

			for (unsigned int i = countStart; i < indexLookup.size() - countStart; i++)
			{
				ModelIndex* possibleIndex = indexLookup[current + i];

				if (possibleIndex == currentIndex)
					continue;

				if (possibleIndex->vertexIndex != currentIndex->vertexIndex)
					break;
				else if ((!hasUVs || possibleIndex->uvIndex == currentIndex->uvIndex)
					&& (!hasNormals || possibleIndex->normalIndex == currentIndex->normalIndex))
				{
					glm::vec3 currentPosition = verts[currentIndex->vertexIndex];
					glm::vec2 currentTexCoord;
					glm::vec3 currentNormal;

					if (hasUVs)
						currentTexCoord = uvs[currentIndex->uvIndex];
					else
						currentTexCoord = glm::vec2(0, 0);

					if (hasNormals)
						currentNormal = normal[currentIndex->normalIndex];
					else
						currentNormal = glm::vec3(0, 0, 0);

					for (unsigned int j = 0; j < result.positions.size(); j++)
					{
						if (currentPosition == result.positions[j]
							&& ((!hasUVs || currentTexCoord == result.texCoords[j])
								&& (!hasNormals || currentNormal == result.normals[j])))
						{
							return j;
						}
					}
				}
			}

			return -1;
		}
		else
		{
			if (testIndex->vertexIndex < currentIndex->vertexIndex)
				start = current;
			else
				end = current;
		}

		previous = current;
		current = (end - start) / 2 + start;
	}

	return -1;
}

void GameModel::CreateFace(const std::string& line)
{
	std::vector<std::string> tokens = SplitString(line, ' ');

	this->modelIndices.push_back(ParseModelIndex(tokens[1], &this->hasUVs, &this->hasNormals));
	this->modelIndices.push_back(ParseModelIndex(tokens[2], &this->hasUVs, &this->hasNormals));
	this->modelIndices.push_back(ParseModelIndex(tokens[3], &this->hasUVs, &this->hasNormals));

	if ((int)tokens.size() > 4)
	{
		this->modelIndices.push_back(ParseModelIndex(tokens[1], &this->hasUVs, &this->hasNormals));
		this->modelIndices.push_back(ParseModelIndex(tokens[3], &this->hasUVs, &this->hasNormals));
		this->modelIndices.push_back(ParseModelIndex(tokens[4], &this->hasUVs, &this->hasNormals));
	}
}

ModelIndex GameModel::ParseModelIndex(const std::string& token, bool* hasUVs, bool* hasNormals)
{
	unsigned int tokenLength = token.length();
	const char* tokenString = token.c_str();

	unsigned int vertIndexStart = 0;
	unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

	ModelIndex result;
	result.vertexIndex = ParseModelIndexValue(token, vertIndexStart, vertIndexEnd);
	result.uvIndex = 0;
	result.normalIndex = 0;

	if (vertIndexEnd >= tokenLength)
		return result;

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

	result.uvIndex = ParseModelIndexValue(token, vertIndexStart, vertIndexEnd);
	*hasUVs = true;

	if (vertIndexEnd >= tokenLength)
		return result;

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

	result.normalIndex = ParseModelIndexValue(token, vertIndexStart, vertIndexEnd);
	*hasNormals = true;

	return result;
}

glm::vec3 GameModel::ParseModelVec3(const std::string& line)
{
	unsigned int tokenLength = line.length();
	const char* tokenString = line.c_str();

	unsigned int vertIndexStart = 2;

	while (vertIndexStart < tokenLength)
	{
		if (tokenString[vertIndexStart] != ' ')
			break;
		vertIndexStart++;
	}

	unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float x = ParseModelFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float y = ParseModelFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float z = ParseModelFloatValue(line, vertIndexStart, vertIndexEnd);

	return glm::vec3(x, y, z);

	//glm::vec3(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()))
}

glm::vec2 GameModel::ParseModelVec2(const std::string& line)
{
	unsigned int tokenLength = line.length();
	const char* tokenString = line.c_str();

	unsigned int vertIndexStart = 3;

	while (vertIndexStart < tokenLength)
	{
		if (tokenString[vertIndexStart] != ' ')
			break;
		vertIndexStart++;
	}

	unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float x = ParseModelFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float y = ParseModelFloatValue(line, vertIndexStart, vertIndexEnd);

	return glm::vec2(x, y);
}

static bool CompareModelIndexPtr(const ModelIndex* a, const ModelIndex* b)
{
	return a->vertexIndex < b->vertexIndex;
}

static inline unsigned int FindNextChar(unsigned int start, const char* str, unsigned int length, char token)
{
	unsigned int result = start;
	while (result < length)
	{
		result++;
		if (str[result] == token)
			break;
	}

	return result;
}

static inline unsigned int ParseModelIndexValue(const std::string& token, unsigned int start, unsigned int end)
{
	return atoi(token.substr(start, end - start).c_str()) - 1;
}

static inline float ParseModelFloatValue(const std::string& token, unsigned int start, unsigned int end)
{
	return atof(token.substr(start, end - start).c_str());
}

static inline std::vector<std::string> SplitString(const std::string &s, char delim)
{
	std::vector<std::string> elems;

	const char* cstr = s.c_str();
	unsigned int strLength = s.length();
	unsigned int start = 0;
	unsigned int end = 0;

	while (end <= strLength)
	{
		while (end <= strLength)
		{
			if (cstr[end] == delim)
				break;
			end++;
		}

		elems.push_back(s.substr(start, end - start));
		start = end + 1;
		end = start;
	}

	return elems;
}
