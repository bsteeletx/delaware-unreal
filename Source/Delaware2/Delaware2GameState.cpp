// Fill out your copyright notice in the Description page of Project Settings.


#include "Delaware2GameState.h"
#include "Card.h"
#include "Engine/TargetPoint.h"
#include "FDealLocationVectors.h"
#include "Delaware2PlayerState.h"
#include "Delaware2Pawn.h"
#include "Hand.h"
#include "Misc/CString.h"
#include "Blueprint/UserWidget.h"
#include "Delaware2BiddingWidget.h"

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

	switch (CurrentState)
	{
	case (EGameStates::Dealing): HandleDealingState(DeltaTime);
	case (EGameStates::Bidding): HandleBiddingState();
	}
}

void ADelaware2GameState::BeginPlay()
{
	Super::BeginPlay();

	float HeightIterator = 1;

	for (ACard* Card : TActorRange<ACard>(GetWorld()))
	{
		CardStartLocation.Z += (CardSpacingByDepth * HeightIterator);
		Card->SetToLocation(&CardStartLocation);
		Card->SetActorRotation(FRotator::ZeroRotator);
		Deck.Emplace(Card);
		HeightIterator++;
	}

	DealLocationSetup();

	Shuffle();
}

void ADelaware2GameState::HandleDealingState(float DeltaTime)
{
	const bool WithinDeckCounterRange = (DeckCounter < 80 && DeckCounter >= 0); //DeckCounter is incremented in GetNextCard
	const bool WaitLonger = DealTime < DealDelay;
	const bool NextSetOfFour = (DeckCounter + 1) % 4 == 0;
	const bool FirstCard = DeckCounter == 0;

	if (WaitLonger)
	{
		DealTime += DeltaTime;
		return;
	}

	if (!WithinDeckCounterRange)
	{
		UE_LOG(LogTemp, Warning, TEXT("Finished Dealing"));
		//in case some cards didn't quite make it, do one more sort
		for (ADelaware2PlayerState* Player : PlayerStates)
		{
			Player->SortHand();
		}
		++CurrentState;
	}
	else if (FirstCard || !NextSetOfFour) //DealFrom at roughly the same time
	{
		DealCard();
		DealTime += DeltaTime;
	}
	else if (NextSetOfFour)
	{
		DealTime = -4 * DealDelay; //reset DealTime, giving extra time between players
		DealCard();
	}
}

/// <summary>
/// Everything involved in Bidding
/// </summary>
void ADelaware2GameState::HandleBiddingState()
{
	if (BidPhaseWidgetSubclass)
	{
		if (!BidPhaseWidget)
		{
			BidPhaseWidget = static_cast<UDelaware2BiddingWidget*>(CreateWidget<UUserWidget>(GetWorld(), BidPhaseWidgetSubclass, TEXT("BidPhase Widget")));
		}
		if (!BidPhaseWidget->GetIsVisible())
		{
			BidPhaseWidget->AddToViewport();
		}
	}
}

/// <summary>
/// Set up all of the locations of Targets that were placed in the level as Dealing To Locations including Deck Location, and Dealing From Location
/// </summary>
void ADelaware2GameState::DealLocationSetup()
{
	FDealLocationVectors Locations[4];

	for (AActor* Actor : TActorRange<AActor>(GetWorld()))
	{
		EPlayers ThisPlayer = GetPlayerNameFromActor(Actor); //Player
		
		if (ThisPlayer != EPlayers::None)
		{
			EDealingLocations ThisLocation = GetDealingTargetPointName(Actor); //DealFrom, DealTo, or Deck

			if (ThisLocation != EDealingLocations::DealTo)
			{
				Locations[(int)ThisPlayer - 1].SetALocation(ThisLocation, Actor->GetActorLocation());
			}
			else
			{
				uint8 IndexLocation = GetDealingLocationIndex(Actor);
				Locations[(int)ThisPlayer - 1].SetALocation(ThisLocation, IndexLocation, Actor->GetActorLocation());
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		DealLocations.Emplace((EPlayers)(i + 1), Locations[i]);
	}
}

/// <summary>
/// Find which Player State the target is associated with
/// </summary>
/// <param name="target">PlayerState as AActor</param>
/// <returns>uint8 of PlayerState number</returns>
uint8 ADelaware2GameState::GetDealingLocationIndex(AActor* target)
{
	int32 Ending = 0;
	FString End = "";

	End = target->GetActorNameOrLabel().Right(2);
	Ending = Atoi(*target->GetActorNameOrLabel().Right(2));
	return Ending - 1;
}

/// <summary>
/// Had to make my own Atoi as the one in Unreal didn't work for some reason
/// </summary>
/// <param name="string">Maximum 3 characters, all must be numbers</param>
/// <returns>uint8 value of characters if they were numbers</returns>
uint8 ADelaware2GameState::Atoi(FString string)
{
	int8 values[3] = { -1, -1, -1 };

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

	return total;
}

/// <summary>
/// Find out which deal location target is
/// </summary>
/// <param name="target">TargetPoint</param>
/// <returns>EDealingLocations enum of what target TargetPoint is referencing</returns>
EDealingLocations ADelaware2GameState::GetDealingTargetPointName(AActor* target)
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

/// <summary>
/// Returns player name of target, if valid
/// </summary>
/// <param name="target">Player to find name of</param>
/// <returns>EPlayers enum referencing player name</returns>
EPlayers ADelaware2GameState::GetPlayerNameFromActor(AActor* target)
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

/// <summary>
/// Deal next card to the appropriate player
/// </summary>
void ADelaware2GameState::DealCard()
{
	if (DeckCounter % 4 == 0)
	{
		if (DeckCounter == 0)
		{
			PlayerToDealTo = Dealer + 1;
		}
		else
		{
			++PlayerToDealTo;
		}
	}

	if (DeckCounter > 79)
	{
		UE_LOG(LogTemp, Warning, TEXT("DeckCounter > 79!!"));
		return;
	}
	
	ACard* CardToDeal = GetNextCard(); //DeckCounter incremented here
	FVector LocationToDealFrom = DealLocations[Dealer].GetALocation(EDealingLocations::DealFrom);
	LocationToDealFrom.Z += DeckCounter * CardSpacingByDepth;
	CardToDeal->SetToLocation(&LocationToDealFrom);

	if (DealLocations.Num() != 4)
	{
		UE_LOG(LogTemp, Warning, TEXT("DealLocations TMap not set!"));
		return;
	}

	FVector LocationToDealTo = GetDealLocation(PlayerToDealTo);

	CardToDeal->SetPlayerOwner(PlayerToDealTo);
	CardToDeal->DealToLocation(&LocationToDealTo);
	PlayerStates[(int)PlayerToDealTo - 1]->AddCardToHand(CardToDeal);
}

/// <summary>
/// Returns location of next target to deal to
/// </summary>
/// <param name="playerToDealTo">EPlayers enum of which player to deal to</param>
/// <returns>location of next dealing target</returns>
FVector ADelaware2GameState::GetDealLocation(EPlayers playerToDealTo)
{
	int Index = 0; 
	int RoundTurn = (DeckCounter-1) / 16;
	int CardTurn = (DeckCounter-1) % 4;

	Index = RoundTurn * 4 + CardTurn; 

	return DealLocations[PlayerToDealTo].GetALocation(EDealingLocations::DealTo, Index);
}

/// <summary>
/// Called either after each hand or at the end of the game, not sure which (not being used yet)
/// </summary>
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

/// <summary>
/// Looks like it sets the Hand Counter to whatever you want, which isn't logical. You either increment that HandCounter, or set it to zero
/// I'm not sure why it's a set and not an increment? Maybe to set it to 0 after game is over?
/// </summary>
/// <param name="counter">number that HandCounter equals</param>
void ADelaware2GameState::SetHandCounter(uint8 counter)
{
	HandCounter = counter;
}

/// <summary>
/// Shuffles cards before deal
/// </summary>
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

/// <summary>
/// Gets a Card by it's location in deck
/// </summary>
/// <param name="value">index of card in the deck</param>
/// <returns>Pointer to a card in the deck</returns>
ACard* ADelaware2GameState::GetCardByDeckLocation(uint8 value)
{
	return Deck[value];
}

/// <summary>
/// Gets the next Card in the deck AND increments the deck's counter
/// </summary>
/// <returns>A pointer to the next card in the deck</returns>
ACard* ADelaware2GameState::GetNextCard()
{
	//UE_LOG(LogTemp, Warning, TEXT("Getting Next Card, DeckCounter: %d"), DeckCounter);
	return GetCardByDeckLocation(DeckCounter++);
}

/// <summary>
/// Gets the current dealer
/// </summary>
/// <returns></returns>
EPlayers ADelaware2GameState::GetDealer()
{
	return Dealer;
}

/// <summary>
/// Increments EPlayers variable by using the overloaded operator which takes into account restarting at the beginning
/// </summary>
/// <param name="e"></param>
/// <returns></returns>
EPlayers ADelaware2GameState::IncrementEPlayers(EPlayers e)
{
	return ++e;
}

/// <summary>
/// Increments the current dealer to the next player
/// </summary>
void ADelaware2GameState::IncrementDealer()
{
	++Dealer;
}

/// <summary>
/// returns an EGameState that represents the current state of the game
/// </summary>
/// <returns></returns>
EGameStates ADelaware2GameState::GetCurrentState()
{
	return CurrentState;
}

/// <summary>
/// Returns an FString that represents the EPlayers equivalent--essentially a 'ToString' method
/// /// </summary>
/// <param name="player">EPlayers Enum of player to turn into a string</param>
/// <returns>actual string of the EPlayers Enum</returns>
FString ADelaware2GameState::EPlayersToString(EPlayers player)
{
	return EPlayerAsString[(int)player - 1];
}

/// <summary>
/// Assigns Player Pawns to each player in the game
/// </summary>
/// <param name="player">EPlayers to receive Pawn</param>
/// <param name="pawn">ADelaware2Pawn* to give to EPlayers</param>
void ADelaware2GameState::InitPlayer(EPlayers player, ADelaware2Pawn* pawn)
{
	uint8 PlayerNumber = (int)player - 1;
	Players[PlayerNumber] = pawn;
	PlayerStates[PlayerNumber] = static_cast<ADelaware2PlayerState*>(Players[PlayerNumber]->GetPlayerState());
	PlayerStates[PlayerNumber]->SetPlayerID(PlayerNumber);
	PlayerStates[PlayerNumber]->InitHand();
}

/// <summary>
/// This replaces the sorting state that was in previously. Each time a player receives their 4th card, it triggers an automatic sort
/// </summary>
/// <param name="owner">Player to check if they are ready to sort</param>
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