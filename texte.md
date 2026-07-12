<
- Introduction : présentation du problème (la recherche dans un arbre trié)
     - Introduction du problème :

Le premier programme que tous les develloppeur apprenent hello world est d'apprendre a chercher un element dans une liste. 
L'approche la plus simple est de parcourir la liste complete et trouver si une valeur correspond a la valeur rechercher. 
C'est une fonction extrement utile dans de nombreux contexte nottament verifier qu'une valeur n'est pas deja presente pour verifier les valeurs dupliquer ou bien pour la recherche de correspondance avec un hash.

    

         // ici je vais simplement présenter le problème : rechercher un élément dans une liste déjà connue. Quelle organisation des valeurs peut-on adopter ?
     
L'algorithmique nous permet d'analyser le comportement d' algorithme dans le cas ou la taille des donnees (souvent nommee N) est tres grand. 
Cela nous permet donc de determiner le meilleur algorthme pour des cas extreme comme rechercher une valeurs une valeur parmis 1 000 000 000 de valeurs. 
La recherche linaire possede une [complexite linaire](https://fr.wikipedia.org/wiki/Comparaison_asymptotique) c'est a dire que cherche dans une liste 2 fois plus grandes prend 2 fois plus de temps. 
C'est pourquoi une autre approche a etait invente: la recherche dicotomique.


-- mettre ici le premier bench lineaire

    - Premier exemple : recherche binaire dans une liste triée :
          // Ensuite, première implémentation avec la liste triée ordonnée par ordre croissant + binary search

Pour applique la recherche dichotomique il est necessaire de posseder une donnees ayant un [ordre total](https://fr.wikipedia.org/wiki/Ordre_total) afin d'ordonnes les valeurs de la plus petite a la plus grande. 
Ensuite, on prend une valeur au milieu de notre set de donnees: si la valeurs qu'on cherche est plus petite on cherche dans la motie inferieur. 
Si elle est plus grande on cherche dans la moitie supperieurs. 
Ainsi en une comparaison la moitie des donnees sont ecarte du champs de recherche. On dit que l'algorithme est de complexite logarithmique.

-- deuxieme bench avec binary search

     - Introduction de la forme d'Eytzinger
         // Dans cette section, je vais comparer la performance de la recherche binaire définie plus haut avec un binary search baser sur le la forme d'Eytzinger. (J'espère avoir des résultats concluants. Dans l'article, il obtient une x6 en performance)

Sur petite valeurs ( < TODO), les performance ne sont pas trop differentes, cependant, sur les liste de tres grande taille, le gain de temps des gigantesques.
Mais maintenant que l'on a un algorithme efficace, est-il le meilleur ?

-- 3 eme bench mark avec la comparaison du binary search et du eytzinger basique. 

On observe ici que les performance des query dite du Layout Eytzinger sont 1.5x plus rappide que la recherche binaire.
Ce nouvelle algorithme est une variante de la recherche binaire / dichotomique mais sa difference reside dans sont odonnacement des donnees: le layout Eytzinger.
Ce layout est un encodage dans une liste d'un arbre binaire: un noeud est une case d'un tableau, dont l'index est i (avec des index commencant a 1), alors le fils gauche se trouve en i * 2 et le fils droit dans la case i * 2 + 1.
D'un point de vu code, hormis le calcules des indexs, le principe reste le meme, on choisi une valeurs, on la compare avec celle que l'on cherche et on reduis le champs des possibles. Pour le bianry search, on divise physiquement la liste en 2 tandis qu'avec le layout Eytzinger on parcours un arbre binaire de recherche equilibre.



La recherche dichotomique est semblable a la recheche dans un arbre binaire de recherche, on parcours l'arbre en prenant la valeur mediane et en descendant ainsi dans les feuilles jusqu'a trouver la valeurs qui nous interresse. 
Le layout Eytzinger, developper par Michaël Eytzinger, est un layout tres rependu dans les data structures tel que les heaps pour les arbre n'utilisant pas de pointeurs.
     
-- TODO refaire une la descriptions du pointeur

Voici donc la difference du code de la boucle

 => le but de cette partie est de définir le problème et de démontrer que la notation Big O ne suffit pas pour avoir les meilleurs résultats.




- Introduction aux caches matériels.
     //partie asser théorique sur leur existence, leurs nombres et une présentation breve de leur capacité

On il regulierement indique que les performances d'un cpu sont limite de 2 facon: CPU bound ou IO bound. C'est a dire qu'il est soit limite par la puissance de calcule du CPU en nombre d'operation par seconde. Soit le CPU est obligé d'attendre des evenement exterieur tel que l'ecriture de donnees ou bien la lecture. C'est ce deuxieme cas qui nous interresse. En effet le CPU sur lequel tourne le benchmark est capable de realiser 2 milliard d'operation par seconde, tandis que comme nous l'avon vu, nous ne realisons qu'une trentaine de tour de boucle (Log2(2³²) = 32). En prennant 2 milliard d'operation par seconde et 30*20 operations par calcule: 0,5 ns par operation soit 300 nano secondes soit 3 fois moins de temps que le temps mesuré. Les causes sont donc lié à la mémoire. En utilisant l'outil perf

=> cette partie est simplement là afin de définir le contexte et pour donner une explication de la différence de vitesse entre les 2.

- Amélioration et nouvelle structure.
     // dans cette partie, je vais présenter un benchmark sur différentes organisation de données afin de montrer en quoi ils sont plus efficients en cache et en les gains qu'ils apportent respectivement.
    - Eytzinger
    - Arbre S+ => aucun resultat concluant, c'est tres triste

=>  Cette partie est là comme exemple et permet d'introduire de nouvelle organisations de données plus pertinentes face à la nouvelle capacité hardware

- conclusion
    // conclusion sur les résultats obtenue et les choses apprises. 
    // une ouverture sur les peu être faite : soit sur l'autre optimisation telle que les SIMD, le branchless. Soit une ouverture technique sur une application (dans redis par exemple) mais je n'en ai pas encore.
>