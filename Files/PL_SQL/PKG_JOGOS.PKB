---------Excecoes---------
--VAL_NEG
--GEN_INV
--DUP_VAL_ON_INDEX
--ANO_IMP
--ANT_NA
--ED_MEN
--NASC_DPS
--POS_INV
--NULL_PART

create or replace PACKAGE BODY pkg_jogos IS


-- Funcao 1
PROCEDURE regista_atleta(
    nome_in IN atleta.nome%TYPE,
    nascimento_in IN atleta.nascimento%TYPE,
    genero_in IN atleta.genero%TYPE,
    peso_in IN atleta.peso%TYPE,
    pais_in IN atleta.pais%TYPE)
IS
CURSOR numero_max IS SELECT MAX(numero) as maxN FROM atleta;                -- Criamos um cursor que vai buscar o numero MAX na tabela atleta
n_max numero_max%ROWTYPE;
n_out atleta.numero%TYPE;
VAL_NEG EXCEPTION;                                                          -- Excepcao para quando algum valor e negativo
GEN_INV EXCEPTION;                                                          -- Excepcao para quando o genero dado nao é igual a m f M F
BEGIN
IF(nascimento_in <= 0 OR peso_in <= 0) THEN                                 -- Caso algum valor seja negativo levantamos um erro
    RAISE VAL_NEG;
    DBMS_OUTPUT.PUT_LINE('Removido a edicao');
ELSIF(UPPER(genero_in) != 'M' AND UPPER(genero_in) != 'F') THEN             -- Caso o genero seja diferente de M ou F levantamos um erro
    RAISE GEN_INV;
END IF;

OPEN numero_max;
FETCH numero_max INTO n_max;                                                -- Colocamos o valor do cursor na variavel numero_max
CLOSE numero_max;

INSERT INTO atleta(numero, nome, nascimento, genero, peso, pais)            -- Inserir valores
    VALUES(n_max.maxN + 1, nome_in, nascimento_in, genero_in, peso_in, pais_in); 

n_out := n_max.maxN + 1;                                                    -- Colocar o valor do atleta criado na variavel n_out
    DBMS_OUTPUT.PUT_LINE('O Numero escolhido para o atleta foi o:  ' || n_out);
    
EXCEPTION                                                                   -- Levantamento de excecoes
    WHEN  VAL_NEG THEN
    RAISE_APPLICATION_ERROR(-20001,'Valores negativos inseridos');
    WHEN GEN_INV THEN
    RAISE_APPLICATION_ERROR(-20002,'Genero invalido (M, F) ');    
    WHEN DUP_VAL_ON_INDEX THEN
        RAISE_APPLICATION_ERROR(-20003, 'Numero de atleta e UNIQUE...');
n_out := n_max.maxN + 1;                                                    -- Imprimir o numero do atleta criado para o utilizador
DBMS_OUTPUT.PUT_LINE('O Numero escolhido para o atleta foi o:  ' || n_out);

END regista_atleta;


-- Funcao 2
PROCEDURE regista_edicao(
	edicao_in	IN jogos.edicao%TYPE,
	ano_in		IN jogos.ano%TYPE,
	pais_in		IN jogos.pais%TYPE,
	cidade_in	IN jogos.cidade%TYPE)
IS
CURSOR cursor_ano IS SELECT ano from jogos where edicao = edicao_in -1;     -- Cursor com o ano da edicao anterior a que queremos inserir
ano_edicao_ant cursor_ano%ROWTYPE;
ANO_IMP EXCEPTION;                                                          -- Excecao para quanto o ano e impossivel (tem de ser 4 anos a mais que o anterior)
ANT_NA EXCEPTION;                                                           -- Excecao para quando a edicao anterior a edicao que queremos adicionar nao existe (fica impossivel calcular o ano correto) 
ED_MEN EXCEPTION;                                                           -- Excecao para quando a edicao e menor que 1
BEGIN
OPEN cursor_ano;    
FETCH cursor_ano INTO ano_edicao_ant;                                       -- Colocamos o valor do cursor na variavel cursor_ano
CLOSE cursor_ano;

    IF edicao_in > 1 THEN                                                   -- Caso a edicao seja maior que 1 e necessario outras condicoes se verificarem
        IF (ano_edicao_ant.ano IS NULL) THEN                                -- Caso o ano anterior seja NULL (a edicao anterior nao foi colocada entao nao e possicel calcular o ano correto)
            RAISE ANT_NA;                   
        ELSIF( (ano_in != (ano_edicao_ant.ano + 4))) THEN
            RAISE ANO_IMP;                                                  -- Caso o ano nao seja igual ao ano anterior + 4 (jogos olimpicos a cada 4 anos)                              
        END IF;
    ELSIF edicao_in < 1 THEN
        RAISE ED_MEN;                                                       -- Caso a edicao seja menor que 1
    END IF;

INSERT INTO jogos(edicao, ano, pais, cidade)                                -- Inserir valores
    VALUES(edicao_in, ano_in, pais_in, cidade_in);
    
EXCEPTION                                                                   -- Levantamento de excecoes
    WHEN DUP_VAL_ON_INDEX THEN
        RAISE_APPLICATION_ERROR(-20003, 'Ano e edicao tem de ser UNIQUES...');
    WHEN ANO_IMP THEN
        RAISE_APPLICATION_ERROR(-20004, 'O ano nao bate certo (jogos de 4 em 4 anos)...');
    WHEN ANT_NA THEN
        RAISE_APPLICATION_ERROR(-20005, 'Edicao ' || (edicao_in - 1) || ' ainda nao existe...');
    WHEN ED_MEN THEN
        RAISE_APPLICATION_ERROR(-20006, 'Edicao tem de ser >= 1');
    
END regista_edicao;


-- Funcao 3
PROCEDURE regista_participacao(
    atleta_in IN participa.atleta%TYPE,
    jogos_in IN participa.jogos%TYPE,
    modalidade_in IN participa.modalidade%TYPE,
    posicao_in IN participa.posicao%TYPE)
IS                                                                          -- Cursor que guarda em que ano aconteceu a edicao inserida e o ano em que o atleta inserido nasceu
CURSOR cursor_ano IS SELECT jogos.ano AS anoJ, atleta.nascimento AS anoA FROM jogos, atleta WHERE edicao = jogos_in AND atleta.numero = atleta_in;  
anoS cursor_ano%ROWTYPE;
NASC_DPS EXCEPTION;                                                         -- Excecao para quando o atleta nasce depois da edicao ocorrer
POS_INV EXCEPTION;                                                          -- Excecao para quando a posicao e invalida (numero negativos)
BEGIN
OPEN cursor_ano;
FETCH cursor_ano INTO anoS;                                                 -- Colocamos o valor do cursor na variavel cursor_ano
CLOSE cursor_ano;

IF (anoS.anoJ <= anoS.anoA) THEN                                            -- Caso o atleta tenha nascido depois da edicao
    RAISE NASC_DPS; 
ELSIF (posicao_in <1) THEN                                                  -- Caso a posicao seja invalida
    RAISE POS_INV;  
END IF;

INSERT INTO participa(atleta, jogos, modalidade, posicao)                   -- Inserir valores
    VALUES(atleta_in, jogos_in, modalidade_in, posicao_in);
    
EXCEPTION                                                                   -- Levantamento de excecoes
    WHEN DUP_VAL_ON_INDEX THEN
        RAISE_APPLICATION_ERROR(-20007, 'Conjunto Atleta-edicao-modalidade sao UNIQUE...');
    WHEN NASC_DPS THEN
        RAISE_APPLICATION_ERROR(-20008, 'O Atleta nao pode ter nascido depois da edicao ter ocorrido...');
    WHEN POS_INV THEN
        RAISE_APPLICATION_ERROR(-20009, 'A posicao nao pode ser menos que 1...');
END regista_participacao;   


-- Funcao 4
PROCEDURE remove_participacao(
    atleta_in IN participa.atleta%TYPE,
    jogos_in IN participa.jogos%TYPE,
    modalidade_in IN participa.modalidade%TYPE)
IS
NULL_PART EXCEPTION;                                                        -- Excecao para quando a participacao que queremos remover nao existe
BEGIN
    DELETE FROM participa                                                   -- Remover da tabela participa quando a linha da mesma se iguala aos valores recebidos
            WHERE atleta_in = atleta
            AND jogos_in = jogos
            AND modalidade_in = modalidade;

IF (SQL%ROWCOUNT = 0) THEN                                                  -- Caso nao haja nada a ser removido com os parametros recebidos
      RAISE NULL_PART;
    END IF;
    
EXCEPTION                                                                   -- Levantamento de excecoes
    WHEN NULL_PART THEN
        RAISE_APPLICATION_ERROR(-20010, 'Participacao a remover nao existe.');

END remove_participacao;


-- Funcao 5
PROCEDURE remove_edicao(
    numero_in IN jogos.edicao%TYPE)
IS                                                                          -- Cursor que guarda o numero do atleta, a edicao dos jogos e a modalidade para todas as participacoes que tem edicao igual a que queremos remover
CURSOR edicao_A IS SELECT participa.atleta AS atleta, participa.jogos as jogos, participa.modalidade as modalidade
                        FROM participa
                        WHERE participa.jogos = numero_in;
NULL_PART EXCEPTION;                                                        -- Excepcao para quando nao tem nada a remover na edicao inserida
BEGIN

FOR atl IN edicao_A LOOP                                                    -- Loop que pega em cada linha do cursor criado e elimina da tabela de participacao as linhas com valores semelhantes (chamando o procedimento remove_participacao)
    remove_participacao(atl.atleta, atl.jogos, atl.modalidade);
END LOOP;
CLOSE edicao_A;

DELETE FROM jogos                                                           -- Eliminar a edicao em si
    WHERE jogos.edicao = numero_in;
IF (SQL%ROWCOUNT = 0) THEN                                                  -- Caso nao haja edicao para remover com o valor recebido
    RAISE NULL_PART;
END IF;

EXCEPTION                                                                   -- Levantamento de excepcoess
    WHEN NULL_PART THEN
        RAISE_APPLICATION_ERROR(-20011, 'Edicao a remover nao existe.');
END remove_edicao;


-- Funcao 6
PROCEDURE remove_atleta(
    numero_in IN atleta.numero%TYPE)
IS                                                                          -- Cursor que guarda todas as participacoes nos jogos olimpicos do atleta cujo numero foi inserido
CURSOR participacao_A IS SELECT participa.atleta AS atleta, participa.jogos as jogos, participa.modalidade as modalidade
                            FROM participa
                            WHERE participa.atleta = numero_in;
NULL_PART EXCEPTION;                                                        -- Excecao para quando o atleta inserido nao existe
BEGIN
    
FOR atl IN participacao_A LOOP                                              -- Loop que pega em cada linha do cursor e elimina da tabela de participacao as linhas com valores semelhantes (chamando a funcao remove_participacao)
    remove_participacao(atl.atleta, atl.jogos, atl.modalidade);
    DBMS_OUTPUT.PUT_LINE('Removido a participacao' || atl.atleta || ' ' || atl.jogos || ' ' || atl.modalidade);
END LOOP;
CLOSE participacao_A;

DELETE FROM atleta                                                          -- Eliminar a edicao em si
    WHERE atleta.numero = numero_in;
IF (SQL%ROWCOUNT = 0) THEN                                                  -- Caso nao haja atleta para remover com o valor recebido
    RAISE NULL_PART;
END IF;

EXCEPTION                                                                   -- Levantamento de excepcoess
    WHEN NULL_PART THEN
    RAISE_APPLICATION_ERROR(-20012, 'Atleta a remover nao existe.');
END remove_atleta;


-- Funcao 7
FUNCTION lista_medalhas(
    atleta_in IN atleta.numero%TYPE)
    RETURN SYS_REFCURSOR
IS
cursor_lista_medalhas SYS_REFCURSOR;                                        -- Criamos um cursor para ser devolvido pela funcao

BEGIN
OPEN cursor_lista_medalhas FOR                                              -- Colocamos as participacoes de um atleta em edicoes de jogos, nas quais tenha ficado nas tres primeiras posicoes
        SELECT  A.numero, A.nome, J.ano, P.modalidade, P.posicao
            FROM atleta A, jogos J, participa P
            WHERE P.atleta = A.numero
            AND P.jogos = J.edicao
            AND P.posicao <= 3
            AND P.atleta = atleta_in
            ORDER BY P.posicao;
        
    RETURN cursor_lista_medalhas;                                           -- Devolvemos o valor do cursor cursor_lista_medalhas 
CLOSE cursor_lista_medalhas;  

END lista_medalhas;


END pkg_jogos;
