// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Delaware2/Delaware2PlayerController.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeDelaware2PlayerController() {}
// Cross Module References
	DELAWARE2_API UClass* Z_Construct_UClass_ADelaware2PlayerController_NoRegister();
	DELAWARE2_API UClass* Z_Construct_UClass_ADelaware2PlayerController();
	ENGINE_API UClass* Z_Construct_UClass_APlayerController();
	UPackage* Z_Construct_UPackage__Script_Delaware2();
	NIAGARA_API UClass* Z_Construct_UClass_UNiagaraSystem_NoRegister();
// End Cross Module References
	void ADelaware2PlayerController::StaticRegisterNativesADelaware2PlayerController()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ADelaware2PlayerController);
	UClass* Z_Construct_UClass_ADelaware2PlayerController_NoRegister()
	{
		return ADelaware2PlayerController::StaticClass();
	}
	struct Z_Construct_UClass_ADelaware2PlayerController_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ShortPressThreshold_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ShortPressThreshold;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_FXCursor_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_FXCursor;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ADelaware2PlayerController_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_APlayerController,
		(UObject* (*)())Z_Construct_UPackage__Script_Delaware2,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADelaware2PlayerController_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Collision Rendering Transformation" },
		{ "IncludePath", "Delaware2PlayerController.h" },
		{ "ModuleRelativePath", "Delaware2PlayerController.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADelaware2PlayerController_Statics::NewProp_ShortPressThreshold_MetaData[] = {
		{ "Category", "Input" },
		{ "Comment", "/** Time Threshold to know if it was a short press */" },
		{ "ModuleRelativePath", "Delaware2PlayerController.h" },
		{ "ToolTip", "Time Threshold to know if it was a short press" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ADelaware2PlayerController_Statics::NewProp_ShortPressThreshold = { "ShortPressThreshold", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ADelaware2PlayerController, ShortPressThreshold), METADATA_PARAMS(Z_Construct_UClass_ADelaware2PlayerController_Statics::NewProp_ShortPressThreshold_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ADelaware2PlayerController_Statics::NewProp_ShortPressThreshold_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ADelaware2PlayerController_Statics::NewProp_FXCursor_MetaData[] = {
		{ "Category", "Input" },
		{ "Comment", "/** FX Class that we will spawn when clicking */" },
		{ "ModuleRelativePath", "Delaware2PlayerController.h" },
		{ "ToolTip", "FX Class that we will spawn when clicking" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ADelaware2PlayerController_Statics::NewProp_FXCursor = { "FXCursor", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ADelaware2PlayerController, FXCursor), Z_Construct_UClass_UNiagaraSystem_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ADelaware2PlayerController_Statics::NewProp_FXCursor_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ADelaware2PlayerController_Statics::NewProp_FXCursor_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ADelaware2PlayerController_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADelaware2PlayerController_Statics::NewProp_ShortPressThreshold,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ADelaware2PlayerController_Statics::NewProp_FXCursor,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_ADelaware2PlayerController_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ADelaware2PlayerController>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ADelaware2PlayerController_Statics::ClassParams = {
		&ADelaware2PlayerController::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_ADelaware2PlayerController_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_ADelaware2PlayerController_Statics::PropPointers),
		0,
		0x008002A4u,
		METADATA_PARAMS(Z_Construct_UClass_ADelaware2PlayerController_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ADelaware2PlayerController_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ADelaware2PlayerController()
	{
		if (!Z_Registration_Info_UClass_ADelaware2PlayerController.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ADelaware2PlayerController.OuterSingleton, Z_Construct_UClass_ADelaware2PlayerController_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ADelaware2PlayerController.OuterSingleton;
	}
	template<> DELAWARE2_API UClass* StaticClass<ADelaware2PlayerController>()
	{
		return ADelaware2PlayerController::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ADelaware2PlayerController);
	struct Z_CompiledInDeferFile_FID_Delaware2_Source_Delaware2_Delaware2PlayerController_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Delaware2_Source_Delaware2_Delaware2PlayerController_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ADelaware2PlayerController, ADelaware2PlayerController::StaticClass, TEXT("ADelaware2PlayerController"), &Z_Registration_Info_UClass_ADelaware2PlayerController, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ADelaware2PlayerController), 3785566977U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Delaware2_Source_Delaware2_Delaware2PlayerController_h_2030180278(TEXT("/Script/Delaware2"),
		Z_CompiledInDeferFile_FID_Delaware2_Source_Delaware2_Delaware2PlayerController_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Delaware2_Source_Delaware2_Delaware2PlayerController_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
