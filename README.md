# BE_CHTI de Terani Luque et Thomas Ballotin

Objectif 1 - Programmer la DFT en simulation sur un tableau de données imposés : 

Le programme main appelle une fonction écrite en ASM qui renvoie le module mis au carré de la DFT de rang k qui est stocké dans tabk[k]. Le paramètre k ( de 0 à 63) est fourni à la fonction ainsi que l'adresse du tableau de données imposé.

Pour vérifié les résulats dans le débugger:
1/ Si ce n'est pas le cas ajouter le tableau tab à la fenêtre Watch 1 en n'oubliant de supprimer "[k]". <br>
2/ Dans l'onglet TabSig.s, les valeurs attendues sont affichées en commentaire. Ce sont les "jeux de test" officiels. 
3/ Faire un run aprés avoir placé le point d'arrêt au debut du while() pour permettre au for() de finir. 
4/ Observer dans la fenêtre Watch 1 tab[1] qui est bien égal à 0x3FFFCDE5. 
5/ Pour toutes les autres valeurs de k, tab[k] respecte bien les valeurs attendues : tab[k] < 0x0000000F;

Objectif 2 :

Objectif 3 :

Objectif 4 :


