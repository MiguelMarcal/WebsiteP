import java.io.*;
import java.util.*;
import java.lang.*;



public class TrafficAnalysis {

	public static void main (String argv[]) throws Exception
	{
		//Starting time
        long tot = System.currentTimeMillis();

		//Reading CSV
		File file = new File("MP2_Grupo62.csv");
		//File file = new File("testing_trace.csv");  //file para o .csv de teste
      		FileReader fr = new FileReader(file);
      		BufferedReader br = new BufferedReader(fr);
		
		String line; //Para o br
		
		//Para guardar tamanho dos pacotes (posteriormente colocado no ficheiro "myData.txt")
		List<Integer> list = new ArrayList<Integer>();

		//Q1/Q2
		int ipv4 = 0;	//Para o numero de IPv4
		int ipv6 = 0;	//Para o numero de IPv6

		//Q5
		int max = 0;
		int min = 0;
		long total = 0L;

		int rst = 0;
		
		int qt = 0;		//Numero da linha atual

		
		HashSet<String> ipdif = new HashSet<String>();	//HashSet para guardar os Ips unicos
		HashSet<String> pcdif = new HashSet<String>();	//HashSet para guardar os portos TCP unico


		while ((line = br.readLine()) != null) {
			
			//Ignorar a 1a linha
			if(qt == 0){
			qt++;
			continue;
			}

			//Colocar a linha lida como token, separando por ','
			StringTokenizer token = new StringTokenizer(line,",");
			token.nextToken();
			token.nextToken(); 		//Passa ao proximo token que seria o tempo passado (nu)
			String source = token.nextToken();		//String que guarda o IP Origem
			String dest = token.nextToken();		//String que guarda o IP Destino

			String port_or = token.nextToken().replace("\"", "");	//String que guarda o porto de origem

			token.nextToken();	//Passa ao proximo token que seria o porto de destino (nu)

			String prot = token.nextToken().replace("\"", "");		//String que guarda o protocolo do pacote

			int lg = Integer.parseInt(token.nextToken().replace("\"", ""));	// Int que guarda o tamanho do pacote
			//String lg = token.nextToken();

			String flag = token.nextToken().replace("\"", "");	//String que guarda o valor da flag
			
			

			//Calcular o numero de protocolos com ipv4's e protocolos com ipv6's Q1/Q2
			if((source.length()) <= 17){
				ipv4 ++;
			} else{
				ipv6 ++;
			}
			


		//Para contar o numero de ipv4's destino diferentes Q3
			if(ipdif.contains(dest) || dest.length() > 17){
			} else{
					ipdif.add(dest);
			}
			


			//Para contar os ports de origem diferentes Q4
			if(pcdif.contains(port_or)  || prot.compareTo("TCP") != 0){
			} else{
					pcdif.add(port_or);
			}
			


		//Tamanho medio, maximo e minimo dos pacotes Q5

			//Na primeira passagem o minimo e o maximo sao iguais ao valor do 'lg'
			if(qt == 1){
				min = lg;
				max = lg;
			}

			if(lg > max){			//colocar o 'max' = 'lg' caso o 'lg' seja maior que 'max'
				max = lg;
			} else if(lg < min){
				min = lg;			//colocar o 'min' = 'lg' caso o 'lg' seja menor que o 'min'
			}			
			

			//Verificar o envio de RST's (flag 0x004)
			if(flag.compareTo("0x004") == 0){
				rst ++;
			}

			list.add(lg);	//Adicionar o tamanho do pacote a lista
			
			qt ++;			//Aumentar o numero da linha
			total += lg;	//Variavel 'total' que será igual a soma do tamanho de todos os pacotes




		}	

		br.close();

		BufferedWriter respostas = new BufferedWriter(new FileWriter("Respostas_Grupo62.csv"));

		System.out.println("\nQ1\tPacotes IPv4 -> " + ipv4 + "\nQ2\tPacotes IPv6 -> " + ipv6);
		respostas.write("Q1:" + ipv4 + "\n");
		respostas.write("Q2:" + ipv6 + "\n");

		System.out.println("Q3\tTemos " + ipdif.size() + " Hosts IPV4 a receber pacotes");
		respostas.write("Q3:" + ipdif.size() + "\n");

		System.out.println("Q4\tTemos " + pcdif.size() + " Portos TCP Origem diferentes");
		respostas.write("Q4:" + pcdif.size() + "\n");

		float media = (float)total / qt;

		System.out.printf("Q5\t-Tamanho Medio -> %.2f\n  \t-Tamanho Maximo %d\n  \t-Tamanho Minimo %d", media, max, min);
		respostas.write("Q5:" + String.format("%.2f", media) + "," + max + "," + min + "\n");

		System.out.println("\nQ6\tTemos " + rst + " RST's enviados");
		respostas.write("Q6:" + rst + "\n");

		System.out.println("Respostas_Grupo62.csv Atualizado ");



        long start = System.currentTimeMillis();

		//Colocar o Tamanho dos pacote (x) e a % dos pacotes que sao maiores que x (y) myData.txt
		BufferedWriter myDataFile = new BufferedWriter(new FileWriter("myData.txt"));
		HashSet<Integer> llist = new HashSet<Integer>(list);	//HashSet para guardar os portos TCP unico
		
		int[] occore = new int[llist.size()];
		for(int i = 0; i < occore.length; i ++){
			occore[i] = Collections.frequency(list, i + min);
		}
		
		//Iterator xAxis = llist.iterator();
		int maiores = 0;

		//while(xAxis.hasNext()){
		for(int i =0;i < (max+min);i++){
			//int i = (Integer)xAxis.next();
			Iterator valores = llist.iterator();

			while(valores.hasNext())
			{
				int valor = (Integer)valores.next();
				if(valor > i){
					maiores += occore[valor-min];
				}
			}

			float porcentagem = (float)maiores/list.size();
			myDataFile.write(i + " " + (porcentagem * 100) + "\n");
			maiores = 0;
		}



		//Finaliza o Temporizador e imprime o tempo que demorou a inserir os valores em myData.txt
		long fin = System.currentTimeMillis();
		System.out.println("myData.txt Atualizado\n" + ((fin - start)/1000) + "s para Atualizar o ficheiro");

		

		//Criar o script para o gnuplot
		BufferedWriter myGnuplotFile = new BufferedWriter(new FileWriter("myPlot.gp"));
		myGnuplotFile.write("set terminal svg size 1000,700\n");
		myGnuplotFile.write("set output 'Q7_Grupo62.svg'\n");
		myGnuplotFile.write("set xrange [0:" + (max +50) + "]\n");
		myGnuplotFile.write("set yrange [-10:110]\n");
		myGnuplotFile.write("unset key\n");
		myGnuplotFile.write("set key\n");
		myGnuplotFile.write("set title 'CCDF Pacotes Captura'\n");
		myGnuplotFile.write("media(x) = " + media +"\n");
		myGnuplotFile.write("set xlabel 'Tamanho Pacotes'\n");
		myGnuplotFile.write("set ylabel '% Pacotes > x'\n");
		myGnuplotFile.write("plot 'myData.txt' linetype 1  linecolor 0 title 'Probabilidade' with lines\n");
			
		System.out.println("myPlot.gp Atualizado ");


		respostas.close();
		myDataFile.close();
    	myGnuplotFile.close();

		//Finaliza o Temporizador e imprime o tempo que demorou a correr o programa na totalidade
		long end = System.currentTimeMillis();
        System.out.println("\n\n-------" + "o\n" + "       |\n       o--------> " +
        ((end - tot)/1000) + "s " + "Para correr o Programa");						
    }
}
