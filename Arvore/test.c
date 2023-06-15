#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct DirectoryNode {
    char name[100];
    struct DirectoryNode *parent;
    struct DirectoryNode *child;
    struct DirectoryNode *sibling;
};

struct DirectoryNode* createDirectoryNode(const char* name, struct DirectoryNode* parent) {
    struct DirectoryNode* newNode = (struct DirectoryNode*)malloc(sizeof(struct DirectoryNode));
    strcpy(newNode->name, name);
    newNode->parent = parent;
    newNode->child = NULL;
    newNode->sibling = NULL;
    return newNode;
}

struct DirectoryNode* findDirectory(struct DirectoryNode* currentDirectory, const char* name) {
    struct DirectoryNode* node = currentDirectory->child;
    while (node != NULL) {
        if (strcmp(node->name, name) == 0) {
            return node;
        }
        node = node->sibling;
    }
    return NULL;
}

struct DirectoryNode* changeDirectory(struct DirectoryNode* currentDirectory, const char* name) {
    if (strcmp(name, "..") == 0) {
        return currentDirectory->parent;
    } else {
        struct DirectoryNode* directory = findDirectory(currentDirectory, name);
        if (directory != NULL) {
            return directory;
        } else {
            printf("Diretório '%s' não encontrado.\n", name);
            return currentDirectory;
        }
    }
}

void createDirectory(struct DirectoryNode* currentDirectory, const char* name) {
    struct DirectoryNode* newDirectory = createDirectoryNode(name, currentDirectory);
    if (currentDirectory->child == NULL) {
        currentDirectory->child = newDirectory;
    } else {
        struct DirectoryNode* node = currentDirectory->child;
        while (node->sibling != NULL) {
            node = node->sibling;
        }
        node->sibling = newDirectory;
    }
    printf("Diretório '%s' criado.\n", name);
}

void deleteDirectory(struct DirectoryNode* currentDirectory, const char* name) {
    struct DirectoryNode* directory = findDirectory(currentDirectory, name);
    if (directory != NULL) {
        if (currentDirectory->child == directory) {
            currentDirectory->child = directory->sibling;
        } else {
            struct DirectoryNode* node = currentDirectory->child;
            while (node->sibling != directory) {
                node = node->sibling;
            }
            node->sibling = directory->sibling;
        }
        free(directory);
        printf("Diretório '%s' excluído.\n", name);
    } else {
        printf("Diretório '%s' não encontrado.\n", name);
    }
}

void listDirectories(struct DirectoryNode* currentDirectory) {
    struct DirectoryNode* node = currentDirectory->child;
    printf("Diretórios dentro de '%s':\n", currentDirectory->name);
    while (node != NULL) {
        printf("- %s\n", node->name);
        node = node->sibling;
    }
}

void listAllDirectories(struct DirectoryNode* currentDirectory, int level) {
    for (int i = 0; i < level; i++) {
        printf("\t");
    }
    printf("%s\n", currentDirectory->name);
    
    struct DirectoryNode* node = currentDirectory->child;
    while (node != NULL) {
        listAllDirectories(node, level + 1);
        node = node->sibling;
    }
}

void showHelp() {
    printf("Palavras reservadas:\n");
    printf("  'mudar_diretorio' ou 'md': Navega entre os diretórios criados.\n");
    printf("  'criar_diretorio' ou 'crd': Cria um diretório.\n");
    printf("  'excluir_diretorio' ou 'exd': Exclui um diretório.\n");
    printf("  'listar_diretorios' ou 'ld': Lista todos os diretórios dentro do diretório atual.\n");
    printf("  'listar_completo' ou 'lc': Lista todos os diretórios a partir do diretório raiz, exibindo a memória alocada em bits para cada um.\n");
    printf("  'socorro' ou 'scr': Mostra todas as palavras reservadas e suas respectivas funções.\n");
    printf("  'testar': Cria automaticamente os diretórios de teste.\n");
    printf("  'sair' ou 'x': Encerra o programa.\n");
}

void createTestDirectories(struct DirectoryNode* rootDirectory) {
    // Create test directories inside root
    createDirectory(rootDirectory, "teste1");
    createDirectory(rootDirectory, "teste2");
    createDirectory(rootDirectory, "teste3");

    // Create test directories inside teste1
    struct DirectoryNode* teste1Directory = findDirectory(rootDirectory, "teste1");
    createDirectory(teste1Directory, "teste11");
    createDirectory(teste1Directory, "teste12");
    createDirectory(teste1Directory, "teste13");

    // Create test directories inside teste2
    struct DirectoryNode* teste2Directory = findDirectory(rootDirectory, "teste2");
    createDirectory(teste2Directory, "teste21");
    createDirectory(teste2Directory, "teste22");

    // Create test directory inside teste3
    struct DirectoryNode* teste3Directory = findDirectory(rootDirectory, "teste3");
    createDirectory(teste3Directory, "teste31");

    printf("Diretórios de teste criados.\n");
}

int main() {
    struct DirectoryNode* rootDirectory = createDirectoryNode("root", NULL);
    struct DirectoryNode* currentDirectory = rootDirectory;

    char command[100];
    char parameter[100];

    while (1) {
        printf("\nDigite um comando ou o comando 'scr' para ver a lista de comandos disponíveis: ");
        scanf("%s", command);

        if (strcmp(command, "mudar_diretorio") == 0 || strcmp(command, "md") == 0) {
            scanf("%s", parameter);
            currentDirectory = changeDirectory(currentDirectory, parameter);
        } else if (strcmp(command, "criar_diretorio") == 0 || strcmp(command, "crd") == 0) {
            scanf("%s", parameter);
            createDirectory(currentDirectory, parameter);
        } else if (strcmp(command, "excluir_diretorio") == 0 || strcmp(command, "exd") == 0) {
            scanf("%s", parameter);
            deleteDirectory(currentDirectory, parameter);
        } else if (strcmp(command, "listar_diretorios") == 0 || strcmp(command, "ld") == 0) {
            listDirectories(currentDirectory);
        } else if (strcmp(command, "listar_completo") == 0 || strcmp(command, "lc") == 0) {
            printf("\n");
            listAllDirectories(rootDirectory, 0);
        } else if (strcmp(command, "socorro") == 0 || strcmp(command, "scr") == 0) {
            showHelp();
        } else if (strcmp(command, "testar") == 0) {
            createTestDirectories(rootDirectory);
        } else if (strcmp(command, "sair") == 0 || strcmp(command, "x") == 0) {
            break;
        } else {
            printf("Comando inválido. Digite 'socorro' para obter ajuda.\n");
        }
    }

    return 0;
}