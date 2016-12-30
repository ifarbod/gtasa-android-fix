// Camera stuff
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include <Hooking/HookingUtils.hpp>
#include <Hooking/HookFunction.hpp>

using namespace Util;

static HookFunction hookFunction([]()
{
    // Disable CPopulation::AddToPopulation
    MakeRET0Ex(0x614720);

    // Disable CObject::ProcessSamSiteBehaviour
    MakeRET(0x5A07D0);

    // Disable CTrain::DoTrainGenerationAndRemoval
    MakeRET(0x6F7900);

    // Prevent trains spawning with peds
    MakeJMP(0x6F7865);
    MakeJMP(0x6F8E7B, 0x6F8F89);
    MakeNOP(0x6F8E80);

    // Disable CPlane::DoPlaneGenerationAndRemoval
    MakeRET(0x6CD2F0);

    // Disable CCarCtrl::GenerateEmergencyServicesCar
    MakeRET(0x42F9C0);

    // Disable CCarCtrl::GenerateOneEmergencyServicesCar
    MakeRET(0x42B7D0);

    // Disable CCarCtrl::ScriptGenerateOneEmergencyServicesCar
    MakeRET(0x42FBC0);

    // Disable CTheCarGenerators::Process
    MakeRET(0x6F3F40);

    // Disable CCarCtrl::RemoveDistantCars
    MakeRET(0x42CD10);

    // Disable CCarCtrl::RemoveCarsIfThePoolGetsFull
    MakeRET(0x4322B0);

    // Disable CCarCtrl::GenerateRandomCars
    MakeRET(0x4341C0);

    // Disable CCarCtrl::GenerateOneRandomCar
    MakeRET(0x430050);
});
