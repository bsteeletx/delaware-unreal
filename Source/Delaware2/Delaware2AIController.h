// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Delaware2GameModeBase.h"
#include "Delaware2AIController.generated.h"

/**
 * 
 */
UCLASS()
class DELAWARE2_API ADelaware2AIController : public AAIController
{
	GENERATED_BODY()

public:
	
	ADelaware2AIController();
	
private:

	uint8 KnownCardLocations[3][5][4]; //Player (not including self), Rank, Suit
	uint8 PlayedCards[5][4]; 
	class Hand* ThisHand;

	uint8 GetKnownOfSuit(ESuit suit);
	uint8 GetID();
	uint8 GetMeldBid();
	uint8 GetNumOfBids();
	bool GetSaveBid();
	bool IsTrumpingThisSuit(ESuit suit);
	void IncrementNumOfBids();
	void Reset();
	void ResetKnownAndPlayed();
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
};
