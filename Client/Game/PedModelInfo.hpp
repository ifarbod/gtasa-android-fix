// Ped model info
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "ClumpModelInfo.hpp"

namespace ctn::SA
{

class CPedModelInfo : public CClumpModelInfo
{
public:
	uint32_t m_dwAnimType;
	uint32_t m_dwPedType;
	uint32_t m_dwStatType;
	struct
	{
		uint16_t m_wNormalCar : 1;
		uint16_t m_wPoorFamilyCar : 1;
		uint16_t m_wRichFamilyCar : 1;
		uint16_t m_wExecutiveCar : 1;
		uint16_t m_wWorkerCar : 1;
		uint16_t m_wBigCar : 1;
		uint16_t m_wTaxiCar : 1;
		uint16_t m_wMoped : 1;
		uint16_t m_wMotorBike : 1;
		uint16_t m_wLeisureBoat : 1;
		uint16_t m_wWorkerBoat : 1;
		uint16_t m_wBicycle : 1;
		uint16_t m_wOnFoot : 1;
	} m_wCarCanDriveMask;
	struct
	{
		bool m_bBuysDrugs : 1;
		bool m_bUnk1 : 1;
		bool m_bUnk2 : 1;
		bool m_bUnk3 : 1;
		bool m_bUnk4 : 1;
		bool m_bUnk5 : 1;
		bool m_bUnk6 : 1;
		bool m_bUnk7 : 1;
	} m_wPedFlags;
	uint32_t field_34;
	uint8_t m_nRadio1;
	uint8_t m_nRadio2;
	uint16_t m_wModelIndex;
	uint16_t m_wVoiceType;
	uint16_t m_wVoice1, m_wVoice2, m_wVoice3;
};

VALIDATE_SIZE(CPedModelInfo, 0x44);

}
