// Population hooks
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include <Hooking/Hook.hpp>
#include <Hooking/HookFunction.hpp>

using namespace ctn;
using namespace ctn::Hook;

static HookFunction hookFunction([]()
{
    // Disable CPopulation::Initialise
    MakeRet(0x610E10);

    // Disable CPopCycle::Initialise
    MakeRet(0x5BC090);

    // Disable CPedType::LoadPedData
    MakeRet(0x608B30);

    // Disable CDecisionMakerTypesFileLoader::LoadDefaultDecisionMaker
    MakeRet(0x5BF400);
    // Disable CPedStats::LoadPedStats
    MakeRet(0x5BB890);
    // Change CPedStats::fHeadingChangeRate (was 15.0)
    MemWrite<f32>(0x5BFA1D + 4, 9.5f);

    // Disable CStreaming::StreamVehiclesAndPeds_Always
    MakeRet(0x40B650);

    // Stop the loading of ambient traffic models and textures
    // by skipping CStreaming::StreamVehiclesAndPeds() and CStreaming::StreamZoneModels()
    MakeShortJmp(0x40E7DF);

    // Disable CPopulation::AddToPopulation
    MakeRet0(0x614720);

    // Disable CPopulation::Update
    //MakeRet(0x616650);

    // Disable CTrain::DoTrainGenerationAndRemoval
    MakeRet(0x6F7900);

    // Prevent trains spawning with peds
    MakeShortJmp(0x6F7865);
    MakeJmp(0x6F8E7B, 0x6F8F89);
    MakeNop(0x6F8E80);

    // Disable CPlane::DoPlaneGenerationAndRemoval
    MakeRet(0x6CD2F0);

    // Disable CCarCtrl::GenerateEmergencyServicesCar
    MakeRet(0x42F9C0);

    // Disable CCarCtrl::GenerateOneEmergencyServicesCar
    MakeRet(0x42B7D0);

    // Disable CCarCtrl::ScriptGenerateOneEmergencyServicesCar
    MakeRet(0x42FBC0);

    // Disable CTheCarGenerators::Process
    MakeRet(0x6F3F40);

    // Disable CCarCtrl::RemoveDistantCars
    MakeRet(0x42CD10);

    // Disable CCarCtrl::RemoveCarsIfThePoolGetsFull
    MakeRet(0x4322B0);

    // Disable CCarCtrl::GenerateRandomCars
    MakeRet(0x4341C0);

    // Disable CCarCtrl::GenerateOneRandomCar
    MakeRet(0x430050);
});
