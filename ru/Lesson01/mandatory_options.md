### Обязательные пользовательские настройки: геометрия, генератор событий и физический лист

Как мы уже сказали GEANT4 это не программа для моделирования, а платформа \(фреймворк\) для написания программ моделирования. Фактически это устроенно следующим образом: в GEANT4 определены абстрактные классы, для которых пользователю нужно самостоятельно реализовать класс-наследник в своей программе. Эти классов позволяют ввести информациию о конкретной части модели, оказать влияние на процесс моделирования или провести регистрацию результатов моделирования.

Для части абстрактных классов необходимо реализовать класс-наследник в обязательном порядке - это классы отвечающие за описание геометрии модели, генерацию первичных частиц и описание физических процессов. В данном уроке мы создадим простейшую однофайловую программу минимально реализующую все обязательные требования, а в последующих уроках мы рассмотрим каждый обязательное требование более детально \(код к данному уроку лежит в [директории examples/lesson01](/)\).

Поскольку мы занимаемся Монте-Карло моделированием, то важной частью является генератор случайных чисел:

```cpp
#include <random>

...

int main() {
    ...
    random_device rd;
    uniform_int_distribution<long> uid(0, LONG_MAX);
    G4long seed = uid(rd);
    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    G4Random::setTheSeed(seed);
    ...
```

В данной части программы мы выбираем какой из генераторов случайных чисел нам использовать \(GEANT4 предлагает несколько на выбор\) и устанавливаем его начальное состояние переменной `seed`. Задавать одно и тоже начальное состояние генератора бывает полезно при отладке программы, поскольку оно позволяет повторить псевдослучайную последовательность выдаваемую генератором и тем самым исключить влияние случайности. В рабочих симуляциях напротив, необходимо рандомизировать выбор начального состояния генератора, для этого в данном примере используется класс `random_device` - он позволяет сгенерировать случайное число используя пул энтропии создаваемой шумами в электронике.

Упрп\авление симуляций GEANT4 выполянет с помощью набора так называемых классов-менеджеров (Их можно отличить по наличию в названии класса слова `Manager`), отвечающих  за различные аспекты симуляции и делающих это на разных уровнях. Менеджером верхнего уровня является `G4RunManager`, в с помощью которого мы зарегистрируем созданные пользователем классы и будем управлять запуском _сеанса_ симуляции:
```cpp
#include "G4RunManager.hh"

...

int main() {
    ...
    auto runManager = new G4RunManager;
    ...
```
 Для начала подключим физический лист - объект класса отвечающего за набор физических процессов в которых участвуют элементарные частицы в симуляции. В качестве физического лист вы используем один из готовых листов `FTFP_BERT`, который включает в себя набор электромагнитной и андронной физики, обеспечивающий достаточную точность если ваша симуляция не предпологает каких то сверхточных вычислений или сложных физических моделей (например не рекомендуется данный набор если вы хотите работать с точными симуляциями мягкого рентгена).
 Характерный прием который используется во многих методах GEANT4 класов --- это прием в качесве входного параметра целочисленной переменной (`verbose`) отвечающий за подробность текстового вывода о работе системы (0 соотвесвует наименне подробному выводу или его отсвутвию). Покдлючение физического листа осущетсвляется вызовом метода `SetUserInitialization` и передачей туда объекта физического листа.
```cpp
#include "FTFP_BERT.hh"

...

int main() {
    ...
    G4int verbose = 1;
    FTFP_BERT *physlist = new FTFP_BERT(verbose);
    runManager->SetUserInitialization(physlist);
    ...
```


Для геометрического объемв нет стандартных классов, поэтому соссздаим свой класс, описывающий очень простую ситуацию: кубическое пространсво с вакуумом, не содержащее никаких других геометрических объектов.
Для этого опишем наш класс `DetectorConstruction` унаследовав его от `G4VUserDetectorConstruction` и реализуем в нем вирутальный метод `Construct`. Геометрическая модель GEANT4 пердпологает три уровня абстрации и представляет собой  иерахическую структуру геометрических примитивов, примитив верхнего уровня называется _мировым объемом_ и указатель на этот мировой объем (переменная `world`) должен возращать метод `Construct`. 
```cpp
#include "G4SystemOfUnits.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    G4VPhysicalVolume *Construct() {
        auto nistMngr = G4NistManager::Instance();
        auto vacuum = nistMngr->FindOrBuildMaterial("G4_Galactic");
        G4Box *worldSolid = new G4Box("world",1000*meter, 1000*meter, 1000*meter);
        G4LogicalVolume *worldLogic = new G4LogicalVolume(worldSolid, vacuum, "worldLogic");
        G4VPhysicalVolume *world = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), worldLogic, "WorldPhys", 0, false, 0);
        return world;
    }
};
```
Далее мы должны зарегистрировать наш класс в `G4RunManager`, это делается аналогично регичтрации физического листа:
```cpp
#include 

...

int main() {
    ...
    auto massWorld = new DetectorConstruction();
    runManager->SetUserInitialization(massWorld);
    ...
```




Сздадим пушку

```cpp
#include "G4SystemOfUnits.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    G4ParticleGun* fParticleGun;
    PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction() {
        G4int nofParticles = 1;
        fParticleGun = new G4ParticleGun(nofParticles);
        // default particle kinematic

        G4ParticleDefinition* particleDefinition
                = G4ParticleTable::GetParticleTable()->FindParticle("proton");

        fParticleGun->SetParticleDefinition(particleDefinition);
        fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
        fParticleGun->SetParticleEnergy(3.0*GeV);
        fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
    }

    void GeneratePrimaries(G4Event *anEvent) override {
        fParticleGun->GeneratePrimaryVertex(anEvent);
        fParticleGun->SetParticleEnergy(2*GeV);
        fParticleGun->GeneratePrimaryVertex(anEvent);
    }

};
```

```cpp
#include 

...

int main() {
    ...
    auto gun = new PrimaryGeneratorAction();
    runManager->SetUserAction(gun);
    ...
```


```cpp
#include 

...

int main() {
    ...
    runManager->Initialize();
    runManager->BeamOn(10);
    ...
```









