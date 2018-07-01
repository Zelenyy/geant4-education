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


```cpp
#include 

...

int main() {
    ...
    auto runManager = new G4RunManager;
    ...
```


В качестве физического лист вы используем один из готовых листов  
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



Создание геометрического объемв ограничимся мировым объемом  


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









