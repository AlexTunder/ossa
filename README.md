# OSSA
Open Source Social Alience

* keemo.src - папка для тестовых кодов на языке JS под ботов. Реализуем позже. Куда позже
* src - код ядра. Основной функционал
* network - код для работы с сетью (логично, да?)
* ui - код для UI (логично, да?)

## Скачать исполняемый файл
[Сам чат](https://drive.google.com/file/d/1a_IDv1pZulpkaI3KB8KAmspbyNlafeY_/view?usp=sharing) и
[менеджер ресурсов](https://drive.google.com/file/d/16fniIvitGigYiTRNTXNFYnpDqnDOg7g2/view?usp=sharing) теперь доступны на Google Disk

## Фичи и новости
- Смена программы с интерфейсом: osmic переходит на иную архитектуру
- Динамическая локализация

## Задачи на Январь/Февраль
### Core и API
- [x] Решить баг в функции loadMLFromFile, line 175. SIGSEGV (bug issue #0001)
- [x] Допилить loadULFromFile
- [x] Решить баг в функции loadMLFromFile с ошибочными символами в начале каждого сообщения (Bug issue #0002)

- [x] Допилить loadRolerFromFile
- [x] Реализовать loadULFromStream
- [x] Реализовать loadMLFromStream
- [x] Реализовать loadRolerFromStream
### Network
- [ ] Разработать схему объединения клиентов в блокчейн
- [ ] Разработать алгоритм шифрования и протокол
### UI
- [x] Придерживаться VIM-подобного порядка комманд
- [x] Поддержка языковых пакетов
- [ ] Перейти на curses вариант
- [ ] Сделать backdround daemon для чтения сообщений из терминала командой в терминале
### Languages
- [ ] Cделать режим динамического хранения локализации
- [ ] Runtime переводчик
- [x] Решить баг с загрузкой языка кодом из [languages.c](src/languages.c)
### Extra задачи (неприоритетные)
- [ ] Сделать модуль ядра для чата
- [ ] Сделать поддержку FileStream для Linux
- [ ] Создать GUI на electron/QT5/**GTK+**/BREND
- [ ] Создать движок для своих скриптов и/или запилить поддержку JS
