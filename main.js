const readline = require('readline');

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

rl.question('quel nombre voulez-vous tester ? ', (reponse) => {
  let nombre = parseInt(reponse);
  let estPremier = true;
  for(let i = 2;i <= Math.sqrt(nombre);i++){
    if(nombre % i === 0){
        estPremier = false
        break
    }
  }
  if(estPremier){
    console.log(nombre + " est un nombre premier.")
  }else if(!(estPremier)){
    console.log(nombre + " n'est pas un nombre premier.")
  }
  rl.close();
});
