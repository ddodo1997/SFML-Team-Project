<div align="center">
  <h2>🎮 C++ SFML 팀 프로젝트: Hotline Miami 모작 🎮</h2>
</div>

<div align="center">
  <img width="600" src="https://github.com/user-attachments/assets/27894175-f3c7-4280-969c-4ca582b49c69"/>
</div>

# 목차
1. ### [프로젝트 개요](#프로젝트-개요)
2. ### [기술 스택](#기술-스택)
3. ### [구현 요소](#구현-요소)
4. ### [구현 상세](#구현-상세)


## 프로젝트 개요
- C++과 **SFML**을 활용한 탑다운 액션게임 **Hotline Miami** 모작.
- 첫 팀 게임프로젝트로써 팀원간의 협업과 Git 사용 전문화를 목표로 프로젝트를 진행함.

## 기술 스택

<p align="left">
  <img src="https://img.shields.io/badge/C++-00599C?style=flat-square&logo=c%2B%2B&logoColor=white"/>
  <img src="https://img.shields.io/badge/SFML-74C365?style=flat-square&logo=sfml&logoColor=white"/>
  <img src="https://img.shields.io/badge/Visual_Studio-5C2D91?style=flat-square&logo=visual%20studio&logoColor=white"/>
  <img src="https://img.shields.io/badge/Git-F05032?style=flat-square&logo=git&logoColor=white"/>
  <img src="https://img.shields.io/badge/GitHub-181717?style=flat-square&logo=github&logoColor=white"/>
  <img src="https://img.shields.io/badge/SourceTree-0052CC?style=flat-square&logo=sourcetree&logoColor=white"/>
</p>

## 구현 요소

|[<img width="450" src="https://github.com/user-attachments/assets/0aeeea80-a694-4d62-b0af-774bcdc1e084"/>](#UI)|[<img width="450" src="https://github.com/user-attachments/assets/87f1ff13-a9c2-4cb5-b3c6-fe163b32ce14"/>](#플레이어)|
|:---:|:---:|
| [UI](#UI) | [플레이어](#플레이어)|
|[<img width="450" src="https://github.com/user-attachments/assets/f4035101-e2c0-4832-83f2-df1cb01cd703"/>](#적군)|[<img width="450" src="https://github.com/user-attachments/assets/23eaec95-f3d8-4edc-b7b9-30d470263e5a"/>](#맵-에디터)|
| [적군](#적군) | [맵 에디터](#맵-에디터)|

## 구현 상세
1. ### UI
  - Title
  - Pause
  - Status

2. ### 플레이어
  - 무기별 애니메이션, 판정 상이
  - 적이 드랍한 무기 노획
  - 무기 던지기
  - 적 처형

3. ### 적군
  - 상태 패턴을 활용하여 구현
  - 원/근거리 무기, 상태를 전이하며 행동을 결정
  - 플레이어의 소음을 감지해 소음 발생지로 길찾기 구현

4. ### 맵 에디터
  - 타일, 벽, 적, 무기, 보스, 플레이어 스폰위치, 스테이지 끝 위치 배치 가능
  - 만들어진 맵은 json형태로 저장 및 불러오기 가능

