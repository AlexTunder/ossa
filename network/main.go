package main

//#include "../src/types.h"
import "C"

/* Connection API */
//export setServer
func setServer(host string, port int) int {

	return 0
}

//export authServer
func authServer(username string, password string) int {

	return 0
}

//export useServer
func useServer(serverID int) int {

	return 0
}

//export closeServer
func closeServer(serverID int) int {

	return 0
}

//export useraddServer
func useraddServer(name string, pwd string) int {

	return 0
}

/* Send API */
/* Messages */
//export sendMessage
func sendMessage(mes C.struct_Message) int {

	return 0
}

//export syncMessages
func syncMessages(ml C.struct_MessageList) int {

	return 0
}

//export delMessage
func delMessage(mes C.struct_Message) int {

	return 0
}

/* Rolers */
//export syncRoles
func syncRoles(roler C.struct_Roler) int {

	return 0
}

//export syncUsers
func syncUsers(userList C.struct_UserList) int {

	return 0
}

//export pullRoles
func pullRoles(roler C.struct_Roler) int {

	return 0
}

//export pullUsers
func pullUsers(userList C.struct_UserList) int {

	return 0
}

func main() {

}
