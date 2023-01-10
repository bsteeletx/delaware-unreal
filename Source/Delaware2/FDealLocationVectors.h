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
		Locations[(int)location - 1] = value; //Skipping None
	}

	FVector GetALocation(EDealingLocations location)
	{
		return Locations[(int)location - 1];
	}

	FDealLocationVectors()
	{
		for (FVector V : Locations)
		{
			V = FVector::Zero();
		}
	}
};
