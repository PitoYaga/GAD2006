// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "CoABaseCharacter.generated.h"

UCLASS()
class ACoABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACoABaseCharacter();
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="COA")
	float Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="COA")
	float MaxHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="COA")
	float HealingRate;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="COA")
	float WalkSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="COA")
	bool bDead;
	

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="COA")
	bool Update;


	UFUNCTION(BlueprintCallable, Category="COA")
	void SetHealth(float NewHealth);
	
	UFUNCTION(BlueprintCallable, Category="COA")
	float GetHealth();
	

public:
	UFUNCTION(BlueprintNativeEvent, Category = "COA")
	float ModifyDamage(float IncomingDamage);

	UFUNCTION(BlueprintImplementableEvent, Category = "COA")
	void OnPlayerDied();
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;





protected:
	// Called when the game starts or when spawned
	
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;


	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
