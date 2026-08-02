#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>
#include <locale.h>
#include <time.h>
#include <windows.h>

#define MINIAUDIO_IMPLEMENTATION
#include "../libraries/miniaudio.h"

#define FLASHING             "\033[5m"
#define RESET                "\033[0m"
#define BACKGROUND_Orange    "\033[48;5;208m"
#define BACKGROUND_White     "\033[47m"
#define BACKGROUND_Green     "\033[48;5;120m"
#define BACKGROUND_Brown     "\033[48;5;137m"
#define BACKGROUND_Blue      "\033[48;5;21m"
#define BACKGROUND_Red       "\033[48;5;88m"
#define Black                "\033[0;30m"
#define Red                  "\033[0;31m"
#define Green                "\033[38;5;196m"
#define Yellow               "\033[0;33m"
#define Blue                 "\033[0;34m"
#define Magenta              "\033[0;35m"
#define Cyan                 "\033[0;36m"
#define White                "\033[0;37m"
#define Gray                 "\033[38;5;244m"

#define map_Columns     141
#define map_Lines       46
#define walls_Quantity  2000

// Mapa
char* mapArray[map_Columns][map_Lines];
int arrayInitialColumn=0, arrayFinaColumn=140, arrayInitialLine=0, arrayFinalLine=45;
int wallsPos = 0;

// Coordenadas base da posição do mapa na tela
int initialColumn = 10, finalColumn = 150, initialLine = 5, finalLine = 50;

// Booleanos do Jogo
bool isGameRunning = false;

// Áudio
ma_engine engine;
ma_sound sound;

struct Characters
{
    int life, attack, defense, posX, posY;
    char* sprite;
} player, monsters[30], boss;

struct Progress
{
    int kills, difficulty, bonus_attack, bonus_defense;
    bool exit;
} user;

struct Walls
{
    int posX, posY;
    char* sprite;
} walls[walls_Quantity];

        // Função usada para mover o cursor de texto para uma coordenada específica dentro da janela do console.
void gotoXY(int x, int y)
{
COORD c;
c.X = x;
c.Y = y;
SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

        // Contém os Status do Boss
void Boss_Stats()
{
    boss.life = 15;
    boss.attack = 4;
    boss.defense = 2;
    boss.posX = 100;
    boss.posY = 30;
    boss.sprite = "\u00D6";          // Ö
}

        // Contém os Status do Player
void Player_Stats()
{
    player.life = 30;
    player.attack = 5;
    player.defense = 2;
    player.posX = 0;
    player.posY = 0;
    player.sprite = "\u2640";
}

        // Contém os Status dos Monstros
void Monster_Stats(int monstersQuantity)
{
    for(int q=0; q < monstersQuantity; q++)
    {
        monsters[q].life = 8;
        monsters[q].attack = 2;
        monsters[q].defense = 1;
    }
}

void Player_Progress()
{
    user.kills = 0;
    user.difficulty = 1;
    user.bonus_attack = 0;
    user.bonus_defense = 0;
    user.exit = false;
}


        // Desenha a Borda da masmorra para maior clareza ao jogar.
void Border(int initialColumn, int finalColumn, int initialLine, int finalLine)
{
    int c, l;
    char* characters;

    characters = "\u2550";
    for(c = initialColumn; c <= finalColumn; c++)
    {
        gotoXY(c, initialLine-1); printf("%s", characters);
        gotoXY(c, finalLine+1); printf("%s", characters);
    }

    characters = "\u2551";
    for (l = initialLine; l <= finalLine; l++)
    {
        gotoXY(initialColumn-1, l); printf("%s", characters);
        gotoXY(finalColumn+1, l); printf("%s", characters);
    }

    characters = "\u2554";
    gotoXY(initialColumn-1, initialLine-1); printf("%s", characters);

    characters = "\u2557";
    gotoXY(finalColumn+1, initialLine-1); printf("%s", characters);

    characters = "\u255A";
    gotoXY(initialColumn-1, finalLine+1); printf("%s", characters);

    characters = "\u255D";
    gotoXY(finalColumn+1, finalLine+1); printf("%s", characters);
}

        // Armazena as posições da Masmorra e printa espaços vazios.
void Map(int initialColumn, int finalColumn, int initialLine, int finalLine)
{
    for(int c = 0; c <= ((finalColumn + 1) - (initialColumn + 1)); c++)
    {
        for(int l = 0; l <= ((finalLine + 1) - (initialLine + 1)); l++)
        {
            gotoXY(c + initialColumn, l + initialLine);
            mapArray[c][l] = " ";
            printf(BACKGROUND_Green "%s" RESET, mapArray[c][l]);
        }
    }
}

bool Area_Validation(int columns, int lines, int mapC, int mapL)
{
    if ((mapC + columns) > map_Columns || (mapL + lines) > map_Lines)
    {
        return false;
    }

    int area = columns * lines;
    int contArea = 0;

    for(int i=mapC; i < mapC + columns; i++)
    {
        for (int j=mapL; j < mapL + lines; j++)
        {
            if((strcmp(mapArray[i][j], " ") == 0) &&
               (i != arrayInitialColumn && j != arrayFinaColumn && i != arrayInitialColumn+1 && j != arrayInitialLine+1) &&
               (i != arrayFinaColumn && j != arrayFinalLine && i != arrayFinaColumn-1 && j != arrayFinalLine-1))
            {
                contArea++;
            }
        }
    }
    if(contArea == area)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Walls(int wallsQuantity)
{
    for(int q = 0; q < wallsQuantity; q++)
    {
        bool verificator = false;
        int columns;
        int lines;

        int c;
        int l;

        while(!verificator)
        {
            columns = (rand() % 10) + 11;
            lines = (rand() % 5) + 4;

            c = rand() % map_Columns;
            l = rand() % map_Lines;

            verificator = Area_Validation(columns, lines, c, l);
        }

        for(int i = c; i < (c + columns); i++)
        {
            for(int j = l; j < (l + lines); j++)
            {
                walls[wallsPos].sprite = "\u256C";      // "\u2588";
                walls[wallsPos].posX = i;
                walls[wallsPos].posY = j;

                mapArray[i][j] = walls[wallsPos].sprite;

                gotoXY(i + initialColumn, j + initialLine);
                printf(BACKGROUND_Red "%s" Black RESET, walls[wallsPos].sprite);

                wallsPos++;
            }
        }
    }

    /*
    for(int q = 0; q < walls_Quantity; q++)
    {
        walls[q].sprite = "\u2588";     // █

        bool verificator = false;

        while(!verificator)
        {
            int i = rand() % map_Columns;
            int j = rand() % map_Lines;

            if((strcmp(mapArray[i][j], " ") == 0) && (strcmp(mapArray[i+1][j], " ") == 0) && (strcmp(mapArray[i][j+1], " ") == 0) && (strcmp(mapArray[i+1][j+1], " ") == 0))
            {
                if((i+1 < map_Columns) && (j+1 < map_Lines))
                {
                    mapArray[i][j] = walls[q].sprite;
                    mapArray[i+1][j] = walls[q].sprite;
                    mapArray[i][j+1] = walls[q].sprite;
                    mapArray[i+1][j+1] = walls[q].sprite;

                    walls[q].posX = i;
                    walls[q].posY = j;

                    gotoXY(i+initialColumn, j+initialLine);
                    printf(Gray "%s" BACKGROUND_Green RESET, mapArray[walls[q].posX][walls[q].posY]);

                    gotoXY(i+1+initialColumn, j+initialLine);
                    printf(Gray "%s" BACKGROUND_Green RESET, mapArray[walls[q].posX][walls[q].posY]);

                    gotoXY(i+initialColumn, j+1+initialLine);
                    printf(Gray "%s" BACKGROUND_Green RESET, mapArray[walls[q].posX][walls[q].posY]);

                    gotoXY(i+1+initialColumn, j+1+initialLine);
                    printf(Gray "%s" BACKGROUND_Green RESET, mapArray[walls[q].posX][walls[q].posY]);

                    verificator = true;
                }
            }
        }
    }*/
}

        // Armazena a posição da Saída e printa na tela
void Exit_Position(int initialColumn, int finalColumn, int initialLine, int finalLine)
{
    gotoXY(finalColumn, finalLine);
    mapArray[finalColumn-initialColumn][finalLine-initialLine] = "\u2229";   //"∩";
    printf(Yellow BACKGROUND_Blue "%s" RESET, mapArray[finalColumn-initialColumn][finalLine-initialLine]);
}

        // Sorteia aleatoriamente um Monstro dentro do Array de monstros.
char* Monsters()
{
    char* variableMonsters[] = {"\u03A8", "\u03A6", "\u04DC", "\u046A", "\u03A9"};      //{"Ψ", "Φ", "Ӝ", "Ѫ", "Ω"};

    int randomized = rand() % (sizeof(variableMonsters) / sizeof(variableMonsters[0]));

    char* sortedMonster = variableMonsters[randomized];

    return sortedMonster;
}

        // Define e Armazena a posição ao qual cada montro estará e será printado na tela.
void Monster_Position(int monstersQuantity, int initialColumn, int initialLine)
{
    Monster_Stats(monstersQuantity);

    for(int q = 0; q < monstersQuantity; q++)
    {
        bool verificator = false;

        while(!verificator)
        {
            int i = rand() % map_Columns;
            int j = rand() % map_Lines;

            if(!(i == arrayInitialColumn && j == arrayInitialLine) && !(i == arrayFinaColumn && j == arrayFinalLine) && !(i == boss.posX && j == boss.posY))
            {
                if(strcmp(mapArray[i][j], " ") == 0)
                {
                    gotoXY(i + initialColumn, j + initialLine);

                    monsters[q].sprite = Monsters();

                    mapArray[i][j] = monsters[q].sprite;

                    monsters[q].posX = i;
                    monsters[q].posY = j;

                    printf(Red BACKGROUND_Green "%s" RESET, mapArray[monsters[q].posX][monsters[q].posY]);

                    verificator = true;
                }
            }
        }
    }
}

        // Armazena a posição do Boss e printa na tela
void Boss_Position(int initialColumn, int initialLine)
{
    Boss_Stats();

    if(strcmp(mapArray[boss.posX][boss.posY], " ") == 0)
    {
        gotoXY(boss.posX + initialColumn, boss.posY + initialLine);
        mapArray[boss.posX][boss.posY] = boss.sprite; // ⚗
        printf(Black BACKGROUND_Green "%s" RESET, mapArray[boss.posX][boss.posY]);
    }
}

        // Armazena a posição do Player e printa na tela
void Player_Position(int initialColumn, int initialLine)
{
    gotoXY(initialColumn + player.posX, initialLine + player.posY);
    mapArray[player.posX][player.posY] = player.sprite; // 🕴️
    printf(Blue BACKGROUND_Green "%s" RESET, mapArray[player.posX][player.posY]);
}

        // Recebe e calcula o movimento que o Player fará
void Player_Movement(int initialColumn, int finalColumn, int initialLine, int finalLine)
{
    Player_Stats();

    while (true)
    {
        Player_Position(initialColumn, initialLine);

        gotoXY(finalColumn+3, finalLine+3);
        char key = getch();

        gotoXY(initialColumn + player.posX, initialLine + player.posY);
        mapArray[player.posX][player.posY] = " ";
        printf(BACKGROUND_Green " " RESET);

        if((key == 'w' || key == 'W') && player.posY > 0 && strcmp(mapArray[player.posX][player.posY-1], "\u256C") != 0)
        {
            player.posY--;
        }
        else if((key == 's' || key == 'S') && player.posY < 45 && strcmp(mapArray[player.posX][player.posY+1], "\u256C") != 0)
        {
            player.posY++;
        }
        else if((key == 'a' || key == 'A') && player.posX > 0 && strcmp(mapArray[player.posX-1][player.posY], "\u256C") != 0)
        {
            player.posX--;
        }
        else if ((key == 'd' || key == 'D') && player.posX < 140 && strcmp(mapArray[player.posX+1][player.posY], "\u256C") != 0)
        {
            player.posX++;
        }
        else if(key == 27)
        {
            isGameRunning = false;
            break;
        }
    }
}

void Menu()     // c210 l60     Pc56 Pl6
{
    char* purusu[7] = {
        Red "████████╗   ███╗    ███╗  ████████═╗  ███╗    ███╗  ████████╗  ███╗    ███╗",
            "████╔═███╗  ███║    ███║  ███╔══███║  ███║    ███║  ██╔═════╝  ███║    ███║",
            "████║ ███║  ███║    ███║  ███║  ███║  ███║    ███║  ████████╗  ███║    ███║",
            "████████╔╝  ███║    ███║  ███████╔═╝  ███║    ███║  ╚═════██║  ███║    ███║",
            "████╔═══╝   ████   ████║  ███╔═███═╗  ████   ████║  ██    ██║  ████   ████║",
            "████║        ╚███████╔═╝  ███║  ███║   ╚███████╔═╝  ████████║   ╚███████╔═╝",
            "╚═══╝         ╚══════╝    ╚══╝  ╚══╝    ╚══════╝    ╚═══════╝     ╚═════╝"RESET
    };

    for(int i=0, l=10; i < 7; i++, l++)
    {
        gotoXY(68, l);
        printf("%s", purusu[i]);
    }

    char* play[5] = {
        " ██╗               ███╗   █████╗   █████╗   ███╗   ████═╗",
        "███║               ╚██║  ██╔══██╗ ██╔═══╝  █╔══█╗  █╔═██║",
        " ██║    ════    ██╗ ██║  ██║  ██║ ██║  ██╗ █████║  ████═╗",
        " ██║            ╚████╔╝  ╚█████╔╝ ╚█████╔╝ █║  █║  █║ ██║",
        " ╚═╝             ╚═══╝    ╚════╝   ╚════╝  ╚╝  ╚╝  ╚╝  ╚╝",
    };

    for(int i=0, l=40; i < 5; i++, l++)
    {
        gotoXY(40, l);
        printf("%s", play[i]);
    }

    char* leave[5] = {
        "▄███▄╗            ▄████▄╗   ███╗   █████╗  ████═╗ ",
        "█╔═██║            ██▄▄══╝  █╔══█╗  ╚═█╔═╝  █╔═██║",
        " ▄█▀╔╝    ════     ╚═▀█▄╗  █████║    █║    ████═╗",
        "█████╗            ▀████▀║  █║  █║  █████╗  █║ ██║",
        "╚════╝             ╚════╝  ╚╝  ╚╝  ╚════╝  ╚╝  ╚╝",
    };

    for(int i=0, l=40; i < 5; i++, l++)
    {
        gotoXY(121, l);
        printf("%s", leave[i]);
    }

    while(!isGameRunning)
    {
        char key = getch();

        if(key == '1')
        {
            isGameRunning = !isGameRunning;
        }
        if(key == '2')
        {
            break;
        }
        else if (key != '1' && key != '2')
        {
            gotoXY(98, 55);
            printf("OPÇÃO INVÁLIDA!");
        }
    }
}

void Start_Sound_Track()
{
    if(ma_engine_init(NULL, &engine) != MA_SUCCESS)
    {
        gotoXY(finalColumn+5, finalLine+5);
        printf("Erro ao inicializar a engine de áudio!");
    }

    ma_result result = ma_sound_init_from_file(&engine, "sounds/lost_woods.mp3", 0, NULL, NULL, &sound);

    if(result != MA_SUCCESS)
    {
        gotoXY(finalColumn+5, finalLine+5);
        printf("Erro ao carregar o áudio!");
        ma_engine_uninit(&engine);
    }

    ma_sound_set_volume(&sound, 0.1f);

    ma_sound_set_pitch(&sound, 0.8f);

    ma_sound_set_looping(&sound, MA_TRUE);

    ma_sound_start(&sound);
}

void End_Sound_Track()
{
    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);
}

void Terminal_Config()
{
        // Obtém o identificador da saída padrão do terminal (stdout)
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

        // Obtém o modo atual do terminal
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

        // Ativa o suporte a cores e comandos de renderização ANSI
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

        // Aplica a nova configuração no terminal do Windows
    SetConsoleMode(hOut, dwMode);

        // Informa qual a codificação que será usada nas funções do C.
    setlocale(LC_ALL, ".utf8");

        // Configura o console do Windows para aceitar e exibir caracteres no formato UTF-8
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
}

void Hide_Cursor() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info = {1, FALSE};
    SetConsoleCursorInfo(h, &info);
}



int main()
{
        // Configura o terminal pra funcionar de acordo com a utf8 (acredito que so precise para o meu PC)
    Terminal_Config();

        // Diminui a previsibilidade das randomizações.
    srand(time(NULL));

    Hide_Cursor();

    Start_Sound_Track();

    Menu();

    while(true)
    {
        if(isGameRunning)
        {
            Player_Stats();

            Boss_Stats();

            Map(initialColumn, finalColumn, initialLine, finalLine);

            Border(initialColumn, finalColumn, initialLine, finalLine);

            Boss_Position(initialColumn, initialLine);

            Walls(10);

            Monster_Position(30, initialColumn, initialLine); //6484

            Exit_Position(initialColumn, finalColumn, initialLine, finalLine);

            Player_Movement(initialColumn, finalColumn, initialLine, finalLine);
        }
        else if(!isGameRunning)
        {
            break;
        }
    }

    End_Sound_Track();

    gotoXY(finalColumn+3, finalLine+3);
    printf("[%d]", monsters[31].life);
}
