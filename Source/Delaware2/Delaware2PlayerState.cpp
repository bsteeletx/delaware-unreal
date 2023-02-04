// Fill out your copyright notice in the Description page of Project Settings.


#include "Delaware2PlayerState.h"
#include "Card.h"
#include "Hand.h"
#include "Delaware2GameState.h"

ADelaware2PlayerState::ADelaware2PlayerState()
{
	PlayerHand = new Hand();
	PlayerHand->Reset();
}

/// <summary>
/// unused really at this point
/// Will be used to reset hands and other variables after a hand or game completes
/// </summary>
void ADelaware2PlayerState::Reset()
{
	PlayerHand->Reset();
}

/// <summary>
/// Add a card to this player's hand
/// </summary>
/// <param name="newCard">Card to Add</param>
void ADelaware2PlayerState::AddCardToHand(ACard* newCard)
{
	if (newCard == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("D2PlayerState AddCard To Hand, newCard is null!"));
	}

	if (PlayerHand)
	{
		PlayerHand->AddCard(newCard);
	}
}

/// <summary>
/// Find out if a card is in the player's hand
/// </summary>
/// <param name="cardToFind">The card to find</param>
/// <returns>true if found</returns>
bool ADelaware2PlayerState::IsCardInHand(ACard* cardToFind)
{
	return (PlayerHand->HasCard(cardToFind) != INDEX_NONE);
}

/// <summary>
/// Not used yet, but will be used
/// Removes card from player's hand
/// </summary>
/// <param name="cardToRemove">The Card to remove</param>
void ADelaware2PlayerState::RemoveCardFromHand(ACard* cardToRemove)
{
	PlayerHand->RemoveCard(cardToRemove);
}

/// <summary>
/// Sort hand, if it is unsorted
/// </summary>
void ADelaware2PlayerState::SortHand()
{
	if (!PlayerHand->IsSorted())
	{
		PlayerHand->Sort();
	}
}

/// <summary>
/// Returns the number of cards in player's hand
/// </summary>
/// <returns></returns>
uint8 ADelaware2PlayerState::GetNumberOfCardsInHand()
{
	return PlayerHand->GetNumberOfCards();
}

/// <summary>
/// unused, as of now
/// Tells hand to start calculating meld
/// </summary>
void ADelaware2PlayerState::CalculateMeld()
{
	PlayerHand->CalculateMeld();
}

/// <summary>
/// not used at the moment, will be used when we get into multiplayer, I think
/// returns playerID
/// </summary>
/// <returns></returns>
uint32 ADelaware2PlayerState::GetPlayerID()
{
	return PlayerID;
}

/// <summary>
/// Sets PlayerID
/// </summary>
/// <param name="NewID"></param>
void ADelaware2PlayerState::SetPlayerID(uint32 NewID)
{
	PlayerID = NewID;
}

/// <summary>
/// unused
/// Returns which EPlayers, North, East, South, West he is actually on
/// As opposed to which side he's visually on--as all PC's will see themselves as being on the South side
/// </summary>
/// <returns></returns>
EPlayers ADelaware2PlayerState::GetPlayerSide()
{
	return PlayerSide;
}

/// <summary>
/// unused
/// Sets which player side the PC will actually be on
/// </summary>
/// <param name="side"></param>
void ADelaware2PlayerState::SetPlayerSide(EPlayers side)
{
	PlayerSide = side;
}

/// <summary>
/// Start new hand, not sure if this will be needed more than at player start up
/// </summary>
void ADelaware2PlayerState::InitHand()
{
	PlayerHand = new Hand();
}

/// <summary>
/// Routine check to see if hand is sorted
/// </summary>
/// <returns>true if sorted</returns>
bool ADelaware2PlayerState::IsHandSorted()
{
	return PlayerHand->IsSorted();
}