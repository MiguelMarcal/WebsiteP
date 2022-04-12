-----------------------------------------------------------


ALTER SESSION SET NLS_DATE_FORMAT = 'YYYY.MM.DD';


-----------------------------------------------------------
DROP TABLE compete ;
DROP TABLE comitiva ;
DROP TABLE disputada ;
DROP TABLE categoria;
DROP TABLE modalidade;
DROP TABLE jogos_olimpicos;
DROP TABLE pais;
DROP TABLE regiao;
DROP TABLE atleta;

-----------------------------------------------------------
CREATE TABLE atleta(
    n_identificacao NUMBER(5),
    nome VARCHAR(50)    CONSTRAINT nn_atleta_nome NOT NULL,
    ano_nascimento NUMBER(4) CONSTRAINT nn_atleta_ano_nascimento NOT NULL,
    genero VARCHAR(10)  CONSTRAINT nn_atleta_genero NOT NULL,
    altura_cm NUMBER(3) CONSTRAINT nn_atleta_altura NOT NULL,
    peso_kg NUMBER(4,1) CONSTRAINT nn_atleta_peso NOT NULL,
--
    CONSTRAINT pk_atleta
    PRIMARY KEY(n_identificacao),
--
    CONSTRAINT ck_atleta_genero_ria12
    CHECK(genero = 'feminino' OR genero = 'masculino'),
--
    CONSTRAINT ck_atleta_altura
    CHECK(altura_cm > 0),
--
    CONSTRAINT ck_atleta_peso
    CHECK(peso_kg > 0)
);

CREATE TABLE regiao(
    id_regiao NUMBER(4),
    nome VARCHAR(20)    CONSTRAINT nn_regiao_nome NOT NULL,
    pib_pc NUMBER(20),
    n_habitantes NUMBER(9) CONSTRAINT nn_regiao_n_habitantes NOT NULL,
--
    CONSTRAINT pk_regiao
    PRIMARY KEY(id_regiao),
--
    CONSTRAINT ck_regiao_pib
    CHECK(pib_pc > 0),
--
    CONSTRAINT ck_regiao_n_habitantes
    CHECK(n_habitantes > 0)

);

CREATE TABLE pais(
    id_regiao,
    codigo VARCHAR(3) CONSTRAINT nn_pais_codigo_ria11 NOT NULL UNIQUE,
--
    CONSTRAINT pk_pais_id_regiao
    PRIMARY KEY(id_regiao),
--
    CONSTRAINT fk_pais_id_regiao
    FOREIGN KEY(id_regiao)
    REFERENCES regiao(id_regiao)
    ON DELETE CASCADE 
);

CREATE TABLE jogos_olimpicos(
    n_edicao NUMBER(3),
    ano NUMBER(4)   CONSTRAINT nn_jogos_ano NOT NULL,
    data_abertura DATE ,
    data_encerramento DATE,
--
    CONSTRAINT pk_jogos
    PRIMARY KEY(n_edicao),
--
    CONSTRAINT ck_jogos_ano
    CHECK(ano >= 1896),
--
    CONSTRAINT ck_jogos_ria4
    CHECK(data_abertura < data_encerramento)
);

CREATE TABLE modalidade(
    nomeM VARCHAR(50),
--
    CONSTRAINT pk_modalidade
    PRIMARY KEY(nomeM)

);

CREATE TABLE categoria(
    nomeC VARCHAR(50) CONSTRAINT unnn_categoria_nomeC UNIQUE NOT NULL,
    nomeM,
    genero VARCHAR(50)  CONSTRAINT nn_categoria_genero NOT NULL,
--
    CONSTRAINT pk_categoria_nomeM
    PRIMARY KEY(nomeM, nomeC),
--
    CONSTRAINT fk_categoria_nomem
    FOREIGN KEY(nomeM)
    REFERENCES modalidade(nomeM)
    ON DELETE CASCADE,
--
    CONSTRAINT ck_categoria_genero_ria12
    CHECK(genero = 'feminino' OR genero = 'masculino')
    
);


CREATE TABLE disputada(
    nomeM,
    nomeC,
--
    CONSTRAINT pk_dispuata_mc
    PRIMARY KEY(nomeM, nomeC),
--  

    CONSTRAINT fk_disputada_mc
    FOREIGN KEY(nomeM, nomeC)
    REFERENCES categoria(nomeM, nomeC)
    ON DELETE CASCADE

);

CREATE TABLE comitiva(
    id_regiao,
    n_edicao,
    data_inscricao DATE CONSTRAINT nn_comitiva_data_inscricao NOT NULL,
--
    CONSTRAINT pk_comitiva
    PRIMARY KEY(id_regiao, n_edicao),
--
    CONSTRAINT fk_comitiva_id_regiao
    FOREIGN KEY(id_regiao)
    REFERENCES regiao(id_regiao)
    ON DELETE CASCADE,
--
    CONSTRAINT fk_comitiva_n_edicao
    FOREIGN KEY(n_edicao)
    REFERENCES jogos_olimpicos(n_edicao)
    ON DELETE CASCADE

);

CREATE TABLE compete(
    n_identificacao,
    nomeC,
    nomeM,
    n_edicao,
    id_regiao,
    posicao VARCHAR(5),
--
    CONSTRAINT pk_compete_ria1
    PRIMARY KEY( n_identificacao, nomeC, nomeM, n_edicao, id_regiao),
--
    CONSTRAINT fk_compete_disputada_cm
    FOREIGN KEY (nomeC, nomeM)
    REFERENCES disputada(nomeC, nomeM),
--
    CONSTRAINT fk_comete_comitiva_nedi_idreg
    FOREIGN KEY (n_edicao, id_regiao)
    REFERENCES comitiva(n_edicao, id_regiao),
--  
    CONSTRAINT fk_compete_atleta_id
    FOREIGN KEY (n_identificacao)
    REFERENCES atleta(n_identificacao)

);
-----------------------------------------------------------


INSERT INTO atleta(n_identificacao, nome, ano_nascimento, genero, altura_cm, peso_kg)
    VALUES(1234, 'Adam Peaty', 1994, 'masculino', 191 , 85 );
INSERT INTO atleta(n_identificacao, nome, ano_nascimento, genero, altura_cm, peso_kg)
    VALUES(5678, 'Eliud Kipchoge', 1985, 'masculino', 167 , 52 );
INSERT INTO atleta(n_identificacao, nome, ano_nascimento, genero, altura_cm, peso_kg)
    VALUES(0000, 'Rosa Mota', 1958, 'feminino', 157 , 45 );  
INSERT INTO atleta(n_identificacao, nome, ano_nascimento, genero, altura_cm, peso_kg)
    VALUES(1111, 'Jorge Fonseca', 1992, 'masculino', 174 , 100);
INSERT INTO atleta(n_identificacao, nome, ano_nascimento, genero, altura_cm, peso_kg)
    VALUES(3333, 'Aaron Wolf', 1996, 'masculino', 180 , 100);
INSERT INTO atleta(n_identificacao, nome, ano_nascimento, genero, altura_cm, peso_kg)
    VALUES(5612,'Thiago Braz',1993,'masculino',183,75);
INSERT INTO atleta(n_identificacao, nome, ano_nascimento, genero, altura_cm, peso_kg)
    VALUES(5784,'Arthur Zanetti',1990, 'masculino', 156, 62);
INSERT INTO atleta(n_identificacao, nome, ano_nascimento, genero, altura_cm, peso_kg)
    VALUES(9984,'Maicon de Andrade',1993, 'masculino', 193, 93);
---
---
---
INSERT INTO regiao(id_regiao,nome,pib_pc,n_habitantes)
    VALUES(024,'Brasil',8710,206200000);
INSERT INTO regiao(id_regiao,nome,pib_pc,n_habitantes)
    VALUES(224,'Rio de Janeiro',44222,6748000);   
INSERT INTO regiao(id_regiao,nome,pib_pc,n_habitantes)
    VALUES(186,'Reino Unido',42449,64525312);
INSERT INTO regiao(id_regiao,nome,pib_pc,n_habitantes)
    VALUES(386,'Londres',NULL,8257000);
INSERT INTO regiao(id_regiao,nome,pib_pc,n_habitantes)
    VALUES(086,'Jap?o',NULL,126300000);
INSERT INTO regiao(id_regiao,nome,pib_pc,n_habitantes)
    VALUES(286,'Tokyo',NULL,13503810);
INSERT INTO regiao(id_regiao,nome,pib_pc,n_habitantes)
    VALUES(061,'Fran?a',NULL,345345);
INSERT INTO regiao(id_regiao,nome,pib_pc,n_habitantes)
    VALUES(261,'Paris',NULL,543322);    
INSERT INTO regiao(id_regiao,nome,pib_pc,n_habitantes)
    VALUES(067,'Grecia',NULL,3453444);
--- 
---
---
INSERT INTO pais(id_regiao,codigo)
    VALUES(067,'GRC');
INSERT INTO pais(id_regiao,codigo)
    VALUES(186,'GBR');
INSERT INTO pais(id_regiao,codigo)
    VALUES(086,'JAP');
INSERT INTO pais(id_regiao,codigo)
    VALUES(061,'FRA');
INSERT INTO pais(id_regiao,codigo)
    VALUES(024,'BRA');
---
---
---
INSERT INTO jogos_olimpicos(n_edicao, ano, data_abertura, data_encerramento)
    VALUES(1, 1986, '1896-04-06', '1896-04-15');
INSERT INTO jogos_olimpicos(n_edicao, ano, data_abertura, data_encerramento)
    VALUES(32, 2020, '2021-07-23', '2021-08-08');
INSERT INTO jogos_olimpicos(n_edicao, ano, data_abertura, data_encerramento)
    VALUES(31, 2016, '2016-8-5', '2016-8-21');
INSERT INTO jogos_olimpicos(n_edicao, ano, data_abertura, data_encerramento)
    VALUES(16, 1956, '1956-11-22', '1956-12-08');
INSERT INTO jogos_olimpicos(n_edicao, ano, data_abertura, data_encerramento)
    VALUES(30, 2012, '2012-11-22', '2012-12-08');
INSERT INTO jogos_olimpicos(n_edicao, ano, data_abertura, data_encerramento)
    VALUES(33, 2024, NULL, NULL);
INSERT INTO jogos_olimpicos(n_edicao, ano, data_abertura, data_encerramento)
    VALUES(28, 2004, NULL, NULL);
---
---
---
INSERT INTO modalidade (nomem)
    VALUES('Natacao');
INSERT INTO modalidade (nomem)
    VALUES('Atletismo');
INSERT INTO modalidade (nomem)
    VALUES('Ginastica');
INSERT INTO modalidade (nomem)
    VALUES('Combate');
---   
---
---
INSERT INTO categoria(nomec, genero, nomem)
    VALUES('100m brucos', 'masculino', 'Natacao');
INSERT INTO categoria(nomec, genero, nomem)
    VALUES('24,195km Maratona', 'feminino', 'Atletismo');
INSERT INTO categoria(nomec, genero, nomem)
    VALUES('Geral-Individual', 'feminino', 'Ginastica');
INSERT INTO categoria(nomec, genero, nomem)
    VALUES('Judo', 'masculino', 'Combate');
INSERT INTO categoria(nomec, genero, nomem)
    VALUES('Argolas', 'masculino', 'Ginastica');    
INSERT INTO categoria(nomec, genero, nomem)
    VALUES('Taekwondo', 'masculino', 'Combate');
---
---   
---
INSERT INTO comitiva(id_regiao,n_edicao,data_inscricao)
    VALUES(186,30,'2005-06-06');
INSERT INTO comitiva(id_regiao,n_edicao,data_inscricao)
    VALUES(024,31,'2009-10-02');
INSERT INTO comitiva(id_regiao,n_edicao,data_inscricao)
    VALUES(086,32,'2013-09-07');
INSERT INTO comitiva(id_regiao,n_edicao,data_inscricao)
    VALUES(061,33,'2024-07-26');    
---
---
---
INSERT INTO disputada(nomeM,nomeC)
	VALUES('Combate','Judo');
INSERT INTO disputada(nomeM,nomeC)
	VALUES('Natacao','100m brucos');
INSERT INTO disputada(nomeM,nomeC)
	VALUES('Ginastica','Argolas');
INSERT INTO disputada(nomeM,nomeC)	
    VALUES('Combate','Taekwondo');
---
---
---
INSERT INTO compete(nomeM,nomeC,n_identificacao,n_edicao,id_regiao,posicao)
	VALUES('Combate','Judo',3333,32,086,1);
INSERT INTO compete(nomeM,nomeC,n_identificacao,n_edicao,id_regiao,posicao)
	VALUES('Natacao','100m brucos',1234,32,086,1);
INSERT INTO compete(nomeM,nomeC,n_identificacao,n_edicao,id_regiao,posicao)
	VALUES('Ginastica','Argolas',5784,31,024,2);
INSERT INTO compete(nomeM,nomeC,n_identificacao,n_edicao,id_regiao,posicao)
	VALUES('Combate','Taekwondo',9984,31,024,3);
