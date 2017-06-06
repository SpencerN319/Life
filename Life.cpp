//
//  Life.cpp
//  
//
//  Created by spencer nicol on 4/25/17.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <time.h>

//Full Screen Terminal
#define x 205
#define y 63

typedef struct Map{
    char mapxy[y][x];
    bool alive;
} map;

void init_map(map *m)
{
    int i,j;
    for(i = 0; i < y; i++){
        for(j = 0; j < x; j++){
            m->mapxy[i][j] = ' ';
        }
    }
}

void print_map(map *m)
{
    int i,j;
    
    for(i = 0; i < y; i++){
        for(j = 0; j < x; j++){
            printf("%c", m->mapxy[i][j]);
        }
        printf("\n");
    }
}

char rand_char()
{
    int ran;
    char choices[4] = { '0', 'O', '0', 'O' };
    
    ran = rand() % 3;
    
    return (choices[ran]);
}

int check_neighbors(map *m, map *copy, int x_loc, int y_loc)
{
    /*          !!! Rules !!!
     
     -Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
     -Any live cell with two or three live neighbours lives on to the next generation.
     -Any live cell with more than three live neighbours dies, as if by overpopulation.
     -Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
     
     */
    int neighbors, yplus, xplus, yminus, xminus, i, j, ret_val;
    
    neighbors = 0;
    ret_val = 0;
    xplus = x_loc + 1;
    yplus = y_loc + 1;
    xminus = x_loc - 1;
    yminus = y_loc - 1;
    
    /* Wrap the field : no index out of bounds */
    if(xplus >= x) { xplus = x % xplus; }
    if(yplus >= y) { yplus = y % yplus; }
    if(xminus < 0) { xminus = (x + xminus) % xminus; }
    if(yminus < 0) { yminus = (y + yminus) % yminus; }
    
    
    
    
    /* Observe Map and Update Copy */
    if(m->mapxy[yplus][xplus] != ' ') { neighbors++; }
    if(m->mapxy[yminus][xminus] != ' ') { neighbors++; }
    if(m->mapxy[yplus][x_loc] != ' ') { neighbors++; }
    if(m->mapxy[yminus][x_loc] != ' ') { neighbors++; }
    if(m->mapxy[y_loc][xplus] != ' ') { neighbors++; }
    if(m->mapxy[y_loc][xminus] != ' ') { neighbors++; }
    if(m->mapxy[yplus][xminus] != ' ') { neighbors++; }
    if(m->mapxy[yminus][xplus] != ' ') { neighbors++; }
    
    if(m->mapxy[y_loc][x_loc] != ' '){
        if(neighbors < 2) { copy->mapxy[y_loc][x_loc] = ' '; ret_val++; }
        if(neighbors == 2 || neighbors == 3) { copy->mapxy[y_loc][x_loc] = rand_char(); }
        if(neighbors > 3) { copy->mapxy[y_loc][x_loc] = ' '; ret_val++; }
    }
    else {
        if(neighbors == 3) { copy->mapxy[y_loc][x_loc] = rand_char(); ret_val++; }
    }
    
    return ret_val;

}

void transfer(map *m, map *copy)
{
    int i,j;
    
    for(i = 0; i < y; i++){
        for(j = 0; j < x; j++){
            m->mapxy[i][j] = copy->mapxy[i][j];
        }
    }
}

int main(int argc, char *argv[])
{
    int run, r, c, q, gen_num, i;
    srand(time(NULL));
    map *m1 = new map;
    map *copy = new map;
    gen_num = 12000; //Max = 24,600
    init_map(m1);
    init_map(copy);
    int x_coor[gen_num/2];
    int y_coor[gen_num/2];
    run = 1;
    
    for(i = 0; i < gen_num/2; i++){
        x_coor[i] = rand() % 205;
        y_coor[i] = rand() % 63;
    }

    //Initialize the first Map
    for(r = 0; r < y; r++){
        for(c = 0; c < x; c++){
            for(q = 0; q < gen_num/2; q++){
                if(r == y_coor[q] && c == x_coor[q]){
                    m1->mapxy[r][c] = rand_char();
                    m1->alive = true;
                }
            }
        }
    }
    
    while(run > 0){
        int i,j;
        
        run = 0;
        print_map(m1);
        
        for(i = 0; i < y; i++){
            for(j = 0; j < x; j++){
                run += check_neighbors(m1, copy, j, i);
            }
        }
        transfer(m1,copy);
        //usleep(83333);
    }

    
    
    
    
    
    delete m1;
    delete copy;
    
    return 0;
}
