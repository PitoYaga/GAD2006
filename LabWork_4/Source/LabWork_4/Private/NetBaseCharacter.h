

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "NetBaseCharacter.generated.h"


UENUM(BlueprintType)
enum class EBodyPart : uint8
{
	BP_Face = 0,
	BP_Hair = 1,
	BP_Chest = 2,
	BP_Hands = 3,
	BP_Legs = 4,
	BP_Beard = 5,
	BP_Eyebrows = 6,
	BP_BodyType = 7,
	BP_COUNT = 8,
};


USTRUCT(BlueprintType)
struct FSMeshAssetList : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TArray<USkeletalMesh*> ListSkeletal;

	UPROPERTY(EditAnywhere)
	TArray<UStaticMesh*> ListStatic;
};


USTRUCT(BlueprintType)
struct FSBodyPartSelection
{
	GENERATED_USTRUCT_BODY()

	int Indicades[(int)EBodyPart::BP_COUNT];

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isFemale;
};




UCLASS()
class ANetBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ANetBaseCharacter();

	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void GetLifetimeReplicatedProps(TArray < class FLifetimeProperty >& OutLifetimeProps) const override;


public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	FString GetCustomizationData();
	void ParseCustomizationData(FString BodyPartData);
	
	UFUNCTION(BlueprintCallable)
	void ChangeBodyPart(EBodyPart index, int value, bool DirectSet);

	UFUNCTION(BlueprintCallable)
	void ChangeGender(bool isFemale);

	UFUNCTION(Server, Reliable)
	void SubmitPlayerInfoToServer(FSPLayerInfo Info);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerInfoChanged();

	UFUNCTION()
	void CheckPlayerState();

	UFUNCTION()
	void CheckPlayerInfo();
	void OnRep_PlayerInfoChanged();

	UPROPERTY()
	USkeletalMeshComponent* PartFace;

	UPROPERTY()
	UStaticMeshComponent* PartHair;

	UPROPERTY()
	UStaticMeshComponent* PartBeard;

	UPROPERTY()
	UStaticMeshComponent* PartEyes;
	
	UPROPERTY()
	USkeletalMeshComponent* PartHands;

	UPROPERTY()
	USkeletalMeshComponent* PartLegs;

	bool PlayerInfoReceived;

private:
	
	int BodyPartIndices[EBodyPart::BP_COUNT];
	void UpdateBodyParts();
	static FSMeshAssetList* GetBodyPartList(EBodyPart part, bool isFemale);
	FTimerHandle ClientDataCheckTimer;
	
};
