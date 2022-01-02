#include "Flags.hpp"

#include <vector>
#include <sstream>
#include <iterator>

std::string StringifyFlags(const UEPropertyFlags flags)
{
	std::vector<const char*> buffer;

	if (flags & UEPropertyFlags::CPF_Edit) { buffer.push_back("CPF_Edit"); }
	if (flags & UEPropertyFlags::CPF_Const) { buffer.push_back("CPF_Const"); }
	if (flags & UEPropertyFlags::CPF_Input) { buffer.push_back("CPF_Input"); }
	if (flags & UEPropertyFlags::CPF_ExportObject) { buffer.push_back("CPF_ExportObject"); }
	if (flags & UEPropertyFlags::CPF_OptionalParm) { buffer.push_back("CPF_OptionalParm"); }
	if (flags & UEPropertyFlags::CPF_Net) { buffer.push_back("CPF_Net"); }
	if (flags & UEPropertyFlags::CPF_EditFixedSize) { buffer.push_back("CPF_EditFixedSize"); }
	if (flags & UEPropertyFlags::CPF_Parm) { buffer.push_back("CPF_Parm"); }
	if (flags & UEPropertyFlags::CPF_OutParm) { buffer.push_back("CPF_OutParm"); }
	if (flags & UEPropertyFlags::CPF_SkipParm) { buffer.push_back("CPF_SkipParm"); }
	if (flags & UEPropertyFlags::CPF_ReturnParm) { buffer.push_back("CPF_ReturnParm"); }
	if (flags & UEPropertyFlags::CPF_CoerceParm) { buffer.push_back("CPF_CoerceParm"); }
	if (flags & UEPropertyFlags::CPF_Native) { buffer.push_back("CPF_Native"); }
	if (flags & UEPropertyFlags::CPF_Transient) { buffer.push_back("CPF_Transient"); }
	if (flags & UEPropertyFlags::CPF_Config) { buffer.push_back("CPF_Config"); }
	if (flags & UEPropertyFlags::CPF_Localized) { buffer.push_back("CPF_Localized"); }
	if (flags & UEPropertyFlags::CPF_EditConst) { buffer.push_back("CPF_EditConst"); }
	if (flags & UEPropertyFlags::CPF_GlobalConfig) { buffer.push_back("CPF_GlobalConfig"); }
	if (flags & UEPropertyFlags::CPF_Component) { buffer.push_back("CPF_Component"); }
	if (flags & UEPropertyFlags::CPF_AlwaysInit) { buffer.push_back("CPF_AlwaysInit"); }
	if (flags & UEPropertyFlags::CPF_DuplicateTransient) { buffer.push_back("CPF_DuplicateTransient"); }
	if (flags & UEPropertyFlags::CPF_NeedCtorLink) { buffer.push_back("CPF_NeedCtorLink"); }
	if (flags & UEPropertyFlags::CPF_NoExport) { buffer.push_back("CPF_NoExport"); }
	if (flags & UEPropertyFlags::CPF_NoImport) { buffer.push_back("CPF_NoImport"); }
	if (flags & UEPropertyFlags::CPF_NoClear) { buffer.push_back("CPF_NoClear"); }
	if (flags & UEPropertyFlags::CPF_EditInline) { buffer.push_back("CPF_EditInline"); }
	if (flags & UEPropertyFlags::CPF_EditInlineUse) { buffer.push_back("CPF_EditInlineUse"); }
	if (flags & UEPropertyFlags::CPF_Deprecated) { buffer.push_back("CPF_Deprecated"); }
	if (flags & UEPropertyFlags::CPF_DataBinding) { buffer.push_back("CPF_DataBinding"); }
	if (flags & UEPropertyFlags::CPF_SerializeText) { buffer.push_back("CPF_SerializeText"); }
	if (flags & UEPropertyFlags::CPF_RepNotify) { buffer.push_back("CPF_RepNotify"); }
	if (flags & UEPropertyFlags::CPF_Interp) { buffer.push_back("CPF_Interp"); }
	if (flags & UEPropertyFlags::CPF_NonTransactional) { buffer.push_back("CPF_NonTransactional"); }
	if (flags & UEPropertyFlags::CPF_EditorOnly) { buffer.push_back("CPF_EditorOnly"); }
	if (flags & UEPropertyFlags::CPF_NotForConsole) { buffer.push_back("CPF_NotForConsole"); }
	if (flags & UEPropertyFlags::CPF_RepRetry) { buffer.push_back("CPF_RepRetry"); }
	if (flags & UEPropertyFlags::CPF_PrivateWrite) { buffer.push_back("CPF_PrivateWrite"); }
	if (flags & UEPropertyFlags::CPF_ProtectedWrite) { buffer.push_back("CPF_ProtectedWrite"); }
	if (flags & UEPropertyFlags::CPF_ArchetypeProperty) { buffer.push_back("CPF_ArchetypeProperty"); }
	if (flags & UEPropertyFlags::CPF_EditHide) { buffer.push_back("CPF_EditHide"); }
	if (flags & UEPropertyFlags::CPF_EditTextBox) { buffer.push_back("CPF_EditTextBox"); }
	if (flags & UEPropertyFlags::CPF_CrossLevelPassive) { buffer.push_back("CPF_CrossLevelPassive"); }
	if (flags & UEPropertyFlags::CPF_CrossLevelActive) { buffer.push_back("CPF_CrossLevelActive"); }

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
	if (flags & UEFunctionFlags::FUNC_Defined) { buffer.push_back("FUNC_Defined"); }
	if (flags & UEFunctionFlags::FUNC_Iterator) { buffer.push_back("FUNC_Iterator"); }
	if (flags & UEFunctionFlags::FUNC_Latent) { buffer.push_back("FUNC_Latent"); }
	if (flags & UEFunctionFlags::FUNC_PreOperator) { buffer.push_back("FUNC_PreOperator"); }
	if (flags & UEFunctionFlags::FUNC_Singular) { buffer.push_back("FUNC_Singular"); }
	if (flags & UEFunctionFlags::FUNC_Net) { buffer.push_back("FUNC_Net"); }
	if (flags & UEFunctionFlags::FUNC_NetReliable) { buffer.push_back("FUNC_NetReliable"); }
	if (flags & UEFunctionFlags::FUNC_Simulated) { buffer.push_back("FUNC_Simulated"); }
	if (flags & UEFunctionFlags::FUNC_Exec) { buffer.push_back("FUNC_Exec"); }
	if (flags & UEFunctionFlags::FUNC_Native) { buffer.push_back("FUNC_Native"); }
	if (flags & UEFunctionFlags::FUNC_Event) { buffer.push_back("FUNC_Event"); }
	if (flags & UEFunctionFlags::FUNC_Operator) { buffer.push_back("FUNC_Operator"); }
	if (flags & UEFunctionFlags::FUNC_Static) { buffer.push_back("FUNC_Static"); }
	if (flags & UEFunctionFlags::FUNC_HasOptionalParms) { buffer.push_back("FUNC_HasOptionalParms"); }
	if (flags & UEFunctionFlags::FUNC_Const) { buffer.push_back("FUNC_Const"); }
	if (flags & UEFunctionFlags::FUNC_Public) { buffer.push_back("FUNC_Public"); }
	if (flags & UEFunctionFlags::FUNC_Private) { buffer.push_back("FUNC_Private"); }
	if (flags & UEFunctionFlags::FUNC_Protected) { buffer.push_back("FUNC_Protected"); }
	if (flags & UEFunctionFlags::FUNC_Delegate) { buffer.push_back("FUNC_Delegate"); }
	if (flags & UEFunctionFlags::FUNC_NetServer) { buffer.push_back("FUNC_NetServer"); }
	if (flags & UEFunctionFlags::FUNC_HasOutParms) { buffer.push_back("FUNC_HasOutParms"); }
	if (flags & UEFunctionFlags::FUNC_HasDefaults) { buffer.push_back("FUNC_HasDefaults"); }
	if (flags & UEFunctionFlags::FUNC_NetClient) { buffer.push_back("FUNC_NetClient"); }
	if (flags & UEFunctionFlags::FUNC_DLLImport) { buffer.push_back("FUNC_DLLImport"); }
	if (flags & UEFunctionFlags::FUNC_K2Call) { buffer.push_back("FUNC_K2Call"); }
	if (flags & UEFunctionFlags::FUNC_K2Override) { buffer.push_back("FUNC_K2Override"); }
	if (flags & UEFunctionFlags::FUNC_K2Pure) { buffer.push_back("FUNC_K2Pure"); }

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