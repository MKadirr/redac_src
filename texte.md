
- Introduction : présentation du problème (la recherche dans un arbre trié)
     - Introduction du problème :

Le premier programme que tous les develloppeur apprenent hello world est d'apprendre a chercher un element dans une liste. 
L'approche la plus simple est de parcourir la liste complete et trouver si une valeur correspond a la valeur rechercher. 
C'est une fonction extrement utile dans de nombreux contexte nottament verifier qu'une valeur n'est pas deja presente pour verifier les valeurs dupliquer ou bien pour la recherche de correspondance avec un hash.

    

         // ici je vais simplement présenter le problème : rechercher un élément dans une liste déjà connue. Quelle organisation des valeurs peut-on adopter ?
     
L'algorithmique nous permet d'analyser le comportement d' algorithme dans le cas ou la taille des donnees (souvent nommee N) est tres grand. 
Cela nous permet donc de determiner le meilleur algorthme pour des cas extreme comme rechercher une valeurs une valeur parmis 1 000 000 000 de valeurs. 
La recherche linaire possede une [complexite linaire](https://fr.wikipedia.org/wiki/Comparaison_asymptotique) c'est a dire que cherche dans une liste 2 fois plus grandes prend 2 fois plus de temps. 
C'est pourquoi une autre approche a etait defini: la recherche dicotomique.


-- mettre ici le premier bench lineaire

    - Premier exemple : recherche binaire dans une liste triée :
          // Ensuite, première implémentation avec la liste triée ordonnée par ordre croissant + binary search

Pour applique la recherche dichotomique il est necessaire de posseder une donnees ayant un [ordre total](https://fr.wikipedia.org/wiki/Ordre_total). 
Afin d'ordonnes les valeurs de la plus grande a la plus petite. 
Ensuite, on prend une valeur au milieu de notre set de donnees: si la valeurs qu'on cherche est plus petite on cherche dans la motie inferieur. 
Si elle est plus grande on cherche dans la moitie supperieurs. 
Ainsi en une comparaison la moitie des donnees sont ecarte du champs de recherche. 
On dit que l'algorithme est de complexite logarithmique.

    
-- deuxieme bench avec binary search

     - Introduction de la forme d'Eytzinger
         // Dans cette section, je vais comparer la performance de la recherche binaire définie plus haut avec un binary search baser sur le la forme d'Eytzinger. (J'espère avoir des résultats concluants. Dans l'article, il obtient une x6 en performance)
     
 => le but de cette partie est de définir le problème et de démontrer que la notation Big O ne suffit pas pour avoir les meilleurs résultats.


- Introduction aux caches matériels.
     //partie asser théorique sur leur existence, leurs nombres et une présentation breve de leur capacité

=> cette partie est simplement là afin de définir le contexte et pour donner une explication de la différence de vitesse entre les 2.

- Amélioration et nouvelle structure.
     // dans cette partie, je vais présenter un benchmark sur différentes organisation de données afin de montrer en quoi ils sont plus efficients en cache et en les gains qu'ils apportent respectivement.
    - Eytzinger
    - Arbre S+

=>  Cette partie est là comme exemple et permet d'introduire de nouvelle organisations de données plus pertinentes face à la nouvelle capacité hardware

- conclusion
    // conclusion sur les résultats obtenue et les choses apprises. 
    // une ouverture sur les peu être faite : soit sur l'autre optimisation telle que les SIMD, le branchless. Soit une ouverture technique sur une application (dans redis par exemple) mais je n'en ai pas encore.
