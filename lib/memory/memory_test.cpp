#include "memory.h"
#include <stdio.h>

int comparaison(int id, int* tableau, int taille) {  // Déclare la fonction 'comparaison' qui prend l'ID du destinataire, l'ID du paquet, un tableau d'IDs, l'ID de l'expéditeur et la taille du tableau.
    int known = 0;  // Déclare et initialise la variable 'known' à 0, qui indiquera si l'ID a été trouvé dans le tableau.

    for (int element_du_tableau = 0; element_du_tableau < taille; element_du_tableau++) {  // Parcourt le tableau d'IDs jusqu'à la taille actuelle du tableau.
        if (tableau[element_du_tableau] == id) {  // Si l'élément actuel du tableau correspond à l'ID cherché.
            known = 1;  // Met la variable 'tknown' à 1 si l'ID a été trouvé.
            break;  // Sort de la boucle immédiatement une fois l'ID trouvé.
        }
    }
    printf("%d\n", known); //Superflu
    return known;  // Retourne 1 si l'ID a été trouvé (known = 1), sinon retourne 0 (known = 0).
}

int main()
{   
    int destinataire =1;
    int id=600;
    int moi=2;
    int taille =0;
    int tableau[50]={0};
    tableau[taille]=700;
    taille++;
    if (destinataire==moi){
        printf("Message pour vous\n"); //Superflu
        //Ajoutez fonction pour récup message
    }
    else if (comparaison(id,tableau,taille)==0){ //Si l'ID est nouveau
        if (taille<10){
            tableau[taille]=id;
            taille++;
        }
        else{
            printf("tableau rempli\n");
            for (int i =0;i<50;i++){
                tableau[i]=0;//Réinitialise le tableau
            }
        //ajoutez fonction pour sendqueu    
        }
            
    }
    else{
        printf("Ce message est déjà passé\n");//Superflu
    }
        
        
    
    
    
    for(int i =0;i<10;i++){
        printf("%d\n",tableau[i]);
    }
    return 0;
}