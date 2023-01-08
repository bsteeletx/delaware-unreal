// Fill out your copyright notice in the Description page of Project Settings.


#include "Delaware2AIController.h"
#include "Card.h"
#include "Hand.h"

ADelaware2AIController::ADelaware2AIController()
{
	Reset();
}

void ADelaware2AIController::Reset()
{
	LastBid = 0;
	NumberOfBids = 0;
	SaveBid = false;
	LastBid = 0;

	ResetKnownAndPlayed();
	//ThisHand->Reset();
}

void ADelaware2AIController::ResetKnownAndPlayed()
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				//Known[k][i][j] = 0;
			}

			PlayedCards[i][j] = 0;
		}
	}
}

bool ADelaware2AIController::IsTrumpingThisSuit(ESuit suit)
{
	return IsTrumpingSuit[(int)suit];
}

void ADelaware2AIController::ToggleSaveBid()
{
	SaveBid = !SaveBid;
}

bool ADelaware2AIController::GetSaveBid()
{
	return SaveBid;
}

void ADelaware2AIController::IncrementNumOfBids()
{
	NumberOfBids++;
}

void ADelaware2AIController::SetMeldBid(uint8 value)
{
	MeldBid = value;
}

uint8 ADelaware2AIController::GetMeldBid()
{
	return MeldBid;
}

