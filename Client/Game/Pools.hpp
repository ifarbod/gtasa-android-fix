// Manages game's entity pools
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Pool.hpp"

#include "PtrNodeSingleLink.hpp"
#include "PtrNodeDoubleLink.hpp"
#include "PlayerPed.hpp"
#include "Heli.hpp"
#include "Building.hpp"
#include "Object.hpp"
#include "Dummy.hpp"
#include "ColModel.hpp"
#include "Task.hpp"
#include "Event.hpp"
#include "PedAttractor.hpp"

namespace ctn::SA
{

using CPtrNodeSinglePool = CPool<CPtrNodeSingleLink>;
using CPtrNodeDoublePool = CPool<CPtrNodeDoubleLink>;
using CEntryInfoNodePool = CPool<CEntryInfoNode>;
//using CPedPool = CPool<CPed, CCopPed>;
using CVehiclePool = CPool<CVehicle, CHeli>;
using CBuildingPool = CPool<CBuilding>;
//using CObjectPool = CPool<CObject, CCutsceneObject>;
using CDummyPool = CPool<CDummy>;
using CColModelPool = CPool<CColModel>;
//using CTaskPool = CPool<CTask, CTaskSimpleSlideToCoord>;
using CEventPool = CPool<CEvent>;
//using CPointRoutePool = CPool<CPointRoute>;
//using CPatrolRoutePool = CPool<CPatrolRoute>;
//using CNodeRoutePool = CPool<CNodeRoute>;
//using CTaskAllocatorPool = CPool<CTaskAllocator>;
//using CPedIntelligencePool = CPool<CPedIntelligence>;
using CPedAttractorPool = CPool<CPedAttractor>;


// Manages game's entity pools.
class CPools
{
public:
    // Initializes pools.
    void Initialize();
    // Shutdowns pools.
    void Shutdown();
};

}

