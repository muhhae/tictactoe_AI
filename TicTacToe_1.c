#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

int turn_count = 0;
int mod, diff;
char turn = 'X', winner;
char board[9] = {"........."};
int empt_pos[8];

int check_win_probability(int , char*, char);
int check_win_AI(char , char*);
int check_close_win_AI(char , char*);
int check_vert_AI(char , char*);
int check_hori_AI(char , char*);
int check_dia_AI(char , char*);

int get_random(int max)
{
    time_t tim;
    int rand_v;
    srand((unsigned) time (&tim));
    
    for (int i = 0; i < rand() % 10 + 1; i++)
    {
        rand_v = rand_v * rand() % max + 1;
    }
    return rand_v % max + 1;
}

void main_menu()
{
    reset();
    system("cls");
    int menu = 0;
    printf("1. Play\n2. Instruction\n3. Exit\n");
    while(menu == 0)
    {
        if (kbhit())
        {
            menu = (int)getch() - 48;
        }   
    }
    printf("%i", menu);
    system("cls");

    switch (menu)
    {
    case 1:
        mode();
        break;
    case 2:
        instruction();
        break;
    case 3:
        return;
        break;
    
    default:
        main_menu();
        break;
    }
}

void mode()
{
    printf("1. 1-Player\n2. 2-Player\n3. Back to main menu\n");
    mod = 0;
    while(mod == 0)
    {
        if (kbhit())
        {
            mod = (int)getch() - 48;
        }   
    }
    system("cls");
    if (mod == 3) main_menu();
    else if (mod == 1) difficulty();
    else main_game();
}

void difficulty()
{
    printf("1. Easy\n2. Normal\n3. Hard\n");
    diff = 0;
    while(diff == 0)
    {
        if (kbhit())
        {
            diff = (int)getch() - 48;
        }   
    }
    system("cls");
    if (diff < 1 || diff > 3) difficulty();
    else first_play();
}
void first_play()
{
    int first_ply = 0;
    printf("Who's Gonna play first ? \n1. You\n2. Bot\n");
    while(first_ply == 0)
    {
        if (kbhit())
        {
            first_ply = (int)getch() - 48;
        }   
    }
    turn = first_ply == 1 ? 'X' : 'O';
    main_game();
}
void instruction()
{
    system("cls");
    printf("1. If its your turn pick empty spot from 1 to 9.\n2. If its AI's turn just wait patiently until it's your turn\n3. The winner is the one that able to put three O or X in the same line, row, or diagonal");
    int menu = 0;
    printf("\n\nPress 1 to back to main menu");
    while(menu == 0)
    {
        if (kbhit())
        {
            menu = (int)getch() - 48;
        }   
    }
    if (menu == 1) main_menu();
    else instruction();
}

void main_game()
{
    if (check_win() == 1)
    {
        win_screen();
        return;
    }
    else if (check_win() == 3)
    {
        tie_screen();
        return;
    }
    system("cls");
    printf("  1 2 3\n");
    int x = 1;
    for (int i = 1; i <= 9; i++ )
    {
        if (i % 3 == 1) printf("%i ", x++);
        printf("%c ", board[i-1]);
        if (i % 3 == 0) printf(" \n");
    }
    printf("%c's turn choose from 1-9 : ", turn);
    input_master();
}

int input_ai()
{
    int highest_point_index = 0;
    int poin[9];
    int mt_pos[9];
    int mt_pos_count = get_empt_pos(mt_pos, board);

    int close_pos = check_close_win_AI('O', board);

    if (close_pos != 0) 
    {
        return close_pos;
    }
    else close_pos = check_close_win_AI('X', board);

    if (close_pos != 0) 
    {
        return close_pos;
    }
    

    for (int i = 0; i < mt_pos_count; i++)
    {
        poin[i] = check_win_probability(mt_pos[i], board, 'X');
    }

    for (int i = 0; i < mt_pos_count; i++)
    {
        //printf("pos %i, poin %i\n", mt_pos[i], poin[i]);
        if(poin[highest_point_index] < poin[i]) highest_point_index = i; 
        else if(poin[highest_point_index] == poin[i])
        {
            if (get_random(10) < 6) highest_point_index = i;
        }
    }

    return mt_pos[highest_point_index];
}

int check_win_probability(int pos, char *dum_board, char prev_player)
{
    
    int poin = 0;
    char dummy[9];
    for (int i = 0; i < 9; i++) dummy[i] = dum_board[i];
    char usr = prev_player == 'X' ? 'O' : 'X';
    dummy[pos-1] = usr;

    int empty_pos[9];
    int empty_pos_count = get_empt_pos(empty_pos, dummy);

    if (check_win_AI('X', dummy)) return -1 * (empty_pos_count + 1) * (empty_pos_count + 1);
    else if (check_win_AI('O', dummy)) return 1 * (empty_pos_count + 1) * (empty_pos_count + 1) ;
    else if (check_tie_AI(dummy)) return 0;
    else if (empty_pos_count > 0)
    {
        int close_pos = check_close_win_AI(usr, dummy);
        if (close_pos != 0) poin += check_win_probability(close_pos, dummy, usr);
        else 
        {
            for(int i = 0; i < empty_pos_count; i++)
            {
                poin += check_win_probability(empty_pos[i], dummy, usr);
            }
        }
        return poin;
    }
    return 0;
    
}

int check_win_AI(char player, char *check_board)
{
   if (check_dia_AI(player, check_board)) return 1;
   else if (check_hori_AI(player, check_board)) return 1;
   else if (check_vert_AI(player, check_board)) return 1;
   else return 0;
}

int check_close_win_AI(char player, char *check_board)
{
    //verti
    for (int i = 0; i < 3; i++)
    {
        if (check_board[i] == check_board[i+3] && check_board[i+6] == '.' && check_board[i] == player)
        {
            return i+7;
        }
        else if (check_board[i+6] == check_board[i+3] && check_board[i] == '.' && check_board[i+3] == player)
        {
            return i+1;
        }
        else if (check_board[i] == check_board[i+6] && check_board[i+3] == '.' && check_board[i] == player)
        {
            return i+4;
        }
    }
    //hori
    for (int i = 0; i < 9; i += 3)
    {
        if (check_board[i] == check_board[i+1] && check_board[i+2] == '.' && check_board[i] == player)
        {
            return i+3;
        }
        else if (check_board[i+2] == check_board[i+1] && check_board[i] == '.' && check_board[i+2] == player)
        {
            return i+1;
        }
        else if (check_board[i] == check_board[i+2] && check_board[i+1] == '.' && check_board[i] == player)
        {
            return i+2;
        }
    }

    if (check_board[0] == check_board[4] && check_board[8] == '.' && check_board[0] == player)
    {
        return 9;
    }
    else if (check_board[8] == check_board[4] && check_board[0] == '.' && check_board[8] == player)
    {
        return 1;
    }
    else if (check_board[0] == check_board[8] && check_board[4] == '.' && check_board[0] == player)
    {
        return 5;
    }

    if(check_board[2] == check_board[4] && check_board[6] == '.' && check_board[2] == player)
    {
        return 7;
    }
    else if(check_board[2] == check_board[6] && check_board[4] == '.' && check_board[2] == player)
    {
        return 5;
    }
    else if(check_board[6] == check_board[4] && check_board[2] == '.' && check_board[4] == player)
    {
        return 3;
    }

    return 0;
}

int check_vert_AI(char player, char *check_board)
{
    for (int i = 0; i < 3; i++)
    {
        if (check_board[i] == check_board[i+3] && check_board[i] == check_board[i+6] && check_board[i] == player)
        {
            return 1;
        }
    }
    
    return 0;
}

int check_hori_AI(char player, char *check_board)
{
    for (int i = 0; i < 9; i += 3)
    {
        if (check_board[i] == check_board[i+1] && check_board[i] == check_board[i+2] && check_board[i] == player)
        {
            return 1;
        }
    }
    return 0;
}

int check_dia_AI(char player, char *check_board)
{
    if (check_board[0] == check_board[4] && check_board[0] == check_board[8] && check_board[0] == player)
    {
        return 1;
    }
    else if(check_board[2] == check_board[4] && check_board[2] == check_board[6] && check_board[2] == player)
    {
        return 1;
    }
    return 0;
}

int check_tie_AI(char *check_board)
{
    for(int i = 0; i < 9; i++)
    {
        if (check_board[i] == '.')
        {
            return 0;
        }
    }
    return 1;
}

int get_empt_pos(int empty_pos_arr[9], char board_1[9])
{
    int empt_pos_count = 0;
    for(int i = 0; i < 9; i++)
    {
        empty_pos_arr[i] = 0;
    }

    for(int i = 0; i < 9; i++)
    {
        if(board_1[i] == '.') 
        {
            empty_pos_arr[empt_pos_count] = i + 1;
            empt_pos_count++;
        }
    }
    return empt_pos_count;
}

void input_master()
{
    turn_count++;
    int pos = 0;
    do
    {
        pos = 0;
        if (turn == 'X')
        {
            while(pos == 0)
            {
                int a = 0,b = 0;
                while (a == 0)
                {
                    if (kbhit())
                    {
                        a = (int)getch() - 48;
                    }  
                }
                while (b == 0)
                {
                    if (kbhit())
                    {
                        b = (int)getch() - 48;
                    }  
                }
                pos = (a-1)*3 + b;
            }
        }

        else
        {
            if (mod == 1)
            {
                int correct_move_prob;
                switch (diff)
                {
                case 1:
                    correct_move_prob = 0;
                    break;
                case 2:
                    correct_move_prob = 50;
                    break;
                case 3:
                    correct_move_prob = 100;
                    break;
                default:
                    break;
                }
                if (get_random(100) <= correct_move_prob)
                {
                    printf("turn = %i\n", turn_count);
                    // if (turn_count == 2)
                    // {
                    //     // int x;
                    //     // while (x == 0)
                    //     // {
                    //     //     if (kbhit())
                    //     //     {
                    //     //         x = (int)getch() - 48;
                    //     //     }  
                    //     // }
                    //     // pos = x;
                    //     pos = get_random(9);
                    // }
                    // else
                    // {
                        if (((board[0] == board[8] && board[0] != '.') || (board[2] == board[6] && board[2] != '.')) && turn_count <= 5)
                        {
                            int arr_corner_pos[4] = {2, 4, 6, 8};
                            pos = arr_corner_pos[get_random(4)-1];
                        }

                        else pos = input_ai();
                    // }
                    
                }
                else 
                {
                    int count = get_empt_pos(empt_pos,board);
                    pos = empt_pos[get_random(count)];
                }
                
            }
            else
            {
                while(pos == 0)
                {
                    int a = 0,b = 0;
                    while (a == 0)
                    {
                        if (kbhit())
                        {
                            a = (int)getch() - 48;
                        }  
                    }
                    while (b == 0)
                    {
                        if (kbhit())
                        {
                            b = (int)getch() - 48;
                        }  
                    }
                    pos = (a-1)*3 + b;
                }
            }
        }
    } while (pos > 9 || pos < 1 || board[pos-1] != '.');

    printf("pos = %i\n", pos);
    board[pos-1] = turn;
    turn = turn == 'O' ? 'X' : 'O';
    main_game();
}

int check_win()
{
    if(check_horizontal()) return 1;
    else if(check_vertical()) return 1;
    else if(check_diagonal()) return 1;
    else if(check_tie()) return 3;
    else return 0;
}

int check_vertical()
{
    for (int i = 0; i < 3; i++)
    {
        if (board[i] == board[i+3] && board[i] == board[i+6] && board[i] != '.')
        {
            winner = board[i];
            return 1;
            break;
        }
    }
    return 0;
}

int check_horizontal()
{
    for (int i = 0; i < 9; i += 3)
    {
        if (board[i] == board[i+1] && board[i] == board[i+2] && board[i] != '.')
        {
            winner = board[i];
            return 1;
            break;
        }
    }
    return 0;
}

int check_diagonal()
{
    if (board[0] == board[4] && board[0] == board[8] && board[0] != '.')
    {
        winner = board[0];
        return 1;
    }
    else if(board[2] == board[4] && board[2] == board[6] && board[2] != '.')
    {
        winner = board[2];
        return 1;
    }
    return 0;
}

int check_tie()
{
    for(int i = 0; i < 9; i++)
    {
        if (board[i] == '.')
        {
            return 0;
        }
    }
    return 1;
}

void win_screen()
{
    system("cls");
    for (int i = 1; i <= 9; i++ )
    {
        printf("  %c", board[i-1]);
        if (i % 3 == 0) printf(" \n");
    }
    printf("Winner = %c\n", winner);
    printf("1. Back To Main Menu\n2. Exit");
    int menu = 0;
    while(menu == 0)
    {
        if (kbhit())
        {
            menu = (int)getch() - 48;
        }   
    }
    if (menu == 1) main_menu();
    else if(menu == 2) return;
    else win_screen();
}

void tie_screen()
{
    system("cls");
    for (int i = 1; i <= 9; i++ )
    {
        printf("  %c", board[i-1]);
        if (i % 3 == 0) printf(" \n");
    }
    printf("Tie\n");
    printf("1. Back To Main Menu\n2. Exit");
    int menu = 0;
    while(menu == 0)
    {
        if (kbhit())
        {
            menu = (int)getch() - 48;
        }   
    }
    if (menu == 1) main_menu();
    else if(menu == 2) return;
    else tie_screen();
}

void reset()
{
    turn_count = 1;
    mod = 0;
    diff = 0;
    turn = 'X';
    winner = ' ';
    for (int i = 0; i < 9; i++) board[i] = '.';
}

int main()
{
    main_menu();
    return 0;
}
