// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Delaware2GameState.h"
#include "Delaware2GameModeBase.generated.h"



/**
 *
 */
UCLASS()
class DELAWARE2_API ADelaware2GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADelaware2GameModeBase();

	EGameStates GetCurrentState();

	virtual void Tick(float DeltaTime) override;

	class ACard* GetNextCard();

	EPlayers GetDealer();
	uint8 GetDeckCounter();
	void IncrementDealer();
	void Reset();
	void SetDeckCounter(uint8 counter);

protected:

	virtual void BeginPlay() override;

private:

	void DealCard();

	UPROPERTY(EditAnywhere)
		float DealDelay = 500;

	float DealTime = 0;

	UPROPERTY(EditAnywhere)
		EGameStates CurrentState;

	UPROPERTY(VisibleAnywhere)
		TMap<EPlayers, FVector> DealLocations;

	UPROPERTY(EditAnywhere)
		float CardSpacing;

	UPROPERTY(EditAnywhere)
		float CardSpacingByDepth;

	UPROPERTY(VisibleAnywhere)
		TArray<class ACard*> Deck;

	void Shuffle();
	ACard* GetCardByID(uint8 value);

	UPROPERTY(VisibleAnywhere)
		EPlayers Dealer;

	EPlayers PlayerToDealTo;

	int8 DeckCounter;

	UPROPERTY(VisibleAnywhere)
		int8 HandCounter;

	void GetDealingOffset(FVector& locationToDealTo);

	//UPROPERTY(VisibleAnywhere)
	//ADelawarePlayerState* PlayerStates[4];

	TMap<EPlayers, uint64> PlayerIDs; //give each player a random ID #, map it to their Enum Key
};
