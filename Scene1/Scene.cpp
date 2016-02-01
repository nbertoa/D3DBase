#include "Scene.h"

#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>

#include <Camera/Camera.h>
#include <DrawManager/DrawManager.h>
#include <DXUtils/DXUtils.h>
#include <EntityDrawer\Drawer.h>
#include <EntityDrawer\VertexType.h>
#include <Input/Keyboard.h> 
#include <ShaderResourcesManager/GlobalResources.h>    
#include <ShaderResourcesManager/ShaderResourcesManager.h>  
#include <Utils/Assert.h>  
#include <Utils/Hash.h>
#include <Utils/MathUtils.h>

using namespace DirectX;

namespace {
	const size_t NUM_ENTITIES = 10000000;
	const float QUAD_HALF_SIZE = 0.125f;
	const float POSITION_OFFSET = 50.0f;
	const float VELOCITY_OFFSET = 1.0f;
	const char* TEXTURE_PATH = "..\\..\\content\\textures\\flare.dds";

	struct Positions {
		std::vector<XMFLOAT3>* mPositions;
		std::vector<XMFLOAT3>* mVelocity;

		void operator()(const tbb::blocked_range<size_t>& range) const {
			for (size_t i = range.begin(); i < range.end(); ++i) {
				(*mPositions)[i].x += (*mVelocity)[i].x;
				(*mPositions)[i].y += (*mVelocity)[i].y;
				(*mPositions)[i].z += (*mVelocity)[i].z;
				
			}
		}
	};
}

Scene::Scene() {
	// Set world matrix
	EntityDrawer::Drawer* entityDrawer = new EntityDrawer::Drawer;
	DrawManager::gInstance->AddDrawer(*entityDrawer);
	XMStoreFloat4x4(&entityDrawer->World(), XMMatrixIdentity());

	// Create texture
	ID3D11ShaderResourceView* textureSRV;
	ShaderResourcesManager::gInstance->AddTextureFromFileSRV(TEXTURE_PATH, &textureSRV);
	ASSERT(textureSRV);

	// Generate positions
	mPositions.reserve(NUM_ENTITIES);
	mVelocity.reserve(NUM_ENTITIES);
	for (size_t i = 0; i < NUM_ENTITIES; ++i) {
		float x = Utils::RandomFloat(-POSITION_OFFSET, POSITION_OFFSET);
		float y = Utils::RandomFloat(-POSITION_OFFSET, POSITION_OFFSET);
		float z = Utils::RandomFloat(-POSITION_OFFSET, POSITION_OFFSET);
		mPositions.push_back(XMFLOAT3(x, y, z));

		x = Utils::RandomFloat(-VELOCITY_OFFSET, VELOCITY_OFFSET);
		y = Utils::RandomFloat(-VELOCITY_OFFSET, VELOCITY_OFFSET);
		z = Utils::RandomFloat(-VELOCITY_OFFSET, VELOCITY_OFFSET);
		mVelocity.push_back(XMFLOAT3(x, y, z));
	}

	// Create vertex buffer
	const size_t id = Utils::Hash("vertex_buffer");
	Utils::CreateInitializedBuffer("vertex_buffer", &mPositions[0], static_cast<unsigned int> (NUM_ENTITIES * sizeof(EntityDrawer::VertexData)), D3D11_USAGE_DYNAMIC, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE);
	mVertexBuffer = ShaderResourcesManager::gInstance->Buffer(id);
	ASSERT(mVertexBuffer);

	// Fill VertexShaderData
	entityDrawer->GetVertexShaderData().VertexBuffer() = mVertexBuffer;
	entityDrawer->GetVertexShaderData().SetVertexCount(NUM_ENTITIES);

	// Fill GeometryShaderData
	entityDrawer->GetGeometryShaderData().SetQuadHalfSize(QUAD_HALF_SIZE);

	// Fill PixelShaderData
	entityDrawer->GetPixelShaderData().SamplerState() = GlobalResources::gInstance->MinMagMipPointSampler();
	entityDrawer->GetPixelShaderData().TextureSRV() = textureSRV;
}

void Scene::Update(ID3D11Device1& device, const float elapsedTime) {
	ASSERT(mVertexBuffer);
	Positions pos;
	pos.mPositions = &mPositions;
	pos.mVelocity = &mVelocity;

	tbb::parallel_for(tbb::blocked_range<size_t>(0, NUM_ENTITIES), pos);

	Utils::CopyData(device, &mPositions[0], mPositions.size() * sizeof(XMFLOAT3), *mVertexBuffer);
}