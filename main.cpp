// *********************************************************************
//                            LIBRERIAS
// *********************************************************************

#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>

#include <iostream>

// *********************************************************************
//                       CONSTANTES GLOBALES
// *********************************************************************

/* Maxima cantidad de generaciones */
const int MAX_GEN = 10000000;

/* Tamaño de la población */
const int TAM_POB = 500;

/* Probabilidad de cruzamiento */
const float PROB_CRU = 0.33;

/* Probabilidad de mutación */
const float PROB_MUT = 0.66;

/* Tamanno del problema */
const int PROBLEM_SIZE = 3;

const int NONET_SIZE = PROBLEM_SIZE * PROBLEM_SIZE;
const int BOARD_SIZE = NONET_SIZE * NONET_SIZE;

// *********************************************************************
//                           ESTRUCTURAS
// *********************************************************************

/* cell::Celda
 * 
 * Representa una celda del tablero.
 * value:  valor de la casilla. Dominio {1, 2, 3, 4, 5, 6, 7, 8, 9, 0} 
 * 		   siendo 1-9 numeros y 0 que no se ha asignado valor.
 * group:  identificador del grupo al que pertenece la casilla 
 * preset: si la casilla fue entregada como pista inicial en el tablero */
struct cell {
	int value;
	bool preset = false; 
};

/* dna::ADN 
 * 
 * Representa el cromozoma del individuo
 * board:   vector de celdas, inicializado en BOARD_SIZE, basicamente 
 * 			el tablero.
 * fitness: valor de la funcion de evaluacion evaluada en el tablero.
 * */
struct dna {
	std::vector<cell> board = std::vector<cell>(BOARD_SIZE);
	int fitness;
	float prob;
};

/* population::Poblacion
 * 
 * Representa a todos los individuos de la poblacion
 * members:  vector de cromozomas.
 * best:     cromozoma elite.
 * */
struct population {
	std::vector<dna> members;
	dna best;
};

/* coord::Coordenada
 * 
 * Representa una coordenada cartesiana
 * x:  indice de la posicion horizontal en el tablero. <col>
 * y:  indice de la posicion vertical en el tablero. <row>
 * */
struct coord {
	int x;
	int y; 
};

/* group::Sub conjunto de celdas 
 * 
 * Representa varias celdas que conforman un grupo
 * members: vector de coordenadas, correspondiente a los miembros.
 * id:      identificador del grupo 
 * */
struct group {
	std::vector<coord> members;
	int total;
	int id;
};

/* killer::Conjunto de subconjuntos 
 * 
 * Representa todos los subconjuntos del tablero
 * groups: vector de grupos.
 * */
struct killer {
	std::vector<group> groups;
};

/* dom::Dominio de casilla
 * 
 * Representa una lista de valores enteras usualmente como range(1,10)
 * values:  vector de enteros.
 * */
struct dom {
	std::vector<int> values;
};

// *********************************************************************
//                           FUNCIONES
// *********************************************************************

int col_fitness(dna& DNA);
int row_fitness(dna& DNA);
int nonet_fitness(dna& DNA);
int groups_fitness(dna& DNA, killer& RULES);
void calculate_fitness(dna& DNA, killer& RULES);

int indexOf(int row, int col);
int indexOfNonet(int row, int col);
int randint(int MAX);
float randfloat(float low, float high);
int randintClose(dna& DNA, int row);
dom createDom();
int findDuplicates(std::vector<int>& set);
void cleanDNA(dna& DNA);
int getAt(const dna& DNA, int row, int col);
void setAt(dna& DNA, int row, int col, int newVal);
int getGAt(const dna& DNA, int row, int col);
void setGAt(dna& DNA, int row, int col, int newVal);
int getNAt(const dna& DNA, int row, int col);
void setNAt(dna& DNA, int row, int col, int newVal);
dna calculateBest(population& POB);
dna getBest(population& POB);
void setBest(population& POB, dna newVal);
void blockCell(dna& DNA, int row, int col);

dna makeChild(dna& DNA1, dna& DNA2, killer& RULES, int split);
void combine(dna& DNA1, dna& DNA2, killer& RULES, population& POB);
population crossover(population& POB, killer& RULES);
void swap(dna& DNA, int cell1, int cell2, killer& RULES);
void mutate(dna& DNA, killer& RULES, population& POB);
population mutation(population& POB, killer& RULES);
population selection(population& POB);
void elite(population& POB, population& newPOB);

void initGen(dna& DNA);
population initPopulation(dna& DNA, killer& RULES);

void loadGame(std::string filename, dna& DNA, killer& RULES);
void showGame(dna& DNA);

// *********************************************************************
//                               MAIN
// *********************************************************************

int main(){
	// Variables del problema
	srand (time(NULL));
	population POB;
	dna ORIGIN;
	killer RULES;
	std::ofstream outfile;
	
	// SI SE QUIERE ATOMICO: TRUE - DIRECTORIO: FALSE
	bool single = true;
	
	// PUZZLE atomico
	//std::string PUZZLE = "real.txt";
	std::string PUZZLE = "extreme_filled.txt";
	
	// PUZZLES en directorio (WARNING:: NOT WORKING FINE!)
	std::string DIR = "./Instancias/instancias-ejem/";
	std::vector<std::string> PUZZLES{"easier.txt", "easier_filled.txt", "easy.txt", "easy_filled.txt", "example.txt", "example_filled.txt", "extreme.txt", "extreme_filled.txt", "hard.txt", "hard_filled.txt", "mind_bending.txt", "mind_bending_filled.txt", "moderate.txt", "moderate_filled.txt", "outrageous.txt", "outrageous_filled.txt"};
	
	if (single){
		PUZZLES.clear();
		PUZZLES.push_back(PUZZLE);
	}
	
	for (auto& name : PUZZLES) {
		PUZZLE = DIR + name;
		outfile.open("./results/out_"+name, std::ios_base::app);
		// CARGA DE PUZZLE
		loadGame(PUZZLE, ORIGIN, RULES);
		// MOSTRAR JUEGO
		showGame(ORIGIN);
		// INICIALIZAR POBLACION
		POB = initPopulation(ORIGIN, RULES);
		// ITERACION GENERACIONAL
		for (int i = 0; i < MAX_GEN; i++) {
			population nextGen;
			// PROCESO DE SELECCION
			nextGen = selection(POB);
			// PROCESO DE CRUZAMIENTO
			nextGen = crossover(nextGen, RULES);
			// PROCESO DE MUTACION
			nextGen = mutation(nextGen, RULES);
			// PROCESO DE ELITISMO
			elite(POB, nextGen);
			POB = nextGen;
			
			// RESULTADOS
			// ARCHIVO TAM_POB:MAX_GEN:PROB_CRU:PROB_MUT:GEN:BESTGEN
			outfile << TAM_POB << ":" << MAX_GEN << ":" << PROB_CRU << ":" << PROB_MUT << ":" << i << ":" << POB.best.fitness << std::endl;
			std::cout << "GENERACION: " << i + 1 << " FITNESS: " << POB.best.fitness << " POB: " << POB.members.size() << std::endl;
			std::cout << std::endl;
			showGame(POB.best);
			std::cout << std::endl;
			if (POB.best.fitness == 0) {
				break;
			}
		}
		outfile.close();
	}
	return 0;
}

// *********************************************************************
//                    FUNCIONES IMPLEMENTADAS
// *********************************************************************

int indexOf (int row, int col) {
	return 9 * row + col;
};

int indexOfNonet (int row, int col) {
	return 3 * (row % 3) + pow(3,3) * (row / 3) + pow(3,2) * (col / 3) + (col % 3);
};

int randint (int MAX) {
	return rand() % MAX + 1;
};

float randfloat(float low, float high) {
	return low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high-low)));
}

int randintClose (dna& DNA, int row) {
	int result, value;
	bool flag = true;
	dom DOM = createDom();
	for (int i = 0; i < NONET_SIZE; i++) {
		value = getAt(DNA, row, i);
		if (value != 0) {
			for (unsigned j = 0; j < DOM.values.size(); j++) {
				if (DOM.values.at(j) == value) {
					DOM.values.erase(DOM.values.begin() + j);
					break;
				}
			}
		}
	}
	while (flag) {
		result = randint(9);
		for (unsigned i = 0; i < DOM.values.size(); ++i) {
			if (result == DOM.values.at(i)) {
				flag = false;
				break;
			}
		}
	}
	return result;
};

dom createDom () {
	dom DOM;
	for (int i = 1; i < NONET_SIZE + 1; i++) {
		DOM.values.push_back(i);
	}
	return DOM;
}

int getAt (const dna& DNA, int row, int col) {
	return DNA.board.at(indexOf(row, col)).value;
};

void setAt (dna& DNA, int row, int col, int newVal) {
	DNA.board.at(indexOf(row, col)).value = newVal;
};

int getNAt (const dna& DNA, int row, int col) {
	return DNA.board.at(indexOfNonet(row, col)).value;
};

void setNAt (dna& DNA, int row, int col, int newVal) {
	DNA.board.at(indexOfNonet(row, col)).value = newVal;
};

void blockCell (dna& DNA, int row, int col) {
	DNA.board.at(indexOf(row, col)).preset = true;
};

int findDuplicates (std::vector<int>& set) {
	int total = 0;
	std::map<int, int> counter;
	for (auto& elem : set) {
		auto result = counter.insert(std::pair<int,int>(elem, 1));
		if (result.second == false) {
			result.first -> second++;
		}
	}
	for (auto it = counter.begin(); it != counter.end();) {
		if (it -> second == 1) {
			it = counter.erase(it);
		}
		else {
			it++;
		}
	}
	for (auto& elem : counter) {
		total = total + elem.second;
	}
	return total;
};

int row_fitness (dna& DNA) {
	int fitness = 0;
	std::vector<int> aux = std::vector<int>(NONET_SIZE);
	for(int row = 0; row < NONET_SIZE; row++) {
		for (int col = 0; col < NONET_SIZE; col++) {
			aux.at(col) = getAt(DNA, row, col);
		}
		fitness = fitness + findDuplicates(aux);
	}
	return fitness;
};

int col_fitness (dna& DNA) {
	int fitness = 0;
	std::vector<int> aux = std::vector<int>(NONET_SIZE);
	for(int col = 0; col < NONET_SIZE; col++) {
		for (int row = 0; row < NONET_SIZE; row++) {
			aux.at(row) = getAt(DNA, row, col);
		}
		fitness = fitness + findDuplicates(aux);
	}
	return fitness;
};

int nonet_fitness (dna& DNA) {
	int fitness = 0;
	std::vector<int> aux = std::vector<int>(NONET_SIZE);
	for(int row = 0; row < NONET_SIZE; row++) {
		for (int col = 0; col < NONET_SIZE; col++) {
			aux.at(col) = getNAt(DNA, row, col);
		}
		fitness = fitness + findDuplicates(aux);
	}
	return fitness;
};

int groups_fitness (dna& DNA, killer& RULES) {
	int fitness = 0;
	for (auto& grp : RULES.groups) {
		int total = 0;
		for (auto& mmb : grp.members) {
			total = total + getAt(DNA, mmb.y, mmb.x);
		}
		if (grp.total != total){
			fitness = fitness + 1;
		}
	}
	return fitness;
};

void calculate_fitness (dna& DNA, killer& RULES) {
	DNA.fitness = row_fitness(DNA) + col_fitness(DNA) + nonet_fitness(DNA) + groups_fitness(DNA, RULES);
};

void cleanDNA(dna& DNA) {
	DNA.board.clear();
};

dna makeChild (dna& DNA1, dna& DNA2, killer& RULES, int split) {
	dna child;
	cleanDNA(child);
	for (int i = 0; i < BOARD_SIZE; i++) {
		if (i < split) {
			child.board.push_back(DNA1.board.at(i));
		}
		else {
			child.board.push_back(DNA2.board.at(i));
		}
	}
	calculate_fitness(child, RULES);
	return child;
};

void combine(dna& DNA1, dna& DNA2, killer& RULES, population& POB, bool twoC) {
	float probability = randfloat(0.0,1.0);
	dna child1, child2;
	if (PROB_CRU >= probability) {
		int cut_point = randint(80);
		child1 = makeChild(DNA1,DNA2, RULES, cut_point);
		child2 = makeChild(DNA2,DNA1, RULES, cut_point);
	}
	else {
		child1 = DNA1;
		child2 = DNA2;
	}
	POB.members.push_back(child1);
	if (twoC) {
		POB.members.push_back(child2);
	}	
};

population crossover(population& POB, killer& RULES){
	population newEPOC;
	unsigned x, y;
	for (unsigned i = 0; i < POB.members.size() / 2 + 1; i++) {
		x = 2 * i;
		y = 2 * i + 1;

		if (x == POB.members.size()) {
			break;
		}
		else if (y == POB.members.size()) {
			y = 0;
			combine(POB.members.at(x), POB.members.at(y), RULES, newEPOC, false);
			break;
		}
		combine(POB.members.at(x), POB.members.at(y), RULES, newEPOC, true);
	}
	return newEPOC;
};

void swap(dna& DNA, int cell1, int cell2, killer& RULES) {
	cell aux;
	aux = DNA.board.at(cell1);
	DNA.board.at(cell1) = DNA.board.at(cell2);
	DNA.board.at(cell2) = aux;
	calculate_fitness(DNA, RULES);
}

void mutate(dna& DNA, killer& RULES, population& POB) {
	float probability = randfloat(0.0,1.0);
	if (PROB_MUT >= probability){
		int mutate_cell = randint(80);
		while (DNA.board.at(mutate_cell).preset) {
			mutate_cell = randint(80);
		}
		int mutate_cell2 = randint(80);
		while (DNA.board.at(mutate_cell2).preset) {
			mutate_cell2 = randint(80);
		}
		swap(DNA, mutate_cell, mutate_cell2, RULES);
	}
	POB.members.push_back(DNA);
};

population mutation(population& POB, killer& RULES){
	population newEPOC;
	for (unsigned i = 0; i < POB.members.size(); i++) {
		mutate(POB.members.at(i), RULES, newEPOC);
	}
	return newEPOC;
};

population selection(population& POB) {
	int GLOBAL_FITNESS = 0;
	float SUM_PROB = 0.0;
	float aux = 0.0;
	std::vector<float> helper;
	population SELECTED;
	for (unsigned i = 0; i < POB.members.size(); i++) {
		GLOBAL_FITNESS = GLOBAL_FITNESS + POB.members.at(i).fitness;
	}
	for (unsigned i = 0; i < POB.members.size(); i++) {
		float prob = (float)(POB.members.at(i).fitness) / (float)(GLOBAL_FITNESS);
		prob = 1.0 - prob;
		helper.push_back(prob);
		SUM_PROB = SUM_PROB + prob;
	}
	for (unsigned i = 0; i < POB.members.size(); i++) {
		float prob = helper.at(i) / SUM_PROB;
		aux = aux + prob;
		POB.members.at(i).prob = aux;
	}
	for (int i = 0; i < TAM_POB; i++) {
		float prob = randfloat(0.0,1.0);
		for (unsigned j = 0; j < POB.members.size(); j++) {
			if (POB.members.at(j).prob >= prob){
				SELECTED.members.push_back(POB.members.at(j));
				break;
			}
		}
	}
	return SELECTED;
};

dna getBest (population& POB){
	return POB.best;
};

dna calculateBest (population& POB) {
	int MIN = 999999999;
	dna TOP;
	for (unsigned j = 0; j < POB.members.size(); j++) {
		if (POB.members.at(j).fitness < MIN){
			MIN = POB.members.at(j).fitness;
			TOP = POB.members.at(j);
		}
	}
	return TOP;
};

void setBest (population& POB, dna newVal) {
	POB.best = newVal;
};

void elite (population& POB, population& newPOB) {
	newPOB.best = calculateBest(newPOB);
	if (newPOB.best.fitness > POB.best.fitness) {
		int random = randint(TAM_POB - 1);
		newPOB.members.at(random).fitness = POB.best.fitness;
		newPOB.members.at(random).board = POB.best.board;
		newPOB.best = POB.best;
	}
};

population initPopulation(dna& DNA, killer& RULES) {
	population POB;
	for (int i = 0; i < TAM_POB; i++) {
		dna newDNA = DNA;
		initGen(newDNA);
		calculate_fitness(newDNA, RULES);
		POB.members.push_back(newDNA);
	}
	setBest(POB, calculateBest(POB));
	return POB;
};

void initGen(dna& DNA) {
	int value;
	for (int row = 0; row < NONET_SIZE; row++) {
		for (int col = 0; col < NONET_SIZE; col++) {
			value = getAt(DNA, row, col);
			if (value == 0) {
				setAt(DNA, row, col, randintClose(DNA, row));
			}
		}
	}
};

void loadGame (std::string filename, dna& DNA, killer& RULES) {
	std::string line;
	std::ifstream file(filename);
	bool flag = true;
	int row, col;
	int id, total, value, gp;
	row = 0;
	while (std::getline(file, line)) {
		std::stringstream linestream(line);
		std::string data;
		std::size_t found = line.find("C");
		if (found!=std::string::npos) {
			if (flag){
				col = 0;
				while (std::getline(linestream, data, ' ')) {
					found = data.find("C");
					if (found!=std::string::npos) {
						if (data.substr(0, data.find("C"))=="."){
							value = 0;
						} 
						else {
							value = std::stoi(data.substr(0,data.find("C")));
							blockCell(DNA, row, col);
						}
						data.erase(0, data.find("C") + 1);
						setAt(DNA, row, col, value);
						col++;
					}
				}
			}
			else {
				line.erase(0,1);
				id = std::stoi(line.substr(0, line.find(":")));
				line.erase(0,line.find(":") + 1);
				total = std::stoi(line.substr(0, line.find(' ')));
				group newG;
				newG.total = total;
				newG.id = id;
				RULES.groups.push_back(newG);
			}
		}
		else {
			flag = false;
		}
		row++;
	}
	row = 0;
	flag = true;
	file.clear();
	file.seekg(0, file.beg);
	while (std::getline(file, line)) {
		std::stringstream linestream(line);
		std::string data;
		std::size_t found = line.find("C");
		if (found!=std::string::npos) {
			if (flag){
				col = 0;
				while (std::getline(linestream, data, ' ')) {
					found = data.find("C");
					if (found!=std::string::npos) {
						data.erase(0, data.find("C") + 1);
						gp = std::stoi(data.substr(0, data.find(' ')));
						coord position;
						position.x = col;
						position.y = row;
						RULES.groups.at(gp-1).members.push_back(position);
						col++;
					}
				}
			}
			else {
				continue;
			}
		}
		else {
			flag = false;
		}
		row++;
	}
};

void showGame(dna& DNA){
	std::cout << "_____________________________" << std::endl;
	for (int i = 0; i < BOARD_SIZE; i++) {
		if (i % 9 == 8) {
			std::cout << "| " << DNA.board.at(i).value << " |"  << std::endl;
			std::cout << "_____________________________" << std::endl;
		}
		else {
			std::cout << "| " << DNA.board.at(i).value;
		}
	}
};
