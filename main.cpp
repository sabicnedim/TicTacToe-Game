#include "mbed.h"
#include "stdio.h"
#include "SPI_TFT_ILI9341.h"
#include "string"
#include "Arial12x12.h"
#include "Arial24x23.h"
#include "Arial28x28.h"
#include "font_big.h"
#include <cstdio>

#define dp23 P0_0

SPI_TFT_ILI9341 TFT(PTD2,PTD3,PTD1,PTD0,PTD5,PTA13,"TFT");
// mosi,miso,sclk,cs ,reset, dc

Ticker joystick; // Ticker za kontrolu igre
InterruptIn button(PTD7); // Izvrsava se prekidna rutina pritiskom na joystick

AnalogIn x(PTC2); // kretanje po x osi
AnalogIn y(PTB3); // kretanje po y osi 

bool restart = 1; // Varijabla koja signalizra reset igre i vracanje na pocetni zaslon
int new_game = 1; // Varijabla koja signalizira da se može ponovo igrati

int mode_play = 0; // Varijabla koja signalizira da se igrica prebaci u mod za igranje
int mode_score = 0; // Varijabla koja signalizira da se igrica prebaci u mod koji prikazuje trenutni rezultat


int topOptionSelected = 1; // Varijabla "topOptionSelected" signalizira da je selektovana gornja opcija u skoru.
int bottomOptionSelected  = 0; // Varijabla "bottomOptionSelected" signalizira da je selektovana donja opcija u skoru.


int x_value = 64; // inicijalna x koordinata na displeju (sluzi za kretanje kroz mrezu)
int y_value = 48; // inicijalna y koordinata na displeju (sluzi za kretanje kroz mrezu)
int x_value_prev = 64; 
int y_value_prev = 48;

bool currentPlayer = 1; // Varijabla "player" koristi se za praćenje trenutno aktivnog igrača.
                // Ako je "currentPlayer" postavljen na 1, trenutno je na potezu igrač X.
                // Ako je "currentPlayer" postavljen na 0, trenutno je na potezu igrač O.


int playerXScore = 0, playerOScore = 0; // Varijable u koje se upisuje broj pobjeda igraca X i O respektivno


char gameBoard[3][3] = {{' ',' ',' '},{' ',' ',' '},{' ',' ',' '}}; // Matrica "gameBoard" koristi se za praćenje stanja igre.
                                                                    // Svaka ćelija u matrici može sadržavati simbol X, O ili biti prazna.

int play_button = 0; // Varijabla koja signalizira da je na "Play" pritisnut joystick

int cellCount = 0; // "cellCount" broji popunjena polja.





// Funkcija "initializeDisplay" postavlja display na početno stanje.
void initializeDisplay(){
    TFT.claim(stdout);
    TFT.set_orientation(1);
    TFT.background(White);
    TFT.cls();
}


// Funkcija "mainMenu" prikazuje početni zaslon igre
void mainMenu(){
    TFT.cls();
    TFT.set_font((unsigned char*) Arial28x28);
    TFT.foreground(Black);
    TFT.locate(70,50);
    TFT._printf("Tic-Tac-Toe",11);
    TFT.set_font((unsigned char*) Arial24x23);
    TFT.locate(70,100);
    TFT._printf("Play",4);
    mode_play = 0;
    mode_score = 0;
    TFT.rect(64,95,150,125, Black);
}

// Funkcija "highlitField" crta kvadratiće kao indikator da smo u nekom polju.

void highlightField(){
    TFT.cls();
    TFT.line(128, 48, 128, 192, Black);
    TFT.line(128+64, 48, 128+64, 192, Black);
    TFT.line(64, 96, 256, 96, Black);
    TFT.line(64, 144, 256, 144, Black);
    TFT.pixel(96,72, Black);
    TFT.rect(x_value+15, y_value + 5, x_value+64-15, y_value+48-10, Black);
    TFT.locate(32, 5);
    TFT.foreground(Blue);
    TFT._printf("Player x", 8); 
}

// Funkcija "clearBoard" isprazni matricu postavljajući sve ćelije na početne vrijednosti.
void clearBoard(){
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++) 
            gameBoard[i][j] = ' ';
   
    mode_play = 0;
    currentPlayer = 1;
    x_value = 64;
    y_value = 48;
}

// Funkcija "displayCurrentScore" prikazuje trenutni rezultat igre korisniku.

void displayCurrentScore(){
    char array[30];
    mode_play = 0;
    TFT.cls();
    TFT.set_font((unsigned char*) Arial24x23);
    TFT.foreground(Blue);
    TFT.locate(150, 20);
    TFT._printf("X", 1);
    TFT.set_font((unsigned char*) Arial24x23);
    TFT.foreground(Red);
    TFT.locate(210, 20);
    TFT._printf("O", 1);
    TFT.set_font((unsigned char*) Arial24x23);
    TFT.foreground(Black);
    TFT.locate(50, 50);
    sprintf(array, "Score %d : %d", playerXScore, playerOScore);
    TFT._printf(array, 12);

    TFT.set_font((unsigned char*) Arial12x12);   
    TFT.locate(70, 100);
    TFT._printf("Main menu",9);
    TFT.locate(70, 137);
    TFT._printf("Play again",10);

    TFT.set_font((unsigned char*) Arial24x23);
    TFT.rect(64, 90, 150, 120, Black);
    mode_score = 1;
    mode_play = 0;
}


// Funkcija "checkGameStatus" provjerava status igre.
// Provjerava da li je neko pobijedio ili je igra završena neriješeno.

void checkGameStatus(char player){
    
    switch(player){
        case 'x':
            playerXScore++;
            TFT.cls();
            TFT.locate(30,120);
            TFT._printf("Winner: Player X", 16);
            cellCount = 0;
            clearBoard();
            displayCurrentScore();
        break;

        case 'o':
            playerOScore++;
            TFT.cls();
            TFT.locate(30,120);
            TFT._printf("Winner: Player O", 16);
            cellCount = 0;
            clearBoard();
            displayCurrentScore();
        break;

        case 'n':
            TFT.cls();
            TFT.locate(120,120);
            TFT.foreground(Black);
            TFT._printf("Draw", 4);
            cellCount = 0;
            clearBoard();
            displayCurrentScore();
        break;

    }
}


// Funkcija "checkWinCondition" provjerava da li je neko povezao tri simbola u nizu.
// Provjerava sve moguće kombinacije (horizontalno, vertikalno i dijagonalno).

int checkWinCondition(bool symbol){
    
    char player;
    if(symbol) player = 'x'; 
    else player = 'o';
    
    // redovi
    for (int i=0;i<3;i++){
            if(gameBoard[i][0]== player && gameBoard[i][1] == player && gameBoard[i][2] == player){ 
                checkGameStatus(player);
                return 1;
                }
    }
 
    // kolone
    for(int i=0;i<3;i++){
        if(gameBoard[0][i] == player && gameBoard[1][i] == player && gameBoard[2][i] == player) {
            checkGameStatus(player);
            return 1;
            }
    }
   
    //dijagonale
    if ((gameBoard[0][0] == player && gameBoard[1][1] == player && gameBoard[2][2] == player) || (gameBoard[0][2] == player && gameBoard[1][1] == player && gameBoard[2][0] == player)) {
        checkGameStatus(player);
        return 1;
        }
   
    // Ako je matrica popunjena a nisu povezana tri simbola u nizu
    if(cellCount == 9){
        checkGameStatus('n'); cellCount = 0;}
    
    return 0;
}


// Funkcija koja omogućava novu igru uz pamćenje trenutnog rezultata

void new_Game(){
    if(mode_play == 1){
    int i  = x_value / 64 - 1;
    int j = y_value / 48 - 1;
        if(gameBoard[i][j] == ' '){
            cellCount++;
            if(currentPlayer) {
                TFT.locate(32, 5);
                TFT.foreground(Red);
                TFT._printf("Player o", 8);
                TFT.foreground(Blue);
                TFT.locate(x_value + 25, y_value + 10);
                TFT._putc('x');  gameBoard[i][j] = 'x'; currentPlayer = !currentPlayer; checkWinCondition(bool(1));
            }
            else {
                TFT.locate(32, 5);
                TFT.foreground(Blue);
                TFT._printf("Player x", 8);
                TFT.foreground(Red);
                TFT.locate(x_value + 25, y_value + 10);
                TFT._putc('o'); gameBoard[i][j] = 'o'; currentPlayer = !currentPlayer; checkWinCondition(0);
                }
        }
    }
}



// Funkcija "joystickPressAction" se izvršava kada se pritisne džojstik.
// Na osnovu trenutnog konteksta igre, ili upisuje u polje ili vrši selekciju na odgovarajucem izborniku.

void joystickPressAction(){ 
         
    if(mode_score == 1 && topOptionSelected == 1 && bottomOptionSelected  == 0 && restart == 1){
        mainMenu(); playerXScore = 0; playerOScore = 0;  return;
    }
    if(mode_score == 1 && bottomOptionSelected  == 1 && topOptionSelected == 0 && new_game == 1){
        mode_score = 0; mode_play = 1; topOptionSelected = 1;
        bottomOptionSelected  = 0;
        clearBoard();
        highlightField();
        new_Game(); mode_play = 1;  return;
    }

    if(mode_play == 0){
        highlightField(); mode_play = 1; 
        }

    else if(mode_play == 1 && play_button == 0){

       int i  = x_value / 64 - 1;
       int j = y_value / 48 - 1;

        if(gameBoard[i][j] == ' '){
            cellCount++;
            if(currentPlayer) {
                
                TFT.locate(32, 5);
                TFT.foreground(Red);
                TFT._printf("Player o", 8);

                TFT.foreground(Blue);
                TFT.locate(x_value + 25, y_value + 10);
                TFT._putc('x');  gameBoard[i][j] = 'x'; currentPlayer = !currentPlayer; checkWinCondition(bool(1));}
            else {
                TFT.locate(32, 5);
                TFT.foreground(Blue);
                TFT._printf("Player x", 8);

                TFT.foreground(Red);
                TFT.locate(x_value + 25, y_value + 10);
                TFT._putc('o'); gameBoard[i][j] = 'o'; currentPlayer = !currentPlayer; checkWinCondition(0);}
            }
    }

}


// Funkcija "moveRight" pomakne selekciju polja desno.

void moveRight(){
        TFT.rect(x_value_prev +15, y_value + 5, x_value_prev + 64-15, y_value+48  -10, White);
        TFT.rect(x_value+15, y_value + 5, x_value+64-15, y_value+48-10, Black);
}

// Funkcija "moveLeft" pomakne selekciju polja lijevo.

void moveLeft(){

        TFT.rect(x_value_prev +15, y_value + 5, x_value_prev + 64-15, y_value+48  -10, White);
        TFT.rect(x_value+15, y_value + 5, x_value+64-15, y_value+48-10, Black);
}

// Funkcija "moveUp" pomakne selekciju polja prema gore.

void moveUp(){

        if(mode_play && mode_score == 0){
        TFT.rect(x_value +15, y_value_prev + 5, x_value + 64-15, y_value_prev+48  -10, White);
        TFT.rect(x_value+15, y_value + 5, x_value+64-15, y_value+48-10, Black);}
        
        if(mode_play == 0 && mode_score == 0){
            TFT.rect(64 , 95, 150, 125, Black); 
            mode_play = 0; 
            play_button = 0;
        }


        if(mode_play == 0 && mode_score == 1){
            restart = 1; 
            topOptionSelected = 1;
            bottomOptionSelected  = 0;
            TFT.rect(64 , 128, 150, 158, White);
            TFT.rect(64, 90, 150, 120, Black);
        }      
}

// Funkcija "moveDown" pomakne selekciju polja prema dolje.

void moveDown(){

        if(mode_play && mode_score == 0){
        TFT.rect(x_value +15, y_value_prev + 5, x_value + 64-15, y_value_prev+48  -10, White);
        TFT.rect(x_value+15, y_value + 5, x_value+64-15, y_value+48-10, Black);}

       if(mode_score == 1){
            bottomOptionSelected  = 1;
            topOptionSelected = 0;
            new_game = 1;
            TFT.rect(64 , 128, 150, 158, Black);
            TFT.rect(64, 90, 150, 120, White);
        }
}


// Funkcija "readJoystick" čita vrijednosti s analognog džojstika.
// Na osnovu očitanih vrijednosti, vrši se kretanje desno, lijevo, gore ili dolje.

void readJoystick(){
    if(mode_play == 1){
    if(x.read() > 0.9){
        x_value_prev = x_value;
        if(x_value == 192) x_value = 192;
        else x_value += 64;
        moveRight();}
    if (x.read() < 0.1){
        x_value_prev = x_value;
        if(x_value == 64) x_value = 64;
        else x_value -= 64;
        moveLeft();
    }
    if (y.read() < 0.1){
        y_value_prev = y_value;
        if(y_value == 48) y_value = 48;
        else y_value -= 48;
        moveUp();}
    if (y.read() > 0.9){
         y_value_prev = y_value;
        if(y_value==144) y_value = 144;
        else y_value += 48;
        moveDown();}
    return;   
    }
    if (y.read() < 0.1) moveUp();
    else if(y.read()> 0.9) moveDown();
}

int main() {

    initializeDisplay();
    mainMenu();
    button.fall(&joystickPressAction);
    joystick.attach(&readJoystick, 0.2);

while(1) {}

}