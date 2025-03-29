# Description
Ceci est un programme en C qui permet de rechercher des nombres premiers.
## Utilisation
Voici un aperçu de la commande :
```
builddir/main [End(int)] [no_print(bool)]
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

Pour utiliser le programme en mode bench, tapez ```meson test -C builddir``` dans le terminal.