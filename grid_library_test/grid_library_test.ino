
#include <GridM.h>
#include<EEPROM.h>

#define len_of_int 100
#define grid_size 45
GridM g(5,9);

void setup()
{
  Serial.begin(9600);

  g.set_node(2);
  g.set_node(5);
  g.set_node(6);
  g.set_node(14);
  g.set_node(17);
  g.set_node(23);
  g.set_node(24);
  
  g.set_node(31);
  g.set_node(32);
  g.set_node(33);
  //g.set_node(36);
  //g.set_node(29);
  
  g.set_source(4);
  g.set_destination(40);
  Serial.println();
  Serial.println(g.get_path());
  Serial.println();
  g.print_array(g.que);
  Serial.println();
  g.print_array(g.pathn);
 g.print_array(g.path1n);
g.print_array(g.path2n); 
 g.print_array(g.path3n);
 Serial.println(g.path1);
Serial.println(g.path2);
Serial.println(g.path3);
}
void loop()
{
}

