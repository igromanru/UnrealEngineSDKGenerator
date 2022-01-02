#include <windows.h>

#include "PatternFinder.hpp"
#include "ObjectsStore.hpp"

#include "EngineClasses.hpp"

// This class contains the class which allows the generator access to global objects list.

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
	FUObjectItem* Objects;
	int32_t MaxElements;
	int32_t NumElements;
};

class FUObjectArray
{
public:
	__int32 ObjFirstGCIndex; //0x0000
	__int32 ObjLastNonGCIndex; //0x0004
	__int32 MaxObjectsNotConsideredByGC; //0x0008
	__int32 OpenForDisregardForGC; //0x000C

	TUObjectArray ObjObjects; //0x0010
};

FUObjectArray* GlobalObjects = nullptr;

bool ObjectsStore::Initialize()
{	
	// 89 0D ? ? ? ? 48 8B DF 48 89 5C 24
	const auto address = FindPattern(GetModuleHandleW(nullptr), reinterpret_cast<const unsigned char*>(
		"\x89\x0D\x00\x00\x00\x00\x48\x8B\xDF\x48\x89\x5C\x24"), "xx????xxxxxxx");
	if (address == -1)
	{
		return false;
	}
	const auto offset = *reinterpret_cast<uint32_t*>(address + 2);
	GlobalObjects = reinterpret_cast<decltype(GlobalObjects)>(address + 6 + offset);
	
	return true;
}

void* ObjectsStore::GetAddress()
{
	return GlobalObjects;
}

size_t ObjectsStore::GetObjectsNum() const
{
	return GlobalObjects->ObjObjects.NumElements;
}

UEObject ObjectsStore::GetById(size_t id) const
{
	return GlobalObjects->ObjObjects.Objects[id].Object;
}
