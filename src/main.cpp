#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "image/Image.h"
#include "matrix_stack/MatrixStack.h"
#include "camera/Camera.h"
#include "scene_object/SceneOBJ.h"
#include "scene_object/Sphere.h"
#include "scene_object/Plane.h"
#include "scene_object/Ellipsoid.h"
#include "scene_object/RFSphere.h"
#include "scene_object/Mesh.h"
#include "scene/Scene.h"

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;


void populateOBJ(vector<float>& posBuf, vector<float>& norBuf, vector<float>& texBuf, string meshName, glm::vec3& center, float& radius) {
	float minX = FLT_MAX;
	float minY = FLT_MAX;
	float minZ = FLT_MAX;
	float maxX = FLT_MIN;
	float maxY = FLT_MIN;
	float maxZ = FLT_MIN;
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
					maxX = max(maxX, attrib.vertices[3*idx.vertex_index+0]);
					minX = min(minX, attrib.vertices[3*idx.vertex_index+0]);
					maxY = max(maxY, attrib.vertices[3*idx.vertex_index+1]);
					minY = min(minY, attrib.vertices[3*idx.vertex_index+1]);
					maxZ = max(maxZ, attrib.vertices[3*idx.vertex_index+2]);
					minZ = min(minZ, attrib.vertices[3*idx.vertex_index+2]);
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
	center.x = 0.5*(maxX + minX);
	center.y = 0.5*(maxY + minY);
	center.z = 0.5*(maxZ + minZ);
	radius = glm::length(glm::vec3(0.5*(maxX-minX), 0.5*(maxY-minY), 0.5*(maxZ-minZ)) - glm::vec3(0.0f))+ 1e-5;
}


// Massive mess of a function
// Needed for hard coded portion of assignment
void generateScene(Scene& scene, int scene_no)
{
	switch(scene_no) {
		case 8:
		case 1:
		case 2:
			// Red Sphere
			{
				glm::vec3 pos(-0.5f, -1.0f, 1.0f);
				glm::vec3 scale(1.0f, 1.0f, 1.0f);
				glm::vec4 rotation(0.0f, 0.0f, 0.0f, 0.0f);
				glm::vec3 diffuse(1.0f, 0.0f, 0.0f);
				glm::vec3 specular(1.0f, 1.0f, 0.5f);
				glm::vec3 ambient(0.1f, 0.1f, 0.1f);
				double exponent = 100.0;
				Sphere* sph = new Sphere(pos, 1.0f, scale, rotation, ambient, diffuse, specular, exponent);
				SceneOBJ* _obj = sph;
				scene.objs.push_back(_obj);
			}
			// Green Sphere
			{
				glm::vec3 pos(0.5f, -1.0f, -1.0f);
				glm::vec3 scale(1.0f, 1.0f, 1.0f);
				glm::vec4 rotation(0.0f, 0.0f, 0.0f, 0.0f);
				glm::vec3 diffuse(0.0f, 1.0f, 0.0f);
				glm::vec3 specular(1.0f, 1.0f, 0.5f);
				glm::vec3 ambient(0.1f, 0.1f, 0.1f);
				double exponent = 100.0;
				Sphere* sph = new Sphere(pos, 1.0f, scale, rotation, ambient, diffuse, specular, exponent);
				SceneOBJ* _obj = sph;
				scene.objs.push_back(_obj);
			}
			// Blue Sphere
			{
				glm::vec3 pos(0.0f, 1.0f, 0.0f);
				glm::vec3 scale(1.0f, 1.0f, 1.0f);
				glm::vec4 rotation(0.0f, 0.0f, 0.0f, 0.0f);
				glm::vec3 diffuse(0.0f, 0.0f, 1.0f);
				glm::vec3 specular(1.0f, 1.0f, 0.5f);
				glm::vec3 ambient(0.1f, 0.1f, 0.1f);
				double exponent = 100.0;
				Sphere* sph = new Sphere(pos, 1.0f, scale, rotation, ambient, diffuse, specular, exponent);
				SceneOBJ* _obj = sph;
				scene.objs.push_back(_obj);
			}
			// Singular light
			{
				glm::vec3 pos(-2.0f, 1.0f, 1.0f);
				float col = 1.0f;
				Light* lh = new Light(col, pos);
				scene.lights.push_back(lh);

			}
		break;
		case 3:
			// Green sphere
			{
				glm::vec3 pos(-0.5f, 0.0f, -0.5f);
				glm::vec3 scale(1.0f, 1.0f, 1.0f);
				glm::vec4 rotation(0.0f, 0.0f, 0.0f, 0.0f);
				glm::vec3 diffuse(0.0f, 1.0f, 0.0f);
				glm::vec3 specular(1.0f, 1.0f, 0.5f);
				glm::vec3 ambient(0.1f, 0.1f, 0.1f);
				double exponent = 100.0;
				Sphere* sph = new Sphere(pos, 1.0f, scale, rotation, ambient, diffuse, specular, exponent);
				SceneOBJ* _obj = sph;
				scene.objs.push_back(_obj);
			}
			// Plane
			{
				glm::vec3 nor(0.0f, 1.0f, 0.0f);
				glm::vec3 pos(0.0f, -1.0f, 0.0f);
				glm::vec3 scale(1.0f, 1.0f, 1.0f);
				glm::vec4 rotation(0.0f, 0.0f, 0.0f, 0.0f);
				glm::vec3 diffuse(1.0f, 1.0f, 1.0f);
				glm::vec3 specular(0.0f, 0.0f, 0.0f);
				glm::vec3 ambient(0.1f, 0.1f, 0.1f);
				double exponent = 0.0;
				Plane* pla = new Plane(nor, pos, scale, rotation, ambient, diffuse, specular, exponent);
				SceneOBJ* _obj = pla;
				scene.objs.push_back(_obj);
			}
			// ellipsoid
			{
				glm::vec3 pos(0.5f, 0.0f, 0.5f);
				glm::vec3 scale(0.5f, 0.6f, 0.2f);
				glm::vec4 rotation(0.0f, 0.0f, 0.0f, 0.0f);
				glm::vec3 diffuse(1.0f, 0.0f, 0.0f);
				glm::vec3 specular(1.0f, 1.0f, 0.5f);
				glm::vec3 ambient(0.1f, 0.1f, 0.1f);
				double exponent = 100.0;
				Ellipsoid* ell = new Ellipsoid(pos, 1.0f, scale, rotation, ambient, diffuse, specular, exponent);
				SceneOBJ* _obj = ell;
				scene.objs.push_back(_obj);
			}
			// Light 1
			{
				glm::vec3 pos(1.0f, 2.0f, 2.0f);
				float col = 0.5f;
				Light* lh = new Light(col, pos);
				scene.lights.push_back(lh);
			}
			// Light 2
			{
				glm::vec3 pos(-1.0f, 2.0f, -1.0f);
				float col = 0.5f;
				Light* lh = new Light(col, pos);
				scene.lights.push_back(lh);
			}
		break;
		case 4:
		case 5:
			// Red Sphere
			{
				glm::vec3 pos(0.5f, -0.7f, 0.5f);
				glm::vec3 scale(0.3f, 0.3f, 0.3f);
				glm::vec4 rotation(0.0f, 0.0f, 0.0f, 0.0f);
				glm::vec3 diffuse(1.0f, 0.0f, 0.0f);
				glm::vec3 specular(1.0f, 1.0f, 0.5f);
				glm::vec3 ambient(0.1f, 0.1f, 0.1f);
				double exponent = 100.0;
				// Radius hard coded to speed up computation
				Sphere* sph = new Sphere(pos, 0.3f, scale, rotation, ambient, diffuse, specular, exponent);
				SceneOBJ* _obj = sph;
				scene.objs.push_back(_obj);
			}
			// Blue Sphere
			{
				glm::vec3 pos(1.0f, -0.7f, 0.0f);
				glm::vec3 scale(0.3f, 0.3f, 0.3f);
				glm::vec4 rotation(0.0f, 0.0f, 0.0f, 0.0f);
				glm::vec3 diffuse(0.0f, 0.0f, 1.0f);
				glm::vec3 specular(1.0f, 1.0f, 0.5f);
				glm::vec3 ambient(0.1f, 0.1f, 0.1f);
				double exponent = 100.0;
				// Radius hard coded to speed up computation
				Sphere* sph = new Sphere(pos, 0.3f, scale, rotation, ambient, diffuse, specular, exponent);
				SceneOBJ* _obj = sph;
				scene.objs.push_back(_obj);
			}
			// Reflective Sphere 1
			{
				glm::vec3 pos(-0.5f, 0.0f, -0.5f);
				glm::vec3 scale(1.0f, 1.0f, 1.0f);
				glm::vec4 rotation(0.0f, 0.0f, 0.0f, 0.0f);
				glm::vec3 diffuse(0.0f, 0.0f, 0.0f);
				glm::vec3 specular(0.0f, 0.0f, 0.0f);
				glm::vec3 ambient(0.0f, 0.0f, 0.0f);
				double exponent = 0.0;
				// Radius hard coded to speed up computation
				RFSphere* sph = new RFSphere(pos, 1.0f, scale, rotation, ambient, diffuse, specular, exponent);
				SceneOBJ* _obj = sph;
				scene.objs.push_back(_obj);
			}
			// Reflective Sphere 2
			{
				glm::vec3 pos(1.5f, 0.0f, -1.5f);
				glm::vec3 scale(1.0f, 1.0f, 1.0f);
				glm::vec4 rotation(0.0f, 0.0f, 0.0f, 0.0f);
				glm::vec3 diffuse(0.0f, 0.0f, 0.0f);
				glm::vec3 specular(0.0f, 0.0f, 0.0f);
				glm::vec3 ambient(0.0f, 0.0f, 0.0f);
				double exponent = 0.0;
				// Radius hard coded to speed up computation
				RFSphere* sph = new RFSphere(pos, 1.0f, scale, rotation, ambient, diffuse, specular, exponent);
				SceneOBJ* _obj = sph;
				scene.objs.push_back(_obj);
			}
			// Floor
			{
				glm::vec3 nor(0.0f, 1.0f, 0.0f);
				glm::vec3 pos(0.0f, -1.0f, 0.0f);
				glm::vec3 scale(1.0f, 1.0f, 1.0f);
				glm::vec4 rotation(0.0f, 0.0f, 0.0f, 0.0f);
				glm::vec3 diffuse(1.0f, 1.0f, 1.0f);
				glm::vec3 specular(0.0f, 0.0f, 0.0f);
				glm::vec3 ambient(0.1f, 0.1f, 0.1f);
				double exponent = 0.0;
				Plane* pla = new Plane(nor, pos, scale, rotation, ambient, diffuse, specular, exponent);
				SceneOBJ* _obj = pla;
				scene.objs.push_back(_obj);
			}
			// Back wall
			{
				glm::vec3 nor(0.0f, 0.0f, 1.0f);
				glm::vec3 pos(0.0f, 0.0f, -3.0f);
				glm::vec3 scale(1.0f, 1.0f, 1.0f);
				glm::vec4 rotation(0.0f, 0.0f, 0.0f, 0.0f);
				glm::vec3 diffuse(1.0f, 1.0f, 1.0f);
				glm::vec3 specular(0.0f, 0.0f, 0.0f);
				glm::vec3 ambient(0.1f, 0.1f, 0.1f);
				double exponent = 0.0;
				Plane* pla = new Plane(nor, pos, scale, rotation, ambient, diffuse, specular, exponent);
				SceneOBJ* _obj = pla;
				scene.objs.push_back(_obj);
			}
			// Light 1
			{
				glm::vec3 pos(-1.0f, 2.0f, 1.0f);
				float col = 0.5f;
				Light* lh = new Light(col, pos);
				scene.lights.push_back(lh);
			}
			// Light 2
			{
				glm::vec3 pos(0.5f, -0.5f, 0.0f);
				float col = 0.5f;
				Light* lh = new Light(col, pos);
				scene.lights.push_back(lh);
			}
		break;
		case 6:
			// Bunny
			{
				glm::vec3 center;
				float radius;
				glm::vec3 translate(0.0f, 0.0f, 0.0f);
				glm::vec3 scale(1.0f, 1.0f, 1.0f);
				glm::vec4 rotate(0.0f, 0.0f, 1.0f, 0.0f);
				glm::vec3 ambient(0.1f, 0.1f, 0.1f);
				glm::vec3 diffuse(0.0f, 0.0f, 1.0f);
				glm::vec3 specular(1.0f, 1.0f, 0.5f);
				double exponent = 100.0;
				vector<float> poss;
				vector<float> norr;
				vector<float> texx;
				string filename = "../resources/bunny.obj";
				populateOBJ(poss, norr, texx, filename, center, radius);
				Mesh* bunny = new Mesh(center, radius, scale, rotate, ambient, diffuse, specular, exponent, poss, norr, texx, translate);
				SceneOBJ* _obj = bunny;
				scene.objs.push_back(_obj);
			}
			// Light
			{
				glm::vec3 pos(-1.0f, 1.0f, 1.0f);
				float col = 1.0f;
				Light* lh = new Light(col, pos);
				scene.lights.push_back(lh);
			}
		break;
		case 7:
			// Bunny
			{
				glm::vec3 center;
				float radius;
				glm::vec3 translate(0.3f, -1.5f, 0.0f);
				glm::vec3 scale(1.5f, 1.5f, 1.5f);
				glm::vec4 rotate(((float)(20*M_PI/180)), 1.0f, 0.0f, 0.0f);
				glm::vec3 ambient(0.1f, 0.1f, 0.1f);
				glm::vec3 diffuse(0.0f, 0.0f, 1.0f);
				glm::vec3 specular(1.0f, 1.0f, 0.5f);
				double exponent = 100.0;
				vector<float> poss;
				vector<float> norr;
				vector<float> texx;
				string filename = "../resources/bunny.obj";
				populateOBJ(poss, norr, texx, filename, center, radius);
				Mesh* bunny = new Mesh(center, radius*max(scale.x, max(scale.y, scale.z)), scale, rotate, ambient, diffuse, specular, exponent, poss, norr, texx, translate);
				SceneOBJ* _obj = bunny;
				scene.objs.push_back(_obj);
			}
			// Light
			{
				glm::vec3 pos(1.0f, 1.0f, 2.0f);
				float col = 1.0f;
				Light* lh = new Light(col, pos);
				scene.lights.push_back(lh);
			}
		break;
		case 0:
			// Red Sphere
			{
				glm::vec3 pos(0.5f, -0.7f, 0.5f);
				glm::vec3 scale(0.3f, 0.3f, 0.3f);
				glm::vec4 rotation(0.0f, 0.0f, 0.0f, 0.0f);
				glm::vec3 diffuse(1.0f, 0.0f, 0.0f);
				glm::vec3 specular(1.0f, 1.0f, 0.5f);
				glm::vec3 ambient(0.1f, 0.1f, 0.1f);
				double exponent = 100.0;
				// Radius hard coded to speed up computation
				Sphere* sph = new Sphere(pos, 0.3f, scale, rotation, ambient, diffuse, specular, exponent);
				SceneOBJ* _obj = sph;
				scene.objs.push_back(_obj);
			}
			// Blue Sphere
			{
				glm::vec3 pos(1.0f, -0.7f, 0.0f);
				glm::vec3 scale(0.3f, 0.3f, 0.3f);
				glm::vec4 rotation(0.0f, 0.0f, 0.0f, 0.0f);
				glm::vec3 diffuse(0.0f, 0.0f, 1.0f);
				glm::vec3 specular(1.0f, 1.0f, 0.5f);
				glm::vec3 ambient(0.1f, 0.1f, 0.1f);
				double exponent = 100.0;
				// Radius hard coded to speed up computation
				Sphere* sph = new Sphere(pos, 0.3f, scale, rotation, ambient, diffuse, specular, exponent);
				SceneOBJ* _obj = sph;
				scene.objs.push_back(_obj);
			}
			// Reflective Sphere 1
			{
				glm::vec3 pos(-0.5f, 0.0f, -0.5f);
				glm::vec3 scale(1.0f, 1.0f, 1.0f);
				glm::vec4 rotation(0.0f, 0.0f, 0.0f, 0.0f);
				glm::vec3 diffuse(0.7f, 0.0f, 0.7f);
				glm::vec3 specular(1.0f, 1.0f, 0.5f);
				glm::vec3 ambient(0.1f, 0.1f, 0.1f);
				double exponent = 100.0;
				// Radius hard coded to speed up computation
				RFSphere* sph = new RFSphere(pos, 1.0f, scale, rotation, ambient, diffuse, specular, exponent);
				SceneOBJ* _obj = sph;
				scene.objs.push_back(_obj);
			}
			// Reflective Sphere 2
			{
				glm::vec3 pos(1.5f, 0.0f, -1.5f);
				glm::vec3 scale(1.0f, 1.0f, 1.0f);
				glm::vec4 rotation(0.0f, 0.0f, 0.0f, 0.0f);
				glm::vec3 diffuse(0.0f, 0.7f, 0.0f);
				glm::vec3 specular(1.0f, 1.0f, 0.5f);
				glm::vec3 ambient(0.1f, 0.1f, 0.1f);
				double exponent = 100.0;
				// Radius hard coded to speed up computation
				RFSphere* sph = new RFSphere(pos, 1.0f, scale, rotation, ambient, diffuse, specular, exponent);
				SceneOBJ* _obj = sph;
				scene.objs.push_back(_obj);
			}
			// Floor
			{
				glm::vec3 nor(0.0f, 1.0f, 0.0f);
				glm::vec3 pos(0.0f, -1.0f, 0.0f);
				glm::vec3 scale(1.0f, 1.0f, 1.0f);
				glm::vec4 rotation(0.0f, 0.0f, 0.0f, 0.0f);
				glm::vec3 diffuse(1.0f, 1.0f, 1.0f);
				glm::vec3 specular(0.0f, 0.0f, 0.0f);
				glm::vec3 ambient(0.1f, 0.1f, 0.1f);
				double exponent = 0.0;
				Plane* pla = new Plane(nor, pos, scale, rotation, ambient, diffuse, specular, exponent);
				SceneOBJ* _obj = pla;
				scene.objs.push_back(_obj);
			}
			// Back wall
			{
				glm::vec3 nor(0.0f, 0.0f, 1.0f);
				glm::vec3 pos(0.0f, 0.0f, -3.0f);
				glm::vec3 scale(1.0f, 1.0f, 1.0f);
				glm::vec4 rotation(0.0f, 0.0f, 0.0f, 0.0f);
				glm::vec3 diffuse(1.0f, 1.0f, 1.0f);
				glm::vec3 specular(0.0f, 0.0f, 0.0f);
				glm::vec3 ambient(0.1f, 0.1f, 0.1f);
				double exponent = 0.0;
				Plane* pla = new Plane(nor, pos, scale, rotation, ambient, diffuse, specular, exponent);
				SceneOBJ* _obj = pla;
				scene.objs.push_back(_obj);
			}
			// Light 1
			{
				glm::vec3 pos(-1.0f, 2.0f, 1.0f);
				float col = 0.5f;
				Light* lh = new Light(col, pos);
				scene.lights.push_back(lh);
			}
			// Light 2
			{
				glm::vec3 pos(0.5f, -0.5f, 0.0f);
				float col = 0.5f;
				Light* lh = new Light(col, pos);
				scene.lights.push_back(lh);
			}
		break;
	}
}


int main(int argc, char **argv)
{
	// Variables for setup
	int scene_no = -1;
	int scene_size = -1;
	string output_file = "";

	double fovy = 45.0;
	glm::vec3 camera_pos(0.0f, 0.0f, 5.0f);
	glm::vec4 camera_rot(0.0f, 0.0f, 0.0f, 0.0f);

	
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
		cout << "Expected 0-8" << endl;
		cout << "Got: " << scene_no << endl;
		return 0;
	}

	// checks if scene 8
	if(scene_no == 8) {
		fovy = 60.0;
		camera_pos.x = -3.0f;
		camera_pos.y = 0.0f;
		camera_pos.z = 0.0f;
		camera_rot = glm::vec4(M_PI/2.0, 0.0f, 1.0f, 0.0f);

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

	// initialize camera and populate rays
	// TODO: generate camera dependant on scene. If scene 8, camera's fovy will be different.
	// Although aspect will never change in the scope of this assignment, I added a field for it for later editing.
	Camera cam(scene_size, scene_size, fovy, camera_pos, camera_rot);

	// initialize scene objects and populate objs
	Scene scene;
	generateScene(scene, scene_no);

	// draw scene with camera
	if(scene_no != 0) {
		cam.drawScene(scene, output_file);
	} else {
		cam.drawAAScene(scene, output_file);
	}

	return 0;
}

