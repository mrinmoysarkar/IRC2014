
String upor_nich = "";
String path = "";
int arr[]={4,3,8,7,6,11,16,17,22,23,0,0};
void setup()
{
  Serial.begin(9600);
  int c=5;
  int i = 0;
  while(true)
  {
    if(arr[i+1] == 0)break;
    int temp_val = arr[i]-arr[i+1];
    i++;
    Serial.println(temp_val);
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
  Serial.println(path);
}
void loop()
{
  
}
