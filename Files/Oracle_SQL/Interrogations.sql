--ETAPA 3 DO PROJETO DE SIBD 2021/2022 GRUPO38
--André Carreira fc53501 TP16
--Marcos Gonçalves fc56946 TP16
--Miguel Marçal fc57542 TP16
--Rogério Silva fc49554 TP16



----------------------------------------------------------
-----Interrogação 1
----------------------------------------------------------
-----Número, nome, e idade das atletas femininas com apelido Lopes, que ficaram pelo menos
-----uma vez nos três primeiros lugares em alguma participação em jogos olímpicos no Japão.
-----A edição e cidade dos jogos também devem ser mostradas. O resultado deve vir ordenado
-----pela edição dos jogos e pela idade da atleta de forma descendente, e pelo nome e número
-----da atleta de forma ascendente.
----------------------------------------------------------
SELECT DISTINCT A.numero, A.nome, 2021 - A.nascimento AS idade, J.edicao, J.cidade
  FROM   atleta A,(SELECT UNIQUE atleta,jogos, posicao      --Subquery que devolve uma tabela com o numero de atleta, a edicao dos jogos e a posicao em que o atleta ficou colocado na edicao correspondente
                     FROM  participa,jogos
                    WHERE (posicao <= 3)
                      AND (jogos = edicao)
                      AND (pais = 'Japao') 
                    ORDER  BY atleta) NNI, jogos J
 WHERE (A.numero = NNI.atleta)
   AND (A.genero = 'F')
   AND (J.edicao = NNI.jogos)
   AND (A.nome LIKE '%Lopes%')
 ORDER BY J.edicao DESC, idade DESC, A.nome ASC, A.numero ASC;

----------------------------------------------------------
-----Interrogação 2
----------------------------------------------------------
-----Número e nome dos atletas masculinos que nunca participaram em jogos olímpicos realizados 
-----no seu país (independentemente da modalidade), e que participaram fora do seu país
-----menos de três jogos na modalidade de natação. Inclua no resultado os atletas que ainda
-----não participaram em quaisquer jogos. O resultado deve vir ordenado pelo nome do atleta
-----de forma ascendente e pelo número do atleta de forma descendente
----------------------------------------------------------
SELECT A.nome, A.numero
  FROM   atleta A
 WHERE  A.numero IN (SELECT UNIQUE a.numero               --Subquery que devolve o numero dos atletas que tenham participado menos de 3 vezes na modalidade de nataçao e que nao tenham participado em jogos no seu proprio pais
                       FROM   atleta A, participa P, jogos J, 
                       (SELECT atleta.numero, Count(participa.modalidade) AS nparticipacoes ,atleta.pais  AS Apais, atleta.genero --Subquery que devolve o numero de vezes que cada atleta tenha participado em jogos na modalidade de natacao
                          FROM   atleta
                          LEFT JOIN participa ON atleta.numero = participa.atleta 
                           AND (participa.modalidade = '%Natacao%')
                           AND (atleta.numero IS NOT NULL)
                         GROUP  BY atleta.pais, atleta.genero, atleta.numero
                         ORDER  BY atleta.numero DESC) participacoes

                      WHERE  (participacoes.nparticipacoes < 3)
                        AND (participacoes.numero = A.numero) )
                        AND (A.numero NOT IN (SELECT A.numero     ---Subquery que devolve os atletas que ja participaram em jogos no seu proprio pais
                                               FROM   atleta A, participa P, jogos J
                                              WHERE  (P.jogos = J.edicao)
                                                AND (P.atleta = A.numero)
                                                AND (J.pais = A.pais)))
   AND (A.genero = 'M')
 ORDER  BY A.nome ASC,A.numero DESC;

----------------------------------------------------------
-----Interrogação 3
----------------------------------------------------------
-----Atletas com um peso inferior à média de pesos dos atletas, e que participaram na modalidade 
-----de atletismo em todas as edições dos jogos olímpicos realizadas a partir de 2016, 
-----inclusivamente. O resultado deve vir ordenado pelo peso do atleta de forma ascendente e
-----pelo número do atleta de forma descendente.
----------------------------------------------------------
SELECT atleta.* --Para selecionar todas as colunas dos atletas
FROM   atleta, (SELECT atleta, Count(atleta) AS participacoes   --Subquery que devolve o numero de participaçoes que cada atleta teve em jogos desde 2016
                  FROM   participa, jogos
                 WHERE (jogos = edicao)
                   AND (ano >= 2016)
                   AND (modalidade LIKE '%Atletismo%')
                 GROUP  BY atleta)
WHERE  numero = atleta
  AND participacoes = (SELECT Count(DISTINCT edicao)  --Subquery que devolve o numero de jogos que houveram desde 2016
                         FROM   jogos
                        WHERE  (ano >= 2016))
                          AND (peso < (SELECT Avg(peso) --Calculo do peso do atleta
                                        FROM   atleta))
ORDER  BY peso ASC, numero DESC;

----------------------------------------------------------
-----Interrogação 4
----------------------------------------------------------
-----Nome do(s) país(es) com mais medalhas de ouro em cada edição dos jogos olímpicos, 
-----separadamente para atletas femininos e masculinos. O número de medalhas também deve vir
-----no resultado, assim como o país de cada edição dos jogos, e a 
-----ordenação deve ser por edição dos jogos de forma descendente, 
-----e pelo género e país dos atletas de forma ascendente.
----------------------------------------------------------
SELECT MOpais.pedicao AS Edicao_J, MOpais.genero, MOpais.pmedalhantes AS N_Medalhas, MOpais.apais AS P_1Lugar, J.pais AS P_Host
  FROM   jogos J,
       (
       SELECT Count(participa.atleta) AS Pmedalhantes, atleta.pais AS Apais, participa.jogos Pedicao, atleta.genero --Subquery que devolve o numero de medalhas que cada pais teve por genero em cada edicao
         FROM atleta LEFT JOIN participa ON (atleta.numero = participa.atleta) AND (participa.posicao = 1)
        GROUP  BY participa.jogos, atleta.pais, atleta.genero
        ORDER  BY jogos DESC) MOpais,
       (SELECT Max(pmedalhantes) AS maiorNM, pedicao, genero     --Subquery que devolve o maximo de medalhas para cada genero por edicao 
          FROM  (SELECT Count(participa.atleta) AS Pmedalhantes, atleta.pais AS Apais, participa.jogos Pedicao, atleta.genero --subquery igual ao primeiro da interrogaçao para usarmos o MAX() e obtermos o valor mais alto
               FROM   atleta LEFT JOIN participa ON atleta.numero = participa.atleta AND participa.posicao = 1
               GROUP  BY participa.jogos, atleta.pais, atleta.genero
               ORDER  BY jogos DESC)
         WHERE  (pedicao IS NOT NULL)
         GROUP  BY pedicao, genero
         ORDER  BY pedicao) MOuroEdicao
 WHERE (MOpais.pedicao = MOuroEdicao.pedicao)
   AND (MOpais.genero = MOuroEdicao.genero)
   AND (MOuroEdicao.maiornm = MOpais.pmedalhantes)
   AND (j.edicao = MOpais.pedicao)
 ORDER BY Edicao_J DESC, MOpais.genero ASC, P_1Lugar ASC;
