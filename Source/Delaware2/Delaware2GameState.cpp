// Fill out your copyright notice in the Description page of Project Settings.


#include "Delaware2GameState.h"
#include "Engine/TargetPoint.h"
#include "FDealLocationVectors.h"
#include "Delaware2PlayerState.h"
#include "Delaware2Pawn.h"
#include "Hand.h"
#include "TableTopActor.h"
#include "Misc/CString.h"

ADelaware2GameState::ADelaware2GameState()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentState = EGameStates::None;

	Dealer = EPlayers::South;
	HandCounter = 0;
}

void ADelaware2GameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void ADelaware2GameState::BeginPlay()
{
	Super::BeginPlay();

	for (AActor* Actor : TActorRange<AActor>(GetWorld()))
	{
		if (Actor->GetActorNameOrLabel().Contains("Table"))
		{
			TableTop = static_cast<ATableTopActor*>(Actor);
			break;
		}
	}
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
/// Called either after each hand or at the end of the game, not sure which (not being used yet)
/// </summary>
void ADelaware2GameState::Reset()
{
	if ((int)Dealer + 1 > (int)EPlayers::West)
	{
		Dealer = EPlayers::North; //reset to beginning
	}
	else
	{
		Dealer = (EPlayers)((int)(Dealer)+1);
	}

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

void ADelaware2GameState::SetStateComplete()
{
	++CurrentState;
}

void ADelaware2GameState::TellTableToSortPlayerCards(EPlayers player)
{
	TableTop->CardReadyToSort(player);
}





/// <summary>
/// Gets the current dealer
/// </summary>
/// <returns></returns>
EPlayers ADelaware2GameState::GetDealer()
{
	return Dealer;
}

ADelaware2PlayerState* ADelaware2GameState::GetPlayerState(EPlayers player)
{
	return PlayerStates[(int)player-1];
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

