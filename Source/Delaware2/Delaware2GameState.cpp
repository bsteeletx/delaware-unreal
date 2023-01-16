// Fill out your copyright notice in the Description page of Project Settings.


#include "Delaware2GameState.h"
#include "Card.h"
#include "Engine/TargetPoint.h"
#include "FDealLocationVectors.h"

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
			CurrentState = EGameStates::None; //TODO: Change to something reasonable later
		}
		else if (FirstCard || !NextSetOfFour) //Deal at roughly the same time
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
			DealCard(); //temp
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
		UE_LOG(LogTemp, Warning, TEXT("Starting %s at %s"), *Card->GetFullCardName(), *CardStartLocation.ToCompactString());
		Card->SetToLocation(&CardStartLocation);
		Card->SetActorRotation(FRotator::ZeroRotator);
		Deck.Add(Card);
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
			EDealingLocations ThisLocation = GetDealLocationLoc(Actor); //Deal, Hand, or Deck
			Locations[(int)ThisPlayer - 1].SetALocation(ThisLocation, Actor->GetActorLocation());
			//UE_LOG(LogTemp, Warning, TEXT("Setting Location %d for Player %d"), (int)GetDealLocationLoc(Actor), (int)ThisPlayer - 1);
			//UE_LOG(LogTemp, Warning, TEXT("Target Name: %s, is at %s"), *Actor->GetActorNameOrLabel(), *Locations->GetALocation(ThisLocation).ToCompactString());
		}
	}

	for (int i = 0; i < 4; i++)
	{
		DealLocations.Add((EPlayers)(i + 1), Locations[i]);
	}
}

EDealingLocations ADelaware2GameState::GetDealLocationLoc(AActor* target)
{
	if (target->GetActorNameOrLabel().Contains(TEXT("Deal")))
	{
		return EDealingLocations::Deal;
		//return 1;
	}
	
	if (target->GetActorNameOrLabel().Contains(TEXT("Card")))
	{
		return EDealingLocations::Hand;
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
	FVector LocationToDealFrom = DealLocations[Dealer].GetALocation(EDealingLocations::Deal);
	CardToDeal->SetToLocation(&LocationToDealFrom);
	UE_LOG(LogTemp, Warning, TEXT("Adding Card %s to deal"), *CardToDeal->GetFullCardName())

	if (DealLocations.Num() != 4)
	{
		UE_LOG(LogTemp, Warning, TEXT("DealLocations TMap not set!"));
		return;
	}
	
	FVector LocationToDealTo = DealLocations[PlayerToDealTo].GetALocation(EDealingLocations::Hand); 

	LocationToDealFrom.Z += DeckCounter * CardSpacingByDepth;

	GetDealingOffset(&LocationToDealTo);

	CardToDeal->SetToLocation(&LocationToDealFrom); 

	FString DebugPlayerName = EPlayerAsString[(int)PlayerToDealTo - 1];
	FString DebugLocationName = FDealLocationVectors::GetDealingLocationName(EDealingLocations::Hand);

	//UE_LOG(LogTemp, Warning, TEXT("Sending Card to %s's %s"), *DebugPlayerName, *DebugLocationName);
	CardToDeal->SetPlayerOwner(PlayerToDealTo);
	CardToDeal->DealToLocation(&LocationToDealTo);

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
}

uint8 ADelaware2GameState::GetDeckCounter() const
{
	return HandCounter - 1;
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

void ADelaware2GameState::GetDealingOffset(FVector* locationToDealTo)
{
	bool DealingHorizontal = (PlayerToDealTo == EPlayers::North || PlayerToDealTo == EPlayers::South);
	bool DealingReverse = (PlayerToDealTo == EPlayers::North || PlayerToDealTo == EPlayers::East);

	float MoveAmount = ((DeckCounter / 16 * 4) + (DeckCounter % 4)) * CardSpacing; 
	float DepthAmount = ((DeckCounter / 16 * 4) + (DeckCounter % 4)) * CardSpacingByDepth + CardSpacingByDepth;

	if (DealingReverse)
	{
		MoveAmount = -MoveAmount;
	}

	if (PlayerToDealTo == EPlayers::South)
	{
		MoveAmount *= 2;
	}
	else if (PlayerToDealTo != EPlayers::North)
	{
		MoveAmount /= 2;
	}

	if (DealingHorizontal)
	{
		locationToDealTo->X += MoveAmount;
	}
	else
	{
		locationToDealTo->Y += MoveAmount;
	}

	locationToDealTo->Z = DepthAmount;
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