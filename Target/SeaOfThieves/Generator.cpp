#include "IGenerator.hpp"
#include "ObjectsStore.hpp"
#include "NamesStore.hpp"

// This file contains the specific implementation for a game. For more informations can be found in the readme.

class Generator : public IGenerator
{
public:
	bool Initialize(void* module) override
	{
		alignasClasses = {
			{ "ScriptStruct CoreUObject.Plane", 16 },
			{ "ScriptStruct CoreUObject.Quat", 16 },
			{ "ScriptStruct CoreUObject.Transform", 16 },
			{ "ScriptStruct CoreUObject.Vector4", 16 },

			{ "ScriptStruct Engine.RootMotionSourceGroup", 8 }
		};

		virtualFunctionPattern["Class CoreUObject.Object"] = { // 0F ? ? ? ? 00 00 B9 FF FF 00 00 66 3B C1 74
			{ "\x0F\x00\x00\x00\x00\x00\x00\xB9\xFF\xFF\x00\x00\x66\x3B\xC1\x74", "x????xxxxxxxxxxx", 0x400, R"(	inline void ProcessEvent(class UFunction* function, void* parms)
	{
		return GetVFunction<void(*)(UObject*, class UFunction*, void*)>(this, %d)(this, function, parms);
	})" }
		};
		virtualFunctionPattern["Class CoreUObject.Class"] = { // 48 8B F9 48 83 B9 ? ? ? ? ? 0F 85
			{ "\x48\x8B\xF9\x48\x83\xB9\x00\x00\x00\x00\x00\x0F\x85", "xxxxxx?????xx", 0x200, R"(	inline UObject* CreateDefaultObject()
	{
		return GetVFunction<UObject*(*)(UClass*)>(this, %d)(this);
	})" }
		};

		predefinedMembers["Class CoreUObject.Object"] = {
			{ "void*", "Vtable" },
			{ "int32_t", "ObjectFlags" },
			{ "int32_t", "InternalIndex" },
			{ "class UClass*", "Class" },
			{ "FName", "Name" },
			{ "class UObject*", "Outer" }
		};
		predefinedStaticMembers["Class CoreUObject.Object"] = {
			{ "FUObjectArray*", "GObjects" }
		};
		predefinedMembers["Class CoreUObject.Field"] = {
			{ "class UField*", "Next" }
		};
		predefinedMembers["Class CoreUObject.Struct"] = {
			{ "class UStruct*", "SuperField" },
			{ "class UField*", "Children" },
			{ "int32_t", "PropertySize" },
			{ "int32_t", "MinAlignment" },
			{ "unsigned char", "UnknownData0x0048[0x40]" }
		};
		predefinedMembers["Class CoreUObject.Function"] = {
			{ "int32_t", "FunctionFlags" },
			{ "int16_t", "RepOffset" },
			{ "int8_t", "NumParms" },
			{ "int16_t", "ParmsSize" },
			{ "int16_t", "ReturnValueOffset" },
			{ "int16_t", "RPCId" },
			{ "int16_t", "RPCResponseId" },
			{ "class UProperty*", "FirstPropertyToInit" },
			{ "class UFunction*", "EventGraphFunction" },
			{ "int32_t", "EventGraphCallOffset" },
			{ "void*", "Func" }
		};

		predefinedMethods["Class CoreUObject.Object"] = {
			PredefinedMethod::Inline(R"(	static inline TUObjectArray& GetGlobalObjects()
	{
		return GObjects->ObjObjects;
	})"),
			PredefinedMethod::Default("std::string GetName() const", R"(std::string UObject::GetName() const
{
	std::string name(Name.GetName());
	if (Name.Number > 0)
	{
		name += '_' + std::to_string(Name.Number);
	}

	auto pos = name.rfind('/');
	if (pos == std::string::npos)
	{
		return name;
	}
	
	return name.substr(pos + 1);
})"),
			PredefinedMethod::Default("std::string GetFullName() const", R"(std::string UObject::GetFullName() const
{
	std::string name;

	if (Class != nullptr)
	{
		std::string temp;
		for (auto p = Outer; p; p = p->Outer)
		{
			temp = p->GetName() + "." + temp;
		}

		name = Class->GetName();
		name += " ";
		name += temp;
		name += GetName();
	}

	return name;
})"),
			PredefinedMethod::Inline(R"(	template<typename T>
	static T* FindObject(const std::string& name)
	{
		for (int i = 0; i < GetGlobalObjects().Num(); ++i)
		{
			auto object = GetGlobalObjects().GetByIndex(i);
	
			if (object == nullptr)
			{
				continue;
			}
	
			if (object->GetFullName() == name)
			{
				return static_cast<T*>(object);
			}
		}
		return nullptr;
	})"),
			PredefinedMethod::Inline(R"(	static UClass* FindClass(const std::string& name)
	{
		return FindObject<UClass>(name);
	})"),
			PredefinedMethod::Inline(R"(	template<typename T>
	static T* GetObjectCasted(std::size_t index)
	{
		return static_cast<T*>(GetGlobalObjects().GetByIndex(index));
	})"),
			PredefinedMethod::Default("bool IsA(UClass* cmp) const", R"(bool UObject::IsA(UClass* cmp) const
{
	for (auto super = Class; super; super = static_cast<UClass*>(super->SuperField))
	{
		if (super == cmp)
		{
			return true;
		}
	}

	return false;
})")
		};
		predefinedMethods["Class CoreUObject.Class"] = {
			PredefinedMethod::Inline(R"(	template<typename T>
	inline T* CreateDefaultObject()
	{
		return static_cast<T*>(CreateDefaultObject());
	})")
		};

		predefinedMethods["ScriptStruct CoreUObject.Vector2D"] = {
			PredefinedMethod::Inline(R"(	inline FVector2D()
		: X(0), Y(0)
	{ })"),
			PredefinedMethod::Inline(R"(	inline FVector2D(float x, float y)
		: X(x),
		  Y(y)
	{ })"),
			PredefinedMethod::Inline(R"(    inline FVector2D operator + (const FVector2D& other) const 
	{ 
		return FVector2D(X + other.X, Y + other.Y); 
	})"),
            PredefinedMethod::Inline(R"(    inline FVector2D operator - (const FVector2D& other) const 
	{ 
		return FVector2D(X - other.X, Y - other.Y);
	 })"),
            PredefinedMethod::Inline(R"(    inline FVector2D operator * (float scalar) const 
	{ 
		return FVector2D(X * scalar, Y * scalar); 
	})"),
            PredefinedMethod::Inline(R"(    inline FVector2D& operator=  (const FVector2D& other) 
	{	
		X  = other.X;
		Y  = other.Y;
		return *this;
	})"),
            PredefinedMethod::Inline(R"(    inline FVector2D& operator+= (const FVector2D& other) 
	{ 
		X += other.X;
		Y += other.Y;
		return *this;
	})"),
            PredefinedMethod::Inline(R"(    inline FVector2D& operator-= (const FVector2D& other)
	{
		X -= other.X;
		Y -= other.Y;
		return *this;
	})"),
            PredefinedMethod::Inline(R"(    inline FVector2D& operator*= (const float other)
	{
		X *= other;
		Y *= other;
		return *this;
	})"),
			PredefinedMethod::Inline(R"(    friend bool operator==(const FVector2D& one, const FVector2D& two)
	{
		return one.X == two.X && one.Y == two.Y;
	})"),
			PredefinedMethod::Inline(R"(    friend bool operator!=(const FVector2D& one, const FVector2D& two)
	{
		return !(one == two);
	})"),
			PredefinedMethod::Inline(R"(    friend bool operator>(const FVector2D& one, const FVector2D& two)
	{
		return one.X > two.X && one.Y > two.Y;
	})"),
			PredefinedMethod::Inline(R"(    friend bool operator<(const FVector2D& one, const FVector2D& two)
	{
		return one.X < two.X && one.Y < two.Y;
	})")
		};
		
		predefinedMethods["ScriptStruct CoreUObject.LinearColor"] = {
			PredefinedMethod::Inline(R"(	inline FLinearColor()
		: R(0), G(0), B(0), A(0)
	{ })"),
			PredefinedMethod::Inline(R"(	inline FLinearColor(float r, float g, float b, float a)
		: R(r),
		  G(g),
		  B(b),
		  A(a)
	{ })"),
			PredefinedMethod::Inline(R"(	inline FLinearColor(float r, float g, float b)
		: R(r),
		  G(g),
		  B(b),
		  A(1.f)
		{ })"),
			PredefinedMethod::Inline(R"(	bool operator!=(const FLinearColor& other)
	{
	    return R != other.R || G != other.G || B != other.B || A != other.A;
	})"),
			PredefinedMethod::Inline(R"(	bool operator==(const FLinearColor& other)
	{
	    return R == other.R && G == other.G && B == other.B && A == other.A;
	})")
		};

		predefinedMethods["ScriptStruct CoreUObject.Vector"] = {
            PredefinedMethod::Inline(R"(    inline FVector() : X(0.f), Y(0.f), Z(0.f) {})"),

            PredefinedMethod::Inline(R"(    inline FVector(float x, float y, float z) : X(x), Y(y), Z(z) {})"),

            PredefinedMethod::Inline(R"(    inline FVector operator + (const FVector& other) const { return FVector(X + other.X, Y + other.Y, Z + other.Z); })"),
            PredefinedMethod::Inline(R"(    inline FVector operator - (const FVector& other) const { return FVector(X - other.X, Y - other.Y, Z - other.Z); })"),
            PredefinedMethod::Inline(R"(    inline FVector operator * (float scalar) const { return FVector(X * scalar, Y * scalar, Z * scalar); })"),

            PredefinedMethod::Inline(R"(    inline FVector& operator=  (const FVector& other) { X  = other.X; Y  = other.Y; Z  = other.Z; return *this; })"),
            PredefinedMethod::Inline(R"(    inline FVector& operator+= (const FVector& other) { X += other.X; Y += other.Y; Z += other.Z; return *this; })"),
            PredefinedMethod::Inline(R"(    inline FVector& operator-= (const FVector& other) { X -= other.X; Y -= other.Y; Z -= other.Z; return *this; })"),
            PredefinedMethod::Inline(R"(    inline FVector& operator*= (const float other)    { X *= other;   Y *= other;   Z *= other;   return *this; })"),
			PredefinedMethod::Inline(R"(   friend bool operator==(const FVector& first, const FVector& second)
	{
		return first.X == second.X && first.Y == second.Y && first.Z == second.Z;
	})"),
			PredefinedMethod::Inline(R"(   friend bool operator!=(const FVector& first, const FVector& second)
	{
		return !(first == second);
	})"),
		};

		predefinedMethods["ScriptStruct InputCore.Key"] = {
            PredefinedMethod::Inline(R"(    inline FKey() : KeyName(), UnknownData00{} {})"),
            PredefinedMethod::Inline(R"(    inline FKey(const FName InName)	: KeyName(InName), UnknownData00{} {})"),
            PredefinedMethod::Inline(R"(    inline FKey(const char * InName) : KeyName(FName(InName)), UnknownData00{} {})")
		};

		predefinedMethods["ScriptStruct CoreUObject.Quat"] = {
            PredefinedMethod::Inline(R"(    inline FQuat() {})"),
            PredefinedMethod::Inline(R"(    inline FQuat( float InX, float InY, float InZ, float InW ) : X(InX) , Y(InY) , Z(InZ) , W(InW) {})"),
            PredefinedMethod::Inline(R"(    inline FQuat( const FQuat& Q ) : X(Q.X), Y(Q.Y), Z(Q.Z), W(Q.W) {})")
		};

		predefinedMethods["ScriptStruct CoreUObject.Rotator"] = {
            PredefinedMethod::Inline(R"(    inline FRotator() {})"),
            PredefinedMethod::Inline(R"(    inline FRotator(float pitch, float yaw, float roll) : Pitch(pitch), Yaw(yaw), Roll(roll) {})"),

			PredefinedMethod::Inline(R"(    inline FRotator operator + (const FRotator& other) const { return FRotator(Pitch + other.Pitch, Yaw + other.Yaw, Roll + other.Roll); })"),
			PredefinedMethod::Inline(R"(    inline FRotator operator - (const FRotator& other) const { return FRotator(Pitch - other.Pitch,Yaw - other.Yaw, Roll - other.Roll); })"),
			PredefinedMethod::Inline(R"(    inline FRotator operator * (float scalar) const { return FRotator(Pitch * scalar,Yaw * scalar, Roll * scalar); })"),
			PredefinedMethod::Inline(R"(    inline FRotator& operator=  (const FRotator& other) { Pitch = other.Pitch; Yaw = other.Yaw; Roll = other.Roll; return *this; })"),
			PredefinedMethod::Inline(R"(    inline FRotator& operator+= (const FRotator& other) { Pitch += other.Pitch; Yaw += other.Yaw; Roll += other.Roll; return *this; })"),
			PredefinedMethod::Inline(R"(    inline FRotator& operator-= (const FRotator& other) { Pitch -= other.Pitch; Yaw -= other.Yaw; Roll -= other.Roll; return *this; })"),
			PredefinedMethod::Inline(R"(    inline FRotator& operator*= (const float other)    { Yaw *= other; Pitch *= other; Roll *= other; return *this; })"),

			PredefinedMethod::Inline(R"(   friend bool operator==(const FRotator& first, const FRotator& second)
	{
		return first.Pitch == second.Pitch && first.Yaw == second.Yaw && first.Roll == second.Roll;
	})"),
			PredefinedMethod::Inline(R"(    friend bool operator!=(const FRotator& first, const FRotator& second)
	{
		return !(first == second);
	})"),
		};

		predefinedMethods["ScriptStruct CoreUObject.Box2D"] = {
            PredefinedMethod::Inline(R"(    FBox2D() : Min(FVector2D(0,0)), Max(FVector2D(0,0)), IsValid(1), UnknownData00{} {})"),
            PredefinedMethod::Inline(R"(    FBox2D(FVector2D min, FVector2D max) : Min(min), Max(max), IsValid(1), UnknownData00{} {})")
		};

		predefinedMethods["ScriptStruct CoreUObject.Guid"] = {
			PredefinedMethod::Inline(R"(    FGuid() : A(0), B(0), C(0), D(0) {})"),
			PredefinedMethod::Inline(R"(    FGuid(int a, int b, int c, int d) : A(a), B(b), C(c), D(d) {})"),
			PredefinedMethod::Inline(R"(   std::size_t operator()(const FGuid& guid) const
    {
      return std::hash<int>()(guid.A) ^ std::hash<int>()(guid.B) ^ std::hash<int>()(guid.C) ^ std::hash<int>()(guid.D);
    })"),
            PredefinedMethod::Inline(R"(    friend bool operator==( const FGuid& X, const FGuid& Y )
	{
		return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) == 0;
	})"),
			PredefinedMethod::Inline(R"(   friend bool operator!=( const FGuid& X, const FGuid& Y )
	{
		return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) != 0;
	})"),
			PredefinedMethod::Inline(R"(   friend bool operator<( const FGuid& X, const FGuid& Y )
	{
		return	((X.A < Y.A) ? true : ((X.A > Y.A) ? false :
				((X.B < Y.B) ? true : ((X.B > Y.B) ? false :
				((X.C < Y.C) ? true : ((X.C > Y.C) ? false :
				((X.D < Y.D) ? true : ((X.D > Y.D) ? false : false )))))))); //-V583
	})"),
			PredefinedMethod::Inline(R"(   int& operator[](int Index)
	{
		switch(Index)
		{
		case 0: return A;
		case 1: return B;
		case 2: return C;
		case 3: return D;
		}

		return A;
	})"),
			PredefinedMethod::Inline(R"(   const int& operator[](int Index) const
	{
		switch(Index)
		{
		case 0: return A;
		case 1: return B;
		case 2: return C;
		case 3: return D;
		}

		return A;
	})")
		};

		predefinedMethods["Class Engine.Level"] = {
			PredefinedMethod::Inline(R"(	TArray<class AActor*> GetActors()
	{
		return *reinterpret_cast<TArray<AActor*>*>(reinterpret_cast<uintptr_t>(this) + 0xA0);
	})")
		};
		
		predefinedMethods["Class Engine.SkinnedMeshComponent"] = {
			PredefinedMethod::Inline(R"(	TArray<FTransform> GetComponentSpaceTransforms(int currentReadComponentTransforms)
	{
		return *reinterpret_cast<TArray<FTransform>*>(reinterpret_cast<uintptr_t>(this) + 0x5A8 + 0x10 * (uintptr_t)currentReadComponentTransforms);
	})"),
			PredefinedMethod::Inline(R"(	int GetCurrentReadComponentTransforms()
	{
		return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + 0x5CC);
	})")
		};
		
		predefinedMethods["Class Engine.SceneComponent"] = {
			PredefinedMethod::Inline(R"(	FBoxSphereBounds GetBounds()
	{
		return *reinterpret_cast<FBoxSphereBounds*>(reinterpret_cast<uintptr_t>(this) + 0x100);
	})")
		};

		predefinedMethods["Class Athena.HarpoonLauncher"] = {
			PredefinedMethod::Inline(R"(	float GetPitch()
	{
		return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + 0xB04);
	})"),
			PredefinedMethod::Inline(R"(	void SetPitch(float value)
	{
		*reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + 0xB04) = value;
	})"),
			PredefinedMethod::Inline(R"(	float GetYaw()
	{
		return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + 0xB08);
	})"),
			PredefinedMethod::Inline(R"(	void SetYaw(float value)
	{
		*reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + 0xB08) = value;
	})"),
		};

		return true;
	}

	std::string GetOutputDirectory() const override
	{
		// C:\Users\(username)\AppData\Local\Packages\Microsoft.SeaofThieves_8wekyb3d8bbwe\AC\Temp
		return std::string(std::getenv("localappdata")) + "/Temp";
	}

	std::string GetGameName() const override
	{
		return "Sea of Thieves";
	}

	std::string GetGameNameShort() const override
	{
		return "SoT";
	}

	std::string GetGameVersion() const override
	{
		return "2";
	}

	std::string GetNamespaceName() const override
	{
		return "SDK";
	}

	bool ShouldConvertStaticMethods() const override
	{
		return false;
	}

	bool ShouldGenerateFunctionParametersFile() const override
	{
		return false;
	}

	bool ShouldXorStrings() const override
	{
		return true;
	}

	bool ShouldShowMessageBox() const override
	{
		return false;
	}

	std::vector<std::string> GetIncludes() const override
	{
		return { "XorStrEx/XorStrEx.hpp" };
	}
	
	std::string GetBasicDeclarations() const override
	{
		return R"(template<typename Fn>
inline Fn GetVFunction(const void *instance, std::size_t index)
{
	auto vtable = *reinterpret_cast<const void***>(const_cast<void*>(instance));
	return reinterpret_cast<Fn>(vtable[index]);
}

class UObject;

class FUObjectItem
{
public:
	UObject* Object;
	int32_t Flags;
	int32_t ClusterIndex; 
	int32_t SerialNumber;

	enum class EInternalObjectFlags : int32_t
	{
		None = 0,
		Native = 1 << 25,
		Async = 1 << 26,
		AsyncLoading = 1 << 27,
		Unreachable = 1 << 28,
		PendingKill = 1 << 29,
		RootSet = 1 << 30,
		NoStrongReference = 1 << 31
	};

	inline bool IsUnreachable() const
	{
		return !!(Flags & static_cast<std::underlying_type_t<EInternalObjectFlags>>(EInternalObjectFlags::Unreachable));
	}

	inline bool IsPendingKill() const
	{
		return !!(Flags & static_cast<std::underlying_type_t<EInternalObjectFlags>>(EInternalObjectFlags::PendingKill));
	}
};

class TUObjectArray
{
public:
	inline int32_t Num() const
	{
		return NumElements;
	}

	inline UObject* GetByIndex(int32_t index) const
	{
		return Objects[index].Object;
	}

	inline FUObjectItem* GetItemByIndex(int32_t index) const
	{
		if (index < NumElements)
		{
			return &Objects[index];
		}
		return nullptr;
	}

private:
	FUObjectItem* Objects;
	int32_t MaxElements;
	int32_t NumElements;
};

class FUObjectArray
{
public:
	int32_t ObjFirstGCIndex;
	int32_t ObjLastNonGCIndex;
	int32_t MaxObjectsNotConsideredByGC;
	int32_t OpenForDisregardForGC;
	TUObjectArray ObjObjects;
};

template<class T>
class TArray
{
	friend class FString;

public:
	inline TArray()
	{
		Data = nullptr;
		Count = Max = 0;
	};

	inline int Num() const
	{
		return Count;
	};

	inline T& operator[](int i)
	{
		return Data[i];
	};

	inline const T& operator[](int i) const
	{
		return Data[i];
	};

	inline bool IsValidIndex(int i) const
	{
		return i < Num();
	}

private:
	T* Data;
	int32_t Count;
	int32_t Max;
};

class FNameEntry
{
public:
	static const auto NAME_WIDE_MASK = 0x1;
	static const auto NAME_INDEX_SHIFT = 1;

	int32_t Index;
	char UnknownData00[0x04];
	FNameEntry* HashNext;
	union
	{
		char AnsiName[1024];
		wchar_t WideName[1024];
	};

	inline const int32_t GetIndex() const
	{
		return Index >> NAME_INDEX_SHIFT;
	}

	inline bool IsWide() const
	{
		return Index & NAME_WIDE_MASK;
	}

	inline const char* GetAnsiName() const
	{
		return AnsiName;
	}

	inline const wchar_t* GetWideName() const
	{
		return WideName;
	}
};

template<typename ElementType, int32_t MaxTotalElements, int32_t ElementsPerChunk>
class TStaticIndirectArrayThreadSafeRead
{
public:
	inline size_t Num() const
	{
		return NumElements;
	}

	inline bool IsValidIndex(int32_t index) const
	{
		return index < Num() && index >= 0;
	}

	inline ElementType const* const& operator[](int32_t index) const
	{
		return *GetItemPtr(index);
	}

private:
	inline ElementType const* const* GetItemPtr(int32_t Index) const
	{
		const auto ChunkIndex = Index / ElementsPerChunk;
		const auto WithinChunkIndex = Index % ElementsPerChunk;
		const auto Chunk = Chunks[ChunkIndex];
		return Chunk + WithinChunkIndex;
	}

	enum
	{
		ChunkTableSize = (MaxTotalElements + ElementsPerChunk - 1) / ElementsPerChunk
	};

	ElementType** Chunks[ChunkTableSize];
	int32_t NumElements;
	int32_t NumChunks;
};

using TNameEntryArray = TStaticIndirectArrayThreadSafeRead<FNameEntry, 2 * 1024 * 1024, 16384>;

struct FName
{
	int32_t ComparisonIndex;
	int32_t Number;

	inline FName()
		: ComparisonIndex(0),
		  Number(0)
	{
	};

	inline FName(int32_t i)
		: ComparisonIndex(i),
		  Number(0)
	{
	};

	FName(const char* nameToFind)
		: ComparisonIndex(0),
		  Number(0)
	{
		static std::unordered_set<int> cache;

		for (auto i : cache)
		{
			if (!std::strcmp(GetGlobalNames()[i]->GetAnsiName(), nameToFind))
			{
				ComparisonIndex = i;
				
				return;
			}
		}

		for (auto i = 0; i < GetGlobalNames().Num(); ++i)
		{
			if (GetGlobalNames()[i] != nullptr)
			{
				if (!std::strcmp(GetGlobalNames()[i]->GetAnsiName(), nameToFind))
				{
					cache.insert(i);

					ComparisonIndex = i;

					return;
				}
			}
		}
	};

	static TNameEntryArray *GNames;
	static inline TNameEntryArray& GetGlobalNames()
	{
		return *GNames;
	};

	inline const char* GetName() const
	{
		return GetGlobalNames()[ComparisonIndex]->GetAnsiName();
	};

	inline bool operator==(const FName &other) const
	{
		return ComparisonIndex == other.ComparisonIndex;
	};
};

class FString : public TArray<wchar_t>
{
public:
	inline FString()
	{
	};

	FString(const wchar_t* other)
	{
		Max = Count = *other ? static_cast<int32_t>(std::wcslen(other)) + 1 : 0;

		if (Count)
		{
			Data = const_cast<wchar_t*>(other);
		}
	};

	inline bool IsValid() const
	{
		return Data != nullptr;
	}

	inline const wchar_t* c_str() const
	{
		return Data;
	}

	std::string ToString() const
	{
		const auto length = std::wcslen(Data);

		std::string str(length, '\0');

		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);

		return str;
	}
};

template<class TEnum>
class TEnumAsByte
{
public:
	inline TEnumAsByte()
	{
	}

	inline TEnumAsByte(TEnum _value)
		: value(static_cast<uint8_t>(_value))
	{
	}

	explicit inline TEnumAsByte(int32_t _value)
		: value(static_cast<uint8_t>(_value))
	{
	}

	explicit inline TEnumAsByte(uint8_t _value)
		: value(_value)
	{
	}

	inline operator TEnum() const
	{
		return (TEnum)value;
	}

	inline TEnum GetValue() const
	{
		return (TEnum)value;
	}

private:
	uint8_t value;
};

class FScriptInterface
{
private:
	UObject* ObjectPointer;
	void* InterfacePointer;

public:
	inline UObject* GetObject() const
	{
		return ObjectPointer;
	}

	inline UObject*& GetObjectRef()
	{
		return ObjectPointer;
	}

	inline void* GetInterface() const
	{
		return ObjectPointer != nullptr ? InterfacePointer : nullptr;
	}
};

template<class InterfaceType>
class TScriptInterface : public FScriptInterface
{
public:
	inline InterfaceType* operator->() const
	{
		return (InterfaceType*)GetInterface();
	}

	inline InterfaceType& operator*() const
	{
		return *((InterfaceType*)GetInterface());
	}

	inline operator bool() const
	{
		return GetInterface() != nullptr;
	}
};

struct FText
{
	char UnknownData[0x38];
};

struct FScriptDelegate
{
	char UnknownData[20];
};

struct FScriptMulticastDelegate
{
	char UnknownData[16];
};

template<typename Key, typename Value>
class TMap
{
	char UnknownData[0x50];
};

struct FWeakObjectPtr
{
public:
	inline bool SerialNumbersMatch(FUObjectItem* ObjectItem) const
	{
		return true;//ObjectItem->SerialNumber == ObjectSerialNumber;
	}

	bool IsValid() const;

	UObject* Get() const;

	int32_t ObjectIndex;
	int32_t ObjectSerialNumber;
};

template<class T, class TWeakObjectPtrBase = FWeakObjectPtr>
struct TWeakObjectPtr : private TWeakObjectPtrBase
{
public:
	inline T* Get() const
	{
		return (T*)TWeakObjectPtrBase::Get();
	}

	inline T& operator*() const
	{
		return *Get();
	}

	inline T* operator->() const
	{
		return Get();
	}

	inline bool IsValid() const
	{
		return TWeakObjectPtrBase::IsValid();
	}
};

template<class T, class TBASE>
class TAutoPointer : public TBASE
{
public:
	inline operator T*() const
	{
		return TBASE::Get();
	}

	inline operator const T*() const
	{
		return (const T*)TBASE::Get();
	}

	explicit inline operator bool() const
	{
		return TBASE::Get() != nullptr;
	}
};

template<class T>
class TAutoWeakObjectPtr : public TAutoPointer<T, TWeakObjectPtr<T>>
{
public:
};

template<typename TObjectID>
class TPersistentObjectPtr
{
public:
	FWeakObjectPtr WeakPtr;
	int32_t TagAtLastTest;
	TObjectID ObjectID;
};

struct FStringAssetReference_
{
	char UnknownData[0x10];
};

class FAssetPtr : public TPersistentObjectPtr<FStringAssetReference_>
{

};

template<typename ObjectType>
class TAssetPtr : FAssetPtr
{

};

struct FSoftObjectPath
{
	FName AssetPathName;
	FString SubPathString;
};

class FSoftObjectPtr : public TPersistentObjectPtr<FSoftObjectPath>
{

};

template<typename ObjectType>
class TSoftObjectPtr : FSoftObjectPtr
{

};

struct FUniqueObjectGuid_
{
	char UnknownData[0x10];
};

class FLazyObjectPtr : public TPersistentObjectPtr<FUniqueObjectGuid_>
{

};

template<typename ObjectType>
class TLazyObjectPtr : FLazyObjectPtr
{

};)";
	}

	std::string GetBasicDefinitions() const override
	{
		return R"(TNameEntryArray* FName::GNames = nullptr;
FUObjectArray* UObject::GObjects = nullptr;
//---------------------------------------------------------------------------
bool FWeakObjectPtr::IsValid() const
{
	if (ObjectSerialNumber == 0)
	{
		return false;
	}
	if (ObjectIndex < 0)
	{
		return false;
	}
	auto ObjectItem = UObject::GetGlobalObjects().GetItemByIndex(ObjectIndex);
	if (!ObjectItem)
	{
		return false;
	}
	if (!SerialNumbersMatch(ObjectItem))
	{
		return false;
	}
	return !(ObjectItem->IsUnreachable() || ObjectItem->IsPendingKill());
}
//---------------------------------------------------------------------------
UObject* FWeakObjectPtr::Get() const
{
	if (IsValid())
	{
		auto ObjectItem = UObject::GetGlobalObjects().GetItemByIndex(ObjectIndex);
		if (ObjectItem)
		{
			return ObjectItem->Object;
		}
	}
	return nullptr;
}
//---------------------------------------------------------------------------)";
	}
};

Generator _generator;
IGenerator* generator = &_generator;
