#include "CoABaseCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACoABaseCharacter::ACoABaseCharacter():
	MaxHealth(0.0f),
	Health(0.0f),
	HealingRate(10.0f),
	WalkSpeed(200.0f)
	

{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MaxHealth = 100;
}



void ACoABaseCharacter::SetHealth(float NewHealth)
{
	Health = NewHealth;
}

float ACoABaseCharacter::GetHealth()
{
	return Health;
}



// Called when the game starts or when spawned
void ACoABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ACoABaseCharacter::OnConstruction(const FTransform& Transform)
{
	Health = MaxHealth;
	Update = false;
}



// Called every frame
void ACoABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



// Called to bind functionality to input



