#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Image.h"

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;


int main(int argc, char **argv)
{
	// Variables for setup
	int scene_no = -1;
	int scene_size = -1;
	string output_file = "";



	// Checks if number of arguments is valid
	if(argc < 4) {
		cout << "Invalid Arguments" << endl;
		cout << "Usage: A6 <SCENE> <IMAGE SIZE> <OUTPUT FILE NAME>" << endl;
		return 0;
	}

	// Checks valid scene
	scene_no = atoi(argv[1]);
	if(scene_no < 0 || scene_no > 8) {
		cout << "Invalid scene" << endl;
		cout << "Expected 1-8" << endl;
		cout << "Got: " << scene_no << endl;
		return 0;
	}

	// Checks if scene width and height are valid
	scene_size = atoi(argv[2]);
	if(scene_size < 1) {
		cout << "Invalid scene size" << endl;
		cout << "Expected 1-(a lot)" << endl;
		cout << "Got: " << scene_size << endl;
		return 0;
	}

	output_file = argv[3];



	//initialize image object
	Image image(scene_size, scene_size);


	/*
	 * objs not needed as of yet
	// Load geometry
	vector<float> posBuf; // list of vertex positions
	vector<float> norBuf; // list of vertex normals
	vector<float> texBuf; // list of vertex texture coords
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
	if(!rc) {
		cerr << errStr << endl;
	} else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for(size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for(size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+0]);
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+1]);
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+2]);
					if(!attrib.normals.empty()) {
						norBuf.push_back(attrib.normals[3*idx.normal_index+0]);
						norBuf.push_back(attrib.normals[3*idx.normal_index+1]);
						norBuf.push_back(attrib.normals[3*idx.normal_index+2]);
					}
					if(!attrib.texcoords.empty()) {
						texBuf.push_back(attrib.texcoords[2*idx.texcoord_index+0]);
						texBuf.push_back(attrib.texcoords[2*idx.texcoord_index+1]);
					}
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
	*/


	// writes the image to the specified output file name
	image.writeToFile(output_file);
	return 0;
}
