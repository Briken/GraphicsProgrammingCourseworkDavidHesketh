#ifndef  MODEL_LOADER_H_INCLUDED
#define MODEL_LOADER_H_INCLUDED
#include <glm/glm.hpp>
#include <vector>
#include <string>

struct ModelIndex
{
	unsigned int vertexIndex;
	unsigned int uvIndex;
	unsigned int normalIndex;

	bool operator<(const ModelIndex& r) const { return vertexIndex < r.vertexIndex; }
};

class IndexedModel
{
public:
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	void CalculateNormals();
};

class GameModel
{
public:
	std::vector<ModelIndex> modelIndices;
	std::vector<glm::vec3> verts;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normal;
	bool hasUVs;
	bool hasNormals;
	
	GameModel(const std::string& name);
	
	IndexedModel ToIndexedModel();

private:
	unsigned int FindLastVertex(const std::vector<ModelIndex*>& lookup, const ModelIndex* currentIndex, const IndexedModel& result);
	void CreateFace(const std::string& line);
	glm::vec2 ParseModelVec2(const std::string& line);
	glm::vec3 ParseModelVec3(const std::string& line);
	ModelIndex ParseModelIndex(const std::string& token, bool* hasUVs, bool* hasNormals);
};
#endif // ! MODEL_LOADER_H_INCLUDED