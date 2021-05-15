#include "primitive_builder.h"
#include <graphics/mesh.h>
#include <system/platform.h>
#include <graphics/primitive.h>
#include <maths/math_utils.h>
#include <vector>
#include <math.h>

#define PI 3.14159265358979323846f

//
// PrimitiveBuilder
//
PrimitiveBuilder::PrimitiveBuilder(gef::Platform& platform) :
	platform_(platform),
	default_cube_mesh_(NULL),
	default_sphere_mesh_(NULL)
{
	Init();
}

//
// ~PrimitiveBuilder
//
PrimitiveBuilder::~PrimitiveBuilder()
{
	CleanUp();
}

//
// Init
//
void PrimitiveBuilder::Init()
{
	// create helper geometry
	default_cube_mesh_ = CreateBoxMesh(gef::Vector4(0.5f, 0.5f, 0.5f));
	default_sphere_mesh_ = CreateSphereMesh(0.5f, 20, 20);

	// create materials for basic colours
	red_material_.set_colour(0xff0000ff);
	blue_material_.set_colour(0xffff0000);
	green_material_.set_colour(0xff00ff00);
}

//
// CleanUp
//
void PrimitiveBuilder::CleanUp()
{
	delete default_sphere_mesh_;
	default_sphere_mesh_ = NULL;

	delete default_cube_mesh_;
	default_cube_mesh_ = NULL;
}

//
// Create3DTriangleMesh
//
// radius is from the center of mass to a point
gef::Mesh* PrimitiveBuilder::Create3DTriangleMesh(float radius, float depth, gef::Vector4 centre, gef::Material** materials)
{
	gef::Mesh* mesh = gef::Mesh::Create(platform_);

	float radius_to_side = cosf((60 * PI) / 180) * radius;
	  
	float top_front[3]		    = { centre.x() + cosf((90 * PI) / 180) * radius, centre.x() + sinf((90 * PI) / 180) * radius, centre.z() + depth };
	float bottom_left_front[3]  = { centre.x() + cosf((210 * PI) / 180) * radius, centre.x() + sinf((210 * PI) / 180) * radius, centre.z() + depth };
	float bottom_mid_front[3]   = { centre.x() + cosf((270 * PI) / 180) * radius_to_side, centre.x() + sinf((270 * PI) / 180) * radius_to_side, centre.z() + depth };
	float bottom_right_front[3] = { centre.x() + cosf((330 * PI) / 180) * radius, centre.x() + sinf((330 * PI) / 180) * radius, centre.z() + depth };
	float top_back[3]           = { centre.x() + cosf((90 * PI) / 180) * radius, centre.x() + sinf((90 * PI) / 180) * radius, centre.z() - depth };
	float bottom_left_back[3]   = { centre.x() + cosf((210 * PI) / 180) * radius, centre.x() + sinf((210 * PI) / 180) * radius, centre.z() - depth };
	float bottom_mid_back[3]    = { centre.x() + cosf((270 * PI) / 180) * radius_to_side, centre.x() + sinf((270 * PI) / 180) * radius_to_side, centre.z() - depth };
	float bottom_right_back[3]  = { centre.x() + cosf((330 * PI) / 180) * radius, centre.x() + sinf((330 * PI) / 180) * radius, centre.z() - depth };

	// create vertices, 4 for each square face (front and back triangles split into 2) so we have all vertices in a single vertex share the same normal
	const int kNumVertices = 6 * 4;
	gef::Mesh::Vertex vertices[kNumVertices] =
	{
		// front
		{ top_front[0]			, top_front[1]			, top_front[2]			, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },	// #0
		{ bottom_left_front[0]	, bottom_left_front[1]	, bottom_left_front[2]	, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },	// #1
		{ bottom_mid_front[0]	, bottom_mid_front[1]	, bottom_mid_front[2]	, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },	// #2
		{ bottom_right_front[0] , bottom_right_front[1] , bottom_right_front[2]	, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },	// #3
																																		
		//back																															
		{ top_back[0]			, top_back[1]			, top_back[2]			, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f },	// #4
		{ bottom_left_back[0]	, bottom_left_back[1]	, bottom_left_back[2]	, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f },	// #5
		{ bottom_mid_back[0]	, bottom_mid_back[1]	, bottom_mid_back[2]	, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f },	// #6
		{ bottom_right_back[0] , bottom_right_back[1]	, bottom_right_back[2]	, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f },	// #7
																																   			
		// left	
		{ top_back[0]			, top_back[1]			, top_back[2]			, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f },	// #8
		{ top_front[0]			, top_front[1]			, top_front[2]			, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f },	// #9
		{ bottom_left_back[0]	, bottom_left_back[1]	, bottom_left_back[2]	, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f },	// #10
		{ bottom_left_front[0]	, bottom_left_front[1]	, bottom_left_front[2]	, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f },	// #11
																															   		
		// right																													   		
		{ top_front[0]			, top_front[1]			, top_front[2]			, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f },	// #12
		{ top_back[0]			, top_back[1]			, top_back[2]			, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f },	// #13
		{ bottom_right_front[0]	, bottom_right_front[1]	, bottom_right_front[2]	, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f },	// #14
		{ bottom_right_back[0]	, bottom_right_back[1]	, bottom_right_back[2]	, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f },	// #15
																																	   		
		// bottom																													   		
		{ bottom_left_front[0]  , bottom_left_front[1]	, bottom_left_front[2]	, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f },	// #16
		{ bottom_right_front[0]	, bottom_right_front[1]	, bottom_right_front[2]	, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f },	// #17
		{ bottom_left_back[0]	, bottom_left_back[1]	, bottom_left_back[2]	, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f },	// #18
		{ bottom_right_back[0]	, bottom_right_back[1]	, bottom_right_back[2]	, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f },	// #19
	};

	const int kNumIndices = 6 * 6;
	Int32 indices[kNumIndices] =
	{
		// front
		0, 1, 2,
		0, 2, 3,

		// back
		4, 5, 6,
		4, 6, 7,

		// left
		8, 9, 10,
		9, 10, 11,

		// right
		12, 13, 14,
		13, 14, 15,

		// bottom
		16, 17, 18,
		17, 18, 19
	};

	// create the vertex buffer for the box vertices
	mesh->InitVertexBuffer(platform_, vertices, kNumVertices, sizeof(gef::Mesh::Vertex));

	// create a primitive per face so we can alter the material per face
	const int num_faces = 5;
	mesh->AllocatePrimitives(num_faces);

	for (int primitive_num = 0; primitive_num < num_faces; ++primitive_num)
	{
		gef::Primitive* primitive = mesh->GetPrimitive(primitive_num);
		primitive->InitIndexBuffer(platform_, &indices[primitive_num * 6], 6, sizeof(Int32));
		primitive->set_type(gef::TRIANGLE_LIST);

		// if materials pointer is valid then assume we have an array of Material pointers
		// with a size greater than 6 (one material per face)
		if (materials)
			primitive->set_material(materials[primitive_num]);
	}

	// set the bounds

	// axis aligned bounding box
	gef::Aabb aabb(centre - gef::Vector4(radius, radius, radius), centre + gef::Vector4(radius, radius, radius));
	mesh->set_aabb(aabb);

	// bounding sphere
	gef::Sphere sphere(aabb);
	mesh->set_bounding_sphere(sphere);

	return mesh;
}

//
// CreateBoxMesh
//
gef::Mesh* PrimitiveBuilder::CreateBoxMesh(const gef::Vector4& half_size, gef::Vector4 centre, gef::Material** materials)
{
	gef::Mesh* mesh = gef::Mesh::Create(platform_);

	//
	// vertices
	//
	// create vertices, 4 for each face so we have all vertices in a single vertex share the same normal
	const int kNumVertices = 4 * 6;
	gef::Mesh::Vertex vertices[kNumVertices] =
	{
		// front
		{ centre.x() - half_size.x(),	centre.y() + half_size.y(),	centre.z() + half_size.z(), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
		{ centre.x() + half_size.x(),	centre.y() + half_size.y(),	centre.z() + half_size.z(), 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
		{ centre.x() - half_size.x(),	centre.y() - half_size.y(), centre.z() + half_size.z(), 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },
		{ centre.x() + half_size.x(),	centre.y() - half_size.y(), centre.z() + half_size.z(), 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },


		// back
		{ centre.x() + half_size.x(),	centre.y() + half_size.y(),	centre.z() - half_size.z(), 0.0f, 0.0f, -1.0f, 0.0f, 0.0f },
		{ centre.x() - half_size.x(),	centre.y() + half_size.y(),	centre.z() - half_size.z(), 0.0f, 0.0f, -1.0f, 1.0f, 0.0f },
		{ centre.x() + half_size.x(),	centre.y() - half_size.y(), centre.z() - half_size.z(), 0.0f, 0.0f, -1.0f, 0.0f, 1.0f },
		{ centre.x() - half_size.x(),	centre.y() - half_size.y(), centre.z() - half_size.z(), 0.0f, 0.0f, -1.0f, 1.0f, 1.0f },

		// left
		{ centre.x() - half_size.x(),	centre.y() + half_size.y(),	centre.z() - half_size.z(), -1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		{ centre.x() - half_size.x(),	centre.y() + half_size.y(),	centre.z() + half_size.z(), -1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
		{ centre.x() - half_size.x(),	centre.y() - half_size.y(), centre.z() - half_size.z(), -1.0f, 0.0f, 0.0f, 0.0f, 1.0f },
		{ centre.x() - half_size.x(),	centre.y() - half_size.y(), centre.z() + half_size.z(), -1.0f, 0.0f, 0.0f, 1.0f, 1.0f },


		// right
		{ centre.x() + half_size.x(),	centre.y() + half_size.y(),	centre.z() + half_size.z(), 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		{ centre.x() + half_size.x(),	centre.y() + half_size.y(),	centre.z() - half_size.z(), 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
		{ centre.x() + half_size.x(),	centre.y() - half_size.y(), centre.z() + half_size.z(), 1.0f, 0.0f, 0.0f, 0.0f, 1.0f },
		{ centre.x() + half_size.x(),	centre.y() - half_size.y(), centre.z() - half_size.z(), 1.0f, 0.0f, 0.0f, 1.0f, 1.0f },


		// top
		{ centre.x() - half_size.x(),	centre.y() + half_size.y(),	centre.z() - half_size.z(), 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },
		{ centre.x() + half_size.x(),	centre.y() + half_size.y(),	centre.z() - half_size.z(), 0.0f, 1.0f, 0.0f, 1.0f, 0.0f },
		{ centre.x() - half_size.x(),	centre.y() + half_size.y(), centre.z() + half_size.z(), 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ centre.x() + half_size.x(),	centre.y() + half_size.y(), centre.z() + half_size.z(), 0.0f, 1.0f, 0.0f, 1.0f, 1.0f },

		// bottom
		{ centre.x() - half_size.x(),	centre.y() - half_size.y(),	centre.z() + half_size.z(), 0.0f, -1.0f, 0.0f, 0.0f, 0.0f },
		{ centre.x() + half_size.x(),	centre.y() - half_size.y(),	centre.z() + half_size.z(), 0.0f, -1.0f, 0.0f, 1.0f, 0.0f },
		{ centre.x() - half_size.x(),	centre.y() - half_size.y(), centre.z() - half_size.z(), 0.0f, -1.0f, 0.0f, 0.0f, 1.0f },
		{ centre.x() + half_size.x(),	centre.y() - half_size.y(), centre.z() - half_size.z(), 0.0f, -1.0f, 0.0f, 1.0f, 1.0f },
	};

	const int kNumIndices = 6 * 6;
	Int32 indices[kNumIndices] =
	{
		// front
		0, 1, 2,
		1, 3, 2,

		// back
		4, 5, 6,
		5, 7, 6,

		// left
		8, 9, 10,
		9, 11, 10,

		// right
		12, 13, 14,
		13, 15, 14,

		// top
		16, 17, 18,
		17, 19, 18,

		// bottom
		20, 21, 22,
		21, 23, 22
	};

	// create the vertex buffer for the box vertices
	mesh->InitVertexBuffer(platform_, vertices, kNumVertices, sizeof(gef::Mesh::Vertex));

	// create a primitive per face so we can alter the material per face
	const int num_faces = 6;
	mesh->AllocatePrimitives(num_faces);

	for (int primitive_num = 0; primitive_num < num_faces; ++primitive_num)
	{
		gef::Primitive* primitive = mesh->GetPrimitive(primitive_num);
		primitive->InitIndexBuffer(platform_, &indices[primitive_num*6], 6, sizeof(Int32));
		primitive->set_type(gef::TRIANGLE_LIST);

		// if materials pointer is valid then assume we have an array of Material pointers
		// with a size greater than 6 (one material per face)
		if(materials)
			primitive->set_material(materials[primitive_num]);
	}

	// set the bounds

	// axis aligned bounding box
	gef::Aabb aabb(centre - half_size, centre + half_size);
	mesh->set_aabb(aabb);

	// bounding sphere
	gef::Sphere sphere(aabb);
	mesh->set_bounding_sphere(sphere);
	
	return mesh;
}


//
// CalculateSphereSurfaceNormal
//
gef::Vector4 CalculateSphereSurfaceNormal(const float radius, const gef::Vector4& origin, const gef::Vector4& point)
{
	return gef::Vector4(
		(point.x() - origin.x()) / radius,
		(point.y() - origin.y()) / radius,
		(point.z() - origin.z()) / radius);
}




//
// CreateSphereMesh
//
// adapted from
// http://www.visualizationlibrary.org/documentation/_geometry_primitives_8cpp_source.html#l00284
gef::Mesh* PrimitiveBuilder::CreateSphereMesh(const float radius, const int phi, const int theta, gef::Vector4 origin, gef::Material* material)
{
	gef::Mesh* mesh = gef::Mesh::Create(platform_);

	const int kNumVertices = theta*phi + 2;
	std::vector<gef::Mesh::Vertex> vertices;
	vertices.resize(kNumVertices);


	int vert_idx = 0;
	gef::Vector4 position, normal(0.0f, 0.0f, 1.0f);
	gef::Vector2 uv(0.0f, 0.0f);

	position = origin + gef::Vector4(0.0f, radius, 0.0f);
	normal = CalculateSphereSurfaceNormal(radius, origin, position);

	vertices[vert_idx].px = position.x();
	vertices[vert_idx].py = position.y();
	vertices[vert_idx].pz = position.z();
	vertices[vert_idx].nx = normal.x();
	vertices[vert_idx].ny = normal.y();
	vertices[vert_idx].nz = normal.z();
	vertices[vert_idx].u = uv.x;
	vertices[vert_idx].v = uv.y;
	vert_idx++;
	for (int i = 0; i < theta; ++i)
	{
		for (int j = 0; j < phi; ++j)
		{
			gef::Vector4 v(0.0f, radius, 0.0f);

			gef::Matrix44 rotz, roty;

			rotz.RotationZ(gef::DegToRad(180.0f / (theta + 1)*(i + 1)));
			roty.RotationY(gef::DegToRad(360.0f / phi*j));

			v = v.Transform(rotz);
			v = v.Transform(roty);

			position = origin + v;
			normal = CalculateSphereSurfaceNormal(radius, origin, position);

			vertices[vert_idx].px = position.x();
			vertices[vert_idx].py = position.y();
			vertices[vert_idx].pz = position.z();
			vertices[vert_idx].nx = normal.x();
			vertices[vert_idx].ny = normal.y();
			vertices[vert_idx].nz = normal.z();
			vertices[vert_idx].u = uv.x;
			vertices[vert_idx].v = uv.y;
			vert_idx++;
		}
	}

	position = origin + gef::Vector4(0.0f, -radius, 0.0f);
	normal = CalculateSphereSurfaceNormal(radius, origin, position);

	vertices[vert_idx].px = position.x();
	vertices[vert_idx].py = position.y();
	vertices[vert_idx].pz = position.z();
	vertices[vert_idx].nx = normal.x();
	vertices[vert_idx].ny = normal.y();
	vertices[vert_idx].nz = normal.z();
	vertices[vert_idx].u = uv.x;
	vertices[vert_idx].v = uv.y;
	vert_idx++;


	mesh->InitVertexBuffer(platform_, &vertices[0], kNumVertices, sizeof(gef::Mesh::Vertex));
	mesh->AllocatePrimitives(2);

	// side quads
	std::vector<Int32> index_buffer;
	index_buffer.resize((theta - 1)*phi * 6);

	int idx = 0;
	for (int i = 0; i<theta - 1; ++i)
	{
		for (int j = 0; j<phi; ++j)
		{
			// 2 triangles per quad
			index_buffer.at(idx++) = 1 + phi*(i + 0) + (j + 1) % phi;
			index_buffer.at(idx++) = 1 + phi*(i + 1) + (j + 1) % phi;
			index_buffer.at(idx++) = 1 + phi*(i + 1) + (j + 0) % phi;

			index_buffer.at(idx++) = 1 + phi*(i + 0) + (j + 0) % phi;
			index_buffer.at(idx++) = 1 + phi*(i + 0) + (j + 1) % phi;
			index_buffer.at(idx++) = 1 + phi*(i + 1) + (j + 0) % phi;
		}
	}

	gef::Primitive* primitive;

	// setup primitive for side quads
	primitive = mesh->GetPrimitive(0);
	primitive->set_type(gef::TRIANGLE_LIST);
	primitive->set_material(material);
	primitive->InitIndexBuffer(platform_, &index_buffer[0], (UInt32)index_buffer.size(), sizeof(Int32));

	// top/bottom triangles
	index_buffer.resize(phi * 3 + phi * 3);

	idx = 0;
	// top fan
	for (int j = 0; j<phi; ++j)
	{
		index_buffer.at(idx++) = 1 + (j + 1) % phi;
		index_buffer.at(idx++) = 1 + (j + 0) % phi;
		index_buffer.at(idx++) = 0;
	}
	
	// bottom fan
	for (int j = 0; j<phi; ++j)
	{
		index_buffer.at(idx++) = 1 + phi*(theta - 1) + (j + 0) % phi;
		index_buffer.at(idx++) = 1 + phi*(theta - 1) + (j + 1) % phi;
		index_buffer.at(idx++) = (int)kNumVertices - 1;
	}

	// setup primitive for top and bottom fans
	primitive = mesh->GetPrimitive(1);
	primitive->set_type(gef::TRIANGLE_LIST);
	primitive->set_material(material);
	primitive->InitIndexBuffer(platform_, &index_buffer[0], (UInt32)index_buffer.size(), sizeof(Int32));

	// bounds
	gef::Aabb aabb(gef::Vector4(-radius, -radius, -radius) - origin, gef::Vector4(radius, radius, radius)+ origin);
	mesh->set_aabb(aabb);
	gef::Sphere sphere(origin, radius);
	mesh->set_bounding_sphere(sphere);

	return mesh;
}
