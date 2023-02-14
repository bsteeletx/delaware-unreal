// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Delaware2GameState.h"
#include "FDealLocationVectors.h"
#include "TableTopActor.generated.h"

UCLASS()
class DELAWARE2_API ATableTopActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATableTopActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	static uint8 Atoi(FString string);

	void CardReadyToSort(EPlayers owner);

	FVector GetDealLocation(EPlayers toDealTo);
	class ACard* GetNextCard();

private:

	void DealCard();
	void DealLocationSetup();

	class ACard* GetCardByDeckLocation(uint8 value);
	uint8 GetDealingLocationIndex(AActor* target);
	EDealingLocations GetDealingTargetPointName(AActor* target);
	class ADelaware2GameState* GetGameState();
	class ADelaware2PlayerState* GetAPlayerState(EPlayers player);

	void HandleBiddingState();
	void HandleDealingState(float DeltaTime);

	void Reset();

	void Shuffle();

	/***********UPROPERTIES***********/

	UPROPERTY(EditAnywhere, Category = "UI")
		class	UDelaware2BiddingWidget* BidPhaseWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UDelaware2BiddingWidget> BidPhaseWidgetSubclass;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, Category = "Dealing")
		float CardSpacingByDepth;

	UPROPERTY(EditAnywhere, Category = "Dealing")
		FVector CardStartLocation;

	UPROPERTY(EditAnywhere, Category = "Dealing")
		float DealDelay = 500;
	UPROPERTY(VisibleAnywhere, Category = "Dealing")
		TMap<EPlayers, FDealLocationVectors> DealLocations;
	UPROPERTY(EditAnywhere, Category = "Dealing")
		FVector DealingOffset;
	UPROPERTY(VisibleAnywhere, Category = "Dealing")
		TArray<ACard*> Deck;


	UPROPERTY(EditAnywhere, Category = "Flipbooks")
		class UPaperFlipbook* SuitSymbols[5];

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TableTopMesh;
	UPROPERTY(EditAnywhere)
	class UPaperFlipbookComponent* TrumpSelectionSymbol;

	/******Other Properties*****/
	
	uint8 CardsReadyForSorting[4] = { 0 };
	float DealTime = 500;
	int8 DeckCounter;
	EPlayers PlayerToDealTo;


};
