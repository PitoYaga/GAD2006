// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetAvatar.h"
#include "GameFramework/GameModeBase.h"
#include "NetGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ANetGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	ANetGameModeBase();

	virtual void Tick(float DeltaTime) override;
	
	virtual AActor*ChoosePlayerStart_Implementation(AController* Player) override;


	UFUNCTION(BlueprintCallable)
	void AvatarsOverlapped(ANetAvatar*AvatarA,ANetAvatar*AvatarB);

	UFUNCTION(BlueprintCallable)
	void EndGame();

	UFUNCTION(BlueprintCallable)
	void GameTimeFinish();

	UPROPERTY(BlueprintReadWrite)
	float MainGameTime;
	
	UPROPERTY(BlueprintReadWrite)
	float GameTime;

	UPROPERTY(BlueprintReadWrite)
	bool TimeFinished;
	
	UPROPERTY()
	bool GameStarted;



private:
	int TotalPlayerCount;
	int TotalGames;
	int PlayerStartIndex;

	TArray<APlayerController*> AllPlayers;

	static AActor* GetPlayerStart(FString Name,int Index);

	AActor* AssignTeamAndPlayerStart(AController*Player);
	
};
