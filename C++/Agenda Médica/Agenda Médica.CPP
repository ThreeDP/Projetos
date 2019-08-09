/// PROGRAMA AGENDA

/*
  Programa ..................: Agenda de Atendimento
  Autor .....................: Davy P. S. Dantas
  Data ......................: 18 / 05 / 2019
  Versão ....................: v1.0.0
  Contato ...................: davypaulinocbjr@outlook.com
  Linkedin ..................: @davypaulinodsd
  Github ....................: @ThreeDP
  Portifolio ................: davypaulino.myportfolio.com
*/

// INICIO: BIBLIOTECAS .............................|
#include <iostream>
#include <iomanip>
#include <fstream>
#include <clocale>
#include <cctype>
#include <windows.h>
// FIM: BIBLIOTECAS ................................|

// INICIO: Constantes de posicionamento do cursor ..|
#define ROD                                       25
#define PADRAO                                     1
#define ESCRIT                                    23
#define LIMPAT                                     4
#define XPAD                                       0
#define CABEC                                      3
#define L_RE                                       8
// FIM: Constantes de posicionamento do cursor .....|

// INICIO: Constantes de Cores .....................|
#define PRETO                                      0
#define AZUL                                       9
#define VERDE                                     10
#define VERMEL                                     4
#define BRANCO                                    15
#define AMAREL                                    14
// FIM: Constantes de Cores ........................|

// INICIO: Constantes de dimenção TELA .............|
#define COLUN 93
#define LINH 30
// FIM: Constantes de dimenção TELA ................|

// INICIO: Constantes de reset .....................|
#define VAZIO "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" // Constante para verificação de vazio;
#define ESPACO "                                                                               " // Constante para limpar espaço coloridos;
// FIM: Constantes de reset ........................|

/// Registro AGENDA variavel Global
using namespace std;
struct CAD_AGENDA // Registro para armazenar o nome do cliente no dia e horario indicados.
  {
    char NOME[41];
  }; CAD_AGENDA AGENDA[31][10];

/// INICIO: Biblioteca Programador ========================================================================================================================
 void DIMENCIONAR_TELA()
{
    COORD POS;
    POS.X = COLUN;
    POS.Y = LINH;

    SMALL_RECT DIMENCAO;
    DIMENCAO.Top = 0;
    DIMENCAO.Left = 0;
    DIMENCAO.Bottom = LINH - 1;                 // Deternina a parte inferior da tela com o valor da constante LINH (LINHA);
    DIMENCAO.Right = COLUN - 1;                 // Deternina a direita da tela com o valor da constante COLUN (COLUNA);
    /* - 1 é para que o valor permaneça correto transformando o numero em sequencia cardinal;*/

    HANDLE TELA = GetStdHandle(STD_OUTPUT_HANDLE);       // Determina a variavel TELA como idicador de saida no video;
    SetConsoleScreenBufferSize(TELA, POS);            // Set Buffer Size
    SetConsoleWindowInfo(TELA, true, &DIMENCAO);            // Set Window Size
}

void LIMPAR_LINHA(void)
{
  HANDLE TELA;
  COORD POS;
  CONSOLE_SCREEN_BUFFER_INFO VIDEO;
  DWORD ESCRITA = 0;
  TELA = GetStdHandle(STD_OUTPUT_HANDLE);
  GetConsoleScreenBufferInfo(TELA, &VIDEO);
  POS.Y = VIDEO.dwCursorPosition.Y;
  POS.X = VIDEO.dwCursorPosition.X;
  FillConsoleOutputCharacter(TELA, 32, 80 - POS.X, POS, &ESCRITA);
  return;
}

void ATRIBUI_COR(int COR_FUNDO, int COR_TEXTO)
{
  HANDLE TELA;
  int COR;
  TELA = GetStdHandle(STD_OUTPUT_HANDLE);

  if (COR_FUNDO < 0 and COR_FUNDO > 15 and COR_TEXTO < 0 and COR_TEXTO > 15)
    COR = 15;
  else
    COR = COR_TEXTO - 1 + 16 * COR_FUNDO + 1;
  SetConsoleTextAttribute(TELA, COR);
}

void LIMPAR_TELA(int X, int Y)
{
  HANDLE TELA;
  DWORD ESCRITA = 0;
  COORD POS;
  TELA = GetStdHandle(STD_OUTPUT_HANDLE);
  POS.X = X;
  POS.Y = Y;
  FillConsoleOutputCharacter(TELA, 32, 100 * 100, POS, &ESCRITA);
}

int POSICIONA_CURSOR(int LINHA ,int COLUNA)
{
  HANDLE TELA;
  COORD POS;
  TELA = GetStdHandle(STD_OUTPUT_HANDLE);
  POS.X = COLUNA - 1;
  POS.Y = LINHA - 1;
  SetConsoleCursorPosition(TELA, POS);
}
/// FIM: Biblioteca Programador ===========================================================================================================================

/// INICIO: Rotina RESET_MATRIZ ---------------------------------------------------------------------------------------------------------------------------
void RESET_MATRIZ()
{
  int I, J;

  for (I = 1; I <= 31; I++)
  {
    for (J = 1; J <= 10; J++)
    {
      strcpy(AGENDA[I][J].NOME, VAZIO);
    }
  }
  return;
}
/// FIM: Rotina RESET_MATRIZ ------------------------------------------------------------------------------------------------------------------------------

/// INICIO: Rotina Entrar DIA -----------------------------------------------------------------------------------------------------------------------------
int ENTRA_DIA(int *DIA)                       // Função de Entrada e verificação do dado dia;
{
  bool I = false;                             // Variavel para verificação da válidade do numero inserido;
  char C_DIA[10];                             // Char definida com o numero de caracteres numericos possiveis de se entrar sem ocorrer um erro;
  cout << " Informe o DIA .....: ";

  do
  {
    cin.getline(C_DIA, sizeof(C_DIA));        // Realiza a entrada do DIA em forma de Char para evitar Entrada de caracteres especial como o \n;
    *DIA = atoi(C_DIA);                       // Converte o valor de char para inteiro e atribui a varial DIA;
    if (*DIA <= 0 or *DIA > 31)               // Verifica se o numero convertido se encontra na faixa válida de dias;
      {
        POSICIONA_CURSOR(ROD, PADRAO);        // Posiciona Cursor na 1º linha e na 2º coluna;
        ATRIBUI_COR(VERMEL, BRANCO);          // Atribui cor vermelha ao fundo e branca para a letra;

        cout << " Dia Inválido " << endl;

        ATRIBUI_COR(PRETO, BRANCO);           // Restaura cor de fundo e texto para o padrão;
        POSICIONA_CURSOR(9, ESCRIT);          // Posiciona o cursor na 8º linha e em frente ao texto "Informe o DIA";
        LIMPAR_LINHA();                       // Limpa os caracteres a partir da localização do cursor (apaga numero invalidos inseridos);
      }
    else
      {
        I = true;
        POSICIONA_CURSOR(ROD, PADRAO);        // Posiciona o Cursor no rodapé;
        cout << ESPACO<< endl;                // realiza a inserção de caracteres vazios na área onde o fundo foi colorido anteriormente;
        POSICIONA_CURSOR(L_RE + 1, PADRAO);   // Volta o cursor para a linha posterior ao texto "Informe o DIA";
      }
  }
  while (not(I == true));
}
/// FIM: Rotina Entrar DIA --------------------------------------------------------------------------------------------------------------------------------

/// INICIO: Rotina Entrar HORA ----------------------------------------------------------------------------------------------------------------------------
int ENTRA_HORA(int *HORA)
{
  bool I = false;
  char C_HORA[10];                             // Char definida com o numero de caracteres numericos possiveis de se entrar sem ocorrer um erro.

  cout << " Informe a HORA ....: ";

  do
  {
    cin.getline(C_HORA, sizeof(C_HORA));       // Realiza a entrada do DIA em forma de Char para evitar Entrada de caracteres especial como o \n;
    *HORA = atoi(C_HORA);                      // Converte o valor de char para inteiro e atribui a varial HORA;

    if (*HORA < 8 or *HORA > 17)               // Verifica a entrada de hora, sendo valida somente se entre as 8 e 17 horas
    {
      POSICIONA_CURSOR(ROD, PADRAO);           // Posiciona o Cursor no rodapé;
      ATRIBUI_COR(VERMEL, BRANCO);             // Atribui cor vermelha ao fundo e branca para a letra;
      cout << " Hora Inválida " << endl;
      ATRIBUI_COR(PRETO, BRANCO);              // Volta as cores ao padrão
      POSICIONA_CURSOR(L_RE + 1, ESCRIT);      // Posiciona o cursor frente ao texto " Informe a Hora;
      LIMPAR_LINHA();                          // Limpa toda a linha a partir da posição inicial e volta para a mesma posição;
    }
    else
    {
      I = true;
      POSICIONA_CURSOR(ROD, PADRAO);           // Posiciona o cursor na linha de rodapé;
      cout << ESPACO << endl;                  // realiza a inserção de caracteres vazios na área onde o fundo foi colorido anteriormente;
      POSICIONA_CURSOR(L_RE + 2, PADRAO);      // Volta o cursor para a linha posterior ao texto "Informe a HORA";
    }
  }
  while (not(I == true));
}
/// FIM: Rotina Entrar HORA -------------------------------------------------------------------------------------------------------------------------------

/// INICIO: Rotina Marcar atendimento ---------------------------------------------------------------------------------------------------------------------
int CADASTRO(int *SELETOR)
{
  int DIA, HORA;
  char I, NOME[41];
  bool VALIDO = false;
  short RETORNO;

  ofstream ARQUIVO("AGENDAMENTO.DDP", ios::binary);
  LIMPAR_TELA(XPAD, CABEC);                     // Passa 2 parametros de onde será o inicio da limpeza e limpa um numero de colunas e linhas;
  POSICIONA_CURSOR(LIMPAT, PADRAO);             // Posiciona Cursor abaixo da linha de cabeçalho;

  cout << "\n" << setw(58) << "MARCAR ATENDIMENTO\n\n\n";

  ENTRA_DIA(&DIA);
  ENTRA_HORA(&HORA);


  if (strcmp(AGENDA[DIA][HORA-7].NOME, VAZIO) == 0)  // Verifica se o dia e hora escolhidos estão disponiveis;
  {
    cout << " Nome do cliente ...: ";
    do
    {
      LIMPAR_LINHA();                            // Limpa toda a linha a partir de onde o cursor se localiza;
      cin.getline(NOME, sizeof(NOME));

    RETORNO = isalpha(NOME[1]);                  // verifica se o primeiro caracter da cadeia não é um caracter vazio ou especial por exemplo (\0 ou \n) e retorna 0 caso seja.

      if (RETORNO != 0)                          // Caso não seja um caracter vazio ou especial cadastra com sucesso.
      {
        strcpy(AGENDA[DIA][HORA - 7].NOME, NOME);  // Copia o valor entrado em NOME na posição de AGENDA.NOME;

        POSICIONA_CURSOR(ROD, PADRAO);           // Posiciona o cursor na linha de rodapé;
        ATRIBUI_COR(VERDE, BRANCO);              // Atribui cor verde ao fundo e branco para a letra;
        cout << " Sucesso: Horário Agendado!" << endl;
        ATRIBUI_COR(PRETO, BRANCO);              // Restaura cor de fundo e texto para o padrão;
        VALIDO = true;
      }
      else                                       // Caso seja um caracter vazio ou especial possibilita uma nova entrada de nome.
      {
        POSICIONA_CURSOR(ROD, PADRAO);           // Posiciona o cursor na linha de rodapé;
        ATRIBUI_COR(AZUL, BRANCO);               // Atribui Cor azul ao fundo e cor branca para letra;
        cout << " Nome Inválido";
        ATRIBUI_COR(PRETO, BRANCO);              // Restaura cor de fundo e texto para o padrão;

        cout << " (Entre somente valores alfanumericos de A-Z ou a-z)" << endl;
        cout << " Deseja Entra novamente? (S/N) ";
        cin >> I;
        cin.ignore(80, '\n');

        VALIDO = (toupper(I) == 'S') ? false : true;  // Verifica se variavel I é igual ao caracter S se sim atribui falso a VALIDO;
        POSICIONA_CURSOR(ROD, PADRAO);           // Posiciona o cursor na linha de rodapé;
        cout << ESPACO;                          // Atribui espaços vazio a linha de rodapé para restaurar cor de fundo padrão;
      }
      LIMPAR_LINHA();                            // Limpa toda a linha a partir de onde o cursor se localiza;
      POSICIONA_CURSOR(ROD + 1, PADRAO);         // Posiciona o cursor uma linha abaixo da linha de rodapé;
      LIMPAR_LINHA();                            // Limpa toda a linha a partir de onde o cursor se localiza;
      POSICIONA_CURSOR(L_RE + 2, ESCRIT);        // Posiciona o cursor na linha de escrita do nome;

    }while (VALIDO == false);                    // Se a entrada do nome não for valido e o cliente desejar entra novamente deve ser igual a FALSO;
  }
  else
  {
    POSICIONA_CURSOR(ROD, PADRAO);               // Posiciona o cursor na linha de rodapé;
    ATRIBUI_COR(VERMEL, BRANCO);                 // Atribui cor vermelha ao fundo e branca para letra;
    cout << " Horário Ocupado!" << endl;
    ATRIBUI_COR(PRETO, BRANCO);                  // Restaura cor de fundo e texto para o padrão;
  }
  ARQUIVO.write(reinterpret_cast<char*>(&AGENDA), sizeof(AGENDA));
  ARQUIVO.flush();
  ARQUIVO.close();
  POSICIONA_CURSOR(ROD + 1, PADRAO);             // Posiciona cursor abaixo da linha de rodapé;
  ATRIBUI_COR(PRETO, AZUL);                      // Atribui cor azul para a letra;
  cout << " Tecle <Enter> para ir ao menu\n\n";
  *SELETOR = 0;                                  // Atribui Zero ao SELETOR para resetar seleção do swicth case;
  cin.get();

  ATRIBUI_COR(PRETO, BRANCO);                    // Restaura cor de fundo e texto para o padrão;
  POSICIONA_CURSOR(ROD, PADRAO);                 // Posiciona o cursor na linha de rodapé;
  cout << ESPACO;                                // Atribui espaços vazio a linha de rodapé para restaurar cor de fundo padrão;
  LIMPAR_TELA(XPAD, CABEC);                      // Passa 2 parametros de onde será o inicio da limpeza e limpa um numero de colunas e linhas;
  POSICIONA_CURSOR(LIMPAT, PADRAO);              // Posiciona Cursor abaixo da linha de cabeçalho;
}
/// FIM: Rotina Marcar atendimento ------------------------------------------------------------------------------------------------------------------------

/// INICIO: Rotina Desmarcar atendimento ------------------------------------------------------------------------------------------------------------------
int DESMARCAR(int *SELETOR)
{
  ofstream ARQUIVO("AGENDAMENTO.DDP", ios::binary);  // Abre o arquivo Agendamento como modo de Escrita;
  int DIA, HORA;
  char I;

  LIMPAR_TELA(XPAD, CABEC);                       // Passa 2 parametros de onde será o inicio da limpeza e limpa um numero de colunas e linhas;
  POSICIONA_CURSOR(LIMPAT, PADRAO);               // Posiciona Cursor abaixo da linha de cabeçalho;

  cout << "\n" << setw(59) << "DESMARCAR ATENDIMENTO\n\n\n";

  ENTRA_DIA(&DIA);
  ENTRA_HORA(&HORA);

  if (strcmp(AGENDA[DIA][HORA - 7].NOME, VAZIO) == 0)  // Executa as ações caso a posição em AGENDA já esteja vazia;
  {
    POSICIONA_CURSOR(ROD, PADRAO);                // Posiciona o cursor na linha de rodapé;
    ATRIBUI_COR(VERDE, BRANCO);                   // Atribui cor verde ao fundo e branco para a letra;
    cout << "Este Horário já está liberado \n";
    ATRIBUI_COR(PRETO, BRANCO);                   // Restaura cor de fundo e texto para o padrão;
  }
  else
  {
    cout << "Horário Agendado para " << AGENDA[DIA][HORA - 7].NOME;
    cout << "\n" << "DESMARCAR? (S/N): ";
    cin >> I;
    cin.ignore(80, '\n');
    if (toupper(I) == 'S')                         // Executa as ações caso a resposta seja sim;
    {
      strcpy(AGENDA[DIA][HORA - 7].NOME, VAZIO);   // Atribui valores a posição para apagar o dado que estava gravado;
      POSICIONA_CURSOR(ROD, PADRAO);               // Posiciona o cursor na linha de rodapé;
      ATRIBUI_COR(VERDE, BRANCO);                  // Atribui cor verde ao fundo e branco para a letra;
      cout << "Horário liberado";
    }
  }

  ARQUIVO.write(reinterpret_cast<char*>(&AGENDA), sizeof(AGENDA));  // Grava em AGENDAMENTO.DDP o Registro AGENDA;
  ARQUIVO.flush();                                 // Certifica que os daods foram realmente gravados em AGENDAMENTO.DDP;
  ARQUIVO.close();                                 // Fecha o arquivo AGENDAMENTO.DDP;

  POSICIONA_CURSOR(ROD + 1, PADRAO);               // Posiciona cursor abaixo da linha de rodapé;
  ATRIBUI_COR(PRETO, AZUL);                        // Atribui cor azul para a letra;
  cout << "Tecle <Enter> para ir ao menu" << "\n";
  *SELETOR = 0;                                    // Atribui Zero ao SELETOR para resetar seleção do swicth case;
  cin.get();

  ATRIBUI_COR(PRETO, BRANCO);                      // Restaura cor de fundo e texto para o padrão;
  POSICIONA_CURSOR(ROD, PADRAO);                   // Posiciona o cursor na linha de rodapé;
  cout << ESPACO;                                  // Atribui espaços vazio a linha de rodapé para restaurar cor de fundo padrão;
  LIMPAR_TELA(XPAD, CABEC);                        // Limpa tudo a partir da linha de cabeçalho;
  POSICIONA_CURSOR(LIMPAT, PADRAO);                // Posiciona Cursor abaixo da linha de cabeçalho;
}
/// FIM: Rotina Desmarcar atendimento ---------------------------------------------------------------------------------------------------------------------

/// INICIO: Rotina Listar clientes de um dia --------------------------------------------------------------------------------------------------------------
int LISTAR_DIA(int *SELETOR)
{
  int DIA, HORA;
  LIMPAR_TELA(XPAD, CABEC);                        // Limpa tudo a partir da linha de cabeçalho;
  POSICIONA_CURSOR(LIMPAT, PADRAO);                // Posiciona Cursor abaixo da linha de cabeçalho;

  cout << "\n" << setw(54) << "CLIENTES DO DIA" << "\n\n\n";
  ENTRA_DIA(&DIA);
  cout << endl;

  for (HORA = 1; HORA <= 10; HORA++)
  {
    cout << setw(8) << HORA + 7 << " Horas: " << AGENDA[DIA][HORA].NOME << endl;
  }

  POSICIONA_CURSOR(ROD + 1, PADRAO);               // Posiciona cursor abaixo da linha de rodapé;
  ATRIBUI_COR(PRETO, AZUL);                        // Atribui cor azul para a letra;
  cout << "Tecle <Enter> para ir ao menu";
  ATRIBUI_COR(PRETO, BRANCO);                      // Restaura cor de fundo e texto para o padrão;
  *SELETOR = 0;                                    // Atribui Zero ao SELETOR para resetar seleção do swicth case;
  cin.get();

  LIMPAR_TELA(XPAD, CABEC);                        // Limpa tudo a partir da linha de cabeçalho;
  POSICIONA_CURSOR(LIMPAT, PADRAO);                // Posiciona Cursor abaixo da linha de cabeçalho;
}
/// FIM: Rotina Listar clientes de um dia -----------------------------------------------------------------------------------------------------------------

/// INICIO: Rotina Marcações de um cliente ----------------------------------------------------------------------------------------------------------------
int LISTAR_MARC_CLIENTE(int *SELETOR)
{
  int DIA, HORA;
  char NOME[41];

  LIMPAR_TELA(XPAD, CABEC);                         // Limpa tudo a partir da linha de cabeçalho;
  POSICIONA_CURSOR(LIMPAT, PADRAO);                 // Posiciona Cursor abaixo da linha de cabeçalho;

  cout << endl << setw(58) << "HORÁRIOS DE UM CLIENTE" << "\n\n\n";
  cout << " Entre o nome do cliente: ";
  cin.getline(NOME, sizeof(NOME));                  // Entra Nome para comparar como AGENDA.NOME;

  if (strcmp(NOME, VAZIO) != 0)                     // Executa ação se o Nome inserido for diferente de vazio;
  {
    cout << "\n" << " Horários Agendados para:  " << NOME << "\n\n";
    for (DIA = 1; DIA <= 31; DIA++)
    {
      for (HORA = 1; HORA <= 10; HORA++)
      {
        if (strcmp(AGENDA[DIA][HORA].NOME, NOME) == 0)  // Executa a proxima ação se o Nome inserido for igual ao de AGENDA;
          cout << "DIA " << DIA << " as " << HORA + 7 << " Horas\n";
      }
    }
  }
  else
  {
    POSICIONA_CURSOR(ROD, PADRAO);                   // Posiciona o cursor na linha de rodapé;
    ATRIBUI_COR(VERMEL, BRANCO);                     // Atribui cor Vermelha ao fundo e branco para a letra;
    cout << " É preciso entrar um nome para exibir!!";
    ATRIBUI_COR(PRETO, BRANCO);                      // Restaura cor de fundo e texto para o padrão;
  }

  POSICIONA_CURSOR(ROD + 1, PADRAO);                 // Posiciona cursor abaixo da linha de rodapé;
  ATRIBUI_COR(PRETO, AZUL);                          // Atribui cor azul para a letra;
  cout << "Tecle <Enter> para ir ao menu";
  ATRIBUI_COR(PRETO, BRANCO);                        // Restaura cor de fundo e texto para o padrão;
  *SELETOR = 0;                                      // Atribui Zero ao SELETOR para resetar seleção do swicth case;
  cin.get();

  POSICIONA_CURSOR(ROD, PADRAO);                     // Posiciona o cursor na linha de rodapé;
  cout << ESPACO;                                    // Atribui espaços vazio a linha de rodapé para restaurar cor de fundo padrão;
  LIMPAR_TELA(XPAD, CABEC);                          // Limpa tudo a partir da linha de cabeçalho;
  POSICIONA_CURSOR(LIMPAT, PADRAO);                  // Posiciona Cursor abaixo da linha de cabeçalho;
}
/// FIM: Rotina Marcações de um cliente -------------------------------------------------------------------------------------------------------------------

/// INICIO: Rotina Mapa dos dis Marcados ------------------------------------------------------------------------------------------------------------------
int MAPA_MARCACAO(int *SELETOR)
{
  int DIA, HORA;
  LIMPAR_TELA(XPAD, CABEC);                           // Limpa tudo a partir da linha de cabeçalho;
  POSICIONA_CURSOR(LIMPAT, PADRAO);                   // Posiciona Cursor abaixo da linha de cabeçalho;

  /* Configuração de apresentação da Agenda; */
  cout << endl << setw(60) << "MAPA DOS HORÁRIOS OCUPADOS" << "\n\n\n";
  cout << setw(31) << " HORA" << setw(11) << "1" << setw(10) << "2" << setw(10) << "3" << setw(10) << endl;
  cout << setw(63) << "1234567890123456789012345678901" << endl;

  for(HORA = 1; HORA <= 10; HORA++)                   // Percorre pela matriz das 8 horas até as 17 horas;
  {
    cout << setw(31) << HORA + 7 << " ";
    for(DIA = 1; DIA <= 31; DIA++)                    // Percorre pela matriz do dia 1 até o dia 31;
    {
      (strcmp(AGENDA[DIA][HORA].NOME, VAZIO) == 0) ? cout << "-" : cout << "*"; // Caso Nome não possua cadastros apresenta (-) na tela caso possua apresenta (*);
    }
    cout << endl;
  }

  POSICIONA_CURSOR(ROD + 1, PADRAO);                  // Posiciona cursor abaixo da linha de rodapé;
  ATRIBUI_COR(PRETO, AZUL);                           // Atruibir cor azul as letras;
  cout << "Tecle <Enter> para ir ao menu";
  ATRIBUI_COR(PRETO, BRANCO);                         // Volta os padrões de cor de texto e fundo;
  *SELETOR = 0;                                       // Atribui Zero ao SELETOR para resetar seleção do swicth case;
  cin.get();

  LIMPAR_TELA(XPAD, CABEC);                           // Limpa tudo a partir da linha de cabeçalho;
  POSICIONA_CURSOR(LIMPAT, PADRAO);                   // Posiciona Cursor abaixo da linha de cabeçalho;
}
/// FIM: Rotina Mapa dos dis Marcados ---------------------------------------------------------------------------------------------------------------------

/// INICIO: Rotina Principal do Programa ------------------------------------------------------------------------------------------------------------------
int main(void)
{
  DIMENCIONAR_TELA();                                  // Ajusta as proporções da tela do programa;
  RESET_MATRIZ();                                      // Evita a existência de lixos dentro da matriz global;

  int SELETOR;
  ifstream ARQUIVO("AGENDAMENTO.DDP", ios::binary);    // Define Variavel ARQUIVO como variavel de acesso e abre o arquivo;

  cout << setlocale(LC_ALL, "") << "\n";               // Habilita o idioma padrão do computador (programa configurado para linguagem PT_BR);
  cout << setiosflags(ios::right);                     // Define alinhamento da direita para a esquerda;
  cout << setiosflags(ios::uppercase);                 // Define todos os caracteres como maiúsculo;

  LIMPAR_TELA(0, 0);                                   // Limpa a tela a partir do local do cursor;
  POSICIONA_CURSOR(0, 0);                              // Posiciona cursor no canto superior esquedo da tela;
  /*APRESENTA();*/

  ARQUIVO.read(reinterpret_cast<char*>(&AGENDA), sizeof(AGENDA));  // Le todos os dados do arquivo e grava na matriz agenda;
  ARQUIVO.close();                                     // Fecha o arquivo;

  ATRIBUI_COR(PRETO, AZUL);                            // Atribui cor azul as letras;
  cout << setw(69) << "*** AGENDA PARA MARCAÇÂO DE ATENDIMENTO ***" << "\n\n";
  ATRIBUI_COR(PRETO, BRANCO);                          // Volta as cores para o padrão;

  while (not(SELETOR == 6))                            // Caso SELETOR seja == a 6 sai do laço e encerra programa;
  {
    cout << endl << setw(54) << "MENU PRINCIPAL" << "\n\n";
    cout << setw(30) << " " << "[1] Marcar Atendimento" << "\n";
    cout << setw(30) << " " << "[2] Desmarcar Atendimento" << "\n";
    cout << setw(30) << " " << "[3] Listar Marcarções do Dia" << "\n";
    cout << setw(30) << " " << "[4] Clientes Marcados no Dia" << "\n";
    cout << setw(30) << " " << "[5] Mapa dos Horários Livres" << "\n";
    cout << setw(30) << " " << "[6] Fim do Programa" << "\n\n";

    cout << setw(30) << " " << "==> ";
    cin >> SELETOR;
    cin.ignore(80, '\n');
    if (not(SELETOR == 6))                             // Caso SELETOR seja diferente de 6 executa switch;
    {
       switch (SELETOR)                                // Chama uma das subrotinas e retorna valor zero para resetar o switch ou encerra o programa;
       {
        case 1: CADASTRO(&SELETOR); break;             // Caso SELETOR igual a 1 executa função CADASTRO e recebe um retorno em SELETOR;
        case 2: DESMARCAR(&SELETOR); break;            // Caso SELETOR igual a 2 executa função DESMARCAR e recebe um retorno em SELETOR;
        case 3: LISTAR_DIA(&SELETOR); break;           // CASO SELETOR igual a 3 executa função LISTAR_DIA e recebe um retorno em SELETOR;
        case 4: LISTAR_MARC_CLIENTE(&SELETOR); break;  // Caso SELETOR igual a 4 executa função LISTAR_MARC_CLIENTE e recebe um retorno em SELETOR;
        case 5: MAPA_MARCACAO(&SELETOR); break;        // Caso SELETOR igual a 5 executa função MAPA_MARCACAO e recebe um retorno em SELETOR;
        case 6: break;                                 // Caso SELETOR igual a 6 sai para uma nova verificação do laço;
        default: break;                                // Caso SELETOR igual a qualquer outra resposta sai para nova verificação do laço;
       }
    }
  }

  POSICIONA_CURSOR(ROD + 1, PADRAO);                   // Posiciona cursor abaixo da linha de rodapé;
  ATRIBUI_COR(PRETO, AZUL);
  cout << "Tecle <Enter> para ir ao menu";
  cin.get();
  return 0;
}
/// FIM: Rotina Principal do Programa ---------------------------------------------------------------------------------------------------------------------
