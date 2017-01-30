#ifndef GridM_h
#define GridM_h

#include "Arduino.h"
#include <EEPROM.h>
#ifndef len_of_int
#define len_of_int 100
#endif
#ifndef grid_size
#define grid_size 45
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
  byte name;
  vertice path;
}
node;

class GridM
{
  public:
	String path2;
	String path3;
	String path0;
	//String path4="";
	//String path5="";
	//String path6="";
	//String path7="";
	byte pathn[len_of_int];
	byte path1n[len_of_int];
	byte path2n[len_of_int];
	byte path3n[len_of_int];
    GridM(int row ,int col);//
	String get_path();//
    void show_available_path();//
	void print_array(byte ar[]);//
	void set_node(int node_name);//
	void remove_node(int node_name);//
	void set_source(int source_name);//
	void set_destination(int destination_name);//
	void print_board();//
	void write_board_to_eeprom();//
	String path;//
	String path1;//
	byte que[len_of_int];//
	byte que1[len_of_int];//
	int r;//
	int c;//
	int board[grid_size];//
	void init_grid();//
  private:
	void decode_path(byte arr[],String upor_nich);
	void get_path_ur();//
	void get_path_ul();//
	void get_path_rd();//
	void get_path_ru();//
	void get_path_ld();//
	void get_path_lu();//
	void get_path_dl();//
	void get_path_dr();//
	void find_path(byte ar[]);//
	void init_board();//
	void zero_init(byte a[]);//
	void zero_trim(byte a[]);//
	void search_in_array_and_add_cleverly(byte ar[],int old_value,int new_value);//
	void replace_and_shift(byte arr[],int value,int indx);//
	int len;//
	int destination;//
	int source;//
	node grid[grid_size];//
	String quee;//
	byte hidden_pathn[len_of_int];//
	int indx;//
	int indx1;//
};

#endif