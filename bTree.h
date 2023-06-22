
typedef struct bTree bTree;
typedef struct no no;

// Função que cria a B Tree com a ordem passada como parâmetro
bTree *criaBTree(int ordem);

//Função que insere um elemento na B Tree
//Retornos:
//1: Elemento Inserido
//0: Falha ao inserir
int insereBTree(bTree *arv, int indice, int linha);

//Função que procura um elemento na B Tree de acordo com o indice passado
//Retornos:
//-1: Elemento não encontrado
//Número da linha do indice passado como parâmetro
int procuraBTree(bTree *arv, int indice, no *raiz);

//Função que imprime BTree
void imprimeBTree(no *raiz);

//Função que retorna raiz da árvore
no *retornaRaiz(bTree *arv);

//Função que retorna número de nós da árvore
int retornaNroNos(bTree *arv);

//Função que remove um elemento da B Tree de acordo com o indice passado
//Retornos:
//1: Elemento removido
//0: Elemento não está na árvore
int removeBTree(bTree *arv, int indice);

//Lê dados de arquivo externo e insere na B Tree
int processaArquivo(bTree *arv, char *nomeArquivo);

//Função que apaga toda a árvore passado como parâmetro
void excluiBTree(bTree *arv, no *raiz);

//Função para balancear árvore na inserção
void balanceamentoInsercao(bTree *arv, no *atual);

//Função para balancear árvore na remoção
void balanceamentoRemocao(bTree *arv, no *atual);