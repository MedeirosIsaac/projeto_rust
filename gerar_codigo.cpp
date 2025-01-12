#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// mapeia cada dígito de um identficador ao seu respectivo código binário
//                          0           1           2           3          4           5        6          7            8        9
const string L_CODE[10] = {"0001101", "0011001", "0010011", "0111101", "0100011", "0110001", "0101111", "0111011", "0110111", "0001011"};
const string R_CODE[10] = {"1110010", "1100110", "1101100", "1000010", "1011100", "1001110", "1010000", "1000100", "1001000", "1110100"};

//              1           2              3            4           ...
// soma = ( 3 × i_1 ) + ( 1 × i_2 ) + ( 3 × i_3 ) + ( 1 × i_4 ) + ( 3 × i_5 ) + ( 1 × i_6 ) + ( 3 × i_7 ) 
// soma = 3 × ( i_1 + i_3 + i_5 + i_7 ) + 1 × ( i_2 + i_4 + i_6)
bool validar_digito_verificador(int identificador[8]) {
    int soma_impares = 0, soma_pares = 0;
    for (int i = 1; i < 8; i++) {
        if (i % 2 == 0) soma_pares += identificador[i-1];
        else soma_impares += identificador[i-1];
    }
    return (10 - (soma_pares * 1 + soma_impares * 3) % 10) % 10 == identificador[7];
}

// 48 -> 8 -> 10 - 8 -> 2 -> 2 ==
// 42 -> 2 -> 10 - 2 -> 8 -> 2 == 
// 40 -> 0 -> 10 - 0 -> 10 -> 0 ==

string gerar_codigo_de_barras(int identificador[8]) {
    string codigo_binario = "";
    string marcador_inicial = "101";
    string marcador_final = "101";
    string marcador_central = "01010";

    // adiciona o padrão inicial ao código de barras
    codigo_binario += marcador_inicial;
    // adiciona os primeiros 4 dígitos do identificador em formato binário ao código de barras
    for(int i = 0; i < 4; i++){
        codigo_binario += L_CODE[identificador[i]];
    }
    // adiciona o padrão central ao código de barras
    codigo_binario += marcador_central;
    // adiciona os últimos 4 dígitos do identificador em formato binário ao código de barras
    for(int i = 4; i < 8; i++){
        codigo_binario += R_CODE[identificador[i]];
    }
    // adiciona o padrão final ao código de barras
    codigo_binario += marcador_final;

    return codigo_binario;
}

// padrão inicial area 1: 101
// padrão inicial area 2: 110011
// padrão inicial area 3: 111000111
// ...

void salvar_codigo_de_barras(const string& codigo_de_barras, const string& nome_do_arquivo, int laterais=4, int altura=50, int espessura=1) {
    ofstream arquivo(nome_do_arquivo);
    // cout <-> c out
    // ofstream <-> o f stream <-> output file stream
    if (!arquivo.is_open()) {
        cerr << "Erro ao tentar criar aquivo de imagem." << endl;
        return;
    }
    // a largura tem que mudar com a espessura do código
    int largura_imagem = codigo_de_barras.size() * espessura + laterais * 2;
    int altura_imagem = altura + laterais * 2;
    arquivo << "P1" << endl;
    arquivo << largura_imagem << " " << altura_imagem << endl;
    
    // escreve espaçamento superior
    for (int i = 0; i < laterais; i++) {
        for (int j = 0; j < largura_imagem; j++) {
            arquivo << "0 ";
        }
        arquivo << endl;
    }

    //escreve as barras para cada linha
    for (int y = 0; y < altura; y++) {
        // escreve espaçamento lateral esquerdo
        for (int i = 0; i < laterais; i++) {
            arquivo << "0 ";
        }
        // escreve código da linha
        for (char bit : codigo_de_barras) {
            for (int k = 0; k < espessura; k++) {
                arquivo << bit << " ";
            }
        }
        // escreve espaçamento lateral direito
        for (int i = 0; i < laterais; i++) {
            arquivo << "0 ";
        }
        arquivo << endl;
    }

    // escreve espaçamento inferior
    for (int i = 0; i < laterais; i++) {
        for (int j = 0; j < largura_imagem; j++) {
            arquivo << "0 ";
        }
        arquivo << endl;
    }
    
    arquivo.close();
}

int main(int argc, char* argv[]) {
    cout << argc << endl;
    int identificador[8];
    int laterais = 4;
    int altura = 50;
    int espessura = 1;
    string nome_do_arquivo = "codigo.pbm";
    if (argc < 2) {
        cerr << "Erro! Identificador não encontrado. Tente:" << endl;
        cerr << argv[0] << " <identificador>" << endl;
        return 1;
    }
    string entrada_identificador = argv[1];
    if (entrada_identificador.length() != 8) {
        cerr << "Erro! O identificador deve ter 8 digitos." << endl;
        return 1;
    }
    
    for (int i = 0; i < 8; i++) {
        // 48, 49, 50, 51, 52, 53, 54, 55, 56, 57
        if (argv[1][i] < 48 || argv[1][i] > 57) {
            cerr << "Erro! O digito identificador deve ser composto apenas de digitos entre 0 e 9." << endl;
            return 1;
        }
        identificador[i] = argv[1][i] - 48;
    }

    if (!validar_digito_verificador(identificador)) {
        cerr << "Erro! Digito verificador do identificador inválido." << endl;
        return 1;
    }

    if (argc > 2) {
        laterais = stoi(argv[2]);
    }
    if (argc > 3) {
        altura = stoi(argv[3]);
    }
    if (argc > 4) {
        espessura = stoi(argv[4]);
    }
    if (argc > 5) {
        nome_do_arquivo = argv[5];
    }
    
    string codigo_binario = gerar_codigo_de_barras(identificador);
    salvar_codigo_de_barras(codigo_binario, nome_do_arquivo, laterais, altura, espessura);
    cout << "Arquivo PBM salvo como: " << nome_do_arquivo << endl;
    
    return 0;
}
// 3(1 + 3 + 5 + 7) + 1(2 + 4 + 6) = 48 + 12 = 60 % 10 = 0 % 10 = 0