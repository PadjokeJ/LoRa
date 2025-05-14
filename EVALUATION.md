# Evaluation  

Here are the contributor's code repartition. Some of it may have been slightly modified by someone else, or copy pasted into a relevant file.  
A tick inside of a box means the code was used as is in the loop. When there is no tick, that means the code was adapted, copy pasted or reformulated into another file. They are documented by being in italics.

# Repartition

## Mikael, Max - Docs

c.f. relevant documentation wiki

## Samuel, Sacha - Receive, Lorainit

- [ ] [Lorainit](/main/lorainit.cpp)
- [ ] [Lorainit header](/main/lorainit.h)
- [ ] [Receive](/main/receive.cpp)
- [ ] [Receive header](/main/receive.h)
- [ ] [Receive test](/main/test/receive_test.cpp)
- [x] [*main setup*](https://github.com/PadjokeJ/LoRa/blob/7861ee6142816efc9611615ba2689ff3fff9efca/main/main.ino#L108-L136)
- [x] [*main receive*](https://github.com/PadjokeJ/LoRa/blob/7861ee6142816efc9611615ba2689ff3fff9efca/main/main.ino#L53-L58)

For unknown technical reasons, using Lorainit and Receive through separate files and headers does not work, copying and pasting them as is in the main file, works.

## Romain, Jules - Send

- [ ] [Send](/main/send.cpp)
- [ ] [Send header](/main/send.h)
- [x] [*main send*](https://github.com/PadjokeJ/LoRa/blob/7861ee6142816efc9611615ba2689ff3fff9efca/main/main.ino#L178-L188)

Same issue as Receive and lorainit.

## Timur, Nawfel - Process

- [ ] [Process](/main/process.cpp)

Was asked to make a process response, ended up coding queue system that would crash the program if present in main folder, even when not included through a header. This is why it is commented out.

## Arthur, Jonatan - Encode, Decode, Packets

- [x] [Encode](/main/encode.cpp)
- [x] [Encode header](/main/encode.h)
- [ ] [Encode decode test](/main/test/encode_test.cpp)
- [x] [Decode](/main/decode.cpp)
- [x] [Decode header](/main/decode.h)
- [x] [Packets](/main/packet.cpp)
- [x] [Packets header](/main/packet.h)

## Anshul, Guillaume - Serial

- [ ] [Serial](/main/serial.cpp)
- [ ] [Serial header](/main/serial.h)
- [x] [*main input*](https://github.com/PadjokeJ/LoRa/blob/7861ee6142816efc9611615ba2689ff3fff9efca/main/main.ino#L87-L106)
- [x] [*main output*](https://github.com/PadjokeJ/LoRa/blob/7861ee6142816efc9611615ba2689ff3fff9efca/main/main.ino#L64-L69)

## Agnes, Maurits - Message memory

- [x] [Memory](/main/memory.cpp)
- [x] [Memory header](/main/memory.h)
- [x] [Memory test](/main/test/memory_test.cpp)