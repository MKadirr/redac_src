# redac_src

Benchmarks comparant différentes structures de données / algorithmes de recherche
(`find`) sur des tableaux d'entiers triés, en mesurant le temps de requête à
différentes tailles de tableau (de la taille L1 jusqu'à plusieurs centaines de Mo,
pour observer l'effet des caches CPU).

Chaque algorithme implémente la même interface (voir `find.h`), ce qui permet de
les brancher un par un dans le même harnais de mesure (`main_warmup.c` /
`main_warm_bench.c`).

## Algorithmes disponibles

| Nom (cible `make`)   | Fichier               | Description                              |
|-----------------------|-----------------------|-------------------------------------------|
| `linear`              | `linear.c`             | Recherche linéaire                        |
| `binary`              | `binary.c`             | Recherche binaire classique                |
| `eytzinger`           | `eytzinger.c`          | Layout Eytzinger (arbre binaire implicite) |
| `eytzinger_prefetch`  | `eytzinger_prefetch.c` | Eytzinger + prefetching manuel             |
| `pbtree`              | `pbtree.c`             | Arbre B "paginé"                           |
| `stree`               | `stree.c`              | S-tree (arbre à branchement large, SIMD)   |
| `stree_plus`          | `stree_plus.c`         | Variante optimisée du S-tree                |

Chaque fichier implémente les fonctions déclarées dans `find.h` :

```c
void  pres_find();                    // affiche le nom de l'algo (pour les logs)
void* init_find(int* arr, size_t len); // construit la structure à partir d'un tableau trié
int   find(void* data, int value);     // cherche value dans la structure
void  free_find(void* data);           // libère la structure
```

## Prérequis

- `gcc` (ou `cc`) avec support `-mbmi` (instructions BMI, utilisées par certains algos)
- `make`
- Linux avec :
  - `taskset` (pinning sur un cœur, package `util-linux`)
  - `numactl` (contrôle NUMA, package `numactl`)
  - `perf` (uniquement pour les benchmarks avec compteurs matériels, package `linux-tools-common` / `linux-tools-$(uname -r)`)

```bash
sudo apt install build-essential numactl linux-tools-common linux-tools-generic
```

## Compilation

Le `Makefile` compile un binaire unique `bench.exe` en liant le harnais principal
(`main_warmup.c` par défaut) avec l'algorithme choisi.

```bash
make <algo>       # ex: make binary, make eytzinger, make stree...
./bench.exe        # lance le benchmark, affiche une ligne de résultats
```

`make all` compile successivement `linear`, `binary`, `pbtree` et `stree`.

`make clean` supprime les `.o` et l'exécutable.

### Format de sortie

Le binaire affiche une seule ligne CSV : pour chaque taille de tableau testée
(définies dans `arr_sizes[]` dans `main_warmup.c`), le temps moyen (en ns) pour
`NB_QUERY` recherches sur cette taille.

```bash
./bench.exe 1   # affiche uniquement l'en-tête CSV (les tailles testées), sans lancer le bench
./bench.exe      # lance le bench et affiche les temps mesurés
```

## Lancer un benchmark simple

`bench.sh` compile puis lance un algo (ici configuré pour `linear`, à adapter en
éditant la variable `benchs` dans le script) :

```bash
./bench.sh
```

Ou manuellement, pour n'importe quel algo, avec pinning CPU + NUMA (recommandé pour
des mesures stables) :

```bash
make clean <algo>
taskset -c 3 numactl --membind=0 ./bench.exe
```

## Lancer un benchmark "warm" (répété)

`bench_warm.sh` recompile avec le harnais `main_warm_bench.c` (cible `b_warm_<algo>`
du Makefile) et relance le binaire 500 fois de suite, pour accumuler des mesures
stables sur un algo donné :

```bash
./bench_warm.sh <algo>
# ex:
./bench_warm.sh eytzinger
./bench_warm.sh stree_plus
```

Sans argument, le script affiche l'usage attendu et s'arrête.

## Désactiver le warmup

Par défaut, chaque mesure est précédée de `NB_QUERY_WARMUP` requêtes "à blanc"
pour chauffer les caches avant la mesure réelle. Pour désactiver ce warmup et
mesurer les performances "à froid" :

```bash
make nowarm_<algo>
./bench.exe
```

Ceci recompile avec `-DNO_WARMUP=1`.

## Benchmarks avec compteurs matériels (`perf`)

`perf.sh` compile les cibles `perf_binary` et `perf_eytzinger` (macro
`-DPERF_ENABLE=1`), qui pilotent `perf stat` via des FIFOs pour n'activer les
compteurs matériels que pendant la phase de mesure (pas pendant le warmup) :

```bash
./perf.sh
```

Cela produit `perf_binary.data` et `perf_eytzinger.data`, contenant les compteurs
`cycles`, `cache-misses`, `L1-dcache-load-misses`, `dTLB-load-misses` et
`instructions`, moyennés sur 20 répétitions (`perf stat -r 20`).

Pour lancer `perf` sur un autre algo, adapter le script ou reproduire la commande :

```bash
make clean perf_<algo>
taskset -c 2,3 numactl --membind=0 perf stat -r 20 --delay=-1 \
  -e cycles,cache-misses,L1-dcache-load-misses,dTLB-load-misses,instructions \
  -o perf_<algo>.data \
  --control=fifo:/tmp/perf_ctl.fifo,/tmp/perf_ack.fifo \
  -- ./bench.exe
```

> Note : `perf_<algo>` dépend de la création préalable des FIFOs
> `/tmp/perf_ctl.fifo` et `/tmp/perf_ack.fifo` — le Makefile s'en charge
> automatiquement via les cibles associées.

## Mesures de latence cache

`cache_latency.c` est un binaire indépendant mesurant la latence d'accès mémoire
à différentes tailles (pour situer les seuils L1/L2/L3 utilisés comme repères
dans `arr_sizes[]`) :

```bash
make cache_latency
./latency.exe
```

## Résumé des commandes utiles

```bash
make <algo>                  # compile un algo donné
make nowarm_<algo>            # compile sans warmup
make b_warm_<algo>            # compile avec le harnais "warm" (500 runs)
make perf_<algo>              # compile avec les hooks perf
make clean                    # nettoie les binaires

./bench.sh                    # lance un run simple (algo configuré dans le script)
./bench_warm.sh <algo>        # lance 500 runs successifs pour un algo
./perf.sh                     # lance perf stat sur binary et eytzinger
```

## Structure du projet

```
.
├── Makefile              # règles de compilation pour chaque algo et mode
├── find.h                # interface commune à tous les algos de recherche
├── binary.c               # recherche binaire
├── linear.c               # recherche linéaire
├── eytzinger.c            # layout Eytzinger
├── eytzinger_prefetch.c   # Eytzinger + prefetch
├── pbtree.c               # arbre B
├── stree.c / stree_plus.c # S-tree et variante optimisée
├── cache_latency.c        # mesure indépendante de latence mémoire
├── main_warmup.c          # harnais principal (mesure ponctuelle avec warmup)
├── main_warm_bench.c      # harnais pour les runs répétés (bench_warm.sh)
├── bench.sh               # script de lancement simple
├── bench_warm.sh          # script de lancement répété (500x)
└── perf.sh                # script de lancement avec perf stat
```
