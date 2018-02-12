#include "Shape.h"
#include <iostream>

#include "GLSL.h"
#include "Program.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

using namespace std;


void Shape::loadMesh(const string &meshName)
{
	// Load geometry
	// Some obj files contain material information.
	// We'll ignore them for this assignment.
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> objMaterials;
	string errStr;
	bool rc = tinyobj::LoadObj(shapes, objMaterials, errStr, meshName.c_str());

	if (! rc)
	{
		cerr << errStr << endl;
	}
	else if (shapes.size())
	{
		posBuf = shapes[0].mesh.positions;
		norBuf = shapes[0].mesh.normals;
		texBuf = shapes[0].mesh.texcoords;
		eleBuf = shapes[0].mesh.indices;
	}
}

void Shape::resize()
{
	float minX, minY, minZ;
	float maxX, maxY, maxZ;
	float scaleX, scaleY, scaleZ;
	float shiftX, shiftY, shiftZ;
	float epsilon = 0.001f;

	minX = minY = minZ = 1.1754E+38F;
	maxX = maxY = maxZ = -1.1754E+38F;

	// Go through all vertices to determine min and max of each dimension
	for (size_t v = 0; v < posBuf.size() / 3; v++)
	{
		if (posBuf[3*v+0] < minX) minX = posBuf[3*v+0];
		if (posBuf[3*v+0] > maxX) maxX = posBuf[3*v+0];

		if (posBuf[3*v+1] < minY) minY = posBuf[3*v+1];
		if (posBuf[3*v+1] > maxY) maxY = posBuf[3*v+1];

		if (posBuf[3*v+2] < minZ) minZ = posBuf[3*v+2];
		if (posBuf[3*v+2] > maxZ) maxZ = posBuf[3*v+2];
	}

	// From min and max compute necessary scale and shift for each dimension
	float maxExtent, xExtent, yExtent, zExtent;
	xExtent = maxX-minX;
	yExtent = maxY-minY;
	zExtent = maxZ-minZ;
	if (xExtent >= yExtent && xExtent >= zExtent)
	{
		maxExtent = xExtent;
	}
	if (yExtent >= xExtent && yExtent >= zExtent)
	{
		maxExtent = yExtent;
	}
	if (zExtent >= xExtent && zExtent >= yExtent)
	{
		maxExtent = zExtent;
	}
	scaleX = 2.0f / maxExtent;
	shiftX = minX + (xExtent / 2.0f);
	scaleY = 2.0f / maxExtent;
	shiftY = minY + (yExtent / 2.0f);
	scaleZ = 2.0f / maxExtent;
	shiftZ = minZ + (zExtent / 2.0f);

	// Go through all verticies shift and scale them
	for (size_t v = 0; v < posBuf.size() / 3; v++)
	{
		posBuf[3*v+0] = (posBuf[3*v+0] - shiftX) * scaleX;
		assert(posBuf[3*v+0] >= -1.0f - epsilon);
		assert(posBuf[3*v+0] <= 1.0f + epsilon);
		posBuf[3*v+1] = (posBuf[3*v+1] - shiftY) * scaleY;
		assert(posBuf[3*v+1] >= -1.0f - epsilon);
		assert(posBuf[3*v+1] <= 1.0f + epsilon);
		posBuf[3*v+2] = (posBuf[3*v+2] - shiftZ) * scaleZ;
		assert(posBuf[3*v+2] >= -1.0f - epsilon);
		assert(posBuf[3*v+2] <= 1.0f + epsilon);
	}
        norBuf.resize(posBuf.size());
        for (size_t i = 0; i< posBuf.size();i++){
               norBuf[i] = 0;
        }
        for (size_t i = 0; i< eleBuf.size()/3;i++){//for each face
                int i1 = eleBuf[i*3];
                int i2 = eleBuf[i*3+1];
                int i3 = eleBuf[i*3+2];
                int v1x= i1*3;
                int v1y= i1*3+1;
                int v1z= i1*3+2;
                int v2x= i2*3;
                int v2y= i2*3+1;
                int v2z= i2*3+2;
                int v3x= i3*3;
                int v3y= i3*3+1;
                int v3z= i3*3+2;
                float v1[3] = {posBuf[v1x],posBuf[v1y],posBuf[v1z]};
                float v2[3] = {posBuf[v2x],posBuf[v2y],posBuf[v2z]};
                float v3[3] = {posBuf[v3x],posBuf[v3y],posBuf[v3z]};
                float a[3] = {v2[0]-v1[0],v2[1]-v1[1],v2[2]-v1[2]};
                float b[3] = {v3[0]-v1[0],v3[1]-v1[1],v3[2]-v1[2]};
                float nor[3] = {((a[1]*b[2]) - (a[2]*b[1])),((a[2]*b[0])-(a[0]*b[2])),((a[0]*b[1])-(a[1]*b[0]))};
                norBuf[v1x] += nor[0];
                norBuf[v1y] += nor[1];
                norBuf[v1z] += nor[2];
                norBuf[v2x] += nor[0];
                norBuf[v2y] += nor[1];
                norBuf[v2z] += nor[2];
                norBuf[v3x] += nor[0];
                norBuf[v3y] += nor[1];
                norBuf[v3z] += nor[2];
        }
}

void Shape::init()
{
	// Initialize the vertex array object
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	// Send the position array to the GPU
	glGenBuffers(1, &posBufID);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), posBuf.data(), GL_STATIC_DRAW);

	// Send the normal array to the GPU
	if (norBuf.empty())
	{
		norBufID = 0;
	}
	else
	{
		glGenBuffers(1, &norBufID);
		glBindBuffer(GL_ARRAY_BUFFER, norBufID);
		glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), norBuf.data(), GL_STATIC_DRAW);
	}

	// Send the texture array to the GPU
	if (texBuf.empty())
	{
		texBufID = 0;
	}
	else
	{
		glGenBuffers(1, &texBufID);
		glBindBuffer(GL_ARRAY_BUFFER, texBufID);
		glBufferData(GL_ARRAY_BUFFER, texBuf.size()*sizeof(float), texBuf.data(), GL_STATIC_DRAW);
	}

	// Send the element array to the GPU
	glGenBuffers(1, &eleBufID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, eleBuf.size()*sizeof(unsigned int), eleBuf.data(), GL_STATIC_DRAW);

	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	assert(glGetError() == GL_NO_ERROR);
}

void Shape::draw(const shared_ptr<Program> prog) const
{
	int h_pos, h_nor, h_tex;
	h_pos = h_nor = h_tex = -1;

	glBindVertexArray(vaoID);
	// Bind position buffer
	h_pos = prog->getAttribute("vertPos");
	GLSL::enableVertexAttribArray(h_pos);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);

	// Bind normal buffer
	h_nor = prog->getAttribute("vertNor");
	if (h_nor != -1 && norBufID != 0)
	{
		GLSL::enableVertexAttribArray(h_nor);
		glBindBuffer(GL_ARRAY_BUFFER, norBufID);
		glVertexAttribPointer(h_nor, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);
	}

	if (texBufID != 0)
	{
		// Bind texcoords buffer
		h_tex = prog->getAttribute("vertTex");
		if (h_tex != -1 && texBufID != 0)
		{
			GLSL::enableVertexAttribArray(h_tex);
			glBindBuffer(GL_ARRAY_BUFFER, texBufID);
			glVertexAttribPointer(h_tex, 2, GL_FLOAT, GL_FALSE, 0, (const void *)0);
		}
	}

	// Bind element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID);

	// Draw
	glDrawElements(GL_TRIANGLES, (int)eleBuf.size(), GL_UNSIGNED_INT, (const void *)0);

	// Disable and unbind
	if (h_tex != -1)
	{
		GLSL::disableVertexAttribArray(h_tex);
	}
	if (h_nor != -1)
	{
		GLSL::disableVertexAttribArray(h_nor);
	}
	GLSL::disableVertexAttribArray(h_pos);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
