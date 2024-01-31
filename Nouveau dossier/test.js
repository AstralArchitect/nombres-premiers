const readline = require('readline');

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

function main() {
    rl.question('Combien de nombres premiers voulez-vous afficher ? ', (reponse) => {
    let fin = parseInt(reponse);
    let nombre = 3;
    let liste = [2];
    let moitie = false
    console.log("recherche...")
    while (liste.length < fin){
        if(liste.length === fin / 2 && !(moitie)){
            console.log("50%")
            moitie = true
        }
        let estPremier = true;
        for(let i = 0; i < liste.length; i++){
            if(nombre % liste[i] === 0){
                estPremier = false;
                break;
            }
        }
        if(estPremier){
            liste.push(nombre);
        }
        nombre++;
    }
    console.log("Voici la liste des " + fin + " nombres premiers")
    for(let i = 0;i < liste.length;i++){
        console.log(liste[i]);
    }
    
    a = 1
    rl.close();
    });
}
let a = 0
main();
while (true){
    if (a === 1){
        main();
        a = 0;
    }
}