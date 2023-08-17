// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Camera/CameraComponent.h"

int ABird::SprintOffset = 10;
 	
// Sets default values
ABird::ABird()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	capsule -> SetCapsuleHalfHeight(20.f);
	capsule -> SetCapsuleRadius(15.f);
	SetRootComponent(capsule);

	birdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	birdMesh->SetupAttachment(GetRootComponent());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	//DO this in blueprint also, set auto possess player to player0
	
}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();
	ABird::SprintOffset=10;
	UFloatingPawnMovement* FloatingPawnMovement = Cast<UFloatingPawnMovement>(GetMovementComponent());	
   	
	FloatingPawnMovement->Acceleration = 4000.f;
	FloatingPawnMovement->MaxSpeed = 1200.f;
	if(APlayerController* PlayerController = Cast<APlayerController>(GetController())){
		
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())){
				Subsystem->AddMappingContext(BirdMappingContext,0);
		}
	}

}

void ABird::MoveForward(float Value)
{
	UE_LOG(LogTemp,Warning,TEXT("Value : %f"),Value);
}

void ABird::Look(const FInputActionValue& Value){
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if(GetController()){
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}
void ABird::Sprint(const FInputActionValue& Value){
	const bool isSprint = Value.Get<bool>();
	UFloatingPawnMovement* FloatingPawnMovement = Cast<UFloatingPawnMovement>(GetMovementComponent());	
   	if(GetController() && isSprint){	
		ABird::SprintOffset++;
		FloatingPawnMovement->Acceleration+=100;
		FloatingPawnMovement->MaxSpeed+=100;
  	}
}

void ABird::Move(const FInputActionValue& Value){
	const float DirectionValue = Value.Get<float>();
	if(GetController() && (DirectionValue != 0.f)){
		UE_LOG(LogTemp,Warning,TEXT("IA_MOVE Triggered %f"),DirectionValue*ABird::SprintOffset);
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward,DirectionValue*ABird::SprintOffset);
	}
	
	// if(CurrentValue){
	// 		UE_LOG(LogTemp,Warning,TEXT("IA_MOVE Triggered "));
	// }
}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)){
		EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&ABird::Move);
		EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Triggered,this,&ABird::Look);
		EnhancedInputComponent->BindAction(SprintAction,ETriggerEvent::Triggered,this,&ABird::Sprint);
	}

	//PlayerInputComponent -> BindAxis(FName("MoveForward"),this,&ABird::MoveForward);

}

