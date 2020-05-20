# BE_CHTI de Terani Luque et Thomas Ballotin

Objectif 1 - Programmer la DFT en simulation sur un tableau de données imposés : <br>
<br>
Le programme main appelle une fonction écrite en ASM qui renvoie le module mis au carré de la DFT de rang k qui est stocké dans tabk[k]. Le paramètre k ( de 0 à 63) est fourni à la fonction ainsi que l'adresse du tableau de données imposé.<br>
<br>
Pour vérifié les résulats dans le débugger:<br>
1/ Si ce n'est pas le cas ajouter le tableau tab à la fenêtre Watch 1 en n'oubliant de supprimer "[k]". <br>
2/ Dans l'onglet TabSig.s, les valeurs attendues sont affichées en commentaire. Ce sont les "jeux de test" officiels.<br> 
3/ Faire un run aprés avoir placé le point d'arrêt au debut du while() pour permettre au for() de finir. <br>
4/ Observer dans la fenêtre Watch 1 tab[1] qui est bien égal à 0x3FFFCDE5. <br>
5/ Pour toutes les autres valeurs de k, tab[k] respecte bien les valeurs attendues : tab[k] < 0x0000000F;<br>
<br>
Objectif 2 -  Faire tourner la DFT « en réel » et gérer le score des 6 joueurs:<br>
<br>
Notre objectif est ici est d'écrire un programme en C qui, périodiquement fait l'acquisition du signal, calcule la DFT et affecte le score à chacun des 6 joueurs.

Comme nous ne pouvons pas généré les tirs nous même une DMA à été crée grâce à la librairie GFSSP72 et fournit un paquet de 64 echantillons en RAM chaque fois qu'on le lance. Les échantillons sont donc stockés dans le tableau dma_buff.

Grâce à la documentation de la librairie, nous pouvons voir que des tirs sont générés dans dma_buff pour les valeurs suivantes : 17,18,19,20,23,24. Nous avons donc calculer la DFT à ces points.

Ensuite, il fallait incrèmenter le compteur cmp lorsque la DFT des tirs générés dépassent M2TIR. M2TIR correspond à une valeur seuil permettant de prendre en compte un tir ou non. Pour la calculer on savais qu'on ne pouvais pas dépassé un signal C-C de 100mV à l'entrée de l'ADC. Nous avions donc une amplitude de 50mv.
Donc pour 50mV= 50*4095/3300 = 62mV signal en sortie.

(ADC 12 bits (plage min max analogique) / 3300mv (numérique)
plage d'entrée 0-3.3V
plage de sortie 0 à 2^12-1=4095 valeurs diff ox0fff)

Objectif 3 - Gérer le son (étape indépendante):<br>

Objectif 4 - Projet final :<br>


