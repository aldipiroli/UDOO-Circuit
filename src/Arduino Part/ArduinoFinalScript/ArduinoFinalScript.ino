#include <Wire.h>  
#include <LiquidCrystal_I2C.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;
LiquidCrystal_I2C lcd(0x27,16,2);

void setup()
{
  Serial.begin(9600);
  Serial0.begin(9600);
  
  lcd.init();                    
  lcd.backlight();

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
}

void loop()
{
  if(Serial0.available() > 0)
  {
    if(gps.encode(Serial0.read()))
    {
      float lat_, lng_, vel_;
      
      if(gps.location.isValid())
      {
        lat_ = gps.location.lat();
        lng_ = gps.location.lng();
        vel_ = gps.speed.kmph();

        LCD_Print_Words("LAT: ",0,0);
        LCD_Print_Value(lat_,0,5,7);
        LCD_Print_Words("LNG: ",0,0);
        LCD_Print_Value(lng_,1,5,7);
  
        String in_string = "";
        int degree = 0;
        
        while(Serial.available() > 0)
        {
          char in_byte = Serial.read();
          
          if(in_byte != '#')
          {
            in_string = in_string + in_byte;
          }
          else
          {
            degree = in_string.toInt();
            in_string = "";
  
            Activate_LED(degree);
  
            lat_ = ( lat_ * pow(10.0,6) );
            lng_ = ( lng_ * pow(10.0,6) );
            
            Serial.println(lat_);
            Serial.println(lng_);
            Serial.println(vel_);
          }
        } 
      }
    }
  }
}

void LCD_Print_Words(String words,int r, int c)
{
  int len=words.length();
  char c_val[len];
  
  words.toCharArray(c_val,len);
  
  if(r<2 && c<17)
  {
    lcd.setCursor(c,r);
    for(int i=0; i<len-1; i++)
    {
      lcd.write(int(c_val[i]));
    }
  }
  else
  {
    if(r>=2)
      LCD_Print_Words("Errore riga. ", 0, 0);
      
    if(c>= 17) 
      LCD_Print_Words("Errore colonna ", 1, 0);
      
    if(r>=2 && c>=17)
    {
      LCD_Print_Words("Errore riga. ", 0, 0);
      LCD_Print_Words("Errore colonna. ", 1, 0);
    }
  }
}

void LCD_Print_Value(float value_, int row, int column, int precision)
{
  int nr_int, i, j, len_string, flag=0;
  String nr_string; 
      
  nr_int = int(pow(10.0, precision) * value_);
  nr_string = String(nr_int);
  len_string = nr_string.length();
  
  char nr_charArray[len_string];
  nr_string.toCharArray(nr_charArray, len_string);
  
  int aux = precision - len_string;

  lcd.setCursor(column,row);
  
  for(i = 0; i < len_string-1; i++)
  {
    if((i == (len_string - precision) || (len_string - precision) <= 0 ) && flag == 0 )
    {    
      if(aux > 0)
      {
        lcd.write(48);
        lcd.write(46); 
                
        for(j = 0; j < aux; j++)
        {
          lcd.write(48);
        }
        flag = 1;
      }
      else 
      {
        if(i == 0)
        {
          lcd.write(48); 
          lcd.write(46);
          flag = 1;
        }
        else
        {
          lcd.write(46);
          flag = 1;
        }
      }
    }          
    lcd.write(int(nr_charArray[i]));
  }
}

void Activate_LED(int degree_)
{
  if( degree_ >= -90 && degree_ <= 90 )
  {
    if( degree_ >= -10 )
    {
      if( degree_ >= -10 && degree_ <=10 )
      {
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        digitalWrite(9, LOW);
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);
        digitalWrite(12, LOW);
      }
      else if( degree_ > 10 && degree_ <= 20 )
      {
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        digitalWrite(9, LOW);
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);
        digitalWrite(12, LOW);
      }
      else if( degree_ > 20 && degree_ <= 30 )
      {
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        digitalWrite(9, LOW);
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);
        digitalWrite(12, LOW);
      }
      else if( degree_ >30 && degree_ <= 45 )
      {
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        digitalWrite(9, LOW);
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);
        digitalWrite(12, LOW);
      }
      else if( degree_ >45 && degree_ <= 65 )
      { 
        digitalWrite(2, LOW);
        digitalWrite(3, HIGH);
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        digitalWrite(9, LOW);
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);
        digitalWrite(12, LOW);
      }
      else if( degree_ >65 && degree_ <= 90 )
      {
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        digitalWrite(4, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        digitalWrite(9, LOW);
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);
        digitalWrite(12, LOW);
      }
    }
    else
    {
      if(degree_ >= -20 && degree_ < -10)
      {
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
        digitalWrite(7, HIGH);
        digitalWrite(8, HIGH);
        digitalWrite(9, LOW);
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);
        digitalWrite(12, LOW);
      }
      else if( degree_ <-20 && degree_ >= -30 )
      {
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
        digitalWrite(7, HIGH);
        digitalWrite(8, HIGH);
        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);
        digitalWrite(12, LOW);
      }
      else if( degree_<-30 && degree_ >= -45)
      {
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
        digitalWrite(7, HIGH);
        digitalWrite(8, HIGH);
        digitalWrite(9, HIGH);
        digitalWrite(10, HIGH);
        digitalWrite(11, LOW);
        digitalWrite(12, LOW);
      }
      else if( degree_<-45 && degree_ >= -65)
      {
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
        digitalWrite(7, HIGH);
        digitalWrite(8, HIGH);
        digitalWrite(9, HIGH);
        digitalWrite(10, HIGH);
        digitalWrite(11, HIGH);
        digitalWrite(12, LOW);
      }
      else if( degree_<-65 && degree_ >= -90)
      {
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
        digitalWrite(7, HIGH);
        digitalWrite(8, HIGH);
        digitalWrite(9, HIGH);
        digitalWrite(10, HIGH);
        digitalWrite(11, HIGH);
        digitalWrite(12, HIGH);
      }
    }
  }
  else
  {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);;
  }
}
