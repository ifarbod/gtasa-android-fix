// RenderWare macros
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 CtNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

// rwplcore.h

#define RwInt32MAXVAL 0x7FFFFFFF
#define RwInt32MINVAL 0x80000000
#define RwUInt32MAXVAL 0xFFFFFFFF
#define RwUInt32MINVAL 0x00000000
#define RwRealMAXVAL (RwReal)(3.40282347e+38)
#define RwRealMINVAL (RwReal)(1.17549435e-38)
#define RwInt16MAXVAL 0x7FFF
#define RwInt16MINVAL 0x8000
#define RwUInt16MAXVAL 0xFFFF
#define RwUInt16MINVAL 0x0000

#define RwInt32FromRealMacro(x) ((RwInt32)(x))
#define RwFastRealToUInt32(_x) ((RwUInt32)RwInt32FromRealMacro(((RwReal)(_x))))

#define _RW_C1 ((float)4.1666667908e-02)
#define _RW_C2 ((float)-1.3888889225e-03)
#define _RW_C3 ((float)2.4801587642e-05)
#define _RW_C4 ((float)-2.7557314297e-07)
#define _RW_C5 ((float)2.0875723372e-09)
#define _RW_C6 ((float)-1.1359647598e-11)
#define _RW_S1 ((float)-1.6666667163e-01)
#define _RW_S2 ((float)8.3333337680e-03)
#define _RW_S3 ((float)-1.9841270114e-04)
#define _RW_S4 ((float)2.7557314297e-06)
#define _RW_S5 ((float)-2.5050759689e-08)
#define _RW_S6 ((float)1.5896910177e-10)
#define _RW_one ((float)1.0000000000e+00)
#define _RW_pS0 ((float)1.6666667163e-01)
#define _RW_pS1 ((float)-3.2556581497e-01)
#define _RW_pS2 ((float)2.0121252537e-01)
#define _RW_pS3 ((float)-4.0055535734e-02)
#define _RW_pS4 ((float)7.9153501429e-04)
#define _RW_pS5 ((float)3.4793309169e-05)
#define _RW_pi ((float)3.1415925026e+00)
#define _RW_pi_tol ((float)0.0312500000e+00)
#define _RW_pio2_hi ((float)1.5707962513e+00)
#define _RW_pio2_lo ((float)7.5497894159e-08)
#define _RW_qS1 ((float)-2.4033949375e+00)
#define _RW_qS2 ((float)2.0209457874e+00)
#define _RW_qS3 ((float)-6.8828397989e-01)
#define _RW_qS4 ((float)7.7038154006e-02)

#define rwLIBRARYBASEVERSION 0x34000
#define rwLIBRARYNOWARNVERSION 0x35000
#define rwLIBRARYCURRENTVERSION 0x36003
#define rwLIBRARYVERSION34001 0x34001
#define RWBUILDNUMBER 0xffff

#define RWFORCEENUMSIZEINT ((RwInt32)((~((RwUInt32)0)) >> 1))

#define RW_STRINGIFY(X) #X
#define RW_STRINGIFY_EXPANDED(X) RW_STRINGIFY(X)

#define __RWUNUSED__
#define __RWUNUSEDRELEASE__
#define __RWFORMAT__(_archetype, _string_index, _first_to_check)

#define RWASMCALL __cdecl
#define RWASMAPI(TYPE) TYPE RWASMCALL

#define rwMAXPIPECONTEXT 10

#ifndef MACRO_START
#define MACRO_START do
#endif /* MACRO_START */

#ifndef MACRO_STOP
#define MACRO_STOP while (0)
#endif /* MACRO_STOP */

#ifdef FALSE
#undef FALSE
#endif
#define FALSE 0

#ifdef TRUE
#undef TRUE
#endif
#define TRUE !FALSE

#define _RWSTRING(x) x
#define RWSTRING(x) _RWSTRING(x)

#define RwSphereAssign(_target, _source) (*(_target) = *(_source))
#define RwLineAssign(_target, _source) (*(_target) = *(_source))

#define rwMAXTEXTURECOORDS 8

#define rwSLLinkGetData(link, type, entry) ((type*)(((RwUInt8*)(link)) - offsetof(type, entry)))

#define rwSLLinkGetConstData(link, type, entry) ((const type*)(((const RwUInt8*)(link)) - offsetof(type, entry)))

#define rwSLLinkInitialize(linkvar) (linkvar)->next = NULL;

#define rwSLLinkGetNext(linkvar) ((linkvar)->next)

#define rwSingleListInitialize(list) (list)->link.next = NULL;
#define rwSingleListEmpty(list) (((list)->link.next) == NULL)
#define rwSingleListAddSLLink(list, linkvar) ((linkvar)->next = (list)->link.next, (list)->link.next = (linkvar))
#define rwSingleListGetFirstSLLink(list) ((list)->link.next)
#define rwSingleListGetTerminator(list) (NULL)

#define rwLLLinkGetData(linkvar, type, entry) ((type*)(((RwUInt8*)(linkvar)) - offsetof(type, entry)))

#define rwLLLinkGetConstData(linkvar, type, entry) ((const type*)(((const RwUInt8*)(linkvar)) - offsetof(type, entry)))

#define rwLLLinkGetNext(linkvar) ((linkvar)->next)

#define rwLLLinkGetPrevious(linkvar) ((linkvar)->prev)

#define rwLLLinkInitialize(linkvar) ((linkvar)->prev = (RwLLLink*)NULL, (linkvar)->next = (RwLLLink*)NULL)

#define rwLLLinkAttached(linkvar) ((linkvar)->next)

#define rwLinkListInitialize(list) ((list)->link.next = ((RwLLLink*)(list)), (list)->link.prev = ((RwLLLink*)(list)))
#define rwLinkListEmpty(list) (((list)->link.next) == (&(list)->link))
#define rwLinkListAddLLLink(list, linkvar)                                                                             \
    ((linkvar)->next = (list)->link.next, (linkvar)->prev = (&(list)->link), ((list)->link.next)->prev = (linkvar),    \
        (list)->link.next = (linkvar))
#define rwLinkListRemoveLLLink(linkvar)                                                                                \
    (((linkvar)->prev)->next = (linkvar)->next, ((linkvar)->next)->prev = (linkvar)->prev)
#define rwLinkListGetFirstLLLink(list) ((list)->link.next)
#define rwLinkListGetLastLLLink(list) ((list)->link.prev)
#define rwLinkListGetTerminator(list) (&((list)->link))

#if (!defined(RwSurfacePropertiesAssign))
#define RwSurfacePropertiesAssign(_target, _source) (*(_target) = *(_source))
#endif // (!defined(RwSurfacePropertiesAssign))

#define RWFIX_MIN (1)
#define RWFIX_MAX (0x7fffffff)
#define RwFixedToInt(a) ((a) >> 16)
#define RwFixedToFloat(a) ((float)(((float)(a)) * (1.0f / 65536.0f)))
#define RwFixedToReal(a) ((RwReal)(((RwReal)(a)) * (1.0f / 65536.0f)))
#define RwIntToFixed(a) ((a) << 16)
#define RwRealToFixed(a) (RwInt32FromRealMacro((a)*65536.0f))
#define RwRealAbs(a) ((RwReal)((a) >= (RwReal)(0.0) ? (a) : (-(a))))
#define RwRealMin2(a, b) ((RwReal)(((a) <= (b)) ? (a) : (b)))
#define RwRealMax2(a, b) ((RwReal)(((a) >= (b)) ? (a) : (b)))
#define RwRealMin3(a, b, c) RwRealMin2(a, RwRealMin2(b, c))
#define RwRealMax3(a, b, c) RwRealMax2(a, RwRealMax2(b, c))

#ifndef NORWREALSHORTCUT
#define RToFixed RwRealToFixed
#define RAbs RwRealAbs
#define FxToInt RwFixedToInt
#define FxToFloat RwFixedToFloat
#define FxToReal RwFixedToFloat
#endif

#ifndef rwPI
#define rwPI ((RwReal)(3.1415926535f))
#define rwPIOVER2 (rwPI / (RwReal)(2.0f))
#endif
#define RWRGBALONG(r, g, b, a) ((RwUInt32)(((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))

#define rwSECTORATOMIC -1
#define rwSECTORBUILD -2 /* Only used when building a world */

/* vect is a RwV3d, y is the component */
#define GETCOORD(vect, y) (*(RwReal*)(((RwUInt8*)(&((vect).x))) + (RwInt32)(y)))
#define GETCONSTCOORD(vect, y) (*(const RwReal*)(((const RwUInt8*)(&((vect).x))) + (RwInt32)(y)))
#define SETCOORD(vect, y, value) (((*(RwReal*)(((RwUInt8*)(&((vect).x))) + (RwInt32)(y)))) = (value))
#define SETCONSTCOORD(vect, y, value) (((*(const RwReal*)(((const RwUInt8*)(&((vect).x))) + (RwInt32)(y)))) = (value))
#define GETCOORDINT(vect, y) (*(RwInt32*)(((RwUInt8*)(&((vect).x))) + (y)))
#define GETCONSTCOORDINT(vect, y) (*(const RwInt32*)(((const RwUInt8*)(&((vect).x))) + (y)))

/*
* Object Types - these are used in the binary object
* representations and in the debug library. They must
* be unique.  They are the old system.
*/

#define rwID_DATABASE 0x64617462 /* datb */

#define MAKECHUNKID(vendorID, chunkID) (((vendorID & 0xFFFFFF) << 8) | (chunkID & 0xFF))
#define GETOBJECTID(chunkID) (chunkID & 0xFF)
#define GETVENDORID(chunkID) ((chunkID >> 8) & 0xFFFFFF)

#define rwObjectCopy(d, s)                                                                                             \
    \
MACRO_START                                                                                                     \
    \
{                                                                                                               \
        ((RwObject*)(d))->type = ((const RwObject*)(s))->type;                                                         \
        ((RwObject*)(d))->subType = ((const RwObject*)(s))->subType;                                                   \
        ((RwObject*)(d))->flags = ((const RwObject*)(s))->flags;                                                       \
        ((RwObject*)(d))->privateFlags = ((const RwObject*)(s))->privateFlags;                                         \
        ((RwObject*)(d))->parent = NULL;                                                                               \
    \
}                                                                                                               \
    \
MACRO_STOP

#define rwObjectInitialize(o, t, s)                                                                                    \
    \
MACRO_START                                                                                                     \
    \
{                                                                                                               \
        ((RwObject*)(o))->type = (RwUInt8)(t);                                                                         \
        ((RwObject*)(o))->subType = (RwUInt8)(s);                                                                      \
        ((RwObject*)(o))->flags = 0;                                                                                   \
        ((RwObject*)(o))->privateFlags = 0;                                                                            \
        ((RwObject*)(o))->parent = NULL;                                                                               \
    \
}                                                                                                               \
    \
MACRO_STOP

/* Debug */
#define RwObjectGetType(o) (((const RwObject*)(o))->type)

#define rwObjectSetType(o, t) (((RwObject*)(o))->type) = (RwUInt8)(t)

/* Sub type */
#define rwObjectGetSubType(o) (((const RwObject*)(o))->subType)
#define rwObjectSetSubType(o, t) (((RwObject*)(o))->subType) = (RwUInt8)(t)

/* Flags */
#define rwObjectGetFlags(o) (((const RwObject*)(o))->flags)
#define rwObjectSetFlags(o, f) (((RwObject*)(o))->flags) = (RwUInt8)(f)
#define rwObjectTestFlags(o, f) ((((const RwObject*)(o))->flags) & (RwUInt8)(f))

/* Private flags */
#define rwObjectGetPrivateFlags(c) (((const RwObject*)(c))->privateFlags)
#define rwObjectSetPrivateFlags(c, f) (((RwObject*)(c))->privateFlags) = (RwUInt8)(f)
#define rwObjectTestPrivateFlags(c, flag) ((((const RwObject*)(c))->privateFlags) & (RwUInt8)(flag))

/* Hierarchy */
#define rwObjectGetParent(object) (((const RwObject*)(object))->parent)
#define rwObjectSetParent(c, p) (((RwObject*)(c))->parent) = (void*)(p)

#if (!defined(rwFREELISTNOMANSLANDFILL))
#define rwFREELISTNOMANSLANDFILL 0xFD
#endif /* (!defined(rwFREELISTNOMANSLANDFILL)) */

#if (!defined(rwFREELISTDEADLANDFILL))
#define rwFREELISTDEADLANDFILL 0xDD
#endif /* (!defined(rwFREELISTDEADLANDFILL)) */

#if (!defined(rwFREELISTCLEANLANDFILL))
#define rwFREELISTCLEANLANDFILL 0xCD
#endif /* (!defined(rwFREELISTCLEANLANDFILL)) */

#define RWFREELISTALIGNED(_pData, _freelist) (!(((RwUInt32)(_pData)) & ((_freelist)->alignment - 1)))

#if (!defined(rwDEADPTRFILL))
#define rwDEADPTRFILL (NULL)
#endif /* (!defined(rwDEADPTRFILL)) */

#if (!defined(RwMalloc))
#define RwMalloc(_s, _h) ((RWSRCGLOBAL(memoryFuncs).rwmalloc)((_s), (_h)))
#endif /* (!defined(RwMalloc)) */

#if (!defined(RwFree))
#define RwFree(_p) ((RWSRCGLOBAL(memoryFuncs).rwfree)((_p)))
#endif /* (!defined(RwFree)) */

#if (!defined(RwCalloc))
#define RwCalloc(_n, _s, _h) ((RWSRCGLOBAL(memoryFuncs).rwcalloc)((_n), (_s), (_h)))
#endif /* (!defined(RwCalloc)) */

#if (!defined(RwRealloc))
#define RwRealloc(_p, _s, _h) ((RWSRCGLOBAL(memoryFuncs).rwrealloc)((_p), (_s), (_h)))
#endif /* (!defined(RwRealloc)) */

#if (!defined(RWCRTORDBGFLAG))
#define RWCRTORDBGFLAG(_flag) /* No op */
#endif                        /* (!defined(RWCRTORDBGFLAG)) */

#if (!defined(RWCRTCHECKMEMORY))
#define RWCRTCHECKMEMORY() /* No Op */
#endif                     /* (!defined(RWCRTCHECKMEMORY)) */

#if (!defined(RWCRTDBGBREAK))
#define RWCRTDBGBREAK() /* No Op */
#endif                  /* (!defined(RWCRTDBGBREAK)) */

#if (!defined(RWCRTDOFORALLCLIENTOBJECTS))
#define RWCRTDOFORALLCLIENTOBJECTS(_f, _c) /* No Op */
#endif                                     /* (!defined(RWCRTDOFORALLCLIENTOBJECTS)) */

#if (!defined(RWCRTDUMPMEMORYLEAKS))
#define RWCRTDUMPMEMORYLEAKS() /* No Op */
#endif                         /* (!defined(RWCRTDUMPMEMORYLEAKS)) */

#if (!defined(RWCRTHEAPDIFFERENCESINCE))
#define RWCRTHEAPDIFFERENCESINCE(_Then) /* No Op */
#endif                                  /* (!defined(RWCRTHEAPDIFFERENCESINCE)) */

#if (!defined(RWCRTISMEMORYBLOCK))
#define RWCRTISMEMORYBLOCK(_p, _t, _r, _f, _l) (NULL != (_p))
#endif /* (!defined(RWCRTISMEMORYBLOCK)) */

#if (!defined(RWCRTISVALIDHEAPPOINTER))
#define RWCRTISVALIDHEAPPOINTER(_p) (NULL != (_p))
#endif /* (!defined(RWCRTISVALIDHEAPPOINTER)) */

#if (!defined(RWCRTISVALIDPOINTER))
#define RWCRTISVALIDPOINTER(_p, _n, _r) (NULL != (_p))
#endif /* (!defined(RWCRTISVALIDPOINTER)) */

#if (!defined(RWCRTMEMCHECKPOINT))
#define RWCRTMEMCHECKPOINT(_s) /* No Op */
#endif                         /* (!defined(RWCRTMEMCHECKPOINT)) */

#if (!defined(RWCRTMEMDIFFERENCE))
#define RWCRTMEMDIFFERENCE(_s1, _s2, _s3) /* No Op */
#endif                                    /* (!defined(RWCRTMEMDIFFERENCE)) */

#if (!defined(RWCRTMEMDUMPALLOBJECTSSINCE))
#define RWCRTMEMDUMPALLOBJECTSSINCE(_s) /* No Op */
#endif                                  /* (!defined(RWCRTMEMDUMPALLOBJECTSSINCE)) */

#if (!defined(RWCRTMEMDUMPSTATISTICS))
#define RWCRTMEMDUMPSTATISTICS(_s) (NULL)
#endif /* (!defined(RWCRTMEMDUMPSTATISTICS)) */

#if (!defined(RWCRTSETALLOCHOOK))
#define RWCRTSETALLOCHOOK(_f) (NULL)
#endif /* (!defined(RWCRTSETALLOCHOOK)) */

#if (!defined(RWCRTSETBREAKALLOC))
#define RWCRTSETBREAKALLOC(_a) (0)
#endif /* (!defined(RWCRTSETBREAKALLOC)) */

#if (!defined(RWCRTSETDBGFLAG))
#define RWCRTSETDBGFLAG(_f) (0)
#endif /* (!defined(RWCRTSETDBGFLAG)) */

#if (!defined(RWCRTSETDUMPCLIENT))
#define RWCRTSETDUMPCLIENT(_f) (NULL)
#endif /* (!defined(RWCRTSETDUMPCLIENT)) */

#if (!defined(RWCRTSETREPORTFILE))
#define RWCRTSETREPORTFILE(_t, _f) (NULL)
#endif /* (!defined(RWCRTSETREPORTFILE)) */

#if (!defined(RWCRTSETREPORTHOOK))
#define RWCRTSETREPORTHOOK(_f) (NULL)
#endif /* (!defined(RWCRTSETREPORTHOOK)) */

#if (!defined(RWCRTSETREPORTMODE))
#define RWCRTSETREPORTMODE(_t, _f) (0)
#endif /* (!defined(RWCRTSETREPORTMODE)) */

#if (!defined(RWREGSETBREAKALLOC))
#define RWREGSETBREAKALLOC(_name) /* No op */
#endif                            /* (!defined(RWREGSETBREAKALLOC)) */

#if (!defined(RWREGSETASSERTPRINT))
#define RWREGSETASSERTPRINT(_name) /* No op */
#endif                             /* (!defined(RWREGSETASSERTPRINT)) */

#if (!defined(RWGETWINREGDWORD))
#define RWGETWINREGDWORD(_env_var, _match) /* No op */
#endif                                     /* (!defined(RWGETWINREGDWORD)) */

#if (!defined(RWGETWINREGBINARY))
#define RWGETWINREGBINARY(_env_var, _match) /* No op */
#endif                                      /* (!defined(RWGETWINREGBINARY)) */

#if (!defined(RWGETWINREGSTRING))
#define RWGETWINREGSTRING(_env_var, _match) /* No op */
#endif                                      /* (!defined(RWGETWINREGSTRING)) */

#if (!defined(_CRTDBG_FLAGS))
#define _CRTDBG_FLAGS 0x33
#endif /* (!defined(_CRTDBG_FLAGS)) */

/**
* \ingroup memoryfileinterface
* Macro for extracting chunkID from a hint. This is the id of either
* the object for which a memory request came, or of the module a call
* for allocation belongs to. Refer to \ref RwCorePluginID for a list of
* all object IDs and to \ref rwengine, \ref RwCriterionCoreID
* and \ref RwCriterionWorldID for a list of all module IDs. Refer to
* RwMemoryFunctions for more details about the hints.
*/
#define RwMemoryHintGetChunkID(_h) ((_h)&0xFFFF)
/**
* \ingroup memoryfileinterface
* Macro for extracting duration from a hint. Refer to RwMemoryFunctions
* for more details about the hints.
* \see RwMemoryHintDuration
*/
#define RwMemoryHintGetDuration(_h) ((_h)&rwMEMHINTDUR_MASK)
/**
* \ingroup memoryfileinterface
* Macro for extracting flags from a hint. Refer to RwMemoryFunctions
* for more details about the hints.
* \see RwMemoryHintFlag
*/
#define RwMemoryHintGetFlags(_h) ((_h)&rwMEMHINTFLAG_MASK)

/**
* \ingroup rwfreelist
* The free list was statically allocated
*
* \see RwFreeListSetFlags
*/
#define rwFREELISTFLAG_STATIC 0x00000001

/**
* \ingroup rwfreelist
* \hideinitializer
* Free blocks as soon as they are empty.
*
* \see RwFreeListSetFlags
*/
#define rwFREELISTFLAG_FREEBLOCKS 0x00000002

#if (!defined(RwFreeListAlloc))
#define RwFreeListAlloc(_f, _h) RWSRCGLOBAL(memoryAlloc)((_f), (_h))
#endif /* (!defined(RwFreeListAlloc)) */

#if (!defined(RwFreeListFree))
#define RwFreeListFree(_f, _p) RWSRCGLOBAL(memoryFree)(_f, _p)
#endif /* (!defined(RwFreeListFree)) */

#define rwSTREAMSTACKSIZE 512

#define rwsprintf RWSRCGLOBAL(stringFuncs).vecSprintf
#define rwvsprintf RWSRCGLOBAL(stringFuncs).vecVsprintf
#define rwstrcpy RWSRCGLOBAL(stringFuncs).vecStrcpy
#define rwstrncpy RWSRCGLOBAL(stringFuncs).vecStrncpy
#define rwstrcat RWSRCGLOBAL(stringFuncs).vecStrcat
#define rwstrncat RWSRCGLOBAL(stringFuncs).vecStrncat
#define rwstrrchr RWSRCGLOBAL(stringFuncs).vecStrrchr
#define rwstrchr RWSRCGLOBAL(stringFuncs).vecStrchr
#define rwstrstr RWSRCGLOBAL(stringFuncs).vecStrstr
#define rwstrcmp RWSRCGLOBAL(stringFuncs).vecStrcmp
#define rwstrncmp RWSRCGLOBAL(stringFuncs).vecStrncmp
#define rwstricmp RWSRCGLOBAL(stringFuncs).vecStricmp
#define rwstrlen RWSRCGLOBAL(stringFuncs).vecStrlen
#define rwstrupr RWSRCGLOBAL(stringFuncs).vecStrupr
#define rwstrlwr RWSRCGLOBAL(stringFuncs).vecStrlwr
#define rwstrtok RWSRCGLOBAL(stringFuncs).vecStrtok
#define rwsscanf RWSRCGLOBAL(stringFuncs).vecSscanf

#define rwstrdup(_result, _string)                                                                                     \
    \
do                                                                                                              \
    \
{                                                                                                               \
        _result = ((RwChar*)NULL);                                                                                     \
                                                                                                                       \
        if (((RwChar*)NULL) != (_string))                                                                              \
        {                                                                                                              \
            _result = (RwChar*)RwMalloc((rwstrlen(_string) + 1) * sizeof(RwChar), rwID_STRING | rwMEMHINTDUR_EVENT);   \
                                                                                                                       \
            if (((RwChar*)NULL) != (_result))                                                                          \
            {                                                                                                          \
                rwstrcpy(_result, _string);                                                                            \
            }                                                                                                          \
        }                                                                                                              \
    \
}                                                                                                               \
    \
while(0)

#define rwPluginRegistryOpen() _rwPluginRegistryOpen()
#define rwPluginRegistryClose() _rwPluginRegistryClose()
#define rwPluginRegistrySetStaticPluginsSize(reg, size) _rwPluginRegistrySetStaticPluginsSize(reg, size)
#define rwPluginRegistryAddPlugin(reg, size, pluginID, constructCB, destructCB, copyCB)                                \
    _rwPluginRegistryAddPlugin(reg, size, pluginID, constructCB, destructCB, copyCB)
#define rwPluginRegistryGetPluginOffset(reg, pluginID) _rwPluginRegistryGetPluginOffset(reg, pluginID)
#define rwPluginRegistryInitObject(reg, object) _rwPluginRegistryInitObject(reg, object)
#define rwPluginRegistryDeInitObject(reg, object) _rwPluginRegistryDeInitObject(reg, object)
#define rwPluginRegistryCopyObject(reg, dstObject, srcObject) _rwPluginRegistryCopyObject(reg, dstObject, srcObject)
#define rwPluginRegistryValidateObject(reg, object) _rwPluginRegistryValidateObject(reg, object)

#define rwPluginRegistryAddPluginStream(reg, pluginID, readCB, writeCB, getSizeCB)                                     \
    _rwPluginRegistryAddPluginStream(reg, pluginID, readCB, writeCB, getSizeCB)
#define rwPluginRegistryAddPluginStreamAlwaysCB(reg, pluginID, alwaysCB)                                               \
    _rwPluginRegistryAddPlgnStrmlwysCB(reg, pluginID, alwaysCB)
#define rwPluginRegistryReadDataChunks(reg, stream, object) _rwPluginRegistryReadDataChunks(reg, stream, object)
#define rwPluginRegistryWriteDataChunks(reg, stream, object) _rwPluginRegistryWriteDataChunks(reg, stream, object)
#define rwPluginRegistrySkipDataChunks(reg, stream) _rwPluginRegistrySkipDataChunks(reg, stream)
#define rwPluginRegistryGetSize(reg, object) _rwPluginRegistryGetSize(reg, object)

#if (!(defined(RWMATRIXPRINT)))
#define RWMATRIXPRINT(_matrix) // No op
#endif

#if (!defined(RwMatrixCopyMacro))
#define RwMatrixCopyMacro(_target, _source) (*(_target) = *(_source))
#endif /* (!defined(RwMatrixCopyMacro)) */

#if (!defined(RwMatrixSetIdentityMacro))
#define RwMatrixSetIdentityMacro(m)                                                                                    \
    \
MACRO_START                                                                                                     \
    \
{                                                                                                               \
        (m)->right.x = (m)->up.y = (m)->at.z = (RwReal)((1.0));                                                        \
        (m)->right.y = (m)->right.z = (m)->up.x = (RwReal)((0.0));                                                     \
        (m)->up.z = (m)->at.x = (m)->at.y = (RwReal)((0.0));                                                           \
        (m)->pos.x = (m)->pos.y = (m)->pos.z = (RwReal)((0.0));                                                        \
        rwMatrixSetFlags((m), rwMatrixGetFlags(m) | (rwMATRIXINTERNALIDENTITY | rwMATRIXTYPEORTHONORMAL));             \
    \
}                                                                                                               \
    \
MACRO_STOP
#endif /* (!defined(RwMatrixSetIdentityMacro)) */

#define rwMatrixSetFlags(m, flagsbit) ((m)->flags = (flagsbit))
#define rwMatrixGetFlags(m) ((m)->flags)
#define rwMatrixTestFlags(m, flagsbit) ((m)->flags & (RwInt32)(flagsbit))

#define RwMatrixCopy(dst, src) RwMatrixCopyMacro(dst, src)
#define RwMatrixSetIdentity(m) RwMatrixSetIdentityMacro(m)

#define RwMatrixGetRight(m) (&(m)->right)
#define RwMatrixGetUp(m) (&(m)->up)
#define RwMatrixGetAt(m) (&(m)->at)
#define RwMatrixGetPos(m) (&(m)->pos)

#define rwMatrixSetOptimizations(optimizeFlags) _rwMatrixSetOptimizations(optimizeFlags)

#define rwMatrixSetMultFn(multMat) _rwMatrixSetMultFn(multMat)

#define rwMatrixOpen(instance, offset, size) _rwMatrixOpen(instance, offset, size)

#define rwMatrixClose(instance, offset, size) _rwMatrixClose(instance, offset, size)

/* Initialisation/deinitialisation */
#define rwMatrixInitialize(m, t)                                                                                       \
    \
MACRO_START                                                                                                     \
    \
{                                                                                                               \
        rwMatrixSetFlags((m), (t));                                                                                    \
    \
}                                                                                                               \
    \
MACRO_STOP

#define rwMatrixIsNormal(_matrix, _epsilon) ((_epsilon) >= _rwMatrixNormalError(_matrix))

#define rwMatrixIsOrthogonal(_matrix, _epsilon) ((_epsilon) >= _rwMatrixOrthogonalError(_matrix))

#define rwMatrixIsOrthonormal(_matrix, _epsilon)                                                                       \
    (rwMatrixIsNormal(_matrix, _epsilon) && rwMatrixIsOrthogonal(_matrix, _epsilon))

#define rwMatrixIsOrthonormalPositive(_matrix, _epsilon)                                                               \
    (rwMatrixIsOrthonormal(_matrix, _epsilon) && \
((((RwReal)1) - (_epsilon)) <= _rwMatrixDeterminant(_matrix)))

#define rwMatrixIsIdentity(_matrix, _epsilon) ((_epsilon) >= _rwMatrixIdentityError(_matrix))

#define rwMatrixValidFlags(_matrix, _epsilon)                                                                          \
    ((_matrix) &&                          /* valid pointer */                                                         \
        ((!(rwMatrixGetFlags(_matrix) &    /* not flagged as identity */                                               \
             rwMATRIXINTERNALIDENTITY)) || /* .. or actually is */                                                     \
            rwMatrixIsIdentity(_matrix, _epsilon)) &&                                                                  \
        ((!(rwMatrixGetFlags(_matrix) & /* not flagged as normal */                                                    \
             rwMATRIXTYPENORMAL)) ||    /* ... or actually is */                                                       \
            rwMatrixIsNormal(_matrix, _epsilon)) &&                                                                    \
        ((!(rwMatrixGetFlags(_matrix) &  /* not flagged as orthogonal */                                               \
             rwMATRIXTYPEORTHOGONAL)) || /* ... or actually is */                                                      \
            rwMatrixIsOrthogonal(_matrix, _epsilon)))

#define rwMat01Det(_mAA) ((_mAA))

#define rwMat02Det(_mAA, _mAB, _mBA, _mBB) ((_mAA)*rwMat01Det(_mBB) - (_mAB)*rwMat01Det(_mBA))

#define rwMat03Det(_mAA, _mAB, _mAC, _mBA, _mBB, _mBC, _mCA, _mCB, _mCC)                                               \
    ((_mAA)*rwMat02Det(_mBB, _mBC, _mCB, _mCC) - (_mAB)*rwMat02Det(_mBA, _mBC, _mCA, _mCC) +                           \
        (_mAC)*rwMat02Det(_mBA, _mBB, _mCA, _mCB))

#define rwMat04Det(_mAA, _mAB, _mAC, _mAD, _mBA, _mBB, _mBC, _mBD, _mCA, _mCB, _mCC, _mCD, _mDA, _mDB, _mDC, _mDD)     \
    ((_mAA)*rwMat03Det(_mBB, _mBC, _mBD, _mCB, _mCC, _mCD, _mDB, _mDC, _mDD) -                                         \
        (_mAB)*rwMat03Det(_mBA, _mBC, _mBD, _mCA, _mCC, _mCD, _mDA, _mDC, _mDD) +                                      \
        (_mAC)*rwMat03Det(_mBA, _mBB, _mBD, _mCA, _mCB, _mCD, _mDA, _mDB, _mDD) -                                      \
        (_mAD)*rwMat03Det(_mBA, _mBB, _mBC, _mCA, _mCB, _mCC, _mDA, _mDB, _mDC))

#define rwMat02Inv(_rAA, _rAB, _rBA, _rBB)                                                                             \
                   _mAA, _mAB,                                          \
                   _mBA, _mBB)                                          \
MACRO_START                                 \
    \
{                                                                                                               \
        RwSplitBits determinant;                                                                                       \
                                                                                                                       \
        (_rAA) = rwMat01Det(_mBB);                                                                                     \
        (_rAB) = -rwMat01Det(_mAB);                                                                                    \
                                                                                                                       \
        determinant.nReal = ((_rAA) * (_mAA) + (_rAB) * (_mBA));                                                       \
                                                                                                                       \
        {                                                                                                              \
            const RwReal normalize = ((determinant.nInt != 0) ? (((RwReal)1) / determinant.nReal) : ((RwReal)1));      \
                                                                                                                       \
            (_rAA) *= normalize;                                                                                       \
            (_rAB) *= normalize;                                                                                       \
                                                                                                                       \
            (_rBA) = -rwMat01Det(_mBA) * normalize;                                                                    \
            (_rBB) = rwMat01Det(_mAA) * normalize;                                                                     \
        }                                                                                                              \
    \
}                                                                                                               \
    \
MACRO_STOP

#define rwMat03Inv(                                                                                                    \
    _rAA, _rAB, _rAC, _rBA, _rBB, _rBC, _rCA, _rCB, _rCC, _mAA, _mAB, _mAC, _mBA, _mBB, _mBC, _mCA, _mCB, _mCC)        \
    \
MACRO_START                                                                                                     \
    \
{                                                                                                               \
        RwSplitBits determinant;                                                                                       \
                                                                                                                       \
        (_rAA) = rwMat02Det(_mBB, _mBC, _mCB, _mCC);                                                                   \
        (_rAB) = -rwMat02Det(_mAB, _mAC, _mCB, _mCC);                                                                  \
        (_rAC) = rwMat02Det(_mAB, _mAC, _mBB, _mBC);                                                                   \
                                                                                                                       \
        determinant.nReal = ((_rAA) * (_mAA) + (_rAB) * (_mBA) + (_rAC) * (_mCA));                                     \
                                                                                                                       \
        {                                                                                                              \
            const RwReal normalize = ((determinant.nInt != 0) ? (((RwReal)1) / determinant.nReal) : ((RwReal)1));      \
            (_rAA) *= normalize;                                                                                       \
            (_rAB) *= normalize;                                                                                       \
            (_rAC) *= normalize;                                                                                       \
                                                                                                                       \
            (_rBA) = -rwMat02Det(_mBA, _mBC, _mCA, _mCC) * normalize;                                                  \
            (_rBB) = rwMat02Det(_mAA, _mAC, _mCA, _mCC) * normalize;                                                   \
            (_rBC) = -rwMat02Det(_mAA, _mAC, _mBA, _mBC) * normalize;                                                  \
                                                                                                                       \
            (_rCA) = rwMat02Det(_mBA, _mBB, _mCA, _mCB) * normalize;                                                   \
            (_rCB) = -rwMat02Det(_mAA, _mAB, _mCA, _mCB) * normalize;                                                  \
            (_rCC) = rwMat02Det(_mAA, _mAB, _mBA, _mBB) * normalize;                                                   \
        }                                                                                                              \
    \
}                                                                                                               \
    \
MACRO_STOP

#define rwMat04Inv(_rAA, _rAB, _rAC, _rAD, _rBA, _rBB, _rBC, _rBD, _rCA, _rCB, _rCC, _rCD, _rDA, _rDB, _rDC, _rDD,     \
                   _mAA, _mAB, _mAC, _mAD, _mBA, _mBB, _mBC, _mBD, _mCA, _mCB, _mCC, _mCD, _mDA, _mDB, _mDC, _mDD)     \
    \
MACRO_START                                                                                                     \
    \
{                                                                                                               \
        RwSplitBits determinant;                                                                                       \
                                                                                                                       \
        (_rAA) = rwMat03Det(_mBB, _mBC, _mBD, _mCB, _mCC, _mCD, _mDB, _mDC, _mDD);                                     \
        (_rAB) = -rwMat03Det(_mAB, _mAC, _mAD, _mCB, _mCC, _mCD, _mDB, _mDC, _mDD);                                    \
        (_rAC) = rwMat03Det(_mAB, _mAC, _mAD, _mBB, _mBC, _mBD, _mDB, _mDC, _mDD);                                     \
        (_rAD) = -rwMat03Det(_mAB, _mAC, _mAD, _mBB, _mBC, _mBD, _mCB, _mCC, _mCD);                                    \
                                                                                                                       \
        determinant.nReal = ((_rAA) * (_mAA) + (_rAB) * (_mBA) + (_rAC) * (_mCA) + (_rAD) * (_mDA));                   \
                                                                                                                       \
        {                                                                                                              \
            const RwReal normalize = ((determinant.nInt != 0) ? (((RwReal)1) / determinant.nReal) : ((RwReal)1));      \
                                                                                                                       \
            (_rAA) *= normalize;                                                                                       \
            (_rAB) *= normalize;                                                                                       \
            (_rAC) *= normalize;                                                                                       \
            (_rAD) *= normalize;                                                                                       \
                                                                                                                       \
            (_rBA) = -rwMat03Det(_mBA, _mBC, _mBD, _mCA, _mCC, _mCD, _mDA, _mDC, _mDD) * normalize;                    \
            (_rBB) = rwMat03Det(_mAA, _mAC, _mAD, _mCA, _mCC, _mCD, _mDA, _mDC, _mDD) * normalize;                     \
            (_rBC) = -rwMat03Det(_mAA, _mAC, _mAD, _mBA, _mBC, _mBD, _mDA, _mDC, _mDD) * normalize;                    \
            (_rBD) = rwMat03Det(_mAA, _mAC, _mAD, _mBA, _mBC, _mBD, _mCA, _mCC, _mCD) * normalize;                     \
                                                                                                                       \
            (_rCA) = rwMat03Det(_mBA, _mBB, _mBD, _mCA, _mCB, _mCD, _mDA, _mDB, _mDD) * normalize;                     \
            (_rCB) = -rwMat03Det(_mAA, _mAB, _mAD, _mCA, _mCB, _mCD, _mDA, _mDB, _mDD) * normalize;                    \
            (_rCC) = rwMat03Det(_mAA, _mAB, _mAD, _mBA, _mBB, _mBD, _mDA, _mDB, _mDD) * normalize;                     \
            (_rCD) = -rwMat03Det(_mAA, _mAB, _mAD, _mBA, _mBB, _mBD, _mCA, _mCB, _mCD) * normalize;                    \
                                                                                                                       \
            (_rDA) = -rwMat03Det(_mBA, _mBB, _mBC, _mCA, _mCB, _mCC, _mDA, _mDB, _mDC) * normalize;                    \
            (_rDB) = rwMat03Det(_mAA, _mAB, _mAC, _mCA, _mCB, _mCC, _mDA, _mDB, _mDC) * normalize;                     \
            (_rDC) = -rwMat03Det(_mAA, _mAB, _mAC, _mBA, _mBB, _mBC, _mDA, _mDB, _mDC) * normalize;                    \
            (_rDD) = rwMat03Det(_mAA, _mAB, _mAC, _mBA, _mBB, _mBC, _mCA, _mCB, _mCC) * normalize;                     \
        }                                                                                                              \
    \
}                                                                                                               \
    \
MACRO_STOP

#if (!defined(ERR_WRAP))
#define ERR_WRAP(A) (A)
#endif /* (!defined(ERR_WRAP)) */

#define RwIm2DVertexSetCameraX(vert, camx) /* Nothing */
#define RwIm2DVertexSetCameraY(vert, camy) /* Nothing */
#define RwIm2DVertexSetCameraZ(vert, camz) /* Nothing */

#define RwIm2DVertexSetRecipCameraZ(vert, recipz) ((vert)->rhw = recipz)

#define RwIm2DVertexGetCameraX(vert) (cause an error)
#define RwIm2DVertexGetCameraY(vert) (cause an error)
#define RwIm2DVertexGetCameraZ(vert) (cause an error)
#define RwIm2DVertexGetRecipCameraZ(vert) ((vert)->rhw)

/* Set screen space coordinates in a device vertex */
#define RwIm2DVertexSetScreenX(vert, scrnx) ((vert)->x = (scrnx))
#define RwIm2DVertexSetScreenY(vert, scrny) ((vert)->y = (scrny))
#define RwIm2DVertexSetScreenZ(vert, scrnz) ((vert)->z = (scrnz))
#define RwIm2DVertexGetScreenX(vert) ((vert)->x)
#define RwIm2DVertexGetScreenY(vert) ((vert)->y)
#define RwIm2DVertexGetScreenZ(vert) ((vert)->z)

/* Set texture coordinates in a device vertex */
#define RwIm2DVertexSetU(vert, texU, recipz) ((vert)->u = (texU))
#define RwIm2DVertexSetV(vert, texV, recipz) ((vert)->v = (texV))
#define RwIm2DVertexGetU(vert) ((vert)->u)
#define RwIm2DVertexGetV(vert) ((vert)->v)

/* Modify the luminance stuff */
#define RwIm2DVertexSetRealRGBA(vert, red, green, blue, alpha)                                                         \
    ((vert)->emissiveColor = (((RwFastRealToUInt32(alpha)) << 24) | ((RwFastRealToUInt32(red)) << 16) |                \
                              ((RwFastRealToUInt32(green)) << 8) | ((RwFastRealToUInt32(blue)))))

#define RwIm2DVertexSetIntRGBA(vert, red, green, blue, alpha)                                                          \
    ((vert)->emissiveColor = ((((RwUInt32)(alpha)) << 24) | (((RwUInt32)(red)) << 16) | (((RwUInt32)(green)) << 8) |   \
                              (((RwUInt32)(blue)))))

#define RwIm2DVertexGetRed(vert) (((vert)->emissiveColor >> 16) & 0xFF)

#define RwIm2DVertexGetGreen(vert) (((vert)->emissiveColor >> 8) & 0xFF)

#define RwIm2DVertexGetBlue(vert) ((vert)->emissiveColor & 0xFF)

#define RwIm2DVertexGetAlpha(vert) (((vert)->emissiveColor >> 24) & 0xFF)

#define RwIm2DVertexCopyRGBA(dst, src) ((dst)->emissiveColor = (src)->emissiveColor)


#define RwIm2DVertexClipRGBA(o, i, n, f)                                                                               \
    \
MACRO_START                                                                                                     \
    \
{                                                                                                               \
        const RwInt32 _factor = (RwFastRealToUInt32(i * (RwReal)(255))) & 255;                                         \
                                                                                                                       \
        (o)->emissiveColor =                                                                                           \
            (((((RwIm2DVertexGetAlpha(f) - RwIm2DVertexGetAlpha(n)) * _factor) >> 8) + RwIm2DVertexGetAlpha(n))        \
                << 24) |                                                                                               \
            (((((RwIm2DVertexGetRed(f) - RwIm2DVertexGetRed(n)) * _factor) >> 8) + RwIm2DVertexGetRed(n)) << 16) |     \
            (((((RwIm2DVertexGetGreen(f) - RwIm2DVertexGetGreen(n)) * _factor) >> 8) + RwIm2DVertexGetGreen(n))        \
                << 8) |                                                                                                \
            (((((RwIm2DVertexGetBlue(f) - RwIm2DVertexGetBlue(n)) * _factor) >> 8) + RwIm2DVertexGetBlue(n)));         \
    \
}                                                                                                               \
    \
MACRO_STOP







#define rwCHUNKGROUPMAXNAME 32

#define rwCHUNKGROUPVERSION 0x01


#define SUBSYSTEMNAME_MAXLEN 80

#define rwSTANDARDNUMOFSTANDARD 29


#define RWPALQUANT_MAXDEPTH 8
#define RWPALQUANT_MAXCOLOR (1 << RWPALQUANT_MAXDEPTH)

#define rwTEXTUREBASENAMELENGTH 32

// rpuvanim.h

#define RP_UVANIM_MAXSLOTS (8)
#define RP_UVANIM_APPLIEDCHANNELS (2)

#define RP_UVANIM_FIRST_PASS_CHANNEL (0)
#define RP_UVANIM_DUAL_PASS_CHANNEL (1)
#define RP_UVANIM_MAXNAME (32)
