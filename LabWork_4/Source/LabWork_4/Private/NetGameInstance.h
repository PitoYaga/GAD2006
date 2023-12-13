

#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NetBaseCharacter.h"
#include "NetGameInstance.generated.h"


/**
 * 
 */

UENUM(BlueprintType)
enum class EPlayerTeam : uint8
{
	TEAM_Unknown,
	TEAM_Blue,
	TEAM_Red,
};

UENUM(BlueprintType)
enum class EGameResult : uint8
{
	RESULT_Undefined,
	RESULT_Lost,
	RESULT_Won,
};

USTRUCT(BlueprintType)
struct FSPLayerInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText NickName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSBodyPartSelection BodyParts;

	UPROPERTY(BlueprintReadOnly)
	EPlayerTeam TeamID;
};



UCLASS()
class UNetGameInstance : public UGameInstance
{
	GENERATED_BODY()

	
public:
	UFUNCTION(BlueprintCallable)
	void Host(FString MapName, FSPLayerInfo Info);

	UFUNCTION(BlueprintCallable)
	void Join(FString Address, FSPLayerInfo Info);

	UPROPERTY(BlueprintReadWrite)
	FSPLayerInfo PlayerInfo;
};
