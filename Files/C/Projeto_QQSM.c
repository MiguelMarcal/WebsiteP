#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<time.h>


//nome dos ficheiros que vamos criar e o limite de questoes que podem haver
#define RESULTTXT "resultados.txt"
#define FICHBIN "db.bin"
#define FICHTXT "db.txt"
#define LIM 150

// numero de perguntas existentes no momento e vetores para as questeos e alineas aleatorias
int n_perguntas;
int vetor_perguntas_aleatorias[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
int alineas_random[36] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

//strcuts para o jogador e para as questoes
struct player
{
    char name[50];
    int reward;
};
struct question
{ 
    char questao[257]; 
    char respostacorreta[257];
    char resposta2[257];
    char resposta3[257];
    char resposta4[257];
    char status;
    char dificuldade[10];
    char validade;
}; 

//Inicialização das structs
struct question vetorquestao[LIM];
struct player jogador;


int tens_a_certeza()
{
    //esta função impede o user de realizar uma escolha nos menus indesejada
    char continuar;
    do{
        printf("Deseja continuar?(s/n)");
        scanf("%c", &continuar);
        while(getchar() != '\n');
        if(toupper(continuar) != 'N' && toupper(continuar) != 'S')
        {
            printf("Opcao invalida...\n");
        }
    }while(toupper(continuar) != 'N' && toupper(continuar) != 'S');

    if((int)toupper(continuar) == 83)
    {
        return 1;
    }
    else if((int)toupper(continuar) == 78)
    {
        printf("Ok...\n\n");
        return 0;
    }
}


void mostrar_questao(struct question *vetor, int n)
{
    //esta função recebe como argumento vetor de structs e imprime o elemento do vetor indicado bem como um int n que é o indice da questao
    if((vetor->status) != '*')
    {
        printf("\nQuestao nº %d", n);
        printf("\nPergunta: %s\t", vetor->questao);
        printf("Dificuldade: %s\n\n", vetor->dificuldade);
        printf("A) %s\t", vetor->respostacorreta);
        printf("B) %s\n\n", vetor->resposta2);
        printf("C) %s\t", vetor->resposta3);
        printf("D) %s\n", vetor->resposta4);
    }
    else
    {
        printf("\n\n\n-----Questao eliminada-----\n\n\n");
    }
}
void colocar_perguntas_em_ficheiro_bin(void)
{
    //função coloca as perguntas do vetor carregado em memoria e coloca-as no ficheiro bin
    int i;
    FILE *questions;

    questions = fopen (FICHBIN, "w+b");
    if (questions == NULL)
    { 
        fprintf(stderr, "\n ups, algo deu errado, tenta outra vez!\n");
        exit (1); 
    }
    for(int i = 0; i <= n_perguntas; i ++ )
    {
        fwrite(&vetorquestao[i], sizeof(struct question), 1, questions);   
    }

    if(fwrite != 0)
        printf("Ficheiro com as questoes adicionadas pronto, vamos jogar!\n");
    else
    {
        printf("que chatice, algo deu errado, tenta outra vez! Segue os passos indicados!\n");
    }

    // close file 
    fclose (questions); /* fechamos a cena*/
}
int colocar_questoes_em_vetor_lendo_do_bin(struct question *vetorquestao)
{
    //coloca as questoes em no vetor em memoria lendo-as apartir do ficheiro bin
    int i = 0;
    
    FILE *bin;
    bin = fopen(FICHBIN, "rb");

    if (bin == NULL)
    { 
        fprintf(stderr, "\nerro a abrir a questao, tenta novamente!\n"); 
        exit (1); 
    }
    for(i = 0; fread(&vetorquestao[i], sizeof(struct question), 1, bin); i++ );
    {
    }
    fclose (bin);
    return i;
}
void colocar_perguntas_em_ficheiro_txt(void)
{
    //função coloca as perguntas do vetor carregado em memoria e coloca-as no ficheiro txt
    FILE *txt = fopen("db.txt", "w+"); 
    if (txt == NULL)
    { 
        fprintf(stderr, "\nbro, falhou a cena, arranja la essa merda\n");
        exit (1);
    }
    for (int i = 0; i < n_perguntas; i++) {
        fprintf(txt, "%s\n%s\n%s\n%s\n%s\n%c\n%s\n%c", vetorquestao[i].questao, vetorquestao[i].respostacorreta, vetorquestao[i].resposta2, vetorquestao[i].resposta3, vetorquestao[i].resposta4, vetorquestao[i].status, vetorquestao[i].dificuldade, vetorquestao[i].validade);
    }
    fclose(txt); 
}
int colocar_questoes_em_vetor_lendo_do_txt(struct question *vetorquestao)
{
    //coloca as questoes em no vetor em memoria lendo-as apartir do ficheiro txt
    FILE *txt = fopen("db.txt", "r"); 
    if (txt == NULL)
    { 
        fprintf(stderr, "\nbro,  falhou a cena, arranja la essa merda\n");
        exit (1); 
    }
    int numero_da_pergunta = 0;
    while (fscanf(txt, "%[^\n] %[^\n] %[^\n] %[^\n] %[^\n] %c %[^\n] %c", vetorquestao[numero_da_pergunta].questao, vetorquestao[numero_da_pergunta].respostacorreta, vetorquestao[numero_da_pergunta].resposta2, vetorquestao[numero_da_pergunta].resposta3, vetorquestao[numero_da_pergunta].resposta4, &vetorquestao[numero_da_pergunta].status, vetorquestao[numero_da_pergunta].dificuldade, &vetorquestao[numero_da_pergunta].validade) == 8)
    {
        numero_da_pergunta ++;
    }
    fclose(txt);
    return numero_da_pergunta;
}
void repor_perguntas_originais(void)

{
    //Usada para colocar as questoes originais nos fichieros bin e txt
    int i;
    FILE *questions;
    questions = fopen (FICHBIN, "w+b");
    if (questions == NULL)
    { 
        fprintf(stderr, "\nUps, algo deu errado, tenta outra vez!\n");
        exit (1); 
    } 
    struct question questoes_originais[90] = {
        {"Qual das alternativas contem apenas nomes de rios?", "Nilo, Amazonas, Mississipi", "Sao Francisco, Douro, Antartico", "Caspio, Vermelho, Reno", "Tocantins, Bering, Ganges", 'o', "facil", 'o'},
        {"Indique quais as respectivas invencoes dos inventores Alexander Graham Bell e Galileu Galilei.", "Telefone e termometro", "Futebol e celular", "Internet e televisao", "Lampada e aviao", 'o', "facil", 'o'},
        {"Quantos bracos tem um polvo?", "Oito", "Seis", "Dez", "Sete", 'o', "facil", 'o'},
        {"Qual das alternativas contem apenas animais cujos esqueletos sao externos?", "Caracois, caranguejos e lagostas", "Besouros, peixes e formigas", "Caracois, lulas e aranhas", "Borboletas, caranguejos e peixes", 'o', "facil", 'o'},
        {"Nesses pares, ambos sao mamiferos:", "Baleia azul e golfinhos", "Morcegos e galinhas", "Girafas e tartarugas", "Porcos e pinguins", 'o', "facil", 'o'},
        {"Em qual local da Asia o portugues e lingua oficial?", "Macau", "India", "Mocambique", "Filipinas", 'o', "facil", 'o'},
        {"\"It is six twenty\" ou \"twenty past six\". Que horas sao em ingles?", "6:20", "12:06", "2:20", "6:02 ", 'o', "facil", 'o'},
        {"Quais destas construcoes famosas ficam nos Estados Unidos?", "Estatua da Liberdade, Golden Gate Bridge e Empire State Building", "Lincoln Memorial, Sidney Opera House e Burj Khalifa", "Angkor Wat, Taj Mahal e Skywalk no Grand Canyon", "Estatua da Liberdade, Big Ben e The High Line", 'o', "facil", 'o'},
        {"Qual destes paises e transcontinental?", "Russia", "Groenlandia", "Istambul", "Filipinas", 'o', "facil", 'o'},
        {"Quais os planetas do sistema solar?", "Terra, Venus, Saturno, Urano, Jupiter, Marte, Netuno, Mercurio", "Jupiter, Marte, Mercurio, Netuno, Plutao, Saturno, Sol, Terra, Urano, Venus ", "Venus, Saturno, Urano, Jupiter, Marte, Netuno, Mercurio", "Jupiter, Marte, Mercurio, Netuno, Plutao, Saturno, Terra, Urano, Venus", 'o', "facil", 'o'},
        {"Qual o maior animal terrestre?", "Elefante africano", "Tubarao Branco", "Dinossauro", "Baleia Azul", 'o', "facil", 'o'},
        {"Quais os nomes dos tres Reis Magos?", "Belchior, Gaspar e Baltazar", "Gabriel, Benjamim e Melchior", "Belchior, Gaspar e Nataniel", "Gaspar, Nicolau e Natanael", 'o', "facil", 'o'},
        {"Quais os principais heteronimos de Fernando Pessoa?", "Alberto Caeiro, Ricardo Reis e Alvaro de Campos", "Ariano Suassuna, Raul Bopp e Quincas Borba", "Bento Teixeira, Ricardo Reis e Haroldo de Campos", "Alberto Caeiro, Ricardo Leite e Augusto de Campos", 'o', "facil", 'o'},
        {"Quem foi o primeiro homem a pisar na Lua? Em que ano aconteceu?", "Neil Armstrong, em 1969", "Yuri Gagarin, em 1961", "Buzz Aldrin, em 1969", "Charles Conrad, em 1969", 'o', "facil", 'o'},
        {"Qual o nome do cientista que descobriu o processo de pasteurizacao e a vacina contra a raiva?", "Louis Pasteur", "Marie Curie", "Blaise Pascal", "Antoine Lavoisier", 'o', "facil", 'o'},
        {"As pessoas de qual tipo sanguineo sao consideradas doadores universais?", "Tipo O", "Tipo AB", "Tipo B", "Tipo A", 'o', "facil", 'o'},
        {"Quem amamentou os gemeos Romulo e Remo?", "uma loba ", "uma ovelha", "uma vaca", "uma cabra", 'o', "facil", 'o'},
        {"Quantas casas decimais tem o numero pi?", "Infinitas", "Vinte", "Centenas", "Duas", 'o', "facil", 'o'},
        {"Os sobrinhos do personagem da Disney conhecido como \"Pato Donald\" sao:", "Luizinho, Huguinho e Zezinho", "Juninho, Zezinho e Huguinho", "Huguinho, Zezinho e Paulinho", "Joaozinho, Zezinho, Huguinho e Paulinho", 'o', "facil", 'o'},
        {"Listas azuis e brancas intercaladas e um sol no canto superior esquerdo caracterizam a bandeira de qual pais da America do Sul?", "Uruguai", "Namibia", "Brasil", "Argentina", 'o', "facil", 'o'},
        {"Em que pais nasceu o Conde Dracula?", "Transilvania", "Escocia", "Irlanda", "Polonia", 'o', "facil", 'o'},
        {"Que pais foi dividido em 1949?", "Alemanha", "Austria", "Polonia", "Belgica", 'o', "facil", 'o'},
        {"Qual era o deus romano do amor?", "Cupido", "Baco", "Apolo", "Eros", 'o', "facil", 'o'},
        {"Que personagem da mitologia grega era metade homem metade cavalo?", "Centauro", "Gigante", "Gorgona", "Satiro", 'o', "facil", 'o'},
        {"Como se chama o passarinho amarelo amigo de Snoopy?", "Woodstock", "Magoo", "Daffy", "Bobby", 'o', "facil", 'o'},
        {"Em que periodo da pre-historia o fogo foi descoberto?", "Paleolitico", "Idade dos Metais", "Periodo da Pedra Polida", "Neolitico", 'o', "facil", 'o'},
        {"Qual das alternativas abaixo apenas contem classes de palavras?", "Substantivo, verbo e preposicao ", "Fonologia, Morfologia e Sintaxe", "Artigo, verbo transitivo e verbo intransitivo", "Vogais, semivogais e consoantes", 'o', "facil", 'o'},
        {"Como e a conjugacao do verbo caber na 1ª pessoa do singular do presente do indicativo?", "Eu caibo", "Eu cabo ", "Que eu caiba", "Ele cabe", 'o', "facil", 'o'},
        {"De que sao constituidos os diamantes?", "Carbono", "Feldspato", "Argila", "Grafite", 'o', "facil", 'o'},
        {"Qual dessas aves nao voa?", "Pinguim", "Galinha", "Cegonha", "Pato", 'o', "facil", 'o'},

        {"Quantas medalhas olimpicas o nadador Michael Phelps conquistou na sua carreira?", "28", "25", "21", "18", 'o', "medio", 'o'},
        {"Segundo que meridiano acertamos os nossos relogios?", "Meridiano de Greenwich", "Meridiano da Terra", "Meridiano do Equador", "Meridiano Central", 'o', "medio", 'o'},
        {"Qual desses filmes foi baseado na obra de Shakespeare?", "Muito Barulho por Nada (2012)", "A Revolucao dos Bichos (1954)", "A Dama das Camelias (1936)", "Capitaes de Areia (2011)", 'o', "medio", 'o'},
        {"Normalmente, quantos litros de sangue uma pessoa tem? Em media, quantos sao retirados numa doacao de sangue?", "Tem entre 4 a 6 litros. Sao retirados 450 mililitros", "Tem 7 litros. Sao retirados 1,5 litros", "Tem 10 litros. Sao retirados 2 litros", "Tem entre 2 a 4 litros. Sao retirados 450 mililitros", 'o', "medio", 'o'},
        {"De quem e a famosa frase \"Penso, logo existo\"?", "Descartes", "Socrates", "Galileu Galilei", "Platao", 'o', "medio", 'o'},
        {"Quais o menor e o maior pais do mundo?", "Vaticano e Russia", "Malta e Estados Unidos", "Monaco e Canada", "Nauru e China", 'o', "medio", 'o'},
        {"O que mede a Escala Mercalli?", "A intensidade sismica de acordo com os danos provocados", "A magnitude de um terremoto", "A resistencia dos minerais", "A intensidade dos ventos", 'o', "medio", 'o'},
        {"Atualmente, quantos elementos quimicos a tabela periodica possui?", "118", "108", "109", "113", 'o', "medio", 'o'},
        {"Quanto tempo a luz do Sol demora para chegar a Terra?", "8 minutos", "12 minutos", "1 dia", "12 horas", 'o', "medio", 'o'},
        {"Quem foi o anterior Primeiro Ministro ao atual?", "Pedro Passos Coelho", "Andre Ventura", "Manuela Ferreira Leite", "Paulo Portas", 'o', "medio", 'o'},
        {"As cantigas de Escarnio e Maldizer pertencem a qual estilo literaria?", "Trovadorismo", "Barroco", "Classicismo", "Modernismo", 'o', "medio", 'o'},
        {"O portugues e a lingua oficial nesses tres paises:", "Guine Equatorial, Cabo Verde e Angola", "Guine-Bissau, Africa do Sul e Brasil", "Macau, Timor-Leste e Mocambique", "Venezuela, Angola e Portugal", 'o', "medio", 'o'},
        {"Quantos dias, aproximadamente, a Lua demora para dar uma volta a Terra?", "28 dias", "31 dias", "366 dias", "365 dias", 'o', "medio", 'o'},
        {"Quem foi o criador da tragedia grega?", "Esquilo", "Plutarco", "Euripedes", "Homero", 'o', "medio", 'o'},
        {"Jim Morrison era vocalista de que grupo?", "The Doors", "The Police", "Nirvana", "Pink Floyd", 'o', "medio", 'o'},
        {"Quem pintou o teto da capela sistina?", "Michelangelo", "Leonardo da Vinci", "Rafael", "Sandro Botticelli", 'o', "medio", 'o'},
        {"Quem viveu, segundo a biblia, 969 anos?", "Matusalem", "Jesus Cristo", "Benjamim", "Noe", 'o', "medio", 'o'},
        {"Qual o metal cujo simbolo quimico e o Au?", "Ouro", "Cobre", "Prata", "Mercurio", 'o', "medio", 'o'},
        {"Que artista e conhecido como um dos expoentes maximos do Ready-Mades?", "Marcel Duchamp", "Pablo Picasso", "Van Gogh", "Salvador Dali", 'o', "medio", 'o'},
        {"Em que oceano fica Madagascar?", "Oceano Indico", "Oceano Antartico", "Oceano Atlantico", "Oceano Pacifico", 'o', "medio", 'o'},
        {"Que substancia e absorvida pelas plantas e expirada por todos os seres vivos?", "o dioxido de carbono", "o nitrato de sodio", "o nitrogenio", "o oxigenio", 'o', "medio", 'o'},
        {"Qual e o maior arquipelago da Terra?", "a Indonesia", "a Finlandia", "as Bahamas", "a Filipinas", 'o', "medio", 'o'},
        {"Que animal gruguleja?", "o peru ", "o papagaio", "a garca", "o pavao", 'o', "medio", 'o'},
        {"Com que dois paises faz fronteira o Equador?", "com a Colombia e com o Peru", "com o Peru e com o Equador", "com a Colombia e com a Venezuela", "com o Brasil e com a Colombia", 'o', "medio", 'o'},
        {"Como se chamam os vasos que transportam sangue do coracao para a periferia do corpo?", "arterias ", "ventriculos", "atrios", "veias", 'o', "medio", 'o'},
        {"No exterior de que famoso edificio frances foi construida uma enorme piramide de vidro em 1989?", "Museu do Louvre", "Grand Palais", "Petit Palais", "Torre Eiffel", 'o', "medio", 'o'},
        {"Que matematico e geografo grego calculou o perimetro da Terra em 240 a.C.?", "Eratostenes", "Socrates", "Sofocles", "Descartes", 'o', "medio", 'o'},
        {"Segundo o tratado assinado em 1971, que regiao do planeta ficou interdita a armas nucleares?", "Antartida", "Bahamas", "Africa", "Sara", 'o', "medio", 'o'},
        {"Qual o unico clube portugues campeao mundial?", "FC Porto", "Boavista ", "Sporting", "Benfica", 'o', "medio", 'o'},
        {"Em Portugal, a que cidadaos foi retirado o direito de voto em 1913?", "Analfabetos", "Emigrantes", "Idosos", "Mulheres", 'o', "medio", 'o'},

          {"A quem foi atribuido pela primeira vez o Premio Nobel da Paz?", "Frederic Passy e Henry Dunant", "Henri Becquerel e Marie Curie", "Martin Luther King Jr.", "Frederic Passy", 'o', "dificil", 'o'},
        {"O etanol e produzido atraves de qual fonte de energia?", "Biomassa", "Solar", "Geotermica", "Eolica", 'o', "dificil", 'o'},
        {"Qual destas, apesar do seu nome, nao e considerada um tipo de forca?", "Forca eletromotriz", "Forca centripeta", "Forca peso", "Forca de atrito", 'o', "dificil", 'o'},
        {"Em que cidade ocorreu a Eco-92, a Conferencia das Nacoes Unidas sobre ambiente e desenvolvimento?", "Rio de Janeiro", "Assuncao", "Montevideu", "Buenos Aires", 'o', "dificil", 'o'},
        {"Que expressao alema significa \"guerra-relampago\"?", "Blitzkrieg", "Reichstag", "Glasnost", "Sauerkraut", 'o', "dificil", 'o'},
        {"Junto a que cidade do Novo Mexico se julga ter caido um OVNI em 1947?", "Roswell", "Chihuahua", "Las Vegas", "Tijuana", 'o', "dificil", 'o'},
        {"Qual e o mais famoso detetive criado por Raymond Chandler?", "Philip Marlowe", "Agatha Christie", "Sherlock Holmes", "Hercule Poirot", 'o', "dificil", 'o'},
        {"Em que atividade se destacou Le Corbusier?", "Arquitetura", "Literatura", "Teatro", "Cinema", 'o', "dificil", 'o'},
        {"Quem recebeu um premio Nobel devido ao desenvolvimento da encefalografia arterial?", "Egas Moniz", "Sousa Martins", "Michael Faraday", "Julio de Matos", 'o', "dificil", 'o'},
        {"Que parque natural na regiao de Braganca foi criado em 1979?", "Montesinho", "Arrabida", "Alto Douro", "Peneda-Geres", 'o', "dificil", 'o'},
        {"Como se chama a personagem principal do filme Touro Enraivecido", "Jake La Motta ", "Muhamad Ali", "Belarmino", "Sugar Ray Robinson", 'o', "dificil", 'o'},
        {"Qual a classe literaria ganhou o primeiro Premio Nobel de Literatura?", "Poesia", "Ensaio", "Dramaturgia", "Romance", 'o', "dificil", 'o'},
        {"Quantos mares banham a Peninsula Balcanica?", "6", "5", "3", "4", 'o', "dificil", 'o'},
        {"Que eletrodomestico foi criado por Murray Spangler em 1907 e que ainda hoje e vendido praticamente inalterado?", "Aspirador", "Micro-ondas", "Torradeira", "Esquentador", 'o', "dificil", 'o'},
        {"Que modalidade desportiva foi introduzida em Portugal Continental em 1888 por Guilherme Pinto Basto?", "Futebol", "Tenis", "Andebol", "Corfebol", 'o', "dificil", 'o'},
        {"Qual e o objeto de estudo do puericultor?", "Criancas", "Coelhos", "Abelhas", "Galinhas", 'o', "dificil", 'o'},
        {"Que pais sul-americano e o maior produtor de vinho?", "Argentina", "Brasil", "Venezuela", "Canada", 'o', "dificil", 'o'},
        {"Quem comandou a segunda invasao napoleonica em Portugal?", "Soult", "Wellington", "Junot", "Napoleao", 'o', "dificil", 'o'},
        {"Qual e a corrente filosofica defendida por Martin Heidegger?", "Existencialismo", "Positivismo", "Abstracionismo", "Niilismo", 'o', "dificil", 'o'},
        {"Que corpo militar de elite foi criado em Franca por Luis XIII como guarda pessoal no seculo XVII existiu ate 1815?", "Mosqueteiros", "Escuteiros", "Rangers", "Guarda Suica", 'o', "dificil", 'o'},
        {"O que mudou na personagem Lucky Luke em 1983?", "Deixou de fumar", "Deixou de usar armas", "Tornou-se vegetariano", "Comecou a usar oculos", 'o', "dificil", 'o'},
        {"Qual foi a primeira moeda a ser usada em toda a Europa Ocidental?", "Dracma", "Sestercio", "Euro", "ECU", 'o', "dificil", 'o'},
        {"Quem escreveu A Arte da Guerra?", "Sun Tzu", "Gandhi", "Confucio", "Mao Tse Tung", 'o', "dificil", 'o'},
        {"Que expressao nautica foi sugerida por Alexander Bell para ser usada quando se atende o telefone?", "Ahoy", "All aboard", "Hi", "OveZr", 'o', "dificil", 'o'},
        {"Que palavra de origem inuite significa casa?", "Iglu", "Quiosque", "Caiaque", "Anoraque", 'o', "dificil", 'o'},
        {"Qual e a capital da Africa do Sul?", "Cidade do Cabo", "Johanesburgo", "Knysna", "Durban", 'o', "dificil", 'o'},
        {"Qual e a capital do Canada?", "Ottawa", "Toront", "Quebec", "Montreal", 'o', "dificil", 'o'},
        {"Ate 1923, como se chamava a cidade turca de Istambul?", "Constantinopla", "Inca ", "Ancara", "Atenas", 'o', "dificil", 'o'},
        {"Qual destes nao foi Presidente da Republica Portuguesa?", "Mario Centeno", "Manuel de Arriaga", "Sidonio Pai", "Teofilo Braga", 'o', "dificil", 'o'},
        {"O muro  de Berlim caiu em que ano?", "1989", "1884", "1991 ", "1985", 'o', "dificil", 'o'},
};
    
    for(int i = 0; i < 90; i ++ )
    {
        fwrite (&questoes_originais[i], sizeof(struct question), 1, questions);
    }

    if(fwrite != 0)
    {

    }
    else 
        printf("Que chatice, algo deu errado, tenta outra vez!\n");

    fclose (questions); /* fechamos a cena*/

    FILE *txt = fopen(FICHTXT, "w+"); 
    if (txt == NULL)
    {
        fprintf(stderr, "\nbro, falhou a cena, arranja la essa merda\n");
        exit (1); 
    }
    for(int i = 0; i < 90; i++) 
    {
        fprintf(txt, "%s\n%s\n%s\n%s\n%s\n%c\n%s\n%c", questoes_originais[i].questao, questoes_originais[i].respostacorreta, questoes_originais[i].resposta2, questoes_originais[i].resposta3, questoes_originais[i].resposta4, questoes_originais[i].status, questoes_originais[i].dificuldade, questoes_originais[i].validade);
    }
    fclose(txt); 
}
int listar_questoes(void)
{
    //usada para mostrar as questoes todas de 5 de cada vez
    int i = 1, u = 0, n = 0;
    char ch;

    do
    {
        for(u; u < i * 5; u++)
        {
            if(u < n_perguntas)
            {
                printf("\n--------\n    ___|\n    |______\n          V");
                printf("\n............................................................................................................................\n                                                                                                                           |");
                mostrar_questao(&vetorquestao[u], n);
                printf("                                                                                                                           |\n............................................................................................................................\n");

                n++;
            }
            else
            {
                printf("Chegou ao final do ficheiro!\n\n");
                return 0;
            }
        }

        do{
            printf("\nPara ver mais prima <ENTER> para sair insira \"q\"\n");
            scanf("%c", &ch);
            if(ch != 'q' && ch != '\n')
            {
                printf("Opcao invalida, tenta novamente por favor...\n");
                while(getchar() != '\n');
            }
        }while(ch != 'q' && ch != '\n');
        i ++;

    }while(ch != 'q');

    while(getchar() != '\n');
}
int funcao_para_procurar_questoes()
{
    //procuramos questoes, de acordo com uma palavra chave, na qual  o uso de maiusculas em nada interfere e mostramos 5 a 5 
    printf("Agora insira a sequencia de letras para localizar a pergunta!\n(limite de 20 caracteres):\n");
    char agulha [20];
    char needle [20];
    char haystack [257];
    char haystackquestao1 [257];
    char haystackquestao2 [257];
    char haystackquestao3 [257];
    char haystackquestao4 [257];
    int u;

    int cinco_so_de_cada_vez = 0;
    char ch;

    fgets(agulha, 20, stdin);
    char *pos = strchr(agulha, '\n');
    *pos = '\0';
    for(int i = 0; i < 20; i ++)
    {
        needle [i] = toupper(agulha[i]);
    }
        for(int i = 0; i < n_perguntas; i++)
        {
            for(u = 0; u < 257; u ++)
            {
                haystack[u] = toupper(vetorquestao[i].questao[u]);
            }
            u = 0;            
            for(u = 0; u < 257; u ++)
            {
                haystackquestao1[u] = toupper(vetorquestao[i].respostacorreta[u]);
            }
            u = 0;            
            for(u = 0; u < 257; u ++)
            {
                haystackquestao2[u] = toupper(vetorquestao[i].resposta2[u]);
            }
            u = 0;            
            for(u = 0; u < 257; u ++)
            {
                haystackquestao3[u] = toupper(vetorquestao[i].resposta3[u]);
            }
            u = 0;            
            for(u = 0; u < 257; u ++)
            {
                haystackquestao4[u] = toupper(vetorquestao[i].resposta4[u]);
            }
            u = 0;
            
            if(strstr(haystack, needle) != NULL || strstr(haystackquestao1, needle) != NULL || strstr(haystackquestao2, needle) != NULL || strstr(haystackquestao3, needle) != NULL || strstr(haystackquestao4, needle) != NULL)
            {
                if(cinco_so_de_cada_vez % 5 == 0 && cinco_so_de_cada_vez != 0)
                {
                    do{
                        printf("Insira <ENTER> para ver mais e q para sair\n");
                        scanf("%c", &ch);
                        printf("\n\n");
                        if(ch != '\n')
                        {
                            while(getchar() != '\n');
                        }
                        if(toupper(ch) != 'Q' && ch != '\n')
                        {
                            printf("Opção invalida...\n");
                        }
                        if(toupper(ch) == 'Q')
                        {
                            printf("Espero que tenha obtido aquilo que procura!\n");
                            return 0;
                        }
                    }while(toupper(ch) != 'Q' && ch != '\n');
                }
                printf("\n\n\n..............................................................................................\n");
                printf("-----------\n          |\n          V\n");
                printf("Visto em: %s\n", strstr(haystack, needle));

                mostrar_questao(&vetorquestao[i], i);
                printf("..........................................\\    \\..............................................\n\n");
                cinco_so_de_cada_vez ++;
            }
        }

    printf("Todas as funções com a palavra escolhida imprimidas!\n\n");
    return 0;
}
int adicionar_questoes(int *n_faceis, int *n_normais, int *n_dificeis)
{
    //função para adicionar questoes, tendo em conta se estas foram eliminadas pelo user e tambem os limites das perguntas
    int i;
    while(n_perguntas < LIM)
    {
        for(i = 0; i < n_perguntas; i ++)
        {
            if(vetorquestao[i].status == '*')
            {
                char dificuldade[10];
                do
                {
                    printf("Insira qual a dificuldade(\"facil\" \"dificil\" \"medio\")\n");
                    scanf("%s", vetorquestao[i].dificuldade);
                    while(getchar() != '\n');
                                        
                    if((strcmp(vetorquestao[i].dificuldade, "facil") != 0) && *n_faceis == 50)
                    {
                        printf("Numero maximo de perguntas faceis (50) atingido! Vamos jogar!\n\n");
                        return 0;
                    }            
                    if((strcmp(vetorquestao[i].dificuldade, "medio") != 0) && *n_normais == 50)
                    {
                        printf("Numero maximo de perguntas normais (50) atingido! Vamos lá jogar!\n\n");
                        return 0;
                    }            
                    if((strcmp(vetorquestao[i].dificuldade, "dificil") != 0) && *n_dificeis == 50)
                    {
                        printf("Numero maximo de perguntas dificeis (50) atingido! Vamos lá ganhar uns trocos!\n\n");
                        return 0;
                    }
                    if((strcmp(vetorquestao[i].dificuldade, "facil") != 0)&& (strcmp(vetorquestao[i].dificuldade, "medio") != 0) && (strcmp(vetorquestao[i].dificuldade, "dificil") != 0))
                    {
                        printf("Dificuldade Invalida, tenta mais uma vez. Somos chatos mas no fim pelo dinheiro vale a pena!");
                    }
                }while((strcmp(vetorquestao[i].dificuldade, "facil") != 0)&& (strcmp(vetorquestao[i].dificuldade, "medio") != 0) && (strcmp(vetorquestao[i].dificuldade, "dificil") != 0));


                printf("insira qual e a questao(256 caracteres max): ");
                fgets(vetorquestao[i].questao, 257 , stdin);
                char *pos = strchr(vetorquestao[i].questao, '\n');
                *pos = '\0';

                printf("\n\nNOTA: Apenas 256 caracteres permitidos por resposta! Nao escrevas um testamento!\n\n");

                printf("Insira a resposta Correta: ");
                fgets(vetorquestao[i].respostacorreta, 257 , stdin);
                char *pos1 = strchr(vetorquestao[i].respostacorreta, '\n');
                *pos1 = '\0';

                printf("Insira Uma das repostas Incorretas: ");
                fgets(vetorquestao[i].resposta2, 257 , stdin);
                char *pos2 = strchr(vetorquestao[i].resposta2, '\n');
                *pos2 = '\0';

                printf("Insira Uma das repostas Incorretas: ");
                fgets(vetorquestao[i].resposta3, 257 , stdin);
                char *pos3 = strchr(vetorquestao[i].resposta3, '\n');
                *pos3 = '\0';

                printf("Insira Uma das repostas Incorretas: ");
                fgets(vetorquestao[i].resposta4, 257 , stdin);
                char *pos4 = strchr(vetorquestao[i].resposta4, '\n');
                *pos4 = '\0';

                vetorquestao[i].status = 'o';

                if((vetorquestao[i].status == 'o') && (strcmp(vetorquestao[i].dificuldade, "facil") == 0))
                {
                    *n_faceis = *n_faceis + 1;
                }        
                else if((vetorquestao[i].status == 'o') && (strcmp(vetorquestao[i].dificuldade, "medio") == 0))
                {
                    *n_normais = *n_normais + 1;
                }        
                else if((vetorquestao[i].status == 'o') && (strcmp(vetorquestao[i].dificuldade, "dificil") == 0))
                {
                    *n_dificeis = *n_dificeis + 1;
                }

                return 0;
            }
        }   

        char dificuldade[10];
        do
        {
            printf("Insira qual a dificuldade!(\"facil\" \"dificil\" \"medio')\n");
            scanf("%s", vetorquestao[n_perguntas].dificuldade);
            while(getchar() != '\n');

            if((strcmp(vetorquestao[n_perguntas].dificuldade, "facil") != 0) && *n_faceis == 50)
            {
                printf("Numero maximo de perguntas faceis (50) atingido! Siga jogar!\n\n");
                return 0;
            }            
            if((strcmp(vetorquestao[n_perguntas].dificuldade, "medio") != 0) && *n_normais == 50)
            {
                printf("Numero maximo de perguntas normais (50) atingido! Vamos lá jogar!\n\n");
                return 0;
            }            
            if((strcmp(vetorquestao[n_perguntas].dificuldade, "dificil") != 0) && *n_dificeis == 50)
            {
                printf("Numero maximo de perguntas dificeis (50) atingido! Vamos lá ganhar dinheiro!\n\n");
                return 0;
            }


            if((strcmp(vetorquestao[n_perguntas].dificuldade, "facil") != 0)&& (strcmp(vetorquestao[n_perguntas].dificuldade, "medio") != 0) && (strcmp(vetorquestao[n_perguntas].dificuldade, "dificil") != 0))
            {
                printf("Dificuldade Invalida, tenta mais uma vez. Somos chatos mas no fim pelo dinheiro vale a pena!");
            }

        }while((strcmp(vetorquestao[n_perguntas].dificuldade, "facil") != 0)&& (strcmp(vetorquestao[n_perguntas].dificuldade, "medio") != 0) && (strcmp(vetorquestao[n_perguntas].dificuldade, "dificil") != 0));

        printf("insira qual e a questao por favor!(256 caracteres max): ");
        fgets(vetorquestao[n_perguntas].questao, 257 , stdin);
        char *pos = strchr(vetorquestao[n_perguntas].questao, '\n');
        *pos = '\0';

        printf("\n\nNOTA: Apenas 256 caracteres permitidos por resposta\n\n");

        printf("Insira a resposta Correta: ");
        fgets(vetorquestao[n_perguntas].respostacorreta, 257 , stdin);
        char *pos1 = strchr(vetorquestao[n_perguntas].respostacorreta, '\n');
        *pos1 = '\0';

        printf("Insira Uma das repostas Incorretas: ");
        fgets(vetorquestao[n_perguntas].resposta2, 257 , stdin);
        char *pos2 = strchr(vetorquestao[n_perguntas].resposta2, '\n');
        *pos2 = '\0';

        printf("Insira Uma das repostas Incorretas: ");
        fgets(vetorquestao[n_perguntas].resposta3, 257 , stdin);
        char *pos3 = strchr(vetorquestao[n_perguntas].resposta3, '\n');
        *pos3 = '\0';

        printf("Insira Uma das repostas Incorretas: ");
        fgets(vetorquestao[n_perguntas].resposta4, 257 , stdin);
        char *pos4 = strchr(vetorquestao[n_perguntas].resposta4, '\n');
        *pos4 = '\0';

        vetorquestao[n_perguntas].status = 'o';
        vetorquestao[n_perguntas].validade = 'o';

        if((vetorquestao[n_perguntas].status == 'o') && (strcmp(vetorquestao[n_perguntas].dificuldade, "facil") == 0))
        {
            *n_faceis = *n_faceis + 1;
        }        
        else if((vetorquestao[n_perguntas].status == 'o') && (strcmp(vetorquestao[n_perguntas].dificuldade, "medio") == 0))
        {
            *n_normais = *n_normais + 1;
        }        
        else if((vetorquestao[n_perguntas].status == 'o') && (strcmp(vetorquestao[n_perguntas].dificuldade, "dificil") == 0))
        {
            *n_dificeis = *n_dificeis + 1;
        }

        n_perguntas ++;
        return 0;
    } 
    printf("Algo de errado aconteceu, talvez tenha sido alcançado o numero maximo de questoes(150)!!\n\n");
}
int remover_questoes(int *n_faceis, int *n_normais, int *n_dificeis)
{
    //função para "remover" questoes
    long int questao_eliminar;
    char resp;
    do{
    printf("Qual o numero da questao que deseja remover?\n");
    scanf("%ld", &questao_eliminar);while(getchar() != '\n');
    if(questao_eliminar > n_perguntas || questao_eliminar < 0)
    {
        printf("Número de questao invalido, tenta mais uma vez...\n");
    }

    }while(questao_eliminar > n_perguntas || questao_eliminar < 0);
    if(vetorquestao[questao_eliminar].status == '*')
    {
        printf("Questao ja apagada...\n");
        return 0;
    }

    printf("Esta e a questao que escolheu apagar!\n");
    mostrar_questao(&vetorquestao[questao_eliminar], questao_eliminar);
    if((vetorquestao[questao_eliminar].status == 'o') && (strcmp(vetorquestao[questao_eliminar].dificuldade, "facil") == 0) && *n_faceis < 5)
    {
        printf("Limite de questoes faceis removidas atingido!\n\n");
        return 0;
    }        
    else if((vetorquestao[questao_eliminar].status == 'o') && (strcmp(vetorquestao[questao_eliminar].dificuldade, "medio") == 0)&& *n_normais < 5)
    {
        printf("Limite de questoes medias removidas atingido!\n\n");
        return 0;
    }        
    else if((vetorquestao[questao_eliminar].status == 'o') && (strcmp(vetorquestao[questao_eliminar].dificuldade, "dificil") == 0)&& *n_dificeis < 5)
    {
        printf("Limite de questoes dificeis removidas atingido!\n\n");
        return 0;
    }


    printf("\n\nDeseja apagar a questao acima escolhida?(s/n)\n");
    resp = getchar(); while(getchar() != '\n');
    if(toupper(resp) != 'S')
    {
        printf("Questao %ld mantida!\n\n", questao_eliminar);
        return 0;
    }

    vetorquestao[questao_eliminar].status = '*';
/*
    fseek(fpa, -(long) sizeof(struct question), SEEK_CUR);
    fwrite(&vetorquestao[questao_eliminar -1], sizeof(struct question), 1, fpa);
    fclose(fpa);
    */
    printf("Questao removida do Jogo...\n\n");
    if(strcmp(vetorquestao[questao_eliminar].dificuldade, "dificil") == 0)
    {
        *n_dificeis = *n_dificeis - 1;
    }    
    if(strcmp(vetorquestao[questao_eliminar].dificuldade, "medio") == 0)
    {
        *n_normais = *n_normais - 1;
    }   
    if(strcmp(vetorquestao[questao_eliminar].dificuldade, "facil") == 0)
    {
        *n_faceis = *n_faceis - 1;
    }
}
void criar_files_para_questoes(void)
{
    //função para criar os filhes bin e txt
    FILE *binario;
    binario = fopen(FICHBIN, "w+b");
    if (binario == NULL)
    {
        fprintf(stderr, "ERRO FATAL: Impossivel Criar o Ficheiro de Dados Binario\n");
        exit(1);
    }
    fclose(binario);

    FILE *txt;
    txt = fopen(FICHTXT, "w+");

    if (txt == NULL)
    {
        fprintf(stderr, "ERRO FATAL: Impossivel Criar o Ficheiro de Dados .txt\n");
        exit(1);
    }
    fclose(txt);
    printf("Files criados\t");
}
int reiniciar_files(char tipo_de_file)
{
    //função que dá uso de algumas acima defenidas para a qualquer ponto colocar os ficheiros de prerguntas iguais ao que eram no inicio
    char remover;
    printf("Isto ira criar db.txt e db.bin novos com apenas as questoes originais\nsendo assim perdida qualquer alteracao ao ficheiro...\n");
    do{
        printf("Deseja repor os dados iniciais dos ficheiro?(s/n)\n");
        scanf("%c", &remover);
        while(getchar() != '\n');
        if(toupper(remover) != 'N' && toupper(remover) != 'S')
        {
            printf("Opcao invalida...\n");
        }
    }while(toupper(remover) != 'N' && toupper(remover) != 'S');

    if(toupper(remover) != 'S')
    {
        return 0;;
    }
    else
    {
        criar_files_para_questoes();
        printf("Files originais repostos!\n");
        repor_perguntas_originais();
        printf("Questoes originais repostas!\n");

        if((int)toupper(tipo_de_file) == 66)
        {
        colocar_questoes_em_vetor_lendo_do_bin(vetorquestao);
        }        
        
        if((int)toupper(tipo_de_file) == 84)
        {
        colocar_questoes_em_vetor_lendo_do_txt(vetorquestao);
        }

        printf("Vetor Questoes reposto!\n");
        n_perguntas = 90;
    }
}


int criar_resultados_txt()
{
    //cria o ficheiro resultados.txt
    FILE *resultados;
    resultados = fopen(RESULTTXT, "w");
    if(resultados == NULL)
    {
        fprintf(stderr, "ERRO FATAL: Impossivel criar o ficheiro para os resultados...\n");
        exit(1);
    }
    fclose(resultados);
}
int reiniciar_files_resulstados_txt()
{
    //Cria o ficheiro resultados.txt denovo limpando assim o "historico" dos resultados obtidos
    char remover;
    printf("Isto ira criar resultados.txt novos sendo assim perdida qualquer Jogador e respetivo prêmio guardados até este momento...\n");
    do{
        printf("Deseja repor o ficheiro resultados.txt?(s/n)\n");
        scanf("%c", &remover);
        while(getchar() != '\n');
        if(toupper(remover) != 'N' && toupper(remover) != 'S')
        {
            printf("Opcao invalida...\n");
        }
    }while(toupper(remover) != 'N' && toupper(remover) != 'S');

    if(toupper(remover) != 'S')
    {
        return 0;;
    }
    else
    {   
        criar_resultados_txt();
        printf("Ficheiro resultados.txt limpo!\n\n");
    }
}
int colocar_jogador_em_resultados_txt()
{
    //imprime os resultados obtidos n ficheiro resultados.txt
    FILE *resultados;
    resultados = fopen(RESULTTXT, "a+");
    if (resultados == NULL)
    { 
        fprintf(stderr, "\nAlgum erro sucedeu, repita o processo por-favor!\n");
        exit (1);
    }
    fprintf(resultados, "Nome: %s\nQuantia: %d€\n", jogador.name, jogador.reward);
    fclose(resultados);
}
int ler_resultados_de_resultados_txt()
{
    //Mostra o conteudo do ficheiro resultados.txt ao user
    FILE *lerresultados;
    char ch;
    lerresultados = fopen(RESULTTXT, "r");
    if(lerresultados == NULL)
    {
        printf("Erro a abrir o ficheiro resultados.txt");
    }
    while((ch = fgetc(lerresultados))!= EOF )
    {
        putchar(ch);
    }
    printf("\n\nFinal do Ficheiro alcancado!\n\n");
    fclose(lerresultados);
}
int gerar_vetores_perguntas_random_por_dificuldade()    
{
    //associa numeros pseudoaleatorios, seguindo regras aos vetores usados para durante o jogo as perguntas e as possiveis respostas serem aleatorias
    int n_random;
    srand(time(NULL));
    int n_maximo;
    int nqf, nqd, nqn;
    int i = 0, o = 0;
    n_maximo = n_perguntas + 1;

    for(int i = 0, nqf = 0, nqn = 3, nqd = 6; nqf < 3 || nqn < 6 || nqd < 9; i++)
    {
        n_random = (rand() % n_maximo); 
        if(vetorquestao[n_random].status == 'o' && (strcmp(vetorquestao[n_random].dificuldade, "facil") == 0) && vetorquestao[n_random].validade == 'o'  && nqf < 3 && n_random != vetor_perguntas_aleatorias[0] && n_random != vetor_perguntas_aleatorias[1] && n_random != vetor_perguntas_aleatorias[2])
        {
            vetor_perguntas_aleatorias[nqf] = n_random;
            nqf ++;
        }

        if(vetorquestao[n_random].status == 'o' && (strcmp(vetorquestao[n_random].dificuldade, "dificil") == 0) && vetorquestao[n_random].validade == 'o'  && nqd < 9 && n_random != vetor_perguntas_aleatorias[6] && n_random != vetor_perguntas_aleatorias[7] && n_random != vetor_perguntas_aleatorias[8])
        {
            vetor_perguntas_aleatorias[nqd] = n_random;
            nqd ++;
        }        
        
        
        if(vetorquestao[n_random].status == 'o' && (strcmp(vetorquestao[n_random].dificuldade, "medio") == 0) && vetorquestao[n_random].validade == 'o'  && nqn < 6 && n_random != vetor_perguntas_aleatorias[3] && n_random != vetor_perguntas_aleatorias[4] && n_random != vetor_perguntas_aleatorias[5])
        {
            vetor_perguntas_aleatorias[nqn] = n_random;
            nqn ++;
        }

    }
    int alineas_random_generator;

    for(int i = 0; i < 36; )
    {
        for(int o = 0; o < 4; )
        {
            alineas_random_generator = (rand() % 4) + 1;

            if(o == 0)
            {
            alineas_random[i] = alineas_random_generator;
            i ++;
            o ++;
            }
            if(o == 1)
            {
                if(alineas_random_generator != alineas_random[i - o])
                {
                    alineas_random[i] = alineas_random_generator;
                    o ++;
                    i ++;
                }
            }
            if(o == 2)
            {
                if(alineas_random_generator != alineas_random[i - o] && alineas_random_generator != alineas_random[i + 1 - o])
                {
                    alineas_random[i] = alineas_random_generator;
                    o ++;
                    i ++;
                }
            }
            if(o == 3)
            {
                if(alineas_random_generator != alineas_random[i - o] && alineas_random_generator != alineas_random[i + 1 - o] && alineas_random_generator != alineas_random[i + 2 - o])
                {
                    alineas_random[i] = alineas_random_generator;
                    o ++;
                    i ++;
                }
            }
        }
    }
    printf("\n");    
    return 0;
}
int mostrar_perguntas_e_validar_respostas(int numero_da_pergunta, int valor_acumulado)
{
    //imprime as perguntas e respostas durante o jogo e valida a reposta do user
    char resposta;
    printf("Dificuldade - %s \n\tValor Acumulado %d€\n", vetorquestao[vetor_perguntas_aleatorias[numero_da_pergunta]].dificuldade, valor_acumulado);
    printf("%s\n\n",vetorquestao[vetor_perguntas_aleatorias[numero_da_pergunta]].questao);
    for(int i = 1; i < 5; i ++)
    {
        if(i == 1)
        {
            printf("A.");
        }
        if(i == 2)
        {
            printf("B.");
        }
        if(i == 3)
        {
            printf("\n");
            printf("C.");
        }
        if(i == 4)
        {
            printf("D.");
        }
        if(i == 5)
        {
            printf("erro! Try again!");
        }
        
        else if(i == alineas_random[4 * numero_da_pergunta])
        {
            printf("%s\t\t\t\t",vetorquestao[vetor_perguntas_aleatorias[numero_da_pergunta]].respostacorreta );
        }
        else if(i == alineas_random[4 * numero_da_pergunta + 1])
        {
            printf("%s\t\t\t\t", vetorquestao[vetor_perguntas_aleatorias[numero_da_pergunta]].resposta2);
        }
        else if(i == alineas_random[4 * numero_da_pergunta + 2])
        {
            printf("%s\t\t\t\t", vetorquestao[vetor_perguntas_aleatorias[numero_da_pergunta]].resposta3 );
        }
        else if(i == alineas_random[4 * numero_da_pergunta + 3])
        {
            printf("%s\t\t\t\t", vetorquestao[vetor_perguntas_aleatorias[numero_da_pergunta]].resposta4);
        }
    }
    printf("\n\n");
    printf("\nResposta:\t");
    scanf("%c", &resposta);
    while(getchar() != '\n');
    switch(((int)toupper(resposta)))
    {
        case 88:
            return 2;
            break;
        case 65:
            if(alineas_random[numero_da_pergunta * 4] == 1)
            {
                printf("---------------------------------------------------------------------CORRETO!! PARABENS!!---------------------------------------------------------------------\n\n\n");
                return 1;
            }
            else
            {
                printf("---------------------------------------------------------------------Resposta Errada---------------------------------------------------------------------\n\n");
                printf("A resposta correta era: %s", vetorquestao[vetor_perguntas_aleatorias[numero_da_pergunta]].respostacorreta);
                return 0;
            }
            break;

        case 66:
            if(alineas_random[numero_da_pergunta * 4] == 2)
            {
                printf("---------------------------------------------------------------------E a resposta esta... CERTA!!!---------------------------------------------------------------------\n\n\n");
                return 1;
            }
            else
            {
                printf("---------------------------------------------------------------------Resposta Errada---------------------------------------------------------------------\n\n");
                printf("A resposta correta era: %s", vetorquestao[vetor_perguntas_aleatorias[numero_da_pergunta]].respostacorreta);
                return 0;
            }
            break;

        case 67:
            if(alineas_random[numero_da_pergunta * 4] == 3)
            {
                printf("---------------------------------------------------------------------CORRETO!! MUITO BEM!!---------------------------------------------------------------------\n\n\n");
                return 1;
            }
                else
            {
                printf("---------------------------------------------------------------------Resposta Errada---------------------------------------------------------------------\n\n");
                printf("A resposta correta era: %s", vetorquestao[vetor_perguntas_aleatorias[numero_da_pergunta]].respostacorreta);
                return 0;
            }
            break;

        case 68:
            if(alineas_random[numero_da_pergunta * 4] == 4)
            {
                printf("---------------------------------------------------------------------ESTA CERTISSIMO!!---------------------------------------------------------------------\n\n\n");
                return 1;
            }
            else
            {
                printf("---------------------------------------------------------------------Resposta Errada---------------------------------------------------------------------\n\n");
                printf("A resposta correta era: %s", vetorquestao[vetor_perguntas_aleatorias[numero_da_pergunta]].respostacorreta);
                return 0;
            }
            break;

        default:
            printf("Resposta Invalda, sera desqualificado por isso...\n");
            printf("A resposta correta era: %s", vetorquestao[vetor_perguntas_aleatorias[numero_da_pergunta]].respostacorreta);
            
            return 0;
    }

}
int perguntas_e_respostas(char *repetir) 
{
    //dá uso da função acima definida para fazer o jogo acontecer bem como atualiza a struct jogador
    printf("Vamos agora dar inicio a um Novo Concurso!\n");
    int valor_acumulado = 0;
    int valor_ganho = 0;
    char esperar;
    int i;
    printf("Insira o seu nome:\n");
    fgets(jogador.name, 50, stdin);
    char *pos = strchr(jogador.name, '\n');
    *pos = '\0';
    printf("\n\n\nEntao vamos la %s, Boa sorte para o Concurso!! Siga ganhar uma pipa de massa!!\n", jogador.name);
    printf("Nao se esqueca de ler as perguntas e as respostas com atencao e responder na melhor das suas capacidades, tendo em conta que podera desistir, mantendo o valor ganho até a pergunta onde desistiu\n respondendo a pergunta com um 'x' e tambem que para cada tres perguntas correta, um patamar é alcançado, sendo que se errar uma pergunta, o dinheiro obtido até o patamar e-lhe garantido\nainda, cuidado com as respostas, certifique-se de que responde apenas 'a', 'b', 'c' ou 'd' pois quaisquer respostas diferentes e será desqualificado, ganhando apenas o valor que é seu!\nPassando a frente, o concurso é composto por nove questos, tres para cada dificuldade (facil, medio, dificil), cada questão está associada a quatro respostas, a, b, c, d sendo apenas uma considerada correta\nao longo do jogo será lhe mostrado um resumo de o que ja ganhou e o que pode ganhar!\n\n\n");
    printf("\n\nPrima <ENTER> ou insira qualquer letra para continuar...\n\n\n\n\n");
    do{
        scanf("%c", &esperar);
        if(esperar != '\n')
        {
            while(getchar() != '\n');
        }
    }while(esperar == '\n' && esperar != '\n');

    printf("Primeira questao:   Vale 100€ em caso de acertar!\n\n");
    switch(mostrar_perguntas_e_validar_respostas(0, valor_acumulado))
    {
    case 2:
        jogador.reward = 0;
        printf("Muito bem '%s', alcançou a quantia de '%d€'!!\n\n", jogador.name, jogador.reward);
        return 0;

    case 1:
        valor_acumulado += 100 ;
        printf("\nJa adquiriu o valor de '%d?'!\n\n", valor_acumulado);
        break;
    
    default:
        return 0;
    }


    printf("Segunda questao:   Vale 300€ em caso de acertar!\n\n");
    switch(mostrar_perguntas_e_validar_respostas(1, valor_acumulado))
    {
    case 2:
        jogador.reward = valor_acumulado;
        printf("Muito bem '%s', alcançou a quantia de '%d€'!\n\n", jogador.name, jogador.reward);
        return 0;

    case 1:
        valor_acumulado += 300 ;
        printf("\nJa adquiriu o valor de '%d?'!\n\n", valor_acumulado);
        break;
    
    default:
        return 0;
    }


    printf("Terceira questao:   Vale 500€ em caso de acertar!\n\n");
    switch(mostrar_perguntas_e_validar_respostas(2, valor_acumulado))
    {
    case 2:
        jogador.reward = valor_acumulado;
        printf("Muito bem '%s', alcançou a quantia de '%d€'!\n\n", jogador.name, jogador.reward);
        return 0;

    case 1:
        valor_acumulado += 500;
        printf("\nJa adquiriu o valor de '%d?'!\n\n", valor_acumulado);
        break;
    
    default:
        return 0;
    }
    jogador.reward = valor_acumulado;
    printf("Patamar alcançao, ja garantiu %d€\n", jogador.reward);

    printf("Quarta questao:   Vale 2000€ em caso de acertar!\n\n");
    switch(mostrar_perguntas_e_validar_respostas(3, valor_acumulado))
    {
    case 2:
        jogador.reward = valor_acumulado;
        printf("Muito bem '%s', alcançou a quantia de '%d€'!\n\n", jogador.name, jogador.reward);
        return 0;

    case 1:
        valor_acumulado += 2000;
        printf("\nJa adquiriu o valor de '%d?'!\n\n", valor_acumulado);
        break;
    
    default:
        return 0;
    }
    
    printf("Quinta questao:   Vale 3500€ em caso de acertar!\n\n");
    switch(mostrar_perguntas_e_validar_respostas(4, valor_acumulado))
    {
    case 2:
        jogador.reward = valor_acumulado;
        printf("Muito bem '%s', alcançou a quantia de '%d€'!\n\n", jogador.name, jogador.reward);
        return 0;

    case 1:
        valor_acumulado += 3500;
        printf("\nJa adquiriu o valor de '%d?'!\n\n", valor_acumulado);
        break;
    
    default:
        return 0;
    } 
    
    printf("Sexta questao:   Vale 5000€ em caso de acertar!\n\n");
    switch(mostrar_perguntas_e_validar_respostas(5, valor_acumulado))
    {
    case 2:
        jogador.reward = valor_acumulado;
        printf("Muito bem '%s', alcançou a quantia de '%d€'!\n\n", jogador.name, jogador.reward);
        return 0;

    case 1:
        valor_acumulado += 5000;
        printf("\nJa adquiriu o valor de '%d?'!\n\n", valor_acumulado);
        break;
    
    default:
        return 0;
    } 

    jogador.reward = valor_acumulado;
    printf("Patamar alcançao, ja garantiu %d€\n", jogador.reward);
    
    printf("Setima questao:   Vale 10000€ em caso de acertar!\n\n");
    switch(mostrar_perguntas_e_validar_respostas(6, valor_acumulado))
    {
    case 2:
        jogador.reward = valor_acumulado;
        printf("Muito bem '%s', alcançou a quantia de '%d€'!\n\n", jogador.name, jogador.reward);
        return 0;

    case 1:
        valor_acumulado += 10000;
        printf("\nJa adquiriu o valor de '%d?'!\n\n", valor_acumulado);
        break;
    
    default:
        return 0;
    } 


    printf("Oitava questao:   Vale 15000€ em caso de acertar!\n\n");
    switch(mostrar_perguntas_e_validar_respostas(7, valor_acumulado))
    {
    case 2:
        jogador.reward = valor_acumulado;
        printf("Muito bem '%s', alcançou a quantia de '%d€'!\n\n", jogador.name, jogador.reward);
        return 0;

    case 1:
        valor_acumulado += 15000;
        printf("\nJa adquiriu o valor de '%d?'!\n\n", valor_acumulado);
        break;
    
    default:
        return 0;
    } 
  
    
    printf("Nona questao:   Vale 2000€ em caso de acertar!\n\n");
    switch(mostrar_perguntas_e_validar_respostas(8, valor_acumulado))
    {
    case 2:
        jogador.reward = valor_acumulado;
        printf("Muito bem '%s', alcançou a quantia de '%d€'!\n\n", jogador.name, jogador.reward);
        return 0;

    case 1:
        valor_acumulado += 20000;
        jogador.reward = valor_acumulado;
        printf("\nJa adquiriu o valor de '%d?'!\n\n", valor_acumulado);
        break;
    
    default:
        return 0;
    } 
}
int iniciar_concurso()
{
    //faz p concurso acontecer e repetir ate o user quiser
    char repetir;
    do
    {
    int vetor_perguntas_aleatorias[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
    alineas_random[1] = -1;
    alineas_random[2] = -1;
    alineas_random[3] = -1;
    alineas_random[4] = -1;
    
    printf("Seja bem vindo ao Quem quer ser milionário! O jogo onde uma boa dose de conhecimento e cultura geral o pode levar à fortuna!");
    gerar_vetores_perguntas_random_por_dificuldade();
    perguntas_e_respostas(&repetir);
    printf("\nParabens %s, pela sua performance, ganhou %d?!!\n", jogador.name, jogador.reward);
    colocar_jogador_em_resultados_txt();
    printf("%s -- %d?\n", jogador.name, jogador.reward);
    do
    {
        printf("Deseja repetir? (s/n)\n");
        scanf("%c", &repetir);
        while(getchar() != '\n');
        switch (toupper(repetir))
        {
        case 'S':
            printf("Boa Sorte!\n");
            break;
        
        case 'N':
            printf("Ok... Tu e que perdes...\n");
            break;
        
        default:
            printf("Escolha invalida! Insira 's' para repetir ou 'n' para sair do modo de jogo...\n");
            break;
        }
    } while (toupper(repetir) != 'S' && toupper(repetir) != 'N');
    } while (toupper(repetir) != 'N');
    printf("%s -- %d?", jogador.name, jogador.reward);
    

}
int n_de_questoes_cada_ficiculdade(int *n_faceis, int *n_normais, int *n_dificeis)
{
    //identifica quantas questoes de cada dificuldade existem em memoria
    *n_faceis = 0;
    *n_normais = 0;
    *n_dificeis = 0;
    for(int i = 0; i < n_perguntas; i ++)
    {
        if((vetorquestao[i].status == 'o') && (strcmp(vetorquestao[i].dificuldade, "facil") == 0))
        {
            *n_faceis = *n_faceis + 1;
        }        
        else if((vetorquestao[i].status == 'o') && (strcmp(vetorquestao[i].dificuldade, "medio") == 0))
        {
            *n_normais = *n_normais + 1;
        }        
        else if((vetorquestao[i].status == 'o') && (strcmp(vetorquestao[i].dificuldade, "dificil") == 0))
        {
            *n_dificeis = *n_dificeis + 1;
        }
    }
}

void inicio_de_jogo(char *tipo_de_file)
{
    //coloca tudo pronto para começar o jogo, cria ficheiros que nao tenham sido criados e coloca as questoes em memoria
    if(((fopen(FICHBIN, "r")) == NULL) || (fopen(FICHTXT, "r") == NULL))
    {
        criar_files_para_questoes();
        repor_perguntas_originais();
    }

    if(fopen(RESULTTXT, "r") == NULL)
    {
        criar_resultados_txt();
        printf("Ficheiro para os resultados criado!\n");
    }

    if((int)toupper(*tipo_de_file) == 66)
    {
        printf("Iremos carregar as questoes através do ficheiro Binário!\n");
        n_perguntas = colocar_questoes_em_vetor_lendo_do_bin(vetorquestao);
    }    
    if((int)toupper(*tipo_de_file) == 84)
    {
        printf("Iremos carregar as questoes através do ficheiro txt!\n");
        n_perguntas = colocar_questoes_em_vetor_lendo_do_txt(vetorquestao); 
    }
    printf("\n\n\n\n");

}
char Menu(char *opcoes[])
{
    //usada para mostrar o menu e obter qual a ação do user
    int i;
    char ch;
    while(1)
    {
        printf("\n");
        for(i = 0; opcoes[i] != NULL; i ++)
        {
            printf("\t %s\n", opcoes[i]);
        }
        printf("\tOpcao:    \n");
        scanf("%c", &ch);
        if(ch == '\n')
        {
            printf("<Enter> E Uma Escolha Invalida...\n\n");
            return 0;
        }
        while(getchar() != '\n');
        for(i = 0; opcoes[i] != NULL; i ++)
        {
            if(opcoes[i][0] == ch)
            {
                return ch;
            }
        }
        printf("Escolha Invalida, tenta la outra vez...\n\n");
    }
}
void escolha_menu_secundario(int opcao, int *endn_faceis, int *endn_normais, int *endn_dificeis, char tipo_de_file)
{
    char *MenuSecundario[] =
    {
        "1. Listar Questoes:   ",
        "2. Procurar Questoes: ",
        "3. Adicionar Questao: ",
        "4. Remover Questao:   ",
        "5. Reiniciar Files de Questoes:   ",
        "6. Historico de resultados:",
        "7. Limpar o histórico de resultados",
        "0. Sair:              ",
        NULL
    };
    char continuaraq;
    printf("Menu secundario");
    while((opcao = Menu(MenuSecundario)) != '0')
    {
        switch (opcao)
        {
            case '1':
                if(tens_a_certeza() == 0)
                {
                    break;
                }
                else
                {
                    printf("Entao iremos Prosseguir!\n\n\n");
                }
                listar_questoes();
                break;

            case '2':   
                if(tens_a_certeza() == 0)
                {
                    break;
                }
                else
                {
                    printf("Entao iremos Prosseguir!\n\n\n");
                }
                do{
                    funcao_para_procurar_questoes();
                    do{
                        printf("Deseja procurar mais questoes?(s/n)\t");
                        scanf("%c", &continuaraq);
                        while(getchar() != '\n');
                        if(continuaraq != 's' && continuaraq != 'n')
                        {
                            printf("Escolha invalida, 's' para repetir ou 'n' para voltar ao menu anterior!\n");
                        }


                    }while(continuaraq != 's' && continuaraq != 'n');

                }while(continuaraq == 's');
                break;

            case '3':
                if(tens_a_certeza() == 0)
                {
                    break;
                }
                else
                {
                    printf("Entao iremos Prosseguir!\n\n\n");
                }
                do{
                adicionar_questoes(endn_faceis, endn_normais, endn_dificeis);
                    do{
                        printf("Deseja adicionar mais alguma questao?(s/n)\t");
                        scanf("%c", &continuaraq);
                        while(getchar() != '\n');
                        if(continuaraq != 's' && continuaraq != 'n')
                        {
                            printf("Escolha invalida, 's' para repetir ou 'n' para voltar ao menu anterior!\n");
                        }


                    }while(continuaraq != 's' && continuaraq != 'n');

                }while(continuaraq == 's');
                colocar_perguntas_em_ficheiro_bin();
                colocar_perguntas_em_ficheiro_txt();
                break;

            case '4':
                if(tens_a_certeza() == 0)
                {
                    break;
                }
                else
                {
                    printf("Entao iremos Prosseguir!\n\n\n");
                }
                do{
                    remover_questoes(endn_faceis, endn_normais, endn_dificeis);
                    do{
                        printf("Deseja remover mais alguma questao?(s/n)\t");
                        scanf("%c", &continuaraq);
                        while(getchar() != '\n');
                        if(continuaraq != 's' && continuaraq != 'n')
                        {
                            printf("Escolha invalida, 's' para repetir ou 'n' para voltar ao menu anterior!\n");
                        }


                    }while(continuaraq != 's' && continuaraq != 'n');

                }while(continuaraq == 's');
                colocar_perguntas_em_ficheiro_bin();
                colocar_perguntas_em_ficheiro_txt();
                break;

            case '5':
                if(tens_a_certeza() == 0)
                {
                    break;
                }
                else
                {
                    printf("Entao iremos Prosseguir!\n\n\n");
                }
                    reiniciar_files(tipo_de_file);
                    n_de_questoes_cada_ficiculdade(endn_faceis, endn_normais, endn_dificeis);
                break;
                            
                case '6':
                if(tens_a_certeza() == 0)
                {
                    break;
                }
                else
                {
                    printf("Entao iremos Prosseguir!\n\n\n");
                }
                do{
                    ler_resultados_de_resultados_txt();
                    do{
                        printf("Deseja repetir?(s/n)\t");
                        scanf("%c", &continuaraq);
                        while(getchar() != '\n');
                        if(continuaraq != 's' && continuaraq != 'n')
                        {
                            printf("Escolha invalida, 's' para repetir ou 'n' para voltar ao menu anterior!\n");
                        }


                    }while(continuaraq != 's' && continuaraq != 'n');

                }while(continuaraq == 's');
                break;                
                
                case '7':
                if(tens_a_certeza() == 0)
                {
                    break;
                }
                else
                {
                    printf("Entao iremos Prosseguir!\n\n\n");
                }
                do{
                    reiniciar_files_resulstados_txt();
                    do{
                        printf("Deseja repetir?(s/n)\t");
                        scanf("%c", &continuaraq);
                        while(getchar() != '\n');
                        if(continuaraq != 's' && continuaraq != 'n')
                        {
                            printf("Escolha invalida, 's' para repetir ou 'n' para voltar ao menu anterior!\n");
                        }


                    }while(continuaraq != 's' && continuaraq != 'n');

                }while(continuaraq == 's');
                break;

        }
    }
}
int main()
{
    char *MenuPrincipal[] = 
    {
        "1. Gerir Concurso:    ",
        "2. Iniciar Concurso:  ",
        "0. Sair:              ",
        NULL
    };
    char esperar;
    printf("Seja bem vindo ao Quem Quer Ser Milionario, um jogo onde iremos testar as suas capaciadades quanto a conhecimento geral\ncomeçaremos por pedir de que modo gostaria de carregar as perguntas para a memoria, tenha em conta que a opcao que escolher\nem nada afeta a experiencia do jogo visto que todas as alteacoes realizadas se verificam em ambos os ficheiros. \nPosteriormente podera escolher comecar um jogo ou passar a manipulacao de ficheiros sem qualquer restricao visto que existe\na opcao de reiniciar os files o que fara os ficheiros voltar a sua forma original nao tendo em conta as alterações feitas.\n\n");

    printf("----------------------------o\n                            |\n                            o---o\n                                |\n                      o---------o            o------------------> Divirta-se\n                      |                      |\n                      o----------------------o");
    printf("\n\n\n\nPrima <ENTER> ou insira qualquer letra para continuar...\n\n\n\n");
    do{
        scanf("%c", &esperar);
        if(esperar != '\n')
        {
            while(getchar() != '\n');
        }
    }while(esperar == '\n' && esperar != '\n');

    int n_faceis, n_normais, n_dificeis;
    char opcao;
    char tipo_de_file;
    do{

    printf("\n\n\nDeseja ler as questoes para memoria apartir do ficheiro bin ou txt?\n(\"b\" para Binario e \"t\" para txt)\t");
    scanf("%c", &tipo_de_file);
    while(getchar() != '\n');
    if((int)toupper(tipo_de_file) != 84 && (int)toupper(tipo_de_file) != 66 )
    {
        printf("\nEscolha invalida\t(\"b\" para Binario e \"t\" para txt)!\n");
    }
    }while((int)toupper(tipo_de_file) != 84 && (int)toupper(tipo_de_file) != 66);

    inicio_de_jogo(&tipo_de_file);
    n_de_questoes_cada_ficiculdade(&n_faceis, &n_normais, &n_dificeis);
    while((opcao = Menu(MenuPrincipal)) != '0')
    {
        switch (opcao)
        {
            case '1':
                escolha_menu_secundario(opcao, &n_faceis, &n_normais, &n_dificeis, tipo_de_file);
                break;

            case '2':
                if(tens_a_certeza() == 0)
                {
                    break;
                }
                else
                {
                    printf("Entao iremos Prosseguir!\n\n\n");
                }
                iniciar_concurso();
                break;

            default:
                printf("Opcao Invalida... Tenta la outra vez nem que seja pelo dinheiro...");
                break;
        }
    }
}
