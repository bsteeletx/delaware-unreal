// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "EngineUtils.h"
#include "FDealLocationVectors.h"
#include "Delaware2GameState.generated.h"


UENUM(BlueprintType)
enum class EGameStates : uint8 {
	None = 0,
	Dealing,
	Sorting, //Update operator when adding another state
	All,
	Any
}; ////Loading, Menu, Options, Start?, InGameMenu, InGameOptions?, / Animation,
//Bidding,
//ShowingMeld,

UENUM(BlueprintType)
enum class EPlayers : uint8 { None = 0, North, East, South, West, All, Any };

const static EPlayers AllPlayers[] = { EPlayers::North, EPlayers::East, EPlayers::South, EPlayers::West };

const static FString EPlayerAsString[] = { "North", "East", "South", "West" };

static EGameStates& operator++ (EGameStates& e)
{
	if (e == EGameStates::Sorting)
	{
		e = EGameStates::None;
	}
	else
	{
		e = (EGameStates)((int)e + 1);
	}

	return e;
}

static EPlayers operator+ (EPlayers const e, int value)
{
	int EasInt = (int)e;

	EasInt += value;

	while (EasInt > 4)
	{
		EasInt -= 4;
	}

	return (EPlayers)EasInt;
}

static EPlayers& operator++ (EPlayers& e)
{
	//UE_LOG(LogTemp, Warning, TEXT("EPlayers Incrementer Called"));
	if (e == EPlayers::West)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Last Player was WEST, setting to North"));
		e = EPlayers::North;
		return e;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Incrementing PlayerToDealTo"));
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

	void CardReadyToSort(EPlayers owner);

	uint8 GetCardDealtNumber(class ACard* compare);
	EGameStates GetCurrentState();
	EPlayers GetDealer();
	FVector GetDealLocation(EPlayers toDealTo);
	uint8 GetDeckCounter() const;
	ACard* GetNextCard();
	EPlayers GetPlayerToDealTo() const;
	static FString EPlayersToString(EPlayers player);
	static uint8 Atoi(FString string);

	void InitPlayer(EPlayers player, class ADelaware2Pawn* pawn);
	void IncrementDealer();
	void PlayerHandSorted(EPlayers playerHandIsSorted);
	void Reset();
	void SetDeckCounter(const uint8 counter);
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;
	
private:
	void DealCard();
	void DealLocationSetup();

	class ACard* GetCardByID(uint8 value);
	EDealingLocations GetDealLocationLoc(AActor* target);
	EPlayers GetDealLocationPlayer(AActor* target);
	uint8 GetDealingLocationIndex(AActor* target);

	void Shuffle();

	class ADelaware2PlayerState* PlayerStates[4];
	//class UHandActorComponent* PlayerHands[4];

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

	UPROPERTY(VisibleAnywhere, Category = "Dealing")
	EPlayers Dealer;

	EPlayers PlayerToDealTo;

	int8 DeckCounter;

	UPROPERTY(VisibleAnywhere, Category = "Misc")
		int8 HandCounter;

	UPROPERTY(EditAnywhere, Category = "Misc")
		FVector CardStartLocation;

	uint8 SortedHands = 0;

	UPROPERTY(VisibleAnywhere)
	class ADelaware2Pawn* Players[4];

	bool PlayersInitialized[4] = { false, false, false, false };

	uint8 CardsReadyForSorting[4] = { 0 };

	//TMap<EPlayers, uint64> PlayerIDs; //give each player a random ID #, map it to their Enum Key--Later when adding multiplayer


	
};
