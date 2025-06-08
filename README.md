10초 기준
NPC 수 : 200

최적화 On
CharacterMovementComponent
 - Count : 18,247
 - Total Inclusive Time : 1.2s
 - Total Exclusive Time : 3.2ms
SkinnedMeshComponent
 - Count : 18,247
 - Total Inclusive Time : 246.6ms
 - Total Exclusive Time : 2.8ms
 
최적화 Off
CharacterMovementComponent
 - Count : 49,894
 - Total Inclusive Time : 2.7s
 - Total Exclusive Time : 8ms
SkinnedMeshComponent
 - Count : 49,894
 - Total Inclusive Time : 839,4ms
 - Total Exclusive Time : 6.8ms
 
CharacterMovementComponent 많이 잡아먹는 지점
TickComponent()->PerformMovement()->PhysWalking()->FindFloor()->ComputeFloorDist()

UCharacterMovementComponent::ComputeFloorDist() 요약
- 캐릭터와 바닥 간 거리 계산
1. Sweep Test
- 캐릭터의 캡슐 컴포넌트의 높이의 절반을 기반으로 MakeCapsule()로 캡슐 생성
- 바닥으로 FloorSweepTest()를 진행해서 바닥이 있는지 확인
- 옆면과 땅바닥에 파고드는 것을 염두한 결과 저장
2. Line trace
- Sweep이 실패했을 경우 수행
- 캡슐 컴포넌트의 좌표(중앙)를 시작으로 바닥까지 라인 트레이싱