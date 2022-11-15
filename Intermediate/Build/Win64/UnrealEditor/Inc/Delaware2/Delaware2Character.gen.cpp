// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Delaware2/Delaware2Character.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeDelaware2Character() {}
// Cross Module References
	DELAWARE2_API UClass* Z_Construct_UClass_ADelaware2Character_NoRegister();
	DELAWARE2_API UClass* Z_Construct_UClass_ADelaware2Character();
	ENGINE_API UClass* Z_Construct_UClass_ACharacter();
	UPackage* Z_Construct_UPackage__Script_Delaware2();
	ENGINE_API UClass* Z_Construct_UClass_UCameraComponent_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_USpringArmComponent_NoRegister();
// End Cross Module References
	void ADelaware2Character::StaticRegisterNativesADelaware2Character()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ADelaware2Character);
	UClass* Z_Construct_UClass_ADelaware2Character_NoRegister()
	{
		return ADelaware2Character::StaticClass();
	}
	struct Z_Construct_UClass_ADelaware2Character_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TopDownCameraComponent_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_TopDownCameraComponent;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_CameraBoom_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_CameraBoom;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ADelaware2Character_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_ACharacter,
		(UObject* (*)())Z_Construct_UPackage__Script_Delaware2,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADelaware2Character_Statics::Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "Delaware2Character.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Delaware2Character.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADelaware2Character_Statics::NewProp_TopDownCameraComponent_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Camera" },
		{ "Comment", "/** Top down camera */" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Delaware2Character.h" },
		{ "ToolTip", "Top down camera" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADelaware2Character_Statics::NewProp_TopDownCameraComponent = { "TopDownCameraComponent", nullptr, (EPropertyFlags)0x00400000000a001d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ADelaware2Character, TopDownCameraComponent), Z_Construct_UClass_UCameraComponent_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ADelaware2Character_Statics::NewProp_TopDownCameraComponent_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ADelaware2Character_Statics::NewProp_TopDownCameraComponent_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADelaware2Character_Statics::NewProp_CameraBoom_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Camera" },
		{ "Comment", "/** Camera boom positioning the camera above the character */" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Delaware2Character.h" },
		{ "ToolTip", "Camera boom positioning the camera above the character" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADelaware2Character_Statics::NewProp_CameraBoom = { "CameraBoom", nullptr, (EPropertyFlags)0x00400000000a001d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ADelaware2Character, CameraBoom), Z_Construct_UClass_USpringArmComponent_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ADelaware2Character_Statics::NewProp_CameraBoom_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ADelaware2Character_Statics::NewProp_CameraBoom_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ADelaware2Character_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADelaware2Character_Statics::NewProp_TopDownCameraComponent,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADelaware2Character_Statics::NewProp_CameraBoom,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_ADelaware2Character_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ADelaware2Character>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ADelaware2Character_Statics::ClassParams = {
		&ADelaware2Character::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_ADelaware2Character_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_ADelaware2Character_Statics::PropPointers),
		0,
		0x008000A4u,
		METADATA_PARAMS(Z_Construct_UClass_ADelaware2Character_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ADelaware2Character_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ADelaware2Character()
	{
		if (!Z_Registration_Info_UClass_ADelaware2Character.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ADelaware2Character.OuterSingleton, Z_Construct_UClass_ADelaware2Character_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ADelaware2Character.OuterSingleton;
	}
	template<> DELAWARE2_API UClass* StaticClass<ADelaware2Character>()
	{
		return ADelaware2Character::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ADelaware2Character);
	struct Z_CompiledInDeferFile_FID_Delaware2_Source_Delaware2_Delaware2Character_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Delaware2_Source_Delaware2_Delaware2Character_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ADelaware2Character, ADelaware2Character::StaticClass, TEXT("ADelaware2Character"), &Z_Registration_Info_UClass_ADelaware2Character, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ADelaware2Character), 2176484174U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Delaware2_Source_Delaware2_Delaware2Character_h_755609154(TEXT("/Script/Delaware2"),
		Z_CompiledInDeferFile_FID_Delaware2_Source_Delaware2_Delaware2Character_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Delaware2_Source_Delaware2_Delaware2Character_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
