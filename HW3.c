#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void write_dot(FILE* fname, int** mas, int size)         //Создание файла на языке DOT.
{
	fprintf(fname, "graph G{\n");

	for (int i = 0; i < size; i++)
		fprintf(fname, "%d;\n", i);

	for (int j = 0; j < size; j++) {
		for (int i = j; i < size; i++)
			for (int k = 0; k < mas[i][j]; k++)
				fprintf(fname, "%d -- %d;\n", j, i);
	}
	fprintf(fname, "}");

	fclose(fname);
}

int main(void) {
	int sizeM;
	printf("Введите размер квадратной матрицы\n");
	scanf("%d", &sizeM);												//Пользователь вводит размер матрицы.
	printf("\nВведите строки вашей матрицы. При нажатии \"ENTER\" вы перейдёте на новый столбец.\n");
	printf("Учтите, что данная матрица является матрицой смежности, поэтому должна быть симметричной.\n");
	printf("Если не указать число, оно будет равнно '0'.\n");
	getchar();
	int** mas; //= NULL;
	mas = (int**)malloc(sizeof(int*) * sizeM);
	for (int i = 0; i < sizeM; i++) {
		mas[i] = (int*)malloc(sizeof(int) * sizeM);
	}

	char* s, c;

	s = (char*)malloc(sizeof(char) * CHAR_MAX);

	for (int i = 0, m = 0, j = 0; j < sizeM; j++) {				// Заполненение массива числами. 

		while ((c = getchar()) != '\n') {

			if (c != ' ')
				s[m++] = c;

			else {
				s[m] = '\0';

				if (i >= sizeM) {
					printf("ОШИБКА: ширина матрицы превышает введенное ранее значение!\n ");
					return -1;
				}

				mas[i][j] = atoi(s);
				i++;
				free(s);
				s = (char*)malloc(sizeof(char) * CHAR_MAX);
				m = 0;

			}

		}

		s[m] = '\0';
		m = 0;

		if (i >= sizeM) {
			printf("ОШИБКА: ширина матрицы превышает введенное ранее значение!\n ");
			return -1;
		}

		mas[i][j] = atoi(s);

		if (i < sizeM) {            							 // Замена числа на 0, если оно не введено.
			++i;
			while (i < sizeM)
				mas[i++][j] = 0;
		}

		i = 0;
		free(s);
		s = (char*)malloc(sizeof(char) * CHAR_MAX);
	}
	free(s);

	FILE* fout;
	fout = fopen("graph_before.gv", "w");  						//Открытие файла и запись.
	write_dot(fout, mas, sizeM);
    system("sudo dot -Tjpg graph_before.gv -o file_before.jpg"); //Получаем граф до удаления ребра.

	int i, j, a, b;
	printf("Введите номера вершин через \"SPACE\", между которыми хотите удалить ребро\n");
	scanf("%d %d", &i, &j);										// Считываем вершины, которые нужно удалить.

	if (i >= sizeM) {
		printf("ОШИБКА: первая вершина не существует!\n");
		return 0;
		}

	if (j >= sizeM) {
		printf("ОШИБКА: первая вершина не существует!\n");
		return 0;
		}

	a = mas[i][j];
	b = a - 1;													//Удаление ребра.
	mas[i][j] = b;
	mas[j][i] = b;
	
	if (b < 0) 	{
		printf("ОШИБКА: данного ребра не существует!\n");
		return 0;
	}

	int* mass_top;
	int* mass_name;
	mass_top = (int*)malloc(sizeM * sizeof(int));				//Выделяем память под 2 массива - номера вершины и её степени.
	mass_name = (int*)malloc(sizeM * sizeof(int));

	for (int i = 0; i < sizeM; i++) {								//Подсчет степени вершины.
		int sum = 0;
		
		for (int j = 0; j < sizeM; j++) {
				
				if ((i == j) && (mas[i][j])) {
					sum += mas[i][j] * 2;
				}

				else {
					sum += mas[i][j];
				}
		}

		mass_top[i] = sum;					
		mass_name[i] = i;
	}

	for (int i = 1; i < sizeM; i++)	{							//Сортировка вершин алгоритмом "пузырька".
	
		for (int j = 0; j < (sizeM - 1); j++) {
		
			if (mass_top[j] > mass_top[j + 1]) {
			
				int temp = mass_top[j];
				mass_top[j] = mass_top[j + 1];
				mass_top[j + 1] = temp;
				
				int index = mass_name[j];
				mass_name[j] = mass_name[j + 1];
				mass_name[j + 1] = index;
			}
		}
	}


	for (int i = 0; i < sizeM; i++) {
	
		printf("Вершина:%d степень:%d \n", mass_name[i], mass_top[i]); //Вывод вершин, отсортированых по возрастанию степени вершины.

	}

	fout = fopen("graph_after.gv", "w");  
	write_dot(fout, mas, sizeM);	
	system("sudo dot -Tjpg graph_after.gv -o file_after.jpg");	//Получаем граф после удаления ребра.

	for (int i = 0; i < sizeM; i++) {
	
		free(mas[i]);
	}

	free(mas);
	free(mass_top);
	free(mass_name);
	
	return 0;
}

