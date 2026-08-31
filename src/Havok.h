#pragma once

#include "RE/H/hkArray.h"
#include "RE/H/hkMemoryRouter.h"
#include "RE/H/hkpConvexVerticesShape.h"

namespace RE
{
	struct hkpConvexVerticesShapeBuildConfig
	{
		bool         createConnectivity;
		bool         shrinkByConvexRadius;
		bool         useOptimizedShrinking;
		float        convexRadius;
		std::int32_t maxVertices;
		float        maxRelativeShrink;
		float        maxShrinkingVerticesDisplacement;
		float        maxCosAngleForBevelPlanes;
	};
	static_assert(sizeof(hkpConvexVerticesShapeBuildConfig) == 0x18);

	struct hkStridedVertices
	{
		hkStridedVertices() = default;

		hkStridedVertices(const hkArrayBase<hkVector4>& a_vertices)
		{
			set(a_vertices);
		}

		hkStridedVertices(const hkVector4* a_vertices, std::int32_t a_numVertices)
		{
			set(a_vertices, a_numVertices);
		}

		const float* vertices{ nullptr };
		std::int32_t numVertices{ 0 };
		std::int32_t striding{ 0 };

		void set(const hkArrayBase<hkVector4>& a_vertices)
		{
			set(a_vertices.begin(), a_vertices.size());
		}

		template <class T>
		void set(const T* a_vertices, std::int32_t a_numVertices)
		{
			vertices = reinterpret_cast<const float*>(a_vertices);
			numVertices = a_numVertices;
			striding = static_cast<std::int32_t>(sizeof(T));
		}
	};
	static_assert(sizeof(hkStridedVertices) == 0x10);
}

inline void* hkHeapAlloc(std::int32_t a_numBytes)
{
	return RE::hkMemoryRouter::GetInstance().Heap->BlockAlloc(a_numBytes);
}
