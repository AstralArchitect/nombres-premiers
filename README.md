# Description
Ceci est un programme en C qui permet de rechercher des nombres premiers.
## Utilisation
Voici un aperçu de la commande :
```
builddir/main [End(int] [no_print(bool)]
```
Si les valeurs de 'End' et de 'no_print' ne sont pas précisées 'End' vous sera demandé et 'no_print' sera defini sur false.
## Compilation
Pour compiler le programme, vous pouvez utiliser `meson` ou `make`.
### Avec Make
Utilisez :
```
make setup
```
Pour configurer le projet. 
#### Note 
Si vous avez MSVC et GCC de installé et que vous voulez utiliser GCC, configurez le projet avec `make setupGcc`.
####
Ensuite utilisez :
```
make
```
Pour compiler ou :
```
make run
```
Pour compiler et executer sans argument.
## Benchmark
Si vous voulez tester les performances de l'algorithme, utilisez la commande :
```
make bench
```
### Important
Il est de désactiver le commentaire mit sur la ligne 3 du fichier `meson.build` (en enlevant le # au début de la ligne) pour désactiver l'inerface et ainsi augmenter les performances.
