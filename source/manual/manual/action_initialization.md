## Урок 7: Инициализация пользовательских действий

### ActionInitiliaztion
Класс ActionInitliazation позволяет регистрировать пользовательские действия: одно из них --- создание генератора первичных частиц --- обязательно и будет рассмотрено в этом уроке, остальные необязательны и будут рассмотрены в следующем уроке.


Необязательные пользовательские действия регистрируются в классе ActionInitialization. Существует 5 типов пользовательских действий, различающихся уровнем, на котором происходит вмешательство пользователя в симуляцию. Соответственно пользователь может управлять симуляцией на уровне сеанса, события, трека , шага и появления новой частицы.

### Run
Вызывается в начале сеанса

### Event
Вызвается в начале и конце данного события

### Track
Методы данного класса вызываются перед началом моделирования трека частицы и по его окончанию

### Stack
Данный класс определяет, что будет происходить со вновь появившимися частицами --- в зависимости от критериев, заданных пользователем, трэку частицы присваивается один из возможных статусов, определяемых перечислением: 

Частица будет использована в данном событии
Частица будет уничтожена, а её кинетическая энергия будет засчитана в энерговыделение родительской частицы
Частица будет использована в следующем событии
 

### Step

Методы данного класса вызываются на каждом шаге



