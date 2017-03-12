// Cam.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
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
#define CAM_NUM_TARGET_HISTORY 4

class CCam
{
public:
    // Used for follow ped mode
    bool m_belowMinDist;
    // Used for follow ped mode
    bool m_behindPlayerDesired;
    bool m_camLookingAtVector;
    bool m_collisionChecksOn;
    // Used for camera on a string
    bool m_fixingBeta;
    bool m_theHeightFixerVehicleIsATrain;
    bool m_lookBehindCamWasInFront;
    bool m_lookingBehind;
    bool m_lookingLeft;
    bool m_lookingRight;
    // For interpolation type stuff to work
    bool m_resetStatics;
    bool m_rotating;

    s16 m_mode; // CameraMode
    u32 m_finishTime;

    s32 m_doCollisionChecksOnFrameNum;
    s32 m_doCollisionCheckEveryNumOfFrames;
    s32 m_frameNumWereAt;
    s32 m_runningVectorArrayPos;
    s32 m_runningVectorCounter;
    s32 m_directionWasLooking;

    f32 m_maxRoleAngle;         //=DEGTORAD(5.0f)
                                // Used for adding a slight roll to the camera in the
    f32 m_roll;
    // Camera on a string mode
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
    // Used for follow ped mode
    f32 m_realGroundDist;
    f32 m_targetBeta;
    f32 m_timeElapsedf32;
    f32 m_tilt;
    f32 m_tiltSpeed;

    f32 m_transitionBeta;           // 156
    f32 m_trueBeta;                 // 160
    f32 m_trueAlpha;                // 164
    f32 m_initialPlayerOrientation; // used for first person // 168

    f32 m_fVerticalAngle;   // alpha // 172
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
    f32 m_cameraHeightMultiplier; // used by TwoPlayer_Separate_Cars_TopDown // 216

    // ped onfoot zoom distance
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

    // For targetting in cooperative mode.
    f32 m_betaTargeting; // 260
    CVector2D m_targeting;
    s32 m_carWeAreFocusingOn;  // which car is closer to the camera in 2 player cooperative mode with separate cars.
    f32 m_carWeAreFocusingOnI; // interpolated version

    f32 m_camBumpedHorz; // 280
    f32 m_camBumpedVert;
    s32 m_camBumpedTime; // 288

    CVector m_sourceSpeedOverOneFrame; // 292
    CVector m_targetSpeedOverOneFrame; // 304
    CVector m_upOverOneFrame;          // 316

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
