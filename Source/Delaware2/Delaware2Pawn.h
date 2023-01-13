// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Delaware2GameState.h"
#include "Card.h"
#include "Delaware2Pawn.generated.h"

UCLASS()
class DELAWARE2_API ADelaware2Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADelaware2Pawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	class Hand* ThisHand;
	uint8 GetKnownOfSuit(ESuit suit);
	uint8 GetID();
	uint8 GetMeldBid();
	uint8 GetNumOfBids();
	bool GetSaveBid();
	bool IsTrumpingThisSuit(ESuit suit);
	void IncrementNumOfBids();
	void Reset();
	void ResetKnown();
	void SetMeldBid(uint8 value);
	void SetPlayerID(uint8 newID);
	void ToggleSaveBid();


	bool IsTrumpingSuit[4];
	void SetLastBid(uint8 bidAmount);
	uint8 GetLastBid();
	uint8 MeldBid;
	uint8 NumberOfBids;
	uint8 PlayerID;
	bool SaveBid;
	uint8 LastBid;
	bool IsAIControlled;

};
