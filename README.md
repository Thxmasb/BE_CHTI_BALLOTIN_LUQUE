# BE_CHTI de Terani Luque et Thomas Ballotin

<h1>Objectif 1 - Programmer la DFT en simulation sur un tableau de données imposés : <br></h1>
<br>
Le programme main appelle une fonction écrite en ASM qui renvoie le module mis au carré de la DFT de rang k qui est stocké dans tabk[k]. Le paramètre k ( de 0 à 63) est fourni à la fonction ainsi que l'adresse du tableau de données imposé.<br>
<br>
<h2>Pour vérifié les résulats dans le débugger:<br></h2>
1/ Si ce n'est pas le cas ajouter le tableau tab à la fenêtre Watch 1 en n'oubliant de supprimer "[k]". <br>
2/ Dans l'onglet TabSig.s, les valeurs attendues sont affichées en commentaire. Ce sont les "jeux de test" officiels.<br> 
3/ Faire un run aprés avoir placé le point d'arrêt au debut du while() pour permettre au for() de finir. <br>
4/ Observer dans la fenêtre Watch 1 tab[1] qui est bien égal à 0x3FFFCDE5. <br>
5/ Pour toutes les autres valeurs de k, tab[k] respecte bien les valeurs attendues : tab[k] < 0x0000000F;<br>
<br>
<h1>Objectif 2 -  Faire tourner la DFT « en réel » et gérer le score des 6 joueurs:<br></h1>
<br>
Notre objectif est ici est d'écrire un programme en C qui, périodiquement fait l'acquisition du signal, calcule la DFT et affecte le score à chacun des 6 joueurs.<br>
<br>
Comme nous ne pouvons pas généré les tirs nous même une DMA à été crée grâce à la librairie GFSSP72 et fournit un paquet de 64 echantillons en RAM chaque fois qu'on le lance. Les échantillons sont donc stockés dans le tableau dma_buff.<br>
<br>
Grâce à la documentation de la librairie, nous pouvons voir que des tirs sont générés dans dma_buff pour les valeurs suivantes : 17,18,19,20,23,24. Nous avons donc calculer la DFT à ces points.<br>
<br>
Ensuite, il fallait incrèmenter le compteur cmp lorsque la DFT des tirs générés dépassent M2TIR. M2TIR correspond à une valeur seuil permettant de prendre en compte un tir ou non. Pour la calculer on savais qu'on ne pouvais pas dépassé un signal C-C de 100mV à l'entrée de l'ADC. Nous avions donc une amplitude de 50mv.<br>
<br>
Donc pour 50mV= 50*4095/3300 = 62mV signal en sortie.<br>
<br>
L'amplitude de la transformée de fourier est (A*M/2) avec M le nombre d'échantillons.<br>
<br>
Ce qui donne une raie d'amplitude= A/2*M = (31*64)^2=3936256.<br>
<br>
Qu'il faut divisé par 4 pour avoir M2TIR car on a fait le choix de conserver les 32 bits de poids fort.<br>
<br>
De plus, il fallais ensuite chercher un seuil pour lequel nous contions les points de chaque joueur. Nous avons débouchés aprés quelques test fait avec le débugger sur la valeur 19 (0x13).<br>
<br>
Quant à la variable SYSTICKPER correspond à 1 période pour le timer système. La période de traitement est de 5ms. On sait que 72 ticks c'est 1us. Donc 5ms = 72 * 5000 = 36 * 10000 = 360 000.<br>
<br>
<h2>Vérification:<br></h2>
1/ On choisit la durée d'un ech_tick dans la fonction Init_TimingADC_ActiveADC_ff() selon ce que l'ont veut tester:
<ul>
  <li>0x33 : sig.1 = 248, bruit = 4 (cas facile)</li>
  <li>0x52 : sig.1 = 124, bruit = 4, sig.2 = 1000</li>
  <li>0x3E : sig.1 = 124, bruit = 100</li>
  <li>0x3C : sig.1 =  31, bruit = 100 (scores nuls)</li>
 </ul>
2/ Lancer le debugger.<br>
3/ On ajoute les variables cmpt[0],[1],[2],[5] dans le logic analyzer.<br>
4/ On ajoute score à la Watch 1 également.<br>
5/ Si l'on à choisi: 
<ul>
  <li>0x33 : nous allons voir seulement les scores des 5 joueurs étant sur le signal 1 (k=17,18,19,20,23) et allons visualiser les tirs des joueurs 0,1 et 2 sur le logic analyser; </li>
  <li>0x52 : nous allons voir le score des 6 joueurs;</li>
  <li>0x3E : Score des 5 joueurs du signal 1;</li>
  <li>0x3C : Les scores sont nuls car trop bruités.</li>
 </ul><br>
 
<h1>Objectif 3 - Gérer le son (étape indépendante):<br></h1>
Notre objectif est de déclencher depuis le main, l'émission d'un son (bruit de verre cassé) dans le logic analyzer. <br>
<br>
Tout d'abord on calcule la période en ticks de notre son. Sachant que 72 ticks équivalent à une micro seconde, on obtient la période en ticks directement avec le calcul : 72*PeriodeSonMicroSec. <br>
<br>
Dans le sujet, il est précisé que la fréquence du PWM doit être largement supérieure à 20kHz, nous avons donc choisi une fréquence de 200kHz. En période système, celà correspond à 360 ticks. <br>

<h2>Vérification:<br></h2>
<h3>A) Pour observer le son :</h3> <br> <br>
1/ Enlever les points d'arrêts s'il y en a <br> 
2/ Lancer le debugger <br>
3/ Faire un clic droit que TIM3_CCR3 et l'ajouter dans le Logic Analizer avec "Add to..." .<br>
4/ Faire un run et arrêter seulement quand le logic analizer affiche une droite.<br>
5/ Verifier que le logic analizer est en mode analog avec un clic droit, ensuite faire un "Zoom All" et un "Min/Max Auto". <br><br>

<h3>B) Pour verifier qu'il y a bien 91 micro secondes entre deux échantillons successifs</h3><br>
1/ Ajouter un point d'arrêt dans le fichier GestionSon.s au niveau du push{r4} (ligne 32). <br>
2/ Lancer le debugger
3/ Faire un run jusqu'au point d'arrêt et remmetre à zéro le timer 1. Pour le faire, il suffit d'effectuer un clic droit sur t1 qui se trouve en bas à droite et de cliquer sur "Reset Stop Watch(t1)". <br>
4/ Faire un deuxième run et observer que la valeur de t1 est bien 91 micro secondes. <br>


<h1>Objectif 4 - Projet final :<br></h1>


