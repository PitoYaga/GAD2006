// Fill out your copyright notice in the Description page of Project Settings.


#include "NetPlayerState.h"

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
