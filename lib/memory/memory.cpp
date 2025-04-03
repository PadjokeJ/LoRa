#include "memory.h"
#include <stdint.h>
#include <stdio.h>

int comparaison(uint16_t id, uint16_t* knownlist,int taille) {  // Déclare la fonction 'comparaison' qui prend l'ID du destinataire, l'ID du paquet, un tableau d'IDs, l'ID de l'expéditeur et la taille du tableau.
    int known = 0;  // Déclare et initialise la variable 'known' à 0, qui indiquera si l'ID a été trouvé dans le tableau.
    uint16_t *tmp;
    int verif;
    for (int i = 0; knownlist[i] !=0; i++) {  // Parcourt le tableau d'IDs jusqu'à la taille actuelle du tableau.
        if (knownlist[i] == id) {  // Si l'élément actuel du tableau correspond à l'ID cherché.
            known = 1;  // Met la variable 'known' à 1 si l'ID a été trouvé.
            break;  // Sort de la boucle immédiatement une fois l'ID trouvé.
        }
        tmp=&knownlist[i];
        verif = i;
    }
    tmp++;
    if (known==0){
        if (verif>=taille){//empeche le debordement
            for (int i =0;i<(taille);i++){
                knownlist[i]=0;//Réinitialise la knownlist
            knownlist[0]=id;//Ajoute l'id à la knownlist
            }
        printf("Liste reset\n"); //Superflu
        }
    else{
        *tmp=id;//Ajoute l id a la knownlist
        printf("added\n");//Superflu
    }    
    }
    return known;  // Retourne 1 si l'ID a été trouvé (known = 1), sinon retourne 0 (known = 0).
}