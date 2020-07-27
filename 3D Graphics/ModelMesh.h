

// EDIT
#include "Utils.h"
#include "Camera.h"
// EDIT END

struct Vertex {
	
	glm::vec3 Position; // Position	
	glm::vec3 Normal; // Normal	
	glm::vec2 TexCoords; // TexCoords
};

struct MeshTexture {
	GLuint id = 0;
	string type;
	aiString path;
};

class ModelMesh {
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<MeshTexture> Textures;

	/*  Functions  */
	// Constructor
	ModelMesh(vector<Vertex> vertices, vector<GLuint> indices, vector<MeshTexture> Textures);

	// Render the mesh
	void Render(CCamera* camera, GLuint program);

	void Update(mat4 model);

private:
	/*  Render data  */
	GLuint VAO, VBO, EBO;
	mat4 model;
	mat4 m_Scale;
	mat4 mvp;
	vec3 m_v3ObjPosition;
	mat4 m_m4TranslationMatrix;
	vec3 m_v3RotationAxisZ;
	float rotationAngle;
	mat4 m_m4RotationZ;
	vec3 m_v3ObjScale;
	
	mat4 m_m4ScaleMatrix;

	/*  Functions    */
	// Initializes all the buffer objects/arrays
	void setupMesh();
};