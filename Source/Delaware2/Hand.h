// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Delaware2GameMode.h"
#include "Card.h"

UENUM()
enum class ESuitMeld : uint8 { None = 0, 
	Clubs = 0x01, 
	Diamonds = 0x02, 
	Hearts = 0x04, 
	Spades = 0x08, 
	All = 0x0F
};

static const ESuitMeld ESuitMeldAll[] = { ESuitMeld::Clubs, ESuitMeld::Diamonds, ESuitMeld::Hearts, ESuitMeld::Spades };

UENUM()
enum class ERankAround : uint8 { None = 0x00, 
	Jacks = 0x01, 
	Queens = 0x02, 
	Kings = 0x04, 
	Aces = 0x08, 
	DoubleJacks = 0x10, 
	DoubleQueens = 0x20, 
	DoubleKings = 0x40, 
	DoubleAces = 0x80 };

UENUM()
enum class EAroundAmount : uint8 { None = 0, Single, Double, Triple, Quad }; //triple and quad for Pinochles only

UENUM()
enum class EMeldType : uint32 { None = 0, 
	MeldTypeBegin = 0x00001, 
	Marriage = MeldTypeBegin, 
	MarriageInTrump = 0x00002, 
	Pinochle = 0x00004, 
	JacksAround = 0x00008, 
	QueensAround = 0x00010, 
	KingsAround = 0x00020, 
	AcesAround = 0x00040, 
	Run = 0x00080, 
	RoundRobin = 0x00100, 
	DoublePinochle = 0x00200, 
	DoubleJacksAround = 0x00400, 
	DoubleQueensAround = 0x00800, 
	DoubleKingsAround = 0x01000, 
	TriplePinochle = 0x02000, 
	DoubleAcesAround = 0x04000, 
	QuadPinochle = 0x08000, 
	DoubleRun = 0x10000, MeldTypeEnd 
};

static const EMeldType EMeldTypeAll[] = { EMeldType::Marriage,
	EMeldType::MarriageInTrump,
	EMeldType::Pinochle,
	EMeldType::JacksAround,
	EMeldType::QueensAround,
	EMeldType::KingsAround,
	EMeldType::AcesAround,
	EMeldType::Run,
	EMeldType::RoundRobin,
	EMeldType::DoublePinochle,
	EMeldType::DoubleJacksAround,
	EMeldType::DoubleQueensAround,
	EMeldType::DoubleKingsAround,
	EMeldType::TriplePinochle,
	EMeldType::DoubleAcesAround,
	EMeldType::QuadPinochle,
	EMeldType::DoubleRun
};

/**
 * 
 */
class DELAWARE2_API Hand
{
public:
	Hand();

	void AddCard(class ACard* toAdd);

	void CalculateMeld();
	
	uint8 GetNumberOfCardsOfType(ERank rank, ESuit suit);
	uint8 GetNumberOfCards();
	
	int HasCard(ACard* cardToFind);
	bool HasMarriageIn(ESuit suit);
	
	bool IsSorted();
	
	ACard* RemoveCard(ACard* toRemove);
	void Reset();
	
	void Sort();

	//////////Properties--do we need them? And do we need them to be public?
	bool HasRoundRobin;
	bool HasDoubleRun;

private:

	void DumpToLog();

	TArray<class ACard*> GetAllCardsOfSuit(ESuit suit);
	uint8 GetMeldFromAround();
	uint8 GetMeldFromMarriages();
	uint8 GetMeldFromPinochles();
	uint8 GetNumberOfAces();
	EAroundAmount GetNumberOfAround(ERank rank);
	uint8 GetNumberOfCardsInSuit(ESuit suit);
	uint8 GetSuitValue(ESuit suit);

	void RotateCardToProperLocation(ACard* card);

	void SetRoundRobinFlag();
	void SetRunsInHand();

	void UpdateRanksAround();

	/////////////////UProperties
	UPROPERTY(VisibleAnywhere)
	TArray<ACard*> Cards;

	//////////////Other Properties
	ESuitMeld DoubleRunInSuit = ESuitMeld::None;
	
	bool HandIsSorted = false;

	uint8 HasMarriageInSuit = 0;

	uint8 MeldTotal = 0;
	int MeldType = 0;
	static TMap<EMeldType, uint8> MeldValues;
	
	uint8 NumberOfMarriagesForEachSuit[4] = { 0, 0, 0, 0 };

	uint8 Pinochles = 0;

	uint8 RankAround = 0;
	uint8 RunsInSuits = 0;
	
	ESuitMeld StrongestSuit = ESuitMeld::None;
	ESuitMeld StrongestSuitWithRun = ESuitMeld::None;
	uint8 SuitValue = 0;
	
	
	
	
	
	
	
};
