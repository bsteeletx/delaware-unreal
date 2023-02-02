// Fill out your copyright notice in the Description page of Project Settings.


#include "Delaware2GameState.h"
#include "Card.h"
#include "Engine/TargetPoint.h"
#include "FDealLocationVectors.h"
#include "Delaware2PlayerState.h"
#include "Delaware2Pawn.h"
#include "Hand.h"
#include "Misc/CString.h"

ADelaware2GameState::ADelaware2GameState()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentState = EGameStates::None;

	Dealer = EPlayers::South;
	DeckCounter = 0;
	HandCounter = 0;

	DealLocations.Reserve(4);

	CardStartLocation.X = -4000;
	CardStartLocation.Y = 4800;
	CardStartLocation.Z = 500;
}

void ADelaware2GameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("Tick Function called!"));

	if (CurrentState == EGameStates::Dealing)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Dealing State!"));
		const bool WithinDeckCounterRange = (DeckCounter < 80 && DeckCounter >= 0); //DeckCounter is incremented in GetNextCard
		const bool WaitLonger = DealTime < DealDelay;
		const bool NextSetOfFour = (DeckCounter + 1) % 4 == 0;
		const bool FirstCard = DeckCounter == 0;

		if (WaitLonger)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Too soon to deal next card, DealTime is %f"), DealTime)
			DealTime += DeltaTime;
			return;
		}

		if (!WithinDeckCounterRange)
		{
			UE_LOG(LogTemp, Warning, TEXT("Finished Dealing"));
			++CurrentState;
		}
		else if (FirstCard || !NextSetOfFour) //DealFrom at roughly the same time
		{
			//UE_LOG(LogTemp, Warning, TEXT("Dealing Card %d"), DeckCounter);
			DealCard();
			//UE_LOG(LogTemp, Warning, TEXT("Adding %fl to DealTime %fl"), DeltaTime, DealTime)
			DealTime += DeltaTime;
		}
		else if (NextSetOfFour)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Dealing to Next Player, setting DealTime %fl to -%fl"), DealTime, -4*DealDelay);
			DealTime = -4*DealDelay; //reset DealTime, giving extra time between players
			DealCard(); 
		}
	}
}

void ADelaware2GameState::BeginPlay()
{
	Super::BeginPlay();

	float HeightIterator = 1;

	for (ACard* Card : TActorRange<ACard>(GetWorld()))
	{
		CardStartLocation.Z += (CardSpacingByDepth * HeightIterator);
		//UE_LOG(LogTemp, Warning, TEXT("Starting %s at %s"), *Card->GetFullCardName(), *CardStartLocation.ToCompactString());
		Card->SetToLocation(&CardStartLocation);
		Card->SetActorRotation(FRotator::ZeroRotator);
		Deck.Emplace(Card);
		HeightIterator++;
	}

	DealLocationSetup();

	Shuffle();

	
	
}

uint8 ADelaware2GameState::GetCardDealtNumber(ACard* compare)
{
	uint8 CardCounter = 0;

	for (int i = 0; i < GetDeckCounter(); i++)
	{
		if (Deck[i] == compare)
		{
			CardCounter++;
		}
	}

	return CardCounter;
}

void ADelaware2GameState::DealLocationSetup()
{
	FDealLocationVectors Locations[4];

	for (AActor* Actor : TActorRange<AActor>(GetWorld()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *Actor->GetName());

		EPlayers ThisPlayer = GetDealLocationPlayer(Actor); //Player
		
		if (ThisPlayer != EPlayers::None)
		{
			EDealingLocations ThisLocation = GetDealLocationLoc(Actor); //DealFrom, DealTo, or Deck

			if (ThisLocation != EDealingLocations::DealTo)
			{
				Locations[(int)ThisPlayer - 1].SetALocation(ThisLocation, Actor->GetActorLocation());
				//UE_LOG(LogTemp, Warning, TEXT("Setting Location %d for Player %d"), (int)GetDealLocationLoc(Actor), (int)ThisPlayer - 1);
				//UE_LOG(LogTemp, Warning, TEXT("Target Name: %s, is at %s"), *Actor->GetActorNameOrLabel(), *Locations->GetALocation(ThisLocation).ToCompactString());
			}
			else
			{
				uint8 IndexLocation = GetDealingLocationIndex(Actor);
				Locations[(int)ThisPlayer - 1].SetALocation(ThisLocation, IndexLocation, Actor->GetActorLocation());
				//UE_LOG(LogTemp, Warning, TEXT("Setting Location %d for Player %d"), (int)GetDealLocationLoc(Actor), (int)ThisPlayer - 1);
				//UE_LOG(LogTemp, Warning, TEXT("Target Name: %s, is at %s"), *Actor->GetActorNameOrLabel(), *Locations->GetALocation(ThisLocation).ToCompactString());
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		DealLocations.Emplace((EPlayers)(i + 1), Locations[i]);
	}
}

uint8 ADelaware2GameState::GetDealingLocationIndex(AActor* target)
{
	int32 Ending = 0;
	FString End = "";

	//if (target->GetActorNameOrLabel().Contains(TEXT("West")))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("%s is at %s"), *target->GetActorNameOrLabel(), *target->GetActorLocation().ToCompactString());
	//}
	//UE_LOG(LogTemp, Warning, TEXT("Target Name and Location: %s (%s)"), *target->GetActorNameOrLabel(), *target->GetActorLocation().ToCompactString());
	End = target->GetActorNameOrLabel().Right(2);
	//UE_LOG(LogTemp, Warning, TEXT("Stripped Ending of Target is: %s"), *End);
	Ending = Atoi(*target->GetActorNameOrLabel().Right(2));
	//UE_LOG(LogTemp, Warning, TEXT("Ending for Target Name %s is: %d"), *target->GetActorNameOrLabel(), Ending);
	return Ending - 1;
}

uint8 ADelaware2GameState::Atoi(FString string)
{
	int8 values[3] = { -1, -1, -1 };

	//UE_LOG(LogTemp, Warning, TEXT("Atoi received string: %s"), *string);

	if (string.Len() > 3)
		return 0;

	int index = 0;

	for (char c : string)
	{
		if (c < 48 || c > 57)
			return 0;

		values[index++] = (c - 48);
	}

	uint8 total = 0;
	uint8 multiplier = 1;

	for (int i = 2; i >= 0; i--) //examples of [1 -1 -1], [0 0 1], [0 1 -1], and [2 4 4]
	{
		if (values[i] > -1) //1: -1, -1, 1 | 001: 0, 0, 1 | 01: 0, 1, -1 | 244: 2, 4, 4
		{
			total += values[i] * multiplier; //1: 1 | 001: 1 | 01: 1 | 244: 4 + 40+ 200
			multiplier *= 10;
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Atoi returning value: %d"), total);

	return total;
}

EDealingLocations ADelaware2GameState::GetDealLocationLoc(AActor* target)
{
	if (target->GetActorNameOrLabel().Contains(TEXT("Deal")))
	{
		return EDealingLocations::DealFrom;
		//return 1;
	}
	
	if (target->GetActorNameOrLabel().Contains(TEXT("Card")))
	{
		return EDealingLocations::DealTo;
		//return 2;
	}

	return EDealingLocations::Deck;
	//return 3;
}


EPlayers ADelaware2GameState::GetDealLocationPlayer(AActor* target)
{
	if (target->GetActorNameOrLabel().Contains(TEXT("North")))
	{
		return EPlayers::North;
	}
	if (target->GetActorNameOrLabel().Contains(TEXT("East")))
	{
		return EPlayers::East;
	}
	if (target->GetActorNameOrLabel().Contains(TEXT("South")))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Found a location!"));
		return EPlayers::South;
	}
	if (target->GetActorNameOrLabel().Contains(TEXT("West")))
	{
		return EPlayers::West;
	}

	return EPlayers::None;
}

void ADelaware2GameState::DealCard()
{
	if (DeckCounter % 4 == 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%d mod 4 is 0"), DeckCounter);
		if (DeckCounter == 0)
		{
			PlayerToDealTo = Dealer + 1;
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Telling system to increment PlayerToDealTo"));
			++PlayerToDealTo;
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Dealing Card %d to Player %d"), DeckCounter, (int)PlayerToDealTo);

	if (DeckCounter > 79)
	{
		UE_LOG(LogTemp, Warning, TEXT("DeckCounter > 79!!"));
		return;
	}
	
	ACard* CardToDeal = GetNextCard(); //DeckCounter incremented here
	FVector LocationToDealFrom = DealLocations[Dealer].GetALocation(EDealingLocations::DealFrom);
	LocationToDealFrom.Z += DeckCounter * CardSpacingByDepth;
	CardToDeal->SetToLocation(&LocationToDealFrom);
	//UE_LOG(LogTemp, Warning, TEXT("Adding Card %s to deal"), *CardToDeal->GetFullCardName())

	if (DealLocations.Num() != 4)
	{
		UE_LOG(LogTemp, Warning, TEXT("DealLocations TMap not set!"));
		return;
	}

	FVector LocationToDealTo = GetDealLocation(PlayerToDealTo);

	//UE_LOG(LogTemp, Warning, TEXT("GameState: Received LocationToDealTo as: %s"), *LocationToDealTo.ToCompactString());

	CardToDeal->SetPlayerOwner(PlayerToDealTo);
	CardToDeal->DealToLocation(&LocationToDealTo);
	//UE_LOG(LogTemp, Warning, TEXT("Dealing %s\tto %s,\tslot %d,\twhich is at %s"), *CardToDeal->GetFullCardName(), *EPlayerAsString[(int)PlayerToDealTo - 1], (DeckCounter-1) / 16 * 4 + (DeckCounter-1) % 4, *LocationToDealTo.ToCompactString());
	PlayerStates[(int)PlayerToDealTo - 1]->AddCardToHand(CardToDeal, PlayerToDealTo);
	//PlayerHands[(int)PlayerToDealTo - 1]->AddCard(CardToDeal);
}

FVector ADelaware2GameState::GetDealLocation(EPlayers playerToDealTo)
{
	int Index = 0; 
	int RoundTurn = (DeckCounter-1) / 16;
	int CardTurn = (DeckCounter-1) % 4;

	Index = RoundTurn * 4 + CardTurn; 

	return DealLocations[PlayerToDealTo].GetALocation(EDealingLocations::DealTo, Index);
}

void ADelaware2GameState::Reset()
{
	Shuffle();

	if ((int)Dealer + 1 > (int)EPlayers::West)
	{
		Dealer = EPlayers::North; //reset to beginning
	}
	else
	{
		Dealer = (EPlayers)((int)(Dealer)+1);
	}

	DeckCounter = 0;
	HandCounter = 0;

	SortedHands = 0;
}

uint8 ADelaware2GameState::GetDeckCounter() const
{
	return DeckCounter - 1;
}

void ADelaware2GameState::SetDeckCounter(uint8 counter)
{
	HandCounter = counter;
}

void ADelaware2GameState::Shuffle()
{
	uint8 Random;

	for (int i = 0; i < 14; i++)
	{
		for (int j = 79; j > 0; j--)
		{
			Random = FMath::RandRange(0, j);

			Swap<ACard*>(Deck[j], Deck[Random]);
		}
	}

	//debug
	/*UE_LOG(LogTemp, Warning, TEXT("DECK DUMP"))
	for (int i = 0; i < 80; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Card %d: %d"), i, Deck[i]->GetCardID());
	}*/
}

ACard* ADelaware2GameState::GetCardByID(uint8 value)
{
	return Deck[value];
}

ACard* ADelaware2GameState::GetNextCard()
{
	//UE_LOG(LogTemp, Warning, TEXT("Getting Next Card, DeckCounter: %d"), DeckCounter);
	return GetCardByID(DeckCounter++);
}

EPlayers ADelaware2GameState::GetDealer()
{
	return Dealer;
}

void ADelaware2GameState::IncrementDealer()
{
	++Dealer;
}

EGameStates ADelaware2GameState::GetCurrentState()
{
	return CurrentState;
}

void ADelaware2GameState::PlayerHandSorted(EPlayers playerWithSortedHand)
{
	SortedHands += (int)playerWithSortedHand;

	if (SortedHands == 10) //1 + 2 + 3 + 4
	{
		++CurrentState;
	}
}

EPlayers ADelaware2GameState::GetPlayerToDealTo() const
{
	return PlayerToDealTo;
}

FString ADelaware2GameState::EPlayersToString(EPlayers player)
{
	return EPlayerAsString[(int)player - 1];
}

void ADelaware2GameState::InitPlayer(EPlayers player, ADelaware2Pawn* pawn)
{
	uint8 PlayerNumber = (int)player - 1;
	UE_LOG(LogTemp, Warning, TEXT("Assigning Player state %d to Game State Member %d"), PlayerNumber, PlayerNumber);
	Players[PlayerNumber] = pawn;
	PlayerStates[PlayerNumber] = static_cast<ADelaware2PlayerState*>(Players[PlayerNumber]->GetPlayerState());
	PlayerStates[PlayerNumber]->SetPlayerID(PlayerNumber);
	PlayerStates[PlayerNumber]->InitHand();
}

void ADelaware2GameState::CardReadyToSort(EPlayers owner)
{
	uint8 PlayerNumber = (int)owner - 1;
	CardsReadyForSorting[PlayerNumber]++;

	if (CardsReadyForSorting[PlayerNumber] == 4)
	{
		PlayerStates[PlayerNumber]->SortHand();
		CardsReadyForSorting[PlayerNumber] = 0;
	}
}