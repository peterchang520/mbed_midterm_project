#include "mbed.h"
#include "uLCD_4DGL.h"
#include "DNN.h"


//mode
#define FORWARD 0
#define BACKWARD 1
#define CHANGE_SONGS 2
//state
#define MODE_SELECTION 3
#define PLAYING_SONG 4
#define SONG_SELECTION 5
//song
#define BEE 6
#define STAR 7
#define COFFIN 8

uLCD_4DGL uLCD(D1, D0, D2); // serial tx, serial rx, reset pin;
InterruptIn sw2(SW2);
InterruptIn sw3(SW3);
DigitalOut greenLED(LED2);
EventQueue queue1;
EventQueue queue2;
Thread thread1;
Thread thread2;


void confirm_selection(int* mode, int song, int* state){
  greenLED = 1;
  if(*state==MODE_SELECTION){
    switch(*mode){
      case FORWARD:
          *state = PLAYING_SONG;
          break;
      case BACKWARD:
          *state = PLAYING_SONG;
          break;
      case CHANGE_SONGS:
          *state = SONG_SELECTION;
          break;
    }
  }
  else if(*state==SONG_SELECTION){
    switch(song){
      case BEE:
          *state = PLAYING_SONG;
          break;
      case STAR:
          *state = PLAYING_SONG;
          break;
      case COFFIN:
          *state = PLAYING_SONG;
          break;
    }
  }
}
void mode_selection(int argc, char* argv[], int* mode, int* state){
  greenLED = 0;
  *state = MODE_SELECTION;
  if(*mode==CHANGE_SONGS)
    *mode = FORWARD;
  else
    *mode = *mode + 1;
}

int main(int argc, char* argv[]){

  int mode = FORWARD;
  int state = MODE_SELECTION;
  int song = BEE;
  int pre_state;

  thread1.start(callback(&queue1, &EventQueue::dispatch_forever));
  thread2.start(callback(&queue2, &EventQueue::dispatch_forever));

  sw2.rise(queue1.event(mode_selection, argc, argv, &mode, &state));
  sw3.rise(queue2.event(confirm_selection, &mode, song, &state));

  greenLED = 1;

  while(true){
      if(state!=pre_state)
        uLCD.cls();
      pre_state = state;
      if(state==MODE_SELECTION){
        uLCD.color(BLUE);
        uLCD.set_font(FONT_7X8);
        uLCD.locate(0, 1);
        uLCD.printf("Mode Selection\n");
        uLCD.locate(3, 3);
        uLCD.printf("0: forward\n");
        uLCD.locate(3, 5);
        uLCD.printf("1: backward\n");
        uLCD.locate(3, 7);
        uLCD.printf("2: change_songs\n");
        uLCD.locate(3, 9);
        switch (mode){
          case FORWARD:
            uLCD.printf("Enter mode: 0\n");
            break;
          case BACKWARD:
            uLCD.printf("Enter mode: 1\n");
            break;
          case CHANGE_SONGS:
            uLCD.printf("Enter mode: 2\n");
            break;
          default:
            uLCD.printf("Please select mode\n");
            break;
        }
      }
      else if(state==PLAYING_SONG){
        uLCD.color(GREEN);
        uLCD.set_font(FONT_7X8);
        uLCD.locate(3, 5);
        uLCD.printf("Playing Song\n");
        uLCD.locate(0, 9);
        uLCD.printf("Push SW2 to enter\n");
        uLCD.locate(2, 10);
        uLCD.printf("selection mode \n");
      }
      else if(state==SONG_SELECTION){
        uLCD.color(RED);
        uLCD.set_font(FONT_7X8);
        uLCD.locate(0, 1);
        uLCD.printf("Song Selection\n");
        uLCD.locate(3, 3);
        uLCD.printf("0: Bee\n");
        uLCD.locate(3, 5);
        uLCD.printf("1: Star\n");
        uLCD.locate(3, 7);
        uLCD.printf("2: Coffin\n");
        uLCD.locate(3, 9);
        switch (song){
          case BEE:
            uLCD.printf("Play song: 0\n");
            break;
          case STAR:
            uLCD.printf("Play song: 1\n");
            break;
          case COFFIN:
            uLCD.printf("Play song: 2\n");
            break;
          default:
            uLCD.printf("Please select song\n");
            break;
        }
      }
  }
}