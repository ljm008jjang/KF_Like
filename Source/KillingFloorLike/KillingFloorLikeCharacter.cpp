// Copyright Epic Games, Inc. All Rights Reserved.

#include "KillingFloorLikeCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TP_PickUpComponent.h"
#include "TP_WeaponComponent.h"


//////////////////////////////////////////////////////////////////////////
// AKillingFloorLikeCharacter

class UTP_PickUpComponent;

AKillingFloorLikeCharacter::AKillingFloorLikeCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	CurrentUnitType = EUnitType::Ally;
}

void AKillingFloorLikeCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AKillingFloorLikeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
		                                   &AKillingFloorLikeCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
		                                   &AKillingFloorLikeCharacter::Look);

		// Fire
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this,
		                                   &AKillingFloorLikeCharacter::Fire);

		// DropWeapon
		EnhancedInputComponent->BindAction(DropWeaponAction, ETriggerEvent::Started, this,
		                                   &AKillingFloorLikeCharacter::DropWeapon);

		// SwapWeapon
		EnhancedInputComponent->BindAction(SwapWeaponAction, ETriggerEvent::Triggered, this,
		                                   &AKillingFloorLikeCharacter::OnTriggerSwapWeapon);
	}
}

EWeaponType AKillingFloorLikeCharacter::GetCurrnetWeaponType()
{
	return CurrentWeaponType;
}

UTP_WeaponComponent* AKillingFloorLikeCharacter::GetWeapon(EWeaponType WeaponType)
{
	if (WeaponArray.Contains(WeaponType) == false)
	{
		return nullptr;
	}
	return WeaponArray[WeaponType];
}


void AKillingFloorLikeCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AKillingFloorLikeCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AKillingFloorLikeCharacter::Fire()
{
	if (WeaponArray.Contains(CurrentWeaponType) && WeaponArray[CurrentWeaponType])
	{
		WeaponArray[CurrentWeaponType]->Fire();
	}
}

void AKillingFloorLikeCharacter::DropWeapon()
{
	if (WeaponArray.Contains(CurrentWeaponType) == false)
	{
		return;
	}

	if (UTP_PickUpComponent* PickUpComponent = WeaponArray[CurrentWeaponType]->GetOwner()->FindComponentByClass<
		UTP_PickUpComponent>())
	{
		PickUpComponent->OnDrop.Broadcast(this);
	}
	WeaponArray.Remove(CurrentWeaponType);
	for (const TPair<EWeaponType, UTP_WeaponComponent*>& Pair : WeaponArray)
	{
		UTP_WeaponComponent* Value = Pair.Value;
		SwapWeapon(Value->GetWeaponType());
		break;
	}
}

void AKillingFloorLikeCharacter::OnTriggerSwapWeapon(const FInputActionValue& ActionValue)
{
	if (ActionValue.Get<FVector2D>().X == 1 && WeaponArray.Contains(EWeaponType::Main) && CurrentWeaponType !=
		EWeaponType::Main)
	{
		SwapWeapon(EWeaponType::Main);
	}
	else if (ActionValue.Get<FVector2D>().X == 2 && WeaponArray.Contains(EWeaponType::Sub) && CurrentWeaponType !=
		EWeaponType::Sub)
	{
		SwapWeapon(EWeaponType::Sub);
	}
}

void AKillingFloorLikeCharacter::SwapWeapon(EWeaponType WeaponType)
{
	if (WeaponArray.Contains(CurrentWeaponType))
	{
		EnableActor(false, WeaponArray[CurrentWeaponType]->GetOwner());
	}
	CurrentWeaponType = WeaponType;
	EnableActor(true, WeaponArray[CurrentWeaponType]->GetOwner());
}

void AKillingFloorLikeCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AKillingFloorLikeCharacter::GetHasRifle()
{
	return bHasRifle;
}

void AKillingFloorLikeCharacter::PickUpWeapon(UTP_WeaponComponent* NewWeapon)
{
	if (NewWeapon == nullptr)
	{
		return;
	}

	WeaponArray.Add(NewWeapon->GetWeaponType(), NewWeapon);

	bool isUnderWeaponType = static_cast<int32>(CurrentWeaponType) >= static_cast
		<int32>(NewWeapon->GetWeaponType());

	//첫 종류의 무기이거나 낮은 단계의 무기이면
	if (WeaponArray.Contains(CurrentWeaponType) == false || isUnderWeaponType)
	{
		SwapWeapon(NewWeapon->GetWeaponType());
	}
	//아니면 작동하지 않게 처리
	else
	{
		EnableActor(false, NewWeapon->GetOwner());
	}
}

void AKillingFloorLikeCharacter::EnableActor(bool isEnable, AActor* Actor)
{
	if (!Actor)
	{
		return;
	}

	Actor->SetActorHiddenInGame(!isEnable);

	Actor->SetActorEnableCollision(isEnable);

	Actor->SetActorTickEnabled(isEnable);
}
