#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Car {
    char placa[8];
    char marca[50];
    int ano;
    struct Car* next;
} Car;

typedef struct TreeNode {
    Car* car;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

Car* carList = NULL;
TreeNode* plateTree = NULL;
TreeNode* brandTree = NULL;
TreeNode* yearTree = NULL;

Car* createCar(char placa[], char marca[], int ano) {
    Car* newCar = (Car*) malloc(sizeof(Car));
    strcpy(newCar->placa, placa);
    strcpy(newCar->marca, marca);
    newCar->ano = ano;
    newCar->next = NULL;
    return newCar;
}

TreeNode* createNode(Car* car) {
    TreeNode* newNode = (TreeNode*) malloc(sizeof(TreeNode));
    newNode->car = car;
    newNode->left = newNode->right = NULL;
    return newNode;
}

TreeNode* insertByPlate(TreeNode* root, Car* car) {
    if (root == NULL) {
        return createNode(car);
    }

    if (strcmp(car->placa, root->car->placa) < 0) {
        root->left = insertByPlate(root->left, car);
    } else if (strcmp(car->placa, root->car->placa) > 0) {
        root->right = insertByPlate(root->right, car);
    }

    return root;
}

TreeNode* insertByBrand(TreeNode* root, Car* car) {
    if (root == NULL) {
        return createNode(car);
    }

    if (strcmp(car->marca, root->car->marca) < 0) {
        root->left = insertByBrand(root->left, car);
    } else {
        root->right = insertByBrand(root->right, car);
    }

    return root;
}

TreeNode* insertByYear(TreeNode* root, Car* car) {
    if (root == NULL) {
        return createNode(car);
    }

    if (car->ano < root->car->ano) {
        root->left = insertByYear(root->left, car);
    } else {
        root->right = insertByYear(root->right, car);
    }

    return root;
}

void insertCar(char placa[], char marca[], int ano) {
    Car* newCar = createCar(placa, marca, ano);

    if (carList == NULL || strcmp(newCar->placa, carList->placa) < 0) {
        newCar->next = carList;
        carList = newCar;
    } else {
        Car* current = carList;
        while (current->next && strcmp(newCar->placa, current->next->placa) > 0) {
            current = current->next;
        }
        newCar->next = current->next;
        current->next = newCar;
    }

    plateTree = insertByPlate(plateTree, newCar);
    brandTree = insertByBrand(brandTree, newCar);
    yearTree = insertByYear(yearTree, newCar);
}

Car* searchByPlate(TreeNode* root, char placa[]) {
    if (root == NULL) {
        return NULL;
    }
    if (strcmp(placa, root->car->placa) == 0) {
        return root->car;
    } else if (strcmp(placa, root->car->placa) < 0) {
        return searchByPlate(root->left, placa);
    } else {
        return searchByPlate(root->right, placa);
    }
}

void listByPlate(Car* list) {
    while (list) {
        printf("Placa: %s, Marca: %s, Ano: %d\n", list->placa, list->marca, list->ano);
        list = list->next;
    }
}

void inOrderTraversal(TreeNode* root) {
    if (root) {
        inOrderTraversal(root->left);
        printf("Placa: %s, Marca: %s, Ano: %d\n", root->car->placa, root->car->marca, root->car->ano);
        inOrderTraversal(root->right);
    }
}

void listByBrand() {
    inOrderTraversal(brandTree);
}

void listByYear() {
    inOrderTraversal(yearTree);
}

void deleteFromList(Car* car) {
    if (!carList) return;

    if (carList == car) {
        Car* temp = carList;
        carList = carList->next;
        free(temp);
        return;
    }

    Car* prev = carList;
    while (prev->next && prev->next != car) {
        prev = prev->next;
    }

    if (prev->next == car) {
        Car* temp = prev->next;
        prev->next = prev->next->next;
        free(temp);
    }
}

TreeNode* deleteFromTree(TreeNode* root, Car* car, int (*comparator)(Car*, Car*)) {
    if (!root) return NULL;

    if (comparator(car, root->car) < 0) {
        root->left = deleteFromTree(root->left, car, comparator);
    } else if (comparator(car, root->car) > 0) {
        root->right = deleteFromTree(root->right, car, comparator);
    } else {
        if (!root->left) {
            TreeNode* temp = root->right;
            free(root);
            return temp;
        } else if (!root->right) {
            TreeNode* temp = root->left;
            free(root);
            return temp;
        }

        TreeNode* temp = root->right;
        while (temp->left) temp = temp->left;
        root->car = temp->car;
        root->right = deleteFromTree(root->right, temp->car, comparator);
    }

    return root;
}

int compareByPlate(Car* a, Car* b) {
    return strcmp(a->placa, b->placa);
}

int compareByBrand(Car* a, Car* b) {
    return strcmp(a->marca, b->marca);
}

int compareByYear(Car* a, Car* b) {
    return a->ano - b->ano;
}

void deleteCar(char placa[]) {
    Car* carToDelete = searchByPlate(plateTree, placa);
    if (!carToDelete) {
        printf("Carro não encontrado!\n");
        return;
    }

    deleteFromList(carToDelete);
    plateTree = deleteFromTree(plateTree, carToDelete, compareByPlate);
    brandTree = deleteFromTree(brandTree, carToDelete, compareByBrand);
    yearTree = deleteFromTree(yearTree, carToDelete, compareByYear);
}

int main() {
    int choice;

    do {
        printf("\nMENU:\n");
        printf("1. Inserir Carro\n");
        printf("2. Listar por Placa\n");
        printf("3. Listar por Marca\n");
        printf("4. Listar por Ano\n");
        printf("5. Buscar por Placa\n");
        printf("6. Deletar Carro\n");
        printf("7. Sair\n");
        printf("Digite sua escolha: ");
        scanf("%d", &choice);

        char placa[8], marca[50];
        int ano;

        switch (choice) {
            case 1:
                printf("Digite a placa: ");
                scanf("%s", placa);
                printf("Digite a marca: ");
                scanf("%s", marca);
                printf("Digite o ano: ");
                scanf("%d", &ano);
                insertCar(placa, marca, ano);
                break;
            case 2:
                listByPlate(carList);
                break;
            case 3:
                listByBrand();
                break;
            case 4:
                listByYear();
                break;
            case 5:
                printf("Digite a placa para buscar: ");
                scanf("%s", placa);
                Car* searchedCar = searchByPlate(plateTree, placa);
                if (searchedCar) {
                    printf("Carro encontrado: Placa: %s, Marca: %s, Ano: %d\n", searchedCar->placa, searchedCar->marca, searchedCar->ano);
                } else {
                    printf("Carro não encontrado!\n");
                }
                break;
            case 6:
                printf("Digite a placa do carro a ser deletado: ");
                scanf("%s", placa);
                deleteCar(placa);
                break;
            case 7:
                printf("Saindo...\n");
                break;
            default:
                printf("Escolha inválida!\n");
                break;
        }
    } while (choice != 7);

    return 0;
}
