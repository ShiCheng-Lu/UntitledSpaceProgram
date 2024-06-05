// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ConstructionPawn.h"
#include "Player/MyPlayerController.h"
#include "ConstructionHUD.h"

#include "GameFramework/Controller.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PlayerInput.h"

// Sets default values
AConstructionPawn::AConstructionPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SetActorEnableCollision(false);
	SetCanBeDamaged(true);

	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	NetPriority = 3.0f;

	BaseEyeHeight = 0.0f;
	bCollideWhenPlacing = false;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("AConstructionPawn::CollisionComponentName");
	CollisionComponent->InitSphereRadius(35.0f);
	CollisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	CollisionComponent->CanCharacterStepUpOn = ECB_No;
	CollisionComponent->SetShouldUpdatePhysicsVolume(true);
	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->bDynamicObstacle = true;

	RootComponent = CollisionComponent;

	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>("AConstructionPawn::MovementComponentName");
	MovementComponent->UpdatedComponent = CollisionComponent;

	// pointer static mesh
	/*
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereAsset(TEXT("/Game/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(SphereAsset.Object);
		StaticMesh->SetRelativeLocation(FVector(150.0f, 0.0f, 0.0f));
		StaticMesh->SetWorldScale3D(FVector(0.1f));
	}
	*/
}

// Called when the game starts or when spawned
void AConstructionPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AConstructionPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	/*
	AMyPlayerController* PlayerController = (AMyPlayerController*)GetController();
	AConstructionHUD* HUD = (AConstructionHUD*)PlayerController->GetHUD();

	FHitResult result;
	if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, result)) {
		
		FVector location;
		FVector direction;
		if (PlayerController->DeprojectMousePositionToWorld(location, direction)) {

			// UE_LOG(LogTemp, Warning, TEXT("Loc %s"), *location.ToString());
			// StaticMesh->SetWorldLocation(location + direction * 100);
		}
	}
	*/
}

void InitializeDefaultPawnInputBindings()
{
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::W, 1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::S, -1.f));

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::A, -1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::D, 1.f));

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveUp", EKeys::LeftShift, -1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveUp", EKeys::SpaceBar, 1.f));

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Turn", EKeys::MouseX, 1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("LookUp", EKeys::MouseY, -1.f));

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("ZoomIn", EKeys::MouseWheelAxis));

	// UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("LeftMouseButton", EKeys::LeftMouseButton));


	UE_LOG(LogTemp, Warning, TEXT("Added all axis"));
}

// Called to bind functionality to input
void AConstructionPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	InitializeDefaultPawnInputBindings();

	PlayerInputComponent->BindAxis("MoveForward", this, &AConstructionPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AConstructionPawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &AConstructionPawn::MoveUp);
	PlayerInputComponent->BindAxis("ZoomIn", this, &AConstructionPawn::ZoomIn);
	PlayerInputComponent->BindAxis("Turn", this, &AConstructionPawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AConstructionPawn::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AConstructionPawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AConstructionPawn::LookUpAtRate);

	// PlayerInputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &AConstructionPawn::HandleClick);

	UE_LOG(LogTemp, Warning, TEXT("PlayerInput Setup"));
}

void AConstructionPawn::MoveRight(float Val)
{
	if (Val != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			// transform to world space and add it
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), Val);
		}
	}
}

void AConstructionPawn::MoveForward(float Val)
{
	if (Val != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			FVector Direction = FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X);
			Direction.Z = 0;
			AddMovementInput(Direction, Val);
		}
	}
}

void AConstructionPawn::MoveUp(float Val)
{
	if (Val != 0.f)
	{
		AddMovementInput(FVector::UpVector, Val);
	}
}

void AConstructionPawn::ZoomIn(float Val) {
	if (Val != 0.f) {
		if (Controller)
		{
			Val *= 100;
			UE_LOG(LogTemp, Warning, TEXT("Zoom in %f"), Val);
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			FVector NewLocation = GetActorLocation();
			NewLocation += FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X) * Val;
			SetActorLocation(NewLocation);
			((AMyPlayerController*)Controller)->PlaceDistance -= Val;
		}
	}
}

void AConstructionPawn::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
}

void AConstructionPawn::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
}

void AConstructionPawn::HandleClick(FKey key) {
	UE_LOG(LogTemp, Warning, TEXT("Clicked"));
}

UPawnMovementComponent* AConstructionPawn::GetMovementComponent() const
{
	return MovementComponent;
}
