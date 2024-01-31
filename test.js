/*let nombre = parseInt(reponse);
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
*/
let nombre = 3;
let liste = [2];
while (true){
    for(let i = 0; i === liste.length;i++){
        if(nombre % liste[i] === 0){
            break
        }else{
            liste.push(nombre)
            console.log(nombre)
            nombre++
        }
    }
}