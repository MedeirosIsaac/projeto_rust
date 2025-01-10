#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

// mapeia cada dígito de um identficador ao seu respectivo código binário
//                          0           1           2           3          4           5        6          7            8        9
const string L_CODE[10] = {"0001101", "0011001", "0010011", "0111101", "0100011", "0110001", "0101111", "0111011", "0110111", "0001011"};
const string R_CODE[10] = {"1110010", "1100110", "1101100", "1000010", "1011100", "1001110", "1010000", "1000100", "1001000", "1110100"};

string gerar_codigo_de_barras(vector<int> identificador) {
    string codigo_de_barras = "";
    string marcador_inicial = "101";
    string marcador_final = "101";
    string marcador_central = "01010";

    // adiciona o padrão inicial ao código de barras
    codigo_de_barras += marcador_inicial;
    // adiciona os primeiros 4 dígitos do identificador em formato binário ao código de barras
    for(int i = 0; i < 4; i++){
        codigo_de_barras += L_CODE[identificador[i]];
    }
    // adiciona o padrão central ao código de barras
    codigo_de_barras += marcador_central;
    // adiciona os últimos 4 dígitos do identificador em formato binário ao código de barras
    for(int i = 4; i < 8; i++){
        codigo_de_barras += R_CODE[identificador[i]];
    }
    // adiciona o padrão final ao código de barras
    codigo_de_barras += marcador_final;
    return codigo_de_barras;
}

void salvar_codigo_de_barras(const string& codigo_de_barras, const string& nome_do_arquivo) {
    ofstream arquivo(nome_do_arquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao tentar criar aquivo de imagem." << endl;
        return;
    }
    int largura = codigo_de_barras.size();
    int altura = 50;
    arquivo << "P1" << endl;
    arquivo << largura << " " << altura << endl;
    
    //escreve as barras para cada linha
    for (int y = 0; y < altura; y++) {
        for (char bit : codigo_de_barras) {
            arquivo << (bit == '1' ? "1 " : "0 ");
        } 
        arquivo << endl;
    }
    
    arquivo.close();
    cout << "Arquivo PBM salvo como: " << nome_do_arquivo << endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Erro! Identificador não encontrado. Tente:" << endl;
        cout << argv[0] << " <identificador>" << endl;
        return 1;
    }

    vector<int> identificador(8);

    for (int i = 0; i < 6; i++) {
        identificador[i] = (int) argv[1][i];
    }
    
    string codigo_binario = gerar_codigo_de_barras(identificador);
    
    return 0;
}