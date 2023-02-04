#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "EngineUtils.h"
#include "FDealLocationVectors.generated.h"

UENUM()
enum class EDealingLocations : uint8 { None = 0, Deck, DealFrom, DealTo, All, Any };


USTRUCT() struct FDealLocationVectors
{
	GENERATED_BODY()

	FVector DeckLocation;
	FVector DealFromLocation;
	FVector DealToHandLocations[20];

	/// <summary>
	/// Setup each TargetPoint location in structure
	/// </summary>
	/// <param name="location">location in struct to set</param>
	/// <param name="value">FVector for that location</param>
	void SetALocation(EDealingLocations location, FVector value)
	{
		if (location == EDealingLocations::Deck)
		{
			DeckLocation = value; 
		}
		else if (location == EDealingLocations::DealFrom)
		{
			DealFromLocation = value;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid location in FDealLocationVectors->SetALocation"));
		}
	}

	/// <summary>
	/// Setup each TargetPoint location in structure
	/// </summary>
	/// <param name="location">location in struct to set</param>
	/// <param name="indexValue">index of player hand to set location to</param>
	/// <param name="value">FVector for that location</param>
	void SetALocation(EDealingLocations location, int indexValue, FVector value)
	{
		if (location == EDealingLocations::DealTo)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Setting Player %d's DealTo, index %d to %s"), (int)location, indexValue, *value.ToCompactString());
			DealToHandLocations[indexValue] = value;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid location in FDealLocationVectors->SetALocation 2"));
		}
	}

	/// <summary>
	/// Returns EDealingLocations asked for
	/// </summary>
	/// <param name="location">Dealing Location asked for</param>
	/// <returns>FVector of location asked for</returns>
	const FVector GetALocation(EDealingLocations location)
	{
		if (location == EDealingLocations::Deck)
		{
			return DeckLocation;
		}
		else if (location == EDealingLocations::DealFrom)
		{
			return DealFromLocation;
		}

		UE_LOG(LogTemp, Warning, TEXT("invalid location in FDealLocationVectors->GetALocation"));

		return FVector::Zero();
	}

	/// <summary>
	/// Returns EDealingLocations asked for
	/// </summary>
	/// <param name="location">Dealing Location asked for</param>
	/// <param name="index">Index of Player Hand location</param>
	/// <returns>FVector of location asked for</returns>
	const FVector GetALocation(EDealingLocations location, int index)
	{
		if (location == EDealingLocations::DealTo)
		{
			if (index >= 0)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Index for Getting a location is: %d"), index);
				return DealToHandLocations[index];
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("index paramter is less than zero in FDealLocationVectors->GetALocation"));
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("invalid location in FDealLocationVectors->GetALocation"));

		return FVector::Zero();
	}

	/// <summary>
	/// Get the name of the dealing location--essentially a 'ToString()'
	/// </summary>
	/// <param name="location">EDealingLocation to turn to string</param>
	/// <returns>String representing EDealingLocation</returns>
	static const FString GetDealingLocationName(EDealingLocations location)
	{
		switch (location)
		{
		case EDealingLocations::None: return TEXT("None");
		case EDealingLocations::Deck: return TEXT("Deck");
		case EDealingLocations::DealFrom: return TEXT("Deal");
		case EDealingLocations::DealTo: return TEXT("Hand");
		case EDealingLocations::All: return TEXT("All");
		}

		return TEXT("Any");
	}

	FDealLocationVectors()
	{
		DeckLocation = FVector::Zero();
		DealFromLocation = FVector::Zero();

		for (FVector V : DealToHandLocations)
		{
			V = FVector::Zero();
		}
	}
};
