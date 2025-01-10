#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

// Tabelas de codificação binária
const string LEFT_ODD[] = {"0001101", "0011001", "0010011", "0111101", "0100011", "0110001", "0101111", "0111011", "0110111", "0001011"};
const string RIGHT[] = {"1110010", "1100110", "1101100", "1000010", "1011100", "1001110", "1010000", "1000100", "1001000", "1110100"};

// Função para calcular o dígito verificador
int calcularDigitoVerificador(const vector<int>& numeros) {
    int somaImpares = 0, somaPares = 0;
    for (int i = 0; i < 7; i++) {
        if (i % 2 == 0) somaImpares += numeros[i];
        else somaPares += numeros[i];
    }
    int total = somaImpares * 3 + somaPares;
    return (10 - (total % 10)) % 10;
}

// Função para gerar a codificação binária EAN-8
string gerarCodigoEAN8(const vector<int>& numeros) {
    string binario = "101"; // Padrão inicial

    // Codificação dos 4 primeiros dígitos (esquerda)
    for (int i = 0; i < 4; i++) {
        binario += LEFT_ODD[numeros[i]];
    }

    binario += "01010"; // Separador central

    // Codificação dos 4 últimos dígitos (direita)
    for (int i = 4; i < 8; i++) {
        binario += RIGHT[numeros[i]];
    }

    binario += "101"; // Padrão final
    return binario;
}

// Função para salvar o código binário em formato PBM
void salvarPBM(const string& binario, const string& nomeArquivo, int altura = 50) {
    ofstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo para escrita." << endl;
        return;
    }

    // Escrevendo o cabeçalho do arquivo PBM
    int largura = binario.size();
    arquivo << "P1" << endl;          // Formato PBM
    arquivo << largura << " " << altura << endl; // Dimensões da imagem

    // Escrevendo as barras para cada linha
    for (int y = 0; y < altura; y++) {
        for (char bit : binario) {
            arquivo << (bit == '1' ? "1 " : "0 ");
        }
        arquivo << endl;
    }

    arquivo.close();
    cout << "Arquivo PBM salvo como: " << nomeArquivo << endl;
}

int main() {
    vector<int> numeros(7);

    // Entrada dos 7 primeiros dígitos
    cout << "Digite os 7 primeiros dígitos do EAN-8: ";
    for (int i = 0; i < 7; i++) {
        cin >> numeros[i];
        if (numeros[i] < 0 || numeros[i] > 9) {
            cout << "Digite apenas números entre 0 e 9!" << endl;
            return 1;
        }
    }

    // Calcular o dígito verificador
    int digitoVerificador = calcularDigitoVerificador(numeros);
    numeros.push_back(digitoVerificador);

    // Gerar o código binário
    string codigoBinario = gerarCodigoEAN8(numeros);

    // Salvar o código binário em arquivo PBM
    salvarPBM(codigoBinario, "codigo_de_barras.pbm");

    return 0;
}