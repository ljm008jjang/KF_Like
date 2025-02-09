// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "BaseWeapon.h"
#include "InputActionValue.h"
#include "KillingFloorLikeCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class AKillingFloorLikeCharacter : public ABaseCharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

public:
	AKillingFloorLikeCharacter();

protected:
	virtual void BeginPlay();

public:
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void PickUpWeapon(ABaseWeapon* bNewHasRifle);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* ChangeAimTypeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* DropWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* ReloadWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SwapWeaponAction;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Fire();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ChangeAimType();

	void DropWeapon();

	
	UFUNCTION(BlueprintCallable)
	EWeaponType IsSwapWeaponable(int ActionValue);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SwapWeapon(EWeaponType ActionValue);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION(BlueprintCallable)
	EWeaponType GetCurrnetWeaponType();
	UFUNCTION(BlueprintCallable)
	ABaseWeapon* GetWeapon(EWeaponType WeaponType);
	UFUNCTION(BlueprintCallable)
	ABaseWeapon* GetCurrentWeapon();

	UFUNCTION(BlueprintSetter)
	void SetNextWeaponType(EWeaponType NewNextWeaponType);

private:
	UPROPERTY(VisibleAnywhere)
	TMap<EWeaponType, class ABaseWeapon*> WeaponArray;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseWeapon> BaseWeaponClass;
	UPROPERTY(VisibleAnywhere)
	EWeaponType CurrentWeaponType = EWeaponType::None;
	UPROPERTY(VisibleAnywhere, BlueprintSetter = SetNextWeaponType)
	EWeaponType NextWeaponType = EWeaponType::None;

	void EnableActor(bool isEnable, AActor* Actor);

	/*UFUNCTION(BlueprintCallable)
	void ExecWeaponEvent(FString EventName);*/

	UFUNCTION(BlueprintCallable)
	void SwapWeaponCallback();
};
