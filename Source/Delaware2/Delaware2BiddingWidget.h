// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Delaware2BiddingWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class DELAWARE2_API UDelaware2BiddingWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

public:

	virtual void SynchronizeProperties() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* NorthBidBubble;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* EastBidBubble;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* SouthBidBubble;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* WestBidBubble;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* NorthBidText;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* EastBidText;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* SouthBidText;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* WestBidText;
};
