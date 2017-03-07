// Cam.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
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
    bool m_belowMinDist;        // Used for follow ped mode
    bool m_behindPlayerDesired; // Used for follow ped mode
    bool m_camLookingAtVector;
    bool m_collisionChecksOn;
    bool m_fixingBeta; // Used for camera on a string // 4
    bool m_theHeightFixerVehicleIsATrain;
    bool m_lookBehindCamWasInFront;
    bool m_lookingBehind;
    bool m_lookingLeft; // 8
    bool m_lookingRight;
    bool m_resetStatics; // For interpolation type stuff to work
    bool m_rotating;

    s16 m_mode;  // CameraMode
    u32 m_finishTime;

    u32 m_doCollisionChecksOnFrameNum;
    u32 m_doCollisionCheckEveryNumOfFrames;
    u32 m_frameNumWereAt;
    u32 m_runningVectorArrayPos; // 32
    u32 m_runningVectorCounter;
    u32 m_directionWasLooking; // 40

    // ...


};

//VALIDATE_SIZE(CCam, 0x238);

}
