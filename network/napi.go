package main

//#cgo LDFLAGS: -L../lib -lossa -Wl,-rpath=../lib
//#include "../core/types.h"
//#include "../core/base.h"
import "C"
import (
	"bufio"
	"crypto/md5"
	"fmt"
	"net"
	"strconv"
	"strings"
	// "unsafe"
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
	if words[1] == "000faa1c" {
		//Free auth based on MAC-addr
		return 0x2e
	} else if words[1] == "000faf1c" {
		//Free auth based on IP-address
		return 0x64
	} else if words[1] == "000f0010" {
		//Username and password protection
		return 0x7b
	} else if words[1] == "000f0a10" {
		//Password protection. Using MAC-adderss as username
		return 0x6c
	}

	// Unknowen auth method
	return 0x5d
}

//export authServer
func authServer(username *C.char, password *C.char) C.int {
	hashPWD := md5.Sum([]byte(C.GoString(password)))
	// fmt.Printf("Using %v user with %x hash of password\n", C.GoString(username), hashPWD)s
	fmt.Fprintf(mainStream.serverFD, "OSSA-PTC: 000f001c\nUsername: %v\nPwd-hash: %x\r", C.GoString(username), hashPWD)
	message, _ := bufio.NewReader(mainStream.serverFD).ReadString('\n')
	lines := strings.Split(message, "\r")
	words := strings.Split(lines[0], " ")
	if words[0] != "OSSA-PTC:" {
		// OSSA_INVALID_PROTO
		return -3
	}
	// fmt.Printf("getted lines: %v\n",lines)
	if words[1] == "000f0a1c" {
		words = strings.Split(lines[1], " ")
		userid, _ := strconv.Atoi(words[1])
		return C.int(userid)
	}
	//Auth failed
	return -4
}

//export useServer
func useServer(serverID C.int) C.int {
	// Don't touch it yet!
	return 0
}

//export closeServer
func closeServer(serverID C.int) C.int {
	mainStream.serverFD.Close()
	return 0
}

//export useraddServer
func useraddServer(name *C.char, pwd *C.char) C.int {
	fmt.Fprintf(mainStream.serverFD, "OSSA-PTC: 5c0f001c\nUsername: %v\nPwd-clear: %v\r", C.GoString(name), C.GoString(pwd))
	message, err := bufio.NewReader(mainStream.serverFD).ReadString('\n')
	fmt.Println(message)
	if err != nil {
		return -1
	}
	lines := strings.Split(message, "\r")
	words := strings.Split(lines[0], " ")
	if words[0] != "OSSA-PTC:" {
		return -3
	}

	var uid, gerr int = 0, 0

	for i := 1; i < len(lines)-1; i++ {
		words = strings.Split(lines[i], " ")
		if words[0] == "UID: " {
			uid, _ = strconv.Atoi(words[1])
		} else if words[0] == "ERROR:" {
			gerr, _ = strconv.Atoi(words[1])
		}
	}
	if gerr != 0 {
		return -1 * C.int(gerr)
	}
	return C.int(uid)
}

/* Send API */
/* Messages */
//export sendMessage
func sendMessage(mes C.struct_Message) C.int {
	jsonSend := "a"
	jsonSend = fmt.Sprintf("{\"body\":\"%v\",\"date\":\"%v\",\"sender\":\"%v\"}", mes.content, mes.date, mes.userid)
	fmt.Fprintf(mainStream.serverFD, "OSSA-PTC: 4c0f001c\nlen:%v\n%v\n", len(jsonSend), jsonSend)
	message, err := bufio.NewReader(mainStream.serverFD).ReadString('\n')
	if err != nil {
		return -1
	}
	lines := strings.Split(message, "\r")
	words := strings.Split(lines[0], " ")
	if words[0] != "OSSA-PTC:" {
		return -3
	}
	fmt.Printf("'%v'\n'%v'\n'%v'\n", words, lines, words[1])
	if words[1] == "0000f10a" {
		for i := 1; i < len(lines)-1; i++ {
			words = strings.Split(lines[i], " ")
			if words[0] == "ERROR:" {
				ret, _ := strconv.Atoi(words[1])
				fmt.Printf("error: %v\n", ret)
				return -1 * C.int(ret)
			}
		}
		return -1000
	} else if words[1] == "0000f11a" {
		for i := 1; i < len(lines)-1; i++ {
			words = strings.Split(lines[i], " ")
			if words[0] == "Total: " {
				ret, _ := strconv.Atoi(words[1])
				fmt.Printf("total: %v\n", ret)
				return C.int(ret)
			}
		}
		return 0
	} else {
		return -3
	}
}

//export syncMessages
func syncMessages(ml *C.struct_MessageList) C.int {

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
