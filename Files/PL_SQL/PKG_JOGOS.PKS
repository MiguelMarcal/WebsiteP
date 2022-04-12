create or replace PACKAGE PKG_jogos IS


-- Cria um novo resisto de atleta               PERGUNTA 1
PROCEDURE regista_atleta(
    nome_in IN atleta.nome%TYPE,
    nascimento_in IN atleta.nascimento%TYPE,
    genero_in IN atleta.genero%TYPE,
    peso_in IN atleta.peso%TYPE,
    pais_in IN atleta.pais%TYPE);


-- Cria um novo registro de edição              PERGUNTA 2      
PROCEDURE regista_edicao(
	edicao_in	IN jogos.edicao%TYPE,
	ano_in		IN jogos.ano%TYPE,
	pais_in		IN jogos.pais%TYPE,
	cidade_in	IN jogos.cidade%TYPE);
    
    
-- Cria um novo registro de Participacao        PERGUNTA 3
PROCEDURE regista_participacao(
    atleta_in IN participa.atleta%TYPE,
    jogos_in IN participa.jogos%TYPE,
    modalidade_in IN participa.modalidade%TYPE,
    posicao_in IN participa.posicao%TYPE);


-- Elimina um registo de participação           PERGUNTA 4
PROCEDURE remove_participacao(
    atleta_in IN participa.atleta%TYPE,
    jogos_in IN participa.jogos%TYPE,
    modalidade_in IN participa.modalidade%TYPE);
    

-- Elimina um registo de edicao                 PERGUNTA 5
PROCEDURE remove_edicao(
    numero_in IN jogos.edicao%TYPE);


-- Elimina um registo de atleta                 PERGUNTA 6
PROCEDURE remove_atleta(
    numero_in IN atleta.numero%TYPE);


-- Dá o return das medalhas do atleta           PERGUNTA 7
FUNCTION lista_medalhas(
    atleta_in IN atleta.numero%TYPE)
    RETURN SYS_REFCURSOR;

END PKG_jogos;
