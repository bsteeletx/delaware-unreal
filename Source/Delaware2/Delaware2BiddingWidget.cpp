// Fill out your copyright notice in the Description page of Project Settings.


#include "Delaware2BiddingWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Internationalization/Text.h"
#include "Delaware2GameState.h"

void UDelaware2BiddingWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UDelaware2BiddingWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (NorthBidText)
	{
		NorthBidText->SetText(FText::FromString(TEXT("100")));
	}
	if (EastBidText)
	{
		EastBidText->SetText(FText::FromString(TEXT("Pass")));
	}
	if (SouthBidText)
	{
		SouthBidText->SetText(FText::FromString(TEXT("Pass")));
	}
	if (WestBidText)
	{
		WestBidText->SetText(FText::FromString(TEXT("Pass")));
	}
	
}