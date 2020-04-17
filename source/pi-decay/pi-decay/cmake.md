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
 * `add_executable(pi-decay main.cc` ---  добаляет задание на создание испольняемого файл  с именем `pi-deacy`, используя код из файла `main.cc`.

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
Для того чтобы не выполнять эту команду каждый раз, можно добавить её в файл `~/.bashrc`. Тогда она будет автоматчески выполнятся при каждом запуске терминала. Если вы хотите чтобы эти переменые окружения унаследовали графиеческие прихожения (например, ваша IDE), нужно или запускать их из консоли, или проводить настройку в соответсвии с вашим графическим окружением.

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
 * `option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON` --- включает поддержку инреактивной и графической сессий.
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
 

