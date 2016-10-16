// String library
// Author(s):       AliAssassiN <ailsynthax@gmail.com>
//
// Copyright (c) 2015-2016 San Andreas Online
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Util.Preprocessors.hpp"
#include "Util.MemHelpers.hpp"

#ifdef SAO_WIN32
    #include "Win32\Util.Win32Headers.hpp"
#elif defined(SAO_UNIX)
    // TODO
#endif

#include <tuple>

template<class... Targs>
class VariadicParams
{
private:

    std::tuple<Targs...> tupleParams;

public:

    VariadicParams(Targs... Args)
    {
        tupleParams = std::tuple<Targs...>(std::forward<TArgs>(Args)...);
    }

    bool DoesParamExists(const u32 uiPosition)
    {
        return (static_cast<u32>(std::tuple_size<tupleParams>>::value) > uiPosition);
    }

    const char* GetParamTypeName(const u32 uiPosition)
    {
        assert(DoesParamExists(uiPosition));

        return typeid(std::tuple_element<uiPosition, tupleParams>::type).name();
    }

    template<const u32 uiPosition>
    decltype(std::get<uiPosition>(tupleParams)) GetParam()
    {
        assert(DoesParamExists(uiPosition));

        return std::get<uiPosition>(tupleParams);
    }
};

enum eStringType
{
    UTF8,
    UTF16,
    UTF32,
    UTF64
};

template<class AllocT>
class String
{
private:

    AllocT* m_pData;
    u32 m_uiSize;
    u32 m_uiLength;

public:

    String() : m_pData(nullptr), m_uiSize(0), m_uiLength(0) {}
    
    String(const char* szText)
    {
        Resize(strlen(szText));
        
    }

    void Resize(const u32 uiSize)
    {
        assert(uiSize > 0);
        if(uiSize > 1) m_uiSize = uiSize + 1;
        else m_uiSize = 1;
        
        // Not initialized at all
        if(!m_pData)
        {
            m_pData = new AllocT[m_uiSize];
            return;
        }

        // Initialized and it isn't empty
        if(!IsEmpty())
        {
            if(m_uiLength > m_uiSize) m_uiLength = uiSize;
            AllocT* pTemp = new AllocT[m_uiSize];
            MemCopy(pTemp, m_pData, m_uiLength);
            pTemp[uiSize] = '\0';

            delete[] m_pData;
            m_pData = new AllocT[m_uiSize];
            MemCopy(m_pData, pTemp, m_uiLength);
            m_pData[uiSize] = '\0';

            delete[] pTemp;
        }
        else // Initialized but empty
        {
            delete[] m_pData;
            m_pData = new AllocT[m_uiSize];
        }
    }

    bool IsEmpty() const { return (m_uiLength); }

    eStringType GetStringType() const
    {
        switch(decltype(AllocT))
        {
            case s8:
            case u8:
                return UTF8;

            case s16:
            case u16:
                return UTF16;

            case s32:
            case u32:
                return UTF32;

            case s64:
            case u64:
                return UTF64;
        }
    }
};

using Str = String<char>;
using WStr = String<wchar_t>;
