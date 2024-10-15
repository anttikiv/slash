// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Components/AttributeComponent.h"



// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
void ABaseCharacter::Attack()
{

}
//Attack montage player
void ABaseCharacter::PlayAttackMontage()
{

}
void ABaseCharacter::PlayHitReactMontage(const FName & SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector &ImpactPoint)
{
	// getting and normalizing vectors
	const FVector Forward = GetActorForwardVector();
	
	//Lower impact point to enemys location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();
	
	// Forward (dot) ToHit = |Forward|(dot)|ToHit| = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	//inversing cosine (CosTheta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	// if crossproduct points dow, Theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section("FromBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if(Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if(Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}


	PlayHitReactMontage(FName(Section));
	
}

bool ABaseCharacter::CanAttack()
{
    return false;
}

void ABaseCharacter::AttackEnd()
{
 
}

void ABaseCharacter::Die()
{

}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}
