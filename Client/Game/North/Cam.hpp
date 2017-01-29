// Cam.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"

namespace ctn::SA
{

class CCam
{
public:
    bool belowMinDist_;        // Used for follow ped mode
    bool behindPlayerDesired_; // Used for follow ped mode
    bool camLookingAtVector_;
    bool collisionChecksOn_;
    bool fixingBeta_; // Used for camera on a string // 4
    bool theHeightFixerVehicleIsATrain_;
    bool lookBehindCamWasInFront_;
    bool lookingBehind_;
    bool lookingLeft_; // 8
    bool lookingRight_;
    bool resetStatics_; // For interpolation type stuff to work
    bool rotating_;

    s16 mode_;  // CameraMode
    u32 finishTime_;

    u32 doCollisionChecksOnFrameNum_;
    u32 doCollisionCheckEveryNumOfFrames_;
    u32 frameNumWereAt_;
    u32 runningVectorArrayPos; // 32
    u32 runningVectorCounter;
    u32 directionWasLooking; // 40

    // ...


};

//VALIDATE_SIZE(CCam, 0x238);

}
