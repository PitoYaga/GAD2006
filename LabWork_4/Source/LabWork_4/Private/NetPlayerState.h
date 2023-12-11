// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetGameInstance.h"
#include "GameFramework/PlayerState.h"
#include "NetPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ANetPlayerState : public APlayerState
{
	GENERATED_BODY()


public:
	ANetPlayerState();
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PlayerInfo)
	FSPLayerInfo Data;

	UPROPERTY(BlueprintReadOnly, Replicated)
	int PlayerIndex;

	EPlayerTeam TeamID;
	EGameResult Result;

private:
	UFUNCTION()
	void OnRep_PlayerInfo();
};
