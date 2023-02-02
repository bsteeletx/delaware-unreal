// Fill out your copyright notice in the Description page of Project Settings.


#include "Hand.h"
#include "Card.h"
#include "Delaware2GameState.h"

TMap<EMeldType, uint8> Hand::MeldValues;

Hand::Hand()
{
	UE_LOG(LogTemp, Warning, TEXT("Creating Empty Hand"));

	Cards.SetNum(0);

	if (MeldValues.Num() == 0)
	{
		MeldValues.Emplace(EMeldType::Marriage, 2);
		MeldValues.Emplace(EMeldType::MarriageInTrump, 4);
		MeldValues.Emplace(EMeldType::Pinochle, 4);
		MeldValues.Emplace(EMeldType::JacksAround, 4);
		MeldValues.Emplace(EMeldType::QueensAround, 6);
		MeldValues.Emplace(EMeldType::KingsAround, 8);
		MeldValues.Emplace(EMeldType::AcesAround, 10);
		MeldValues.Emplace(EMeldType::Run, 15);
		MeldValues.Emplace(EMeldType::RoundRobin, 24);
		MeldValues.Emplace(EMeldType::DoublePinochle, 30);
		MeldValues.Emplace(EMeldType::DoubleJacksAround, 40);
		MeldValues.Emplace(EMeldType::DoubleQueensAround, 60);
		MeldValues.Emplace(EMeldType::DoubleKingsAround, 80);
		MeldValues.Emplace(EMeldType::TriplePinochle, 90);
		MeldValues.Emplace(EMeldType::DoubleAcesAround, 100);
		MeldValues.Emplace(EMeldType::QuadPinochle, 120);
		MeldValues.Emplace(EMeldType::DoubleRun, 150);
	}
}

void Hand::Reset()
{
	for (int i = 0; i < 4; i++)
	{
		NumberOfMarriagesForEachSuit[i] = 0;
	}
	
	RunsInSuits = (int)ESuitMeld::None;
	DoubleRunInSuit = ESuitMeld::None;
	StrongestSuitWithRun = ESuitMeld::None;
	StrongestSuit = ESuitMeld::None;
	HasMarriageInSuit = (int)ESuitMeld::None;
	HasRoundRobin = false;
	HasDoubleRun = false;
	HandIsSorted = false;

	MeldTotal = 0;
	MeldType = 0;

	UE_LOG(LogTemp, Warning, TEXT("Emptying Hand"));
	Cards.Empty(40);
}

uint8 Hand::GetNumberOfCardsOfType(ERank rank, ESuit suit)
{
	if (rank == ERank::Any || suit == ESuit::Any)
	{
		return 0; //error, not valid for this function
	}
	bool AllRanksOK = rank == ERank::All;
	bool AllSuitsOK = suit == ESuit::All;
	uint8 NumberOfCards = 0;

	for (ACard* Card : Cards)
	{
		if ((AllRanksOK || Card->GetRank() == rank) && (AllSuitsOK || Card->GetSuit() == suit))
		{
			NumberOfCards++;
		}
	}

	return NumberOfCards;
}

ACard* Hand::RemoveCard(ACard* toRemove)
{
	ACard* ReturningCard = nullptr;
	int IndexLocation = HasCard(toRemove);

	if (IndexLocation != INDEX_NONE)
	{
		ReturningCard = Cards[IndexLocation];
		Cards.RemoveAt(IndexLocation);
	}

	return ReturningCard;
}

int Hand::HasCard(ACard* toFind)
{
	return Cards.Find(toFind);
}

void Hand::AddCard(ACard* toAdd)
{
	Cards.Add(toAdd);
	HandIsSorted = false;
}

void Hand::Sort()
{
	if (HandIsSorted)
	{
		return;
	}

	bool Done = false;

	UE_LOG(LogTemp, Warning, TEXT("Hand-Sort: Attempting To Sort..."));

	ADelaware2GameState* GameState = static_cast<ADelaware2GameState*>(Cards[0]->GetWorld()->GetGameState());
	EPlayers Owner = Cards[0]->GetPlayerOwner();
	//FVector InitialLocation = GameState->GetDealLocation(Owner);
	bool Sideways = (Owner == EPlayers::East || Owner == EPlayers::West);

	for (ACard* Card : Cards)
	{
		if (Sideways)
		{
			Card->Rotate(90.f);
		}
		else
		{
			Card->Rotate(0.f);
		}
		if (Card->GetPlayerOwner() == EPlayers::South || Card->GetPlayerOwner() == EPlayers::North)
		{
			Card->Flip();
		}
	}

	for (ACard* Card : Cards)
	{
		Card->SetCollision(false);

		//Placing in Proper Playing Area
		if (Card->GetActorLocation() != *Card->GetFinalDestination())
		{
			Card->SetToLocation(Card->GetFinalDestination(), true);
		}
	}

	while (!Done)
	{
		Done = true;

		for (int i = 0; i < Cards.Num() - 1; i++)
		{
			if (Cards[i]->GetCardID() < Cards[i + 1]->GetCardID())
			{
				FVector Temp = *Cards[i]->GetFinalDestination();
				UE_LOG(LogTemp, Warning, TEXT("Temp FVector: %s"), *Temp.ToCompactString());
				
				Cards[i]->SetFinalDestination(Cards[i + 1]->GetFinalDestination()); //for some reason, they are all getting assigned the same FVectors
				Cards[i + 1]->SetFinalDestination(&Temp);
				Cards.Swap(i, i + 1);
				Cards[i]->SetToLocation(Cards[i]->GetFinalDestination(), true);
				Cards[i+1]->SetToLocation(Cards[i+1]->GetFinalDestination(), true);

				if (Cards[i]->GetPlayerOwner() == EPlayers::North)
				{
					UE_LOG(LogTemp, Warning, TEXT("After Swap: Slot %d\tCard %s\tat %s\t and \n\tSlot %d\tCard %s\tat %s..."),
						i, *Cards[i]->GetFullCardName(), *Cards[i]->GetActorLocation().ToCompactString(), i + 1, *Cards[i + 1]->GetFullCardName(),
						*Cards[i + 1]->GetActorLocation().ToCompactString());
					//UE_LOG(LogTemp, Warning, TEXT("Swapping the location 1 (%s) with location 2 (%s)"), *Cards[i]->GetFinalDestination()->ToCompactString(), *Cards[i + 1]->GetFinalDestination()->ToCompactString());
				}

				DumpToLog();

				Done = false;
			}
		}
	}
}

void Hand::DumpToLog()
{

	UE_LOG(LogTemp, Warning, TEXT("Hand Dump..."));

	for (ACard* Card : Cards)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s\t%s"), *Card->GetFullCardName(), *Card->GetActorLocation().ToCompactString());
	}

	HandIsSorted = true;
}



bool Hand::IsSorted()
{
	return HandIsSorted;
}

uint8 Hand::GetSize()
{
	return Cards.Num();
}

void Hand::SetRoundRobinFlag()
{
	if (HasMarriageInSuit == (int)ESuitMeld::All)
		HasRoundRobin = true;
}

uint8 Hand::GetMeldFromMarriages()
{
	uint8 TotalNumberOfMarriages = 0;

	for (int i = (int)ESuit::Clubs; i < (int)ESuit::All; i++)
	{
		int Kings = GetNumberOfCardsOfType(ERank::King, (ESuit)i); 
		int Queens = GetNumberOfCardsOfType(ERank::Queen, (ESuit)i);
		uint8 MinMarriages = FMath::Min(Kings, Queens);

		if (MinMarriages > 0)
		{
			HasMarriageInSuit += (uint8)ESuitMeldAll[i];
			NumberOfMarriagesForEachSuit[i] = MinMarriages;
			TotalNumberOfMarriages += MinMarriages;
		}
	}

	SetRoundRobinFlag();
	
	if (HasRoundRobin)
		return TotalNumberOfMarriages * MeldValues[EMeldType::Marriage] + 2; //2 for marriage in trump
	
	return TotalNumberOfMarriages * MeldValues[EMeldType::Marriage];
}

EAroundAmount Hand::GetNumberOfAround(ERank rank)
{
	bool HasAround = true;
	bool Has2xAround = true;

	for (int j = (int)ESuit::Clubs; j < (int)ESuit::All; j++)
	{
		ACard Temp = ACard();
		Temp.SetRank(rank);
		Temp.SetSuit((ESuit)j);

		if (HasCard(&Temp))
		{
			if (Has2xAround && GetNumberOfCardsOfType(Temp.GetRank(), Temp.GetSuit()) < 2)
				Has2xAround = false;
		}
		else
		{
			HasAround = false;
			Has2xAround = false;
			break;
		}
	}

	if (!HasAround)
		return EAroundAmount::None;
	if (Has2xAround)
		return EAroundAmount::Double;

	return EAroundAmount::Single;
}

void Hand::UpdateRanksAround()
{
	for (int i = (int)ERank::Jack; i < (int)ERank::All; i++)
	{
		if (i == (int)ERank::Ten)
			continue;

		EAroundAmount Around = GetNumberOfAround((ERank)i);

		if (Around == EAroundAmount::Double)
		{
			switch ((ERank)i)
			{
			case ERank::Jack: RankAround += (int)ERankAround::DoubleJacks;
			case ERank::Queen: RankAround += (int)ERankAround::DoubleQueens;
			case ERank::King: RankAround += (int)ERankAround::DoubleKings;
			case ERank::Ace: RankAround += (int)ERankAround::DoubleAces;
			}
		}
		else if (Around == EAroundAmount::Single)
		{
			switch ((ERank)i)
			{
			case ERank::Jack: RankAround += (int)ERankAround::Jacks;
			case ERank::Queen: RankAround += (int)ERankAround::Queens;
			case ERank::King: RankAround += (int)ERankAround::Kings;
			case ERank::Ace: RankAround += (int)ERankAround::Aces;
			}
		}

	}
}

uint8 Hand::GetMeldFromAround()
{
	UpdateRanksAround();

	uint8 MeldFromAround = 0;

	if (RankAround - (int)ERankAround::DoubleAces >= 0)
	{
		MeldFromAround += MeldValues[EMeldType::DoubleAcesAround];
		MeldType += (int)EMeldType::DoubleAcesAround;
		RankAround -= (int)ERankAround::DoubleAces;
	}
	if (RankAround - (int)ERankAround::DoubleKings >= 0)
	{
		MeldFromAround += MeldValues[EMeldType::DoubleKingsAround];
		MeldType += (int)EMeldType::DoubleKingsAround;
		RankAround -= (int)ERankAround::DoubleKings;
	}
	if (RankAround - (int)ERankAround::DoubleQueens >= 0)
	{
		MeldFromAround += MeldValues[EMeldType::DoubleQueensAround];
		MeldType += (int)EMeldType::DoubleQueensAround;
		RankAround -= (int)ERankAround::DoubleQueens;
	}
	if (RankAround - (int)ERankAround::DoubleJacks >= 0)
	{
		MeldFromAround += MeldValues[EMeldType::DoubleJacksAround];
		MeldType += (int)EMeldType::DoubleJacksAround;
		RankAround -= (int)ERankAround::DoubleJacks;
	}
	if (RankAround - (int)ERankAround::Aces >= 0)
	{
		MeldFromAround += MeldValues[EMeldType::AcesAround];
		MeldType += (int)EMeldType::AcesAround;
		RankAround -= (int)ERankAround::Aces;
	}
	if (RankAround - (int)ERankAround::Kings >= 0)
	{
		MeldFromAround += MeldValues[EMeldType::KingsAround];
		MeldType += (int)EMeldType::KingsAround;
		RankAround -= (int)ERankAround::Kings;
	}
	if (RankAround - (int)ERankAround::Queens >= 0)
	{
		MeldFromAround += MeldValues[EMeldType::QueensAround];
		MeldType += (int)EMeldType::QueensAround;
		RankAround -= (int)ERankAround::Queens;
	}
	if (RankAround - (int)ERankAround::Jacks >= 0)
	{
		MeldFromAround += MeldValues[EMeldType::JacksAround];
		MeldType += (int)EMeldType::JacksAround;
		RankAround -= (int)ERankAround::Jacks;
	}

	return MeldFromAround;
}

uint8 Hand::GetMeldFromPinochles()
{
	ACard QueenOfSpades;
	ACard JackOfDiamonds;
	QueenOfSpades.SetRank(ERank::Queen);
	QueenOfSpades.SetSuit(ESuit::Spades);
	JackOfDiamonds.SetRank(ERank::Jack);
	JackOfDiamonds.SetSuit(ESuit::Diamonds);

	uint8 NumberOfPinochles = 0;

	if (HasCard(&QueenOfSpades) && HasCard(&JackOfDiamonds))
	{
		int NumOfQueens = GetNumberOfCardsOfType(QueenOfSpades.GetRank(), QueenOfSpades.GetSuit());
		int NumOfJacks = GetNumberOfCardsOfType(JackOfDiamonds.GetRank(), JackOfDiamonds.GetSuit());
		
		NumberOfPinochles = FMath::Min(NumOfQueens, NumOfJacks);
	}

	if (NumberOfPinochles == 1)
	{
		MeldType += (int)EMeldType::Pinochle;
		return MeldValues[EMeldType::Pinochle];
	}
	else if (NumberOfPinochles == 2)
	{
		MeldType += (int)EMeldType::DoublePinochle;
		return MeldValues[EMeldType::DoublePinochle];
	}
	else if (NumberOfPinochles == 3)
	{
		MeldType += (int)EMeldType::TriplePinochle;
		return MeldValues[EMeldType::TriplePinochle];
	}
	else if (NumberOfPinochles == 4)
	{
		MeldType += (int)EMeldType::QuadPinochle;
		return MeldValues[EMeldType::QuadPinochle];
	}

	return 0;
}

void Hand::CalculateMeld()
{
	MeldTotal += GetMeldFromMarriages();
	MeldTotal += GetMeldFromAround();
	MeldTotal += GetMeldFromPinochles();
}

void Hand::SetRunsInHand()
{
	for (ESuit j = ESuit::Clubs; j < ESuit::All; j = (ESuit)((int)(j)+1))
	{
		bool EveryRankIsGreaterThan2 = true;

		for (ERank i = ERank::Jack; i < ERank::All; i = (ERank)((int)(i)+1))
		{
			int NumberOfCardsInHand = GetNumberOfCardsOfType(i, j);

			if (NumberOfCardsInHand == 0)
			{
				break;
			}
			if (EveryRankIsGreaterThan2 && NumberOfCardsInHand < 2)
			{
				EveryRankIsGreaterThan2 = false;
			}
			if (i == ERank::Ace)
			{
				RunsInSuits += (int)ESuitMeldAll[(int)(j) - 1];

				if (EveryRankIsGreaterThan2)
				{
					HasDoubleRun = true;
					DoubleRunInSuit = ESuitMeldAll[(int)(j)-1];
				}
			}
		}
	}
}

uint8 Hand::GetRunSize(ESuit suit)
{
	return GetNumberOfCardsOfType(ERank::All, suit);
}

bool Hand::HasMarriageIn(ESuit suit)
{
	//ESuitMeld ThisSuit = ESuitMeldAll[(int)(suit)-1];
	uint8 LocalHMIS = HasMarriageInSuit;

	if (LocalHMIS == 0)
	{
		return false;
	}

	if (LocalHMIS < (int)suit)
	{
		return false;
	}

	for (int i = (int)ESuit::Spades; i > (int)ESuit::None; i--)
	{
		if (LocalHMIS >= (uint8)ESuitMeldAll[i])
		{
			if (suit == (ESuit)i)
			{
				return true;
			}

			LocalHMIS -= (uint8)ESuitMeldAll[i];
		}
	}

	return false;
}

uint8 Hand::GetSuitValue(ESuit suit)
{
	TArray<ACard*> CardsInSuit = GetAllCardsOfSuit(suit);
	uint8 Value = 0;

	for (ACard* Card : CardsInSuit)
	{
		Value += (int)Card->GetRank();
	}

	return Value;
}

TArray<ACard*> Hand::GetAllCardsOfSuit(ESuit suit)
{
	TArray<ACard*> AllCardsInSuit;

	for (ACard* Card : Cards)
	{
		if (Card->GetSuit() == suit)
		{
			AllCardsInSuit.Emplace(Card);
		}
	}

	return AllCardsInSuit;
}

uint8 Hand::GetNumberOfAces()
{
	return GetNumberOfCardsOfType(ERank::Ace, ESuit::All);
}