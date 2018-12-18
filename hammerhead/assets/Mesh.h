#include <GL/glcorearb.h>
#include <glm/vec3.hpp>
#include <vector>

using namespace std;

class Mesh
{
public:
	Mesh(std::vector<GLfloat>&& vertices, std::vector<GLuint>&& indices, std::vector<GLfloat>&& normals, std::vector<GLuint>&& normalIndices);

private:
	vector<glm::vec3> vertices;
	vector<glm::vec3> uvs;
	vector<glm::vec3> normals;
	string name;
};

