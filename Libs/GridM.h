#ifndef GridM_h
#define GridM_h

#include "Arduino.h"
#ifndef len_of_int 
#define len_of int 100
#endif
typedef struct
{
  boolean left;
  boolean right;
  boolean up;
  boolean down;
}
vertice;
typedef struct
{
  int name;
  vertice path;
}
node;

class GridM
{
  public:
    GridM(int row ,int col);//
	void get_path();//
    void show_available_path();//
	void print_array(byte ar[]);//
	void set_node(int node_name);//
	void set_source(int source_name);//
	void set_destination(int destination_name);//
  private:
	void find_path(byte ar[]);//
	void init_board();//
	void init_grid();//
	void zero_init(byte a[]);//
	void zero_trim(byte a[]);//
	void search_in_array_and_add_cleverly(byte ar[],int old_value,int new_value);//
	void replace_and_shift(byte arr[],int value,int indx);//
	int r;
	int c
	int len;
	int board[len];
	int destination;
	int source;
	node grid[len];
	String quee;
	byte que[len_of_int];
	byte hidden_pathn[len_of_int];
	int indx;
	int indx1;
};

#endif