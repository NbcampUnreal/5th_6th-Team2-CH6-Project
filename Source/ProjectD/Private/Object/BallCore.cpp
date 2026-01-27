#include "Object/BallCore.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "Pawn/PDPawnBase.h" // 폰 함수 호출용

ABallCore::ABallCore()
{
	bReplicates = true;
	SetReplicateMovement(true);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

    Mesh->SetMobility(EComponentMobility::Movable);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));

    Mesh->BodyInstance.bUseCCD = true;
}

void ABallCore::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABallCore, CarrierPawn);
}

// [인터페이스 구현] 헤더에 선언된 _Implementation 함수 정의
void ABallCore::OnInteract_Implementation(AActor* Interactor)
{
	// 상호작용 시도한 폰 확인
	if (APDPawnBase* PDPawn = Cast<APDPawnBase>(Interactor))
	{
		// 폰에게 줍기 요청 (Double Dispatch)
		PDPawn->Server_PickUpBall(this);
	}

	UE_LOG(LogTemp, Warning, TEXT("Core"));
}

void ABallCore::Server_SetCarrier(APawn* NewCarrier)
{
	if (!HasAuthority()) return;

	CarrierPawn = NewCarrier;
	HandleCarrierChanged();
}

// [Linker Error 해결] 헤더에 선언된 함수 구현 필수
void ABallCore::Server_ClearCarrier()
{
	if (!HasAuthority()) return;

	Server_SetCarrier(nullptr);
}

void ABallCore::Server_DropPhysics(const FVector& DropLocation, const FVector& Impulse)
{
	if (!HasAuthority()) return;

	CarrierPawn = nullptr;
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// 1. 위치 이동
	SetActorLocation(DropLocation, false, nullptr, ETeleportType::TeleportPhysics);

	// 2. [중요] 콜리전 프로필과 Enabled 설정을 먼저 확실하게 적용
	SetActorEnableCollision(true);
	Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 경고 해결의 핵심!

	// 3. 물리 시뮬레이션 켜기
	Mesh->SetSimulatePhysics(true);
	Mesh->SetEnableGravity(true);
	
	// 4. 물리 상태 초기화 (옵션)
	Mesh->WakeAllRigidBodies();

	// 5. [중요] 모든 설정이 끝난 뒤에 힘을 가해야 함
	Mesh->AddImpulse(Impulse, NAME_None, true);
}

void ABallCore::OnRep_CarrierPawn()
{
	HandleCarrierChanged();
}

void ABallCore::HandleCarrierChanged()
{
    if (CarrierPawn)
    {
        //SetReplicateMovement(false);
        UE_LOG(LogTemp, Warning, TEXT("HandleCarrierChanged 1"));
        Mesh->SetSimulatePhysics(false);
        Mesh->SetEnableGravity(false);
        SetActorEnableCollision(false); // 액터 자체 콜리전 끔
        Mesh->SetCollisionProfileName(TEXT("NoCollision"));

        if (APDPawnBase* PD = Cast<APDPawnBase>(CarrierPawn))
        {
            USkeletalMeshComponent* CharacterMesh = PD->GetSkeletalMeshComponent();
            FName SocketName = PD->BallSocketName;

            if (CharacterMesh)
            {
                bool bAttached = AttachToComponent(
                    CharacterMesh,
                    FAttachmentTransformRules::SnapToTargetNotIncludingScale,
                    SocketName
                );

                if (bAttached)
                {
                    SetActorRelativeLocation(FVector::ZeroVector);
                    SetActorRelativeRotation(FRotator::ZeroRotator);
                }
            }
        }
    }
    else 
    {
        UE_LOG(LogTemp, Warning, TEXT("HandleCarrierChanged 0"));
        DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

        // 2. 위치 동기화 다시 켜기
        //SetReplicateMovement(true);

        // 3. 콜리전 먼저 복구 (순서 중요!)
        // 액터 콜리전 켜기
        SetActorEnableCollision(true);

        // [핵심 해결] 프로필을 원래대로(PhysicsActor) 돌려놔야 땅에 닿고, 라인 트레이스에도 걸립니다.
        Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));
        Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // Query(트레이스) + Physics(물리) 둘 다 켜기

        // 4. 물리 켜기 (콜리전이 켜진 뒤에 해야 함)
        Mesh->SetSimulatePhysics(true);
        Mesh->SetEnableGravity(true);
        Mesh->WakeAllRigidBodies(); // 잠자던 물리 깨우기
    }
}