// Fill out your copyright notice in the Description page of Project Settings.


#include "NetAvatar.h"

#include "GameFramework/CharacterMovementComponent.h"

ANetAvatar::ANetAvatar():
WalkSpeed(300.0f),
RunSpeed(500.0f)

{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

void ANetAvatar::BeginPlay()
{
	Super::BeginPlay();

	Camera->bUsePawnControlRotation = false;
	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ANetAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Lookup", this, &ACharacter::AddControllerPitchInput);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ANetAvatar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANetAvatar::MoveRight);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ANetAvatar::RunPressed);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ANetAvatar::RunRelased);
}

void ANetAvatar::MoveForward(float Amount)
{
	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation(0, Rotation.Yaw, 0);
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, Amount);
}

void ANetAvatar::MoveRight(float Amount)
{
	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation(0, Rotation.Yaw, 0);
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(ForwardDirection, Amount);
}

void ANetAvatar::RunPressed()
{
	bRunKeyPressed = true;
	UpdateMovementParams();
}

void ANetAvatar::RunRelased()
{
	bRunKeyPressed = false;
	UpdateMovementParams();
}

void ANetAvatar::UpdateMovementParams()
{
	GetCharacterMovement()->MaxWalkSpeed =
	bRunKeyPressed && !bRunKeyPressed ? RunSpeed : WalkSpeed;
}
