


#include "ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CharacterInstanceComponent.h"
#include "DSSpellData.h"
#include "DSGameMode.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = CollisionComp;
	CollisionComp->SetGenerateOverlapEvents(true);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnProjectileOverlap);
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectileBase::OnProjectileHit);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AProjectileBase::Initialize(UCharacterInstanceComponent* InCaster, UDSSpellData* InSpellData)
{
	Caster = InCaster;
	SpellData = InSpellData;

	// 시전자 본인과의 자기 충돌만 무시 — Pawn 채널 전체를 Ignore하면 진짜 타겟도 못 맞으니 그렇게 하지 말 것
	if (Caster && Caster->GetOwner())
		CollisionComp->IgnoreActorWhenMoving(Caster->GetOwner(), true);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::OnProjectileOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;

	UCharacterInstanceComponent* OtherCharacter = OtherActor->FindComponentByClass<UCharacterInstanceComponent>();
	if (!OtherCharacter) return; // 캐릭터가 아님(벽 등) — OnProjectileHit에서 처리

	if (!Caster || !Caster->IsHostileForParam(OtherCharacter))
		return; // 아군이거나 판정 불가 — 무시하고 계속 비행

	Caster->GetDSGameMode()->SwitchFocus(this, Caster);

	if (SpellData)
		OtherCharacter->ReceiveSpellDamage(Caster, SpellData);

	Destroy();
}

void AProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 캐릭터면 OnProjectileOverlap에서 이미 처리됨 — 여기선 벽 등 비캐릭터 충돌만 소멸 처리
	if (OtherActor && OtherActor->FindComponentByClass<UCharacterInstanceComponent>())
		return;

	Caster->GetDSGameMode()->PopFocus(this);
	Destroy();
}


