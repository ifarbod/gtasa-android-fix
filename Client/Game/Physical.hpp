// Physical entity base
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "Entity.hpp"
#include "EntryInfoList.hpp"
#include "PtrNodeDoubleLink.hpp"
#include "Quaternion.hpp"

namespace ctn::SA
{

// TODO: Reverse this?
class CRealTimeShadow;

class CPhysical : public CEntity
{
public:
    f32 m_field038;                        // +0x038
    u32 m_lastCollisionTime;               // +0x03C

    union
    {
        struct
        {
            //  0  0x00000001
            u32 m_unknown0 : 1;
            //  1  0x00000002
            u32 m_applyGravity : 1;
            //  2  0x00000004  m_disableCollisionForce ?
            u32 m_disableFriction : 1;
            //  3  0x00000008
            u32 m_collidable : 1;
            //  4  0x00000010
            u32 m_disableTurnForce : 1;
            //  5  0x00000020  m_disableMoveForce ?
            u32 m_disableMovement : 1;
            //  6  0x00000040
            u32 m_infiniteMass : 1;
            //  7  0x00000080
            u32 m_disableZ : 1;

            //  8  0x00000100
            u32 m_submergedInWater : 1;
            //  9  0x00000200
            u32 m_onSolidSurface : 1;
            // 10  0x00000400
            u32 m_broken : 1;
            // 11  0x00000800  Xref @ 0x6F5CF0 - TrainHitStuff(CPtrList &, CEntity *)
            u32 m_unknown11 : 1;
            // 12  0x00001000
            u32 m_unknown12 : 1;
            // 13  0x00002000
            u32 m_dontApplySpeed : 1;
            // 14  0x00004000
            u32 m_unknown14 : 1;
            // 15  0x00008000
            u32 m_unknown15 : 1;

            // 16  0x00010000
            u32 m_unknown16 : 1;
            // 17  0x00020000  Xref @ CPhysical::ProcessCollision
            u32 m_unknown17 : 1;
            // 18  0x00040000
            u32 m_bulletProof : 1;
            // 19  0x00080000
            u32 m_fireProof : 1;
            // 20  0x00100000
            u32 m_collisionProof : 1;
            // 21  0x00200000
            u32 m_meleeProof : 1;
            // 22  0x00400000
            u32 m_invulnerable : 1;
            // 23  0x00800000
            u32 m_explosionProof : 1;

            // 24  0x01000000
            u32 m_unknown24 : 1;
            // 25  0x02000000
            u32 m_attachedToEntity : 1;
            // 26  0x04000000
            u32 m_unknown26 : 1;
            // 27  0x08000000
            u32 m_touchingWater : 1;
            // 28  0x10000000
            u32 m_enableCollision : 1;
            // 29  0x20000000
            u32 m_destroyed : 1;
            // 30  0x40000000
            u32 m_unknown30 : 1;
            // 31  0x80000000
            u32 m_unknown31 : 1;
        };
        u32 m_physicalFlags;               // +0x040
    };
    CVector m_moveSpeed;                   // +0x044 Velocity ?
    CVector m_turnSpeed;                   // +0x050 Angular velocity ?
    CVector m_frictionMoveSpeed;           // +0x05C Acceleration ?
    CVector m_frictionTurnSpeed;           // +0x068 Angular acceleration
    CVector m_force;                       // +0x074
    CVector m_torque;                      // +0x080
    f32 m_mass;                            // +0x08C
    f32 m_turnMass;                        // +0x090
    f32 m_velocityFrequency;               // +0x094
    f32 m_airResistance;                   // +0x098
    f32 m_elasticity;                      // +0x09C
    f32 m_buoyancyConstant;                // +0x0A0
    CVector m_centerOfMass;                // +0x0A4
    CEntryInfoList m_entryInfoList;        // +0x0B0
    CPtrNodeDoubleLink* m_movingList;      // +0x0B4
    u8 m_field0B8;                         // +0x0B8
    u8 m_numEntitiesCollided;              // +0x0B9
    u8 m_contactSurface;                   // +0x0BA
    u8 m_field0BB;                         // +0x0BB
    CEntity* m_collidedEntities[6];        // +0x0BC
    f32 m_movingSpeed;                     // +0x0D4
    f32 m_damageIntensity;                 // +0x0D8
    CEntity* m_damageEntity;               // +0x0DC
    CVector m_lastCollisionImpactVelocity; // +0x0E0 m_lastCollisionDirection ?
    CVector m_lastCollisionPosition;       // +0x0EC
    u16 m_damagedPieceType;                // +0x0F8 m_damagedBodyPart ?
    u16 m_field0FA;                        // +0x0FA
    CPhysical* m_attachedTo;               // +0x0FC
    CVector m_attachOffset;                // +0x100
    CVector m_attachedEntityPosition;      // +0x10C m_attachRotation ?
    CQuaternion m_attachedEntityRotation;  // +0x118
    CEntity* m_entityIgnoredCollision;     // +0x128
    f32 m_contactSurfaceBrightness;        // +0x12C
    u32 m_dynamicLighting;                 // +0x130
    CRealTimeShadow* m_shadow;             // +0x134 m_shadowData ?
};

VALIDATE_SIZE(CPhysical, 0x138);

}
