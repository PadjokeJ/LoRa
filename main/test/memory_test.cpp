#include "memory.h"
#include <stdio.h>
#include <stdint.h>


int comparaison(uint16_t id, uint16_t* knownlist, int taille) {  // Déclare la fonction 'comparaison' qui prend l'ID du paquet, un tableau d'IDs et la taille du tableau.
    int known = 0;  // Déclare et initialise la variable 'known' à 0, qui indiquera si l'ID a été trouvé dans le tableau.
    uint16_t *tmp; // Initialisation d'um pointeur tmp
    int verif; // Initialisation d'une variable verif 
    for (int i = 0; knownlist[i] != 0; i++) {  // Parcourt le tableau d'IDs jusqu'à ce qu'il n'y ait plus d'ID dans la liste
        if (knownlist[i] == id) {  // Si l'élément actuel du tableau correspond à l'ID cherché.
            known = 1;  // Met la variable 'known' à 1, l'ID a été trouvé.
            break;  // Sort de la boucle immédiatement une fois l'ID trouvé.
        }
        tmp = &knownlist[i]; // On met l'adresse du dernier ID comparé dans le pointeur tmp
        verif = i; // On sauvegarde la postion dans la liste dans verif
    }
    tmp++; //On incémente tmp pour pointer sur le prochain élément libre de ma liste
    if (known == 0){ // Si on ne connait pas l'ID
        if (verif >= taille) {// empeche le debordement
            for (int i = 0; i < taille; i++){ // Parcours toute la liste
                knownlist[i] = 0;//Réinitialise la knownlist
                knownlist[0] = id;//Ajoute l'id à la knownlist
            }
        }
        else {
            *tmp=id;//Ajoute l id a la knownlist
        }    
    }
    return known;  // Retourne 1 si l'ID a été trouvé (known = 1), sinon retourne 0 (known = 0).
}
int main()
{   uint16_t id = 60;
    int moi = 2;
    int taille = 5;//taille de la liste known
    uint16_t knownlist[5] = {0}; //création de la known liste
    knownlist[0] = 700;//Superflu
    knownlist[1] = 710;//Superflu
    int destinataire = 1;
    if (destinataire == moi){
        printf("Message pour vous\n"); //Superflu
        //Ajoutez fonction pour récup message
    }
    else if (comparaison(id, knownlist, taille) == 0){ //Si l'ID est nouveau
        printf("Nouvel ID Ajouté\n");//Superflu
    }
    else{
        printf("Ce message est déjà passé\n");//Superflu
    }
        
        
    
    
    
    for(int i = 0; i < taille; i++){//Superflu
        printf("%d\n",knownlist[i]);//Superflu
    }
    return 0;
}