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

#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004

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

// sprites
char* playerBattleSprite[43] = {
    "        ███████████        ███████████████                      ",
    "     ███████████████     █████████████████████                  ",
    "   ███   ████████████    ███████████████████                    ",
    "  ██     ████████████  █████████████████████                    ",
    "       █████████████████████████████████████                    ",
    "       ██████████████████████████████▒▒▒▒█████                  ",
    "     █████████████   ████████████████▒▒▒▒███████                ",
    "     ████     ██     ███████▒▒██▒▒███▒▒▒▒▒▒▒████                ",
    "     ██                █████▒▒██▒▒▒██▒▒▒▒▒▒   ██                ",
    "     █                 █████▒▒██▒▒▒▒▒██▒▒▒▒▒  ██                ",
    "                       ███▒███▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒                   ",
    "                     ██▒▒▒▒▒▒▒██▒▒▒▒▒▒▒▒▒▒▒▒                    ",
    "                     ████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒                    ",
    "                  ████████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒                     ",
    "                ██████████████▒▒▒▒▒▒▒██▒▒                       ",
    "                ████████████████▒▒▒▒▒▒▒██                   ████",
    "              █████████████████████▒▒▒▒▒▒▒██              ██████",
    "              ███████████████████████▒▒▒▒▒██            ██████  ",
    "              ███████████████████████▒▒▒▒█████▒▒▒▒▒▒▒▒▒█████    ",
    "              █████████████████████████▒▒█████▒▒▒▒▒▒▒▒▒▒████    ",
    "              ███████████▒▒▒████████████████▒▒██▒▒▒▒▒█████▒     ",
    "              ███████████▒▒▒▒▒▒▒▒▒▒██████████████████▒▒▒▒       ",
    "              █████████████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒█████████         ",
    "                ███████████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒████████         ",
    "                ████████████▒▒██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒███████         ",
    "                   █████████████████████████▒▒▒▒▒████           ",
    "                   ░░░░░░░████░░░░░░░░░░░░░░░░░                 ",
    "              █████░░░░░░███████████████████████                ",
    "              ██████████████████████████████████                ",
    "       ███████████████████████████████████████████              ",
    "       ███████████████████████████████████████████              ",
    "         ███████     ████████████████████████████████           ",
    "                   ██████████████████████████████████           ",
    "                  ███████████████████████████████████           ",
    "                  ███████████████████████████ ███████           ",
    "               █░░██████████████████████████  ███████           ",
    "            █░░░░░░░░░░██████████████████      ██████           ",
    "            █░░░░░░░░░░░░░░░█████████████     ███████           ",
    "             ░░░░░░░░░░░░░░░░░░░░░░░░░░██     ███████           ",
    "              ░░░░░░░░░░░░░░░░░░░░░░░░░░█      █████            ",
    "                ████░░░░░░░░░░░░░░░░░░░░█     ██▒▒▒▒            ",
    "               ▒▒▒███████████████████░░░       █▒▒▒██▒▒         ",
    "              ███████████                     ███████████       "
};

char* monsterBattleSprite[31] = {
"          ████████████                ",
"        ███████████████               ",
"       ██████████████████             ",
"      ████▒▒▒▒▒▒▒▒▒██████             ",
"      █▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██             ",
"      ▒▒▓▓▓▒▒▒▒▒▒▓▓▓▒▒▒██             ",
"     ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██           ",
"    ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒█████        ",
"    ▒▒▒▒▒▓▓▓▓▓▒▒▒▒▒▒▒▒▒███████        ",
"      ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒███████████      ",
"        ██▓▓▒▒▒▓▓▓▓▓█████████████     ",
"        ███▓▓▓▓▓▓▓▓████████████████   ",
"       ████▓▓▓▓▓▓▓██████████████████  ",
"      ██████▓▓▓▓▓▓██████████████████  ",
"   █████████▓▓▓▓▓███████████████████  ",
" ███████████▓▓▓▓▓████████████████████ ",
"█████████████▓▓▓██████████████████████",
"▒▒████████████████████████████████████",
"▒▒▒▒▒████████████████████████████████ ",
" ▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ ▒▒▒█████  ",
"     ██████████████████████ ▒▒▒▒▒▒▒▒  ",
"     ███████████ ██████████ ▒▒▒▒▒▒▒   ",
"    ███████████   ██████████          ",
"    █████████     ███████████         ",
"    █████████       ██████████        ",
"    ██████████      ██████████        ",
"     ██████████       █████████       ",
"      █████████         ████████      ",
"       ████████          ███████      ",
"     █████████         █████████      ",
"    █████████         █████████       "
};

char* bossBattleSprite[51] = {
"                         ██████                              ",
"                         ██████                              ",
"                       ████████                              ",
"                       ██████                                ",
"                       ▓▓▓▓▓                                 ",
"                      ▓▓▓▓▓                                  ",
"              █████████████                                  ",
"            ████████████████                                 ",
"          █████████████████████                              ",
"        ██████████████████████████                           ",
"       █████▒▒▒▒▒▒▒▒▒█████████████                           ",
"      ██████▒▒▒▒▒▒▒▒▒█████████████                           ",
"      ██████▒▒▒▒▒▒▒▒▒█████████████                           ",
"      ██████▓▒▒▒▒▒▒▓▒█████████████                           ",
"     ████████▓▒▒▓███▒████████   █                            ",
"     ███████▒▒▒▒▒▒▒▒▒████████   █                            ",
"     ███████▒▒▒▒▒▒▒▒▒████████                                ",
"       █████▓▓▓▓▒▒▒▒▒███████                                 ",
"        ████▓▓▓▒▒▒▒░▒███████                                 ",
"        ███▒▒░░░░░░░▒███████                                 ",
"        ███▓▒▒▒░░░▒▒▒███████                                 ",
"        ██▓▓▓▓▒░░░▒▓▓███████                                 ",
"        ██▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓██▒░                                ",
"       ░▒▒▒▒▒▒▒▓▒▒▒▒▒▒▒▒▒▒▒░░░                               ",
"       ░░░░▒▒▒▒▒▒░░░░░░░░░░░░░░░                             ",
"     ░░░░░░▒▒▒▒░░░░░░░░░░░░░░░░░                             ",
"     ░░░░░░▒▒▒▒░░░░░▒▒▒▒▒▒░░░░░░░░                           ",
"       ░▒▒▒▒██▒▒▒▒▒▒▒▒███▒░░░░░░▒░░                          ",
"       ░▒▒████▒▒▒▒███████▒░░░░▒▒▒▒▒▒░                        ",
"       ▒███████████████████▒▒▒░▒▒▒░░                         ",
"     ░▒▒████████████████████▒▒░▒▒▒░░                         ",
"    ░▒▒████████████████████▒▒▒░░░░░░░                        ",
"     ▒▒████████████████████▒▒▒░░░░░░░                  ░░░   ",
"   ░░░░▒█████████████████████▒░░░░░░              ░░░░░▒▓▒░░ ",
"  ░░░░░▒▒▒█▒▒░░▒▒▒▒▒▒▒▒▒▒████▒░▒▒▒░░░             ░▓▓▓▓▓▓▓▓▒░",
"  ░░░░░▒▒▒██▒░░▒▒▒▒▒▒▒▒▒▒▒▒██▒░▒██░░░░░       ░▒▒▓▓▓▓▓▓▓▓███░",
"  ░░░░░▒▒▒██▒░░░▒▒▒░▒▒▒▒▒▒▒████████▒░░ ░▒▒▒▒▒▒▓▓▓▓▓▓▓████▒░░ ",
"░░░░░░░▒▒███▒░░░░░░░▒▒▒▒▒▒▒▒▒█████▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓████▒▒▒▒░   ",
"░░░░░░░▒████▒░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██▓▓▓▓▓▓█████▒▒▒▒░       ",
"░░░░▒▒▒████▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓████████████▒░           ",
"░░░░▒█████░░░▒▒▒█████████▒▒████▒▒▒▒▓██████░                  ",
"░░░░▒█████░░░▒██████████▒▒▒███▒▒▒▒▒▒▒▒░                      ",
"░▒▒▒▒█████░░░▒▒▒▒▒▒█▒▒▒▒▒▒█▒▒▒▒▒▒▒▒▒▒▒▒▒░                    ",
"░▒▒█████████▒▒▒▒▒▒▒▒▒░▒▒▒▒▒▒▒▒▒█████████░                    ",
"░░▒██████████████████▒████████████████▒▒░                    ",
"  ░▒▒▒▒▒▒▒███████▒▒█▒▒▒▒▒▒▒▒▒▒▒██████▒░                      ",
"         ░██▒▒▒▒              ▒██▒▒▒▒░                       ",
"            ▒▒▒▒▒▒               ▒▒▒▒                        ",
"       ▒▒▒██▒▒▒▒▒▒               ▒▒▒▒▒▒▒██▒▒                 ",
"      ████████████               ████████████                ",
"      ████ ███████               ███████ ████                "
};

void gotoXY(int x, int y)
{
COORD c;
c.X = x;
c.Y = y;
SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

struct Characters
{
    int life, attack, defense, posX, posY;
    char* sprite;
} player, monsters[1000], boss;

struct Progress
{
    char name[30];
    int gamePhase, kills, difficulty, bonusAttack, bonusDefense;
    bool key;
} user;

struct Walls
{
    int posX, posY;
    char* sprite;
} walls[walls_Quantity];

void Player_Progress()
{
    user.gamePhase = 0;
    user.kills = 0;
    user.difficulty = 0;
    user.bonusAttack = 0;
    user.bonusDefense = 0;
    user.key = false;
}

void Boss_Stats()
{
    boss.life = 45 + 20 * (user.difficulty - 1); //45
    boss.attack = 10 + 5 * (user.difficulty - 1);
    boss.defense = 4 + 2 * (user.difficulty - 1);
    boss.posX = 100;
    boss.posY = 30;
    boss.sprite = "\u00D6";          // Ö
}

void Player_Stats()
{
    player.life = 100;
    player.attack = 10 + 5 * (user.difficulty - 1);
    player.defense = 5 + 4 * (user.difficulty - 1);
    player.posX = 0;
    player.posY = 0;
    player.sprite = "\u2640";
}

void Monster_Stats(int monstersQuantity)
{
    for(int q=0; q < monstersQuantity; q++)
    {
        monsters[q].life = 14 + 8 * (user.difficulty - 1);
        monsters[q].attack = 7 + 4 * (user.difficulty - 1);
        monsters[q].defense = 2 + 1 * (user.difficulty - 1);
    }
}

void Clear_Screen()
{
    for(int i=0; i <= 210; i++)
    {
        for(int j=0; j <= 60; j++)
        {
            gotoXY(i, j);
            printf(" ");
        }
    }
}

void Speech_Balloon()
{
    char* speechBalloon[15] = {
        "╭─────────────────────────────────────╮    ",
        "╰─╮                                   ╰╮   ",
        " ╭╯                                    ╰╮  ",
        " │                                      │  ",
        " │                                      │  ",
        " │                                      │  ",
        " │                                      │  ",
        " │                                      │  ",
        " │                                      │  ",
        " │                                      │  ",
        " │                                      │  ",
        " │                                      │  ",
        " ╰╮                                    ╭╯  ",
        "  ╰╮                                   ╰─╮ ",
        "   ╰─────────────────────────────────────╯ "
    };

    for(int i=0, j=5; i<15; i++, j++)
    {
        gotoXY(160, j);
        printf("%s", speechBalloon[i]);
    }
}

void Box(int initialColumn, int finalColumn, int initialLine, int finalLine, bool details)
{
    for(int i=initialColumn; i<=finalColumn; i++)
    {
        gotoXY(i, initialLine);
        printf("─");
        gotoXY(i, finalLine);
        printf("─");
    }
    for(int j=initialLine; j<=finalLine; j++)
    {
        gotoXY(initialColumn, j);
        printf("│");
        gotoXY(finalColumn, j);
        printf("│");
    }
    gotoXY(initialColumn, initialLine); printf("┌");
    gotoXY(initialColumn, finalLine); printf("└");
    gotoXY(finalColumn, initialLine); printf("┐");
    gotoXY(finalColumn, finalLine); printf("┘");

    if(details == true)
    {
        gotoXY(initialColumn+1, initialLine+1); printf("┌");
        gotoXY(initialColumn+1, finalLine-1); printf("└");
        gotoXY(finalColumn-1, initialLine+1); printf("┐");
        gotoXY(finalColumn-1, finalLine-1); printf("┘");
    }
}

void Show_Key_Binds()
{
    Box(164, 197, 22, 30, true);

    gotoXY(171, 24);    printf("  W");
    gotoXY(171, 25);    printf("A S D ═══ Movimento");

    gotoXY(175, 27);    printf("Q ═══ Ataque");
}

void Show_Player_Life()
{
    Box(167, 194, 35, 41, true);

    gotoXY(170, 37);    printf("▄█▄ ▄█▄");  // ▄█▀
    gotoXY(170, 38);    printf("▀█████▀     %-3d / 100", player.life);
    gotoXY(170, 39);    printf("  ▀█▀");
}

void Show_Monster_Life(int index)
{
    Box(170, 191, 44, 48, false);

    gotoXY(171, 46);    printf("Vida do monstro = %-2d", monsters[index].life);
}

void Show_Boss_Life()
{
    Box(170, 191, 44, 48, false);

    gotoXY(172, 46);    printf("Vida do boss = %-3d", boss.life);
}

void Show_Game_Phase()
{
    Box(171, 190, 44, 48, true);

    gotoXY(178, 46);    printf("Fase %d", user.gamePhase);
}

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

void Show_Ranking()
{
    FILE *ranking = fopen("../../Ranking/ranking.txt", "r");

    if(ranking == NULL)
    {
        printf("Falha ao abrir o arquivo!");
        return;
    }

    Clear_Screen();

    char* n;
    char* p;
    char* k;
    n = "Nick";
    p = "Fase";
    k = "Kills";

    Border(70, 120, 10, 14);
    gotoXY(85, 12);      printf("Ranking de jogadores");
    gotoXY(70, 16);       printf("%-30s   %-10s   %-10s", n, p, k);

    char line[50];
    char nick[30];
    int phase;
    int kills;

    int count = 18;

    while(fgets(line, sizeof(line), ranking) != NULL)
    {
        sscanf(line, "%s %d %d", nick, &phase, &kills);
        gotoXY(70, count);
        printf("%-30s   %-10d   %-10d", nick, phase, kills);
        count++;
    }

    fclose(ranking);
    getch();
}

void Menu()
{
    char* purusu[8] = {
        Red "█████████╗   ███╗   ███╗   █████████╗   ███╗   ███╗   ████████╗   ███╗   ███╗",
            "███╔══███║   ███║   ███║   ███╔══███║   ███║   ███║   ███╔════╝   ███║   ███║",
            "███║  ███║   ███║   ███║   ███║  ███║   ███║   ███║   ███║        ███║   ███║",
            "█████████║   ███║   ███║   █████████║   ███║   ███║   ████████╗   ███║   ███║",
            "███╔═════╝   ███║   ███║   ███████ ╔╝   ███║   ███║   ╚════███║   ███║   ███║",
            "███║         ████╗ ████║   ███╔═███╚╗   ████╗ ████║        ███║   ████╗ ████║",
            "███║          ████████╔╝   ███║  ███║    ████████╔╝   ████████║    ████████╔╝",
            "╚══╝          ╚═══════╝    ╚══╝  ╚══╝    ╚═══════╝    ╚═══════╝    ╚═══════╝"RESET
    };

    for(int i=0, l=10; i < 8; i++, l++)
    {
        gotoXY(68, l);
        printf("%s", purusu[i]);
    }

    char* play[5] = {
        " ██╗           ███╗   █████╗   █████╗   ███╗   ████═╗",
        "███║           ╚██║  ██╔══██╗ ██╔═══╝  █╔══█╗  █╔═██║",
        " ██║  ════  ██╗ ██║  ██║  ██║ ██║  ██╗ █████║  ████═╗",
        " ██║        ╚████╔╝  ╚█████╔╝ ╚█████╔╝ █║  █║  █║ ██║",
        " ╚═╝         ╚═══╝    ╚════╝   ╚════╝  ╚╝  ╚╝  ╚╝  ╚╝",
    };

    for(int i=0, l=30; i < 5; i++, l++)
    {
        gotoXY(48, l);
        printf("%s", play[i]);
    }

    char* leave[5] = {
        "▄███▄╗        ▄████▄╗   ███╗   █████╗  ████═╗",
        "█╔═██║        ██▄▄══╝  █╔══█╗  ╚═█╔═╝  █╔═██║",
        " ▄█▀╔╝  ════   ╚═▀█▄╗  █████║    █║    ████═╗",
        "█████╗        ▀████▀║  █║  █║  █████╗  █║ ██║",
        "╚════╝         ╚════╝  ╚╝  ╚╝  ╚════╝  ╚╝  ╚╝",
    };

    for(int i=0, l=30; i < 5; i++, l++)
    {
        gotoXY(113, l);
        printf("%s", leave[i]);
    }
    
    char* ranking[6] = {
        "██████╗           ██████╗   █████╗  ███╗   ██╗ ██╗  ██╗ ██╗ ███╗   ██╗  ██████╗  ",
        "╚════██╗          ██╔══██╗ ██╔══██╗ ████╗  ██║ ██║ ██╔╝ ██║ ████╗  ██║ ██╔════╝  ",
        " █████╔╝   ════   ██████╔╝ ███████║ ██╔██╗ ██║ █████╔╝  ██║ ██╔██╗ ██║ ██║  ███╗ ",
        " ╚═══██╗          ██╔══██╗ ██╔══██║ ██║╚██╗██║ ██╔═██╗  ██║ ██║╚██╗██║ ██║   ██║ ",
        "██████╔╝          ██║  ██║ ██║  ██║ ██║ ╚████║ ██║  ██╗ ██║ ██║ ╚████║ ╚██████╔╝ ",
        "╚═════╝           ╚═╝  ╚═╝ ╚═╝  ╚═╝ ╚═╝  ╚═══╝ ╚═╝  ╚═╝ ╚═╝ ╚═╝  ╚═══╝  ╚═════╝  "
    };

    for(int i=0, l=40; i < 6; i++, l++)
    {
        gotoXY(65, l);
        printf("%s", ranking[i]);
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
        if(key == '3')
        {
            Show_Ranking();
            break;
        }
        else if (key != '1' && key != '2' && key != '3')
        {
            gotoXY(98, 55);
            printf("OPÇÃO INVÁLIDA!");
        }
    }
}

void Game_Over()
{
    char* gameOver[6] = {
     Red" ██████╗  █████╗ ███╗   ███╗███████╗     ██████╗ ██╗   ██╗███████╗██████╗ ",
        "██╔════╝ ██╔══██╗████╗ ████║██╔════╝    ██╔═══██╗██║   ██║██╔════╝██╔══██╗",
        "██║  ███╗███████║██╔████╔██║█████╗      ██║   ██║██║   ██║█████╗  ██████╔╝",
        "██║   ██║██╔══██║██║╚██╔╝██║██╔══╝      ██║   ██║╚██╗ ██╔╝██╔══╝  ██╔══██╗",
        "╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗    ╚██████╔╝ ╚████╔╝ ███████╗██║  ██║",
        " ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝     ╚═════╝   ╚═══╝  ╚══════╝╚═╝  ╚═╝"RESET
    };

    Clear_Screen();

    for(int i=0, j=23; i < 6; i++, j++)
    {
        gotoXY(85, j);
        printf("%s", gameOver[i]);
    }
}

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

void Print_Map()
{
    Border(initialColumn, finalColumn, initialLine, finalLine);
    for(int c = 0; c < map_Columns; c++)
    {
        for(int l = 0; l < map_Lines; l++)
        {
            if(strcmp(mapArray[c][l], " ") == 0)
            {
                gotoXY(c + initialColumn, l + initialLine);
                printf(BACKGROUND_Green "%s" RESET, mapArray[c][l]);
            }
            else if(strcmp(mapArray[c][l], "\u256C") == 0)
            {
                gotoXY(c + initialColumn, l + initialLine);
                printf(BACKGROUND_Red "%s" RESET, mapArray[c][l]);
            }
            else if(strcmp(mapArray[c][l], "\u2640") == 0)
            {
                gotoXY(c + initialColumn, l + initialLine);
                printf(Blue BACKGROUND_Green "%s" RESET, mapArray[c][l]);
            }
            else if(strcmp(mapArray[c][l], "\u2229") == 0)
            {
                gotoXY(c + initialColumn, l + initialLine);
                printf(Yellow BACKGROUND_Blue "%s" RESET, mapArray[c][l]);
            }
            else if((strcmp(mapArray[c][l], "\u03A8") == 0) || (strcmp(mapArray[c][l], "\u03A6") == 0) || (strcmp(mapArray[c][l], "\u04DC") == 0) || (strcmp(mapArray[c][l], "\u046A") == 0) || (strcmp(mapArray[c][l], "\u03A9") == 0))
            {
                gotoXY(c + initialColumn, l + initialLine);
                printf(BACKGROUND_Green " " RESET);
            }
            else if(strcmp(mapArray[c][l], "\u00D6") == 0)
            {
                gotoXY(c + initialColumn, l + initialLine);
                printf(Black BACKGROUND_Green "%s" RESET, mapArray[c][l]);
            }
        }
    }
}

void Save_User_Data()
{
    FILE *rankingFile = fopen("../../Ranking/ranking.txt", "a");

    if(rankingFile == NULL)
    {
        gotoXY(180, 40);
        printf("Erro ao abrir o arquivo.");
        return;
    }

    fprintf(rankingFile, "\n%s %d %d", user.name, user.gamePhase, user.kills);

    fclose(rankingFile);
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

void Monster_Battle(int index)
{
    Clear_Screen();
    Border(initialColumn, finalColumn, initialLine, finalLine+5);
    for(int i=0, j=initialLine+8; i < 43; i++, j++)
    {
        gotoXY(initialColumn, j);
        printf("%s", playerBattleSprite[i]);
    }
    for(int i=0, j=initialLine+20; i < 31; i++, j++)
    {
        gotoXY(100, j);
        printf("%s", monsterBattleSprite[i]);
    }
    Show_Player_Life();

    Speech_Balloon();
    gotoXY(175, 8);      printf(Red "Atenção!" RESET);
    gotoXY(167, 10);     printf("você acaba de entrar em");
    gotoXY(167, 11);     printf("uma batalha com um monstro.");

    Show_Key_Binds();

    Show_Monster_Life(index);

    while(player.life > 0 && monsters[index].life > 0)
    {
        char atq = getch();

        if(atq == 'q' || atq == 'Q')
        {
            int damageToMonster = player.attack - monsters[index].defense;
            if(damageToMonster < 0)
            {
                damageToMonster = 0;
            }
            monsters[index].life = monsters[index].life - damageToMonster;

            if(monsters[index].life > 0)
            {
                int damageToPlayer = monsters[index].attack - player.defense;
                if(damageToPlayer < 0)
                {
                    damageToPlayer = 0;
                }
                player.life = player.life - damageToPlayer;
            }
        }
        Show_Player_Life();
        Show_Monster_Life(index);
    }
    if(player.life <= 0)
    {
        isGameRunning = !isGameRunning;
        Save_User_Data();
        Game_Over();
    }
    else if(monsters[index].life <= 0)
    {
        user.kills++;
        mapArray[monsters[index].posX][monsters[index].posY] = " ";
    }
    if(isGameRunning)
    {
        Clear_Screen();
        Print_Map();
        Show_Key_Binds();
        Show_Player_Life();
        Show_Game_Phase();
    }
}

void Boss_Battle()
{
    Clear_Screen();
    Border(initialColumn, finalColumn, initialLine, finalLine+5);
    for(int i=0, j=initialLine+8; i < 43; i++, j++)
    {
        gotoXY(initialColumn, j);
        printf("%s", playerBattleSprite[i]);
    }
    for(int i=0, j=initialLine; i < 51; i++, j++)
    {
        gotoXY(90, j);
        printf("%s", bossBattleSprite[i]);
    }
    Show_Player_Life();

    Speech_Balloon();
    gotoXY(175, 8);      printf(Red "Atenção!" RESET);
    gotoXY(167, 10);     printf("você acaba de entrar em");
    gotoXY(167, 11);     printf("uma batalha com o boss.");

    Show_Key_Binds();

    Show_Boss_Life();

    while(player.life > 0 && boss.life > 0)
    {
        char atq = getch();

        if(atq == 'q' || atq == 'Q')
        {
            int damageToBoss = player.attack - boss.defense;
            if(damageToBoss < 0)
            {
                damageToBoss = 0;
            }
            boss.life = boss.life - damageToBoss;

            if(boss.life > 0)
            {
                int damageToPlayer = boss.attack - player.defense;
                if(damageToPlayer < 0)
                {
                    damageToPlayer = 0;
                }
                player.life = player.life - damageToPlayer;
            }
        }
        Show_Player_Life();
        Show_Boss_Life();
    }
    if(player.life <= 0)
    {
        isGameRunning = !isGameRunning;
        Save_User_Data();
        Game_Over();
    }
    else if(boss.life <= 0)
    {
        user.kills++;
        user.key = true;
        mapArray[boss.posX][boss.posY] = " ";
    }
    if(isGameRunning)
    {
        Clear_Screen();
        Print_Map();
        Show_Key_Binds();
        Show_Player_Life();
        Show_Game_Phase();
    }
}

void Walls(int wallsQuantity)
{
    wallsPos = 0;

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
}

void Exit_Position(int initialColumn, int finalColumn, int initialLine, int finalLine)
{
    gotoXY(finalColumn, finalLine);
    mapArray[finalColumn-initialColumn][finalLine-initialLine] = "\u2229";   //"∩";
    printf(Yellow BACKGROUND_Blue "%s" RESET, mapArray[finalColumn-initialColumn][finalLine-initialLine]);
}

char* Monsters()
{
    char* variableMonsters[] = {"\u03A8", "\u03A6", "\u04DC", "\u046A", "\u03A9"};      //{"Ψ", "Φ", "Ӝ", "Ѫ", "Ω"};

    int randomized = rand() % (sizeof(variableMonsters) / sizeof(variableMonsters[0]));

    char* sortedMonster = variableMonsters[randomized];

    return sortedMonster;
}

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

                    verificator = true;
                }
            }
        }
    }
}

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

void Player_Position(int initialColumn, int initialLine)
{
    gotoXY(initialColumn + player.posX, initialLine + player.posY);
    mapArray[player.posX][player.posY] = player.sprite;
    printf(Blue BACKGROUND_Green "%s" RESET, mapArray[player.posX][player.posY]);
}

void Monsters_Count()
{
    char* variableMonsters[] = {"\u03A8", "\u03A6", "\u04DC", "\u046A", "\u03A9"};
    int length = sizeof(variableMonsters) / sizeof(variableMonsters[0]);

    int count=0;

    for(int x = -1; x <= 1; x++)
    {
        for(int y = -1; y <= 1; y++)
        {
            int validX = player.posX + x;
            int validY = player.posY + y;

            if((validX >= 0 && validX < map_Columns) && (validY >= 0 && validY < map_Lines))
            {
                for(int i=0; i < length; i++)
                {
                    if(strcmp(mapArray[validX][validY], variableMonsters[i]) == 0)
                    {
                        count++;
                    }
                }
            }
        }
    }

    if(count == 0)
    {
        Speech_Balloon();
        gotoXY(163, 8);
        printf("Não tem nenhum monstro em sua volta.");
    }
    else if(count == 1)
    {
        Speech_Balloon();
        gotoXY(163, 8);
        printf(Red"Cuidado!"RESET " tem 1 monstro em sua volta.");
    }
    else if(count > 1)
    {
        Speech_Balloon();
        gotoXY(163, 8);
        printf(Red"Cuidado!"RESET " tem %d monstros em sua volta.", count);
    }
}

int Get_Monster_Index(int posX, int posY, int monstersQuantity)
{
    for(int i=0; i < monstersQuantity; i++)
    {
        if(monsters[i].posX == posX && monsters[i].posY == posY && monsters[i].life > 0)
        {
            return i;
        }
    }
    return -1;
}

void Player_Movement(int initialColumn, int finalColumn, int initialLine, int finalLine, int monstersQuantity)
{
    Player_Stats();

    while (true)
    {
        Monsters_Count();

        Player_Position(initialColumn, initialLine);

        gotoXY(finalColumn+3, finalLine+3);
        char pressionedKey = getch();

        int nextX = player.posX;
        int nextY = player.posY;

        if((pressionedKey == 'w' || pressionedKey == 'W') && player.posY > 0 && strcmp(mapArray[player.posX][player.posY-1], "\u256C") != 0)    // ╬
        {
            nextY--;
        }
        else if((pressionedKey == 's' || pressionedKey == 'S') && player.posY < 45 && strcmp(mapArray[player.posX][player.posY+1], "\u256C") != 0)
        {
            nextY++;
        }
        else if((pressionedKey == 'a' || pressionedKey == 'A') && player.posX > 0 && strcmp(mapArray[player.posX-1][player.posY], "\u256C") != 0)
        {
            nextX--;
        }
        else if ((pressionedKey == 'd' || pressionedKey == 'D') && player.posX < 140 && strcmp(mapArray[player.posX+1][player.posY], "\u256C") != 0)
        {
            nextX++;
        }
        else if(pressionedKey == 27)
        {
            isGameRunning = false;
            break;
        }

        if(nextX == boss.posX && nextY == boss.posY && boss.life > 0)
        {
            Boss_Battle();
            if(boss.life <= 0)
            {
                mapArray[nextX][nextY] = " ";
                gotoXY(initialColumn+nextX, initialLine+nextY);
                printf(BACKGROUND_Green " " RESET);
            }
            if(isGameRunning == false)
            {
                break;
            }
            continue;
        }

        int monsterIndex = Get_Monster_Index(nextX, nextY, monstersQuantity);
        if(monsterIndex != -1)
        {
            Monster_Battle(monsterIndex);
            if(isGameRunning == false)
            {
                break;
            }
            continue;
        }

        if(strcmp(mapArray[nextX][nextY], "\u2229") == 0)
        {
            if(user.key == true)
            {
                return;
            }
            else if(user.key == false)
            {
                Speech_Balloon();
                gotoXY(163, 8);
                printf("A porta está trancada! Derrote o BOSS.");
                continue;
            }
        }

        gotoXY(initialColumn + player.posX, initialLine + player.posY);
        mapArray[player.posX][player.posY] = " ";
        printf(BACKGROUND_Green " " RESET);

        player.posX = nextX;
        player.posY = nextY;

        if(strcmp(mapArray[player.posX][player.posY], "\u2229") == 0 && user.key == true)   //"∩"
        {

            return;
        }
    }
}

void Insert_Nickname()
{
    Clear_Screen();

    char* textNick[13] = {
        "██╗███╗   ██╗███████╗ ██████╗ ██████╗ ███╗   ███╗███████╗ ", 
        "██║████╗  ██║██╔════╝██╔═══██╗██╔══██╗████╗ ████║██╔════╝ ", 
        "██║██╔██╗ ██║█████╗  ██║   ██║██████╔╝██╔████╔██║█████╗   ",   
        "██║██║╚██╗██║██╔══╝  ██║   ██║██╔══██╗██║╚██╔╝██║██╔══╝   ",   
        "██║██║ ╚████║██║     ╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗ ",
        "╚═╝╚═╝  ╚═══╝╚═╝      ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝ ",
        "                                                          ",
        "███████╗███████╗██╗   ██╗    ███╗   ██╗██╗ ██████╗██╗  ██╗",
        "██╔════╝██╔════╝██║   ██║    ████╗  ██║██║██╔════╝██║ ██╔╝",
        "███████╗█████╗  ██║   ██║    ██╔██╗ ██║██║██║     █████╔╝ ",
        "╚════██║██╔══╝  ██║   ██║    ██║╚██╗██║██║██║     ██╔═██╗ ", 
        "███████║███████╗╚██████╔╝    ██║ ╚████║██║╚██████╗██║  ██╗",
        "╚══════╝╚══════╝ ╚═════╝     ╚═╝  ╚═══╝╚═╝ ╚═════╝╚═╝  ╚═╝"
    };
    for(int i = 0, j = 20; i < 13; i++, j++)
    {
        gotoXY(90, j);
        printf("%s", textNick[i]);
    }

    gotoXY(110, 35);
    printf("->  ");
    fgets(user.name, 30, stdin);

    user.name[strcspn(user.name, "\n")] = '\0';
}

void Start_Sound_Track()
{
    if(ma_engine_init(NULL, &engine) != MA_SUCCESS)
    {
        gotoXY(finalColumn+5, finalLine+5);
        printf("Erro ao inicializar a engine de áudio!");
    }

    ma_result result = ma_sound_init_from_file(&engine, "../../sounds/overdose.mp3", 0, NULL, NULL, &sound);

    if(result != MA_SUCCESS)
    {
        gotoXY(finalColumn+5, finalLine+5);
        printf("Erro ao carregar o áudio!");
        ma_engine_uninit(&engine);
    }

    ma_sound_set_volume(&sound, 0.1f);

    ma_sound_set_pitch(&sound, 1.0f);

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
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    SetConsoleMode(hOut, dwMode);

    setlocale(LC_ALL, ".utf8");

    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
}

void Window_Config()
{
    system("mode con: cols=237 lines=63");

    HWND hwnd = GetConsoleWindow();

    ShowWindow(hwnd, SW_MAXIMIZE);

    Sleep(100);
}

void Hide_Cursor()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info = {1, FALSE};
    SetConsoleCursorInfo(h, &info);
}

void Show_Cursor()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info = {1, TRUE};
    SetConsoleCursorInfo(h, &info);
}



int main()
{
    Terminal_Config();

    Window_Config();

    srand(time(NULL));

    Hide_Cursor();

    Start_Sound_Track();

    Menu();

    Player_Progress();

    if(isGameRunning)
    {
        Show_Cursor();
        Insert_Nickname();
        Hide_Cursor();
    }

    while(true)
    {
        if(isGameRunning)
        {
            user.key = false;
            user.gamePhase++;
            user.difficulty++;
            user.bonusAttack++;
            user.bonusDefense++;

            int monstersQuantity = 30 + (5 * user.difficulty);

            Clear_Screen();

            Player_Stats();

            Boss_Stats();

            Border(initialColumn, finalColumn, initialLine, finalLine);

            Map(initialColumn, finalColumn, initialLine, finalLine);

            Show_Key_Binds();
            Show_Player_Life();
            Show_Game_Phase();

            Boss_Position(initialColumn, initialLine);

            Walls(10);

            Monster_Position(monstersQuantity, initialColumn, initialLine); //6484

            Exit_Position(initialColumn, finalColumn, initialLine, finalLine);

            Player_Movement(initialColumn, finalColumn, initialLine, finalLine, monstersQuantity);
        }
        else if(!isGameRunning)
        {
            break;
        }
    }

    End_Sound_Track();
}
