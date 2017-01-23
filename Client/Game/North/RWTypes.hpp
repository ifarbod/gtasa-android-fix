// RenderWare types and structures
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "RWEnums.hpp"
#include "RWMacros.hpp"

#if !(defined(_VA_LIST_DEFINED))
using va_list = char*;
#endif

using RwFixed = long;
using RwInt32 = int;
using RwUInt32 = unsigned int;
using RwInt16 = short;
using RwUInt16 = unsigned short;
using RwUInt8 = unsigned char;
using RwInt8 = signed char;

using RwChar = char;
using RwReal = float;
using RwBool = RwInt32;

using RwInt64 = long long;
using RwUInt64 = unsigned long long;

union RwSplitBits
{
    RwReal nReal;
    volatile RwInt32 nInt;
    volatile RwUInt32 nUInt;
};

struct RwSplitFixed
{
    RwUInt16 fractional;
    RwInt16 integral;
};

union RwUnionReal
{
    RwReal real;
    float floating;
    RwFixed fixed;
    RwSplitFixed splitfixed;
};

struct RwV2d
{
    RwReal x;
    RwReal y;
};

struct RwV3d
{
    RwReal x;
    RwReal y;
    RwReal z;
};

struct RwV4dTag
{
    RwReal x;
    RwReal y;
    RwReal z;
    RwReal w;
};
using RwV4d = RwV4dTag;

struct RwRect
{
    RwInt32 x;
    RwInt32 y;
    RwInt32 w;
    RwInt32 h;
};

struct RwSphere
{
    RwV3d center;
    RwReal radius;
};

struct RwLine
{
    RwV3d start;
    RwV3d end;
};

struct RwTexCoords
{
    RwReal u;
    RwReal v;
};

struct RwSLLink
{
    RwSLLink* next;
};

struct RwSingleList
{
    RwSLLink link;
};

struct RwLLLink
{
    RwLLLink* next;
    RwLLLink* prev;
};

struct RwLinkList
{
    RwLLLink link;
};

struct RwSurfaceProperties
{
    RwReal ambient;
    RwReal specular;
    RwReal diffuse;
};

struct RwPlane
{
    RwV3d normal;
    RwReal distance;
};

struct RwObject
{
    RwUInt8 type;
    RwUInt8 subType;
    RwUInt8 flags;
    RwUInt8 privateFlags;
    void* parent;
};
using RwObjectCallBack = RwObject* (*)(RwObject* object, void* data);

struct RwMemoryFunctions
{
    void* (*rwmalloc)(size_t size, RwUInt32 hint);
    void (*rwfree)(void* mem);
    void* (*rwrealloc)(void* mem, size_t newSize, RwUInt32 hint);
    void* (*rwcalloc)(size_t numObj, size_t sizeObj, RwUInt32 hint);
};

struct RwFreeList
{
    RwUInt32 entrySize;
    RwUInt32 entriesPerBlock;
    RwUInt32 heapSize;
    RwUInt32 alignment;
    RwLinkList blockList;
    RwUInt32 flags;
    RwLLLink link;
};
using RwFreeListCallBack = void (*)(void* pMem, void* pData);
using RwMemoryAllocFn = void* (*)(RwFreeList* fl, RwUInt32 hint);
using RwMemoryFreeFn = RwFreeList* (*)(RwFreeList* fl, void* pData);

struct RwStreamMemory
{
    RwUInt32 position;
    RwUInt32 nSize;
    RwUInt8* memBlock;
};

union RwStreamFile
{
    void* fpFile;
    const void* constfpFile;
};
using rwCustomStreamFnClose = RwBool (*)(void* data);
using rwCustomStreamFnRead = RwUInt32 (*)(void* data, void* buffer, RwUInt32 length);
using rwCustomStreamFnWrite = RwBool (*)(void* data, const void* buffer, RwUInt32 length);
using rwCustomStreamFnSkip = RwBool (*)(void* data, RwUInt32 offset);

struct RwStreamCustom
{
    rwCustomStreamFnClose sfnclose;
    rwCustomStreamFnRead sfnread;
    rwCustomStreamFnWrite sfnwrite;
    rwCustomStreamFnSkip sfnskip;
    void* data;
};

union RwStreamUnion
{
    RwStreamMemory memory;
    RwStreamFile file;
    RwStreamCustom custom;
};

struct RwStream
{
    RwStreamType type;
    RwStreamAccessType accessType;
    RwInt32 position;
    RwStreamUnion Type;
    RwBool rwOwned;
};

struct RwMemory
{
    RwUInt8* start;
    RwUInt32 length;
};

using vecSprintfFunc = int (*)(RwChar* buffer, const RwChar* format, ...);
using vecVsprintfFunc = int (*)(RwChar* buffer, const RwChar* format, va_list argptr);
using vecStrcpyFunc = RwChar* (*)(RwChar* dest, const RwChar* srce);
using vecStrncpyFunc = RwChar* (*)(RwChar* dest, const RwChar* srce, size_t size);
using vecStrcatFunc = RwChar* (*)(RwChar* dest, const RwChar* srce);
using vecStrncatFunc = RwChar* (*)(RwChar* dest, const RwChar* srce, size_t size);
using vecStrrchrFunc = RwChar* (*)(const RwChar* string, int findThis);
using vecStrchrFunc = RwChar* (*)(const RwChar* string, int findThis);
using vecStrstrFunc = RwChar* (*)(const RwChar* string, const RwChar* findThis);
using vecStrcmpFunc = int (*)(const RwChar* string1, const RwChar* string2);
using vecStrncmpFunc = int (*)(const RwChar* string1, const RwChar* string2, size_t max_size);
using vecStricmpFunc = int (*)(const RwChar* string1, const RwChar* string2);
using vecStrlenFunc = size_t (*)(const RwChar* string);
using vecStruprFunc = RwChar* (*)(RwChar* string);
using vecStrlwrFunc = RwChar* (*)(RwChar* string);
using vecStrtokFunc = RwChar* (*)(RwChar* string, const RwChar* delimit);
using vecSscanfFunc = int (*)(const RwChar* buffer, const RwChar* format, ...);

struct RwStringFunctions
{
    vecSprintfFunc vecSprintf;
    vecVsprintfFunc vecVsprintf;
    vecStrcpyFunc vecStrcpy;
    vecStrncpyFunc vecStrncpy;
    vecStrcatFunc vecStrcat;
    vecStrncatFunc vecStrncat;
    vecStrrchrFunc vecStrrchr;
    vecStrchrFunc vecStrchr;
    vecStrstrFunc vecStrstr;
    vecStrcmpFunc vecStrcmp;
    vecStrncmpFunc vecStrncmp;
    vecStricmpFunc vecStricmp;
    vecStrlenFunc vecStrlen;
    vecStruprFunc vecStrupr;
    vecStrlwrFunc vecStrlwr;
    vecStrtokFunc vecStrtok;
    vecSscanfFunc vecSscanf;
};

struct RwChunkGroup
{
    RwChar name[rwCHUNKGROUPMAXNAME + 1];
};

using RwPluginDataChunkWriteCallBack = RwStream* (*)(RwStream* stream, RwInt32 binaryLength, const void* object,
    RwInt32 offsetInObject, RwInt32 sizeInObject);
using RwPluginDataChunkReadCallBack = RwStream* (*)(RwStream* stream, RwInt32 binaryLength, void* object,
    RwInt32 offsetInObject, RwInt32 sizeInObject);
using RwPluginDataChunkGetSizeCallBack = RwInt32 (*)(const void* object, RwInt32 offsetInObject, RwInt32 sizeInObject);
using RwPluginDataChunkAlwaysCallBack = RwBool (*)(void* object, RwInt32 offsetInObject, RwInt32 sizeInObject);
using RwPluginDataChunkRightsCallBack = RwBool (*)(
    void* object, RwInt32 offsetInObject, RwInt32 sizeInObject, RwUInt32 extraData);
using RwPluginObjectConstructor = void* (*)(void* object, RwInt32 offsetInObject, RwInt32 sizeInObject);
using RwPluginObjectCopy = void* (*)(void* dstObject, const void* srcObject, RwInt32 offsetInObject,
    RwInt32 sizeInObject);
using RwPluginObjectDestructor = void* (*)(void* object, RwInt32 offsetInObject, RwInt32 sizeInObject);
using RwPluginErrorStrCallBack = void* (*)(void*);

struct RwPluginRegistry
{
    RwInt32 sizeOfStruct;
    RwInt32 origSizeOfStruct;
    RwInt32 maxSizeOfStruct;
    RwInt32 staticAlloc;
    RwPluginRegEntry* firstRegEntry;
    RwPluginRegEntry* lastRegEntry;
};

struct RwPluginRegEntry
{
    RwInt32 offset;
    RwInt32 size;
    RwUInt32 pluginID;
    RwPluginDataChunkReadCallBack readCB;
    RwPluginDataChunkWriteCallBack writeCB;
    RwPluginDataChunkGetSizeCallBack getSizeCB;
    RwPluginDataChunkAlwaysCallBack alwaysCB;
    RwPluginDataChunkRightsCallBack rightsCB;
    RwPluginObjectConstructor constructCB;
    RwPluginObjectDestructor destructCB;
    RwPluginObjectCopy copyCB;
    RwPluginErrorStrCallBack errStrCB;
    RwPluginRegEntry* nextRegEntry;
    RwPluginRegEntry* prevRegEntry;
    RwPluginRegistry* parentRegistry;
};

struct RwMatrixTag
{
    RwV3d right;
    RwUInt32 flags;
    RwV3d up;
    RwUInt32 pad1;
    RwV3d at;
    RwUInt32 pad2;
    RwV3d pos;
    RwUInt32 pad3;
};
using RwMatrix = RwMatrixTag;

using rwMatrixMultFn = void (*)(RwMatrix* dstMat, const RwMatrix* matA, const RwMatrix* matB);
struct RwMatrixTolerance
{
    RwReal Normal;
    RwReal Orthogonal;
    RwReal Identity;
};

struct RwD3D9Vertex
{
    RwReal x;
    RwReal y;
    RwReal z;
    RwReal rhw;
    RwUInt32 emissiveColor;
    RwReal u;
    RwReal v;
};

using RwIm2DVertex = RwD3D9Vertex;
using RxVertexIndex = RwUInt16;
using RwImVertexIndex = RxVertexIndex;

struct RwD3D9Metrics
{
    RwUInt32 numRenderStateChanges;
    RwUInt32 numTextureStageStateChanges;
    RwUInt32 numSamplerStageStateChanges;
    RwUInt32 numMaterialChanges;
    RwUInt32 numLightsChanged;
    RwUInt32 numVBSwitches;
};

using rwVectorMultFn = RwV3d* (*)(RwV3d* pointOut, const RwV3d* pointIn, const RwMatrix* matrix);
using rwVectorsMultFn = RwV3d* (*)(RwV3d* pointsOut, const RwV3d* pointsIn, RwInt32 numPoints, const RwMatrix* matrix);

struct RwSList
{
    RwUInt8* listElements;
    RwInt32 numElementsFilled;
    RwInt32 numElementsAlloced;
    RwInt32 entrySize;
};

using RwStandardFunc = RwBool (*)(void* pOut, void* pInOut, RwInt32 nI);

struct RwEngineOpenParams
{
    void* displayID;
};

using RwSystemFunc = RwBool (*)(RwInt32 nOption, void* pOut, void* pInOut, RwInt32 nIn);

using RwRenderStateSetFunction = RwBool (*)(RwRenderState nState, void* pParam);
using RwRenderStateGetFunction = RwBool (*)(RwRenderState nState, void* pParam);
using RwIm2DRenderLineFunction = RwBool (*)(RwIm2DVertex* vertices, RwInt32 numVertices, RwInt32 vert1, RwInt32 vert2);
using RwIm2DRenderTriangleFunction = RwBool (*)(
    RwIm2DVertex* vertices, RwInt32 numVertices, RwInt32 vert1, RwInt32 vert2, RwInt32 vert3);
using RwIm2DRenderPrimitiveFunction = RwBool (*)(RwPrimitiveType primType, RwIm2DVertex* vertices, RwInt32 numVertices);
using RwIm2DRenderIndexedPrimitiveFunction = RwBool (*)(RwPrimitiveType primType, RwIm2DVertex* vertices,
    RwInt32 numVertices, RwImVertexIndex* indices, RwInt32 numIndices);
using RwIm3DRenderLineFunction = RwBool (*)(RwInt32 vert1, RwInt32 vert2);
using RwIm3DRenderTriangleFunction = RwBool (*)(RwInt32 vert1, RwInt32 vert2, RwInt32 vert3);
using RwIm3DRenderPrimitiveFunction = RwBool (*)(RwPrimitiveType primType);
using RwIm3DRenderIndexedPrimitiveFunction = RwBool (*)(
    RwPrimitiveType primtype, RwImVertexIndex* indices, RwInt32 numIndices);

struct RwDevice
{
    RwReal gammaCorrection;
    RwSystemFunc fpSystem;
    RwReal zBufferNear;
    RwReal zBufferFar;
    RwRenderStateSetFunction fpRenderStateSet;
    RwRenderStateGetFunction fpRenderStateGet;
    RwIm2DRenderLineFunction fpIm2DRenderLine;
    RwIm2DRenderTriangleFunction fpIm2DRenderTriangle;
    RwIm2DRenderPrimitiveFunction fpIm2DRenderPrimitive;
    RwIm2DRenderIndexedPrimitiveFunction fpIm2DRenderIndexedPrimitive;
    RwIm3DRenderLineFunction fpIm3DRenderLine;
    RwIm3DRenderTriangleFunction fpIm3DRenderTriangle;
    RwIm3DRenderPrimitiveFunction fpIm3DRenderPrimitive;
    RwIm3DRenderIndexedPrimitiveFunction fpIm3DRenderIndexedPrimitive;
};

struct RwMetrics
{
    RwUInt32 numTriangles;
    RwUInt32 numProcTriangles;
    RwUInt32 numVertices;
    RwUInt32 numTextureUploads;
    RwUInt32 sizeTextureUploads;
    RwUInt32 numResourceAllocs;
    void* devSpecificMetrics;
};

struct RwSubSystemInfo
{
    RwChar name[SUBSYSTEMNAME_MAXLEN];
};

struct RwVideoMode
{
    RwInt32 width;
    RwInt32 height;
    RwInt32 depth;
    RwVideoModeFlag flags;
    RwInt32 refRate;
    RwInt32 format;
};

using rwFnFexist = RwBool (*)(const RwChar* name);
using rwFnFopen = void* (*)(const RwChar* name, const RwChar* mode);
using rwFnFclose = int (*)(void* fptr);
using rwFnFread = size_t (*)(void* addr, size_t size, size_t count, void* fptr);
using rwFnFwrite = size_t (*)(const void* addr, size_t size, size_t count, void* fptr);
using rwFnFgets = RwChar* (*)(RwChar* buffer, int maxLen, void* fptr);
using rwFnFputs = int (*)(const RwChar* buffer, void* fptr);
using rwFnFeof = int (*)(void* fptr);
using rwFnFseek = int (*)(void* fptr, long offset, int origin);
using rwFnFflush = int (*)(void* fptr);
using rwFnFtell = int (*)(void* fptr);

struct RwFileFunctions
{
    rwFnFexist rwfexist;
    rwFnFopen rwfopen;
    rwFnFclose rwfclose;
    rwFnFread rwfread;
    rwFnFwrite rwfwrite;
    rwFnFgets rwfgets;
    rwFnFputs rwfputs;
    rwFnFeof rwfeof;
    rwFnFseek rwfseek;
    rwFnFflush rwfflush;
    rwFnFtell rwftell;
};

struct RwError
{
    RwInt32 pluginID;
    RwInt32 errorCode;
};

using RwDebugHandler = void (*)(RwDebugType type, const RwChar* string);

struct RwGlobals
{
    void* curCamera;
    void* curWorld;
    RwUInt16 renderFrame;
    RwUInt16 lightFrame;
    RwUInt16 pad[2];
    RwDevice dOpenDevice;
    RwStandardFunc stdFunc[rwSTANDARDNUMOFSTANDARD];
    RwLinkList dirtyFrameList;
    RwFileFunctions fileFuncs;
    RwStringFunctions stringFuncs;
    RwMemoryFunctions memoryFuncs;
    RwMemoryAllocFn memoryAlloc;
    RwMemoryFreeFn memoryFree;
    RwMetrics* metrics;
    RwEngineStatus engineStatus;
    RwUInt32 resArenaInitSize;
};

struct RwModuleInfo
{
    RwInt32 globalsOffset;
    RwInt32 numInstances;
};

using RwResEntryDestroyNotify = void (*)(RwResEntry* resEntry);
struct RwResEntry
{
    RwLLLink link;
    RwInt32 size;
    void* owner;
    RwResEntry** ownerRef;
    RwResEntryDestroyNotify destroyNotify;
};

struct rwResources
{
    RwInt32 maxSize;
    RwInt32 currentSize;
    RwInt32 reusageSize;
    void* memHeap;
    RwLinkList entriesA;
    RwLinkList entriesB;
    RwLinkList* freeEntries;
    RwLinkList* usedEntries;
};

struct rwResourcesGlobals
{
    rwResources res;
};

struct RwRGBAReal
{
    RwReal red;
    RwReal green;
    RwReal blue;
    RwReal alpha;
};

struct RwRGBA
{
    RwUInt8 red;
    RwUInt8 green;
    RwUInt8 blue;
    RwUInt8 alpha;
};

struct rwStreamMatrix
{
    RwV3d right;
    RwV3d up;
    RwV3d at;
    RwV3d pos;
    RwInt32 type;
};
using RwMatrixChunkInfo = rwStreamMatrix;

struct RwChunkHeaderInfo
{
    RwUInt32 type;
    RwUInt32 length;
    RwUInt32 version;
    RwUInt32 buildNum;
    RwBool isComplex;
};

// =============================================
// =============== rwcore.h ====================
// =============================================

struct rxHeapFreeBlock
{
    RwUInt32 size;
    rxHeapBlockHeader* ptr;
};

struct rxHeapSuperBlockDescriptor
{
    void* start;
    RwUInt32 size;
    rxHeapSuperBlockDescriptor* next;
};

struct RxHeap
{
    RwUInt32 superBlockSize;
    rxHeapSuperBlockDescriptor* head;
    rxHeapBlockHeader* headBlock;
    rxHeapFreeBlock* freeBlocks;
    RwUInt32 entriesAlloced;
    RwUInt32 entriesUsed;
    RwBool dirty;
};

struct rxHeapBlockHeader
{
    rxHeapBlockHeader *prev, *next;
    RwUInt32 size;
    rxHeapFreeBlock* freeEntry;
    RwUInt32 pad[4];
};

struct RxClusterDefinition
{
    RwChar* name;
    RwUInt32 defaultStride;
    RwUInt32 defaultAttributes;
    const RwChar* attributeSet;
};

struct RxOutputSpec
{
    RwChar* name;
    RxClusterValid* outputClusters;
    RxClusterValid allOtherClusters;
};

struct RxClusterRef
{
    RxClusterDefinition* clusterDef;
    RxClusterForcePresent forcePresent;
    RwUInt32 reserved;
};

struct RxIoSpec
{
    RwUInt32 numClustersOfInterest;
    RxClusterRef* clustersOfInterest;
    RxClusterValidityReq* inputRequirements;
    RwUInt32 numOutputs;
    RxOutputSpec* outputs;
};

using RxNodeBodyFn = RwBool (*)(RxPipelineNode* self, const RxPipelineNodeParam* params);
using RxNodeInitFn = RwBool (*)(RxNodeDefinition* self);
using RxNodeTermFn = void (*)(RxNodeDefinition* self);
using RxPipelineNodeInitFn = RwBool (*)(RxPipelineNode* self);
using RxPipelineNodeTermFn = void (*)(RxPipelineNode* self);
using RxPipelineNodeConfigFn = RwBool (*)(RxPipelineNode* self, RxPipeline* pipeline);
using RxConfigMsgHandlerFn = RwUInt32 (*)(RxPipelineNode* self, RwUInt32 msg, RwUInt32 intparam, void* ptrparam);

struct RxNodeMethods
{
    RxNodeBodyFn nodeBody;
    RxNodeInitFn nodeInit;
    RxNodeTermFn nodeTerm;
    RxPipelineNodeInitFn pipelineNodeInit;
    RxPipelineNodeTermFn pipelineNodeTerm;
    RxPipelineNodeConfigFn pipelineNodeConfig;
    RxConfigMsgHandlerFn configMsgHandler;
};

struct RxNodeDefinition
{
    RwChar* name;
    RxNodeMethods nodeMethods;
    RxIoSpec io;
    RwUInt32 pipelineNodePrivateDataSize;
    RxNodeDefEditable editable;
    RwInt32 InputPipesCnt;
};

struct RxPipelineCluster
{
    RxClusterDefinition* clusterRef;
    RwUInt32 creationAttributes;
};

struct RxCluster
{
    RwUInt16 flags;
    RwUInt16 stride;
    void* data;
    void* currentData;
    RwUInt32 numAlloced;
    RwUInt32 numUsed;
    RxPipelineCluster* clusterRef;
    RwUInt32 attributes;
};

struct RxPacket
{
    RwUInt16 flags;
    RwUInt16 numClusters;
    RxPipeline* pipeline;
    RwUInt32* inputToClusterSlot;
    RwUInt32* slotsContinue;
    RxPipelineCluster** slotClusterRefs;
    RxCluster clusters[1];
};

struct RxPipelineNode
{
    RxNodeDefinition* nodeDef;
    RwUInt32 numOutputs;
    RwUInt32* outputs;
    RxPipelineCluster** slotClusterRefs;
    RwUInt32* slotsContinue;
    void* privateData;
    RwUInt32* inputToClusterSlot;
    RxPipelineNodeTopSortData* topSortData;
    void* initializationData;
    RwUInt32 initializationDataSize;
};

struct rxReq;

struct RxPipelineNodeTopSortData
{
    RwUInt32 numIns;
    RwUInt32 numInsVisited;
    rxReq* req;
};

struct RxPipelineNodeParam
{
    void* dataParam;
    RxHeap* heap;
};

struct RxExecutionContext
{
    RxPipeline* pipeline;
    RxPipelineNode* currentNode;
    RwInt32 exitCode;
    RwUInt32 pad;
    RxPipelineNodeParam params;
};

struct RxPipelineRequiresCluster
{
    RxClusterDefinition* clusterDef;
    RxClusterValidityReq rqdOrOpt;
    RwUInt32 slotIndex;
};

struct RxPipeline
{
    RwBool locked;
    RwUInt32 numNodes;
    RxPipelineNode* nodes;
    RwUInt32 packetNumClusterSlots;
    rxEmbeddedPacketState embeddedPacketState;
    RxPacket* embeddedPacket;
    RwUInt32 numInputRequirements;
    RxPipelineRequiresCluster* inputRequirements;
    void* superBlock;
    RwUInt32 superBlockSize;
    RwUInt32 entryPoint;
    RwUInt32 pluginId;
    RwUInt32 pluginData;
};

using RxPipelineNodeOutputCallBack = RxPipelineNode* (*)(RxPipelineNode* node, RxPipelineNode* outputnode,
    void* callbackdata);

using RxNodeOutput = RwUInt32*;
using RxNodeInput = RxPipelineNode*;
using RxLockedPipe = RxPipeline;

struct RwRaster
{
    RwRaster* parent;
    RwUInt8* cpPixels;
    RwUInt8* palette;
    RwInt32 width, height, depth;
    RwInt32 stride;
    RwInt16 nOffsetX, nOffsetY;
    RwUInt8 cType;
    RwUInt8 cFlags;
    RwUInt8 privateFlags;
    RwUInt8 cFormat;
    RwUInt8* originalPixels;
    RwInt32 originalWidth;
    RwInt32 originalHeight;
    RwInt32 originalStride;
};

struct RxObjSpace3DVertex
{
    RwV3d objVertex;
    RwV3d objNormal;
    RwUInt32 color;
    RwReal u;
    RwReal v;
};

using RxObjSpace3DLitVertex = RxObjSpace3DVertex;
using RwIm3DVertex = RxObjSpace3DLitVertex;
using RxScrSpace2DVertex = RwIm2DVertex;

struct RxRenderStateVector
{
    RwUInt32 Flags;
    RwShadeMode ShadeMode;
    RwBlendFunction SrcBlend;
    RwBlendFunction DestBlend;
    RwRaster* TextureRaster;
    RwTextureAddressMode AddressModeU;
    RwTextureAddressMode AddressModeV;
    RwTextureFilterMode FilterMode;
    RwRGBA BorderColor;
    RwFogType FogType;
    RwRGBA FogColor;
};

struct RwImage
{
    RwInt32 flags;
    RwInt32 width;
    RwInt32 height;
    RwInt32 depth;
    RwInt32 stride;
    RwUInt8* cpPixels;
    RwRGBA* palette;
};

using RwImageCallBackRead = RwImage* (*)(const RwChar* imageName);
using RwImageCallBackWrite = RwImage* (*)(RwImage* image, const RwChar* imageName);

struct _rwPalQuantRGBABox
{
    RwInt32 col0[4];
    RwInt32 col1[4];
};

struct _rwPalQuantLeafNode
{
    RwReal weight;
    RwRGBAReal ac;
    RwReal var;
    RwUInt8 palIndex;
};

struct _rwPalQuantBranchNode
{
    _rwPalQuantOctNode* dir[16];
};

struct _rwPalQuantOctNode
{
    _rwPalQuantLeafNode Leaf;
    _rwPalQuantBranchNode Branch;
};

struct RwPalQuant
{
    _rwPalQuantRGBABox Mcube[RWPALQUANT_MAXCOLOR];
    RwReal Mvv[RWPALQUANT_MAXCOLOR];
    _rwPalQuantLeafNode Mvol[RWPALQUANT_MAXCOLOR];
    _rwPalQuantOctNode* root;
    RwFreeList* cubefreelist;
};

struct RwTexDictionary
{
    RwObject object;
    RwLinkList texturesInDict;
    RwLLLink lInInstance;
};

struct RwTexture
{
    RwRaster* raster;
    RwTexDictionary* dict;
    RwLLLink lInDictionary;
    RwChar name[rwTEXTUREBASENAMELENGTH];
    RwChar mask[rwTEXTUREBASENAMELENGTH];
    RwUInt32 filterAddressing;
    RwInt32 refCount;
};

using RwTextureCallBackRead = RwTexture* (*)(const RwChar* name, const RwChar* maskName);
using RwTextureCallBackFind = RwTexture* (*)(const RwChar* name);
using RwTextureCallBack = RwTexture* (*)(RwTexture* texture, void* pData);
using RwTexDictionaryCallBack = RwTexDictionary* (*)(RwTexDictionary* dict, void* data);
using RwTextureCallBackMipmapGeneration = RwRaster* (*)(RwRaster* raster, RwImage* image);
using RwTextureCallBackMipmapName = RwBool (*)(RwChar* name, RwChar* maskName, RwUInt8 mipLevel, RwInt32 format);

struct RxCamSpace3DVertex
{
    RwV3d cameraVertex;
    RwUInt8 clipFlags;
    RwUInt8 pad[3];
    RwRGBAReal col;
    RwReal u;
    RwReal v;
};
using RwCameraVertex = RxCamSpace3DVertex;

struct RxMeshStateVector
{
    RwInt32 Flags;
    void* SourceObject;
    RwMatrix Obj2World;
    RwMatrix Obj2Cam;
    RwSurfaceProperties SurfaceProperties;
    RwTexture* Texture;
    RwRGBA MatCol;
    RxPipeline* Pipeline;
    RwPrimitiveType PrimType;
    RwUInt32 NumElements;
    RwUInt32 NumVertices;
    RwInt32 ClipFlagsOr;
    RwInt32 ClipFlagsAnd;
    void* SourceMesh;
    void* DataObject;
};

struct RxScatter
{
    RxPipeline* pipeline;
    RxPipelineNode* node;
};

struct RxInterp
{
    RxVertexIndex originalVert;
    RxVertexIndex parentVert1;
    RxVertexIndex parentVert2;
    RwReal interp;
};

struct RxUV
{
    RwReal u;
    RwReal v;
};

struct RxTriPlane
{
    RwUInt32 N;
    RwReal w;
};

#pragma pack(push, 1)
struct RxVStep
{
    RwUInt8 step;
};
#pragma pack(pop)

using RxCamNorm = RwV3d;

struct _rwIm3DPoolStash
{
    RwUInt32 flags;
    RwMatrix* ltm;
    RwUInt32 numVerts;
    RxObjSpace3DVertex* objVerts;
    RxCamSpace3DVertex* camVerts;
    RxScrSpace2DVertex* devVerts;
    RxMeshStateVector* meshState;
    RxRenderStateVector* renderState;
    RxPipeline* pipeline;
    RwPrimitiveType primType;
    RxVertexIndex* indices;
    RwUInt32 numIndices;
};

struct rwIm3DPool
{
    RwUInt16 numElements;
    RwUInt16 pad;
    void* elements;
    RwInt32 stride;
    _rwIm3DPoolStash stash;
};

struct rwIm3DRenderPipelines
{
    RxPipeline* triList;
    RxPipeline* triFan;
    RxPipeline* triStrip;
    RxPipeline* lineList;
    RxPipeline* polyLine;
    RxPipeline* pointList;
};

struct rwImmediGlobals
{
    RxPipeline* im3DTransformPipeline;
    rwIm3DRenderPipelines im3DRenderPipelines;
    RxPipeline* platformIm3DTransformPipeline;
    rwIm3DRenderPipelines platformIm3DRenderPipelines;
    rwIm3DPool curPool;
};

struct RxD3D9VertexStream
{
    void* vertexBuffer;
    RwUInt32 offset;
    RwUInt32 stride;
    RwUInt16 geometryFlags;
    RwUInt8 managed;
    RwUInt8 dynamicLock;
};

using rwD3D9DeviceRestoreCallBack = void (*)(void);

struct RwFrame
{
    RwObject object;
    RwLLLink inDirtyListLink;
    RwMatrix modelling;
    RwMatrix ltm;
    RwLinkList objectList;
    struct RwFrame* child;
    struct RwFrame* next;
    struct RwFrame* root;
};

using RwFrameCallBack = RwFrame* (*)(RwFrame* frame, void* data);

struct RwObjectHasFrame
{
    RwObject object;
    RwLLLink lFrame;
    RwObjectHasFrameSyncFunction sync;
};
using RwObjectHasFrameSyncFunction = RwObjectHasFrame* (*)(RwObjectHasFrame* object);

struct rpTextureChunkInfo
{
    RwTextureFilterMode filtering;
    RwTextureAddressMode addressingU;
    RwTextureAddressMode addressingV;
};
using RwTextureChunkInfo = rpTextureChunkInfo;

struct rwFrameList
{
    RwFrame** frames;
    RwInt32 numFrames;
};

struct RwBBox
{
    RwV3d sup;
    RwV3d inf;
};

using RwCameraBeginUpdateFunc = RwCamera* (*)(RwCamera* camera);
using RwCameraEndUpdateFunc = RwCamera* (*)(RwCamera* camera);

struct RwFrustumPlane
{
    RwPlane plane;
    RwUInt8 closestX;
    RwUInt8 closestY;
    RwUInt8 closestZ;
    RwUInt8 pad;
};

struct RwCamera
{
    RwObjectHasFrame object;
    RwCameraProjection projectionType;
    RwCameraBeginUpdateFunc beginUpdate;
    RwCameraEndUpdateFunc endUpdate;
    RwMatrix viewMatrix;
    RwRaster* frameBuffer;
    RwRaster* zBuffer;
    RwV2d viewWindow;
    RwV2d recipViewWindow;
    RwV2d viewOffset;
    RwReal nearPlane;
    RwReal farPlane;
    RwReal fogPlane;
    RwReal zScale, zShift;
    RwFrustumPlane frustumPlanes[6];
    RwBBox frustumBoundBox;
    RwV3d frustumCorners[8];
};

using RwCameraCallBack = RwCamera* (*)(RwCamera* camera, void* data);

struct rwPipeGlobals
{
    RwFreeList* pipesFreeList;
    RxRenderStateVector defaultRenderState;
    RwLinkList allPipelines;
    RwUInt32 maxNodesPerPipe;
    RxPipeline* currentAtomicPipeline;
    RxPipeline* currentWorldSectorPipeline;
    RxPipeline* currentMaterialPipeline;
    RxPipeline* genericAtomicPipeline;
    RxPipeline* genericWorldSectorPipeline;
    RxPipeline* genericMaterialPipeline;
    RxPipeline* platformAtomicPipeline;
    RxPipeline* platformWorldSectorPipeline;
    RxPipeline* platformMaterialPipeline;
};

struct rwStreamCamera
{
    RwV2d viewWindow;
    RwV2d viewOffset;
    RwReal nearPlane, farPlane;
    RwReal fogPlane;
    RwUInt32 projection;
};
using RwCameraChunkInfo = rwStreamCamera;

// rphanim.h

struct RpHAnimAtomicGlobalVars
{
    RwInt32 engineOffset;
    RwFreeList* HAnimFreeList;
};

struct RpHAnimKeyFrame
{
    RpHAnimKeyFrame* prevFrame;
    RwReal time;
    RtQuat q;
    RwV3d t;
};

struct RpHAnimInterpFrame
{
    RpHAnimKeyFrame* keyFrame1;
    RpHAnimKeyFrame* keyFrame2;
    RtQuat q;
    RwV3d t;
};

struct RpHAnimNodeInfo
{
    RwInt32 nodeID;
    RwInt32 nodeIndex;
    RwInt32 flags;
    RwFrame* pFrame;
};

struct RpHAnimHierarchy
{
    RwInt32 flags;
    RwInt32 numNodes;
    RwMatrix* pMatrixArray;
    void* pMatrixArrayUnaligned;
    RpHAnimNodeInfo* pNodeInfo;
    RwFrame* parentFrame;
    RpHAnimHierarchy* parentHierarchy;
    RwInt32 rootParentOffset;
    RtAnimInterpolator* currentAnim;
};

struct RpHAnimFrameExtension
{
    RwInt32 id;
    RpHAnimHierarchy* hierarchy;
};

using RpHAnimAnimation = RtAnimAnimation;
using RpHAnimStdKeyFrame = RpHAnimKeyFrame;

// rpskin.h

struct RwMatrixWeights
{
    RwReal w0;
    RwReal w1;
    RwReal w2;
    RwReal w3;
};

#pragma pack(push, 1)
struct RpSkin
{
    RwInt32 numBones;
    RwInt32 numUsedIndices;
    RwInt32 usedIndices;
    RwInt32 inverseMatrices;
    RwInt32 maxIndex;
    RwInt32 indices;
    RwInt32 weights;
    RwInt32 field_1C;
    RwInt32 field_20;
    RwInt32 splitInfo1;
    RwInt32 splitInfo2;
    RwInt32 splitInfo3;
    RwInt32 splitData;
    RwInt32 field_34;
    RwInt32 field_38;
    RwInt32 data;
};
#pragma pack(pop)


// rpuvanim.h

struct RpUVAnimMaterialGlobalVars
{
    RwInt32 engineOffset;
};

struct _rpUVAnimCustomData
{
    RwChar name[RP_UVANIM_MAXNAME];
    RwUInt32 nodeToUVChannelMap[RP_UVANIM_MAXSLOTS];
    RwUInt32 refCount;
};

typedef RtAnimAnimation RpUVAnim;
typedef RtAnimInterpolator RpUVAnimInterpolator;

struct RpUVAnimLinearKeyFrameData
{
    RwReal uv[6];
};

struct RpUVAnimParamKeyFrameData
{
    RwReal theta;
    RwReal s0;
    RwReal s1;
    RwReal skew;
    RwReal x;
    RwReal y;
};

union RpUVAnimKeyFrameData
{
    RpUVAnimLinearKeyFrameData linear;
    RpUVAnimParamKeyFrameData param;
};

struct RpUVAnimKeyFrame
{
    RpUVAnimKeyFrame* prevFrame;
    RwReal time;
    RpUVAnimKeyFrameData data;
};

struct RpUVAnimInterpFrame
{
    RpUVAnimKeyFrame* keyFrame1;
    RpUVAnimKeyFrame* keyFrame2;
    RpUVAnimKeyFrameData data;
};

struct RpUVAnimMaterialExtension
{
    RwMatrix* uv[RP_UVANIM_APPLIEDCHANNELS];
    RtAnimInterpolator* interp[RP_UVANIM_MAXSLOTS];
};

// rpworld.h

struct _rpD3D9VertexShaderDescriptor
{
    RwUInt8 numDirectionalLights : 4;
    RwUInt8 numPointLights : 4;
    RwUInt8 numSpotLights : 4;
    RwUInt8 numTexCoords : 4;
    RwUInt8 morphing : 1;
    RwUInt8 numWeights : 3;
    RwUInt8 prelit : 1;
    RwUInt8 normals : 1;
    RwUInt8 normalizeNormals : 1;
    RwUInt8 modulateMaterial : 1;
    RwUInt8 fogMode : 2;
    RwUInt8 effect : 6;
};

struct _rpD3D9VertexShaderDispatchDescriptor
{
    RwUInt8 offsetMaterialColor;
    RwUInt8 offsetFogRange;
    RwUInt8 offsetEffect;
    RwUInt8 offsetMorphingCoef;
    RwUInt8 offsetBoneMatrices;
    RwUInt8 pad[3];
};

struct RpMaterialChunkInfo
{
    RwInt32 flags;
    RwRGBA color;
    RwInt32 unused;
    RwBool textured;
    RwSurfaceProperties surfaceProps;
};
using _rpMaterial = RpMaterialChunkInfo;

struct RpMaterial
{
    RwTexture* texture;
    RwRGBA color;
    RxPipeline* pipeline;
    RwSurfaceProperties surfaceProps;
    RwInt16 refCount;
    RwInt16 pad;
};

using RpMaterialCallBack = RpMaterial* (*)(RpMaterial* material, void* data);

struct RpMaterialList
{
    RpMaterial** materials;
    RwInt32 numMaterials;
    RwInt32 space;
};
