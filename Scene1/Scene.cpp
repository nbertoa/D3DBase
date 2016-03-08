#include "Scene.h"

#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/tick_count.h>

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
	const size_t NUM_ENTITIES = 1000000;
	const float QUAD_HALF_SIZE = 0.125f;
	const float POSITION_OFFSET = 50.0f;
	const float VELOCITY_OFFSET = 1.0f;
	const char* TEXTURE_PATH = "..\\..\\content\\textures\\flare.dds";
}

namespace EntitiesScene {
	void simulation(XMFLOAT3* *currentPosBuffer, XMFLOAT3* pos1, XMFLOAT3* pos2, XMFLOAT3* vel, const float* elapsedTime) {
		ASSERT(currentPosBuffer);
		ASSERT(pos1);
		ASSERT(pos2);
		ASSERT(vel);
		ASSERT(elapsedTime);

		XMFLOAT3* primaryPosBuffer = pos2;
		while (*currentPosBuffer) {
			tbb::task_scheduler_init init(6);

			tbb::tick_count t0 = tbb::tick_count::now();
			tbb::parallel_for(tbb::blocked_range<size_t>(0, NUM_ENTITIES), 
			[=](const tbb::blocked_range<size_t>& r) {
				for (size_t i = r.begin(); i < r.end(); ++i) {
					primaryPosBuffer[i].x = (*currentPosBuffer)[i].x + vel[i].x * *elapsedTime;
					primaryPosBuffer[i].y = (*currentPosBuffer)[i].y + vel[i].y * *elapsedTime;
					primaryPosBuffer[i].z = (*currentPosBuffer)[i].z + vel[i].z * *elapsedTime;
					vel[i].x *= (-POSITION_OFFSET < primaryPosBuffer[i].x && primaryPosBuffer[i].x < POSITION_OFFSET) ? 1.0f : -1.0f;
					vel[i].y *= (-POSITION_OFFSET < primaryPosBuffer[i].y && primaryPosBuffer[i].y < POSITION_OFFSET) ? 1.0f : -1.0f;
					vel[i].z *= (-POSITION_OFFSET < primaryPosBuffer[i].z && primaryPosBuffer[i].z < POSITION_OFFSET) ? 1.0f : -1.0f;
				}
			}
			);
			tbb::tick_count t1 = tbb::tick_count::now();
			const double ticks = (t1 - t0).seconds() * 1000;

			XMFLOAT3* tmp = primaryPosBuffer;
			primaryPosBuffer = *currentPosBuffer;
			*currentPosBuffer = tmp;
		}
	}

	Scene::Scene() 
		: mPositions1(new XMFLOAT3[NUM_ENTITIES])
		, mPositions2(new XMFLOAT3[NUM_ENTITIES])
		, mPositionsBufferPtr(mPositions1)
		, mVelocities(new XMFLOAT3[NUM_ENTITIES]) 
		, mElapsedTime(0.0f)
	{
		// Set world matrix
		EntityDrawer::Drawer* entityDrawer = new EntityDrawer::Drawer;
		DrawManager::gInstance->AddDrawer(*entityDrawer);
		XMStoreFloat4x4(&entityDrawer->World(), XMMatrixIdentity());

		// Create texture
		ID3D11ShaderResourceView* textureSRV;
		ShaderResourcesManager::gInstance->AddTextureFromFileSRV(TEXTURE_PATH, &textureSRV);
		ASSERT(textureSRV);

		// Generate positions
		for (size_t i = 0; i < NUM_ENTITIES; ++i) {
			float x = Utils::RandomFloat(-POSITION_OFFSET, POSITION_OFFSET);
			float y = Utils::RandomFloat(-POSITION_OFFSET, POSITION_OFFSET);
			float z = Utils::RandomFloat(-POSITION_OFFSET, POSITION_OFFSET);
			mPositions1[i] = XMFLOAT3(x, y, z);
			mPositions2[i] = XMFLOAT3(x, y, z);

			x = Utils::RandomFloat(-VELOCITY_OFFSET, VELOCITY_OFFSET);
			y = Utils::RandomFloat(-VELOCITY_OFFSET, VELOCITY_OFFSET);
			z = Utils::RandomFloat(-VELOCITY_OFFSET, VELOCITY_OFFSET);
			mVelocities[i] = XMFLOAT3(x, y, z);
		}

		// Create vertex buffer
		const size_t id = Utils::Hash("vertex_buffer");
		Utils::CreateInitializedBuffer("vertex_buffer", mPositionsBufferPtr, static_cast<unsigned int> (NUM_ENTITIES * sizeof(EntityDrawer::VertexData)), D3D11_USAGE_DYNAMIC, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE);
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
		
		// Simulation thread
		mSimulationThread = std::thread(simulation, &mPositionsBufferPtr, mPositions1, mPositions2, mVelocities, &mElapsedTime);
	}

	Scene::~Scene() {
		mPositionsBufferPtr = nullptr;
		mSimulationThread.join();

		delete[] mPositions1;
		delete[] mPositions2;
		delete[] mVelocities;	
	}

	void Scene::Update(ID3D11Device1& device, const float elapsedTime) {
		mElapsedTime = elapsedTime;

		ASSERT(mVertexBuffer);

		// Single Threaded Simulation
		/*for (size_t i = 0; i < NUM_ENTITIES; ++i) {
			mPositions1[i].x += mVelocities[i].x * mElapsedTime;
			mPositions1[i].y += mVelocities[i].y * mElapsedTime;
			mPositions1[i].z += mVelocities[i].z * mElapsedTime;
			mVelocities[i].x *= (-POSITION_OFFSET < mPositions1[i].x && mPositions1[i].x < POSITION_OFFSET) ? 1.0f : -1.0f;
			mVelocities[i].y *= (-POSITION_OFFSET < mPositions1[i].y && mPositions1[i].y < POSITION_OFFSET) ? 1.0f : -1.0f;
			mVelocities[i].z *= (-POSITION_OFFSET < mPositions1[i].z && mPositions1[i].z < POSITION_OFFSET) ? 1.0f : -1.0f;
		}*/

		// Multithreaded Simulation
		/*tbb::parallel_for(tbb::blocked_range<size_t>(0, NUM_ENTITIES), 
			[&](const tbb::blocked_range<size_t>& r) {
			for (size_t i = r.begin(); i < r.end(); ++i) {
				mPositions1[i].x += mVelocities[i].x * mElapsedTime;
				mPositions1[i].y += mVelocities[i].y * mElapsedTime;
				mPositions1[i].z += mVelocities[i].z * mElapsedTime;
				mVelocities[i].x *= (-POSITION_OFFSET < mPositions1[i].x && mPositions1[i].x < POSITION_OFFSET) ? 1.0f : -1.0f;
				mVelocities[i].y *= (-POSITION_OFFSET < mPositions1[i].y && mPositions1[i].y < POSITION_OFFSET) ? 1.0f : -1.0f;
				mVelocities[i].z *= (-POSITION_OFFSET < mPositions1[i].z && mPositions1[i].z < POSITION_OFFSET) ? 1.0f : -1.0f;
			}
		});*/

		// Draw
		Utils::CopyData(device, mPositionsBufferPtr, NUM_ENTITIES * sizeof(XMFLOAT3), *mVertexBuffer);
	}
}

