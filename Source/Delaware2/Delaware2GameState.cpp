// Fill out your copyright notice in the Description page of Project Settings.


#include "Delaware2GameState.h"
#include "Card.h"
#include "Engine/TargetPoint.h"


ADelaware2GameState::ADelaware2GameState()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentState = EGameStates::None;

	Dealer = EPlayers::South;
	DeckCounter = 0;
	HandCounter = 0;

	DealLocations.Reserve(4);
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
		const bool NextSetOfFour = DeckCounter % 4 == 0;
		const bool FirstCard = DeckCounter == 0;

		if (WaitLonger)
		{
			UE_LOG(LogTemp, Warning, TEXT("Too soon to deal next card, DealTime is %f"), DealTime)
			DealTime += DeltaTime;
			return;
		}

		if (!WithinDeckCounterRange)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Finished Dealing"));
			CurrentState = EGameStates::None; //TODO: Change to something reasonable later
		}
		else if (FirstCard || !NextSetOfFour) //Deal at roughly the same time
		{
			UE_LOG(LogTemp, Warning, TEXT("Dealing Card %d"), DeckCounter);
			DealCard();
			DealTime += DeltaTime;
		}
		else if (NextSetOfFour)
		{
			UE_LOG(LogTemp, Warning, TEXT("Dealing to Next Player"));
			DealTime = 0; //reset DealTime
			DealCard(); //temp
		}
	}
}

void ADelaware2GameState::BeginPlay()
{
	Super::BeginPlay();

	float HeightIterator = 0;

	for (ACard* Card : TActorRange<ACard>(GetWorld()))
	{
		//FVector CardStartLocation = DealStartLocations[(int)Dealer];
		FVector CardStartLocation = FVector::Zero();
		CardStartLocation.Z = 0.2f + HeightIterator;
		Card->SetToLocation(&CardStartLocation);
		UE_LOG(LogTemp, Warning, TEXT("Setting Card %d to Location %s"), Card->GetCardID(), *CardStartLocation.ToString());
		Deck.Add(Card);
		HeightIterator += 0.2f;
	}

	Shuffle();
	
	TActorRange<AActor> Actors = TActorRange<AActor>(GetWorld());
	for (AActor* Actor : Actors)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *Actor->GetName());

		if (Actor->GetActorNameOrLabel() == TEXT("NorthCardStartLoc"))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Found North location!"));
			DealLocations.Add(EPlayers::North, Actor->GetActorLocation());
		}
		else if (Actor->GetActorNameOrLabel() == TEXT("EastCardStartLoc")) //Definitely works! TODO: Find more elegant solution
		{
			//UE_LOG(LogTemp, Warning, TEXT("Found a location!"));
			DealLocations.Add(EPlayers::East, Actor->GetActorLocation());
		}
		else if (Actor->GetActorNameOrLabel() == TEXT("SouthCardStartLoc")) //Definitely works! TODO: Find more elegant solution
		{
			//UE_LOG(LogTemp, Warning, TEXT("Found a location!"));
			DealLocations.Add(EPlayers::South, Actor->GetActorLocation());
		}
		else if (Actor->GetActorNameOrLabel() == TEXT("WestCardStartLoc")) //Definitely works! TODO: Find more elegant solution
		{
			//UE_LOG(LogTemp, Warning, TEXT("Found a location!"));
			DealLocations.Add(EPlayers::West, Actor->GetActorLocation());
		}
	}
}

void ADelaware2GameState::DealCard()
{
	if (DeckCounter % 4 == 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%d mod 4 is 0"), DeckCounter);
		if (DeckCounter == 0)
		{
			PlayerToDealTo = ++Dealer;
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Telling system to increment PlayerToDealTo"));
			PlayerToDealTo = ++PlayerToDealTo;
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Dealing Card %d to Player %d"), DeckCounter, (int)PlayerToDealTo);

	if (DeckCounter > 79)
	{
		UE_LOG(LogTemp, Warning, TEXT("DeckCounter > 79!!"));
		return;
	}
	
	ACard* CardToDeal = GetNextCard();
	CardToDeal->Enable();

	if (DealLocations.Num() != 4)
	{
		UE_LOG(LogTemp, Warning, TEXT("DealLocations TMap not set!"));
		return;
	}

	FVector LocationToDealTo = DealLocations[PlayerToDealTo];

	//have to raise the card by 0.2 and the offset by width and multiply each by the (int)(DeckCounter / 16) + (int)DeckCounter % 4 : West(DeckCounter, Z): 
	GetDealingOffset(&LocationToDealTo);

	CardToDeal->MoveToLocation(&LocationToDealTo);

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
	UE_LOG(LogTemp, Warning, TEXT("DECK DUMP"))
	for (int i = 0; i < 80; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Card %d: %d"), i, Deck[i]->GetCardID());
	}
}

ACard* ADelaware2GameState::GetCardByID(uint8 value)
{
	return Deck[value];
}

void ADelaware2GameState::GetDealingOffset(FVector* locationToDealTo)
{
	bool DealingHorizontal = (int)PlayerToDealTo % 2 == 1;
	bool DealingReverse = (int)PlayerToDealTo % 2 == 0;

	float MoveAmount = ((int)(DeckCounter / 16) + (int)DeckCounter % 4) * CardSpacing;
	float DepthAmount = CardSpacingByDepth * ((int)(DeckCounter / 16) + (int)DeckCounter % 4);

	if (DealingReverse)
	{
		MoveAmount = -MoveAmount;
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
	UE_LOG(LogTemp, Warning, TEXT("Getting Next Card, DeckCounter: %d"), DeckCounter);
	return GetCardByID(DeckCounter++);
}

EPlayers ADelaware2GameState::GetDealer() const
{
	return Dealer;
}

void ADelaware2GameState::IncrementDealer()
{
	++Dealer;
}

EGameStates ADelaware2GameState::GetCurrentState() const
{
	return CurrentState;
}