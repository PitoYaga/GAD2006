// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameMode.h"
#include "Components/CapsuleComponent.h"
#include "NetPlayerState.h"
#include "EngineUtils.h"
#include "NetBaseCharacter.h"
#include "NetGameState.h"
#include "GameFramework/PlayerStart.h"




ANetGameModeBase::ANetGameModeBase():
MainGameTime(5),
GameTime(5),
TimeFinished(false),
GameStarted(false),
TotalPlayerCount(0),
TotalGames(0),
PlayerStartIndex(0)

{
	DefaultPawnClass = ANetBaseCharacter::StaticClass();
	PlayerStateClass = ANetPlayerState::StaticClass();
	GameStateClass = ANetGameState::StaticClass();
}

void ANetGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANetGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GameStarted)
	{
		if (GameTime > 0)
		{
			GameTime -= DeltaTime;
		}
		else if (GameTime <= 0 && TimeFinished == false)
		{
			GameTime = 0;
			TimeFinished = true;
			GameTimeFinish();
		}
	}
}


AActor* ANetGameModeBase::GetPlayerStart(FString Name, int Index)
{
	FName PSName;
	if (Index<0)
	{
		PSName=*Name;
	}
	else
	{
		PSName =*FString::Printf(TEXT("%s%d"),*Name,Index %4);
	}

	for (TActorIterator<APlayerStart>It(GWorld); It; ++It)
	{
		if (APlayerStart *PS =Cast<APlayerStart>(*It))
		{
			if (PS->PlayerStartTag == PSName)return *It;
		}
	}

	return nullptr;
}


AActor* ANetGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	AActor*Start =AssignTeamAndPlayerStart(Player);
	return Start ? Start :Super::ChoosePlayerStart_Implementation(Player);
}



void ANetGameModeBase::AvatarsOverlapped(ANetAvatar* AvatarA, ANetAvatar* AvatarB)
{
	ANetGameState*GState = GetGameState<ANetGameState>();
	if (GState==nullptr || GState->WinningPlayer >= 0)return;
	
	ANetPlayerState*StateA=AvatarA->GetPlayerState<ANetPlayerState>();
	ANetPlayerState*StateB=AvatarB->GetPlayerState<ANetPlayerState>();

	if (StateA->TeamID == StateB->TeamID) return;

	if (StateA->TeamID ==EPlayerTeam::TEAM_Red && TimeFinished == false)
	{
		GState ->WinningPlayer =StateA ->PlayerIndex;
	}
	else
	{
		GState ->WinningPlayer =StateB ->PlayerIndex;
	}

	AvatarA->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	AvatarB->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);

	GState->OnVictory();

	for (APlayerController*Player:AllPlayers)
	{
		auto State =Player->GetPlayerState<ANetPlayerState>();
		if (State->TeamID==EPlayerTeam::TEAM_Blue)
		{
			State->Result =EGameResults::RESULT_Lost;
		}
		else
		{
			State->Result =EGameResults::RESULT_Won;
		}
	}
	SetActorTickEnabled(false);
	FTimerHandle EndGameTimerHandle;
	GWorld->GetTimerManager().SetTimer(EndGameTimerHandle,this,&ANetGameModeBase::EndGame,2.5f,false);
}



void ANetGameModeBase::EndGame()
{
	PlayerStartIndex=0;
	TotalGames++;
	GetGameState<ANetGameState>()->WinningPlayer=-1;
	for (APlayerController*Player :AllPlayers)
	{
		APawn*Pawn =Player->GetPawn();
		Player->UnPossess();
		Pawn->Destroy();
		Player->StartSpot.Reset();
		RestartPlayer(Player);
	}
	GameTime = MainGameTime;
	TimeFinished = false;
	SetActorTickEnabled(true);
	ANetGameState*GState = GetGameState<ANetGameState>();
	GState->TriggerRestart();
}



void ANetGameModeBase::GameTimeFinish()
{
	for (APlayerController* Player : AllPlayers)
	{
		auto State = Player->GetPlayerState<ANetPlayerState>();
		if (State->TeamID == EPlayerTeam::TEAM_Blue)
		{
			State->Result = EGameResults::RESULT_Won;
		}
		else
		{
			State->Result = EGameResults::RESULT_Lost;
		}
	}

	// Blue player wins
	ANetGameState*GState = GetGameState<ANetGameState>();

	//GState->WinningPlayer = MyNetAvatar->GetPlayerState<ANetPlayerState>()->PlayerIndex;

	GState->OnVictory();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Time is out")); //GetGameState<ANetGameState>()->WinningPlayer
	
	FTimerHandle EndGameTimerHandle;
	GWorld->GetTimerManager().SetTimer(EndGameTimerHandle,this,&ANetGameModeBase::EndGame,2.5f,false);
}




AActor* ANetGameModeBase::AssignTeamAndPlayerStart(AController* Player)
{
	AActor*Start =nullptr;
	ANetPlayerState*State =Player ->GetPlayerState<ANetPlayerState>();
	if (State)
	{
		if (TotalGames==0)
		
		{
			State ->TeamID =TotalPlayerCount == 0? EPlayerTeam::TEAM_Blue:EPlayerTeam::TEAM_Red;
			State ->PlayerIndex =TotalPlayerCount++;
			AllPlayers.Add(Cast<APlayerController>(Player));
		}
	}
	else
	{
		State ->TeamID =State ->Result ==EGameResults::RESULT_Won?EPlayerTeam::TEAM_Blue :EPlayerTeam::TEAM_Red;
	}

	if (State->TeamID ==EPlayerTeam::TEAM_Blue)
	{
		Start=GetPlayerStart("Blue",-1);
	}
	else
	{
		Start =GetPlayerStart("Red",PlayerStartIndex++);
	}
	GameStarted = true;
	return Start;
}
