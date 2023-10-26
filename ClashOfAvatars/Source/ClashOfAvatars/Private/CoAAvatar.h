// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoABaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "CoAAvatar.generated.h"

/**
 * 
 */
UCLASS()
class ACoAAvatar : public ACoABaseCharacter
{
	GENERATED_BODY()

public:
	ACoAAvatar();
	void BeginPlay();
	void OnConstruction(const FTransform& Transform);
	void Tick(float DeltaTime);

	UPROPERTY(VisibleAnywhere, Category = "COA")
	UCameraComponent* mCamera;

	UPROPERTY(VisibleAnywhere, Category = "COA")
	USpringArmComponent* mSpringArm;


	
	UPROPERTY(EditAnywhere, Category="COA")
	float RunSpeed;

	UPROPERTY(EditAnywhere, Category="COA")
	bool bRunKeyPressed;

	UPROPERTY(VisibleAnywhere, Category="COA")
	float Stamina;

	UPROPERTY(EditAnywhere, Category="COA")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category="COA")
	float StaminaGainRate;

	UPROPERTY(EditAnywhere, Category="COA")
	float StaminaDrainRate;

	UPROPERTY(EditAnywhere, Category="COA")
	bool bStaminaDrained;


	
	UFUNCTION(BlueprintCallable, Category="COA")
	void SetStamina(float NewStamina);

	UFUNCTION(BlueprintCallable, Category="COA")
	float GetStamina();


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
private:
	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void RunPressed();
	void RunRelased();
	void UpdateMovementParams();
};
