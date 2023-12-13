// Fill out your copyright notice in the Description page of Project Settings.

#include "NetBaseCharacter.h"
#include "NetPlayerState.h"
#include "Net/UnrealNetwork.h"

ANetPlayerState::ANetPlayerState():
PlayerIndex(0),
TeamID(),
Result()
{
}

void ANetPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANetPlayerState, Data);
	DOREPLIFETIME(ANetPlayerState, PlayerIndex);
}

void ANetPlayerState::OnRep_PlayerInfo()
{
	ANetBaseCharacter* Char = Cast<ANetBaseCharacter>(GetPawn());
	if (Char)
	{
		Char->PlayerInfoReceived = true;
	}
	else
	{
		FTimerHandle PlayerInfoUpdateTimer;
		GWorld->GetTimerManager().SetTimer(PlayerInfoUpdateTimer, this,
			&ANetPlayerState::OnRep_PlayerInfo,0.25f, false);
	}
}
