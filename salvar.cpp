#include <iostream>
#include <string>
#include <fstream>
#include "ean_8.h"

using namespace std;

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

