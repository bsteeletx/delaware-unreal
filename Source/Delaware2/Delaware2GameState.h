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
	Bidding, //Update operator when adding another state
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
	if (e == EGameStates::Bidding)
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
	int EAsInt = (int)e;

	EAsInt += value;

	while (EAsInt > 4)
	{
		EAsInt -= 4;
	}

	return (EPlayers)EAsInt;
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

	virtual void Tick(float DeltaTime) override;
	
protected:

	virtual void BeginPlay() override;

public:
	
	/*****************UFUNCTIONS*/
	UFUNCTION(BlueprintCallable)
	EGameStates GetCurrentState();
	UFUNCTION(BluePrintCallable)
	EPlayers GetDealer();
	UFUNCTION(BluePrintCallable)
	EPlayers IncrementEPlayers(EPlayers e);

	static uint8 Atoi(FString string);

	void CardReadyToSort(EPlayers owner);

	static FString EPlayersToString(EPlayers player);

	FVector GetDealLocation(EPlayers toDealTo);
	class ACard* GetNextCard();
	
	void IncrementDealer();
	void InitPlayer(EPlayers player, class ADelaware2Pawn* pawn);
		
	void Reset();
	
	void SetHandCounter(const uint8 counter);

////////////////////UProperties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		TSubclassOf<class UDelaware2BiddingWidget> BidPhaseWidgetSubclass;
	
private:

	void DealCard();
	void DealLocationSetup();

	class ACard* GetCardByDeckLocation(uint8 value);
	uint8 GetDealingLocationIndex(AActor* target);
	EDealingLocations GetDealingTargetPointName(AActor* target);
	EPlayers GetPlayerNameFromActor(AActor* target);
	
	void HandleBiddingState();
	void HandleDealingState(float DeltaTime);

	void Shuffle();

	//////////////////UProperties
	
	UPROPERTY(EditAnywhere, Category = "Dealing")
		float CardSpacingByDepth; 
	UPROPERTY(EditAnywhere, Category = "Misc")
		FVector CardStartLocation;
	UPROPERTY(EditAnywhere, Category = "State Handling")
		EGameStates CurrentState;
	UPROPERTY(EditAnywhere, Category = "Dealing")
		float DealDelay = 500;
	UPROPERTY(VisibleAnywhere, Category = "Dealing")
		TMap<EPlayers, FDealLocationVectors> DealLocations;
	UPROPERTY(VisibleAnywhere, Category = "Dealing")
		EPlayers Dealer;
	UPROPERTY(EditAnywhere, Category = "Dealing")
		FVector DealingOffset;
	UPROPERTY(VisibleAnywhere, Category = "Dealing")
		TArray<ACard*> Deck;
	UPROPERTY(VisibleAnywhere, Category = "Misc")
		int8 HandCounter;
	UPROPERTY(VisibleAnywhere)
		class ADelaware2Pawn* Players[4];

	UPROPERTY()
		UDelaware2BiddingWidget* BidPhaseWidget;

	////////////////Other Properties
	
	uint8 CardsReadyForSorting[4] = { 0 };
	float DealTime = 500;
	int8 DeckCounter;
	//class UHandActorComponent* PlayerHands[4];
	//TMap<EPlayers, uint64> PlayerIDs; //give each player a random ID #, map it to their Enum Key--Later when adding multiplayer
	class ADelaware2PlayerState* PlayerStates[4];
	EPlayers PlayerToDealTo;
	bool PlayersInitialized[4] = { false, false, false, false };
	uint8 SortedHands = 0;
	
};
