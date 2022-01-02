// Unreal Engine SDK Generator
// OldSchoolHack Community Version
// Credits to KN4CK3R for the original code

#include <windows.h>

#include <fstream>
#include <unordered_map>
#include <chrono>
#include <filesystem>
#include <bitset>
namespace fs = std::filesystem;
#include "cpplinq.hpp"

#include "Logger.hpp"

#include "IGenerator.hpp"
#include "ObjectsStore.hpp"
#include "NamesStore.hpp"
#include "Package.hpp"
#include "NameValidator.hpp"
#include "PrintHelper.hpp"
#include "ExceptionHandler.hpp"

extern IGenerator* generator;

/// <summary>
/// Dumps the names to file.
/// </summary>
/// <param name="path">The path where to create the dumps.</param>
void DumpNames(const fs::path& path)
{
	std::ofstream o(path / "NamesDump.txt");
	//tfm::format(o, "Address: 0x%P\n\n", NamesStore::GetAddress());

	for (auto name : NamesStore())
	{
		tfm::format(o, "[%06i] %s\n", name.Index, name.Name);
	}
}

/// <summary>
/// Dumps the objects to file.
/// </summary>
/// <param name="path">The path where to create the dumps.</param>
void DumpObjects(const fs::path& path)
{
	std::ofstream o(path / "ObjectsDump.txt");
	//tfm::format(o, "Address: 0x%P\n\n", ObjectsStore::GetAddress());

	tfm::format(o,
		"[ idx  ] | Name                                                                                                 instance           | vtable\n"
		"---------+-------------------------------------------------------------------------------------------------------------------------+-------------------\n");
	//  "[000001] | Class CoreUObject.Object                                                                             0x000001C80CF61E80 | 0x000001C80CF61E80"

	for (auto obj : ObjectsStore())
	{
		tfm::format(o, "[%06i] | %-100s 0x%P | 0x%P\n", obj.GetIndex(), obj.GetFullName(), obj.GetAddress(), *static_cast<void**>(obj.GetAddress()));
	}
}


void DumpCoreUObjectSizes(const fs::path& path)
{
	UEObject corePackage;

	for (auto obj : ObjectsStore())
	{
		const auto package = obj.GetPackageObject();

		if (package.IsValid() && package.GetFullName() == "Package CoreUObject")
		{
			corePackage = package;
		}
	}

	std::ofstream o(path / "CoreUObjectInfo.txt");

	if (!corePackage.IsValid()) return;

	for (auto obj : ObjectsStore())
	{
		if (obj.GetPackageObject() == corePackage && obj.IsA<UEClass>())
		{
			auto uclass = obj.Cast<UEClass>();
			
			if (obj.GetName().find("Default__") == 0) continue;

			tfm::format(o, "class %-35s", uclass.GetNameCPP());
			
			auto superclass = uclass.GetSuper();

			if (superclass.IsValid())
			{
				tfm::format(o, ": public %-25s ", superclass.GetNameCPP());

				tfm::format(o, "// 0x%02X (0x%02X - 0x%02X) \n", uclass.GetPropertySize() - superclass.GetPropertySize(), superclass.GetPropertySize(), uclass.GetPropertySize());
			}
			else
			{
				tfm::format(o, "%-35s// 0x%02X (0x00 - 0x%02X) \n", "", uclass.GetPropertySize(), uclass.GetPropertySize());
			}
		}
	}
	

} 


/// <summary>
/// Generates the sdk header.
/// </summary>
/// <param name="path">The path where to create the sdk header.</param>
/// <param name="processedObjects">The list of processed objects.</param>
/// <param name="packageOrder">The package order info.</param>
void SaveSDKHeader(const fs::path& path, const std::unordered_map<UEObject, bool>& processedObjects, const std::vector<Package>& packages)
{
	std::ofstream os(path / "SDK.hpp");

	os << "#pragma once\n\n"
		<< tfm::format("// %s (%s) SDK\n\n", generator->GetGameName(), generator->GetGameVersion());

	//include the basics
	{
		{
			std::ofstream os2(path / "SDK" / tfm::format("%s_Basic.hpp", generator->GetGameNameShort()));
			
			std::vector<std::string> includes{ { "<locale>" }, { "<unordered_set>" }, { "<string>" } };

			auto&& generatorIncludes = generator->GetIncludes();
			includes.insert(includes.end(), std::begin(generatorIncludes), std::end(generatorIncludes));

			PrintFileHeader(os2, includes, true);
			
			os2 << generator->GetBasicDeclarations() << "\n";

			PrintFileFooter(os2);

			os << "\n#include \"SDK/" << tfm::format("%s_Basic.hpp", generator->GetGameNameShort()) << "\"\n";
		}
		{
			std::ofstream os2(path / "SDK" / tfm::format("%s_Basic.cpp", generator->GetGameNameShort()));

			PrintFileHeader(os2, { "../SDK.hpp" }, false);

			os2 << generator->GetBasicDefinitions() << "\n";

			PrintFileFooter(os2);
		}
	}

	using namespace cpplinq;

	//check for missing structs
	const auto missing = from(processedObjects) >> where([](auto&& kv) { return kv.second == false; });
	if (missing >> any())
	{
		std::ofstream os2(path / "SDK" / tfm::format("%s_MISSING.hpp", generator->GetGameNameShort()));

		PrintFileHeader(os2, true);

		for (auto&& s : missing >> select([](auto&& kv) { return kv.first.Cast<UEStruct>(); }) >> experimental::container())
		{
			os2 << "// " << s.GetFullName() << "\n// ";
			os2 << tfm::format("0x%04X\n", s.GetPropertySize());

			os2 << "struct " << MakeValidName(s.GetNameCPP()) << "\n{\n";
			os2 << "\tunsigned char UnknownData[0x" << tfm::format("%X", s.GetPropertySize()) << "];\n};\n\n";
		}

		PrintFileFooter(os2);

		os << "\n#include \"SDK/" << tfm::format("%s_MISSING.hpp", generator->GetGameNameShort()) << "\"\n";
	}

	os << "\n";

	for (auto&& package : packages)
	{
		os << R"(#include "SDK/)" << GenerateFileName(FileContentType::Classes, package) << "\"\n";
	}
}

/// <summary>
/// Process the packages.
/// </summary>
/// <param name="path">The path where to create the package files.</param>
void ProcessPackages(const fs::path& path)
{
	using namespace cpplinq;

	const auto sdkPath = path / "SDK";
	fs::create_directories(sdkPath);
	
	std::vector<Package> packages;

	std::unordered_map<UEObject, bool> processedObjects;

	auto packageObjects = from(ObjectsStore())
		>> select([](auto&& o) { return o.GetPackageObject(); })
		>> where([](auto&& o) { return o.IsValid(); })
		>> distinct()
		>> to_vector();

	std::ofstream fndmp(path / "FunctionsDump.txt", std::ostream::out | std::ostream::trunc);
	fndmp.close();

	for (auto obj : packageObjects)
	{
		Package package(obj);

		package.Process(processedObjects);
		package.DumpFunctions(path);
		if (package.Save(sdkPath))
		{
			packages.emplace_back(std::move(package));
		}
	}

	SaveSDKHeader(path, processedObjects, packages);
}

DWORD WINAPI OnAttach(LPVOID lpParameter)
{
	// setup logger
	fs::path outputDirectory(generator->GetOutputDirectory());
	if (!outputDirectory.is_absolute())
	{
		char buffer[2048];
		if (GetModuleFileNameA(static_cast<HMODULE>(lpParameter), buffer, sizeof(buffer)) == 0)
		{
			Logger::Log("Error: GetModuleFileName failed");
			if(generator->ShouldShowMessageBox())
			{
				MessageBoxA(nullptr, "GetModuleFileName failed", "Error", 0);
			}
			return -1;
		}

		outputDirectory = fs::path(buffer).remove_filename() / outputDirectory;
	}

	outputDirectory /= (generator->GetGameNameShort() + "_" + generator->GetGameVersion());
	fs::create_directories(outputDirectory);

	std::ofstream log(outputDirectory / "Generator.log");
	Logger::SetStream(&log);

	if(generator->EnableVectoredExceptionHandler())
	{
		ExceptionHandler::Init();
	}

	Logger::Log("Process Baseaddress: 0x%P", (void*)GetModuleHandleW(NULL));

	if (!ObjectsStore::Initialize())
	{
		Logger::Log("Error: ObjectsStore::Initialize failed");
		if(generator->ShouldShowMessageBox())
		{
			MessageBoxA(nullptr, "ObjectsStore::Initialize failed", "Error", 0);
		}
		return -1;
	}
	if (!NamesStore::Initialize())
	{
		Logger::Log("Error: NamesStore::Initialize failed");
		if(generator->ShouldShowMessageBox())
		{
			MessageBoxA(nullptr, "NamesStore::Initialize failed", "Error", 0);
		}
		return -1;
	}

	if (!generator->Initialize(lpParameter))
	{
		Logger::Log("Error: Failed to initialize generator!");
		if(generator->ShouldShowMessageBox())
		{
			MessageBoxA(nullptr, "Failed to initialize generator!\nSee Logfile for details.", "Error", 0);	
		}
		return -1;
	}

	if (generator->ShouldDumpArrays())
	{
		Logger::Log("Dumping GNames...");
		DumpNames(outputDirectory);
	}


	if (generator->ShouldDumpArrays())
	{
		Logger::Log("Dumping GObjects...");
		DumpObjects(outputDirectory);
	}

	//return 0;

	Logger::Log("Dumping CoreUobject Infos...");
	DumpCoreUObjectSizes(outputDirectory);

	//return 0;

	fs::create_directories(outputDirectory);

	const auto begin = std::chrono::system_clock::now();

	ProcessPackages(outputDirectory);

	Logger::Log("\nFinished, took %d seconds.", std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - begin).count());

	Logger::SetStream(nullptr);

	if(generator->ShouldShowMessageBox())
	{
		MessageBoxA(nullptr, "Finished!", "Info", 0);
	}

	if (generator->EnableVectoredExceptionHandler())
	{
		ExceptionHandler::Destroy();
	}

	return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);

		const auto threadHandle = CreateThread(nullptr, 0, [](LPVOID param) -> DWORD 
		{
			const auto returnValue = OnAttach(param);
			if(param)
			{
				Sleep(200);
				FreeLibraryAndExitThread(static_cast<HMODULE>(param), returnValue);
			}
			else
			{
				Logger::Log("hModule is nullptr");
			}
			return returnValue;

		}, hModule, 0, nullptr);
		if(threadHandle)
		{
			CloseHandle(threadHandle);
		}

		return TRUE;
	}

	return FALSE;
}
