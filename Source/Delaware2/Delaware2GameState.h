// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "EngineUtils.h"
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
	
	EGameStates GetCurrentState();
	EPlayers GetDealer();
	EPlayers GetPlayerNameFromActor(AActor* target);
	class ADelaware2PlayerState* GetPlayerState(EPlayers player);

	EPlayers IncrementEPlayers(EPlayers e);

	static FString EPlayersToString(EPlayers player);
	
	void IncrementDealer();
	void InitPlayer(EPlayers player, class ADelaware2Pawn* pawn);
		
	void Reset();
	
	void SetHandCounter(const uint8 counter);
	void SetStateComplete();

	void TellTableToSortPlayerCards(EPlayers player);

////////////////////UProperties
	
	
private:

	//////////////////UProperties
	
	UPROPERTY(EditAnywhere, Category = "State Handling")
		EGameStates CurrentState;
	UPROPERTY(VisibleAnywhere, Category = "Dealing")
		EPlayers Dealer;
	UPROPERTY(VisibleAnywhere, Category = "Misc")
		int8 HandCounter;
	UPROPERTY(VisibleAnywhere, Category = "Misc")
		class ADelaware2Pawn* Players[4];
	UPROPERTY(EditAnywhere, Category = "Misc")
		class ATableTopActor* TableTop;

	////////////////Other Properties
	
	class ADelaware2PlayerState* PlayerStates[4];
	bool PlayersInitialized[4] = { false, false, false, false };

	uint8 SortedHands = 0;
};
