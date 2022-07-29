#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <map>
#include <time.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#define TAM 151
using namespace std;

enum type{null,normal,fight,flying,poison,ground,rock,bug,ghost,steel,fire,water,grass,electric,psychic,ice,dragon,dark,fairy};
enum clima{sunny,partly_cloudy,cloudy,rain,snow,fog,windy};
struct golpe{
		string nome;
        type t;
        float dano;
        float energy; //ataque rápido faz ganhar essa quantidade, ataque carregado usa esse valor
    };
struct pokemon{
        string nome;
        float level;
        float ps, base_ps, atk, def, vel, energy;
        type t1;
        type t2; //null se tiver so um tipo
        struct golpe rapido, carregado;
        vector <string> gr, gc;
    }pokedex[TAM];
struct jogador{
	string nome;
	int shield;
	int nocaute;
	struct pokemon party[3];
}j1,j2;

map<string,struct golpe> movelist;

int random(int);
void escolha(struct jogador*);
struct pokemon escPokemon(int);
clima setClima();
void transform(struct pokemon*,struct pokemon*);
void batalha(struct pokemon*,struct pokemon*,struct jogador*,struct jogador*,clima);
void partida();
struct golpe ataque(struct pokemon*,struct pokemon*,struct jogador*,clima);
float modDano(type,struct pokemon,struct pokemon,clima);
float modStab(type,struct pokemon);
float modClima(type,clima);
float modTipo(type,struct pokemon);
type strToType(string);
void lerArquivo();
void arquivoGolpe();
void arquivoPokemon();

int main(){
	cout.setf(ios::fixed);
	lerArquivo();
	escolha(&j1);
	escolha(&j2);
	partida();
	system("pause");
    return 0;
}

int random(int x){	//Retorna um numero aleatório de 0 a x-1 (Só queria facilitar o uso do rand() )
	srand(time(NULL));
	return rand() % x;	//Em conjunto com srand(), gera um numero aleatório
}

void escolha(struct jogador* j){	//Permite o usuario escolher os Pokemons
	int esc;
	cout << "Entre com seu nome:";
	cin >> (*j).nome;
	cout << "\nTreinador(a) " << (*j).nome << ", escolha seus pokemons (1 - " << TAM << ")\n";
	for(int i=0;i<3;i++){
		cin >> esc;
		while((esc<1)||(esc>TAM)){
			cout << "\nEscolha um numero valido\n";
			cin >> esc;
		}
		cout << "\n" << pokedex[esc-1].nome << " escolhido!\n\n";
		(*j).party[i]=escPokemon(esc-1);
		float l;
		cout << "\nEscolha o seu level (1 - 40)\n";
		cin >> l;
		while((l<1)||(l>40)){
			cout << "\nEscolha um numero valido\n";
			cin >> l;
		}
		(*j).party[i].level=l;
		(*j).party[i].ps=(((*j).party[i].base_ps*2*l)/100.0)+l+10;	//Define a vida com base no level
		(*j).party[i].energy=0;
		if(i!=2){
			cout << "\nEscolha o proximo pokemon (1 - " << TAM << ")\n";
		}else{
			cout << "\n\nParty completa!\n\n";
			cout << "1 - " << "LVL." << setw(2) << setprecision(0) << (*j).party[0].level << " " << left << setw(15) << (*j).party[0].nome << "  R:" << left << setw(20) << (*j).party[0].rapido.nome << "  C:" << left << setw(20) << (*j).party[0].carregado.nome << "\n";
			cout << "2 - " << "LVL." << setw(2) << setprecision(0) << (*j).party[1].level << " " << left << setw(15) << (*j).party[1].nome << "  R:" << left << setw(20) << (*j).party[1].rapido.nome << "  C:" << left << setw(20) << (*j).party[1].carregado.nome << "\n";
			cout << "3 - " << "LVL." << setw(2) << setprecision(0) << (*j).party[2].level << " " << left << setw(15) << (*j).party[2].nome << "  R:" << left << setw(20) << (*j).party[2].rapido.nome << "  C:" << left << setw(20) << (*j).party[2].carregado.nome << "\n";
			cout << "\n\n";
		}
	}
	(*j).shield=2;
	(*j).nocaute=0;
}

struct pokemon escPokemon(int p){		//Escolhe ataques aleatórios para o Pokemon escolhido
	struct pokemon pk = pokedex[p];
	
	int size = pk.gr.size();
	int i = 0;
	while(i<size){	//Escolha do rápido
		cout << i+1 << " - " << pk.gr[i] << "\n";
		i++;
	}
	cout << "\nEscolha um ataque rapido (1 - " << size << ")\n";
	int esc;
	cin >> esc;
	while((esc < 1)||(esc > size)){
		cout <<"\nNumero invalido.Escolha novamente.\n";
		cin >> esc;
	}
	pk.rapido = movelist[pk.gr[esc-1]];
	
	size = pk.gc.size();
	i = 0;
	cout << "\n";
	while(i<size){	//Escolha do carregado
		cout << i+1 << " - " << pk.gc[i] << "\n";
		i++;
	}
	cout << "\nEscolha um ataque carregado (1 - " << size << ")\n";
	cin >> esc;
	while((esc < 1)||(esc > size)){
		cout <<"\nNumero invalido.Escolha novamente.\n";
		cin >> esc;
	}
	pk.carregado = movelist[pk.gc[esc-1]];
	
	return pk;
}

clima setClima(){	//Escolhe o clima para a batalha
	int r = random(7);
	cout << "\nClima atual: ";
	clima c;
	switch(r){
		case 0:
			c = sunny;
			cout << "Sunny";
			break;
		case 1:
			c = partly_cloudy;
			cout << "Partly Cloudy";
			break;
		case 2:
			c = cloudy;
			cout << "Cloudy";
			break;
		case 3:
			c = rain;
			cout << "Rain";
			break;
		case 4:
			c = snow;
			cout << "Snow";
			break;
		case 5:
			c = fog;
			cout << "Fog";
			break;
		case 6:
			c = windy;
			cout << "Windy";
			break;
	}
	cout << "\n";
	return c;
}

void transform(struct pokemon* pk1,struct pokemon* pk2){	//Ao usar transform, muda as caracteristicas do Pokemon
	if((*pk2).nome != "Ditto"){	//Impede um loop infinito de Ditto(Ditto)...
		if((*pk2).nome.find("Ditto") != -1){	//Verifica se o adversario é um Ditto transformado
			string s((*pk2).nome,5);
			(*pk1).nome += s;
		}else{
			(*pk1).nome += " (" + (*pk2).nome + ")";
		}
		(*pk1).rapido = (*pk2).rapido;
		(*pk1).carregado = (*pk2).carregado;
		(*pk1).base_ps = (*pk2).base_ps;
		(*pk1).ps=(((*pk1).base_ps*2*(*pk1).level)/100.0)+(*pk1).level+10;
		(*pk1).atk = (*pk2).atk;
		(*pk1).def = (*pk2).def;
		(*pk1).vel = (*pk2).vel;
		(*pk1).t1 = (*pk2).t1;
		(*pk1).t2 = (*pk2).t2;
	}	
}

void batalha(struct pokemon* p1,struct pokemon* p2,struct jogador* j1,struct jogador* j2,clima c){	//Faz a troca de turnos numa batalha pokemon
	struct jogador j[2];
	j[0] = *j1;
	j[1] = *j2;
	struct pokemon pk1 = *p1;
	struct pokemon pk2 = *p2;
	int i;
	if(pk1.vel>=pk2.vel){		//Determina que pokemon ataca primeiro
		i = 0;
	}else{
		i = 1;
	}
	struct golpe g;
	int turno = 1;
	cout << " Turnos  \t" << setw(28) << "Atacante" << "\t" << setw(25) << "Golpe" << "\t" << setw(8) << "P1 Vida" << "\t" << setw(8) << "P2 Vida" << "\t" << setw(11) << "P1 Energia" << "\t" << setw(11) << "P2 Energia" << "\n";
	cout << "Turno:" << setw(3) << 0 << "\t" << setw(28) << "-------" << "\t" << setw(25) << "-------" << "\t" << setw(8) << setprecision(2) << pk1.ps << "\t" << setw(8) << setprecision(2) << pk2.ps << "\t" << setw(11) << setprecision(2) << pk1.energy << "\t" << setw(11) << setprecision(2) << pk2.energy << "\n";
	while((pk1.ps>0)&&(pk2.ps>0)){
		if(i==0){
			g=ataque(&pk1,&pk2,&j[1],c);
			if(g.nome == "Transform"){
				transform(&pk1,&pk2);
			}
			cout << "Turno:" << setw(3) << turno << "\t" << setw(28) << pk1.nome << "\t" << setw(25) << g.nome << "\t" << setw(8) << setprecision(2) << pk1.ps << "\t" << setw(8) << setprecision(2) << pk2.ps << "\t" << setw(11) << setprecision(2) << pk1.energy << "\t" << setw(11) << setprecision(2) << pk2.energy << "\n";
			i=1;		
		}else{
			g=ataque(&pk2,&pk1,&j[0],c);
			if(g.nome == "Transform"){
				transform(&pk2,&pk1);
			}
			cout << "Turno:" << setw(3) << turno << "\t" << setw(28) << pk2.nome << "\t" << setw(25) << g.nome << "\t" << setw(8) << setprecision(2) << pk1.ps << "\t" << setw(8) << setprecision(2) << pk2.ps << "\t" << setw(11) << setprecision(2) << pk1.energy << "\t" << setw(11) << setprecision(2) << pk2.energy << "\n";
			i=0;
		}
		turno++;
		if(turno > 250){	//Anti rodada infinita devido a imunidade
			cout << "\n\n\t\t\tPossivel rodada infinita, aquele que possui maior HP ganha";
			if(pk1.ps > pk2.ps){
				pk2.ps = 0;
			}else{
				pk1.ps = 0;
			}
			break;
		}
	}
	if(pk1.ps <= 0){		//Verifica quem perdeu
		cout << "\n\n\t\t\t" << pk1.nome << " de " << (*j1).nome << " foi derrotado!\n\n";
		(*j1).nocaute ++;
	}else{
		cout << "\n\n\t\t\t" << pk2.nome << " de " << (*j2).nome << " foi derrotado!\n\n";
		(*j2).nocaute ++;
	}
	*p1=pk1;		//atualiza as informações dos pokemons e dos treinadores
	*p2=pk2;
	(*j1).shield = j[0].shield;
	(*j2).shield = j[1].shield;
}

void partida(){
	clima c = setClima();
	int b=1;
	while((j1.nocaute!=3)&&(j2.nocaute!=3)){
		cout << "\nRodada " << b <<": " << j1.party[j1.nocaute].nome << " vs " << j2.party[j2.nocaute].nome << "\n\n";
		batalha(&j1.party[j1.nocaute],&j2.party[j2.nocaute],&j1,&j2,c);
		b++;
	}
	if(j1.nocaute==3){
		cout << "\n\n\t\t\tVitoria de " << j2.nome << "!\n\n";
	}else{
		cout << "\n\n\t\t\tVitoria de " << j1.nome << "!\n\n";
	}
}

struct golpe ataque(struct pokemon* pkAtk,struct pokemon* pkDef,struct jogador* j,clima c){
    struct golpe g;
    int ver=0;
	if((*pkAtk).energy>=(*pkAtk).carregado.energy){	//Checa se o ataque carregado poderá ser usado
    	(*pkAtk).energy-=(*pkAtk).carregado.energy;
		g=(*pkAtk).carregado;
    	if((*j).shield>0){	//Checa a existencia de escudo
    		(*j).shield--;
    		return g;
		}
	}else{
		g=(*pkAtk).rapido;
		(*pkAtk).energy+=g.energy;
	}
	float n=(*pkAtk).level;
   	float d=((((((2*n)/5.0)+2)*g.dano*((*pkAtk).atk/(*pkDef).def))/50.0)+2)*modDano(g.t,*pkAtk,*pkDef,c);	//Calcula o dano, aplica na vida do inimigo e incrementa 50% do valor na energia
   	(*pkDef).ps-=d;
	(*pkDef).energy+=d*0.5;
   	return g;
}

float modDano(type atk,struct pokemon pkAtk,struct pokemon pkDef,clima c){//Junto todos os modificadores e adiciona o fator aleatório
	float alpha = (100.0 - random(16))/100.0;
	return modTipo(atk,pkDef) * modClima(atk,c) * modStab(atk,pkAtk) * alpha;
}

float modStab(type atk,struct pokemon pk){//tipo do ataque, pokemon atacante
	float mod=1;
	if((atk==pk.t1)||(atk==pk.t2)){	//verifica se o tipo de ataque combina com algum tipo do pokemon
		mod=1.5;
	}
	return mod;
}

float modClima(type atk,clima c){//tipo do ataque, clima atual
	float mod=1;
	switch(c){					//Verifica se o tipo de ataque tem bonus no clima atual
		case sunny:
			if((atk==fire)||(atk==grass)||(atk==ground)){
				mod=1.3;
			}
			break;
		case partly_cloudy:
			if((atk==normal)||(atk==rock)){
				mod=1.3;
			}
			break;
		case cloudy:
			if((atk==fairy)||(atk==fight)||(atk==poison)){
				mod=1.3;
			}
			break;
		case rain:
			if((atk==water)||(atk==electric)||(atk==bug)){
				mod=1.3;
			}
			break;
		case snow:
			if((atk==ice)||(atk==steel)){
				mod=1.3;
			}
			break;
		case fog:
			if((atk==dark)||(atk==ghost)){
				mod=1.3;
			}
			break;
		case windy:
			if((atk==dragon)||(atk==flying)||(atk==psychic)){
				mod=1.3;
			}
			break;
	}
	return mod;
}

float modTipo(type atk,struct pokemon pk){ //tipo do ataque, pokemon defensor
	float mod=1;
	type t[2]={pk.t1,pk.t2};
	for(int i=0;i<2;i++){		//Checa para cada tipo do pokemon defensor se há fraqueza/resistencia ao tipo de ataque recebido
		switch(t[i]){
			case null:		//Caso o pokemon n tenha 2 tipos, irá apenas quebrar a repetição
				break;
			case normal:
				switch(atk){
					case fight:
						mod*=2;
						break;
					case ghost:
						mod*=0;
						break;
				}
				break;
			case fight:
				switch(atk){
					case flying:
						mod*=2;
						break;
					case rock:
						mod*=0.5;
						break;
					case bug:
						mod*=0.5;
						break;
					case psychic:
						mod*=2;
						break;
					case dark:
						mod*=0.5;
						break;
					case fairy:
						mod*=2;
						break;
				}
				break;
			case flying:
				switch(atk){
					case fight:
						mod*=0.5;
						break;
					case ground:
						mod*=0;
						break;
					case rock:
						mod*=2;
						break;
					case bug:
						mod*=0.5;
						break;
					case grass:
						mod*=0.5;
						break;
					case electric:
						mod*=2;
						break;
					case ice:
						mod*=2;
						break;
				}
				break;
			case poison:
				switch(atk){
					case fight:
						mod*=0.5;
						break;
					case poison:
						mod*=0.5;
						break;
					case ground:
						mod*=2;
						break;
					case bug:
						mod*=0.5;
						break;
					case grass:
						mod*=0.5;
						break;
					case psychic:
						mod*=2;
						break;
					case fairy:
						mod*=0.5;
						break;
				}
				break;
			case ground:
				switch(atk){
					case poison:
						mod*=0.5;
						break;
					case rock:
						mod*=0.5;
						break;
					case water:
						mod*=2;
						break;
					case grass:
						mod*=2;
						break;
					case electric:
						mod*=0;
						break;
					case ice:
						mod*=2;
						break;
				}
				break;
			case rock:
				switch(atk){
					case normal:
						mod*=0.5;
						break;
					case fight:
						mod*=2;
						break;
					case flying:
						mod*=0.5;
						break;
					case poison:
						mod*=0.5;
						break;
					case ground:
						mod*=2;
						break;
					case steel:
						mod*=2;
						break;
					case fire:
						mod*=0.5;
						break;
					case water:
						mod*=2;
						break;
					case grass:
						mod*=2;
						break;
				}
				break;
			case bug:
				switch(atk){
					case fight:
						mod*=0.5;
						break;
					case flying:
						mod*=2;
						break;
					case ground:
						mod*=0.5;
						break;
					case rock:
						mod*=2;
						break;
					case fire:
						mod*=2;
						break;
					case grass:
						mod*=0.5;
						break;
				}
				break;
			case ghost:
				switch(atk){
					case normal:
						mod*=0;
						break;
					case fight:
						mod*=0;
						break;
					case poison:
						mod*=0.5;
						break;
					case bug:
						mod*=0.5;
						break;
					case ghost:
						mod*=2;
						break;
					case dark:
						mod*=2;
						break;
				}
				break;
			case steel:
				switch(atk){
					case normal:
						mod*=0.5;
						break;
					case fight:
						mod*=2;
						break;
					case flying:
						mod*=0.5;
						break;
					case poison:
						mod*=0;
						break;
					case ground:
						mod*=2;
						break;
					case rock:
						mod*=0.5;
						break;
					case bug:
						mod*=0.5;
						break;
					case steel:
						mod*=0.5;
						break;
					case fire:
						mod*=2;
						break;
					case grass:
						mod*=0.5;
						break;
					case psychic:
						mod*=0.5;
						break;
					case ice:
						mod*=0.5;
						break;
					case dragon:
						mod*=0.5;
						break;
					case fairy:
						mod*=0.5;
						break;
				}
				break;
			case fire:
				switch(atk){
					case ground:
						mod*=2;
						break;
					case rock:
						mod*=2;
						break;
					case bug:
						mod*=0.5;
						break;
					case steel:
						mod*=0.5;
						break;
					case fire:
						mod*=0.5;
						break;
					case water:
						mod*=2;
						break;
					case grass:
						mod*=0.5;
						break;
					case ice:
						mod*=0.5;
						break;
					case fairy:
						mod*=0.5;
						break;
				}
				break;
			case water:
				switch(atk){
					case steel:
						mod*=0.5;
						break;
					case fire:
						mod*=0.5;
						break;
					case water:
						mod*=0.5;
						break;
					case grass:
						mod*=2;
						break;
					case electric:
						mod*=2;
						break;
					case ice:
						mod*=0.5;
						break;
				}
				break;
			case grass:
				switch(atk){
					case flying:
						mod*=2;
						break;
					case poison:
						mod*=2;
						break;
					case ground:
						mod*=0.5;
						break;
					case bug:
						mod*=2;
						break;
					case fire:
						mod*=2;
						break;
					case water:
						mod*=0.5;
						break;
					case grass:
						mod*=0.5;
						break;
					case electric:
						mod*=0.5;
						break;
					case ice:
						mod*=2;
						break;
				}
				break;
			case electric:
				switch(atk){
					case flying:
						mod*=0.5;
						break;
					case ground:
						mod*=2;
						break;
					case steel:
						mod*=0.5;
						break;
					case electric:
						mod*=0.5;
						break;
				}
				break;
			case psychic:
				switch(atk){
					case fight:
						mod*=0.5;
						break;
					case bug:
						mod*=2;
						break;
					case ghost:
						mod*=2;
						break;
					case psychic:
						mod*=0.5;
						break;
					case dark:
						mod*=2;
						break;
				}
				break;
			case ice:
				switch(atk){
					case fight:
						mod*=2;
						break;
					case rock:
						mod*=2;
						break;
					case steel:
						mod*=2;
						break;
					case fire:
						mod*=2;
						break;
					case ice:
						mod*=0.5;
						break;
				}
				break;
			case dragon:
				switch(atk){
					case fire:
						mod*=0.5;
						break;
					case water:
						mod*=0.5;
						break;
					case grass:
						mod*=0.5;
						break;
					case electric:
						mod*=0.5;
						break;
					case ice:
						mod*=2;
						break;
					case dragon:
						mod*=2;
						break;
					case fairy:
						mod*=2;
						break;
				}
				break;
			case dark:
				switch(atk){
					case fight:
						mod*=2;
						break;
					case bug:
						mod*=2;
						break;
					case ghost:
						mod*=0.5;
						break;
					case psychic:
						mod*=0;
						break;
					case dark:
						mod*=0.5;
						break;
					case fairy:
						mod*=2;
						break;
				}
				break;
			case fairy:
				switch(atk){
					case fight:
						mod*=0.5;
						break;
					case poison:
						break;
						mod*=2;
					case bug:
						mod*=0.5;
						break;
					case steel:
						mod*=2;
						break;
					case dragon:
						mod*=0;
						break;
					case dark:
						mod*=0.5;
						break;
				}
				break;
		}
		if(mod==0){
			break;
		}
	}
	return mod;
}

type strToType(string s){ //transforma uma string no tipo escrito para pegar a informação no arquivo
	if(s == "null")
		return null;
		
	if(s == "normal")
		return normal;
		
	if(s == "fight")
		return fight;
		
	if(s == "poison")
		return poison;
		
	if(s == "ground")
		return ground;
		
	if(s == "rock")
		return rock;
		
	if(s == "bug")
		return bug;
		
	if(s == "ghost")
		return ghost;
		
	if(s == "steel")
		return steel;
		
	if(s == "fire")
		return fire;
	
	if(s == "water")
		return water;
		
	if(s == "grass")
		return grass;
		
	if(s == "electric")
		return electric;
	
	if(s == "psychic")
		return psychic;
		
	if(s == "ice")
		return ice;
		
	if(s == "dragon")
		return dragon;
		
	if(s == "dark")
		return dark;
		
	if(s == "fairy")
		return fairy;
}

void lerArquivo(){	//Acabei separando o arquivo de golpe dos de Pokemon pq achei q seria mais facil mexer e evitar bugs
	arquivoGolpe();
	arquivoPokemon();
}

void arquivoGolpe(){
	fstream fin;
	fin.open("movelist_database.csv", ios::in);
	string line,word;
	while(fin.peek()!=EOF){
		vector<string> row;
		getline(fin,line);
		stringstream s(line);
		while (getline(s, word,',')) {
            row.push_back(word); 
        }
        struct golpe g;
        g.nome = row[0];
        g.energy = stof(row[1]);
        g.dano = stof(row[2]);
        g.t = strToType(row[3]);
        movelist[g.nome] = g;
    }
}

void arquivoPokemon(){
	fstream fin;
	fin.open("pokemon_database.csv", ios::in);
	string line,word;
	for(int i=0;i<TAM;i++){
		vector<string> row;
		getline(fin,line);
		stringstream s(line);
		while (getline(s, word,',')) {
            row.push_back(word); 
        }
		pokedex[i].nome = row[0];
		pokedex[i].base_ps = stof(row[1]);
		pokedex[i].atk = stof(row[2]);
		pokedex[i].def = stof(row[3]);
		pokedex[i].vel = stof(row[4]);
		pokedex[i].t1 = strToType(row[5]);
		pokedex[i].t2 = strToType(row[6]);
		int j = 7;
		while(row[j]!="NULL"){
			pokedex[i].gr.push_back(row[j]);
			j++;
		}
		j++;
		while(row[j]!="NULL"){
			pokedex[i].gc.push_back(row[j]);
			j++;
		}
	}
}
