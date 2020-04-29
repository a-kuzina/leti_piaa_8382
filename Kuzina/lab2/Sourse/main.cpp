#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

class graf {
	char* vertex;
	int count;
	int actual_count;
	char from;
	char to;
	int* evrist;

	int* num_way;
	std::string* way;
	
	int open_count;
	int* open_vertex;
	double** edges;
	int** pos;
	std::string answ;

public:
	graf() {
		//Выделение памяти под все структуры и задание в них необходимых "нулевых" значений
		count = 66;
		actual_count = 0;
		vertex = new char[count];
		open_vertex = new int[count];
		way = new std::string[count];
		num_way = new int[count];
		evrist = new int[count];

		open_count = 0;
		answ = "";
		edges = (double**)malloc(sizeof(double*) * count);
		pos = (int**)malloc(sizeof(int*) * count);
		for (int i = 0; i < count; i++) {
			vertex[i] = ' ';
			open_vertex[i] = 0;
			way[i] = "";
			num_way[i] = 0;
			evrist[i] = 0;
		}
		for (int i = 0; i < count; i++) {
			edges[i] = new double[count];
			pos[i] = new int[count];
		}
		for (int i = 0; i < count; i++) {
			for (int j = 0; j < count; j++) {
				edges[i][j] = 0;
				pos[i][j] = 1;
			}
		}

	}
	int is_new(char a) {
		// проверка на то, является ли заданная вершина новой или уже существующей
		int i;
		for (i = 0; i < count; i++) {
			if (vertex[i] == a) {
				return i;
			}
			if (vertex[i] == ' ') break;
		}
		return -1;
	}
	int add_vertex(char a) {
		// добавление новой вершины в список вершин
		int i = 0;
		while (vertex[i] != ' ') i++;
		vertex[i] = a;
		actual_count++;
		std::cout << "Add new vertex " << a << " at " << i << std::endl;
		return i;
	}
	bool new_connect(char first, char second, float way) {
		// добавление ребра в матрицу ребер
		int a, b;
		if (first == second) return 0;
		a = is_new(first);
		b = is_new(second); // проверка на то, разные ли вершины и новые ли они
		if (a == -1) a = add_vertex(first);
		if (b == -1) b = add_vertex(second);
		if (a == b) return 0;
		std::cout << "Add new adge: " << first << second << ": " << way << std::endl;
		edges[a][b] = way;
		return 1;
	}
	void init() {
		// Инициализация графа: заполнение матрицы ребер, массива вершин, а также задание эвристических значений для вершин.
		// 
		scanf("%c %c\n", &from, &to);
		char first, second;
		int evr = 0;
		float way = 0;
		int enter = 3;
		int i = 0;
		char c;
		while (true) {
			// считывание  идет: вершина начала ребра, вершина конца ребра, длина ребра, значение эвристической функции для вершины конца ребра
			std::cin >> first >> second >> way >> evr;
			enter = scanf("%c", &c);
			//cout << first << second << way << " " << evr << endl;
			new_connect(first, second, way);
			i = is_new(second);
			evrist[i] = evr;
			if (enter != 1) break;
			if (c == 'q') break;

		}
	}
	void print() {
		// Вывод на экран матрицы ребер
		std::cout << "edge matrix:\n  ";
		for (int i = 0; i < actual_count; i++) {
			std::cout << vertex[i] << " ";
		}
		std::cout << "\n";

		for (int i = 0; i < actual_count; i++) {
			std::cout << vertex[i] << " ";
			for (int j = 0; j < actual_count; j++) {
				std::cout << edges[i][j] << " ";
			}
			std::cout << "\n";
		}
	}
	int is_closed(int i) {
		//Возвращает 1, если вершина i еще не добавлена в список доступных для перехода вершин. Иначе 0.
		for (int j = 0; j < open_count; j++) {
			if (open_vertex[j] == i) return 0;
		}
		return 1;
	}
	void change(int tmp, int old_num, int new_num, std::string new_way) {
		std::cout << "Change all ways that include " << vertex[tmp] << "\nBefore: \n";
		print_ways();
		char u = vertex[tmp];
		// обновляем пути для всех вершин, пути которых содержали в себе обновленную вершину
		std::string change = "";
		int n = 0;
		int len = 0;
		for (int i = 0; i < actual_count; i++) { // обход всех вершин
			change = "";
			// cout << way[i] <<endl;
			n = way[i].find(u);
			len = way[i].size();
			if (n != std::string::npos) { // если в пути содержится искомая вершина - заменяем все до нее на ее новый кратчайший путь
				num_way[i] = num_way[i] - old_num + new_num;

				for (int j = n; j < len - 1; j++) {
					change += way[i][n + j];
				}
				way[i] = new_way;
				way[i] += change;

			}
			
		}
		std::cout << "After: \n";
			print_ways();

	}
	void reboot(int tmp, int next) {
		if (next == is_new(from)) return;
		if (num_way[next] == 0) { // если в вершину мы пришли первый раз, то:
			num_way[next] = num_way[tmp] + edges[tmp][next];
			way[next] = way[tmp];
			way[next].push_back(vertex[next]);// задаем вершине кратчайшие длину и путь
			print_ways();
		}
		else if (num_way[next] > num_way[tmp] + edges[tmp][next]) { // если мы пришли в вершину более коротким путем, чем раньше, то:

			way[next] = way[tmp];
			way[next].push_back(vertex[next]);
			std::cout << "New min way for vertex " << vertex[next] << ": " << way[next] << std::endl;
			change(next, num_way[next], num_way[tmp] + edges[tmp][next], way[next]); // обновляем пути для всех вершин, содержащих данную
			num_way[next] = num_way[tmp] + edges[tmp][next]; // обновляем кратчайшие длину и путь для даной вершины
		}

	}

	void print_ways() {
		// Для каждой вершины выводит кратчайший на данный момент путь от начальной. (Если путь уже найден)
		std::cout << "Ways: \n";
		for (int i = 0; i < actual_count; i++) {
			std::cout <<"\t"<< vertex[i] << " way: " << way[i] << std::endl;
		}
		std::cout << std::endl;
		return;
	}
	int find_next() {

		float min = 666;
		float c = 0;
		int next = -1;
		int tmp = 0;

		for (int i = 0; i < open_count; i++) { // Обход всех доступных к переходу вершин
			std::cout << "View vertex: " << vertex[open_vertex[i]] << "\n";
			for (int j = 0; j < actual_count; j++) {
				if (edges[open_vertex[i]][j] > 0 && pos[open_vertex[i]][j]) {
					// если ребро между вершинами существует и этот переход еще не совершался :
					c = edges[open_vertex[i]][j] + evrist[j] + num_way[open_vertex[i]];// вычисление "веса" перехода по данному ребру: длина ребра + сумма уже пройденого пути (для вершины из которой мы идем) + значение эвристической фуункции для вершины в которую мы идем.
					std::cout << "\tweight of edge " << vertex[open_vertex[i]] << vertex[j] << " = " << c << std::endl;
					if (c <= min) {
						min = c;
						next = j;
						tmp = open_vertex[i]; // устанавливаем временный минимум для данного шага
					}
				}
			}
		}
		pos[tmp][next] = 0;
		if (is_closed(next)) { // если вершина нам еще не встречалась, добавляем в список доступных для переходов вершин
			open_vertex[open_count] = next;
			open_count++;
		}
		if (next != -1) {
			std::cout << "Step from " << vertex[tmp] << " to " << vertex[next] << std::endl;
			reboot(tmp, next);
			
		}//обновляем значение вершины в которую мы перешли
		return next;
	}

	void serch() {
		std::cout << "Find way from " << from << " to " << to << "\n\n";
		int tmp = is_new(from);
		int end = is_new(to);
		evrist[end] = 0;
		int check = 0;
		int flag = 0;

		if (tmp == -1 || end == -1) { // проверка на то, существуют ли исходная и конечная вершины в списке вершин
			std::cout << "There is no way!\n";
			return;
		}
		open_vertex[0] = tmp;
		open_count++;
		way[tmp] = from;
		int next = find_next();
		while (next != -1) { // пока не найден путь или не пройден весь граф
			if (next == end) {
				std::cout << "Answer: " << way[end] << std::endl;
				return;
			}

			next = find_next(); // поиск следующей вершины
		}
		std::cout << "There is no way!\n";
		return;
	}

};


int main() {

	graf* one = new graf;
	one->init();
	one->print();
	one->serch();

	return 0;
}