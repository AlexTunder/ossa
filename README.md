![Open Source Social Alience](https://github.com/AlexTunder/ossa/blob/gh-pages/readme-heading-01.png)

**Открытый набор библиотек и утилит** для работы с чатами и создания собственного чата, а также отдельный вариант чата с открытым исходным кодом как для сервера, так и для клиентов

Данный **набор может оказаться хорошим шаблоном для написания вашего собственного чата**, но также может быть использован в каких-либо личных целях. 

Например, для написания простого и в тот же момент удобного чата для небольшой команды разработчиков  

## Скачивание

[Полностью готовый чат](https://drive.google.com/file/d/1a_IDv1pZulpkaI3KB8KAmspbyNlafeY_/view) доступен для скачивания прямо по ссылке. Тестовая версия чата пока не получила полноценной net-поддержки

Для скачивания прямо из консоли можно воспользоваться распределённой системой управления версиями git 

```
git clone https://github.com/AlexTunder/ossa.git
```
## Сборка

Сборка проекта совершается при помощи коренного makefile. ниже описаны таргеты для сборок

**lib** - Сборка всех статических библиотек

**slib** - сборка всех динамических библиотек

**test.net** - сборка теста network-api

**net** - сборка только нетворк-либ

**CBase** - сборка только базовой либы

**full** - все таргеты

## Архитектура набора

**core** — весь код ядра находится здесь, там же расположен основной функционал

**network** — работа с сетью осуществляется с помощью скриптов, расположенных здесь

**ui** — интерфейс пользователя и все его модификации располагаются здесь 

**tools** - различные вспомогательные утилиты для тестов и дебага

**lib** - скомпилированные библиотеки
   
----

## Новости и обновления

- Сетевая часть чата теперь будет написана на Go

## Задачи на февраль-март

Всё что нужно сделать описано ниже. Выполненные пункты помечаются галочкой
### Ядро и API
- [x] оформить ```core/base.h```
### Работа с сетью (Network)
- [ ] Разработать алгоритм шифрования и протокол
- [ ] Допилить API для серверов и сам сервер
### Интерфейс пользования (UI)
- [ ] Перейти на curses вариант
- [ ] Сделать backdround daemon для чтения сообщений из терминала командой в терминале
### Поддержка языков (Lang Support)
- [ ] Runtime переводчик
### Дополнительные задачи
- [ ] Сделать поддержку FileStream для Linux
- [ ] Создать GUI на electron/QT5/**GTK+**/BREND
- [ ] Создать движок для своих скриптов и/или запилить поддержку JS
### Организация
- [ ] Найти сервер или белый IP для установки тестового сервера
- [ ] Найти GUI-прогера, желательно на С++ (GTK+)

## Близкие релизы
   **Onion 1** - 12.03.21, готовая тестовая версия для чата

## Авторы и разработчики

* **AlexThunder** — *Работа с низкоуровневой частью* — [Github Profile](https://github.com/AlexTunder)

* **Andrezoo** — *Документация и front-end* — [Github Profile](https://github.com/andrezoo)

* **Bjorrain** — *Сетевая часть, безопастность и протоколы*
----
