// Fill out your copyright notice in the Description page of Project Settings.


#include "Delaware2Pawn.h"
#include "Delaware2PlayerState.h"
#include "Delaware2GameState.h"

// Sets default values
ADelaware2Pawn::ADelaware2Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADelaware2Pawn::BeginPlay()
{
	Super::BeginPlay();


	//Reset();
	ADelaware2GameState* GameState = StaticCast<ADelaware2GameState*>(GetWorld()->GetGameState());
	FString PawnName = GetActorNameOrLabel();
	
	if (PawnName[0] == 'N')
	{
		PlayerSide = EPlayers::North;
	}
	else if (PawnName[0] == 'E')
	{
		PlayerSide = EPlayers::East;
	}
	else if (PawnName[0] == 'S')
	{
		PlayerSide = EPlayers::South;
	}
	else if (PawnName[0] == 'W')
	{
		PlayerSide = EPlayers::West;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Playerside found in D2Pawn BeginPlay, Pawn Name is %s"), *PawnName);
	}

	GameState->InitPlayer(PlayerSide, this);
}

// Called every frame
void ADelaware2Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	ADelaware2GameState* GameState = static_cast<ADelaware2GameState*>(GetWorld()->GetGameState());

	if (GameState->GetCurrentState() == EGameStates::Sorting)
	{
		if (!PlayerHand->IsSorted())
		{
			PlayerHand->Sort();
		}

	}*/
}

// Called to bind functionality to input
void ADelaware2Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


// Fill out your copyright notice in the Description page of Project Settings.

/*
void ADelaware2Pawn::Reset()
{
	LastBid = 0;
	NumberOfBids = 0;
	SaveBid = false;
	LastBid = 0;

	ResetKnown();
	//ThisHand->Reset();
}

void ADelaware2Pawn::ResetKnown()
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//Known[i][j] = 0;
		}
	}
}

bool ADelaware2Pawn::IsTrumpingThisSuit(ESuit suit)
{
	return IsTrumpingSuit[(int)suit];
}

void ADelaware2Pawn::ToggleSaveBid()
{
	SaveBid = !SaveBid;
}

bool ADelaware2Pawn::GetSaveBid()
{
	return SaveBid;
}

void ADelaware2Pawn::IncrementNumOfBids()
{
	NumberOfBids++;
}

void ADelaware2Pawn::SetMeldBid(uint8 value)
{
	MeldBid = value;
}

uint8 ADelaware2Pawn::GetMeldBid()
{
	return MeldBid;
}


*/

