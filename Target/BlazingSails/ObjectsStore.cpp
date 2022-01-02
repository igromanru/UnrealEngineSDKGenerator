#include <windows.h>

#include "PatternFinder.hpp"
#include "ObjectsStore.hpp"
#include "EngineClasses.hpp"

class FUObjectItem
{
public:
	UObject* Object; //0x0000
	int32_t Flags; //0x0008
	int32_t ClusterIndex; //0x000C
	int32_t SerialNumber; //0x0010
};

class TUObjectArray
{
public:
	enum
	{
		NumElementsPerChunk = 64 * 1024, //66560 = 0x10400
	};

	FUObjectItem** Objects;
	FUObjectItem* PreAllocatedObjects;
	int32_t MaxElements;
	int32_t NumElements;
	int32_t MaxChunks;
	int32_t NumChunks;
	
	int32_t Num() const
	{
		return NumElements;
	}
	
	bool IsValidIndex(int32_t Index) const
	{
		return Index < Num() && Index >= 0;
	}

	FUObjectItem*  GetObjectPtr(size_t Index) const
	{
		const int32_t ChunkIndex = Index / NumElementsPerChunk;
		const int32_t WithinChunkIndex = Index % NumElementsPerChunk;
		FUObjectItem* Chunk = Objects[ChunkIndex];
		return Chunk + WithinChunkIndex;
	}

	UObject* GetUObject(int32_t Index) const
	{
		UObject* result = nullptr;
		if(IsValidIndex(Index))
		{
			const int32_t ChunkIndex = Index / NumElementsPerChunk;
			const int32_t WithinChunkIndex = Index % NumElementsPerChunk;
			FUObjectItem* Chunk = Objects[ChunkIndex];
			if(Chunk != nullptr)
			{
				FUObjectItem* object = Chunk + WithinChunkIndex;
				if(object != nullptr)
				{
					result = object->Object;
				}					
			}			
		}		
		return result;
	}
};

class FUObjectArray
{
public:
	/*int32_t ObjFirstGCIndex; The FindPattern searchs directly for ObjObjects
	int32_t ObjLastNonGCIndex;
	int32_t MaxObjectsNotConsideredByGC;
	bool OpenForDisregardForGC;*/

	TUObjectArray ObjObjects;
};

FUObjectArray* GlobalObjects = nullptr;

bool ObjectsStore::Initialize()
{
	// 48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 04 D1 EB 03 49 8B C1
	// 48 8D 14 40 48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 04 D1 EB 03 49 8B C1 + 4
	auto address = FindPattern(GetModuleHandleW(nullptr), reinterpret_cast<const unsigned char*>(
		"\x48\x8B\x05\x00\x00\x00\x00\x48\x8B\x0C\xC8\x48\x8D\x04\xD1\xEB\x03\x49\x8B\xC1"), "xxx????xxxxxxxxxxxxx");

	if (address == -1)
	{
		return false;
	}
	const auto offset = *reinterpret_cast<uint32_t*>(address + 3);
	GlobalObjects = reinterpret_cast<decltype(GlobalObjects)>(address + 7 + offset);

	return true;
}

void* ObjectsStore::GetAddress()
{
	return GlobalObjects;
}

size_t ObjectsStore::GetObjectsNum() const
{
	return GlobalObjects->ObjObjects.Num();
}

UEObject ObjectsStore::GetById(size_t id) const
{
	return GlobalObjects->ObjObjects.GetUObject(id);
}
