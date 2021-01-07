# ossa
Open Source Social Alience

* keemo.src - папка для тестовых кодов на языке JS под ботов. Реализуем позже. Куда позже
* src - код ядра. Основной функционал
* network - код для работы с сетью (логично, да?)
* ui - код для UI (логично, да?)

## Скачать исполняемый файл
[Сам чат](https://drive.google.com/file/d/1a_IDv1pZulpkaI3KB8KAmspbyNlafeY_/view?usp=sharing)

[Дистрибутив для скачивания](https://drive.google.com/file/d/16fniIvitGigYiTRNTXNFYnpDqnDOg7g2/view?usp=sharing)


## Задачи на Январь/Февраль
### Core, API
- [x] решить баг в функции loadMLFromFile, line 175. SIGSEGV (bug issue #0001)
- [x] допилить loadULFromFile
- [ ] Решить баг в функции loadMLFromFile с ошибочными символами в начале каждого сообщения (bug issue #0002)
### Network
- [ ] допилить loadRolerFromFile
- [ ] Разработать схему объединения клиентов в блокчейн
- [ ] Разработать алгоритм шифрования и протокол
### UI
- [x] придерживаться VIM-подобного порядка комманд
- [ ] перейти на curses вариант
- [ ] Сделать backdround daemon для чтения сообщений из терминала командой в терминале
