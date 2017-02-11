// Event.hpp
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

class CEvent
{
public:
    virtual ~CEvent() = 0;

public:
    enum eEventEntityType m_eEventType; // What type of event is it?
    enum eEventEntity m_eEventEntity;   // What type of entity does the event relate to?
    DWORD m_nEventEntityRef; // Unique reference to the event entity. Can be used to get a pointer to the event entity.
    CEntity* m_pCommitedBy;  // for crimes, keeps track of who commited the crime
    CVector m_vecEventPosition; // Position of an event. Can register an event using just
    DWORD m_nEventExpiryTime;   // a type and position rather than a type and an entity
    DWORD m_nEventRemoveFrame;  // Time when this event will be removed from the event list
};

// biggest child: 0x44
VALIDATE_SIZE(CEvent, 0x28); // wrong?

}
