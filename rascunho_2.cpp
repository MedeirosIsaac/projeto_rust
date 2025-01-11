#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Tabelas de codificação binária
const string LEFT_ODD[] = {"0001101", "0011001", "0010011", "0111101", "0100011", "0110001", "0101111", "0111011", "0110111", "0001011"};
const string RIGHT[] = {"1110010", "1100110", "1101100", "1000010", "1011100", "1001110", "1010000", "1000100", "1001000", "1110100"};

// Função para carregar a matriz PBM
vector<string> carregarPBM(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo PBM." << endl;
        exit(1);
    }

    string linha;
    vector<string> linhas;

    // Ignorar o cabeçalho PBM
    getline(arquivo, linha); // Formato P1
    getline(arquivo, linha); // Dimensões
    while (getline(arquivo, linha)) {
        linhas.push_back(linha);
    }

    arquivo.close();
    return linhas;
}

// Função para extrair uma linha única de bits do PBM
string extrairLinhaUnica(const vector<string>& linhas) {
    string binario;
    for (char c : linhas[0]) {
        if (c == '0' || c == '1') {
            binario += c;
        }
    }
    return binario;
}

// Função para encontrar o número correspondente ao binário em uma tabela
int buscarTabela(const string& bits, const string tabela[]) {
    for (int i = 0; i < 10; i++) {
        if (tabela[i] == bits) return i;
    }
    return -1; // Não encontrado
}

// Função para decodificar o binário para números
vector<int> decodificarBinario(const string& binario) {
    vector<int> numeros;

    // Verificar padrões de início, separador e final
    if (binario.substr(0, 3) != "101" || binario.substr(binario.size() - 3) != "101" || binario.substr(32, 5) != "01010") {
        cerr << "Formato EAN-8 inválido!" << endl;
        exit(1);
    }

    // Decodificar os 4 primeiros dígitos (esquerda)
    for (int i = 3; i < 3 + 7 * 4; i += 7) {
        string segmento = binario.substr(i, 7);
        int numero = buscarTabela(segmento, LEFT_ODD);
        if (numero == -1) {
            cerr << "Erro ao decodificar os dígitos da esquerda!" << endl;
            exit(1);
        }
        numeros.push_back(numero);
    }

    // Decodificar os 4 últimos dígitos (direita)
    for (int i = 32 + 5; i < 32 + 5 + 7 * 4; i += 7) {
        string segmento = binario.substr(i, 7);
        int numero = buscarTabela(segmento, RIGHT);
        if (numero == -1) {
            cerr << "Erro ao decodificar os dígitos da direita!" << endl;
            exit(1);
        }
        numeros.push_back(numero);
    }

    return numeros;
}

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

int main() {
    string nomeArquivo;
    cout << "Digite o nome do arquivo PBM: ";
    cin >> nomeArquivo;

    // Carregar a imagem PBM
    vector<string> linhas = carregarPBM(nomeArquivo);

    // Extrair uma linha única de bits
    string binario = extrairLinhaUnica(linhas);

    // Decodificar o binário para obter os números
    vector<int> numeros = decodificarBinario(binario);

    // Validar o dígito verificador
    int digitoVerificador = calcularDigitoVerificador(numeros);
    if (digitoVerificador != numeros.back()) {
        cerr << "Dígito verificador inválido!" << endl;
        return 1;
    }

    // Exibir o identificador extraído
    cout << "Identificador EAN-8: ";
    for (int i = 0; i < numeros.size(); i++) {
        cout << numeros[i];
    }
    cout << endl;

    return 0;
}