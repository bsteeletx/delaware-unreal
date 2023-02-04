// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Delaware2GameState.h"
#include "Delaware2PlayerState.generated.h"

/**
 * Essentially my player code - AI which is in AIController
 */
UCLASS()
class DELAWARE2_API ADelaware2PlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	ADelaware2PlayerState();

	void AddCardToHand(class ACard* newCard);
	
	void CalculateMeld();

	uint8 GetNumberOfCardsInHand();
	uint32 GetPlayerID(); 
	EPlayers GetPlayerSide();
	
	void InitHand();
	bool IsCardInHand(ACard* cardToFind);
	bool IsHandSorted();
	
	void RemoveCardFromHand(ACard* cardToRemove);
	void Reset();
	
	void SetPlayerID(uint32 PlayerID);
	void SetPlayerSide(EPlayers side);
	void SortHand();

private:
	
	class Hand* PlayerHand;
	uint32 PlayerID;
	EPlayers PlayerSide;
	
	//Not sure if we need below or not--I think we do. Will alphabetize when they are definitely needed
	bool doubleRun;
	bool roundRobin;
	short int numOfAces;
	short int hand[20];
	short int meldBid;
	short int runInSuit;  //binary
	short int numberOfBids;
	short int playerID;
	short int pMeld;
	bool runInHearts;
	bool runInClubs;
	bool runInSpades;
	bool runInDiamonds;
	short int mHearts;
	short int mClubs;
	short int mSpades;
	short int mDiamonds;
	bool saveBid;
	short int strongSuit;
	short int strongestRun;
	short int numOfRuns;
	short int numOfBids;  //non-meld
	short int cardsInHand;
	short int lastbid;

};
