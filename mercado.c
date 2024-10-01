#include <stdio.h>
#include <string.h>

#define MP 50
#define MC 50

// Estrutura para armazenar os produtos
typedef struct {
    int codigo;
    char nome[30];
    float preco;
} Produto;

// Estrutura para armazenar os produtos no carrinho
typedef struct {
    Produto produto;
    int quantidade;
} Carrinho;

// Variáveis globais para os produtos e o carrinho
Produto produtos[MP];
Carrinho carrinho[MC];
int nP = 0;
int nC = 0;

// Protótipos das funções
void Clean();
int entnumi(char* mensagem);
float entnumf(char* mensagem);
void entcar(char* destino, int tamanho, char* mensagem);
void menu();
void cadastrarProduto();
void listarProdutos();
int pegarProdutoPorCodigo(int codigo);
int temNoCarrinho(int codigo);
void comprarProduto();
void visualizarCarrinho();
void removerProdutoCarrinho();  
void fecharPedido();
void pausar();  
int nomeProdutoExiste(char* nome);  

// Função para limpar o buffer de entrada
void Clean() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Função para ler um número inteiro validando a entrada
int entnumi(char* mensagem) {
    int valor;
    while (1) {
        printf("%s", mensagem);
        if (scanf("%d", &valor) == 1 && valor >= 0) {
            Clean();
            return valor;
        } else {
            Clean();
            printf("Entrada invalida! Por favor, insira um numero inteiro valido.\n");
        }
    }
}

// Função para ler um número float validando a entrada
float entnumf(char* mensagem) {
    float valor;
    while (1) {
        printf("%s", mensagem);
        if (scanf("%f", &valor) == 1 && valor >= 0.0) {
            Clean();
            return valor;
        } else {
            Clean();
            printf("Entrada invalida! Por favor, insira um valor numerico valido.\n");
        }
    }
}

// Função para ler uma string validando o comprimento
void entcar(char* destino, int tamanho, char* mensagem) {
    printf("%s", mensagem);
    fgets(destino, tamanho, stdin);
    destino[strcspn(destino, "\n")] = '\0'; 
}

// Função para pausar a execução até que o usuário pressione Enter
void pausar() {
    printf("\nPressione Enter duas vezes para continuar...");
    Clean();  
    getchar();  
}

// Função para exibir o menu
void menu() {
    int opcao;
    do {
        printf("\n=============================\n");
        printf("\n=== Mercadinho Sal Grosso ===\n");
        printf("\n=============================\n");
        printf("1. Cadastrar Produto\n");
        printf("2. Listar Produtos\n");
        printf("3. Comprar Produto\n");
        printf("4. Visualizar Carrinho\n");
        printf("5. Remover Produto do Carrinho\n"); 
        printf("6. Fechar Pedido\n");
        printf("7. Sair\n");
        opcao = entnumi("\nEscolha uma opcao: ");

        switch(opcao) {
            case 1:
                cadastrarProduto();
                break;
            case 2:
                listarProdutos();
                break;
            case 3:
                comprarProduto();
                break;
            case 4:
                visualizarCarrinho();
                break;
            case 5:
                removerProdutoCarrinho(); 
                break;
            case 6:
                fecharPedido();
                break;
            case 7:
                printf("Saindo...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while(opcao != 7);
}

// Função para verificar se o nome do produto já existe
int nomeProdutoExiste(char* nome) {
    for (int i = 0; i < nP; i++) {
        if (strcmp(produtos[i].nome, nome) == 0) {
            return 1;  
        }
    }
    return 0;  
}

// Função para cadastrar novos produtos
void cadastrarProduto() {
    if(nP < MP) {
        Produto novoProduto;
        novoProduto.codigo = entnumi("\nCodigo do produto: ");
        
        if (pegarProdutoPorCodigo(novoProduto.codigo) != -1) {
            printf("\nJa existe um produto cadastrado com este codigo.\n");
            pausar();  
            return;
        }

        entcar(novoProduto.nome, sizeof(novoProduto.nome), "\nNome do produto: ");

        if (nomeProdutoExiste(novoProduto.nome)) {
            printf("\nJa existe um produto cadastrado com este nome.\n");
            pausar();  
            return;
        }

        novoProduto.preco = entnumf("\nPreco do produto: ");
        produtos[nP] = novoProduto;
        nP++;

        printf("\nProduto cadastrado com sucesso!\n");
    } else {
        printf("\nLimite de produtos atingido!\n");
    }
    pausar();  
}

// Função para listar todos os produtos cadastrados
void listarProdutos() {
    if(nP > 0) {
        printf("\nLista de Produtos:\n");
        for(int i = 0; i < nP; i++) {
            printf("Codigo: %d, Nome: %s, Preco: %.2f\n", produtos[i].codigo, produtos[i].nome, produtos[i].preco);
        }
    } else {
        printf("Nenhum produto cadastrado.\n");
    }
    pausar();  
}

// Função para pegar o índice de um produto pelo código
int pegarProdutoPorCodigo(int codigo) {
    for(int i = 0; i < nP; i++) {
        if(produtos[i].codigo == codigo) {
            return i;
        }
    }
    return -1;  
}

// Função para verificar se um produto já está no carrinho
int temNoCarrinho(int codigo) {
    for(int i = 0; i < nC; i++) {
        if(carrinho[i].produto.codigo == codigo) {
            return i;
        }
    }
    return -1;  
}

// Função para adicionar um produto ao carrinho
void comprarProduto() {
    if (nC >= MC) {
        printf("Carrinho cheio! Nao é possível adicionar mais produtos.\n");
        pausar();  
        return;
    }

    int codigo = entnumi("Digite o codigo do produto que deseja comprar: ");
    int indiceProduto = pegarProdutoPorCodigo(codigo);
    if(indiceProduto == -1) {
        printf("Produto nao encontrado.\n");
        pausar();  
        return;
    }

    int quantidade = entnumi("Digite a quantidade: ");
    if (quantidade <= 0) {
        printf("Quantidade invalida.\n");
        pausar();  
        return;
    }

    int indiceCarrinho = temNoCarrinho(codigo);
    if(indiceCarrinho != -1) {
        carrinho[indiceCarrinho].quantidade += quantidade;
    } else {
        carrinho[nC].produto = produtos[indiceProduto];
        carrinho[nC].quantidade = quantidade;
        nC++;
    }

    printf("Produto adicionado ao carrinho.\n");
    pausar();  
}

// Função para visualizar o carrinho
void visualizarCarrinho() {
    if(nC > 0) {
        printf("\nCarrinho:\n");
        printf("%-10s %-30s %-10s %-15s %-15s\n", "Codigo", "Nome", "Quantidade", "Preco Unitario", "Preco Total");
        for(int i = 0; i < nC; i++) {
            float precoTotal = carrinho[i].quantidade * carrinho[i].produto.preco;
            printf("%-10d %-30s %-10d %-15.2f %-15.2f\n", 
                    carrinho[i].produto.codigo, 
                    carrinho[i].produto.nome, 
                    carrinho[i].quantidade, 
                    carrinho[i].produto.preco, 
                    precoTotal);
        }
    } else {
        printf("Carrinho vazio.\n");
    }
    pausar();  
}

// Função para remover produto do carrinho
void removerProdutoCarrinho() {
    if (nC == 0) {
        printf("Carrinho vazio. Nenhum produto para remover.\n");
        pausar();  
        return;
    }

    int codigo = entnumi("Digite o codigo do produto que deseja remover: ");
    int indiceCarrinho = temNoCarrinho(codigo);
    
    if (indiceCarrinho == -1) {
        printf("Produto nao encontrado no carrinho.\n");
        pausar();  
        return;
    }

    int quantidadeRemover = entnumi("Digite a quantidade a remover: ");
    if (quantidadeRemover <= 0 || quantidadeRemover > carrinho[indiceCarrinho].quantidade) {
        printf("Quantidade invalida ou maior que a quantidade no carrinho.\n");
        pausar();  
        return;
    }

    // Remove a quantidade especificada
    carrinho[indiceCarrinho].quantidade -= quantidadeRemover;

    // Se a quantidade chegar a zero, remove o produto do carrinho
    if (carrinho[indiceCarrinho].quantidade == 0) {
        for (int i = indiceCarrinho; i < nC - 1; i++) {
            carrinho[i] = carrinho[i + 1]; // Move os produtos para preencher o espaço
        }
        nC--; // Reduz o número total de itens no carrinho
    }

    printf("Produto removido do carrinho.\n");
    pausar();  
}

// Função para fechar o pedido e esvaziar o carrinho
void fecharPedido() {
    if(nC > 0) {
        float total = 0;
        printf("\nFatura:\n");
        for(int i = 0; i < nC; i++) {
            float subtotal = carrinho[i].quantidade * carrinho[i].produto.preco;
            total += subtotal;
            printf("Produto: %s, Quantidade: %d, Subtotal: %.2f\n", 
                   carrinho[i].produto.nome, carrinho[i].quantidade, subtotal);
        }
        printf("Valor total: %.2f\n", total);
        nC = 0;  
        printf("Sua Compra foi realizada!\n");
    } else {
        printf("O carrinho esta vazio, pedido invalido.\n");
    }
    pausar();  
}

// Função principal
int main() {
    menu();
    return 0;
}