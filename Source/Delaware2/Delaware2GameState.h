// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "EngineUtils.h"
#include "Delaware2GameState.generated.h"


UENUM(BlueprintType)
enum class ERank : uint8 { None = 0, Jack, Queen, King, Ten, Ace, All, Any };

const static ERank AllRanks[] = { ERank::Jack, ERank::Queen, ERank::King, ERank::Ten, ERank::Ace };

UENUM(BlueprintType)
enum class ESuit : uint8 { None = 0, Diamonds, Clubs, Hearts, Spades, All, Any };

const static ESuit AllSuits[] = { ESuit::Diamonds, ESuit::Clubs,  ESuit::Hearts, ESuit::Spades };

UENUM(BlueprintType)
enum class EGameStates : uint8 {
	None = 0,
	Dealing
}; ////Loading, Menu, Options, Start?, InGameMenu, InGameOptions?, / Animation, Sorting ?
//Bidding,
//ShowingMeld,

UENUM(BlueprintType)
enum class EPlayers : uint8 { None = 0, North, East, South, West, All, Any };

const static EPlayers AllPlayers[] = { EPlayers::North, EPlayers::East, EPlayers::South, EPlayers::West };

UENUM()
enum class EDealingLocations : uint8 { None = 0, Deck, Deal, Hand, All, Any };

USTRUCT() struct FDealLocationVectors
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FVector Locations[3];

	void SetALocation(/*EDealingLocations*/int location, FVector value)
	{
		Locations[(int)location - 1] = value; //Skipping None
	}

	FVector GetALocation(/*EDealingLocations*/int location)
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

static EPlayers operator++ (EPlayers e)
{
	UE_LOG(LogTemp, Warning, TEXT("EPlayers Incrementer Called"));
	if (e == EPlayers::West)
	{
		UE_LOG(LogTemp, Warning, TEXT("Last Player was WEST, setting to North"));
		e = EPlayers::North;
		return e;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Incrementing PlayerToDealTo"));
		e = (EPlayers)((int)e + 1);
		return e;
	}
};
/**
 * 
 */
UCLASS()
class DELAWARE2_API ADelaware2GameState : public AGameState
{
	GENERATED_BODY()

public:
	ADelaware2GameState();

	void SetDeckCounter(const uint8 counter);
	uint8 GetDeckCounter() const;

	EGameStates GetCurrentState() const;

	class ACard* GetNextCard();

	EPlayers GetDealer() const;
	void IncrementDealer();
	void Reset();

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;
	
private:
	void GetDealingOffset(FVector* locationToDealTo);
	void DealCard();
	void Shuffle();
	class ACard* GetCardByID(uint8 value);
	FVector GetSideToSideOffset();
	void DealLocationSetup();
	EPlayers GetDealLocationPlayer(AActor* target);
	/*EDealingLocations*/int GetDealLocationLoc(AActor* target);

	UPROPERTY(EditAnywhere, Category = "Dealing")
		FVector DealingOffset;

	UPROPERTY(EditAnywhere, Category = "Dealing")
		float DealDelay = 500;

	float DealTime = 500;

	UPROPERTY(EditAnywhere, Category = "State Handling")
		EGameStates CurrentState;

	UPROPERTY(VisibleAnywhere, Category = "Dealing")
		TMap<EPlayers, FDealLocationVectors> DealLocations;

	UPROPERTY(EditAnywhere, Category = "Dealing")
		float CardSpacing;

	UPROPERTY(EditAnywhere, Category = "Dealing")
		float CardSpacingByDepth;

	UPROPERTY(VisibleAnywhere, Category = "Dealing")
		TArray<ACard*> Deck;


	EPlayers Dealer;

	EPlayers PlayerToDealTo;

	int8 DeckCounter;

	UPROPERTY(VisibleAnywhere, Category = "Misc")
		int8 HandCounter;

	const FVector* DeckStartLocations[4];
	const FVector* DealStartLocations[4];

	//UPROPERTY(VisibleAnywhere)
	//ADelawarePlayerState* PlayerStates[4];

	//TMap<EPlayers, uint64> PlayerIDs; //give each player a random ID #, map it to their Enum Key--Later when adding multiplayer


	
};
