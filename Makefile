all:
	gcc Code/Recherche_nombresPremiers/main.c -lpthread  -o Bin/Recherche
	gcc Code/verfierPrimarité/main.c -o Bin/Verifier
search:
	gcc Code/Recherche_nombresPremiers/main.c -lpthread -o Bin/Recherche
if:
	gcc Code/verfierPrimarité/main.c -o Bin/Verifier
