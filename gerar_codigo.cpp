#include <iostream>
#include "ean_8.h"

using namespace std;

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
    std::string entrada_identificador = argv[1];
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