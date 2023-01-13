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
	Dealing
}; ////Loading, Menu, Options, Start?, InGameMenu, InGameOptions?, / Animation, Sorting ?
//Bidding,
//ShowingMeld,

UENUM(BlueprintType)
enum class EPlayers : uint8 { None = 0, North, East, South, West, All, Any };

const static EPlayers AllPlayers[] = { EPlayers::North, EPlayers::East, EPlayers::South, EPlayers::West };

const static FString EPlayerAsString[] = { "North", "East", "South", "West" };

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

	void SetDeckCounter(const uint8 counter);
	uint8 GetDeckCounter() const;

	EGameStates GetCurrentState() const;

	class ACard* GetNextCard();

	EPlayers GetDealer() const;
	void IncrementDealer();
	void Reset();
	uint8 GetCardDealtNumber(ACard* compare);

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
	EDealingLocations GetDealLocationLoc(AActor* target);

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

	UPROPERTY(EditAnywhere, Category = "Misc")
		FVector CardStartLocation;

	//const FVector* DeckStartLocations[4];
	//const FVector* DealStartLocations[4];

	//UPROPERTY(VisibleAnywhere)
	//ADelawarePlayerState* PlayerStates[4];

	//TMap<EPlayers, uint64> PlayerIDs; //give each player a random ID #, map it to their Enum Key--Later when adding multiplayer


	
};
