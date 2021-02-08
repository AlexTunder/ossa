package network

type UserList struct {
	name   string
	access int
	next   *UserList
}
type Message struct {
	context string
	time    uint64
	userid  uint
}
type MessageList struct {
	me   Message
	next *MessageList
}
type UserLinks struct {
	id   uint
	next *UserLinks
}
type Role struct {
	usersList UserLinks
	name      string
	access    uint
}
type Roler struct {
	role Role
	next *Roler
}
