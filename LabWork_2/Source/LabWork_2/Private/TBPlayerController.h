// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameSlot.h"
#include "GameManager.h"
#include "GameFramework/PlayerController.h"
#include "TBPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ATBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATBPlayerController();

	void BeginPlay() override;

	void OnACtorClicked(AActor* Slot, FKey Key);

	UPROPERTY(BlueprintReadWrite)
	AGameManager* GameManager;
	
};
