#include "memory.h"
#include <stdio.h>

int main()
{   
    int destinataire = 1;
    uint16_t id = 600;
    int moi = 2;
    int taille = 2;//taille de la liste known
    uint16_t knownlist[2] = {0}; //création de la known liste
    knownlist[0] = 700;//Superflu
    knownlist[1] = 710;//Superflu
    if (destinataire == moi){
        printf("Message pour vous\n"); //Superflu
        //Ajoutez fonction pour récup message
    }
    else if (comparaison(id, knownlist, taille) == 0){ //Si l'ID est nouveau
        printf("nice ca mon reuf\n");
        //ajoutez le send queue
    }
    else{
        printf("Ce message est déjà passé\n");//Superflu
    }
        
        
    
    
    
    for(int i = 0; i < taille; i++){//Superflu
        printf("%d\n",knownlist[i]);//Superflu
    }
    return 0;
}