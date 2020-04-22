# Сборка проекта при помощи cmake

## Hello world

Создадим в рабочей директории два файла: `main.cc` и `CMakeLists.txt` (если вы находитесь в консоли, используйте команду `touch main.cc CMakeLists.txt`).

В файле `main.cc` напишем:
```cpp
#include <iostream>

int main(int argc,char** argv)
{
  std::cout<<"Hello world"<< std::endl;
  return 0;
}
```
А в файле `CMakeLists.txt`:
```cmake
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(pi-decay)
add_executable(pi-decay main.cc)
```
Разберем, что значат эти строки:
 * `cmake_minimum_required(VERSION 3.8 FATAL_ERROR)` --- устанавливает минимальную версию cmake используемую в проекте.
 * `project(pi-decay)` --- задает имя проекта, которое используется как `cmake`, так и внешними программами, например, IDE.
 * `add_executable(pi-decay main.cc` ---  добавляет задание на создание исполняемого файл  с именем `pi-deacy`, используя код из файла `main.cc`.

 Теперь попробуем собрать проект, для этого мы создадим отдельную директорию в которой будет происходить сборка проекта, такую директорию обычно называют `build`. Никогда не собирайте проект прямо в той же директории где лежат файлы с кодом --- запаритесь потом отделять зерна от плевел, создайте поддиректорию, её при необходимости будет легко удалить.
 
 ```bash
 mkdir build && cd build
 ```
Затем запустите `cmake` в директории `build`, указав путь к директории с кодом.
```
cmake path/to/code
```
`cmake` сгенерирует файл `Makefile`, который будет использован программой `make` для сборки проекта. Выполните `make` в папке `build`
```
make
```
Готово, в директории должен появится файл `pi-decay`, который можно запустить:
```
./pi-decay
```

## Установка GEANT4

TODO()

## Подключение GEANT4 к нашему проекту

В первую очередь нужно активировать переменные окружения необходимые для работы GEANT4. Для этого нужно выполнить скрипт `geant4.sh`:
```bash
source path/to/geant4/instalation/geant4.sh
```
Для того чтобы не выполнять эту команду каждый раз, можно добавить её в файл `~/.bashrc`. Тогда она будет автоматически выполнятся при каждом запуске терминала. Если вы хотите чтобы эти переменные окружения унаследовали графические приложения (например, ваша IDE), нужно или запускать их из консоли, или проводить настройку в соответствии с вашим графическим окружением.

Теперь добавим GEANT4 в наш проект, отредактировав файл `CMakeLists.txt`:
```cmake
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(pi-decay)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})

add_executable(pi-decay main.cc)
target_link_libraries(pi-decay ${Geant4_LIBRARIES})
```
Разберем внесенные модификации:
 * `option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON` --- включает поддержку интерактивной и графической сессий.
 * `find_package(...)` --- автоматически ищет и подключает cmake-модули GEANT4 (не сами библиотеки!),  что бы оно работало, должно быть выполнен скрипт `geant4.sh`.
 * `include(${Geant4_USE_FILE})` --- подключает заголовочные файлы GEANT4.
 * `target_link_libraries(pi-decay ${Geant4_LIBRARIES})` --- динамически присоединяет к нашему исполняемому файлу библиотеки GEANT4.

Для того что бы проверить что GEANT4 подключен правильно отредактируем наш файл `main.cc`:
```cpp
#include "G4RunManager.hh"

int main(int argc, char** argv)
{
  G4RunManager* runManager = new G4RunManager;
  delete runManager;
}
```
Пересоберите проект и запустите исполняемый файл, что бы убедится что вы все сделали правильно.

## Подключаем дополнительные файлы и создаем заготовку для моделирования
Пока мы создали проект состоящий только из одного файла с кодом, однако в реальности код проект распределяется по нескольким файлам, которые отвечают за какую отдельную часть проекта. Например, для того чтобы начать симуляцию в GEANT4 пользователь обязан самостоятельно описать геометрию своего эксперимента, задать генератор первичных событий и выбрать используемую физику. Давайте создадим заготовку содержащие отдельные файлы для геометрии и генератора первичных частиц и подключим их к нашему проекту. 

Для начала создадим две директории: `include` в которой мы будем размещать заголовочные файлы и `src` в которой мы будем размешать файлы с реализацией.

В директорию `include` мы поместим файлы `DetectorConstruction.hh`  и `PrimaryGeneratorAction.hh` со следующим содержанием (его суть мы разберем в следующих главах).

`DetectorConstruction.hh`:

```cpp
#ifndef PI_DECAY_DETECTORCONSTRUCTION_HH
#define PI_DECAY_DETECTORCONSTRUCTION_HH

#include <G4VUserDetectorConstruction.hh>

class DetectorConstruction : public G4VUserDetectorConstruction{
public:
    G4VPhysicalVolume *Construct() override;

};

#endif //PI_DECAY_DETECTORCONSTRUCTION_HH
```

`PrimaryGeneratorAction.hh`:

```cpp
#ifndef PI_DECAY_PRIMARYGENERATORACTION_HH
#define PI_DECAY_PRIMARYGENERATORACTION_HH

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ParticleGun.hh>

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction{
public:
    PrimaryGeneratorAction();
    void GeneratePrimaries(G4Event *anEvent) override;
private:
    G4GeneralParticleSource *particleSource;
};

#endif //PI_DECAY_PRIMARYGENERATORACTION_HH
```

А в директории `src` поместим файлы `DetectorConstruction.cc` и `PrimaryGeneratorAction.cc`:

 `DetectorConstruction.cc`:
 ```cpp
 #include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4PVPlacement.hh>
#include "G4LogicalVolume.hh"
#include "DetectorConstruction.hh"
#include "G4SystemOfUnits.hh"

G4VPhysicalVolume *DetectorConstruction::Construct() {
    // Get nist material manager
    auto nist = G4NistManager::Instance();

    // Option to switch on/off checking of volumes overlaps
    G4bool checkOverlaps = true;

    //
    // World
    //
    G4double world_sizeXY = 20*meter;
    G4double world_sizeZ  = 30*meter;
    G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

    G4Box* solidWorld =
            new G4Box("World",                       //its name
                      0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size

    G4LogicalVolume* logicWorld =
            new G4LogicalVolume(solidWorld,          //its solid
                                world_mat,           //its material
                                "World");            //its name

    G4VPhysicalVolume* physWorld =
            new G4PVPlacement(0,                     //no rotation
                              G4ThreeVector(),       //at (0,0,0)
                              logicWorld,            //its logical volume
                              "World",               //its name
                              0,                     //its mother  volume
                              false,                 //no boolean operation
                              0,                     //copy number
                              checkOverlaps);        //overlaps checking
    return physWorld;
}
```

`PrimaryGeneratorAction.cc`:
```cpp
#include "PrimaryGeneratorAction.hh"

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
    particleSource->GeneratePrimaryVertex(anEvent);
}

PrimaryGeneratorAction::PrimaryGeneratorAction()
        : G4VUserPrimaryGeneratorAction(){
    particleSource = new G4GeneralParticleSource();
}
```
Отредактируем файл `main.cc`, чтобы использовать эти новые файлы:
```cpp
#include <DetectorConstruction.hh>
#include <PrimaryGeneratorAction.hh>
#include <QGSP_BERT.hh>
#include <G4VisManager.hh>
#include <G4VisExecutive.hh>
#include <G4UIExecutive.hh>
#include <G4UImanager.hh>
#include "G4RunManager.hh"

int main(int argc, char **argv) {
    G4UIExecutive *ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }
    G4RunManager *runManager = new G4RunManager;
    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new QGSP_BERT);
    runManager->SetUserAction(new PrimaryGeneratorAction());
    runManager->Initialize();

    G4VisManager *visManager = new G4VisExecutive;
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    // Process macro or start UI session
    //
    if (!ui) {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    } else {
        // interactive mode
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;
}
```


Теперь давайте подключим эти файлы к нашему проекту, добавив эти строчки в файл `CMakeLists.txt`:
```cmake
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
```
Давайте разберем что они делают:

* `include_directories(${PROJECT_SOURCE_DIR}/include)` --- подключает заголовочные файлы из указанной директории, `${PROJECT_SOURCE_DIR}` это перменная указывающая путь к корневой директории проекта.
* `file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)` --- создает переменную `sources`, которая содержит список файлов соответвующих заданной маске `${PROJECT_SOURCE_DIR}/src/*.cc`. 

Теперь изменим команду `add_executable(pi-decay main.cc` на 
`add_executable(pi-decay main.cc ${sources})`, для того чтобы добавить файлы из списка `sources` как нашему исполняемому файлу. Скомпиируйте проект, что бы убедится что вы правильно все подключили.