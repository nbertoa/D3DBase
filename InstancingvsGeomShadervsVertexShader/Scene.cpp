#include "Scene.h"

#include <Camera/Camera.h>
#include <CubeGenerator\Drawer.h>
#include <CubeGenerator\VertexType.h>
#include <DrawManager/DrawManager.h>
#include <DXUtils/DXUtils.h>
#include <InstancingTriangleDrawer\Drawer.h>
#include <InstancingTriangleDrawer\VertexType.h>
#include <ShaderResourcesManager/GlobalResources.h>    
#include <ShaderResourcesManager/ShaderResourcesManager.h> 
#include <TriangleDrawer/Drawer.h>
#include <TriangleDrawer\VertexType.h>
#include <Utils/Assert.h>  
#include <Utils/Hash.h>
#include <Utils/MathUtils.h>

using namespace DirectX;

namespace {
	const size_t NUM_ENTITIES = 10000000;
	const float QUAD_HALF_SIZE = 0.125f;
	const float POSITION_OFFSET = 20.0f;
	const size_t CUBE_VERTICES = 8;
	const size_t CUBE_INDICES = 36;

	CubeGenerator::Drawer* generateCubeGeneratorDrawer() {
		CubeGenerator::Drawer* drawer = new CubeGenerator::Drawer;
		XMStoreFloat4x4(&drawer->World(), XMMatrixIdentity());

		// Generate positions
		std::vector<XMFLOAT3> positions;
		positions.reserve(NUM_ENTITIES);
		for (size_t i = 0; i < NUM_ENTITIES; ++i) {
			float x = Utils::RandomFloat(-POSITION_OFFSET, POSITION_OFFSET);
			float y = Utils::RandomFloat(-POSITION_OFFSET, POSITION_OFFSET);
			float z = Utils::RandomFloat(-POSITION_OFFSET, POSITION_OFFSET);
			positions.push_back(XMFLOAT3(x, y, z));
		}

		// Create vertex buffer
		const size_t id = Utils::Hash("cube_generator_vertex_buffer");
		Utils::CreateInitializedBuffer("cube_generator_vertex_buffer", &positions[0], static_cast<unsigned int> (NUM_ENTITIES * sizeof(CubeGenerator::VertexData)), D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0);
		ID3D11Buffer* buffer = ShaderResourcesManager::gInstance->Buffer(id);
		ASSERT(buffer);

		// Fill VertexShaderData
		drawer->GetVertexShaderData().VertexBuffer() = buffer;
		drawer->GetVertexShaderData().SetVertexCount(NUM_ENTITIES);

		// Fill GeometryShaderData
		drawer->GetGeometryShaderData().SetQuadHalfSize(QUAD_HALF_SIZE);

		return drawer;
	}

	InstancingTriangleDrawer::Drawer* generateInstancingTriangleDrawer() {
		InstancingTriangleDrawer::Drawer* drawer = new InstancingTriangleDrawer::Drawer;
		XMStoreFloat4x4(&drawer->World(), XMMatrixIdentity());

		// Positions for the cube
		std::vector<XMFLOAT3> positions;
		positions.reserve(CUBE_VERTICES);
		positions.push_back(XMFLOAT3(-QUAD_HALF_SIZE, QUAD_HALF_SIZE, -QUAD_HALF_SIZE));
		positions.push_back(XMFLOAT3(QUAD_HALF_SIZE, QUAD_HALF_SIZE, -QUAD_HALF_SIZE));
		positions.push_back(XMFLOAT3(-QUAD_HALF_SIZE, -QUAD_HALF_SIZE, -QUAD_HALF_SIZE));
		positions.push_back(XMFLOAT3(QUAD_HALF_SIZE, -QUAD_HALF_SIZE, -QUAD_HALF_SIZE));

		positions.push_back(XMFLOAT3(-QUAD_HALF_SIZE, QUAD_HALF_SIZE, QUAD_HALF_SIZE));
		positions.push_back(XMFLOAT3(QUAD_HALF_SIZE, QUAD_HALF_SIZE, QUAD_HALF_SIZE));
		positions.push_back(XMFLOAT3(-QUAD_HALF_SIZE, -QUAD_HALF_SIZE, QUAD_HALF_SIZE));
		positions.push_back(XMFLOAT3(QUAD_HALF_SIZE, -QUAD_HALF_SIZE, QUAD_HALF_SIZE));
		
		// Indices of the triangles of the cube
		std::vector<unsigned int> indices;
		indices.reserve(CUBE_INDICES);
		// Front face
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(2);
		indices.push_back(1);
		indices.push_back(3);

		// Back face
		indices.push_back(5);
		indices.push_back(4);
		indices.push_back(7);
		indices.push_back(7);
		indices.push_back(4);
		indices.push_back(6);

		// Left face
		indices.push_back(4);
		indices.push_back(0);
		indices.push_back(6);
		indices.push_back(6);
		indices.push_back(0);
		indices.push_back(2);

		// Right face
		indices.push_back(1);
		indices.push_back(5);
		indices.push_back(3);
		indices.push_back(3);
		indices.push_back(5);
		indices.push_back(7);

		// Top face
		indices.push_back(4);
		indices.push_back(5);
		indices.push_back(0);
		indices.push_back(0);
		indices.push_back(5);
		indices.push_back(1);

		// Bottom face
		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(6);
		indices.push_back(6);
		indices.push_back(3);
		indices.push_back(7);

		// Generate instancing directions
		std::vector<XMFLOAT3> instanceDirections;
		instanceDirections.reserve(NUM_ENTITIES);
		for (size_t i = 0; i < NUM_ENTITIES; ++i) {
			float x = Utils::RandomFloat(-POSITION_OFFSET, POSITION_OFFSET);
			float y = Utils::RandomFloat(-POSITION_OFFSET, POSITION_OFFSET);
			float z = Utils::RandomFloat(-POSITION_OFFSET, POSITION_OFFSET);
			instanceDirections.push_back(XMFLOAT3(x , y , z));			
		}

		// Create vertex buffer
		Utils::CreateInitializedBuffer("instancing_triangle_drawer_vertex_buffer", &positions[0], static_cast<unsigned int> (positions.size() * sizeof(TriangleDrawer::VertexData)), D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0);
		ID3D11Buffer* vBuffer = ShaderResourcesManager::gInstance->Buffer(Utils::Hash("instancing_triangle_drawer_vertex_buffer"));
		ASSERT(vBuffer);

		// Create index buffer
		Utils::CreateInitializedBuffer("instancing_triangle_drawer_index_buffer", &indices[0], static_cast<unsigned int> (indices.size() * sizeof(unsigned int)), D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0);
		ID3D11Buffer* iBuffer = ShaderResourcesManager::gInstance->Buffer(Utils::Hash("instancing_triangle_drawer_index_buffer"));
		ASSERT(iBuffer);

		// Create instancing buffer
		Utils::CreateInitializedBuffer("instancing_triangle_drawer_instancing_buffer", &instanceDirections[0], static_cast<unsigned int> (instanceDirections.size() * sizeof(XMFLOAT3)), D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0);
		ID3D11Buffer* instancingBuffer = ShaderResourcesManager::gInstance->Buffer(Utils::Hash("instancing_triangle_drawer_instancing_buffer"));
		ASSERT(instancingBuffer);

		// Fill VertexShaderData
		drawer->GetVertexShaderData().VertexBuffer() = vBuffer;
		drawer->GetVertexShaderData().IndexBuffer() = iBuffer;
		drawer->GetVertexShaderData().InstanceBuffer() = instancingBuffer;
		drawer->GetVertexShaderData().SetIndicesCount(static_cast<unsigned int> (indices.size()));
		drawer->GetVertexShaderData().SetInstancesCount(static_cast<unsigned int> (instanceDirections.size()));

		return drawer;
	}


	TriangleDrawer::Drawer* generateTriangleDrawer() {
		TriangleDrawer::Drawer* drawer = new TriangleDrawer::Drawer;
		XMStoreFloat4x4(&drawer->World(), XMMatrixIdentity());

		// Generate positions
		std::vector<XMFLOAT3> positions;
		positions.reserve(NUM_ENTITIES * CUBE_VERTICES);
		std::vector<unsigned int> indices;
		indices.reserve(NUM_ENTITIES * CUBE_INDICES);
		unsigned int baseIndex = 0;
		for (size_t i = 0; i < NUM_ENTITIES; ++i) {
			float x = Utils::RandomFloat(-POSITION_OFFSET, POSITION_OFFSET);
			float y = Utils::RandomFloat(-POSITION_OFFSET, POSITION_OFFSET);
			float z = Utils::RandomFloat(-POSITION_OFFSET, POSITION_OFFSET);

			positions.push_back(XMFLOAT3(x - QUAD_HALF_SIZE, y + QUAD_HALF_SIZE, z - QUAD_HALF_SIZE));
			positions.push_back(XMFLOAT3(x + QUAD_HALF_SIZE, y + QUAD_HALF_SIZE, z - QUAD_HALF_SIZE));
			positions.push_back(XMFLOAT3(x - QUAD_HALF_SIZE, y - QUAD_HALF_SIZE, z - QUAD_HALF_SIZE));
			positions.push_back(XMFLOAT3(x + QUAD_HALF_SIZE, y - QUAD_HALF_SIZE, z - QUAD_HALF_SIZE));

			positions.push_back(XMFLOAT3(x - QUAD_HALF_SIZE, y + QUAD_HALF_SIZE, z + QUAD_HALF_SIZE));
			positions.push_back(XMFLOAT3(x + QUAD_HALF_SIZE, y + QUAD_HALF_SIZE, z + QUAD_HALF_SIZE));
			positions.push_back(XMFLOAT3(x - QUAD_HALF_SIZE, y - QUAD_HALF_SIZE, z + QUAD_HALF_SIZE));
			positions.push_back(XMFLOAT3(x + QUAD_HALF_SIZE, y - QUAD_HALF_SIZE, z + QUAD_HALF_SIZE));

			// Front face
			indices.push_back(baseIndex + 0);
			indices.push_back(baseIndex + 1);
			indices.push_back(baseIndex + 2);
			indices.push_back(baseIndex + 2);
			indices.push_back(baseIndex + 1);
			indices.push_back(baseIndex + 3);

			// Back face
			indices.push_back(baseIndex + 5);
			indices.push_back(baseIndex + 4);
			indices.push_back(baseIndex + 7);
			indices.push_back(baseIndex + 7);
			indices.push_back(baseIndex + 4);
			indices.push_back(baseIndex + 6);

			// Left face
			indices.push_back(baseIndex + 4);
			indices.push_back(baseIndex + 0);
			indices.push_back(baseIndex + 6);
			indices.push_back(baseIndex + 6);
			indices.push_back(baseIndex + 0);
			indices.push_back(baseIndex + 2);

			// Right face
			indices.push_back(baseIndex + 1);
			indices.push_back(baseIndex + 5);
			indices.push_back(baseIndex + 3);
			indices.push_back(baseIndex + 3);
			indices.push_back(baseIndex + 5);
			indices.push_back(baseIndex + 7);

			// Top face
			indices.push_back(baseIndex + 4);
			indices.push_back(baseIndex + 5);
			indices.push_back(baseIndex + 0);
			indices.push_back(baseIndex + 0);
			indices.push_back(baseIndex + 5);
			indices.push_back(baseIndex + 1);

			// Bottom face
			indices.push_back(baseIndex + 2);
			indices.push_back(baseIndex + 3);
			indices.push_back(baseIndex + 6);
			indices.push_back(baseIndex + 6);
			indices.push_back(baseIndex + 3);
			indices.push_back(baseIndex + 7);

			baseIndex += CUBE_VERTICES;
		}

		// Create vertex buffer
		Utils::CreateInitializedBuffer("triangle_drawer_vertex_buffer", &positions[0], static_cast<unsigned int> (positions.size() * sizeof(TriangleDrawer::VertexData)), D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0);
		ID3D11Buffer* vBuffer = ShaderResourcesManager::gInstance->Buffer(Utils::Hash("triangle_drawer_vertex_buffer"));
		ASSERT(vBuffer);

		// Create index buffer
		Utils::CreateInitializedBuffer("triangle_drawer_index_buffer", &indices[0], static_cast<unsigned int> (indices.size() * sizeof(unsigned int)), D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0);
		ID3D11Buffer* iBuffer = ShaderResourcesManager::gInstance->Buffer(Utils::Hash("triangle_drawer_index_buffer"));
		ASSERT(iBuffer);

		// Fill VertexShaderData
		drawer->GetVertexShaderData().VertexBuffer() = vBuffer;
		drawer->GetVertexShaderData().IndexBuffer() = iBuffer;
		drawer->GetVertexShaderData().SetIndicesCount(static_cast<unsigned int> (indices.size()));

		return drawer;
	}
}

namespace InstancingVsGeometryShaderVsVertexShader {
	Scene::Scene() {
		//DrawManager::gInstance->AddDrawer(*generateCubeGeneratorDrawer());
		//DrawManager::gInstance->AddDrawer(*generateTriangleDrawer());
		DrawManager::gInstance->AddDrawer(*generateInstancingTriangleDrawer());
	}

	void Scene::Update(ID3D11Device1& device, const float elapsedTime) {
	}
}