use std::env;
use std::fs::File;
use std::io::Write;

// Mapeia cada dígito de um identificador ao seu respectivo código binário
const L_CODE: [&str; 10] = [
    "0001101", "0011001", "0010011", "0111101", "0100011", 
    "0110001", "0101111", "0111011", "0110111", "0001011"
];
const R_CODE: [&str; 10] = [
    "1110010", "1100110", "1101100", "1000010", "1011100", 
    "1001110", "1010000", "1000100", "1001000", "1110100"
];

// Valida o dígito verificador de um identificador
fn validar_digito_verificador(identificador: &[u32]) -> bool {
    let (mut soma_impares, mut soma_pares) = (0, 0);
    for (i, &digito) in identificador.iter().take(7).enumerate() {
        if i % 2 == 0 {
            soma_impares += digito;
        } else {
            soma_pares += digito;
        }
    }
    (10 - (soma_pares * 1 + soma_impares * 3) % 10) % 10 == identificador[7]
}

// Gera o código de barras em formato binário
fn gerar_codigo_de_barras(identificador: &[u32]) -> String {
    let marcador_inicial = "101";
    let marcador_central = "01010";
    let marcador_final = "101";

    let mut codigo_binario = String::from(marcador_inicial);

    for &digito in &identificador[..4] {
        codigo_binario.push_str(L_CODE[digito as usize]);
    }

    codigo_binario.push_str(marcador_central);

    for &digito in &identificador[4..] {
        codigo_binario.push_str(R_CODE[digito as usize]);
    }

    codigo_binario.push_str(marcador_final);

    codigo_binario
}

// Salva o código de barras em um arquivo PBM
fn salvar_codigo_de_barras(
    codigo_de_barras: &str, 
    nome_do_arquivo: &str, 
    laterais: usize, 
    altura: usize, 
    espessura: usize
) {
    let largura_imagem = codigo_de_barras.len() * espessura + laterais * 2;
    let altura_imagem = altura + laterais * 2;

    let mut arquivo = match File::create(nome_do_arquivo) {
        Ok(f) => f,
        Err(_) => {
            eprintln!("Erro ao tentar criar arquivo de imagem.");
            return;
        }
    };

    writeln!(arquivo, "P1").unwrap();
    writeln!(arquivo, "{} {}", largura_imagem, altura_imagem).unwrap();

    for _ in 0..laterais {
        writeln!(arquivo, "{}", "0 ".repeat(largura_imagem)).unwrap();
    }

    for _ in 0..altura {
        write!(arquivo, "{}", "0 ".repeat(laterais)).unwrap();
        for bit in codigo_de_barras.chars() {
            write!(arquivo, "{}", format!("{} ", bit).repeat(espessura)).unwrap();
        }
        writeln!(arquivo, "{}", "0 ".repeat(laterais)).unwrap();
    }

    for _ in 0..laterais {
        writeln!(arquivo, "{}", "0 ".repeat(largura_imagem)).unwrap();
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() < 2 {
        eprintln!("Erro! Identificador não encontrado. Tente:");
        eprintln!("{} <identificador>", args[0]);
        return;
    }

    let entrada_identificador = &args[1];
    if entrada_identificador.len() != 8 || !entrada_identificador.chars().all(|c| c.is_digit(10)) {
        eprintln!("Erro! O identificador deve ter 8 dígitos.");
        return;
    }

    let identificador: Vec<u32> = entrada_identificador
        .chars()
        .map(|c| c.to_digit(10).unwrap())
        .collect();

    if !validar_digito_verificador(&identificador) {
        eprintln!("Erro! Dígito verificador do identificador inválido.");
        return;
    }

    let laterais = args.get(2).and_then(|s| s.parse().ok()).unwrap_or(4);
    let altura = args.get(3).and_then(|s| s.parse().ok()).unwrap_or(50);
    let espessura = args.get(4).and_then(|s| s.parse().ok()).unwrap_or(1);
    let nome_do_arquivo = args.get(5).unwrap_or(&"codigo.pbm".to_string());

    let codigo_binario = gerar_codigo_de_barras(&identificador);
    salvar_codigo_de_barras(&codigo_binario, nome_do_arquivo, laterais, altura, espessura);
    println!("Arquivo PBM salvo como: {}", nome_do_arquivo);
}