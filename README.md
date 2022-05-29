# Snake game 완성본 하는법

1. End_snake.zip을 다운로드하고 전부 압축해제한다.
2. 터미널을 열고 make를 친다
3. ./Main 을 치면 실행이 된다.
4. Good!




----------------------------
2022.05.29 주요 update 내용


1. map을 map.h파일과 map.cpp파일로 따로 두어서 전역변수로 설정함(Gate또한 같은 파일에서 관리)
- map을 stage에 종속되어 업데이트 해주는 map_init(int stage) 함수를 만듦
- map을 전역변수로 두어서 관리함으로서 Item과 Gate를 만드는 구조를 보다 간결하게 구현함.




2. item 생성 방법 개선
- 기존에는 growth와 poison이 지속적으로 1개 1개씩 나오도록 구현했는데
- ppt에서 growth와 poison을 합쳐서 최대 3개까지 등장할 수 있도록 구조를 변경함
- 생성되는 아이템 중에서 growth와 poison중 선택되는 것은 랜덤으로 1/2 확률로 등장하게 함(rand함수 이용)
- 
- 이전에는 game이 진행되는 main의 while문에서 일정시간(0.5초)마다 count를 세어서 count가 30이 되면 아이템이 사라지는 구조였는데
- 3개의 아이템에 대해서 각각 ItemValue 라는 struct를 이용해서 y,x좌표, 생성된 시간(clock_t) 값을 저장해서
- while문을 돌때마다 실제 생성된 시간과 현재 시간의 값을 비교해서 일정시간이 지나면 그 아이템이 없어지도록 구현했음
- 부가적으로 기존에는 Snake의 위치를 기준으로 Item이 생성될 수 있는지 판단했는데
- map[x][y] 좌표를 확인한 후 빈칸이면 생성될 수 있도록 구현함

- 이전에는 Snake가 Item을 획득하면 바로 Item이 생성되는 구현을 했는데
- 즉시 생성될 필요는 없고 텀을 두고 생성되는 경우도 만들기 위해서 기존에 while문을 통해 해당 위치에 생성될 수 있는지 확인했는데
- 이를 if문으로 변경해 특정 위치에 생성될 수 없으면 그대로 생성이 되지 않고 다음 turn으로 넘어갈 수 있도록 구현함





3. Gate 생성 방법 개선
- map을 전역변수로 두면서 map_init(int stage)를 통해 각 stage 별로 다른 wall 구조를 생성하도록 구현했음
- 이에따라 Gate 생성 방법도 Item과 유사하게 전체 맵을 보면서 immune wall이 아닌 wall의 위치 2개를 random하게 찾아 Gate를 생성하도록 만듦
- 기존에 Gate를 이동할 때 반대편 Gate에 머리가 생기는 것을 Gate 이후 다음 칸 부터 생기도록 개선
- immune wall(Gate가 생길수 없는 벽 == 사방이 막혀있는 벽)과 wall구분하는 방법은 map_init함수로 일단 모든 벽을 wall(2)로 설정한 후
- wall 마다 검사하면서 나갈 수 있는 통로가 하나도 없다면 immune wall(1)로 재설정 해줌




4. Point와 Mission 달성 여부를 나타내기 위한 Point, Mission Class 생성
- 두 클래스 모두 setter getter method를 두어서 snake가 move함수를 호출할 때마다 변동사항을
- 두 객체의 member instance에 update해주고
- 0.5초 간격으로 update되는 값을 바탕으로 display해줌




5. Display.h Display.cpp를 통해 Point와 Mission의 정보를 가시적으로 나타내주는 함수를 만듦
- 더불어 해당 Stage를 clear하거나 Snake가 죽는 경우, 아니면 모든 Stage를 clear해서 게임이 끝나는 경우
- notice 창을 만들어 게임의 상태를 알려줌





------------------------------------------------------------------------------------------------


# snake

#### 0 : map

#### 1 : wall

#### 3: Head of Snake

#### 4: Body of Snake

#### 5 : Growth Item

#### 6 : Poison Item

#### 7 : Gate

### main 함수에서 map 변경시 -> snake가 지나가는 경우 초기화

### while 함수에서 map 변경시 -> 고정적으로 유지
