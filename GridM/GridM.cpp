#include "Arduino.h"
#include "GridM.h"
GridM::GridM(int row ,int col)
{
	r = row;
	c = col;
	len = r*c;
	indx = 0;
	indx1 = 1;
	quee = "";
	path1 = "";
	path = "";
	init_grid();
	init_board();
}
String GridM::get_path()
{
	path ="";
    path1="";

	//byte path4n[len_of_int];
	//byte path5n[len_of_int];
	//byte path6n[len_of_int];
	//byte path7n[len_of_int];
	
	path2="";
	path3="";
	path0 = "";
	zero_init(pathn);
	zero_init(path1n);
	zero_init(path2n);
	zero_init(path3n);
	//zero_init(path4n);
	//zero_init(path5n);
	//zero_init(path6n);
	//zero_init(path7n);
	
	get_path_ur();
	//print_array(que);
	for(int i=0;i<len_of_int;i++)
	{
		pathn[i] = que[i];
		if(que[i]==0)break;
	}
	
	get_path_rd();
	//print_array(que);
	for(int i=0;i<len_of_int;i++)
	{
		path1n[i] = que[i];
		if(que[i]==0)break;
	}
	
	
	get_path_dl();
	//print_array(que);
	for(int i=0;i<len_of_int;i++)
	{
		path2n[i] = que[i];
		if(que[i]==0)break;
	}
	
	get_path_lu();
	//print_array(que);
	for(int i=0;i<len_of_int;i++)
	{
		path3n[i] = que[i];
		if(que[i]==0)break;
	}
	/*
	get_path_ul();
	for(int i=0;i<len_of_int;i++)
	{
		path5n[i] = que[i];
		if(que[i]==0)break;
	}
	
	get_path_ld();
	for(int i=0;i<len_of_int;i++)
	{
		path6n[i] = que[i];
		if(que[i]==0)break;
	}
	
	get_path_dr();
	for(int i=0;i<len_of_int;i++)
	{
		path7n[i] = que[i];
		if(que[i]==0)break;
	}
	get_path_ru();
	{
		for(int i=0;i<len_of_int;i++)
		{
			path4n[i] = que[i];
			if(que[i]==0)break;
		}
	}
	*/
	decode_path(pathn,quee);
	path0 = path;
	
	decode_path(path1n,quee);
	path1 = path;
	
	decode_path(path2n,quee);
	path2 = path;
	
	decode_path(path3n,quee);
	path3 = path;
	/*
	decode_path(path4n,quee);
	path5 = path;
	
	decode_path(path5n,quee);
	path6 = path;
	
	decode_path(path6n,quee);
	path7 = path;
	
	decode_path(path7n,quee);
	path = path;
	*/
	/*
	Serial.println(path);
	Serial.println(path1);
	Serial.println(path2);
	Serial.println(path3);/*
	Serial.println(path4);
	Serial.println(path5);
	Serial.println(path6);
	Serial.println(path7);
	*/
	//while(true);
	//
	byte all_length[4];
	all_length[0]=path0.length();
	all_length[1]=path1.length();
	all_length[2]=path2.length();
	all_length[3]=path3.length();
	/*
	all_length[4]=path4.length();
	all_length[5]=path5.length();
	all_length[6]=path6.length();
	all_length[7]=path7.length();
	*/
	if(!(pathn[all_length[0]]==destination))
	{
		all_length[0]=100;
	}
	if(!(path1n[all_length[1]]==destination))
	{
		all_length[1]=100;
	}
	if(!(path2n[all_length[2]]==destination))
	{
		all_length[2]=100;
	}
	if(!(path3n[all_length[3]]==destination))
	{
		all_length[3]=100;
	}
	/*
	if(!(path4n[all_length[5]]==destination))
	{
		all_length[5]=100;
	}
	if(!(path5n[all_length[6]]==destination))
	{
		all_length[6]=100;
	}
	if(!(path6n[all_length[7]]==destination))
	{
		all_length[7]=100;
	}
	if(!(path7n[all_length[0]]==destination))
	{
		all_length[0]=100;
	}*/

	byte temp = all_length[0];
	for(int i = 1;i<4;i++)
	{	
		if(temp>all_length[i])
		{
			temp = all_length[i];
		}
	}
	byte no_of_equal_length = 0;
	String ss="";
	for(int i = 0;i<4;i++)
	{
		if(temp == all_length[i])
		{
			ss+=String(i);
			no_of_equal_length++;
		}
	}
	if(no_of_equal_length == 1)
	{
		byte s=ss.charAt(0)-'0';
		switch(s)
		{
			case 0:
			{
				for(int i=0;i<len_of_int;i++)
				{
					que[i] = pathn[i];
				}
				que[0]+=1;
				//
				//Serial.println("returning00");
				//
				return path0;
			}
			break;
			case 1:
			{
				for(int i=0;i<len_of_int;i++)
				{
					que[i] = path1n[i];
				}
				que[0]+=1;
				//
				//Serial.println("returning1");
				//
				return path1;
			}
			break;
			case 2:
			{
				for(int i=0;i<len_of_int;i++)
				{
					que[i] = path2n[i];
				}
				que[0]+=1;
				//
				//Serial.println("returning2");
				//
				return path2;
			}
			break;
			case 3:
			{
				for(int i=0;i<len_of_int;i++)
				{
					que[i] = path3n[i];
				}
				que[0]+=1;
				//
				//Serial.println("returning3");
				//
				return path3;
			}
			break;/*
			case 4:
			{
				for(int i=0;i<len_of_int;i++)
				{
					que[i] = path3n[i];
				}
				que[0]+=1;
				return path4;
			}
			break;
			case 5:
			{      	
				for(int i=0;i<len_of_int;i++)
				{
					que[i] = path4n[i];
				}
				que[0]+=1;
				return path5;
			}
			break;
			case 6:
			{
				for(int i=0;i<len_of_int;i++)
				{
					que[i] = path5n[i];
				}
				que[0]+=1;
				return path6;
			}
			break;
			case 7:
			{
				for(int i=0;i<len_of_int;i++)
				{
					que[i] = path6n[i];
				}
				que[0]+=1;
				return path7;
			}
			break;*/
		}
	}
	else
	{
		
		String sss="";
		byte lent = ss.length();
		for(int i=0;i<lent;i++)
		{
			byte in = ss.charAt(i)-'0';
			int no_of_s=0;
			switch(in)
			{
				case 0:
				{
					for(int i=0;i<all_length[0];i++)
					{
						if(path0.charAt(i)=='S')
						{
							no_of_s++;
						}
					}
					sss+=String(no_of_s);
				}
				break;
				case 1:
				{
					for(int i=0;i<all_length[1];i++)
					{
						if(path1.charAt(i)=='S')
						{
							no_of_s++;
						}
					}
					sss+=String(no_of_s);
				}
				break;
				case 2:
				{
					for(int i=0;i<all_length[2];i++)
					{
						if(path2.charAt(i)=='S')
						{
							no_of_s++;
						}
					}
					sss+=String(no_of_s);
				}
				break;
				case 3:
				{
					for(int i=0;i<all_length[3];i++)
					{
						if(path3.charAt(i)=='S')
						{
							no_of_s++;
						}
					}
					sss+=String(no_of_s);
				}
				break;/*
				case 4:
				{
					for(int i=0;i<all_length[4];i++)
					{
						if(path4.charAt(i)=='S')
						{
							no_of_s++;
						}
					}
					sss+=String(no_of_s);
				}
				break;
				case 5:
				{
					for(int i=0;i<all_length[5];i++)
					{
						if(path5.charAt(i)=='S')
						{
							no_of_s++;
						}
					}
					sss+=String(no_of_s);
				}
				break;
				case 6:
				{
					for(int i=0;i<all_length[6];i++)
					{
						if(path6.charAt(i)=='S')
						{
							no_of_s++;
						}
					}
					sss+=String(no_of_s);
				}
				break;
				case 7:
				{
					for(int i=0;i<all_length[7];i++)
					{
						if(path7.charAt(i)=='S')
						{
							no_of_s++;
						}
					}
					sss+=String(no_of_s);
				}
				break;*/
			}
		}
		
		byte temp = sss.charAt(0)-'0';
		for(int i = 1;i<lent;i++)
		{	
			if(temp<(sss.charAt(i)-'0'))
			{
				temp = (sss.charAt(i)-'0');
			}
		}
		for(int i = 0;i<lent;i++)
		{
			if(temp == (sss.charAt(i)-'0'))
			{
				switch((ss.charAt(i)-'0'))
				{
					case 0:
					{
						for(int i=0;i<len_of_int;i++)
						{
							que[i] = pathn[i];
						}
						que[0]+=1;
						//
						//Serial.println("returning0");
						//
						return path0;
					}
					break;
					case 1:
					{
						for(int i=0;i<len_of_int;i++)
						{
							que[i] = path1n[i];
						}
						que[0]+=1;
						//
						//Serial.println("returning1");
						//
						return path1;
					}
					break;
					case 2:
					{
						for(int i=0;i<len_of_int;i++)
						{
							que[i] = path2n[i];
						}
						que[0]+=1;
						//
						//Serial.println("returning2");
						//
						
						return path2;
					}
					break;
					case 3:
					{
						for(int i=0;i<len_of_int;i++)
						{
							que[i] = path3n[i];
						}
						que[0]+=1;
						//
						//Serial.println("returning3");
						//
						return path3;
					}
					break;/*
					case 4:
					{
						for(int i=0;i<len_of_int;i++)
						{
							que[i] = path3n[i];
						}
						que[0]+=1;
						return path4;
					}
					break;
					case 5:
					{      	
						for(int i=0;i<len_of_int;i++)
						{
							que[i] = path4n[i];
						}
						que[0]+=1;
						return path5;
					}
					break;
					case 6:
					{
						for(int i=0;i<len_of_int;i++)
						{
							que[i] = path5n[i];
						}
						que[0]+=1;
						return path6;
					}
					break;
					case 7:
					{
						for(int i=0;i<len_of_int;i++)
						{
							que[i] = path6n[i];
						}
						que[0]+=1;
						return path7;
					}
					break;*/
				}    
				break;
			}
		}
	}
	return "S";
}

void GridM::decode_path(byte arr[],String upor_nich)
{
  path = "S";
  upor_nich = "";
  int i = 0;
  while(true)
  {
	if(i+1 >= len_of_int)
	{
		break;
	}
    if(arr[i+1] == 0)
	{
		break;
	}
    int temp_val = arr[i]-arr[i+1];
    i++;
    if(temp_val == -1)
    {
      upor_nich += "D";
    }
    else if(temp_val == 1)
    {
      upor_nich += "B";
    }
    else if(temp_val == -c)
    {
      upor_nich += "N";
    }
    else if(temp_val == c)
    {
      upor_nich += "U";
    }
    if(i > 1)
    {
      boolean L_R = upor_nich.equals("BN")||upor_nich.equals("UB")||upor_nich.equals("ND")||upor_nich.equals("DU");
      if(L_R)
      {
        path += "L";
        upor_nich = upor_nich.substring(1);
        continue;
      }
      L_R = upor_nich.equals("NB")||upor_nich.equals("BU")||upor_nich.equals("DN")||upor_nich.equals("UD");
      if(L_R)
      {
        path += "R";
        upor_nich = upor_nich.substring(1);
        continue;
      }
      else
      {
        path += "S";
        upor_nich = upor_nich.substring(1);
      }
    }
  }
}

void GridM::get_path_ul()
{
  zero_init(que);
  zero_init(hidden_pathn);
  que[0] = source;
  quee = "";
  quee = " "+String(source)+" ";
  int indx = 0;
  int indx1 = 1;
  if(source == destination)
  {
    Serial.println("YOu are on the DESTINATION");
  }
  else
  {
    while(true)
    {
	  if(indx1 >45)
	  {
	 	break;
	  }
      node temp = grid[que[indx]-1];

	  if(temp.path.up && (quee.indexOf(" "+String(temp.name-c)+" ") == -1))
      {
        quee += " "+String(temp.name-c)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name-c);
        que[indx1] = temp.name-c;
        indx1++;
        if((temp.name-c) == destination)
        {
          break;
        }
      }

      if(temp.path.left && (quee.indexOf(" "+String(temp.name-1)+" ") == -1))
      {
        quee += " "+String(temp.name-1)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name-1);
        que[indx1] = temp.name-1;
        indx1++;
        if((temp.name-1) == destination)
        {
          break;
        }
      }
	  if(temp.path.down && (quee.indexOf(" "+String(temp.name+c)+" ") == -1))
      {
        quee += " "+String(temp.name+c)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name+c);
        que[indx1] = temp.name+c;
        indx1++;
        if((temp.name+c) == destination)
        {
          break;
        }
      }
	  if(temp.path.right && (quee.indexOf(" "+String(temp.name+1)+" ") == -1))
      {
        quee += " "+String(temp.name+1)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name+1);
        que[indx1] = temp.name+1;
        indx1++;
        if((temp.name + 1) == destination)
        {
          break;
        }
      }
      indx++;
    }
  }
  find_path(hidden_pathn);
}

void GridM::get_path_ur()
{
  zero_init(que);
  zero_init(hidden_pathn);
  que[0] = source;
  quee = "";
  quee = " "+String(source)+" ";
  int indx = 0;
  int indx1 = 1;
  if(source == destination)
  {
    Serial.println("YOu are on the DESTINATION");
  }
  else
  {
    while(true)
    {
	  if(indx1 >45)
	  {
	 	break;
	  }
      node temp = grid[que[indx]-1];

	  if(temp.path.up && (quee.indexOf(" "+String(temp.name-c)+" ") == -1))
      {
        quee += " "+String(temp.name-c)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name-c);
        que[indx1] = temp.name-c;
        indx1++;
        if((temp.name-c) == destination)
        {
          break;
        }
      }
	  if(temp.path.right && (quee.indexOf(" "+String(temp.name+1)+" ") == -1))
      {
        quee += " "+String(temp.name+1)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name+1);
        que[indx1] = temp.name+1;
        indx1++;
        if((temp.name + 1) == destination)
        {
          break;
        }
      }
	  if(temp.path.down && (quee.indexOf(" "+String(temp.name+c)+" ") == -1))
      {
        quee += " "+String(temp.name+c)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name+c);
        que[indx1] = temp.name+c;
        indx1++;
        if((temp.name+c) == destination)
        {
          break;
        }
      }
	  if(temp.path.left && (quee.indexOf(" "+String(temp.name-1)+" ") == -1))
      {
        quee += " "+String(temp.name-1)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name-1);
        que[indx1] = temp.name-1;
        indx1++;
        if((temp.name-1) == destination)
        {
          break;
        }
      }
      indx++;
    }
  }
  find_path(hidden_pathn);
}

void GridM::get_path_ru()
{
  zero_init(que);
  zero_init(hidden_pathn);
  que[0] = source;
  quee = "";
  quee = " "+String(source)+" ";
  int indx = 0;
  int indx1 = 1;
  if(source == destination)
  {
    Serial.println("YOu are on the DESTINATION");
  }
  else
  {
    while(true)
    {
	  if(indx1 >45)
	  {
	 	break;
	  }
      node temp = grid[que[indx]-1];
      if(temp.path.right && (quee.indexOf(" "+String(temp.name+1)+" ") == -1))
      {
        quee += " "+String(temp.name+1)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name+1);
        que[indx1] = temp.name+1;
        indx1++;
        if((temp.name + 1) == destination)
        {
          break;
        }
      }
	  if(temp.path.up && (quee.indexOf(" "+String(temp.name-c)+" ") == -1))
      {
        quee += " "+String(temp.name-c)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name-c);
        que[indx1] = temp.name-c;
        indx1++;
        if((temp.name-c) == destination)
        {
          break;
        }
      }

      if(temp.path.left && (quee.indexOf(" "+String(temp.name-1)+" ") == -1))
      {
        quee += " "+String(temp.name-1)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name-1);
        que[indx1] = temp.name-1;
        indx1++;
        if((temp.name-1) == destination)
        {
          break;
        }
      }
	  if(temp.path.down && (quee.indexOf(" "+String(temp.name+c)+" ") == -1))
      {
        quee += " "+String(temp.name+c)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name+c);
        que[indx1] = temp.name+c;
        indx1++;
        if((temp.name+c) == destination)
        {
          break;
        }
      }
      indx++;
    }
  }
  find_path(hidden_pathn);
}



void GridM::get_path_rd()
{
  zero_init(que);
  zero_init(hidden_pathn);
  que[0] = source;
  quee = "";
  quee = " "+String(source)+" ";
  int indx = 0;
  int indx1 = 1;
  if(source == destination)
  {
    Serial.println("YOu are on the DESTINATION");
  }
  else
  {
    while(true)
    {
	  if(indx1 >45)
	  {
	 	break;
	  }
      node temp = grid[que[indx]-1];
      if(temp.path.right && (quee.indexOf(" "+String(temp.name+1)+" ") == -1))
      {
        quee += " "+String(temp.name+1)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name+1);
        que[indx1] = temp.name+1;
        indx1++;
        if((temp.name + 1) == destination)
        {
          break;
        }
      }

      if(temp.path.down && (quee.indexOf(" "+String(temp.name+c)+" ") == -1))
      {
        quee += " "+String(temp.name+c)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name+c);
        que[indx1] = temp.name+c;
        indx1++;
        if((temp.name+c) == destination)
        {
          break;
        }
      }
      if(temp.path.left && (quee.indexOf(" "+String(temp.name-1)+" ") == -1))
      {
        quee += " "+String(temp.name-1)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name-1);
        que[indx1] = temp.name-1;
        indx1++;
        if((temp.name-1) == destination)
        {
          break;
        }
      }
      if(temp.path.up && (quee.indexOf(" "+String(temp.name-c)+" ") == -1))
      {
        quee += " "+String(temp.name-c)+" ";
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

void GridM::get_path_lu()
{
	
  zero_init(que);
  zero_init(hidden_pathn);
  que[0] = source;
  quee = "";
  quee = " "+String(source)+" ";
  int indx = 0;
  int indx1 = 1;
  if(source == destination)
  {
    Serial.println("YOu are on the DESTINATION");
  }
  else
  {
    while(true)
    {
	  if(indx1 >45)
	  {
	 	break;
	  }
      node temp = grid[que[indx]-1];
      if(temp.path.left && (quee.indexOf(" "+String(temp.name-1)+" ") == -1))
      {
        quee += " "+String(temp.name-1)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name-1);
        que[indx1] = temp.name-1;
        indx1++;
        if((temp.name - 1) == destination)
        {
          break;
        }
      }

      if(temp.path.up && (quee.indexOf(" "+String(temp.name-c)+" ") == -1))
      {
        quee += " "+String(temp.name-c)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name-c);
        que[indx1] = temp.name-c;
        indx1++;
        if((temp.name-c) == destination)
        {
          break;
        }
      }
      if(temp.path.right && (quee.indexOf(" "+String(temp.name+1)+" ") == -1))
      {
        quee += " "+String(temp.name+1)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name+1);
        que[indx1] = temp.name+1;
        indx1++;
        if((temp.name+1) == destination)
        {
          break;
        }
      }
      if(temp.path.down && (quee.indexOf(" "+String(temp.name+c)+" ") == -1))
      {
        quee += " "+String(temp.name+c)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name+c);
        que[indx1] = temp.name+c;
        indx1++;
        if((temp.name+c) == destination)
        {
          break;
        }
      }
      indx++;
    }
  } 
  find_path(hidden_pathn);
}
void GridM::get_path_ld()
{
	
  zero_init(que);
  zero_init(hidden_pathn);
  que[0] = source;
  quee = "";
  quee = " "+String(source)+" ";
  int indx = 0;
  int indx1 = 1;
  if(source == destination)
  {
    Serial.println("YOu are on the DESTINATION");
  }
  else
  {
    while(true)
    {
	  if(indx1 > 45)
	  {
	 	break;
	  }
      node temp = grid[que[indx]-1];
      if(temp.path.left && (quee.indexOf(" "+String(temp.name-1)+" ") == -1))
      {
        quee += " "+String(temp.name-1)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name-1);
        que[indx1] = temp.name-1;
        indx1++;
        if((temp.name - 1) == destination)
        {
          break;
        }
      }
	  if(temp.path.down && (quee.indexOf(" "+String(temp.name+c)+" ") == -1))
      {
        quee += " "+String(temp.name+c)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name+c);
        que[indx1] = temp.name+c;
        indx1++;
        if((temp.name+c) == destination)
        {
          break;
        }
      }
      if(temp.path.right && (quee.indexOf(" "+String(temp.name+1)+" ") == -1))
      {
        quee += " "+String(temp.name+1)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name+1);
        que[indx1] = temp.name+1;
        indx1++;
        if((temp.name+1) == destination)
        {
          break;
        }
      }
      if(temp.path.up && (quee.indexOf(" "+String(temp.name-c)+" ") == -1))
      {
        quee += " "+String(temp.name-c)+" ";
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
void GridM::get_path_dr()
{
	
  zero_init(que);
  zero_init(hidden_pathn);
  que[0] = source;
  quee = "";
  quee = " "+String(source)+" ";
  int indx = 0;
  int indx1 = 1;
  if(source == destination)
  {
    Serial.println("YOu are on the DESTINATION");
  }
  else
  {
    while(true)
    {
	  if(indx1 > 45)
	  {
	 	break;
	  }
      node temp = grid[que[indx]-1];
      
	  if(temp.path.down && (quee.indexOf(" "+String(temp.name+c)+" ") == -1))
      {
        quee += " "+String(temp.name+c)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name+c);
        que[indx1] = temp.name+c;
        indx1++;
        if((temp.name+c) == destination)
        {
          break;
        }
      }
      if(temp.path.right && (quee.indexOf(" "+String(temp.name+1)+" ") == -1))
      {
        quee += " "+String(temp.name+1)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name+1);
        que[indx1] = temp.name+1;
        indx1++;
        if((temp.name+1) == destination)
        {
          break;
        }
      }
      if(temp.path.up && (quee.indexOf(" "+String(temp.name-c)+" ") == -1))
      {
        quee += " "+String(temp.name-c)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name-c);
        que[indx1] = temp.name-c;
        indx1++;
        if((temp.name-c) == destination)
        {
          break;
        }
      }
	  if(temp.path.left && (quee.indexOf(" "+String(temp.name-1)+" ") == -1))
      {
        quee += " "+String(temp.name-1)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name-1);
        que[indx1] = temp.name-1;
        indx1++;
        if((temp.name - 1) == destination)
        {
          break;
        }
      }
      
      indx++;
    }
  } 
  find_path(hidden_pathn);
}

void GridM::get_path_dl()
{
	
  zero_init(que);
  zero_init(hidden_pathn);
  que[0] = source;
  quee = "";
  quee = " "+String(source)+" ";
  int indx = 0;
  int indx1 = 1;
  if(source == destination)
  {
    Serial.println("YOu are on the DESTINATION");
  }
  else
  {
    while(true)
    {
	  if(indx1 > 45)
	  {
	 	break;
	  }

	  
      node temp = grid[que[indx]-1];
	  if(temp.name>45)
	  {
		Serial.print(que[indx]-1);
		Serial.print("  ");
		Serial.print(que[indx-1]);
		Serial.print("  ");
		Serial.print(indx);
	  }
	  if(temp.path.down && (quee.indexOf(" "+String(temp.name+c)+" ") == -1))
      {
        quee += " "+String(temp.name+c)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name+c);
        que[indx1] = temp.name+c;
        indx1++;
        if((temp.name+c) == destination)
        {
          break;
        }
      }
	  if(temp.path.left && (quee.indexOf(" "+String(temp.name-1)+" ") == -1))
      {
        quee += " "+String(temp.name-1)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name-1);
        que[indx1] = temp.name-1;
        indx1++;
        if((temp.name - 1) == destination)
        {
          break;
        }
      }
      if(temp.path.up && (quee.indexOf(" "+String(temp.name-c)+" ") == -1))
      {
        quee += " "+String(temp.name-c)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name-c);
        que[indx1] = temp.name-c;
        indx1++;
        if((temp.name-c) == destination)
        {
          break;
        }
      }
	  if(temp.path.right && (quee.indexOf(" "+String(temp.name+1)+" ") == -1))
      {
        quee += " "+String(temp.name+1)+" ";
        search_in_array_and_add_cleverly(hidden_pathn,temp.name,temp.name+1);
        que[indx1] = temp.name+1;
        indx1++;
        if((temp.name+1) == destination)
        {
          break;
        }
      }      
      indx++;
    }
  }  
  find_path(hidden_pathn);
}

void GridM::show_available_path()
{
  for(int i = 0;i<len;i++)
  {
    Serial.println("\nsource:");
    Serial.print(grid[i].name);
    Serial.println("de:");
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

void GridM::print_array(byte ar[])
{
  for(int i = 0;i<len_of_int;i++)
  {
    Serial.print(ar[i]);
    Serial.print(" ");
  }
  Serial.println();
}
void GridM::print_board()
{
	for(int i =0;i<r;i++)
	{
		Serial.println();
		for(int j=0;j<c;j++)
		{
			Serial.print(board[i*c+j]);
		}
	}
}
void GridM::set_node(int node_name)
{
	board[node_name-1] = 1;
	init_grid();
}
void GridM::remove_node(int node_name)
{
	board[node_name-1] = 0;
	init_grid();
}

void GridM::set_source(int source_name)
{
	source = source_name;
}
void GridM::set_destination(int destination_name)
{
	destination = destination_name;
}

void GridM::find_path(byte ar[])
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

void GridM::init_board()
{
	for(int i = 0;i < r*c;i++)
	{
		board[i] = 0;
	}
}
void GridM::init_grid()
{
  for(int i = 0;i<grid_size;i++)
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


void GridM::zero_init(byte a[])
{
  for(int i = 0;i < len_of_int;i++)
  {
    a[i]=0;
  }
}
void GridM::zero_trim(byte a[])
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
void GridM::search_in_array_and_add_cleverly(byte ar[],int old_value,int new_value)
{
  for(int i = 0;i < len_of_int;i++)
  {
    if(i == len_of_int-1)
    {
      int idex = 0;
      while(true)
      {
		if(idex >= len_of_int)
		{
			break;
		}
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
void GridM::replace_and_shift(byte arr[],int value,int indx)
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
void GridM::write_board_to_eeprom()
{
	
	for(int i = 0;i < r*c;i++)
	{
		EEPROM.write(50+i,byte(board[i]));
		delay(10);
	}
}

