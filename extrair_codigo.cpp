#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

const string L_CODE[10] = {"0001101", "0011001", "0010011", "0111101", "0100011", "0110001", "0101111", "0111011", "0110111", "0001011"};
const string R_CODE[10] = {"1110010", "1100110", "1101100", "1000010", "1011100", "1001110", "1010000", "1000100", "1001000", "1110100"};

// Carrega a imagem PBM e salva num vetor de strings
// Extrai o binario de uma única linha e salva numa string

// Decodifica o binário para obter o identificador
// Valida o dígito verificador

// extrair uma linha única de bits
string extrair_binario_de_linha(string linha) {
    string binario = "";
    for (char c : linha) {
        if (c == '0' || c == '1') {
            binario += c;
        }
    }
    return binario;
}

// carregar binario do arquivo PBM
string carregar_binario_de_arquivo_de_imagem(const string& nome_do_arquivo) {
    ifstream arquivo(nome_do_arquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo PBM." << endl;
        exit(1);
    }

    string linha;
    string binario = "";

    // Ignorar o cabeçalho PBM
    getline(arquivo, linha); // Formato P1
    getline(arquivo, linha); // Dimensões
    while (getline(arquivo, linha) && binario == "") {
        binario = extrair_binario_de_linha(linha);
    }

    arquivo.close();
    return binario;
}

int mapear_codigo_binario_em_numero(const string& bits, const string tabela[]) {
    for (int i = 0; i < 10; i++) {
        if (tabela[i] == bits) return i;
    }
    return -1; // Não encontrado
}

//Decodificar o binario para obter os números
vector<int> extrair_identificador_de_binario(const string& binario) {
    vector<int> numeros;
    // 67 digitos
    // 101 0100011 0001101 0011001 0111011 01010 1110010 1000100 1101100 1001110 101
    // 1010100011000110100110010111011 01010 1110010100010011011001001110101
    // 10101000110001101001100101110110 10101 110010100010011011001001110101
    // Verificar padrões de início, separador e final
    if (binario.substr(0, 3) != "101" || binario.substr(binario.size() - 3) != "101" || binario.substr(31, 5) != "01010") {
        cout << binario.substr(0, 3) << endl;
        cout << binario.substr(binario.size() - 3) << endl;
        cout << binario.substr(31, 5) << endl;
        cerr << "Formato EAN-8 inválido!" << endl;
        exit(1);
    }

    // Decodificar os 4 primeiros dígitos (esquerda)
    for (int i = 3; i < 3 + 7 * 4; i += 7) {
        string segmento = binario.substr(i, 7);
        int numero = mapear_codigo_binario_em_numero(segmento, L_CODE);
        if (numero == -1) {
            cerr << "Erro ao decodificar os dígitos da esquerda!" << endl;
            exit(1);
        }
        numeros.push_back(numero);
    }

    // Decodificar os 4 últimos dígitos (direita)
    for (int i = 31 + 5; i < 31 + 5 + 7 * 4; i += 7) {
        string segmento = binario.substr(i, 7);
        int numero = mapear_codigo_binario_em_numero(segmento, R_CODE);
        if (numero == -1) {
            cerr << "Erro ao decodificar os dígitos da direita!" << endl;
            exit(1);
        }
        numeros.push_back(numero);
    }

    return numeros;
}

bool validar_digito_verificador(vector<int> identificador) {
    int soma_impares = 0, soma_pares = 0;
    for (int i = 1; i < 8; i++) {
        if (i % 2 == 0) soma_pares += identificador[i-1];
        else soma_impares += identificador[i-1];
    }
    return (10 - (soma_pares * 1 + soma_impares * 3) % 10) % 10 == identificador[7];
}

int main(int argc, char* argv[]) {
    string nome_do_arquivo;
    if (argc < 2) {
        cerr << "Erro! Nome do arquivo não fornecido. Tente:" << endl;
        cerr << argv[0] << " <nome_do_arquivo>" << endl;
        return 1;
    }
    nome_do_arquivo = argv[1];

    // Carregar codigo binario da imagem PBM
    string binario = carregar_binario_de_arquivo_de_imagem(nome_do_arquivo);

    // Decodificar o binário para obter os números
    vector<int> identificador = extrair_identificador_de_binario(binario);

    // Validar o dígito verificador
    if (!validar_digito_verificador(identificador)) {
        cerr << "Dígito verificador inválido!" << endl;
        return 1;
    }

    // Exibir o identificador extraído
    cout << "Identificador EAN-8: ";
    for (int i = 0; i < identificador.size(); i++) {
        cout << identificador[i];
    }
    cout << endl;
}