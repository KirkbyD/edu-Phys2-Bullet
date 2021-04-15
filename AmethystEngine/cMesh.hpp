#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

struct sPlyVertexXYZ_N_UV {
	float x, y, z;
	float nx, ny, nz;
	float u, v;				// NOW with TEXTURE COORDINATES!!!

	// Add this, too:
	//float bi_x, bi_y, bi_z;
	//float tan_x, tan_y, tan_z;
	//float boneID[4];		// Which bone impacts this vertex
	//float boneWeights[4];	// How much does each bone
};

struct sPlyTriangle {
	unsigned int vert_index_1;
	unsigned int vert_index_2;
	unsigned int vert_index_3;
};

class cMesh {
public:
	std::vector<sPlyVertexXYZ_N_UV> vecVertices;
	std::vector<sPlyTriangle> vecTriangles;
};
