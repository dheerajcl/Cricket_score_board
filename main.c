#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Added for toupper
#include <unistd.h> // Added for sleep


struct Player {
    char name[50];
    int sixes;
    int fours;
    float economy;
    int wickets;
    int runsScored;
    char position[20];
    int runsGiven;
    float oversBowled;
    float oversPlayed;
    int isCaptain;
    int isBowler;
    int isBatsmen;
};

void resetPlayerDetails(struct Player *player) {
    player->sixes = 0;
    player->fours = 0;
    player->economy = 0.0;
    player->wickets = 0;
    player->runsScored = 0;
    player->runsGiven = 0;
    player->oversBowled = 0.0;
    player->oversPlayed = 0.0;
}

void filereading(struct Player IND_History[11])
{
    FILE *historicalFile = fopen("team_india_history.txt", "r");
    if (historicalFile == NULL) {
        perror("Error opening historical file");
        return 1;
    }

    for (int i = 0; i < 11; ++i) {
        fscanf(historicalFile, "%s %d %d %f %d %d %s %d %f %f %d",
            IND_History[i].name, &IND_History[i].sixes, &IND_History[i].fours, &IND_History[i].economy,
            &IND_History[i].wickets, &IND_History[i].runsScored, IND_History[i].position,
            &IND_History[i].runsGiven, &IND_History[i].oversBowled, &IND_History[i].oversPlayed,
            &IND_History[i].isCaptain);
    }
    fclose(historicalFile);
}

// struct Player* searchPlayer(const char *name, struct Player players[]) {
//     for (int i = 0; i <11 ; ++i) {
//         if (strcasecmp(name, players[i].name) == 0) {
//             return &players[i];
//         }
//     }
//     return NULL;  
// }

void req_rate(float a, float b, float c, float d, float e);
void runs_needed(int a, int b, int c, int d, int e);
void projected_score(int a, int b, int c, int d, float e);
void result(int a, int b, int e, int g, char f, char c[], char d[]);
float curr_rate(float a, float b, float c);
void ball_played(int *a, int *b, int *c, int *d, int *e, int *f, int *g, int *h, int *i, int *j,struct Player players [],struct Player strikearr[]);
int string_comp(char a[], char b[], char c[]);
int my_toupper(int c);

int main() {
    struct Player IND_History[11];
    struct Player IND_current[11];
    struct Player AUS_History[11];
    struct Player AUS_current[11];
    int wd = 0, nb = 0, lb = 0, b = 0, extras = 0, overs = 0, T_overs = 0, k = 0, balls = 0, T = 0, score = 0, wickets = 0, target = 0, q = 0, c[11], j = 1, f = 0, s = 0;
    float c_rate;
    char toss[15], team_1[15], team_2[15], choice, runs;
    char match_type[10];
    printf("Match between:\n");
    fgets(team_1, sizeof(team_1), stdin);
    printf("versus\n");
    fgets(team_2, sizeof(team_2), stdin);
    printf("Toss won by: ");
    fgets(toss, sizeof(toss), stdin);
    printf("Enter B to choose bat first and F to field first: ");
    scanf(" %c", &choice);
    printf("Enter the type of match (T20/ODI/Test): ");
    scanf("%s", match_type);
    if (strcmp(match_type, "T20") == 0) {
        T_overs = 20;
    } else if (strcmp(match_type, "ODI") == 0) {
        T_overs = 50;
    } else if (strcmp(match_type, "Test") == 0) {
        T_overs = 90;
    } else {
        printf("Invalid match type!\n");
        return 1;
    }
    system("clear");
    
    filereading(IND_History);
    filereading(AUS_History);
    for (int i = 0; i < 11; i++) {
    IND_current[i] = IND_History[i];
    AUS_current[i] = AUS_History[i];
    }
    resetPlayerDetails(IND_current);
    resetPlayerDetails(AUS_current);
    struct Player * strikearr[2];
    strikearr[0]=&IND_current[0];
    strikearr[1]=&IND_current[1];

    T = string_comp(team_1, team_2, toss);
    while (overs <= T_overs || overs >= T_overs) {
        do {
            if (wickets > q) {
                *(c + wickets) = score;
            }
            q = wickets;
            system("clear");
            printf("\t\t\t%s vs %s (%d-overs match)\n", team_1, team_2, T_overs);
            if (T == 1) {
                if (choice == 'B' || choice == 'b') {
                    printf("%s won the toss and elected to bat first\n", team_1);
                    printf("\n\n%s %d-%d\t\tovers %d.%d\t\t", team_1, score, wickets, overs, balls);
                } else if (choice == 'F' || choice == 'f') {
                    printf("%s won the toss and elected to field first\n", team_1);
                    printf("\n\n%s %d-%d\t\tovers %d.%d\t\t", team_2, score, wickets, overs, balls);
                }
            } else if (T == 2) {
                if (choice == 'B' || choice == 'b') {
                    printf("%s won the toss and elected to bat first\n", team_2);
                    printf("\n\n%s %d-%d\t\tovers %d.%d\t\t", team_2, score, wickets, overs, balls);
                } else if (choice == 'F' || choice == 'f') {
                    printf("%s won the toss and elected to field first\n", team_2);
                    printf("\n\n%s %d-%d\t\tovers %d.%d\t\t", team_1, score, wickets, overs, balls);
                }
            }
            if (overs > 0 && score > 20) {
                if (overs == T_overs || wickets == 10) {
                    printf("\n");
                    k++;
                    break;
                }
                c_rate = curr_rate(score, overs, balls);
                printf("run rate %0.2f\n", c_rate);
            }
            if (balls == 0) {
                printf("\n");
                if (extras > 0) {
                    printf("extras: %d(wd %d,nb %d,lb %d,b %d)\t", extras, wd, nb, lb, b);
                }
                if (f > 0 || s > 0) {
                    printf("boundaries: %d fours,%d sixes\n", f, s);
                } else
                    printf("\n");
            }
            if (overs > 0 && score > 20 && balls == 0) {
                projected_score(score, overs, T_overs, balls, c_rate);
            }
            printf("\n");
            if (overs == T_overs || wickets == 10) {
                k++;
                break;
            }
            ball_played(&balls, &score, &wickets, &extras, &wd, &nb, &lb, &b, &f, &s,AUS_current, strikearr);
        } while (balls < 6);
        if (balls == 6) {
            balls = 0;
        }
        if (overs < T_overs) {
            overs++;
        }
        if (k > 0) {
            break;
        }
    }
    printf("\nextras: %d(wd %d,nb %d,lb %d,b %d)\tboundaries: %d fours,%d sixes\n", extras, wd, nb, lb, b, f, s);
    if (wickets > 0) {
        printf("\nfall of wickets:  ");
        for (j = 1; j <= wickets; j++) {
            if (j == 1 || j == 6)
                printf("\n");
            else
                printf("\t");
            printf("%d-%d", j, c[j]);
        }
    }
    printf("\n\ninnings completed.");
    sleep(10);
    for (k = 10; k >= 0; k--) {
        system("clear");
        printf("\n\n\n\n\t\t\tINNINGS BREAK...\n\t\t\t\t%d", k);
        sleep(1);
    }
    target = score + 1, score = 0, overs = 0, k = 0, wd = 0, nb = 0, lb = 0, b = 0, balls = 0, wickets = 0, extras = 0, j = 1, q = 0, f = 0, s = 0;
    while (overs <= T_overs || overs >= T_overs) {
        do {
            if (wickets > q) {
                *(c + wickets) = score;
            }
            q = wickets;
            system("clear");
            printf("\t\t\t%s vs %s (%d-overs match)\n", team_1, team_2, T_overs);
            if (T == 1) {
                if (choice == 'B' || choice == 'b') {
                    printf("%s gives target of %d runs to %s to win\n", team_1, target, team_2);
                    printf("\n\n%s %d-%d\t  overs %d.%d\t", team_2, score, wickets, overs, balls);
                } else if (choice == 'F' || choice == 'f') {
                    printf("%s gives target of %d runs to %s to win\n", team_2, target, team_1);
                    printf("\n\n%s %d-%d\t  overs %d.%d\t", team_1, score, wickets, overs, balls);
                }
            } else if (T == 2) {
                if (choice == 'B' || choice == 'b') {
                    printf("%s gives target of %d runs to %s to win\n", team_2, target, team_1);
                    printf("\n\n%s %d-%d\t  overs %d.%d\t", team_1, score, wickets, overs, balls);
                } else if (choice == 'F' || choice == 'f') {
                    printf("%s gives target of %d runs to %s to win\n", team_1, target, team_2);
                    printf("\n\n%s %d-%d\t  overs %d.%d\t", team_2, score, wickets, overs, balls);
                }
            }
            if (overs > 0 && score > 20) {
                if (overs == T_overs || wickets == 10 || target <= score) {
                    printf("\n");
                    k++;
                    break;
                }
                c_rate = curr_rate(score, overs, balls);
                printf("run rate %0.2f", c_rate);
                req_rate(score, overs, T_overs, balls, target);
            }
            if (balls == 0) {
                printf("\n");
                if (extras > 0) {
                    printf("extras: %d(wd %d,nb %d,lb %d,b %d)\t", extras, wd, nb, lb, b);
                }
                if (f > 0 || s > 0) {
                    printf("boundaries: %d fours,%d sixes\n", f, s);
                } else
                    printf("\n");
            }
            if (overs > 0 && score > 20) {
                runs_needed(score, overs, T_overs, balls, target);
            }
            printf("\n");
            if (wickets == 10 || target <= score || overs == T_overs) {
                k++;
                break;
            }
            ball_played(&balls, &score, &wickets, &extras, &wd, &nb, &lb, &b, &f, &s,IND_current,strikearr);
        } while (balls < 6 && overs < T_overs);
        if (balls == 6) {
            balls = 0;
        }
        if (overs < T_overs) {
            overs++;
        }
        if (k > 0) {
            break;
        }
    }
    printf("\nextras: %d(wd %d,nb %d,lb %d,b %d)\tboundaries: %d fours,%d sixes\n", extras, wd, nb, lb, b, f, s);
    if (wickets > 0) {
        printf("\nfall of wickets:  ");
        for (j = 1; j <= wickets; j++) {
            if (j == 1 || j == 6)
                printf("\n");
            else
                printf("\t");
            printf("%d-%d", j, c[j]);
        }
    }
    printf("\n\ninnings completed.");
    sleep(10);
    for (k = 0; k <= 7; k++) {
        system("clear");
        sleep(1);
        printf("\n\n\n");
        result(target, score, T, wickets, choice, team_1, team_2);
        sleep(1);
    }
    getchar();
    return 0;
}

float curr_rate(float a, float b, float c) {
    float d;
    if (c == 0)
        d = a / b;
    else
        d = a / (b + (c / 10));
    return d;
}

int string_comp(char a[], char b[], char c[]) {
    int i = 0, ab = 0, j = 0, k = 0;
    my_toupper(a);
    my_toupper(b);
    my_toupper(c);
    for (i = 0; i < 3; i++) {
        if (a[i] == c[i])
            j++;
        else if (b[i] == c[i])
            k++;
        else
            break;
    }
    if (j == 3)
        ab = 1;
    else if (k == 3)
        ab = 2;
    else
        ab = 0;
    return ab;
}



void ball_played(int *balls_played, int *total_runs, int *wickets, int *extras, int *wide, int *no_ball, int *leg_byes, int *byes, int *fours, int *sixes,struct Player IND_current [],struct Player strikearr[]) {
    char run_type;
    int extra_runs;
    struct Player * batsmen=&IND_current[0];
    printf("%s %d\n",batsmen->name,batsmen->runsScored);
    printf("Enter run type (0-6, W/w for wide, N/n for no ball, L/l for leg byes, B/b for byes, O/o for out): ");
    scanf(" %c", &run_type);
    switch (run_type) {
        case '0':
            (*balls_played)++;
            batsmen->runsScored +=0;
            break;
        case '1':
            (*total_runs)++;
            (*balls_played)++;
            batsmen->runsScored +=1;
            break;
        case '2':
            *total_runs += 2;
            (*balls_played)++;
            batsmen->runsScored +=2;
            break;
        case '3':
            *total_runs += 3;
            (*balls_played)++;
            batsmen->runsScored +=3;
            break;
        case '4':
            *total_runs += 4;
            (*balls_played)++;
            batsmen->runsScored +=4;
            batsmen->fours+=1;
            (*fours)++;
            break;
        case '6':
            *total_runs += 6;
            (*balls_played)++;
            batsmen->runsScored +=6;
            batsmen->sixes+=1;
            (*sixes)++;
            break;
        case 'o':
        case 'O':
            (*wickets)++;
            (*balls_played)++;
            printf("Player is out!\n");
            break;
        case 'w':
        case 'W':
            printf("Enter runs on wide ball: ");
            scanf("%d", &extra_runs);
            *total_runs += 1 + extra_runs;
            batsmen->runsScored += extra_runs;
            (*extras)++;
            (*wide)++;
            break;
        case 'n':
        case 'N':
            printf("Enter runs on no ball: ");
            scanf("%d", &extra_runs);
            *total_runs += extra_runs + 1;
            batsmen->runsScored += extra_runs;
            (*extras)++;
            (*no_ball)++;
            break;
        case 'l':
        case 'L':
            printf("Enter runs as leg byes: ");
            scanf("%d", &extra_runs);
            *total_runs += extra_runs;
            (*leg_byes) += extra_runs;
            batsmen->runsScored += extra_runs;
            (*byes)++;
            (*balls_played)++;
            break;
        case 'b':
        case 'B':
            printf("Enter runs as byes: ");
            scanf("%d", &extra_runs);
            *total_runs += extra_runs;
            (*extras) += extra_runs;
            batsmen->runsScored += extra_runs;
            (*byes) += extra_runs;
            break;
        default:
            printf("Invalid input! Try again.\n");
            break;
    }

    
    // if(!(run_type-'0')%2==0)
    // {
        
    // }
}


void req_rate(float a, float b, float c, float d, float e) {
    float r_runrate, req_rate;
    r_runrate = a / b;
    req_rate = (c - d) / e;
    printf("required run rate %0.2f", req_rate);
}

void runs_needed(int a, int b, int c, int d, int e) {
    int runs_needed;
    runs_needed = (a + 1) - b;
    printf("runs needed to win %d", runs_needed);
}

void projected_score(int a, int b, int c, int d, float e) {
    int p_score;
    p_score = a + (c - b) * e;
    printf("projected score %d", p_score);
}

void result(int a, int b, int e, int g, char f, char c[], char d[]) {
    printf("\n\t\t\tMATCH RESULT");
    if (e == 1) {
        if (a > b) {
            if (f == 'B' || f == 'b') {
                printf("\n%s won the match by %d runs\n", c, a - b - 1);
            } else if (f == 'F' || f == 'f') {
                printf("\n%s won the match by %d wickets\n", c, 10 - g);
            }
        } else if (b > a) {
            if (f == 'B' || f == 'b') {
                printf("\n%s won the match by %d wickets\n", d, 10 - g);
            } else if (f == 'F' || f == 'f') {
                printf("\n%s won the match by %d runs\n", d, b - a - 1);
            }
        } else {
            printf("\nmatch tied\n");
        }
    } else if (e == 2) {
        if (a > b) {
            if (f == 'B' || f == 'b') {
                printf("\n%s won the match by %d wickets\n", d, 10 - g);
            } else if (f == 'F' || f == 'f') {
                printf("\n%s won the match by %d runs\n", d, b - a - 1);
            }
        } else if (b > a) {
            if (f == 'B' || f == 'b') {
                printf("\n%s won the match by %d runs\n", c, a - b - 1);
            } else if (f == 'F' || f == 'f') {
                printf("\n%s won the match by %d wickets\n", c, 10 - g);
            }
        } else {
            printf("\nmatch tied\n");
        }
    }
}

int my_toupper(int c) {
    if (islower(c)) {
        return toupper(c);
    } else {
        return c;
    }
}