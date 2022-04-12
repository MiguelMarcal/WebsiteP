VARIABLE lista REFCURSOR;

BEGIN 
pkg_jogos.regista_edicao(1,2008,'China','Pequim');

pkg_jogos.regista_atleta('FAN Zhendong',1997,'M','70','China');
pkg_jogos.regista_atleta('Ma Long',1935,'M','70','China');
pkg_jogos.regista_atleta('El Primo',2000,'M','30','México');
pkg_jogos.regista_atleta('Jorge Mário',1997,'M','70','Portugal');

pkg_jogos.regista_participacao(1,1,'Ténis de Mesa',1);
pkg_jogos.regista_participacao(2,1,'Ténis de Mesa',2);
pkg_jogos.regista_participacao(3,1,'Ténis de Mesa',3);

pkg_jogos.remove_participacao(3, 1, 'Ténis de Mesa');

pkg_jogos.regista_participacao(4,1,'Ténis de Mesa',3);

pkg_jogos.remove_atleta(numero);


pkg_jogos.remove_edicao(1);

pkg_jogos.regista_edicao(2,2012,'Londres','Grã-Bretanha');


pkg_jogos.regista_edicao(2,2012,'Rio de Janeiro','Brasil');

pkg_jogos.regista_atleta('Charles Oliveira ',1997,'M',64,'USA');
pkg_jogos.regista_atleta('Dustin Poirier',1937,'M',64,'USA');
pkg_jogos.regista_atleta('Justin Gaethje',1937,'M',64,'USA');

pkg_jogos.regista_participacao(5,2,'MMA',1);
pkg_jogos.regista_participacao(6,2,'MMA',3);
pkg_jogos.regista_participacao(7,2,'MMA',2);

pkg_jogos.regista_participacao(5,2,'MMA Pluma',1);

pkg_jogos.regista_edicao(3,2016,'Rio de Janeiro','Brasil');

pkg_jogos.remove_edicao(3);
pkg_jogos.remove_edicao(4);

pkg_jogos.regista_atleta('Chica Patrícia',1997,'F',64,'USA');
pkg_jogos.regista_atleta('Wickaninnish Guálter',1937,'M',64,'USA');
pkg_jogos.regista_atleta('Núria Eduarda',1937,'F',64,'Portugal');

pkg_jogos.remove_atleta(6);
pkg_jogos.remove_atleta(5);

pkg_jogos.regista_atleta('Hiawatha Gracília',1937,'M',64,'USA');
pkg_jogos.regista_atleta('Manuel Leopoldo',1937,'F',64,'Portugal');

pkg_jogos.regista_participacao(1,3,'Ténis de Mesa',1);
pkg_jogos.regista_participacao(5,3,'Ténis de Mesa',2);
pkg_jogos.regista_participacao(6,3,'Ténis de Mesa',3);

pkg_jogos.remove_edicao(99);

pkg_jogos.regista_participacao(6,3,'MMA',3);
pkg_jogos.regista_participacao(7,3,'MMA',2);
pkg_jogos.regista_participacao(6,3,'MMA',3);

pkg_jogos.remove_participacao(6, 3, 'MMA');
pkg_jogos.remove_participacao(7, 3, 'MMA');
pkg_jogos.lista_medalhas(5);
END;

/

-- Visualização dos dados
PRINT lista;
