let rec guloso array nP indiceMoeda valorTeste = 
    let tam = Array.length array in
    if (indiceMoeda < tam) then
        if(array.(indiceMoeda) = 0) then
          max_int
        else if ((valorTeste - array.(indiceMoeda)) > 0) then
            guloso array (nP + 1) indiceMoeda (valorTeste - array.(indiceMoeda))
        else if ((valorTeste - array.(indiceMoeda)) = 0) then
            (nP + 1)
        else guloso array nP (indiceMoeda+1) (valorTeste)
    else max_int
;;

let dinamico currencyArray valor =
    (**moedas é o vetor com as moedas *) 
    (**valor é o valor a ser testado que sera tambem o valor final *)
    let tamanho = Array.length currencyArray in
    let arrayResposta = Array.make (valor + 1) (valor+1) in
    arrayResposta.(0) <- 0;

    for i = 1 to valor do
        for o = 0 to tamanho - 1 do
            if(currencyArray.(o) <= i) then
                arrayResposta.(i) <- (  min (arrayResposta.(i)) (arrayResposta.(i - currencyArray.(o) ) + 1));
            done;
    done;
    arrayResposta.(valor);
;;

let main = 
    let nMoedas = read_int() in
    
    let currencyArray = Array.make nMoedas 0 in     (**Cria o array com ordem contraria á inserida, ficando em ordem decrescente *)
    for i = (nMoedas - 1) downto 0 do
        let num = read_int() in
        currencyArray.(i) <- num;
    done;

    if nMoedas > 2 then
        let finalPossivel = currencyArray.(0) + currencyArray.(1) in 
        let inicioPossivel = currencyArray.(nMoedas - 3) + 1 in
        let q = ref (inicioPossivel + 1) in
        let k = ref 0 in

    
        while ((!q < finalPossivel) && (!k = 0)) do
            let numbero = dinamico currencyArray !q in
            let x = guloso currencyArray 0 0 !q in
            if (x = numbero) then 
                q := !q + 1
            else
                k := !q
        done;
        let p = !q in

        if (!k = 0) then Printf.printf "YES\n"
        else 
        Printf.printf "%d\n" p

    else Printf.printf "YES\n"
;;