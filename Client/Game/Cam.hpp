// Cam.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Vector2D.hpp"
#include "Entity.hpp"
#include "Automobile.hpp"
#include "Ped.hpp"

namespace ctn::SA
{

const s32 NUMBER_OF_VECTORS_FOR_AVERAGE = 2;
const s32 CAM_NUM_TARGET_HISTORY = 4;

class CCam
{
public:
    // Used for follow ped mode
    bool m_belowMinDist;                    // +0x000
    // Used for follow ped mode
    bool m_behindPlayerDesired;             // +0x001
    bool m_camLookingAtVector;              // +0x002
    bool m_collisionChecksOn;               // +0x003
    // Used for camera on a string
    bool m_fixingBeta;                    // +0x004
    bool m_theHeightFixerVehicleIsATrain; // +0x005
    bool m_lookBehindCamWasInFront;       // +0x006
    bool m_lookingBehind;                 // +0x007
    bool m_lookingLeft;                   // +0x008
    bool m_lookingRight;                  // +0x009
    // For interpolation type stuff to work
    bool m_resetStatics;                    // +0x00A
    bool m_rotating;                        // +0x00B

    s16 m_mode;
    u32 m_finishTime;

    s32 m_doCollisionChecksOnFrameNum;
    s32 m_doCollisionCheckEveryNumOfFrames;
    s32 m_frameNumWereAt;
    s32 m_runningVectorArrayPos;
    s32 m_runningVectorCounter;
    s32 m_directionWasLooking;

    f32 m_maxRoleAngle;         //=DEGTORAD(5.0f)

    f32 m_roll;
    f32 m_rollSpeed;
    f32 m_syphonModeTargetZOffSet;
    f32 m_amountFractionObscured;
    f32 m_alphaSpeedOverOneFrame;
    f32 m_betaSpeedOverOneFrame;
    f32 m_bufferedTargetBeta;
    f32 m_bufferedTargetOrientation;
    f32 m_bufferedTargetOrientationSpeed;
    f32 m_camBufferedHeight;
    f32 m_camBufferedHeightSpeed;
    f32 m_closeInPedHeightOffset;
    f32 m_closeInPedHeightOffsetSpeed;
    f32 m_closeInCarHeightOffset;
    f32 m_closeInCarHeightOffsetSpeed;
    f32 m_dimensionOfHighestNearCar;
    f32 m_distanceBeforeChanges;
    f32 m_fovSpeedOverOneFrame;
    f32 m_minDistAwayFromCamWhenInterPolating;
    f32 m_pedBetweenCameraHeightOffset;
    f32 m_playerInFrontSyphonAngleOffSet;
    f32 m_radiusForDead;
    f32 m_realGroundDist;
    f32 m_targetBeta;
    f32 m_timeElapsedf32;
    f32 m_tilt;
    f32 m_tiltSpeed;

    f32 m_transitionBeta;
    f32 m_trueBeta;
    f32 m_trueAlpha;
    f32 m_initialPlayerOrientation;

    f32 m_fVerticalAngle;
    f32 m_AlphaSpeed;       // 176
    f32 m_FOV;              // 180
    f32 m_FOVSpeed;         // 184
    f32 m_fHorizontalAngle; // beta // 188
    f32 m_BetaSpeed;        // 192
    f32 m_Distance;         // 196
    f32 m_DistanceSpeed;
    f32 m_CA_MIN_DISTANCE; // 204
    f32 m_CA_MAX_DISTANCE;
    f32 m_SpeedVar;
    f32 m_cameraHeightMultiplier;

    f32 m_targetZoomGroundOne; // 220
    f32 m_targetZoomGroundTwo;
    f32 m_targetZoomGroundThree; // 228
                                  // ped onfoot alpha angle offset
    f32 m_targetZoomOneZExtra;
    f32 m_targetZoomTwoZExtra;
    f32 m_targetZoomTwoInteriorZExtra; // extra one for interior
    f32 m_targetZoomThreeZExtra;       // 244

    f32 m_targetZoomZCloseIn; // 248
    f32 m_minRealGroundDist;
    f32 m_targetCloseInDist;

    f32 m_betaTargeting;
    CVector2D m_targeting;
    s32 m_carWeAreFocusingOn;
    f32 m_carWeAreFocusingOnI;

    f32 m_camBumpedHorz;
    f32 m_camBumpedVert;
    s32 m_camBumpedTime;

    CVector m_sourceSpeedOverOneFrame;
    CVector m_targetSpeedOverOneFrame;
    CVector m_upOverOneFrame;

    CVector m_targetCoorsForFudgeInter; // 328
    CVector m_camFixedModeVector;       // 340
    CVector m_camFixedModeSource;       // 352
    CVector m_camFixedModeUpOffSet;     // 364
    CVector m_lastAboveWaterCamPosition; // 376  //helper for when the player has gone under the water

    CVector m_bufferedPlayerBodyOffset; // 388

    // The three vectors that determine this camera for this frame
    CVector m_front;                  // 400                                          // Direction of looking in
    CVector m_source;                 // Coors in world space
    CVector m_sourceBeforeLookBehind; // 424
    CVector m_up;                     // Just that
    CVector m_previousVectors[NUMBER_OF_VECTORS_FOR_AVERAGE]; // used to average stuff // 448

    CVector m_targetHistoryPos[CAM_NUM_TARGET_HISTORY]; // 472
    DWORD m_targetHistoryTime[CAM_NUM_TARGET_HISTORY];  // 520
    DWORD m_currentHistoryPoints;                       // 536

    CEntity* m_camTargetEntity;

    f32 m_cameraDistance; // 544
    f32 m_idealAlpha;
    f32 m_playerVelocity;             // 552
    CAutomobile* m_lastCarEntered;    // So interpolation works
    CPed* m_lastPedLookedAt;          // So interpolation works
    bool m_firstPersonRunAboutActive; // 564

    static s32 CAM_BUMPED_SWING_PERIOD;
    static s32 CAM_BUMPED_END_TIME;
    static f32 CAM_BUMPED_DAMP_RATE;
    static f32 CAM_BUMPED_MOVE_MULT;
};

VALIDATE_SIZE(CCam, 0x238);

}
