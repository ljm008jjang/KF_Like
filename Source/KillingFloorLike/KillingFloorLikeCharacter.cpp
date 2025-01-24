// Copyright Epic Games, Inc. All Rights Reserved.

#include "KillingFloorLikeCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TP_PickUpComponent.h"
#include "TP_WeaponComponent.h"
#include "Misc/OutputDeviceNull.h"


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

	if (BaseWeaponClass)
	{
		// 스폰 위치와 회전 설정
		FVector SpawnLocation = GetActorLocation(); // 또는 원하는 위치
		FRotator SpawnRotation = GetActorRotation(); // 또는 원하는 회전

		// 스폰 파라미터 설정
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// 무기 스폰
		ABaseWeapon* SpawnedWeapon = GetWorld()->SpawnActor<ABaseWeapon>(
			BaseWeaponClass, SpawnLocation, SpawnRotation, SpawnParams);

		if (SpawnedWeapon)
		{
			SpawnedWeapon->AttachWeapon(this);
		}
	}

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

		/*// SwapWeapon
		EnhancedInputComponent->BindAction(SwapWeaponAction, ETriggerEvent::Started, this,
		                                   &AKillingFloorLikeCharacter::OnTriggerSwapWeapon);*/
	}
}

EWeaponType AKillingFloorLikeCharacter::GetCurrnetWeaponType()
{
	return CurrentWeaponType;
}

ABaseWeapon* AKillingFloorLikeCharacter::GetWeapon(EWeaponType WeaponType)
{
	if (WeaponArray.Contains(WeaponType) == false)
	{
		return nullptr;
	}
	return WeaponArray[WeaponType];
}

ABaseWeapon* AKillingFloorLikeCharacter::GetCurrentWeapon()
{
	return WeaponArray[CurrentWeaponType];
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

		ExecWeaponEvent(TEXT("EventFire"));
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
	for (const TPair<EWeaponType, ABaseWeapon*>& Pair : WeaponArray)
	{
		ABaseWeapon* Value = Pair.Value;
		SwapWeapon(Value->GetWeaponType());
		break;
	}
}

void AKillingFloorLikeCharacter::OnTriggerSwapWeapon(int ActionValue)
{
	if (ActionValue == 1 && WeaponArray.Contains(EWeaponType::Main) && CurrentWeaponType !=
		EWeaponType::Main)
	{
		SwapWeapon(EWeaponType::Main);
	}
	else if (ActionValue == 2 && WeaponArray.Contains(EWeaponType::Sub) && CurrentWeaponType !=
		EWeaponType::Sub)
	{
		SwapWeapon(EWeaponType::Sub);
	}
	else if (ActionValue == 3 && WeaponArray.Contains(EWeaponType::Knife) && CurrentWeaponType !=
		EWeaponType::Knife)
	{
		SwapWeapon(EWeaponType::Knife);
	}
}

void AKillingFloorLikeCharacter::SwapWeapon(EWeaponType WeaponType)
{
	NextWeaponType = WeaponType;
	ExecWeaponEvent(TEXT("EventPutDown"));
}

void AKillingFloorLikeCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AKillingFloorLikeCharacter::GetHasRifle()
{
	return bHasRifle;
}

void AKillingFloorLikeCharacter::PickUpWeapon(ABaseWeapon* NewWeapon)
{
	if (NewWeapon == nullptr)
	{
		return;
	}

	WeaponArray.Add(NewWeapon->GetWeaponType(), NewWeapon);

	bool isUnderWeaponType = static_cast<int32>(CurrentWeaponType) >= static_cast
		<int32>(NewWeapon->GetWeaponType());

	//첫 획득 무기
	if (WeaponArray.Contains(CurrentWeaponType) == false)
	{
		NextWeaponType = NewWeapon->GetWeaponType();
		SwapWeaponCallback();
	}
	//낮은 단계의 무기이면
	else if (isUnderWeaponType)
	{
		SwapWeapon(NewWeapon->GetWeaponType());
	}
}

void AKillingFloorLikeCharacter::EnableActor(bool isEnable, AActor* Actor)
{
	/*if (!Actor)
	{
		return;
	}

	Actor->SetActorHiddenInGame(!isEnable);

	Actor->SetActorEnableCollision(isEnable);

	Actor->SetActorTickEnabled(isEnable);*/
}

void AKillingFloorLikeCharacter::ExecWeaponEvent(FString EventName)
{
	if (!Mesh1P || !Mesh1P->GetAnimInstance())
	{
		UE_LOG(LogTemp, Error, TEXT("Mesh1P 또는 AnimInstance가 유효하지 않습니다!"));
		return;
	}

	FOutputDeviceNull ar; // 로그 출력을 무시
	UE_LOG(LogTemp, Display, TEXT("Trying to execute event: %s"), *EventName);

	if (Mesh1P->GetAnimInstance()->CallFunctionByNameWithArguments(*EventName, ar, nullptr, true) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Custom Event '%s' 실행 실패!"), *EventName);
	}
}

void AKillingFloorLikeCharacter::SwapWeaponCallback()
{
	CurrentWeaponType = NextWeaponType;

	// 새로운 Skeletal Mesh와 AnimInstance 설정
	Mesh1P->SetSkeletalMesh(WeaponArray[CurrentWeaponType]->GetSkeletalMesh(), true);
	Mesh1P->SetAnimInstanceClass(WeaponArray[CurrentWeaponType]->GetAnimInstance());
}
