# intro
OSMIC try use vim-like command line, so some features will be similiar

For all chat-inside commands use ':' prefix. For example: ':exit'

If you want to use some shell commands from chat use '$' prefix. for example: '$ip a'

# Basic commands
| Name | Syntax | Description |
| ---- | ------ | ----------- |
| :exit | :exit | Exit from chat |
| :drop | :drop (-chat [filename]) (-users [filename]) (-roles [filename]) | write chat data to 3 files. You can manage filenames with opcional arguments. By default using 'users.list', 'roles.list' and [time of 1st message]-[time of last message] files |
| :useradd | :useradd [username] | Add new user to user list |
| :roleadd | :roleadd [role name] | Add new role to roler |
| :view | :view [all/tail [count of messages]] | view all chat history or only tail of N messages |
| :inrole | :inrole [userid] | Check user with USERID for role |
| :ban | :ban [userid] | Ban user with USERID |
| :long | :long [stop-char] | enable multi-line typing while you not press [stop-char]. Use ~as stop-char, cuz it's best one |
| :load | :load (-chat [filename]) (-users [filename]) (-roles [filename]) | read chat data from 3 files. You can manage filenames with opcional arguments. By default using 'users.list', 'roles.list' and [time of 1st message]-[time of last message] files |
| :lsuser | :lsuser | View full list of with their's IDs |
| :lsrole | :lsrole (role ID) | view full info about role(s). If you type just ':lsrole' it's will displayed full list of roles woth full info |
| :set | view next pont | allow to manage roles, users, access and others. View more in next point |

# Set command
For every of this command you should to use next structure: :set [parameter]. Every command can affect to chat working |
## Users
All user's setters have next syntax:
```
user@chat> :set user [user-id] [attribute] [attribute value]
```
### User-id
At 1st you shold to choose user for user[id] parameter. For this you need user-ID. You can view this via ':lsuser' command. With this argument we declire target user
### Role
If you want to change user's role you should to use 'role' attribute. To add new role you should to know role-id. You can see that via ':lsrole' in 'ID' col. 
Use role-id as attribute value. Nothing more
### Name
You also can change user's name via 'name' attribute. It's contain string value, so use attribute value for choosing name
### Remove access
If you want to disallow to write messages for person (or remove some access flag) you can use ban attribute. Use access flags as attribute value

## Roles
All kinds of this commands have similiar syntax with ':set user' 
```
user@chat> :set role [role-id] [attribute] [attribute value]
```
Right now only 'access' and 'name' attributies allowed
### Access
Access - is powerful tool for manage some roles, files, messages and others. Access - is bit mask (32 bits) for all levels of permission. 0 bit - is reading permission, 1st bit - writing
 2nd - users management and etc. You can make template of access masks for every role. For example: "admin" role with full access(11111111) or "reader", who can only read messages(00000001)
 
Use "access" as attribute and use bit's number as attribute value. For example
```
user@chat> :Set role 1 access 0
user@chat> :Set role 1 access 1
user@chat> :Set role 1 access 2
user@chat> :Set role 1 access 3
```
this commands give reading, writing, users-managemant and role-managment permission for 1 role

Full table of access bits values you can see in 'access' paragraph

## Me
You can change user permanently, withoit login if chat-ruler allow this. you shold to have 5-lvl (acc_evlog) access to do that

# Developer commands
Almost of this command needs only for debugging or testing, but users also can use it
## Recompile
This command useful only if you have sources of OSMIC ION realese and 'make' with 'gcc'. Else it's will ends sad :c
```
user@chat> :recompile (-all) (-updater) (-norun) (-ma) (-optimal) (-debug)
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
Beuld with debug info
##Conf
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

# Thx for attension!
