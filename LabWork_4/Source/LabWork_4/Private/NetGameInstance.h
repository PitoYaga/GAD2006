// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetBaseCharacter.h"
#include "Engine/GameInstance.h"
#include "NetGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNetGameInstance : public UGameInstance
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
	void Host(FString MapName, FSPLayerInfo &Info);

	UFUNCTION(BlueprintCallable)
	void Join(FString Address, FSPLayerInfo &Info);

	UPROPERTY(BlueprintReadWrite)
	FSPLayerInfo PlayerInfo;
};
