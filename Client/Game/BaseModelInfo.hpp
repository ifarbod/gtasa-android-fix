// BaseModelInfo.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "ColModel.hpp"

namespace ctn::SA
{

class CBaseModelInfo
{
public:
    virtual ~CBaseModelInfo() = 0;
    virtual CBaseModelInfo* AsAtomicModelInfoPtr();
    virtual CBaseModelInfo* AsDamageAtomicModelInfoPtr();
    virtual CBaseModelInfo* AsLodAtomicModelInfoPtr();
    virtual void GetModelType() = 0;
    virtual class CTimeInfo* GetTimeInfo();
    virtual void Init();
    virtual void Shutdown();
    virtual void DeleteRwObject() = 0;
    virtual u32 GetRwModelType() = 0;
    virtual struct RwObject* CreateInstance(class RwMatrix* matrix) = 0;
    virtual struct RwObject* CreateInstance() = 0;
    virtual void SetAnimFile(char* filename);
    virtual void ConvertAnimFileIndex();
    virtual s32 GetAnimFileIndex();

public:
    u32 m_hashKey;
    u16 m_refCount;
    s16 m_txdSlot;
    u8 m_alpha;
    u8 m_num2dfx;
    s16 m_2dfxIndex;
    s16 m_objectIndex;
    u16 m_flags;
    CColModel* m_colModel;
    f32 m_drawDistance;

    union
    {
        struct RwObject* m_rwObject;
        struct RpClump* m_rpClump;
        struct RpAtomic* m_rpAtomic;
    };
};

VALIDATE_SIZE(CBaseModelInfo, 0x20);

}
