// Fill out your copyright notice in the Description page of Project Settings.


#include "TableTopActor.h"
#include "Components/BoxComponent.h"
#include "Delaware2PlayerState.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Blueprint/UserWidget.h"
#include "Delaware2BiddingWidget.h"
#include "Card.h"


// Sets default values
ATableTopActor::ATableTopActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	RootComponent = BoxComponent;

	TableTopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Table Top Mesh"));
	TableTopMesh->SetupAttachment(RootComponent);

	TrumpSelectionSymbol = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Trump Symbol-All"));
	TrumpSelectionSymbol->SetupAttachment(TableTopMesh);
	TrumpSelectionSymbol->SetVisibility(false);

	DeckCounter = 0;
	DealLocations.Reserve(4);

	CardStartLocation.X = -4000;
	CardStartLocation.Y = 4800;
	CardStartLocation.Z = 500;
}

// Called when the game starts or when spawned
void ATableTopActor::BeginPlay()
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

// Called every frame
void ATableTopActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ADelaware2GameState* GameState = GetGameState();

	switch (GameState->GetCurrentState())
	{
	case (EGameStates::Dealing): HandleDealingState(DeltaTime);
	case (EGameStates::Bidding): HandleBiddingState();
	}
}


ADelaware2GameState* ATableTopActor::GetGameState()
{
	return static_cast<ADelaware2GameState*>(GetWorld()->GetGameState());
}

ADelaware2PlayerState* ATableTopActor::GetAPlayerState(EPlayers player)
{
	return GetGameState()->GetPlayerState(player);
}

/// <summary>
/// Everything involved in Dealing
/// </summary>
void ATableTopActor::HandleDealingState(float DeltaTime)
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
		for (EPlayers i = EPlayers::North; (int)i < 5; ++i)
		{
			GetAPlayerState(i)->SortHand();
		}
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
void ATableTopActor::HandleBiddingState()
{
	if (BidPhaseWidgetSubclass)
	{
		if (!BidPhaseWidget)
		{
			BidPhaseWidget = static_cast<UDelaware2BiddingWidget*>(CreateWidget<UUserWidget>(GetWorld(), BidPhaseWidgetSubclass, TEXT("WGP_BidBubbles")));
			BidPhaseWidget->AddToViewport();
		}
		else if (!BidPhaseWidget->GetIsVisible())
		{
			BidPhaseWidget->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("Making Widget visible!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO Subclass for Bidding!"));
	}
}


/// <summary>
/// Set up all of the locations of Targets that were placed in the level as Dealing To Locations including Deck Location, and Dealing From Location
/// </summary>
void ATableTopActor::DealLocationSetup()
{
	FDealLocationVectors Locations[4];

	for (AActor* Actor : TActorRange<AActor>(GetWorld()))
	{
		EPlayers ThisPlayer = GetGameState()->GetPlayerNameFromActor(Actor); //Player

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
/// Deal next card to the appropriate player
/// </summary>
void ATableTopActor::DealCard()
{
	if (DeckCounter % 4 == 0)
	{
		if (DeckCounter == 0)
		{
			PlayerToDealTo = GetGameState()->GetDealer() + 1;
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
	FVector LocationToDealFrom = DealLocations[GetGameState()->GetDealer()].GetALocation(EDealingLocations::DealFrom);
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
	GetAPlayerState(PlayerToDealTo)->AddCardToHand(CardToDeal);
}

/// <summary>
/// Find which Player State the target is associated with
/// </summary>
/// <param name="target">PlayerState as AActor</param>
/// <returns>uint8 of PlayerState number</returns>
uint8 ATableTopActor::GetDealingLocationIndex(AActor* target)
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
uint8 ATableTopActor::Atoi(FString string)
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
EDealingLocations ATableTopActor::GetDealingTargetPointName(AActor* target)
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
/// Returns location of next target to deal to
/// </summary>
/// <param name="playerToDealTo">EPlayers enum of which player to deal to</param>
/// <returns>location of next dealing target</returns>
FVector ATableTopActor::GetDealLocation(EPlayers playerToDealTo)
{
	int Index = 0;
	int RoundTurn = (DeckCounter - 1) / 16;
	int CardTurn = (DeckCounter - 1) % 4;

	Index = RoundTurn * 4 + CardTurn;

	return DealLocations[PlayerToDealTo].GetALocation(EDealingLocations::DealTo, Index);
}

void ATableTopActor::Reset()
{
	Shuffle();

	DeckCounter = 0;
}

/// <summary>
/// Shuffles cards before deal
/// </summary>
void ATableTopActor::Shuffle()
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
ACard* ATableTopActor::GetCardByDeckLocation(uint8 value)
{
	return Deck[value];
}

/// <summary>
/// Gets the next Card in the deck AND increments the deck's counter
/// </summary>
/// <returns>A pointer to the next card in the deck</returns>
ACard* ATableTopActor::GetNextCard()
{
	//UE_LOG(LogTemp, Warning, TEXT("Getting Next Card, DeckCounter: %d"), DeckCounter);
	return GetCardByDeckLocation(DeckCounter++);
}

/// <summary>
/// This replaces the sorting state that was in previously. Each time a player receives their 4th card, it triggers an automatic sort
/// </summary>
/// <param name="owner">Player to check if they are ready to sort</param>
void ATableTopActor::CardReadyToSort(EPlayers owner)
{
	uint8 PlayerNumber = (int)owner - 1;
	CardsReadyForSorting[PlayerNumber]++;

	if (CardsReadyForSorting[PlayerNumber] == 4)
	{
		GetAPlayerState(owner)->SortHand();
		CardsReadyForSorting[PlayerNumber] = 0;
	}
}