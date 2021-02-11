package main

//#include "../src/types.h"
import "C"
import (
	"bufio"
	"crypto/md5"
	"fmt"
	"net"
	"strconv"
	"strings"
)

func getMacAddr() ([]string, error) {
	ifas, err := net.Interfaces()
	if err != nil {
		return nil, err
	}
	var as []string
	for _, ifa := range ifas {
		a := ifa.HardwareAddr.String()
		if a != "" {
			as = append(as, a)
		}
	}
	return as, nil
}

type requestStage struct {
	req   string
	ans   string
	ready bool

	serverER error
	serverFD net.Conn
	serverID C.int
}

var mainStream requestStage

var e C.int = 0

/* Connection API */
//export setServer
func setServer(host *C.char, port C.int) C.int {
	var fullhost string = C.GoString(host)
	fullhost += ":" + strconv.Itoa(int(port))

	mainStream.serverFD, mainStream.serverER = net.Dial("tcp", fullhost)

	MAC, err := getMacAddr()
	if err != nil {
		return -2
	}

	if mainStream.serverER != nil {
		return -1
	}

	fmt.Fprintf(mainStream.serverFD, "OSSA-PTC: 000f0010\nHardware: %v\r", MAC)

	message, _ := bufio.NewReader(mainStream.serverFD).ReadString('\n')
	lines := strings.Split(message, "\n")
	words := strings.Split(lines[0], " ")
	if words[0] != "OSSA-PTC:" {
		fmt.Print(message)
		return -3
	}

	return 0
}

//export authServer
func authServer(username *C.char, password *C.char) C.int {
	hashPWD := md5.Sum([]byte(C.GoString(password)))
	// fmt.Printf("Using %v user with %x hash of password\n", C.GoString(username), hashPWD)s
	fmt.Fprintf(mainStream.serverFD, "OSSA-PTC: 000f001c\nUsername: %v\nPwd-hash: %x\r", C.GoString(username), hashPWD)
	message, _ := bufio.NewReader(mainStream.serverFD).ReadString('\n')
	lines := strings.Split(message, "\n")
	words := strings.Split(lines[0], " ")
	if words[0] != "OSSA-PTC:" {
		// fmt.Print(message)
		return -3
	}
	if words[1] == "000f0a1c" {
		words = strings.Split(lines[1], " ")
		userid, _ := strconv.Atoi(words[1])
		return C.int(userid)
	} else {
		return -4
	}
}

//export useServer
func useServer(serverID C.int) C.int {

	return 0
}

//export closeServer
func closeServer(serverID C.int) C.int {
	mainStream.serverFD.Close()
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
