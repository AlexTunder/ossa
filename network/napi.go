package main

//#include "../src/types.h"
import "C"

var e C.int = 0

/* Connection API */
//export setServer
func setServer(host *C.char, port C.int) C.int {
	e++
	return e
}

//export authServer
func authServer(username *C.char, password *C.char) C.int {

	return 0
}

//export useServer
func useServer(serverID C.int) C.int {

	return 0
}

//export closeServer
func closeServer(serverID C.int) C.int {

	return 0
}

//export useraddServer
func useraddServer(name *C.char, pwd *C.char) C.int {

	return 0
}

/* Send API */
/* Messages */
//export sendMessage
func sendMessage(mes C.struct_Message) C.int {

	return 0
}

//export syncMessages
func syncMessages(ml C.struct_MessageList) C.int {

	return 0
}

//export delMessage
func delMessage(mes C.struct_Message) C.int {

	return 0
}

/* Rolers */
//export syncRoles
func syncRoles(roler C.struct_Roler) C.int {

	return 0
}

//export syncUsers
func syncUsers(userList C.struct_UserList) C.int {

	return 0
}

//export pullRoles
func pullRoles(roler C.struct_Roler) C.int {

	return 0
}

//export pullUsers
func pullUsers(userList C.struct_UserList) C.int {

	return 0
}

func main() {
	for {

	}
}