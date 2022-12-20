// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Delaware2GameModeBase.h"

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
	~Hand();

	void Reset();
	void AddCard(ACardActor* toAdd);
	bool HasCard(ACardActor* cardToFind);
	void RemoveCard(ACardActor* toRemove);
	uint8 GetNumberOfCardsOfType(ERank rank, ESuit suit);
	uint8 GetSize();
	void Sort();
	bool HasMarriageIn(ESuit suit);

	bool HasRoundRobin;
	bool HasDoubleRun;

private:

	uint8 GetMeldFromPinochles();
	uint8 GetMeldFromMarriages();
	void UpdateRanksAround();
	EAroundAmount GetNumberOfAround(ERank rank);
	uint8 GetMeldFromAround();
	void SetRoundRobinFlag();
	void SetRunsInHand();
	uint8 GetRunSize(ESuit suit);
	void CalculateMeld();
	uint8 GetSuitValue(ESuit suit);
	TArray<class ACardActor*> GetAllCardsOfSuit(ESuit suit);
	uint8 GetNumberOfAces();

	TArray<class ACardActor*> Cards;
	TMap<EMeldType, uint8> MeldValues;
	uint8 RunsInSuits;
	ESuitMeld DoubleRunInSuit;
	ESuitMeld StrongestSuitWithRun;
	ESuitMeld StrongestSuit;
	uint8 HasMarriageInSuit;
	uint8 MeldTotal;
	uint8 Pinochles;
	uint8 RankAround;
	uint8 NumberOfMarriagesForEachSuit[4];
	uint8 SuitValue;
	int MeldType;
};
