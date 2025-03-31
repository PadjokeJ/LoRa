# Ping
This is a small programm to check LoRa connections.
It sends a ping, and when it receives one sendout a reply with pong.

This programm is made for dragino shield. Compile the ping.ino in the arduino IDE or equivalent.

Befor flashing to arduino you should change the myId address. Some other var are configurable.

An adresse querying, and packet id "pairing" is work in progress

## Protocol
### Addresses
Addresses are made of two bytes (arbitryry choice). An adress of 0.0 means broadcast. 
### Message types
- Ping : send out a query for ping, either to 0.0 (broadcast) of a specific addresse.
- Pong : this is the anwer to a ping
- RAR : Research for address resolution. Sends a query for having a readable name for two byte address
- RARA : Answer to a RAR query.
### Message format
All multi-bytes data are from highes to lowest byte (Big Endian form)
1. First byte is for message type
2. Two bytes for the sender address
3. Two bytes for the destination address
4. Two bytes for packeid (this is a random number)
5. Four bytes for the time of the message (time message was issued relaive to sender, except for pong where time is the original ping time).
6. One Byte for the payload length (0 for no payload)
7. Rest is payload 


