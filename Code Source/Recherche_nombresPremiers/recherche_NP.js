const readline = require('readline');

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});
console.log("Bienvenue sur le chercheur de nombres premiers de Astral Architect")

function main() {
    rl.question('Combien de nombres premiers voulez-vous afficher ? ', (reponse) => {
    let fin = parseInt(reponse);
    let nombre = 3;
    let liste = [2];
    console.log(liste[0])
    while (liste.length < fin){
        let estPremier = true;
        for(let i = 0; i < liste.length; i++){
            if(nombre % liste[i] === 0){
                estPremier = false;
                break;
            }
        }
        if(estPremier){
            liste.push(nombre);
            console.log(nombre);
        }
        nombre++;
    }
    console.log("La fenêtre va se fermer dans 5 secondes")
    function apres5Secondes() {
        console.log("fermeture de la fenêtre...");
    }
    
    // Définir le délai en millisecondes (5 secondes équivaut à 5000 millisecondes)
    setTimeout(apres5Secondes, 5000);
    rl.close();
    });
}
main();