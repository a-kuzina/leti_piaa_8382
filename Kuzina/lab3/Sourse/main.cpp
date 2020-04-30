#include <iostream>

class graf {
	int edge_count;
	int vertex_count;
	char* vertex;
	char* alf_vertex;
	int** edges;

	int** tmp_flow;
	int** me;
	char from, to;
	int potok;
	std::string way;
	int* posible;

public:
	graf() {
		// Инициализация и обнуление всех полей класса графа
		std::cin >> edge_count;
		vertex_count = 0;
		potok = 0;
		vertex = new char[edge_count * 2];
		alf_vertex = new char[edge_count * 2];
		posible = new int[edge_count * 2];
		way = "";
		me = (int**)malloc(sizeof(int*) * edge_count * 2);
		edges = (int**)malloc(sizeof(int*) * edge_count * 2);
		
		tmp_flow = (int**)malloc(sizeof(int*) * edge_count * 2);

		for (int i = 0; i < edge_count * 2; i++) {
			vertex[i] = ' ';
			alf_vertex[i] = 'Z';
			me[i] = new int[edge_count * 2];
			edges[i] = new int[edge_count * 2];
			tmp_flow[i] = new int[edge_count * 2];
		
			posible[i] = 1;

		}
		for (int i = 0; i < edge_count * 2; i++) {
			for (int j = 0; j < edge_count * 2; j++) {
				tmp_flow[i][j] = 0;
			
				me[i][j] = 0;
				edges[i][j] = 0;
			}
		}

	}
	int is_new(char a) {
		// Вовращает номер вершины, если она не новая и -1 если вершина новая
		int i;
		for (i = 0; i < vertex_count; i++) {
			if (vertex[i] == a) {
				return i;
			}
			if (vertex[i] == ' ') break;
		}
		return -1;
	}
	int add_vertex(char a) {
		// добавляет вершину в список вершин с обычным порядком ввода и список вершин, рассортированный в алфавитном порядке
		int i = 0;
		while (vertex[i] != ' ') i++;
		vertex[i] = a;
		vertex_count++;
		if (vertex_count == 1) {
			alf_vertex[0] = a;
		}
		char c = a;
		for (int j = 0; j < vertex_count; j++) {
			if (a - alf_vertex[j] < 0) {
				c = alf_vertex[j];
				alf_vertex[j] = a;
				a = c;
			}
			else if (j == vertex_count - 1) {
				alf_vertex[j] = a;
			}
		}
		std::cout << "Add " << a << " at " << i << std::endl;
		return i;
	}
	bool new_connect(char first, char second, int way) {
		// Добавление ребра в матрицу ребер 
		int a, b;
		a = is_new(first);
		b = is_new(second);
		if (a == -1) a = add_vertex(first);
		if (b == -1) b = add_vertex(second);
		if (a == b) return 0;
		edges[a][b] = way;
		std::cout << "Edge " << first << second << " added to edge matrics with weight = " << way << "\n";
		return 1;
	}
	void init() {
		// считывание всех входных данных и запись по соответствующим полям
		std::cin >> from >> to;
		char first, second;
		int way = 0;
		int i = 0;
		while (i < edge_count) {
			std::cin >> first >> second >> way;
			new_connect(first, second, way);
			i++;
		}
		for (int i = 0; i < vertex_count; i++) {
			for (int j = 0; j < vertex_count; j++) {
				tmp_flow[i][j] = edges[i][j];
			}
		}
	}
	void alf_edges_print() {
		// Вывод ребер в алфавитном порядке
		int tmp;
		int to;
		for (int i = 0; i < vertex_count; i++) {
			tmp = is_new(alf_vertex[i]);
			for (int j = 0; j < vertex_count; j++) {
				to = is_new(alf_vertex[j]);
				if (tmp != to && edges[tmp][to] != 0) {
					std::cout << alf_vertex[i] << " " << alf_vertex[j] << " " << me[to][tmp] << "\n";
				}
			}

		}

	}
	void print_edges() {
		// вывод матрицы ребер на экран
		std::cout << "Edges:\n  ";
		for (int i = 0; i < vertex_count; i++) {
			std::cout <<"\t"<< vertex[i] << " ";
		}
		std::cout << "\n";

		for (int i = 0; i < vertex_count; i++) {
			std::cout <<  "\t" << vertex[i] << " ";
			for (int j = 0; j < vertex_count; j++) {
				std::cout <<  "\t" << tmp_flow[i][j] << " ";
			}
			std::cout << "\n";
		}
	}
	int step_back(int tmp) {
		//Если поиск пути зашел в тупик и не может найти сток, данная функция позволит откатиться и найти другой путь
		way.pop_back();
		posible[tmp] = 0; // Запрет прохождения по неудачному ребру


		if (way == "") return -1;
		int prev = is_new(way[way.length() - 1]); //просто получаем номер новой вершины
		tmp_flow[prev][tmp] = 0; 
		return prev;
	}
	void zeroing_posible() {
		//Разрешение прохождения по всем имеющимся ребрам
		for (int i = 0; i < vertex_count; i++) {
			posible[i] = 1;
		}
	}
	int maxflow(int minflow) {
		// Изменение весов для ребер, попавших в путь от истока до стока
		std::cout <<"Find path: " <<way << " with min weight = " << minflow << "\n";
		if (way == "") return 0;
		char ctmp = way[way.length() - 1];
		way.pop_back();
		char cprev;
		int tmp = is_new(ctmp);
		int prev;
		while (way != "") { //Пока не обработаем все ребра из представленных в пути
			cprev = way[way.length() - 1];
			way.pop_back();
			prev = is_new(cprev);
			
			std::cout << "Changed weight of edge " << vertex[prev] << vertex[tmp] << " from " << tmp_flow[prev][tmp];
			tmp_flow[prev][tmp] = tmp_flow[prev][tmp] - minflow; //Уменьшаем исходное ребро на вес получившегося потока
			std::cout << " to "<< tmp_flow[prev][tmp] << "\n";
			std::cout << "Changed weight of edge " << vertex[tmp] << vertex[prev] << " to " << tmp_flow[tmp][prev] << "\n";
			tmp_flow[tmp][prev] = tmp_flow[tmp][prev] + minflow; //Увеличиваем вес обратного ребра на вес потока
			std::cout << " to " << tmp_flow[tmp][prev] << "\n";


			me[tmp][prev] += minflow; //это нужно для корректной работы с графами у которых есть двунаправленные ребра
			if(me[prev][tmp] > 0)
				me[prev][tmp] -= minflow;
			tmp = prev;
		}
	}
	int serch() {
		int next = 1;
		int min = 0;
		int tmp = is_new(from);
		way = vertex[tmp];
		int prevflow = 9999;
		int tmpflow = 9999;
		int h = 0;
		while (next != -1) {

			min = 0;
			next = -1;
			std::cout <<"Vertex "<< vertex[tmp] << " is watching now\n";
			for (int i = 0; i < vertex_count; i++) { // обход всех ребер для выбранной вершины
				if (tmp_flow[tmp][i] > min&& posible[i]) {
					// выбор ребра с наибольшим весом
					next = i;
					min = tmp_flow[tmp][i];
				}
			}
			if (next != -1) { // следующая вершина найдена - продолжаем поиск пути
				std::cout << "Next vertex will be " << vertex[next] << "\n";
				posible[tmp] = 0;
				if (min > prevflow) {
					tmpflow = prevflow;
					prevflow = tmpflow;
				}
				else {
					tmpflow = min;
					prevflow = tmpflow;
				}
				tmp = next;
				way.push_back(vertex[next]); // добавление в путь
			}
			if (next == -1) { // больше идти некуда - пробуем откатиться
				tmp = step_back(tmp);
				next = 1;
				if (tmp == -1) { // откат не удался - граф рассмотрен полностью, больше путей нет
					std::cout << "Can't find any more paths. Here is results:"<< "\n";
					std::cout << potok << "\n";
					return 0;
				}
				std::cout << "Next vertex will be " << vertex[tmp] << "\n"; // откат удался, продолжаем поиск пути
			}
			if (next == is_new(to)) { // нашли какой-то путь
				potok += tmpflow; 
				maxflow(tmpflow); //изменяем данные о графе
				tmpflow = 9999;
				prevflow = 9999;
				std::cout << "Adges after finding this path: \n";
				print_edges();

				way = from;
				zeroing_posible(); // разрешаем переход по всех ребрам
				tmp = is_new(from); // начинаем поиск пути заного
				next = 1;
			}
		}

	}
};



int main() {
	graf* a = new graf;
	a->init();
	a->print_edges();
	a->serch();
	a->alf_edges_print();

}
