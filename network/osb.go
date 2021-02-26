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
}

var Current int = 0
var mainStream []requestStage

var e C.int = 0

/* Connection API */
//export setServer
func setServer(host *C.char, port C.int) C.int {
	var fullhost string = C.GoString(host)
	fullhost += ":" + strconv.Itoa(int(port))

	var serverData requestStage
	serverData.serverFD, serverData.serverER = net.Dial("tcp", fullhost)

	MAC, err := getMacAddr()
	if err != nil {
		return -2
	}

	if serverData.serverER != nil {
		fmt.Printf("(*) libossac.a: failed to connect: %v\n", serverData.serverER)
		return -1
	}

	fmt.Fprintf(serverData.serverFD, "OSSA-PTC: 000f0010\nHardware: %v\r", MAC)

	message, _ := bufio.NewReader(serverData.serverFD).ReadString('\000')
	lines := strings.Split(message, "\n")
	words := strings.Split(lines[0], " ")
	mainStream = append(mainStream, serverData)
	if words[0] != "OSSA-PTC:" {
		fmt.Printf("uP: %v\n", message)
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
	fmt.Printf("Message: %v\n", lines)
	return 0x5d
}

//export authServer
func authServer(username *C.char, password *C.char) C.int {
	// fmt.Printf("[debug]Password: %v\n", C.GoString(password))
	hashPWD := md5.Sum([]byte(C.GoString(password)))
	// fmt.Printf("Using %v user with %x hash of password\n", C.GoString(username), hashPWD)s
	fmt.Fprintf(mainStream[Current].serverFD, "OSSA-PTC: 000f001c\nUsername: %v\nPwd-hash: %x\r", C.GoString(username), hashPWD)
	message, _ := bufio.NewReader(mainStream[Current].serverFD).ReadString('\000')
	lines := strings.Split(message, "\n")
	words := strings.Split(lines[0], " ")
	
	if words[0] != "OSSA-PTC:" {
		// OSSA_INVALID_PROTO
		
		// fmt.Printf("uP: %v\n", message)
		return -3
	}
	// fmt.Printf("getted lines: %v\n",lines)
	if words[1] == "000f0a1c" {
		words = strings.Split(lines[1], " ")
		userid, _ := strconv.Atoi(words[1])
		return C.int(userid)
	} else if words[1] == "000f0a0c" {
		return -4
	}
	//Auth failed
	return -4
}

//export useServer
func useServer(serverID C.int) C.int {
	// Don't touch it yet!
	if serverID < 0 {
		return C.int(len(mainStream))
	}
	Current = int(serverID)
	return C.int(len(mainStream))
}

//export closeServer
func closeServer(serverID C.int) C.int {
	if (serverID >= C.int(len(mainStream))){
		return 0
	}
	mainStream[serverID].serverFD.Close()
	return 0
}

//export useraddServer
func useraddServer(name *C.char, pwd *C.char) C.int {
	fmt.Fprintf(mainStream[Current].serverFD, "OSSA-PTC: 5c0f001c\nUsername: %v\nPwd-clear: %v\r", C.GoString(name), C.GoString(pwd))
	message, err := bufio.NewReader(mainStream[Current].serverFD).ReadString('\000')
	// fmt.Println(message)
	if err != nil {
		return -1
	}
	lines := strings.Split(message, "\n")
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
	jsonSend = fmt.Sprintf("{\"body\":\"%v\",\"date\":\"%v\",\"sender\":\"%v\"}", C.GoString(mes.content), mes.date, mes.userid)
	fmt.Fprintf(mainStream[Current].serverFD, "OSSA-PTC: 4c0f001c\nlen:%v\n%v\n", len(jsonSend), jsonSend)
	message, err := bufio.NewReader(mainStream[Current].serverFD).ReadString('\000')
	if err != nil {
		return -1
	}
	lines := strings.Split(message, "\n")
	words := strings.Split(lines[0], " ")
	if words[0] != "OSSA-PTC:" {
		return -3
	}
	// fmt.Printf("'%v'\000'%v'\000'%v'\n", words, lines, words[1])
	if words[1] == "0000f10a" {
		for i := 1; i < len(lines)-1; i++ {
			words = strings.Split(lines[i], " ")
			if words[0] == "ERROR:" {
				ret, _ := strconv.Atoi(words[1])
				// fmt.Printf("error: %v\n", ret)
				return -1 * C.int(ret)
			}
		}
		return -1000
	} else if words[1] == "0000f11a" {
		for i := 1; i < len(lines)-1; i++ {
			words = strings.Split(lines[i], " ")
			if words[0] == "Total: " {
				ret, _ := strconv.Atoi(words[1])
				// fmt.Printf("total: %v\n", ret)
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
	fmt.Fprintf(mainStream[Current].serverFD, "OSSA-PTC: 000bf11a\nFrom: %v\n", C.getLastFromML(ml).date)
	message, err := bufio.NewReader(mainStream[Current].serverFD).ReadString('\000')
	message = strings.Trim(message, "\n")

	if err != nil {
		return -1
	}

	var newMes int

	lines := strings.Split(message, "\n")
	words := strings.Split(lines[0], " ")

	if words[1] == "000bf110" {
		return 0
	} else if words[1] == "000bf11f" {
		for i := 1; i < len(lines); i++ {
			words = strings.Split(lines[i], " ")
			if words[0] == "Count:" {
				newMes, _ = strconv.Atoi(words[1])
			}
			for j := 0; j < newMes; j++{

				//All JSON contained on the next line
				//Content: {"body":"hello","date":"1774212414","sender":"1"}
				i++

				var itn int = 0

				//fmt.Printf("Conent detected! %v - is json\n", lines[i])

				rw001 := strings.Trim(lines[i], "{")
				rw001 = strings.Trim(rw001, "}")

				jsonParsed := strings.Split(rw001, ",")
				//fmt.Printf("Parsed data: %v, len: %v\n", jsonParsed, len(jsonParsed))
				var gettedMes C.struct_Message = C.makeMes(C.CString(""), 0)
				for j := 0; j < len(jsonParsed)-12; j++ {
					jsonField := strings.Split(jsonParsed[j], ":")
					jsonField[0] = strings.Trim(jsonField[0], "\"")
					jsonField[1] = strings.Trim(jsonField[1], "\"")

					//fmt.Printf("(%v)Parsed data: %v\n", jsonParsed, jsonField)

					//fmt.Printf("(warning) \"%v\" - is unknowen JSON field with \"%v\" data\n", jsonField[0], jsonField[1])

					if jsonField[0] == "body" {
						//fmt.Printf("\"%v\"\n", jsonField[1])
						gettedMes.content = C.CString(jsonField[1])
					} else if jsonField[0] == "date" {
						itn, _ = strconv.Atoi(jsonField[1])
						gettedMes.date = C.time_t(itn)
					} else if jsonField[0] == "sender" {
						itn, _ := strconv.Atoi(jsonField[1])
						// fmt.Printf("%v", reee)
						// fmt.Printf("PARSING USERID (%v)(%v)!\n", jsonField[1], itn)
						gettedMes.userid = C.int(itn)
					}
				}
				C.pushMessageToML(gettedMes, ml)
			}
		}
		// count := strconv.Atpi(words[])
		return C.int(newMes)
	}
	return -3
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
func syncUsers(userList *C.struct_UserList) C.int {

	fmt.Fprintf(mainStream[Current].serverFD, "OSSA-PTC: 000ba110\n")
	message, err := bufio.NewReader(mainStream[Current].serverFD).ReadString('\000')
	message = strings.Trim(message, "\n")

	if err != nil {
		return -1
	}

	var newMes int

	lines := strings.Split(message, "\n")
	words := strings.Split(lines[0], " ")

	if words[1] == "002ba110" {
		for i := 1; i < len(lines); i++ {
			words = strings.Split(lines[i], " ")
			if words[0] == "Count:" {
				newMes, _ = strconv.Atoi(words[1])
			}
			for j := 0; j < newMes; j++ {
				i++
				rw001 := strings.Trim(lines[i], "{")
				rw001 = strings.Trim(rw001, "}")

				jsonParsed := strings.Split(rw001, ",")
				// fmt.Printf("Parsed data: %v, len: %v\n", jsonParsed, len(jsonParsed))
				var uname string
				for j := 0; j < len(jsonParsed); j++ {
					jsonField := strings.Split(jsonParsed[j], ":")
					jsonField[0] = strings.Trim(jsonField[0], "\"")
					jsonField[1] = strings.Trim(jsonField[1], "\"")

					if jsonField[0] == "username" {
						uname = jsonField[1]
					} else if jsonField[0] == "MAC" {
						if len(uname) >= len(jsonField[1]) {
							uname = jsonField[1] + "\n" + uname
						} else {
							uname = jsonField[1] + "\t\t" + uname
						}
					}
				}
				C.pushUserToUL(C.CString(uname), userList)
			}
		}
		// count := strconv.Atpi(words[])
		return C.int(newMes)
	}
	return -3
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
