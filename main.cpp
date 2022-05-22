#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int is_sorted = 0;

struct item{ // структура элемента таблицы, содержит ключ и строку
    int key;
    char *string;
};

int get_size(FILE *input){ // считает количество элементов в таблице
    int n = 0;
    char* buffer = new char[128];
    size_t bfsize = sizeof(buffer);
    while(!feof(input)){
        getline(&buffer, &bfsize, input);
        n++;
    }
    delete [] buffer;
    rewind(input);
    return n;
}

void print_table(item *table, int size){
    printf("----------------------------------------------------------\n");
    printf("| Key |                       Data     |\n");
    printf("-----------------------------------------------------------\n");
    for(int i = 0; i<size; ++i){
        printf("|%3d  | %s", table[i].key, table[i].string);
        printf("-----------------------------------------------------------\n");
    }
    printf("\n");
}

item * make_table(FILE *keys, FILE *data, int size){
    item * table = new item[size];
    for (int i = 0; i < size; ++i){
        table[i].string = new char[128];
    }
    for (int i = 0; i < size; ++i){
        fscanf(keys, "%d", &table[i].key);
        fgets(table[i].string, 128, data);
    }
    return table;
}

void swap_strings(item * table, int a, int b){ // меняет местами строки с данными ключами
    item tmp;
    tmp = table[a];
    table[a] = table[b];
    table[b] = tmp;
}

void bubbleSort(item *table, int size){ // метод простой вставки (пузырьковая сортировка)
    int i, j;
    for (i = 0; i < size-1; i++){
        for (j = i+1; j > 0; j--){
            if (table[j-1].key > table[j].key){
                swap_strings(table, j-1, j);
            }
            else break;
        }
    }
}
void sort_table(item * table, int size){
    is_sorted = 1;
    bubbleSort(table, size);
}

void mix_table(item * table, int size){ // перемешивает строки в таблице
    is_sorted = 0;
    int a, b;
    for(int i = 0; i<size; ++i){
        a = rand() % size;
        b = rand() % size;
        swap_strings(table, a, b);
    }
}

int bin_search(item * table, int key, int size){ // бинарный поиск строки по ключу
    int l = 0, r = size - 1, mid;
    while (l<=r){
        mid = (r + l)/2;
        if (key == table[mid].key){
            return mid;
        }
        else if(key < table[mid].key){
            r = mid;
        }
        else if(key > table[mid].key){
            l = mid+1;
        }
    }
    return -1;
}

int main(){
    srand(time(0));
    FILE* keysFile = fopen("keys.txt", "r");
    if (keysFile == NULL){
        printf("Can't open keys list!\n");
        return 0;
    }
    FILE* stringsFile = fopen("strings.txt", "r");
    if (stringsFile == NULL){
        printf("Can't open strings list!\n");
        return 0;
    }
    int n = get_size(keysFile);
    int action;
    item *table = make_table(keysFile, stringsFile, n);
    fclose(keysFile);
    fclose(stringsFile);
    while(1){
        printf("Menu\n");
        printf("1) Print table\n");
        printf("2) Binary search\n");
        printf("3) Do a bubble sort\n");
        printf("4) Mix table\n");
        printf("5) Exit\n");
        printf("Choose an action: ");
        scanf("%d", &action);
        switch (action){
            case 1:{
                print_table(table, n);
                break;
            }
            case 2:{
                if(is_sorted){
                    printf("Enter a key: ");
                    int k;
                    scanf("%d", &k);
                    int search;
                    printf("\n");
                    search = bin_search(table, k, n);
                    if (search == -1){
                        printf("There is no element with that key in a table.\n");
                        break;
                    }
                    printf("String:\n %s", table[search].string);
                }
                else printf("Please sort the table before searching.\n");
                break;
            }
            case 3:{
                sort_table(table, n);
                break;
            }
            case 4:{
                mix_table(table, n);
                break;
            }
            case 5: return 0;
        }
    }
}