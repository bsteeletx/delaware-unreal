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

void ADelaware2PlayerState::Reset()
{
	PlayerHand->Reset();
}

void ADelaware2PlayerState::ClearHand()
{
	PlayerHand->Reset();
}

void ADelaware2PlayerState::AddCardToHand(ACard* newCard, EPlayers toDealTo) //take the dealto out, that was just for debugging
{
	if (newCard == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("D2PlayerState AddCard To Hand, newCard is null!"));
	}



	//UE_LOG(LogTemp, Warning, TEXT("Adding %s to %s's hand"), *newCard->GetFullCardName(), *ADelaware2GameState::EPlayersToString(toDealTo));
	
	if (PlayerHand)
	{
		PlayerHand->AddCard(newCard);
	}
}

bool ADelaware2PlayerState::IsCardInHand(ACard* cardToFind)
{
	return (PlayerHand->HasCard(cardToFind) != INDEX_NONE);
}

void ADelaware2PlayerState::RemoveCardFromHand(ACard* cardToRemove)
{
	PlayerHand->RemoveCard(cardToRemove);
}

void ADelaware2PlayerState::SortHand()
{
	if (!PlayerHand->IsSorted())
	{
		PlayerHand->Sort();
	}
}

uint8 ADelaware2PlayerState::GetNumberOfCardsInHand()
{
	return PlayerHand->GetSize();
}

void ADelaware2PlayerState::CalculateMeld()
{
	PlayerHand->CalculateMeld();
}

void ADelaware2PlayerState::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("In D2PlayerState BeginPlay"));
}

void ADelaware2PlayerState::Tick(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("D2PlayerState definitely ticks!"));
}

uint32 ADelaware2PlayerState::GetPlayerID()
{
	return PlayerID;
}

void ADelaware2PlayerState::SetPlayerID(uint32 NewID)
{
	PlayerID = NewID;
}

EPlayers ADelaware2PlayerState::GetPlayerSide()
{
	return PlayerSide;
}

void ADelaware2PlayerState::SetPlayerSide(EPlayers side)
{
	PlayerSide = side;
}

void ADelaware2PlayerState::InitHand()
{
	PlayerHand = new Hand();
}

bool ADelaware2PlayerState::IsHandSorted()
{
	return PlayerHand->IsSorted();
}