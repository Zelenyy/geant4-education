## cmake

### Структура проекта

Немного поговорим о том, как не надо программировать. Типичная ошибка это складывать весь код в один файл, как мы сделали в предыдущем уроке. Правильное распределение исходного кода по файлам упрощает понимание кода и работу с ним. К сожалению в C++ нет общепринятого метода распределения кода по отдельным файлам и директориям проекта, хотя есть некоторые рекомендации. Чаще всего лучше делать так, как принято в вашей команде, или же так, как принято в фреймворке с которым вы работаете (это позволяет унифицировать проект и облегчает обмен проектами).

Так что мы возьмем код из предыдущего проекта, и распределим его по отдельным файлам, исходя из рекомендаций, приведенных ниже. Отмечу, что это именно рекомендации по внешнему виду, они не влияют на корректность программы, но обеспечивают комфортную работу.

- В c++ файлы с кодом разделяются на заголовочные файлы (header, ), содержащие определения классов и функций, и собственно файлы с исходным кодом (source, сc-файл), содержащие реализацию функций и методов класса.
- Нет общепринятого стандарта на расширения файлов. Наиболее распространнённые расширения для заголовочных файлов это `.hh`, `.hpp`, `.hp`, `.h++`, `.hxx`,`.icc`,`.inl`,`.tcc`, для файлов с исходниками `.cc`,`.cpp`,`.cp`,`.cxx`,`.i`,`.ii`,`.m`,`.mm`. В GEANT4 используется `.hh` и `.cc`, этой нотации я и буду следовать, также в дальнейшем для краткости я буду употреблять слова hh-файл и сc-файл для соответствующего типа файла.
- Также нет общепринятого стандарта на расположение файлов в директории (в отличии от, например, Java и Python, где есть определенные требования к структуре проекта). В GEANT4 принято hh-файлы хранить в директории `include`, а cc-файлы (кроме файла с функцией `main`) в директории `src`. В качестве примера приведу структуру проекта после того, как мы корректно оформим программу из предыдущего урока:
```
.
├── include
│   ├── ActionInitialization.hh
│   ├── DetectorConstruction.hh
│   └── PrimaryGeneratorAction.hh
├── src
│   ├── ActionInitialization.cc
│   ├── DetectorConstruction.cc
│   └── PrimaryGeneratorAction.cc
├── CMakeLists.txt
├── init_vis.mac
└── main.cc
```
- Для каждого класса нужно писать отдельную пару hh- и cc-файлов. В заголовочных файлах желательно писать только определение класса, без реализации методов (что практически никем не соблюдается). Название файлов до расширения обычно делают совпадающим с именем класса. Для имен классов в GEANT4 принят PascalCase, то есть имя класса состоит из слов без разделителей, каждое слово начинается с заглавной буквы, например `PrimaryGeneratorAction`. 
- Прочие файлы распределяются удобным для вас образом, я обычно выделяю отдельные директории для gdml-файлов, макросов с описанием источников, управляющих макросов.

В следующем параграфе мы обсудим как организовать файл `CMakeLists.txt`, что бы он собирал наш код в работающую программу.

### Система сборки cmake

Итак, чтобы не компилировать каждый файл по отдельности и потом вручную не собирать их в одну программу, люди сделали набор инструментов делающих это автоматически.
В частности для сборки проектов на C/C++ в linux принято использовать набор утилит autotools, главная из которых `make`. Эта программа вызывается из командной строки, и вызванная без аргументов ищет makefile - файл содержащий инструкции по сборке проекта. Однако оказалось, что процесс написания этого makefile тоже можно автоматизировать, и люди создали cmake - утилиту генерирующую makefile, по более упрощенному описанию. Таким образом сборка происходит в два этапа, сначала cmake генерирует makefile, затем make собирает программу.
Сборка происходит обычно следующим образом. Мы создаем директорию (обычно с именем `build`) для того, чтобы потом можно было одним движение удалить весь мусор остающийся после сборки, и в этой директори вызываем `cmake`, в качестве аргумента передав путь до файла `CMakeLists.txt`:
```bash
cmake path/to/CMakeLists.txt

```
Также мы можем вызвать команду `ccmake` и произвести настройку сборки, как мы это делали, когда устанавливали GEANT4.
После того, как был удачно сгенерирован makefile, мы вызываем `make` и смотрим как собирается наша программа.

Теперь рассмотрим содержимое файла `CMakeLists.txt`

```cmake
# Задаем минимальныю версию cmake
CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)
# Задаем имя проекта
PROJECT(lesson02)

# Будет успешно если путь к cmake модулям GEANT4 будет в переменных окружения системы
# Данная команда позвляет автоматически подключать билиотеки установденные в систему, их разработчки предусмотрел такую возможность
FIND_PACKAGE(Geant4 REQUIRED ui_all vis_all)

# Эпик фейл если GEANT4 не найден
# Так же это пример, как можно использовать условные операторы при сборке
IF(NOT Geant4_FOUND)
    MESSAGE(FATAL_ERROR "Geant4 not found!")
ENDIF(NOT Geant4_FOUND)

# Подключаем заголовочные файлы GEANT4
INCLUDE(${Geant4_USE_FILE})
# Подключаем заголовочные файлы нашего проекта
# ${PROJECT_SOURCE_DIR} - обращение к переменной cmake, содержащей путь до
# файла CMakeLists.txt
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

#Cоздаем переменную содержащую пути к исходним файлам
FILE(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

# Создаем исполняемый файл
ADD_EXECUTABLE(lesson02.exe lesson02.cc ${sources})

# Линкуем исполняемый файл и внешние билиотеки
TARGET_LINK_LIBRARIES(lesson02.exe ${Geant4_LIBRARIES})

# Копируем необходимые нам файлы из папки проекта в папку где происходит сборка
# Для этого мы создаем переменную хранящую один или несколко файлов
set( GEANT4_SCRIPTS init_vis.mac)
# И с помощью цикла копируем каждый файл
foreach(_script ${GEANT4_SCRIPTS})
    configure_file(
            ${PROJECT_SOURCE_DIR}/${_script}
            ${PROJECT_BINARY_DIR}/${_script}
            COPYONLY
    )
endforeach()

# Описываем поведение для команды make install
install(TARGETS lesson02.exe DESTINATION bin)
```