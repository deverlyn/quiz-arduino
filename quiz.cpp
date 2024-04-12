#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

unsigned long tempoInicial;
unsigned long duracao;
bool temporizadorAtivo = false;

int screenWidth = 16;
int screenHeight = 2;

char *bonus = "O Jacare e voa?";
int respBonus = 0;


char *faceis[3][5] = {{"A Terra e redonda?", "O ferro e mole?", "O gato mia?", "O mar e doce?", "A agua ferve?"},
                      {"O Brasil tem praia?", "O cavalo voa?", "A lua e queijo?", "O ar e leve?", "A neve e quente?"},
                      {"O atomo e visivel?", "O Sol e uma estrela?", "A China e pequena?", "O ouro é barato?", "O pinguim nada?"},};

int respFaceis[3][5] = {{1, 0, 1, 0, 1},
                        {1, 0, 0, 1, 0},
                        {0, 1, 0, 0, 1}};


int btnSim = 13;
int btnNao = 12;

int pinLed = 3;
int pinBuzzer = 10; 


// JOGO
int nivel = 0;
int pontos = 0;
int rodando = 0;
int acabou = 0;


void setup()
{
  lcd.begin(screenWidth, screenHeight);
  pinMode(pinLed, OUTPUT);
  pinMode(btnSim, INPUT_PULLUP);
  pinMode(btnNao, INPUT_PULLUP);
  

  Serial.begin(9600);
}

void loop()
{
  lcd.clear();

  inicioDoJogo();
}

void temporizador(int tempoDecorrido) {

  if (tempoDecorrido <= 15 && tempoDecorrido > 10) {
    piscarLed();
  } else if (tempoDecorrido <= 10000 && tempoDecorrido > 5) {
    piscarLed();
  } else if (tempoDecorrido <= 5000 && tempoDecorrido > 0) {
    piscarLed();
  } else {
    pararLed();
  }
}

void piscarLed() {
  digitalWrite(pinLed, HIGH);
}

void pararLed() {
  digitalWrite(pinLed, LOW);
}

void mostrarPontuacao(){
  lcd.setCursor(0, 0);
  lcd.print("Pontos: " + String(pontos));
  delay(4000);
}

void errou(){
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Errou!");
  tone(pinBuzzer, 8000);
  delay(2000);
  noTone(pinBuzzer);
  fimDoJogo();
}

void acertou(){
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Acertou!");
  tone(pinBuzzer, 2000);
  delay(2000);
  noTone(pinBuzzer);
  pontos++;
}

int getRespostaCerta(int nivel, int questao)
{
  int resposta;
  resposta = respFaceis[nivel][questao];
  return resposta;
}

int getRespostaJogador(){
  int resposta = -1; 
  int tempo = 20;
  do{
    if (digitalRead(btnSim) == LOW)
    {
      resposta = 1;
    }
    else if (digitalRead(btnNao) == LOW) 
    {
      resposta = 0;
    } 
    else if (tempo == 0)
    {
      resposta = 2;
    }
    temporizador(tempo);
    delay(1000);
    tempo--;
  } while (resposta == -1);

  return resposta;
}


String getPergunta(int nivel, int questao){
  String pergunta;
  pergunta = String(faceis[nivel][questao]);
  return pergunta;
}

void scroll(int stringLength){
  for (int positionCounter = 0; positionCounter < stringLength - screenWidth; positionCounter++)
  {
    lcd.scrollDisplayLeft();
    delay(1000);
  }
}

void mensageria(int nivel){
  if (nivel < 2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Next Level: ");
  lcd.setCursor(0, 1);
  lcd.print(nivel + 2);
  delay(2000); 
  } else { 
    lcd.clear();
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print("GAME OVER!");
    delay(2500);
  }

}

int mostrarPergunta(String pergunta)
{	
  int resposta = -1;
  do{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(pergunta);
    scroll(pergunta.length());
    delay(100);
    resposta = getRespostaJogador();
  } while (resposta == -1);

  return resposta;
}

void acabouOTempo(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Tempo Esgotado!");
  delay(1500);
  lcd.clear();
}

void jogar()
{
  lcd.clear();
  delay(1000);
  for (int nivel = 0; nivel < 3; nivel++) {
    for (int numPergunta = 0; numPergunta < 5; numPergunta++) {
      int respostaUser = mostrarPergunta(getPergunta(nivel, numPergunta));
      int respostaCerta = getRespostaCerta(nivel, numPergunta);
      if (respostaUser == respostaCerta){
      acertou();
      }
      else if (respostaUser == 2){
        acabouOTempo();
      } else {
      errou();
      } 
    }
    mensageria(nivel);
  }

  lcd.clear();
  delay(2000);

  fimDoJogo();
}

void novoJogo()
{
  nivel = 1;
  pontos = 0;
  rodando = 1;
  do {
    jogar();
  } while (rodando == 1);
}

void inicioDoJogo()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("QUIZ DEVERLYN");
    lcd.setCursor(0, 1);
    lcd.print(" Iniciar jogo? ");
    scroll(15);
  	delay(100);
    
    lcd.clear();
  if (digitalRead(btnSim) == LOW){
      lcd.clear();
      novoJogo();
  }
}

void fimDoJogo()
{
  mostrarPontuacao();
  lcd.setCursor(0, 1);
  lcd.print("FIM DE JOGO");
  rodando = 0;
  delay(1000);
  novoJogo();
}