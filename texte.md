# Cache memoire, presentation et impacte

# Introduction

Le premier programme que tous les developpeur apprenent hello world est d'apprendre a chercher un element dans une liste. 
L'approche la plus simple est de parcourir la liste complete et trouver si une valeur correspond a la valeur rechercher. 
C'est une fonction extrement utile dans de nombreux contexte nottament verifier qu'une valeur n'est pas deja presente pour verifier les valeurs dupliquer ou bien pour la recherche de correspondance avec un hash.

## La recherche lineaire

L'algorithmique nous permet d'analyser le comportement d' algorithme dans le cas ou la taille des donnees (souvent nommee N) est tres grand. 
Cela nous permet donc de determiner le meilleur algorthme pour des cas extreme comme rechercher une valeurs une valeur parmis 1 000 000 000 de valeurs. 
La recherche linaire possede une [complexite linaire](https://fr.wikipedia.org/wiki/Comparaison_asymptotique) c'est a dire que cherche dans une liste 2 fois plus grandes prend 2 fois plus de temps. 
C'est pourquoi une autre approche a etait invente: la recherche dicotomique.


-- mettre ici le premier bench lineaire

## Recherche Dichotomique

Pour applique la recherche dichotomique il est necessaire de posseder une donnees ayant un [ordre total](https://fr.wikipedia.org/wiki/Ordre_total) afin d'ordonnes les valeurs de la plus petite a la plus grande. 
Ensuite, on prend une valeur au milieu de notre set de donnees: si la valeurs qu'on cherche est plus petite on cherche dans la motie inferieur. 
Si elle est plus grande on cherche dans la moitie supperieurs. 
Ainsi en une comparaison la moitie des donnees sont ecarte du champs de recherche. On dit que l'algorithme est de complexite logarithmique.

-- deuxieme bench avec binary search

Sur petite valeurs ( < TODO), les performance ne sont pas trop differentes, cependant, sur les liste de tres grande taille, le gain de temps des gigantesques.
Mais maintenant que l'on a un algorithme efficace, est-il le meilleur ?


## Layout d'Eytzinger

-- 3 eme bench mark avec la comparaison du binary search et du eytzinger basique. 

On observe ici que les performance des query dite du Layout Eytzinger sont 1.5x plus rappide que la recherche binaire.
Ce nouvelle algorithme est une variante de la recherche binaire / dichotomique mais sa difference reside dans sont odonnacement des donnees: le layout Eytzinger.
Ce layout est un encodage dans une liste d'un arbre binaire: un noeud est une case d'un tableau, dont l'index est i (avec des index commencant a 1), alors le fils gauche se trouve en i * 2 et le fils droit dans la case i * 2 + 1.
D'un point de vu code, hormis le calcules des indexs, le principe reste le meme, on choisi une valeurs, on la compare avec celle que l'on cherche et on reduis le champs des possibles. 
Pour le bianry search, on divise physiquement la liste en 2 tandis qu'avec le layout Eytzinger on parcours un arbre binaire de recherche equilibre.
     
-- TODO refaire une la descriptions du pointeur

Voici donc la difference du code de la boucle:

-- ajouter le code des 2 cote a cote

## D'ou viens le probleme ? (TODO changer ce titre de merde)

On dit regulierement que les performances d'un cpu sont limite de 2 facon: CPU bound ou IO bound. 
C'est a dire qu'il est soit limite par la puissance de calcule du CPU en nombre d'operation par seconde. 
Soit le CPU est obligé d'attendre des evenement exterieur tel que l'ecriture de donnees ou bien la lecture. 
C'est ce deuxieme cas qui nous interresse. 
En effet le CPU sur lequel tourne le benchmark est capable de realiser 2 milliard d'operation par seconde, tandis que comme nous l'avons vu, nous ne realisons qu'une trentaine de tour de boucle (Log2(2³²) = 32). 
En prennant 2 milliard d'operation par seconde et 30*20 operations par query or on a 0,5 ns par operation soit 300 nano secondes et donc 3 fois moins de temps que le temps mesuré. 
On peut donc en deduire que le que notre query est limite par les IO, la RAM dans notre cas.


# Les caches mémoires

## Presentation

Les caches memoires sont à la RAM ce que la RAM est au SSD. 
Ce sont des memoire volatile bien plus rapide que la RAM permettant d'accelerer l'acces aux information de la RAM en gardant en memoire les dernieres donnees acceder. 
En contre partie ces mémoires sont bien plus petite que leurs predecesseurs et egalement bien plus cher.

Par exeample mon ordinateur possedes les caches suivant.

| Nom | Taille | Latence |
|-----|--------|---------|
| L1d/ L1i | 32KiB | 3 nanosecond |
| L2 | 512KiB | 6 nanosecond |
| L3 | 16Mib | 35 nanosecond |
| RAM | 16Gio | 120 nanosecond |

-- TODO ajouter dans l'intro le liens vers le github / vers la section annexes sur le bench mark

On remarque que le cache le plus petit, le L1, est egalement le cache le plus rapide. 2 fois plus rapide que le cache L2 et ainsi de suite, on a donc une piramide de cache allant du plus rapide (mais egalement du plus petit) au plus lent (le plus grand). 

-- ajouter l'image de la des caches en piramide

On voit donc que chaque accees en RAM possede un cout elevé: 120 nano secondes soit 240 instructions CPU pour chaque recherche dans la memoire RAM.

## Fonctionnement des caches

Sous linux vous pouvez utiliser la commande `lscpu -C` afin de voir les differents caches que possede votre CPU. Vous pouvez également utiliser le fichier de test de la mémoire `make cache_latency && ./latency.exe`. 

```shell
$lscpu -C
NAME ONE-SIZE ALL-SIZE WAYS TYPE        LEVEL  SETS PHY-LINE COHERENCY-SIZE
L1d       32K     256K    8 Data            1    64        1             64
L1i       32K     256K    8 Instruction     1    64        1             64
L2       512K       4M    8 Unified         2  1024        1             64
L3        16M      16M   16 Unified         3 16384        1             64
```

Ce tableau nous indique les caracteristiques physique de chacun des caches.
Les caches sont divisé en ligne de cache: indique ici par la collone **COHERENCY-SIZE**. On peut voir que les lignes de cache L1 sont de 64 octets.
La colonne **SETS** nous indique combien de ligne de cache contient la memoire. 64 dans le cas du caches L1
Enfin la derniere colonnes nous interressant est la colonne **WAYS** indiquant combien de niveau chaque lignes possedes.

Pour completer le process un cache il faut ajouter 2 fonctionnalité:
- le choix du set
- la methode d'elimination.

On peut faire une analogie des cache avec une table de chevet: se lever pour aller chercher un livre dans sa biblioteque est un processus lent. Pour gagner du temps, les livres lu le plus recement sont poser sur la table de chevet. Cela permet un acces rapide mais peu de livre peuvent rester sur la table de chevet. Etant une personne organise vous poser les roman au premier etage, les bande dessine au deuxieme, les comics au troiseme et les livre technique au 4 eme. Les étages representent les **SETS** d'un cache. A chaque etage est capable de tenir un nombre precis de livre (le meme pour tous les etages), ce sont les **WAYS** du cache. Maintenant, si vous souhaitez prendre une livre de la biblioteque, et l'ajouter de la chevet, il faut choisir le bon etage selon le livre pris. C'est **le choix du set**. Cependant, si il n'y a plus de place, alors il faut choisir que livre remplacer. Pour cela il y a plusieurs approche (la **methode d'elimination**):
- **FIFO**: Retirer le livre présent depuis le plus longtemps
- **LRU (least recently used)**: Retirer le livre qu'on n'a pas rouvert depuis le plus longtemps


## L'impacte sur la recherche

On comprend donc que reutiliser les caches, permet de gagner du temps. Pour en profiter au maximum, il est souvent dit de respecter 2 principes: localité temporelle et la localité spacial. La localité temporel indique qu'une donnees qui a etait lu va probablement etre relu dans peu de temps. Pour la localité spacial, il est probable qu'apres a voir lu une valeur, la valeur d'a cote dans la memoire soit lu egalement. 
En reprennant l'analogie, un livre qui a recement eté ajouté a la biblioteque a de forte chance d'etre relu dans les jours qui suivent. Si on s'est arrete au chapitree 3 d'un livre, il est probable que le chapitre 4 sois lu dans les jours qui suivent.

-- mais alors quel impacte pour nous,

Avec ces information en tete, on comprend que la recherche dicotomique n'est pas tres pertinante: on prend une valeur au milie, puis une autre a un autre endroit et ainsi de suite. On ne profite ni de la localité spaciale, ni de la localité temporelle.

A l'inverse, dans le layout Eytzinger, les premieres valeurs utiliser pour la recherche se situent toutes dans les premiers octets de la liste. 

Comme on l'a vu pour ma machine, il y a 64 octets par lignes, et on utilise des i32 (soit 4 octets) comme donnees. Cela veut dire que dans chaque ligne de cache, il y a 16 valeurs.

Pour les premieres iterations de la boucle de recherche on prend donc les index:
- tour 1: 0
- tour 2: 1 à 2
- tour 3: 3 à 6
- tour 4: 7 à 14
- tour 5: 15 à 30

En retracant les index, on voit que les 4 premiers tours de bouclent utilise la meme ligne de cache. 

De plus une autre mechanique des CPU moderne entre en jeu: l'hardware prefetcher. Il en existe differents et sont plus ou moins efficace. Ces prefetcheur hardware reconnaissent des parternes dans les instruction tel que les acces sequentiel afin de prefetch par avance la ligne de cache suivante au lieux d'attendre que les instructions demander la donnees. Des prefetcher plus avancer sont capable de reconnaitre des suite plus complexe. En reprenant les 2 implementations, dans le cas de la recherche dicotomique, l'index suivant est calcule a partir de 2 variable et un calcule couteux difficile a prefetch. A l'inverse pour le layout Eytzinger, l'index est simplement un i*2 ce qui est anticipable par un prefetcher avance.

Afin de mesurer les gains d'algorithme face au cache, le nombre *cache misse* est souvent utiliser. Un cache miss indique que le CPU a essayer d'acceder a une addresse mais que celle ci n'etait pas deja dans le cache. Pour mesurer les caches misses on peut utiliser l'outil `perf`. En effet grace a sa commande stat il est possible d'avoir une approximation du nombre de cache miss d'un programme.

binary_search:
```
       288,458,716      cycles                      ( +-  0.04% )
        17,575,597      cache-misses                ( +-  0.10% )
```

Eytzinger layout
```
       169,767,001      cycles                      ( +-  0.11% )
         6,265,564      cache-misses                ( +-  0.08% )
```

On voit donc que la forme d'Eytzinger reduit de presque 65% le nombre de cache misses, menant une reduction de 40% du nombre de cycle CPU soit le gain de performance observer entre le binary search et le Eytzinger.

# Conclusion

- conclusion
    // conclusion sur les résultats obtenue et les choses apprises. 
    // une ouverture sur les peu être faite : soit sur l'autre optimisation telle que les SIMD, le branchless. Soit une ouverture technique sur une application (dans redis par exemple) mais je n'en ai pas encore.

Bien que ce soit benchmark sur un cas tres particulier, on observe un un gain notable de peformance lorsque le code est optimise pour les hardware recents. Les cachent permet d'accelerer l'acces a des donnees permettant de reduire le temps d'attente IO pour le CPU. 
Les CPU possedent de nombreuse optimisation hardware leur permettant de prefetch les instructions, faire des disjonctions de cas lors des branches, des instructions permettant de faire des calcules sur plusieurs element en 1 cycle CPU. Comprendre le fonctionnement des CPU permet de realiser des optimisation afin d'atteindre des niveau de performance inimaginable. Cet [article](https://curiouscoding.nl/posts/static-search-tree/) poussent la recherche de l'element dans une liste a ses limites en employant toutes ces optimisations.

# Annexes

## Detail sur le bench marque

- on recherche la presence ou non d'un element dans une liste de taille N
- ce sont des int de taille 4 octets
- liens du github avec le code
-  Details sur le hardware du benchmark

## Le cache warmup

## Les Stree+
