#pragma once

#include <type_traits>
#include <string>

enum class UEPropertyFlags : uint64_t
{
	CPF_Edit							= 0x0000000000000001,		// Property is user-settable in the editor.
	CPF_Const							= 0x0000000000000002,		// Actor's property always matches class's default actor property.
	CPF_Input							= 0x0000000000000004,		// Variable is writable by the input system.
	CPF_ExportObject					= 0x0000000000000008,		// Object can be exported with actor.
	CPF_OptionalParm					= 0x0000000000000010,		// Optional parameter (if CPF_Param is set).
	CPF_Net								= 0x0000000000000020,		// Property is relevant to network replication.
	CPF_EditFixedSize					= 0x0000000000000040,		// Indicates that elements of an array can be modified, but its size cannot be changed.
	CPF_Parm							= 0x0000000000000080,		// Function/When call parameter.
	CPF_OutParm							= 0x0000000000000100,		// Value is copied out after function call.
	CPF_SkipParm						= 0x0000000000000200,		// Property is a short-circuitable evaluation function parm.
	CPF_ReturnParm						= 0x0000000000000400,		// Return value.
	CPF_CoerceParm						= 0x0000000000000800,		// Coerce args into this function parameter.
	CPF_Native							= 0x0000000000001000,		// Property is native: C++ code is responsible for serializing it.
	CPF_Transient						= 0x0000000000002000,		// Property is transient: shouldn't be saved, zero-filled at load time.
	CPF_Config							= 0x0000000000004000,		// Property should be loaded/saved as permanent profile.
	CPF_Localized						= 0x0000000000008000,		// Property should be loaded as localizable text.
	CPF_EditConst						= 0x0000000000020000,		// Property is uneditable in the editor.
	CPF_GlobalConfig					= 0x0000000000040000,		// Load config from base class, not subclass.
	CPF_Component						= 0x0000000000080000,		// Property containts component references.
	CPF_AlwaysInit						= 0x0000000000100000,		// Property should never be exported as NoInit	(@todo - this doesn't need to be a property flag...only used during make)
	CPF_DuplicateTransient				= 0x0000000000200000,		// Property should always be reset to the default value during any type of duplication (copy/paste, binary duplication, etc.)
	CPF_NeedCtorLink					= 0x0000000000400000,		// Fields need construction/destruction.
	CPF_NoExport						= 0x0000000000800000,		// Property should not be exported to the native class header file.
	CPF_NoImport						= 0x0000000001000000,		// Property should not be imported when creating an object from text (copy/paste)
	CPF_NoClear							= 0x0000000002000000,		// Hide clear (and browse) button.
	CPF_EditInline						= 0x0000000004000000,		// Edit this object reference inline.
	CPF_EditInlineUse					= 0x0000000010000000,		// EditInline with Use button.
	CPF_Deprecated  					= 0x0000000020000000,		// Property is deprecated.  Read it from an archive, but don't save it.
	CPF_DataBinding						= 0x0000000040000000,		// Indicates that this property should be exposed to data stores
	CPF_SerializeText					= 0x0000000080000000,		// Native property should be serialized as text (ImportText, ExportText)
	CPF_RepNotify						= 0x0000000100000000,		// Notify actors when a property is replicated
	CPF_Interp							= 0x0000000200000000,		// interpolatable property for use with matinee
	CPF_NonTransactional				= 0x0000000400000000,		// Property isn't transacted
	CPF_EditorOnly						= 0x0000000800000000,		// Property should only be loaded in the editor
	CPF_NotForConsole					= 0x0000001000000000,		// Property should not be loaded on console (or be a console cooker commandlet)
	CPF_RepRetry						= 0x0000002000000000,		// retry replication of this property if it fails to be fully sent (e.g. object references not yet available to serialize over the network)
	CPF_PrivateWrite					= 0x0000004000000000,		// property is const outside of the class it was declared in
	CPF_ProtectedWrite					= 0x0000008000000000,		// property is const outside of the class it was declared in and subclasses
	CPF_ArchetypeProperty				= 0x0000010000000000,		// property should be ignored by archives which have ArIgnoreArchetypeRef set
	CPF_EditHide						= 0x0000020000000000,		// property should never be shown in a properties window.
	CPF_EditTextBox						= 0x0000040000000000,		// property can be edited using a text dialog box.
	CPF_CrossLevelPassive				= 0x0000100000000000,		// property can point across levels, and will be serialized properly, but assumes it's target exists in-game (non-editor)
	CPF_CrossLevelActive				= 0x0000200000000000		// property can point across levels, and will be serialized properly, and will be updated when the target is streamed in/out
};

inline bool operator&(UEPropertyFlags lhs, UEPropertyFlags rhs)
{
	return (static_cast<std::underlying_type_t<UEPropertyFlags>>(lhs) & static_cast<std::underlying_type_t<UEPropertyFlags>>(rhs)) == static_cast<std::underlying_type_t<UEPropertyFlags>>(rhs);
}

std::string StringifyFlags(const UEPropertyFlags flags);

enum class UEFunctionFlags : uint32_t
{
	FUNC_Final							= 0x00000001,				// Function is final (prebindable, non-overridable function).
	FUNC_Defined						= 0x00000002,				// Function has been defined (not just declared).
	FUNC_Iterator						= 0x00000004,				// Function is an iterator.
	FUNC_Latent							= 0x00000008,				// Function is a latent state function.
	FUNC_PreOperator					= 0x00000010,				// Unary operator is a prefix operator.
	FUNC_Singular						= 0x00000020,				// Function cannot be reentered.
	FUNC_Net							= 0x00000040,				// Function is network-replicated.
	FUNC_NetReliable					= 0x00000080,				// Function should be sent reliably on the network.
	FUNC_Simulated						= 0x00000100,				// Function executed on the client side.
	FUNC_Exec							= 0x00000200,				// Executable from command line.
	FUNC_Native							= 0x00000400,				// Native function.
	FUNC_Event							= 0x00000800,				// Event function.
	FUNC_Operator						= 0x00001000,				// Operator function.
	FUNC_Static							= 0x00002000,				// Static function.
	FUNC_HasOptionalParms				= 0x00004000,				// Function has optional parameters
	FUNC_Const							= 0x00008000,				// Function doesn't modify this object.
	FUNC_Public							= 0x00020000,				// Function is accessible in all classes (if overridden, parameters much remain unchanged).
	FUNC_Private						= 0x00040000,				// Function is accessible only in the class it is defined in (cannot be overriden, but function name may be reused in subclasses.  IOW: if overridden, parameters don't need to match, and Super.Func() cannot be accessed since it's private.)
	FUNC_Protected						= 0x00080000,				// Function is accessible only in the class it is defined in and subclasses (if overridden, parameters much remain unchanged).
	FUNC_Delegate						= 0x00100000,				// Function is actually a delegate.
	FUNC_NetServer						= 0x00200000,				// Function is executed on servers (set by replication code if passes check)
	FUNC_HasOutParms					= 0x00400000,				// function has out (pass by reference) parameters
	FUNC_HasDefaults					= 0x00800000,				// function has structs that contain defaults
	FUNC_NetClient						= 0x01000000,				// function is executed on clients
	FUNC_DLLImport						= 0x02000000,				// function is imported from a DLL
	FUNC_K2Call							= 0x04000000,				// function can be called from K2
	FUNC_K2Override						= 0x08000000,				// function can be overriden/implemented from K2
	FUNC_K2Pure							= 0x10000000,				// function can be called from K2, and is also pure (produces no side effects). If you set this, you should set K2call as well.
};

inline bool operator&(UEFunctionFlags lhs, UEFunctionFlags rhs)
{
	return (static_cast<std::underlying_type_t<UEFunctionFlags>>(lhs) & static_cast<std::underlying_type_t<UEFunctionFlags>>(rhs)) == static_cast<std::underlying_type_t<UEFunctionFlags>>(rhs);
}

std::string StringifyFlags(const UEFunctionFlags flags);
