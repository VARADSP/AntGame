// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Bird.generated.h"


class UCapsuleComponent;
class USkeletalMeshComponent;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UFloatingPawnMovement;

UCLASS()
class SLASH_API ABird : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABird();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	static int SprintOffset;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveForward(float Value);

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Input)
	UInputMappingContext* BirdMappingContext;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Input)
	UInputAction* SprintAction;


	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;


	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	void Move(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	

private:
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* capsule;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* birdMesh;

};
