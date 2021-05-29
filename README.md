# 3D Renderer с нуля

![Screenshot](./screen.jpg)

## Как это собрать
### Под виндой
Для сборки нужен CMake и Visual Studio:
Команды для сборки:
```
cmake .
cmake --build .
```
Для работы нужды assimp и SDL, под x64_86 винду я их скомпилил и они должны автоматически скопироваться в папку с билдом
### Под не виндой
В теории можно, но я пока не пробовал + надо компилировать библиотеки

## Что осталось доделать

+ Реализовать управление через контроллеры, а не через main
+ Реализовать компонент для света и световые шейдеры
+ Выжить (опционально)
+ Тесты дока и прочие извращения
+ Скопировать к себе лицензии и ссылки на чужой код
## Done
+ Сделать клиппинг, потому что иначе интерполяция ломается при приближении к экрану
+ Избавиться от синглтона пробрасывать ядро в контроллеры как-нибудь по другому

## Управление
Сейчас в тестовой демке 1,2,3 - переключают перcонажей, AWDS двигают персонажей стрелки двигают камеру, LR - вращают персонажей