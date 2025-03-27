#include "memory.h"
#include <stdio.h>


int comparaison(int id, int* knownlist,int taille) {  // Déclare la fonction 'comparaison' qui prend l'ID du destinataire, l'ID du paquet, un tableau d'IDs, l'ID de l'expéditeur et la taille du tableau.
    int known = 0;  // Déclare et initialise la variable 'known' à 0, qui indiquera si l'ID a été trouvé dans le tableau.
    int *tmp;
    for (int i = 0; knownlist[i] >0; i++) {  // Parcourt le tableau d'IDs jusqu'à la taille actuelle du tableau.
        if (knownlist[i] == id) {  // Si l'élément actuel du tableau correspond à l'ID cherché.
            known = 1;  // Met la variable 'known' à 1 si l'ID a été trouvé.
            break;  // Sort de la boucle immédiatement une fois l'ID trouvé.
        }
        tmp=&knownlist[i];
    }
    tmp++;
    if (known==0){
        if (*tmp==-1){//empeche le debordement
            for (int i =0;i<(taille-1);i++){
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

int main()
{   
    int destinataire =1;
    int id=600;
    int moi=2;
    int taille =4;//taille de la liste known
    int knownlist[4]={0}; //création de la known liste
    knownlist[0]=700;//Superflu
    knownlist[1]=710;//Superflu
    knownlist[taille-1]=-1;//Definit la fin de la limite
    if (destinataire==moi){
        printf("Message pour vous\n"); //Superflu
        //Ajoutez fonction pour récup message
    }
    else if (comparaison(id,knownlist,taille)==0){ //Si l'ID est nouveau
        printf("nice ca mon reuf\n");
        //ajoutez le send std::queue
    }
    else{
        printf("Ce message est déjà passé\n");//Superflu
    }
        
        
    
    
    
    for(int i =0;i<(taille);i++){//Superflu
        printf("%d\n",knownlist[i]);//Superflu
    }
    return 0;
}