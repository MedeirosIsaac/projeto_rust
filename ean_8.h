#ifndef EAN8_H
#define EAN8_H
#include <string>
using namespace std;

// class EAN8 {
// public:
//     // Mapeia os códigos binários para os dígitos EAN-8
//     static const std::string L_CODE[10];
//     static const std::string R_CODE[10];

//     // Valida o dígito verificador
//     static bool validar_digito_verificador(int identificador[8]);

//     // Gera o código de barras em formato binário
//     static std::string gerar_codigo_de_barras(int identificador[8]);

//     // Salva o código de barras no formato PBM
//     static void salvar_codigo_de_barras(const std::string& codigo_de_barras, const std::string& nome_do_arquivo, int laterais=4, int altura=50, int espessura=1);
// };

// Mapeia os códigos binários para os dígitos EAN-8
const std::string L_CODE[10] = {"0001101", "0011001", "0010011", "0111101", "0100011", "0110001", "0101111", "0111011", "0110111", "0001011"};
const std::string R_CODE[10] = {"1110010", "1100110", "1101100", "1000010", "1011100", "1001110", "1010000", "1000100", "1001000", "1110100"};

// Valida o dígito verificador
bool validar_digito_verificador(int identificador[8]);

// Gera o código de barras em formato binário
string gerar_codigo_de_barras(int identificador[8]);

// Salva o código de barras no formato PBM
void salvar_codigo_de_barras(const string& codigo_de_barras, const string& nome_do_arquivo, int laterais, int altura, int espessura);


#endif // EAN8_H