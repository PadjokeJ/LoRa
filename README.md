# LoRa

Projet de fin d'année, à deux  

## Plan  

Ordre des choses

### Phase 1 - Organisation  

- [x] [groupes](#groupes)
- [x] [tâches](#tâches)
- [x] planification
- [x] cahier des charges

### Phase 2 - Réalisation 1  

- [x] code
- [x] documentation
- [ ] tester

### Phase 3 - Réalisation 2  

- [ ] mettre ensemble
- [ ] tester
- [ ] corriger

### Phase 4 - Déploiement

- [ ] Validation

## Groupes  

| N | MEMBRES | [TACHES](#tâches) |
| :----: | :----: | :----: |
| 1 | SB & SG | add_recieve recieve |
| 2 | JG & RG | send |
| 3 | NK & TY | process recieve |
| 4 | GK & AS | serial output |
| 5 | AD & JP | encode decode |
| 6 | MG & ME | documentation |
| 7 | AB & MV | if_know add_know_list |

## Tâches  

- [ ] main
- [x] serial input
- [x] encode
- [x] decode
- [x] add sent queue
- [x] send
- [x] recieve
- [x] add recieve queue
- [x] process recieve
- [x] recieve
- [x] if know list
- [x] add know list
- [x] serial output

## Packet  

| Type | PACKET_ID | SRC | DEST | SIZE | PAYLOAD |
| :----: | :----: | :----: | :----: | :----: | :----: |
| 1 | 2 | 1 | 1 | 1 | size - 6 |

il n'y a pas de type de variable en c sur 4 bits, du coup le type sera sur 4 bits  
la taille est représentée en octets
