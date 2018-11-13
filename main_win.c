/*
 Dummy-code from https://github.com/gkovacs/ffmpeg-concat/blob/master/ffmpeg.c
 Music: Edvard Gried - I Dovregubbens hall.
 Music ideas:
 Tsjaikovski 1812? probably way too long
 Could just go for pop music.
 Michael Jackson, queens etc. illegal?
 Needle in a haystack - the velvettes
 

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <sys/stat.h>
#define NUMLINES 30
#define MAXLINE 1000

int get_num_lines(char string[]) {
    int i = 0;
    int lines = 0;
    while (string[i] != '\0') {
        if (string[i] == '\n') {
            lines++;
        }
        i++;
    }
    return lines;
}

void get_substring_values(char string[], int string_size, int start_line, int *size_substring, int *size_pre_sub, int *num_semicolons){
    int line_nr = 0;
    int i;
    int in_substring = 0;
    int is_pre_sub = 1;
    char c;
    for (i = 0; i<string_size; i++) {
        c = string[i];
        if (in_substring) {
            (*size_substring)++;
            if (c == ';') {
                (*num_semicolons)++;
            }
        } else if (is_pre_sub) {
            (*size_pre_sub)++;
        }
        if (c == '\n') {
            line_nr++;
            if (line_nr == start_line) {
                in_substring = 1;
                is_pre_sub = 0;
            }
            if (line_nr == start_line + NUMLINES) {
                in_substring = 0;
            }
        }

    }
}

void get_substring(char string[], int substring_size, int size_pre_sub, char *substring) {
    strncpy(substring, string + size_pre_sub, substring_size);
    int line = 0;
    int i;
    for (i = 0; line < NUMLINES; i++) {
        if (substring[i] == '\n')
            line++;
    }
    substring[i-1] = '\0';
}

int replace_semicolon_and_return_line(char *string, int num_semicolons) {
    int removed_semicolon_idx = rand() % num_semicolons;
    int semicolon_idx = 0;
    int i;
    char c;
    int line_nr = 0;
    for (i = 0; string[i] != '\0'; i++) {
        c = string[i];
        if (c == ';') {
            if (semicolon_idx == removed_semicolon_idx) {
                string[i] = ' ';
                return line_nr;
            } else {
                semicolon_idx++;
            }
        } else if (c == '\n') {
            line_nr++;
        }
    }
    return -1;
}

void print_with_linenumbers(char string[]) {
    printf("1:\t");
    int i;
    char c;
    int line = 1;
    for (i = 0; string[i] != '\0'; i++) {
        c = string[i];
        putchar(c);
        if (c == '\n') {
            line++;
            printf("%d:\t", line);
        }
    }
    printf("\n");
}

int knr_getline(char s[]) {
    int c, i;
    for(i=0; i<MAXLINE-1 && (c=getchar()) != EOF && c!='\n'; i++) {
        s[i] = c;
    }
    if (c == '\n') {
        s[i] = c;
        i++;
    }
    s[i] = '\0';
    return i;
}

int doesFileExist(const char *filename) {
    struct stat st;
    int result = stat(filename, &st);
    return result == 0;
}

void gameover(int rounds, char song[]) {
    int highscore;
    char *highscore_path;
    char foldername[] = "highscores/";
    char suffix[] = "_highscore.txt";
    highscore_path = malloc(sizeof(char) * (strlen(song) + strlen(foldername) + strlen(suffix)));
    if (highscore_path == NULL) {
        fprintf(stderr, "Det var ikke nok plass for å lage highscore_path");
        exit(1);
    }
    strcpy(highscore_path, foldername);
    strcat(highscore_path, song);
    strcat(highscore_path, "_highscore.txt");
    if (doesFileExist(highscore_path)) {
        FILE *file = fopen(highscore_path, "r");
        fscanf(file, "%d", &highscore);
        if (rounds > highscore) {
            FILE *writefile = fopen(highscore_path, "w");
            fprintf(writefile, "%d", rounds);
            fclose(writefile);
            printf("Ny high score!! Du klarte %d runder! Gratulerer!!", rounds);
        } else {
            printf("Du klarte %d runder. Din high score er %d. Bedre lykke neste gang!",
                rounds, highscore);
        }
        fclose(file);
    }
    else {
        FILE *writefile = fopen(highscore_path, "w");
        fprintf(writefile, "%d", rounds);
        fclose(writefile);
        printf("Ny high score!! Du klarte %d runder! Gratulerer!!", rounds);
    }
    free(highscore_path);
}


int get_song(int song_idx, int *song_length, char song_filename[]) {
    FILE *file = fopen("songs.txt", "r");
    char line[255];
    char song[255];
    int in_song;
    int in_song_length;
    int i;
    char song_length_string[3];
    int song_name_idx;
    int song_length_idx;
    int line_length;
    int n_quotes;
    char *song_with_quotes;
    while (fgets(line, sizeof(line), file)) {
        if (line[0] - '0' == song_idx) {
            n_quotes = 0;
            in_song = 0;
            in_song_length = 0;
            line_length = strlen(line);
            for (i = 0; i < line_length; i++) {
                if (in_song_length) {
                    song_length_string[song_length_idx] = line[i];
                    song_length_idx++;
                }
                if (line[i] == '\"') {
                    if (!in_song) {
                        in_song = 1;
                        song_name_idx = 0;

                    } else {
                        in_song = 0;
                        song[song_name_idx] = '"';
                        song[song_name_idx+1] = '\0';
                    }
                    song_name_idx = 0;
                    n_quotes++;
                    if (n_quotes == 2) {
                        in_song_length = 1;
                        song_length_idx = 0;
                    }
                }
                if (in_song) {
                    song[song_name_idx] = line[i];
                    song_name_idx++;
                }
            }
            song_with_quotes = malloc(strlen(song) + 2);
            strcpy(song_with_quotes, "\"");
            strcat(song_with_quotes, song);
            strcat(song_with_quotes, "\"");
            strcpy(song_filename, song_with_quotes);
            *song_length = atoi(song_length_string);
            return 1;
        }
    }
    return 0;
}

void intro(void) {
    //system("clear");
    printf("Velkommen til \"NAVN PÅ SPILL HER\"!\n");
    printf("I spillet vi lager skulle vi bruke det vi har lært i løpet av semesteret.\n");
    printf("I dette spillet gjør man så absolutt det!\n");
    printf("Spillet går nemlig ut på å finne manglende semikolon\n");
    printf("For å gå videre til neste runde, skriver du inn linjenummeret til linjen som");
    printf("det mangler et semikolon\n");
    printf("Det er om å gjøre å finne flest mulig semikolin før musikken tar slutt.\n");
    printf("Du kan velge mellom flere forskjellige sanger, som alle har forskjellige high scores.\n");
    printf("Sangene du kan velge mellom er:\n");
    printf("1: Edvard Grieg - I Dovregubbens Hall (2 minutter og 31 sekunder)\n");
    printf("2: U2 - I Still Haven't Found What I'm Looking For (4 minutter og 32 sekunder\n");
    printf("3: The Lonely Island - Semicolon (2 minuter og 40 sekunder)\n");
    printf("For å velge en sang skriv den sangens nummer som programmets første argument\n");
    printf("Dersom du vil legge til din egen sang, legg filen i \"songs/\"-mappen og skriv ");
    printf("filens indeks, filens navn og lengden på sangen i songs.txt\n");
    printf("Det eneste tillatte filnavnsuffikset er .wav\n");
    printf("Dvs. f.eks. \"./main.exe 1\"\n");
    printf("Dersom du ønsker å hoppe over introen, skriv 1 som det andre argumentet\n");
    printf("Trykk på en tast for å starte");
    getchar();
    //system("clear");
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    FILE *file;
    int num_lines;
    int start_line;
    char *code;
    char *sub;
    int file_size;
    char substring;
    int missing_line_nr;

    int substring_size;
    int size_pre_sub ;
    int num_semicolons;
    char input[MAXLINE];
    int input_int;
    int wrong_answer_idx;

    char song_filename[200];
    char *song_path;
    int song_duration;
    int song_idx;
    int start_time = time(NULL);
    int elapsed;
    int current_round = 1;

    file = fopen("code.txt", "rb");
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);
    code = malloc(file_size * sizeof(char));
    if (code == NULL) {
        fprintf(stderr, "Det var ikke nok plass for å lage code");
        exit(1);
    }
    fread(code, sizeof(char), file_size, file);
    fclose(file);
    num_lines = get_num_lines(code);
    if (argc > 1) {
        song_idx = atoi(argv[1]);
    } else {
        song_idx = 1;
    }
    if (argc <= 2 || argv[2][0] != '1') {
        intro();
    }
    get_song(song_idx, &song_duration, song_filename);
    song_path = malloc(sizeof(song_filename) + strlen("sanger/"));
    if (song_path == NULL) {
        fprintf(stderr, "Det var ikke nok plass for å lage song_path");
        exit(1);
    }
    strcpy(song_path, "songs/");
    strcat(song_path, song_filename);
    PlaySound(song_path, NULL, SND_ASYNC);

    while (1) {

        substring_size = 0;
        size_pre_sub = 0;
        num_semicolons = 0;
        start_line = rand() % (num_lines - NUMLINES);
        get_substring_values(code, file_size, start_line, &substring_size, &size_pre_sub, &num_semicolons);
        char substring[substring_size + 1];
        get_substring(code, substring_size, size_pre_sub, substring);
        missing_line_nr = replace_semicolon_and_return_line(substring, num_semicolons);
        if (missing_line_nr == -1) {
            continue;
        }

        while (1) {
            printf("Runde %d\n", current_round);
            print_with_linenumbers(substring);

            knr_getline(input);
            input_int = atoi(input);
            if (input_int - 1 == missing_line_nr) {
                elapsed = time(NULL) - start_time;
                if (elapsed > song_duration) {
                    gameover(current_round, song_filename);
                    return 0;
                }
                current_round++;
                //system("clear");
                break;
            } else {
                //system("clear");
            }
        }
    }
    free(code);
    free(song_path);
    return 0;
}
