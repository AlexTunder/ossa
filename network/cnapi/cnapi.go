package cnapi

//UserList a
//export UserList
type UserList struct {
	name   string
	access int
	next   *UserList
}

//Message a
//export Message
type Message struct {
	context string
	time    uint64
	userid  uint
}

//MessageList a
//export MessageList
type MessageList struct {
	me   Message
	next *MessageList
}

//UserLinks a
//export UserLinks
type UserLinks struct {
	id   uint
	next *UserLinks
}

//Role a
//export Role
type Role struct {
	usersList UserLinks
	name      string
	access    uint
}

//Roler a
//export Roler
type Roler struct {
	role Role
	next *Roler
}
