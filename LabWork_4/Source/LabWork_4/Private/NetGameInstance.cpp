// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameInstance.h"


void UNetGameInstance::Host(FString MapName, FSPLayerInfo Info)
{
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, TEXT("Hosting Game..."));
	PlayerInfo = Info;
	GWorld->ServerTravel(FString::Printf(TEXT("/Game/Maps/%slisten"), * MapName));
}

void UNetGameInstance::Join(FString Address, FSPLayerInfo Info)
{
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, FString::Printf(TEXT("Joining at %s"), * Address));
	PlayerInfo = Info;
	GWorld->GetFirstPlayerController()->ClientTravel(Address, TRAVEL_Absolute);
}
