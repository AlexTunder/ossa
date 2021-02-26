# intro
OSMIC try use vim-like command line, so some features will be similiar

For all chat-inside commands use ':' prefix. For example: ':exit'

If you want to use some shell commands from chat use '$' prefix. for example: '$ip a'

For fast navigation use :help #[command]
# Basic commands
| Name | Syntax | Description |
| ---- | ------ | ----------- |
| :exit | :exit | Exit from chat |
| :drop | :drop `(-chat [filename]) (-users [filename]) (-roles [filename])` | write chat data to 3 files. You can manage filenames with opcional arguments. By default using 'users.list', 'roles.list' and [time of 1st message]-[time of last message] files |
| :useradd | :useradd `username` | Add new user to user list |
| :roleadd | :roleadd `role name` | Add new role to roler |
| :view | :view `all/tail n` | view all chat history or only tail of `n` messages |
| :inrole | :inrole `userid` | Check user with `userid` for role |
| :ban | :ban `userid` | Ban user with `userid` |
| :long | :long `stop-char` | enable multi-line typing while you not press `stop-char`. Use `~` as stop-char, cuz it's best one |
| :load | :load `(-chat [filename]) (-users [filename]) (-roles [filename])` | read chat data from 3 files. You can manage filenames with opcional arguments. By default using 'users.list', 'roles.list' and [time of 1st message]-[time of last message] files |
| :lsuser | :lsuser | View full list of with their's IDs |
| :lsrole | :lsrole `roleid` | view full info about role(s). If you type just ':lsrole' it's will displayed full list of roles woth full info |
| :set | view next pont | allow to manage roles, users, access and others. View more in next point |
| :dload | :dload `tag` `name` | Download `name` package with `tag` |
| :env-ctl | :env-ctl `update` | Manage eviroments |
| :conn | :conn `ip` `port` `(-p @proto)` | Connect to server on `port` and bind this chat with it. In future you'll can use `-p` flag for choosing protocol |
| :disconn | :disconn | Disconnect current chat from server |
| :mkchat | :mkchat `name` | Make new chat with `name` |
| :lschat | :lschat | Get all stat about all chats |


# Set
For every of this command you should to use next structure: ```:set parameter```. Every command can affect to chat working 
## Users
All user's setters have next syntax:
```
user@chat> :set user [user-id] [attribute] [attribute value]
```
### User-id
At 1st you shold to choose user for user[id] parameter. For this you need user-ID. You can view this via ```:lsuser``` command. With this argument we declire target user
### Role
If you want to change user's role you should to use 'role' attribute. To add new role you should to know role-id. You can see that via ```:lsrole``` in 'ID' col. 
Use role-id as attribute value. Nothing more
### Name
You also can change user's name via 'name' attribute. It's contain string value, so use attribute value for choosing name
### Remove access
If you want to disallow to write messages for person (or remove some access flag) you can use ban attribute. Use access flags as attribute value

## Roles
All kinds of this commands have similiar syntax with ```:set user```
```
user@chat> :set role [role-id] [attribute] [attribute value]
```
Right now only 'access' and 'name' attributies allowed
### Access
Access - is powerful tool for manage some roles, files, messages and others. Access - is bit mask (32 bits) for all levels of permission. 0 bit - is reading permission, 1st bit - writing
 2nd - users management and etc. You can make template of access masks for every role. For example: "admin" role with full access(11111111) or "reader", who can only read messages(00000001)
 
Use ```access``` as attribute and use bit's number as attribute value. For example
```
user@chat> :Set role 1 access 0
user@chat> :Set role 1 access 1
user@chat> :Set role 1 access 2
user@chat> :Set role 1 access 3
```
this commands give reading, writing, users-managemant and role-managment permission for 1 role

Full table of access bits values you can see in ```access``` paragraph

## Me
You can change user permanently, withoit login if chat-ruler allow this. you shold to have 5-lvl (```acc_evlog```) access to do that

P.S. You also can change user #0 name ("system" by default)

## Chat
You can change current chat. For this you should to change `currentChat` enviroment variable. Use it:
```
user@chat> :set chat id
```
You can find ID via `:lschat` command

# Languages (in development)
You can manage languages, localization, string containers and others. So, you also can customize your layout
## Basics
All structure is based on dynamic-translation and smart-translation (in ``bion1c-4 reliase``). With dynamic translation all translation structure looks like one-side list with next structure:
#### struct LangContainer
| Type | Name | Description |
| ------ | --------- | ----------------- |
|```char *``` | fieldname | Name of target phrase. It can using in CLI/GUI/scripts. Ordinary it's have "lang.section.phrase". aka ```langblock.id``` |
| ```char *``` | translation | "lang.section.phrase" translation. aka ```langblock.desc``` |
| ```struct LangContainer*``` | next | Pointer to the next translation block. |
But don't worry: every low-level stuff is function's work

## commands
### rm ```fieldname```
Deleting ```fieldname``` translation block.
### add ```fieldname``` ```translation```
Add new translation block with ```fieldname``` fieldname (id) and ```translation``` translation (````desc````) ot editing already exsisting ```fieldname```
### edit```fieldname``` ```translation```
Same like ```add```, but don't allow making new translation blocks
### list
Show all translation blocks

Example:
```
#add new translation block
user@chat> :lang add "Hello" "Привет!"
#list translations blocks
user@chat> :lang list
    |__________________________ |
    |Fieldname | translation   | 
    |-----------------------------------  |
    |"Hello"       | "Привет!"     |
    |----------Total: 1---------------  |
#edit translation block
user@chat> :lang edit "Hello" "Cześć!"
#list translations blocks
user@chat> :lang list
    |__________________________ |
    |Fieldname | translation   | 
    |-----------------------------------  |
    |"Hello"       | "Cześć!"         |
    |----------Total: 1---------------  |
#removing translation block
user@chat> :lang rm hello
#list translations blocks
user@chat> :lang list
    __________________________
        No translations yet
    ------------------------------------
user@chat>
```
# DLoad
Useful utility for loading new components of OSMIC. You can download only languages packages and protocols yet, but soon `src`, `scr` and `utils` will be avialable
## Tags
It's software tags. For example, `proto` - is all protocols group. It's uses for navigate promgramm. All tags:

|Name|Description|
|---|----|
|`proto`|Dynamic linkable protocols|
|`src`|Source code|
|`lang`|Languages packages|
|`scr`|Eviroment scripts|
|`utils`|Additional software|
# Conn
It's simple: It's just connect you to server. If server require password or username you should to enter it. Example:
```
user@chat> :conn 127.0.0.1 8000
Accepted (Usernames-Chat with password)
Username: user
Password:
Connected
User ID: 4
Loading messages...
Loading users...
Loading roles...
alex@chat: Welcome to out chat!
    <1 sec. left>
user@chat> 
```
# Lschat
Show all chats in this session. It' looks like 
```
(ID)    Name    Users   Message
(.) 0   local   0       0
(*) 1   net     0       0
(.) 2   another 0       0
```
In active chat you can see start around ID.

Brackets have only 2 colors variants: green and yellow. If bracket is green it's mean your this chat is connected to server. If it's yellow it's mean chat is local-only
# Developer
Almost of this command needs only for debugging or testing, but users also can use it
## Recompile
This command useful only if you have sources of OSMIC ION realese and 'make' with 'gcc'. Else it's will ends sad :c
```
user@chat> :recompile (-all) (-updater) (-norun) (-ma) (-optimal) (-debug) (-spb)
```
### -all
Use it if you want to make AND updater AND OSMIC
### -updater
Build only updater
### -norun
Do not launch OSMIC after build
### -ma (MAin only)
Buld ONLY OSMIC
### -optimal
Build with optimization
### -debug
Build with debug info
### -spn
Shared Protocols Building
## Conf
This is default OSMIC settings. You can change it and view. Every commands have next syntax:
```
# Set config field value
user@chat> :conf set [conf-name] [0/1]
# View all config
user@chat> :conf view
```
Table of all configs names
| Default value | Name | Description | 
| - | ---- | ------------------------|
| 1 | crypto | Use cryptografi funcs for encrypt chat while dropping |
| 0 | doc | Drop Only Chat |
| 1 | native-acc | Enable native access after-compile features |
| 0 | runtime-n | Enable runtime network sync |
| 0 | native-aud | Enable native audion support |

Now config table have a special colors:
- cyan - forced enabled. This option is enabled on code level. You can't toggle it
- red - forced disabled. This option is enabled on code level. You can't toggle it
- green - enabled
- yellow - enabled, but conflicting and don't work
# Access
Access system - is system of checking and merging of bits masks, so it's fast and secure. Bit's position in the byte calls 'access flag'. Every action in chat requere some kind of access. You can make your own access if you want, but OSMIC also have reserved access flags. Here table of this
| Name | Access flag | HEX view | Full bit | Description |
| ---- | ----------- | -------- | -------- | ----------- |
| acc_read  | 0 | 00000000 | 00000000000000000000000000000000 | Read messages on chat |
| acc_write | 1 | 00000001 | 00000000000000000000000000000001 | Send messages to chat |
| acc_users | 2 | 00000002 | 00000000000000000000000000000010 | Manage users (only invited/created by from access owner) |
| acc_roler | 3 | 00000008 | 00000000000000000000000000000100 | Manage roles (everyone's roles) |
| acc_taggi | 4 | 0000000f | 00000000000000000000000000001000 | Manage users (everyone) |
| acc_evlog | 5 | 0000001f | 00000000000000000000000000010000 | Login as everyone |
## Warning!
 - User 'system' haven't any role and access, but at the start it's have FULL access. If you will try to load chat from file/stream systme's access will dropped to **0**! Do not forget make new role ("admin", for example) for system as super-user. If you will don't do that you can block all chat for session.
 - "0" access byte is "ban" state. It's mean, if somebody randomly gets ban, he will never come back without admin's help. You just should to change access flags
 - "ban" state always save user in userlist
