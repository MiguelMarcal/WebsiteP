
let regra_um nr =                                                                   (*Recebe um inteiro como argumento, devolve metade desse inteiro se o*)
    if nr mod 2 = 0 && ((nr / 2) >= 42) then (nr / 2) else max_int;;                (*numero for par ou max_int se nao se verificar *)

let regra_dois nr =                                                                 (*Recebe um inteiro como argumento*)
    if (nr mod 3 = 0 || nr mod 4 = 0) && (((nr mod 100) / 10) * (nr mod 10)) > 0    (*caso o numero seja multiplo de tres e e de quatro e a multiplicação dos ultimos dois algarismos seja diferente de 0*) 
    then  (nr - (((nr mod 100) / 10) * (nr mod 10))) else max_int;;                 (*devolve esse numero menos a multiplicação dos ultimos dois algarismos do mesmo ou max int caso nao o seja*)

let regra_tres nr =                                                                 (*Recebe um inteiro como argumento e devolve esse inteiro menos 42 *)                        
    if nr mod 5 = 0 && ((nr - 42) >= 42) then (nr - 42) else max_int;;              (*caso esse inteiro seja multiplo de 5 ou max_int se nao o for*)

let rec alg valor tam =                         (*Função recursiva que recebe 2 inteiros como argumentos, "valor" que é o numero inserido (alterado) e tam que indica quantas vezes ja foram executadas regras ao valor, dando return desse valor tam*)                                  
    if valor = 42 then tam                      (*Caso o valor seja 42 retornamos o tam(numero de regras usadas) *)                      
    else if valor < 42 then max_int             (*Caso o valor seja menor de 42 nao podemos efectuar nenhuma regra então devolvemos uma flag que indica que nao chegamos a 42 *)
    else
        let x = regra_um valor in               (*Atribuimos a x o "return" da função regra_um*)
        let y = regra_dois valor in             (*Atribuimos a y o "return" da função regra_dois*)
        let z = regra_tres valor in             (*Atribuimos a z o "return" da função regra_tres*)
        if ((x != max_int) && (y != max_int) && (z != max_int)) then    (*Colocamos todas as ocorrencias possiveis para o inteiro inserido (relativamente ao cumprimento das regras)*)
            min (alg x(tam+1)) (min (alg y(tam+1)) (alg z (tam+1)))     (*Damos uso da função min, e verificamos de todas as possibilidades de ocorrencias qual é que atinge o valor 42 com a variavel tam menor *)
        else if ((x != max_int) && (y != max_int)) then
            min (alg y(tam+1)) (alg x(tam+1))
        else if (y != max_int) && (z != max_int) then
            min (alg y(tam+1)) (alg z (tam+1))
        else if (x != max_int) && (z != max_int) then
            min (alg x(tam+1)) (alg z (tam+1))
        else if (z != max_int) then
            alg z (tam+1)
        else if (x != max_int) then
            alg x(tam+1)
        else if (y != max_int) then
            alg y(tam+1)
        else max_int
;;

let main =
    let numinicial = read_int() in  (*Ler o Valor ao qual vamos aplicar as regras *)
    let p = alg(numinicial) 0 in    (*Associamos a uma variavel p o return da função alg (o return é o inteiro "tam") *)
    if p = max_int then Printf.printf "BAD LUCK\n" else Printf.printf "%d\n" p;;        (*Imprimimos "BAD LUCK" caso nao consigamos chegar a 42 ou p caso cheguemos ao valor pretendido *)
