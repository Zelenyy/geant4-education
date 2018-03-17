### Подготовка к работе и установка программ
Для работы я буду использовать ОС Ubuntu, однако большая часть излагаемого материала будет верна для любой платформы. 

Для того что бы использвать GEANT4 мы должны его установить. Есть три спосоьа сделать это:

* Скачать скомпилиронный GEANT4 (бинарную сборку) с  оффициального сайта.
* Скачать виртуальную машину с предустановленным GEANT4.
* Скачать исходный код и скомпилировать его самостаятельно.

Для практического использования наиболее правльным является последний пункт, по трем причинам. Во-первых, бинарная сборка не содержит некоторых бибилотек необходимых нам для изучения тем продвинутого уровня, а также можем не подойти к используемой вами платформе. Во-вторых, это позволяет поппрактиковать в сборке крупным библиотек на C++. В-третьих, исходный код нам все равно понадобится, как для лучшего понимания внутренннего устройтсва GEANT4, так и для решения наших прикладных задач.

Итак, мы скачали и распаковали архив с исходным кодом GEANT4. Для его сборки нам необходимы некоторые библиотеки (их список дам в руководстве по установке), так же нам потребеются некоторые утилиты

Рассмотрим процесс установки GEANT4.

    1. Скачиваем архив с [оффициального сайта](http://geant4.web.cern.ch/geant4/support/download.shtml).

    2. Создаем директорию GEANT4 в своем домашнем каталоге.
    3. Распаковываем в неё скачанный архив.
    4. В директории GEANT4 создаем папки build и install. В папке build мы будем собирать GEANT4, а в папку install устанавливать.
    Опционально: можно скачать data-файлы, и разместить их в директории data, что бы затем использовать их при установке. Это освершенно необязательно, так как GEANT4 может скачать их сам при сборке.
    
  5. Сборка без дополнительных опций: 
   1. Устанавливаем необходимые утилиты для сборки: компилятор С++, cmake и консольный конфигуратор ccmake, а ткаже необходимые библиотеки (для Ubuntu):
    sudo apt-get install g++ cmake cmake-curses-gui
    sudo apt-get install libexpat1 libexpat1-dev
    
    2. Переходим в директорию build:
    ```bash
    cd ~/GEANT4/build
    ```
    Открываем ccmake:
    ```bash
    ccmake ../geant4.10.03
    ```
    Здесь в качестве аргумента команды ccmake используется путь до диретории распакованной из скаченного нами архива с GEANT4. ccmake читает расположенной в этой директории файл CMakeLIsts.txt, для того чтобы настроить параметры сборки.
    После того как ccmake открылся, жмем клавишу c, если все нормально, то ccmake выдаст предупреждение об отсуствии data-файлов, которое можно смело проигнорировать. Так же может выдать ошибку если в системе не установлена какая-либо библиотека, тогда её надо доустановить.
    Если ccmake успешно сконфигурировался, отредактируем следующие параметры:

    CMAKE_INSTALL_PREFIX - по умолчанию он пытается утсановить GEANT4 в системную директорию, однако я предлагаю прописать здесь путь до папки install.
    GEANT4_INSTALL_DATA - установим значение ON, для того чтобы GEANT4 сам скачал data-файлы. Если у вас уже есть эти файлы, укажаите к ним путь в параметре GEANT4_INSTALL_DATADIR.

    Жмем клавишу c , до тех пор пока не появиться опция нажать клавишу g. Жмем g. ccmake создаст makefile и закроется.


    Собираем GEANT4:
```bash
    make -jN
    #где N количесвто ядер которое выхотит задействовать, посмотреть количесвто ядер можно в htop (apt-get install htop)
```
     Устанавливаем GEANT4:
     ```bash
    make install
    #если вы не изменяли параметр CMAKE_INSTALL_PREFIX, то используйте sudo make install, еще лучше воспользуейтесь checkinstall
```
    Устанавливаем переменные окружения:
    ```bash
    source /path/to/install/bin/geant4.sh
    #Данную команду нужно прописать в .bashrc для автоматического устанавление переменных среды.
```
    Проверяем работоспособность на примерах:
    ```bash
    cd ~
    mkdir temp
    cd temp
    mkdir build
    cd build
    cmake ../../GEANT4/install/share/Geant4-10.3.0/examples/basic/B1/ #путь до примера
    make
    ./exampleB1
    ```
    PROFIT
    Папку build мы не удалаем, так как она пригодится нам когда мы захотим пересобрать GEANT4 с другими опциями. Это позовлит нам в дальнейшем пересобирать только те файлы которые будут изменены.

Anaconda

Иногда при сборке cmake пытается использовать не системные библиотеки, а бибилиотеки anaconda. Это решается удаление пути к anaconda из переменной PATH. Или смотрим раздел Установа с помощью conda.
Сборка дополнительных опций
Подключение GDML

Для загрузки геометрии из внешних gdml-файлов, на потребуется подключить GDML.

    Устанавливаем необходимые библиотеки:
    aptitude install libxerces-c3.1 libxerces-c-dev
    Запускаем ccmake в папке build.
    Установим значение ON для параметра GEANT4_USE_GDML.
    Конфигурируем, генерируем, собираем и устанавливаем.

Подключение GUI

Для GUI будем использовать qt - установим значение параметра GEANT4_USE_QT равным ON. Если хотим использовать qt4, то установите значение параметра  GEANT4_FORCE_QT4 равным  ON.
```bash
aptitude install qt5-default
#или
aptitude install qt4-default libqt4-opengl-dev
```
Установка с помощью conda (cross-linux)

Если вы не имеет прав суперпользователя, или системные репозитории безнадежно устарели, или вы просто не хотите с ними связываться, то можно использовать пакетный менеджер conda для установки дополнительных библиотек.

     Устанавливаем Anaconda или Miniconda c официального сайта.

    Устанавливаем необходимые пакеты:
    conda install -c anaconda gcc
    conda install libgcc
    conda install -c anaconda xerces-c
    conda install -c anaconda cmake
    conda install -c anaconda qt # если ставили miniconda
    Конфигурируем, генерируем, собираем и устанавливаем.




