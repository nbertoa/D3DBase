#include "Scene.h"

#include <Camera/Camera.h>
#include <DrawManager/DrawManager.h>
#include <EntityDrawer\EntityDrawer.h>
#include <EntityDrawer\VertexType.h>
#include <Input/Keyboard.h> 
#include <ShaderResourcesManager/GlobalResources.h>    
#include <ShaderResourcesManager/ShaderResourcesManager.h>  
#include <Utils/Assert.h>  
#include <Utils\MathUtils.h>

using namespace DirectX;

namespace {
	const size_t NUM_ENTITIES = 10000000;
	const float QUAD_HALF_SIZE = 0.125f;
	const float POSITION_OFFSET = 10.0f;
	const char* TEXTURE_PATH = "..\\..\\content\\textures\\flare.dds";
}

Scene::Scene() {
	// Set world matrix
	XMStoreFloat4x4(&DrawManager::gInstance->GetEntityDrawer().World(), XMMatrixIdentity());	 

	// Create texture
	ID3D11ShaderResourceView* textureSRV;
	ShaderResourcesManager::gInstance->AddTextureFromFileSRV(TEXTURE_PATH, &textureSRV);
	ASSERT(textureSRV);

	// Generate positions
	std::vector<XMFLOAT4> positions;
	positions.reserve(NUM_ENTITIES);
	for (size_t i = 0; i < NUM_ENTITIES; ++i) {
		const float x = Utils::RandomFloat(-POSITION_OFFSET, POSITION_OFFSET);
		const float y = Utils::RandomFloat(-POSITION_OFFSET, POSITION_OFFSET);
		const float z = Utils::RandomFloat(-POSITION_OFFSET, POSITION_OFFSET);
		positions.push_back(XMFLOAT4(x, y, z, 1.0f));
	}

	// Create vertex buffer
	size_t id = VertexData::CreateVertexBuffer("vertex_buffer", &positions[0], NUM_ENTITIES);
	ID3D11Buffer* vertexBuffer = ShaderResourcesManager::gInstance->Buffer(id);
	ASSERT(vertexBuffer);

	// Fill VertexShaderData
	DrawManager::gInstance->GetEntityDrawer().GetVertexShaderData().VertexBuffer() = vertexBuffer;
	DrawManager::gInstance->GetEntityDrawer().GetVertexShaderData().SetVertexCount(NUM_ENTITIES);

	// Fill GeometryShaderData
	DrawManager::gInstance->GetEntityDrawer().GetGeometryShaderData().SetQuadHalfSize(QUAD_HALF_SIZE);

	// Fill PixelShaderData
	DrawManager::gInstance->GetEntityDrawer().GetPixelShaderData().SamplerState() = GlobalResources::gInstance->MinMagMipPointSampler();
	DrawManager::gInstance->GetEntityDrawer().GetPixelShaderData().TextureSRV() = textureSRV;
}

void Scene::Update(const float elapsedTime) {

}