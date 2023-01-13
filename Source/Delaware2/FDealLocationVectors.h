#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "EngineUtils.h"
#include "FDealLocationVectors.generated.h"

UENUM()
enum class EDealingLocations : uint8 { None = 0, Deck, Deal, Hand, All, Any };


USTRUCT() struct FDealLocationVectors
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere)
		FVector Locations[3];

	void SetALocation(EDealingLocations location, FVector value)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Setting location %s at value %s"), *GetDealingLocationName(location), *value.ToCompactString());
		Locations[(int)location - 1] = value; //Skipping None
	}

	const FVector GetALocation(EDealingLocations location)
	{
		return Locations[(int)location - 1];
	}

	static const FString GetDealingLocationName(EDealingLocations location)
	{
		switch (location)
		{
		case EDealingLocations::None: return TEXT("None");
		case EDealingLocations::Deck: return TEXT("Deck");
		case EDealingLocations::Deal: return TEXT("Deal");
		case EDealingLocations::Hand: return TEXT("Hand");
		case EDealingLocations::All: return TEXT("All");
		}

		return TEXT("Any");
	}

	FDealLocationVectors()
	{
		for (FVector V : Locations)
		{
			V = FVector::Zero();
		}
	}
};
