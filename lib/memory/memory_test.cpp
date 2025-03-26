#include <stdio.h>  // Inclut la bibliothèque standard pour les fonctions d'entrée/sortie, comme printf.
#include <memory.h>
int comparaison(int destinataire, int id, int tableau, int moi, int* taille) {  // Déclare la fonction 'comparaison' qui prend l'ID du destinataire, l'ID du paquet, un tableau d'IDs, l'ID de l'expéditeur et la taille du tableau.
    int trouve = 0;  // Déclare et initialise la variable 'trouve' à 0, qui indiquera si l'ID a été trouvé dans le tableau.

    if (destinataire != moi) {  // Vérifie si le destinataire n'est pas moi (l'expéditeur).
       for (int element_du_tableau = 0; element_du_tableau < *taille; element_du_tableau++) {  // Parcourt le tableau d'IDs jusqu'à la taille actuelle du tableau.
            if (tableau[element_du_tableau] == id) {  // Si l'élément actuel du tableau correspond à l'ID cherché.
                trouve = 1;  // Met la variable 'trouve' à 1 si l'ID a été trouvé.
                break;  // Sort de la boucle immédiatement une fois l'ID trouvé.
            }
        }
    }
    return trouve;  // Retourne 1 si l'ID a été trouvé (trouve = 1), sinon retourne 0 (trouve = 0).
}