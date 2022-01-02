#include "Flags.hpp"

#include <vector>
#include <sstream>
#include <iterator>

std::string StringifyFlags(const UEPropertyFlags flags)
{
	std::vector<const char*> buffer;

	if (flags & UEPropertyFlags::CPF_Edit) { buffer.push_back("CPF_Edit"); }
	if (flags & UEPropertyFlags::CPF_ConstParm) { buffer.push_back("CPF_ConstParm"); }
	if (flags & UEPropertyFlags::CPF_BlueprintVisible) { buffer.push_back("CPF_BlueprintVisible"); }
	if (flags & UEPropertyFlags::CPF_ExportObject) { buffer.push_back("CPF_ExportObject"); }
	if (flags & UEPropertyFlags::CPF_BlueprintReadOnly) { buffer.push_back("CPF_BlueprintReadOnly"); }
	if (flags & UEPropertyFlags::CPF_Net) { buffer.push_back("CPF_Net"); }
	if (flags & UEPropertyFlags::CPF_EditFixedSize) { buffer.push_back("CPF_EditFixedSize"); }
	if (flags & UEPropertyFlags::CPF_Parm) { buffer.push_back("CPF_Parm"); }
	if (flags & UEPropertyFlags::CPF_OutParm) { buffer.push_back("CPF_OutParm"); }
	if (flags & UEPropertyFlags::CPF_ZeroConstructor) { buffer.push_back("CPF_ZeroConstructor"); }
	if (flags & UEPropertyFlags::CPF_ReturnParm) { buffer.push_back("CPF_ReturnParm"); }
	if (flags & UEPropertyFlags::CPF_DisableEditOnTemplate) { buffer.push_back("CPF_DisableEditOnTemplate"); }
	if (flags & UEPropertyFlags::CPF_Transient) { buffer.push_back("CPF_Transient"); }
	if (flags & UEPropertyFlags::CPF_Config) { buffer.push_back("CPF_Config"); }
	if (flags & UEPropertyFlags::CPF_DisableEditOnInstance) { buffer.push_back("CPF_DisableEditOnInstance"); }
	if (flags & UEPropertyFlags::CPF_EditConst) { buffer.push_back("CPF_EditConst"); }
	if (flags & UEPropertyFlags::CPF_GlobalConfig) { buffer.push_back("CPF_GlobalConfig"); }
	if (flags & UEPropertyFlags::CPF_InstancedReference) { buffer.push_back("CPF_InstancedReference"); }
	if (flags & UEPropertyFlags::CPF_DuplicateTransient) { buffer.push_back("CPF_DuplicateTransient"); }
	if (flags & UEPropertyFlags::CPF_SubobjectReference) { buffer.push_back("CPF_SubobjectReference"); }
	if (flags & UEPropertyFlags::CPF_SaveGame) { buffer.push_back("CPF_SaveGame"); }
	if (flags & UEPropertyFlags::CPF_NoClear) { buffer.push_back("CPF_NoClear"); }
	if (flags & UEPropertyFlags::CPF_ReferenceParm) { buffer.push_back("CPF_ReferenceParm"); }
	if (flags & UEPropertyFlags::CPF_BlueprintAssignable) { buffer.push_back("CPF_BlueprintAssignable"); }
	if (flags & UEPropertyFlags::CPF_Deprecated) { buffer.push_back("CPF_Deprecated"); }
	if (flags & UEPropertyFlags::CPF_IsPlainOldData) { buffer.push_back("CPF_IsPlainOldData"); }
	if (flags & UEPropertyFlags::CPF_RepSkip) { buffer.push_back("CPF_RepSkip"); }
	if (flags & UEPropertyFlags::CPF_RepNotify) { buffer.push_back("CPF_RepNotify"); }
	if (flags & UEPropertyFlags::CPF_Interp) { buffer.push_back("CPF_Interp"); }
	if (flags & UEPropertyFlags::CPF_NonTransactional) { buffer.push_back("CPF_NonTransactional"); }
	if (flags & UEPropertyFlags::CPF_EditorOnly) { buffer.push_back("CPF_EditorOnly"); }
	if (flags & UEPropertyFlags::CPF_NoDestructor) { buffer.push_back("CPF_NoDestructor"); }
	if (flags & UEPropertyFlags::CPF_AutoWeak) { buffer.push_back("CPF_AutoWeak"); }
	if (flags & UEPropertyFlags::CPF_ContainsInstancedReference) { buffer.push_back("CPF_ContainsInstancedReference"); }
	if (flags & UEPropertyFlags::CPF_AssetRegistrySearchable) { buffer.push_back("CPF_AssetRegistrySearchable"); }
	if (flags & UEPropertyFlags::CPF_SimpleDisplay) { buffer.push_back("CPF_SimpleDisplay"); }
	if (flags & UEPropertyFlags::CPF_AdvancedDisplay) { buffer.push_back("CPF_AdvancedDisplay"); }
	if (flags & UEPropertyFlags::CPF_Protected) { buffer.push_back("CPF_Protected"); }
	if (flags & UEPropertyFlags::CPF_BlueprintCallable) { buffer.push_back("CPF_BlueprintCallable"); }
	if (flags & UEPropertyFlags::CPF_BlueprintAuthorityOnly) { buffer.push_back("CPF_BlueprintAuthorityOnly"); }
	if (flags & UEPropertyFlags::CPF_TextExportTransient) { buffer.push_back("CPF_TextExportTransient"); }
	if (flags & UEPropertyFlags::CPF_NonPIEDuplicateTransient) { buffer.push_back("CPF_NonPIEDuplicateTransient"); }
	if (flags & UEPropertyFlags::CPF_ExposeOnSpawn) { buffer.push_back("CPF_ExposeOnSpawn"); }
	if (flags & UEPropertyFlags::CPF_PersistentInstance) { buffer.push_back("CPF_PersistentInstance"); }
	if (flags & UEPropertyFlags::CPF_UObjectWrapper) { buffer.push_back("CPF_UObjectWrapper"); }
	if (flags & UEPropertyFlags::CPF_HasGetValueTypeHash) { buffer.push_back("CPF_HasGetValueTypeHash"); }
	if (flags & UEPropertyFlags::CPF_NativeAccessSpecifierPublic) { buffer.push_back("CPF_NativeAccessSpecifierPublic"); }
	if (flags & UEPropertyFlags::CPF_NativeAccessSpecifierProtected) { buffer.push_back("CPF_NativeAccessSpecifierProtected"); }
	if (flags & UEPropertyFlags::CPF_NativeAccessSpecifierPrivate) { buffer.push_back("CPF_NativeAccessSpecifierPrivate"); }

	switch (buffer.size())
	{
		case 0:
			return std::string();
		case 1:
			return std::string(buffer[0]);
		default:
			std::ostringstream os;
			std::copy(buffer.begin(), buffer.end() - 1, std::ostream_iterator<const char*>(os, ", "));
			os << *buffer.rbegin();
			return os.str();
	}
}

std::string StringifyFlags(const UEFunctionFlags flags)
{
	std::vector<const char*> buffer;

	if (flags & UEFunctionFlags::FUNC_Final) { buffer.push_back("FUNC_Final"); }
	if (flags & UEFunctionFlags::FUNC_RequiredAPI) { buffer.push_back("FUNC_RequiredAPI"); }
	if (flags & UEFunctionFlags::FUNC_BlueprintAuthorityOnly) { buffer.push_back("FUNC_BlueprintAuthorityOnly"); }
	if (flags & UEFunctionFlags::FUNC_BlueprintCosmetic) { buffer.push_back("FUNC_BlueprintCosmetic"); }
	if (flags & UEFunctionFlags::FUNC_Net) { buffer.push_back("FUNC_Net"); }
	if (flags & UEFunctionFlags::FUNC_NetReliable) { buffer.push_back("FUNC_NetReliable"); }
	if (flags & UEFunctionFlags::FUNC_NetRequest) { buffer.push_back("FUNC_NetRequest"); }
	if (flags & UEFunctionFlags::FUNC_Exec) { buffer.push_back("FUNC_Exec"); }
	if (flags & UEFunctionFlags::FUNC_Native) { buffer.push_back("FUNC_Native"); }
	if (flags & UEFunctionFlags::FUNC_Event) { buffer.push_back("FUNC_Event"); }
	if (flags & UEFunctionFlags::FUNC_NetResponse) { buffer.push_back("FUNC_NetResponse"); }
	if (flags & UEFunctionFlags::FUNC_Static) { buffer.push_back("FUNC_Static"); }
	if (flags & UEFunctionFlags::FUNC_NetMulticast) { buffer.push_back("FUNC_NetMulticast"); }
	if (flags & UEFunctionFlags::FUNC_MulticastDelegate) { buffer.push_back("FUNC_MulticastDelegate"); }
	if (flags & UEFunctionFlags::FUNC_Public) { buffer.push_back("FUNC_Public"); }
	if (flags & UEFunctionFlags::FUNC_Private) { buffer.push_back("FUNC_Private"); }
	if (flags & UEFunctionFlags::FUNC_Protected) { buffer.push_back("FUNC_Protected"); }
	if (flags & UEFunctionFlags::FUNC_Delegate) { buffer.push_back("FUNC_Delegate"); }
	if (flags & UEFunctionFlags::FUNC_NetServer) { buffer.push_back("FUNC_NetServer"); }
	if (flags & UEFunctionFlags::FUNC_HasOutParms) { buffer.push_back("FUNC_HasOutParms"); }
	if (flags & UEFunctionFlags::FUNC_HasDefaults) { buffer.push_back("FUNC_HasDefaults"); }
	if (flags & UEFunctionFlags::FUNC_NetClient) { buffer.push_back("FUNC_NetClient"); }
	if (flags & UEFunctionFlags::FUNC_DLLImport) { buffer.push_back("FUNC_DLLImport"); }
	if (flags & UEFunctionFlags::FUNC_BlueprintCallable) { buffer.push_back("FUNC_BlueprintCallable"); }
	if (flags & UEFunctionFlags::FUNC_BlueprintEvent) { buffer.push_back("FUNC_BlueprintEvent"); }
	if (flags & UEFunctionFlags::FUNC_BlueprintPure) { buffer.push_back("FUNC_BlueprintPure"); }
	if (flags & UEFunctionFlags::FUNC_Const) { buffer.push_back("FUNC_Const"); }
	if (flags & UEFunctionFlags::FUNC_NetValidate) { buffer.push_back("FUNC_NetValidate"); }

	switch (buffer.size())
	{
	case 0:
		return std::string();
	case 1:
		return std::string(buffer[0]);
	default:
		std::ostringstream os;
		std::copy(buffer.begin(), buffer.end() - 1, std::ostream_iterator<const char*>(os, ", "));
		os << *buffer.rbegin();
		return os.str();
	}
}