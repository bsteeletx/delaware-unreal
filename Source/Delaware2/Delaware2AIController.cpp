// Fill out your copyright notice in the Description page of Project Settings.


#include "Delaware2AIController.h"
#include "Card.h"


ADelaware2AIController::ADelaware2AIController()
{
	Reset();

	bWantsPlayerState = true;
}

/// <summary>
/// Reset logic and variables for AI
/// </summary>
void ADelaware2AIController::Reset()
{
	LastBid = 0;
	NumberOfBids = 0;
	SaveBid = false;
	LastBid = 0;

	ResetKnownAndPlayed();
	//ThisHand->Reset();
}

/// <summary>
/// Reset AI Card Knowledge
/// </summary>
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

/// <summary>
/// Returns true if this AI is trumping the named suit
/// </summary>
/// <param name="suit">Named Suit to query</param>
/// <returns>true if this AI is trumping suit</returns>
bool ADelaware2AIController::IsTrumpingThisSuit(ESuit suit)
{
	return IsTrumpingSuit[(int)suit];
}

/// <summary>
/// Called when AI is making a save bid OR resetting logic
/// </summary>
/// <param name="saveBid">Set to true if saving</param>
void ADelaware2AIController::SetSaveBid(bool saveBid)
{
	SaveBid = saveBid;
}

/// <summary>
/// Returns bool describing whether this AI gave a save bid or not
/// </summary>
/// <returns></returns>
bool ADelaware2AIController::GetSaveBid()
{
	return SaveBid;
}

/// <summary>
/// Increments total number of Bids this AI has made for this round of bidding
/// </summary>
void ADelaware2AIController::IncrementNumOfBids()
{
	NumberOfBids++;
}

/// <summary>
/// Sets Meld Bid that AI actually gave (or is it how much he WANTS to give? unsure, not using it yet)
/// </summary>
/// <param name="value">value of Meld Bid</param>
void ADelaware2AIController::SetMeldBid(uint8 value)
{
	MeldBid = value;
}

/// <summary>
/// Returns Meld Bid that AI Actually gave (or is it how much he wanted to give? unsure, not using it yet)
/// </summary>
/// <returns></returns>
uint8 ADelaware2AIController::GetMeldBid()
{
	return MeldBid;
}

