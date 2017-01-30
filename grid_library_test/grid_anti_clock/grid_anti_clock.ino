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
const int r = 5;
const int c = 5;
int board[] = {
  0, 1, 0, 0,0, 
  0, 0, 0, 1,1, 
  0, 1, 0, 0,0, 
  0, 0, 1, 1,0,
  1, 0, 0, 0,0
};
const int len = r * c;
int destination=23;
int source=4;
node grid[len];
const int len_of_int = 100;
String quee="";
byte que[len_of_int];
byte hidden_pathn[len_of_int];
int indx = 0;
int indx1 = 1;
void setup()
{
  Serial.begin(9600);
  init_grid();
  get_path();
  print_array(que);
}
void loop()
{

}
void get_path()
{
  zero_init(que);
  zero_init(hidden_pathn);
  que[0] = source;
  quee = String(source)+" ";
  int indx = 0;
  int indx1 = 1;
  unsigned long time = millis();
  if(source == destination)
  {
    Serial.println("YOu are on the DESTINATION");
  }
  else
  {
    while(true)
    {
      node temp = grid[que[indx]-1];
      if(temp.path.right && (quee.indexOf(String(temp.name+1)+" ") == -1))
      {
        quee += String(temp.name+1)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name+1);
        que[indx1] = temp.name+1;
        indx1++;
        if((temp.name + 1) == destination)
        {
          break;
        }
      }

      if(temp.path.down && (quee.indexOf(String(temp.name+c)+" ") == -1))
      {
        quee += String(temp.name+c)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name+c);
        que[indx1] = temp.name+c;
        indx1++;
        if((temp.name+c) == destination)
        {
          break;
        }
      }
      if(temp.path.left && (quee.indexOf(String(temp.name-1)+" ") == -1))
      {
        quee += String(temp.name-1)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name-1);
        que[indx1] = temp.name-1;
        indx1++;
        if((temp.name-1) == destination)
        {
          break;
        }
      }
      if(temp.path.up && (quee.indexOf(String(temp.name-c)+" ") == -1))
      {
        quee += String(temp.name-c)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name-c);
        que[indx1] = temp.name-c;
        indx1++;
        if((temp.name-c) == destination)
        {
          break;
        }
      }
      indx++;
    }
  } 
  find_path(hidden_pathn);
}
void init_grid()
{
  for(int i = 0;i<len;i++)
  {
    int j = i / c;
    int k = i % c;
    grid[i].name=i+1;
    grid[i].path.left = false;
    grid[i].path.right = false;
    grid[i].path.up = false;
    grid[i].path.down = false;
    if(board[i] == 1) continue;
    if (j == 0 && k == 0)
    {
      grid[i].path.left = false;
      grid[i].path.right = true;
      grid[i].path.up = false;
      grid[i].path.down = true;
    }
    else if (j == 0 && k == (c-1))
    {
      grid[i].path.left = true;
      grid[i].path.right = false;
      grid[i].path.up = false;
      grid[i].path.down = true;
    }
    else if (j == (r-1) && k == 0)
    {
      grid[i].path.left = false;
      grid[i].path.right = true;
      grid[i].path.up = true;
      grid[i].path.down = false;
    }
    else if (j == (r-1) && k == (c-1))
    {
      grid[i].path.left = true;
      grid[i].path.right = false;
      grid[i].path.up = true;
      grid[i].path.down = false;
    }
    else if (j == 0 && k > 0 && k < (c-1))
    {
      grid[i].path.left = true;
      grid[i].path.right = true;
      grid[i].path.up = false;
      grid[i].path.down = true;
    }
    else if (j == (r-1) && k > 0 && k < (c-1))
    {
      grid[i].path.left = true;
      grid[i].path.right = true;
      grid[i].path.up = true;
      grid[i].path.down = false;
    }
    else if (j > 0 && j < (r-1) && k == 0)
    {
      grid[i].path.left = false;
      grid[i].path.right = true;
      grid[i].path.up = true;
      grid[i].path.down = true;
    }
    else if (j > 0 && j < (r-1) && k == (c-1))
    {
      grid[i].path.left = true;
      grid[i].path.right = false;
      grid[i].path.up = true;
      grid[i].path.down = true;
    }
    else
    {
      grid[i].path.left = true;
      grid[i].path.right = true;
      grid[i].path.up = true;
      grid[i].path.down = true;
    }
  }
}
void p(String s)
{
  Serial.println(s);
  delay(100);
}
void p(int s)
{
  Serial.println(s);
  delay(500);
}

void find_path(byte ar[])
{
  int i = 0;
  zero_init(que);
  for(;i<len_of_int;i++)
  {

    if(ar[i]==source)
    {
      int k = 0;
      que[k] = source;
      for(int j = i+1;j<len_of_int;j++)
      {
        k++;
        que[k] = ar[j];
        if(ar[j]==100)
        {
          if(ar[j-1]==destination)
          {
            que[k]=0;
            p(" ");
            print_array(que);
            return;
          }
          i = j;
          zero_init(que);
          break;
        }
      }
    }
  }
}

void show_available_path()
{
  for(int i = 0;i<len;i++)
  {
    p("\nsource:");
    Serial.print(grid[i].name);
    p("de:");
    if(grid[i].path.left)
    {
      Serial.print(" ");
      Serial.print(grid[i].name-1);
    }
    if(grid[i].path.up)
    {
      Serial.print(" ");
      Serial.print(grid[i].name-c);
    }
    if(grid[i].path.down)
    {
      Serial.print(" ");
      Serial.print(grid[i].name+c);
    }
    if(grid[i].path.right)
    {
      Serial.print(" ");
      Serial.print(grid[i].name+1);
    }
  }
}

void zero_init(byte a[])
{
  for(int i = 0;i < len_of_int;i++)
  {
    a[i]=0;
  }
}
void zero_trim(byte a[])
{
  int k = 0;
  for(int i = 0;i < len_of_int;i++)
  {
    if(a[i] != 0)
    {
      a[k] = a[i];
      k++;
    }
  }
}
void print_array(byte ar[])
{
  for(int i = 0;i<len_of_int;i++)
  {
    Serial.print(ar[i]);
    Serial.print(" ");
  }
}
void search_in_array_and_add_cleverly(byte ar[],int old_value,int new_value)
{
  for(int i = 0;i < len_of_int;i++)
  {
    if(i == len_of_int-1)
    {
      int idex = 0;
      while(true)
      {
        if(ar[idex] == 0)
        {
          ar[idex] = old_value;
          ar[idex+1] = new_value;
          ar[idex+2] =100;
          return;
        }
        idex++;
      }
    }
    if(ar[i] == old_value)
    {
      if(ar[i+1] == 100)
      {
        replace_and_shift(ar,new_value,i+1);
        return;
      }
      else
      {
        int j=i-1;
        if(j>=0)
        {
          for(;j!=0;j--)
          {
            if(ar[j]==100)break;
          }
        }
        else
        {
          j = 0;
        }
        if(j == 0)
        {
          for(int l=j;l<=i;l++)
          {
            replace_and_shift(ar,ar[l*2-j],l);
          }
          replace_and_shift(ar,new_value,i+1);
          replace_and_shift(ar,100,i+2);
          return;
        }
        else
        { 
          for(int l=j+1;l<=i;l++)
          {
            replace_and_shift(ar,ar[l*2-j-1],l);
          }
          replace_and_shift(ar,new_value,i+1);
          replace_and_shift(ar,100,i+2);
          return;
        }
      }
    }
  }
}
void replace_and_shift(byte arr[],int value,int indx)
{
  int temp = arr[indx];
  arr[indx]=value;
  for(int i=indx+1;i<len_of_int-1;i++)
  {
    int tem2 = arr[i];
    arr[i]=temp;
    temp = tem2;
  }
}


