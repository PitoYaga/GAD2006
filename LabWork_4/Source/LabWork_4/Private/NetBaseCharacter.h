// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
	BP_EyeBrows = 6,
	BP_COUNT = 7,
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


USTRUCT(BlueprintType)
struct FSPLayerInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText NickName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSBodyPartSelection BodyParts;

	bool Ready;
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

	UFUNCTION(BlueprintCallable)
	void ChangeBodyPart(EBodyPart index, int value, bool DirectSet);

	UFUNCTION(BlueprintCallable)
	void ChangeGender(bool isFemale);

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_PlayerInfoChanged)
	FSBodyPartSelection PartSelection;
	
	UFUNCTION(Server, Reliable)
	void SubmitPlayerInfoToServer(FSPLayerInfo Info);

	UFUNCTION()
	void OnRep_PlayerInfoChanged();

private:
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

	static FSMeshAssetList* GetBodyPartList(EBodyPart part, bool isFemale);

	void UpdateBodyParts();

};
