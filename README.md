# OOP_Dodge_Game
2019 객체지향프로그래밍 과제로 directX를 사용하여 제작한 dodge game

* Our Name and StudentID

	Name	StudentID
	김현주	20184754
	이현주	20184060


* Compilation Environment including compiler type and version

개발 환경: 2019 visual studio, DirectX 12
실행 방법1: VirtualLego.dsw 파일을 visual studio로 실행시킨다.
	프로젝트의 속성에 들어간다.
	VC++디렉터리에 포함 디렉터리를 “(DirectX SDK가 있는 path)\Include”로 적용한다.
	VC++디렉터리에 라이브러리 디렉터리를 “(DirectX SDK가 있는 path)\Lib\x86”로 적용한다.
	C/C++의 코드생성의 함수 수준 링크 사용을 “예”로 변경한다.
	“디버깅” 혹은 “디버그하지 않고 시작”을 한다.

실행 방법2: proj3폴더의 VirtualLego.exe파일을 실행한다.

* Summary of Our Code Modification

기존의 당구 게임 소스 코드를 응용하여 "닷지"라는 게임을 구현하였다. "닷지"라는 게임은 사방에서 날아오는 공을 피해서 오래 살아남는 것을 목표로 하는 게임이다. 날아오는 공과 맞닿으면 게임은 종료된다.

위의 게임을 구현하기 위해 기존의 class와 method들을 수정하고 뿐만 아니라 새로운 class와 method를 생성/호출하였다.

ex) 

- 게임 환경과 객체들을 다루기 위해 추가한 spacebar, arrow key 등 명령키들의 기능 구현

- 파란공은 주어진 코드와 다르게 본 게임에선 마우스로 컨트롤이 되면 안되므로 해당 코드 부분을 삭제

- 게임 초기화를 위한 공 생성 개수, 좌표, 색상, 속도 등의 속성 변경

- 게임 룰 설명과 게임 결과 출력, 타이머 출력을 위한 CText class 생성 및 활용

- 실감나는 게임 환경을 위해 삽입한 효과음을 위한 CSound class 생성 및 활용
(CSound class를 성공적으로 구현하기위해 d3dUtility.h 파일과 .cpp파일을 적절히 수정함)

- 게임 시작 및 게임 종료 조건에 맞도록 각 공 객체들의 물리적 action을 구현하기 위한 hasIntersected, hitBy 등의 method를 수정

- 버그 수정을 위한 코드 추가 (가끔 발생하는 공들의 벽 뚫음 및 게임 화면 out 현상을 방지하여 게임 난이도에 문제가 발생하지 않도록 적절하게 수정)

- 게임의 특성에 맞게 카메라 앵글을 위에서 거의 수직적으로 바라보는 것으로 수정

- 카메라의 앵글을 수직적으로 변화시키다 보니 전등이 게임 화면을 가려 시각적 불편함을 주어 전등 위치를 시야보다 더 높게 수정하여 불편함을 없앰

- 게임의 특성상 마우스로 인한 카메라 앵글 변화가 필요하지 않기 때문에 해당 코드 삭제

위의 예시 외에 닷지 게임을 성공적으로 구현할 수 있도록 기존의 소스 코드를 적절하게 수정



