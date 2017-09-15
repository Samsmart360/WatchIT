
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
int incomingByte = 0;



//ROOT_MODE_CONTROL = 0 => HOME_SCREEN_MODE
//ROOT_MODE_CONTROL = 1 => MAIN_MENU_SCREEN_MODE
//ROOT_MODE_CONTROL = 2 => APPLICATION_MODE

//APPID = 0 => FITNESS APP || APPLICATION_CONTROL_MODE = 0
//APPID = 1 => PULSE APP || APPLICATION_CONTROL_MODE = 0
//APPID = 2 => WEATHER APP || APPLICATION_CONTROL_MODE = 0
//APPID = 3 => NOTIFICATIONS APP || APPLICATION_CONTROL_MODE = 0 -> Custom
//APPID = 4 => CLOCK APP || APPLICATION_CONTROL_MODE = 0 -> Custom
//APPID = 5 => HOME AUTOMATION APP || APPLICATION_CONTROL_MODE = 0 -> Custom
//APPID = 6 => SETTINGS APP || APPLICATION_CONTROL_MODE = 0 -> Custom

const int UP_BUTTON = 0;
const int SELECT_BUTTON = 1;
const int DOWN_BUTTON = 2;
const int BACK_BUTTON = 3;

int MainMenuOptionsDisplayed[3] = {0,1,2};
String MenuItems[7] = {"Fitness","Pulse","Weather","Notifications","Clock","Home Auto","Settings"};

int APPLICATION_ID;
int HOME_SCREEN_CONTROL_MODE = 0; // 0 -> 3
int MAIN_MENU_SCREEN_CONTROL_MODE = 0; // 0 -> 6
int APPLICATION_CONTROL_MODE = 0; // 0 -> Custom

int ROOT_MODE_CONTROL = 0;

void UpButtonPress(){
  switch(ROOT_MODE_CONTROL){
    case 0 : //HOME_SCREEN
              HomeScreenButtonPress(UP_BUTTON);
              break;
              
    case 1 : //MAIN_MENU_SCREEN
              MainMenuScreenButtonPress(UP_BUTTON);
              break;
              
    case 2 : //APPLICATION_SCREEN
              ApplicationScreenButtonPress(UP_BUTTON);
              break;
  }
}


void SelectButtonPress(){
    switch(ROOT_MODE_CONTROL){
    case 0 : //HOME_SCREEN
              HomeScreenButtonPress(SELECT_BUTTON);
              break;
              
    case 1 : //MAIN_MENU_SCREEN
              MainMenuScreenButtonPress(SELECT_BUTTON);
              break;
              
    case 2 : //APPLICATION_SCREEN
              ApplicationScreenButtonPress(SELECT_BUTTON);
              break;
  }
}


void DownButtonPress(){
    switch(ROOT_MODE_CONTROL){
    case 0 : //HOME_SCREEN
              HomeScreenButtonPress(DOWN_BUTTON);
              break;
              
    case 1 : //MAIN_MENU_SCREEN
              MainMenuScreenButtonPress(DOWN_BUTTON);
              break;
              
    case 2 : //APPLICATION_SCREEN
              ApplicationScreenButtonPress(DOWN_BUTTON);
              break;
  }
}


void BackButtonPress(){
    switch(ROOT_MODE_CONTROL){
    case 0 : //HOME_SCREEN
              HomeScreenButtonPress(BACK_BUTTON);
              break;
              
    case 1 : //MAIN_MENU_SCREEN
              MainMenuScreenButtonPress(BACK_BUTTON);
              break;
              
    case 2 : //APPLICATION_SCREEN
              ApplicationScreenButtonPress(BACK_BUTTON);
              break;
  }
}



void HomeScreenButtonPress(int button_press){
 switch(button_press){
  case 0: //UP BUTTON PRESS
          //ROOT_MODE_CONTROL = 1;//GOING TO MENU MODE||NOTIFICATIONS
          Home_Screen_Notifications();
          break;
          
  case 1: //SELECT BUTTON PRESS
          ROOT_MODE_CONTROL = 1;//GOING TO MENU MODE||
          Home_Screen_Main_Menu();
          break;
          
  case 2: //DOWN BUTTON PRESS
          //ROOT_MODE_CONTROL = 1;//GOING TO MENU MODE
          Home_Screen_Scheduling();
          break;
          
  case 3: //BACK BUTTON PRESS
          Home_Screen_Do_Nothing();
          break;     
  } 
}


void MainMenuScreenButtonPress(int button_press){
 switch(button_press){
  case 0: //UP BUTTON PRESS
          if(MAIN_MENU_SCREEN_CONTROL_MODE < 6){
              MAIN_MENU_SCREEN_CONTROL_MODE++;
            }
           else{
              MAIN_MENU_SCREEN_CONTROL_MODE = 0;
            }

            RefreshMenu(MAIN_MENU_SCREEN_CONTROL_MODE);
          break;
          
  case 1: //SELECT BUTTON PRESS
          ROOT_MODE_CONTROL = 2;
          MainMenuOptionsSelect(MAIN_MENU_SCREEN_CONTROL_MODE);
          break;
          
  case 2: //DOWN BUTTON PRESS
           if(MAIN_MENU_SCREEN_CONTROL_MODE > 0){
              MAIN_MENU_SCREEN_CONTROL_MODE--;
            }
           else{
              MAIN_MENU_SCREEN_CONTROL_MODE = 6;
            }
            RefreshMenu(MAIN_MENU_SCREEN_CONTROL_MODE);
          break;
          
  case 3: //BACK BUTTON PRESS
          MAIN_MENU_SCREEN_CONTROL_MODE = 0;
          ROOT_MODE_CONTROL = 0;
          break;     
  } 
}

void ApplicationScreenButtonPress(int button_press){
 switch(button_press){
  case 0: //UP BUTTON PRESS
          break;
          
  case 1: //SELECT BUTTON PRESS
          break;
          
  case 2: //DOWN BUTTON PRESS
          break;
          
  case 3: //BACK BUTTON PRESS
          APPLICATION_ID = 0;
          ROOT_MODE_CONTROL = 1;
          break;     
  } 
}




void MainMenuOptionsSelect(int APPID){
  switch(APPID){
    case 0:  // FITNESS APP
            Application_Fitness();
            break;
            
    case 1:  // PULSE APP
            Application_Pulse();
            break;
              
    case 2:  // WEATHER APP
            Application_Weather();
            break;
            
    case 3:  // NOTIFICATION APP
            Application_Notifications();
            break;
            
    case 4:  // CLOCK APP
            Application_Clock();
            break;
            
    case 5:  // HOME AUTOMATION APP
            Application_Home_Automation();
            break;
            
    case 6:  // SETTINGS APP
            Application_Settings();
            break;
  }  
}


int inp;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(10);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);// initialize with the I2C addr 0x3C (for the 128x32)
  display.clearDisplay();
  RefreshMenu(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    inp =  Serial.parseInt();
    switch(inp){
      case 0 : 
                UpButtonPress();
                break;
      case 1 : 
               SelectButtonPress();
                break;
      case 2 : 
                DownButtonPress();
                break;
      case 3 :
               BackButtonPress();
                break;  
      case 4 : Serial.print("ROOT MODE : ");
               Serial.println(ROOT_MODE_CONTROL);
               Serial.print("MAIN MENU MODE : ");
               Serial.println(MAIN_MENU_SCREEN_CONTROL_MODE);
               Serial.print("APPLICATION ID : ");
               Serial.println(APPLICATION_ID);
    }  
  }
}




void Home_Screen_Notifications(){
  Serial.println("NOTIFICATIONS");
}

void Home_Screen_Main_Menu(){
  
  Serial.println("MAIN MENU");
}

void Home_Screen_Scheduling(){
  Serial.println("SCHEDULING");
}

void Home_Screen_Do_Nothing(){
  Serial.println("DO NOTHING");
}






void Application_Fitness(){
  APPLICATION_ID = 0;
  Serial.println("FITNESS");
}

void Application_Pulse(){
  APPLICATION_ID = 1;
  Serial.println("PULSE");
}

void Application_Weather(){
  APPLICATION_ID = 2;
  Serial.println("WEATHER");
}

void Application_Notifications(){
  APPLICATION_ID = 3;
  Serial.println("NOTIFICATIONS");
}

void Application_Clock(){
  APPLICATION_ID = 4;
  Serial.println("CLOCK");
}

void Application_Home_Automation(){
  APPLICATION_ID = 5;
  Serial.println("HOME AUTOMATION");
}

void Application_Settings(){
  APPLICATION_ID = 6;
  Serial.println("SETTINGS");
}












void RefreshMenu(int x) {
  int flag = 0;
  uint8_t color1 = WHITE;
  uint8_t color2 = WHITE;
  uint8_t color3 = WHITE;
  int i;

  
  for(i = 0 ; i <= 2 ; i++){
    Serial.println(MainMenuOptionsDisplayed[i]);
  }

  Serial.println();

  
  for(i = 0 ; i <= 2 ; i++){
    if(x == MainMenuOptionsDisplayed[i])
    {
      flag = 1;
      break;
    }
  }
  
  if(flag){
  if (i == 0) {
    color2 = BLACK;
    color3 = BLACK;
    display.fillRect(0, 0, display.width(), display.height() / 4, color1);
    display.fillRect(0, 10, display.width(), display.height() / 4, color2);
    display.fillRect(0, 20, display.width(), display.height() / 4, color3);

  }
  else if (i == 1) {
    color1 = BLACK;
    color3 = BLACK;
    display.fillRect(0, 0, display.width(), display.height() / 4, color1);
    display.fillRect(0, 10, display.width(), display.height() / 4, color2);
    display.fillRect(0, 20, display.width(), display.height() / 4, color3);
  }

  else if (i == 2) {
    color1 = BLACK;
    color2 = BLACK;
    display.fillRect(0, 0, display.width(), display.height() / 4, color1);
    display.fillRect(0, 10, display.width(), display.height() / 4, color2);
    display.fillRect(0, 20, display.width(), display.height() / 4, color3);
  }   
  
  
  if(color1 == BLACK){
    color1 = WHITE;
  }else{
    color1 = BLACK;  
  }


 if(color2 == BLACK){
    color2 = WHITE;
 }else{
    color2 = BLACK;  
 }

 if(color3 == BLACK){
    color3 = WHITE;
 }else{
    color3 = BLACK;  
 }

 
   
   for(int j = 0 ; j < 3 ; j++){
     display.setCursor(10, j*10);
     if(j==0)
      display.setTextColor(color1);
     else if(j == 1)
      display.setTextColor(color2);
     else
      display.setTextColor(color3);
     display.println(MenuItems[MainMenuOptionsDisplayed[j]]);
   }
   display.display();
 }
 else{
  if(x == (MainMenuOptionsDisplayed[2]+1) || (x==0 && MainMenuOptionsDisplayed[2] == 6 ) )
    {
      MainMenuOptionsDisplayed[0] = ++MainMenuOptionsDisplayed[0]%7;
      MainMenuOptionsDisplayed[1] = ++MainMenuOptionsDisplayed[1]%7;
      MainMenuOptionsDisplayed[2] = ++MainMenuOptionsDisplayed[2]%7; 
    }
  else{
    if(MainMenuOptionsDisplayed[0] == 0){
      MainMenuOptionsDisplayed[0] = 6;
      MainMenuOptionsDisplayed[1] = (MainMenuOptionsDisplayed[0]+1)%7;
      MainMenuOptionsDisplayed[2] = (MainMenuOptionsDisplayed[1]+1)%7;
    }
    else{
      MainMenuOptionsDisplayed[0] = --MainMenuOptionsDisplayed[0]%7;
      MainMenuOptionsDisplayed[1] = (MainMenuOptionsDisplayed[0]+1)%7;
      MainMenuOptionsDisplayed[2] = (MainMenuOptionsDisplayed[1]+1)%7;
    } 
   }   
   RefreshMenu(x);
 }
}
