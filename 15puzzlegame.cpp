#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Radio_Button.H> // needed to use the Fl_Box class
#include <FL/Fl_Light_Button.H> // needed to use the Fl_Box class

#include <iostream>
using namespace std;


   int dir[8] = { -1,0,0,1,0,-1,1,0}; //this array is for getting neighbours
              //first four and corresponding last 4 element gives the left,lower,upper and right neighboue
              //(-1,0)correspond to left,(0,-1) correspond to lower and so on.

  char *intToString (int v)
  {
     char *s = new char[sizeof(int)];
     sprintf(s,"%d",v);
     return s;
  }

  class Board;   //A SUBCLASS USED FOR MAKE BOARDS OF BUTTONS

  class Button : public Fl_Button {
   Board *game;
   int xp;
   int yp;

   public:
     char *l;
     Button(Board *board, int x, int y,int i,int j, int w, int h,char *l);
     int handle(int);
  };


  //CONSTRUCTOR FOR INITIALISING VALUES
  Button::Button(Board *board, int x, int y,int i,int j, int w, int h,char *label):Fl_Button(x,y,w,h,label)
  {
     game = board;
     xp = i;
     yp = j;
     l = label;
  }

  class Board
  {
     Button* buttons[4][4]; Fl_Button *superbutton;
     public:
     Board(int x, int y,int w, int h);
     bool safe(int r,int c);
     bool check(int br,int bc,int &nr,int &nc);
     void update(int br,int bc,char *l,int color);
     void solved();
  };


  //FUNCTION TO HANDLE EACH CLICK POINT EVENT
  int Button::handle(int event)
  {
     if(event==FL_PUSH)
     {
        int nr=4;int nc=4;
        if(game->check(xp,yp,nr,nc) )
        {
           game->update(nr,nc,l,FL_RED);
           game->update(xp,yp, (char *)"",FL_WHITE);
        }
        game->solved();
     }
     return 1;
   }

   Board::Board(int x, int y, int w, int h)
   {
      int button_number = 1;
      for(int i=0;i<4;i++)
      {
         for(int j=0;j<4;j++)
         {
             if(i!=3 || j!=3) 
             {
               buttons[i][j]=new Button(this,x+(j*w),y+(i*h),i,j, w, h,intToString(button_number));
               button_number++;
               buttons[i][j]->labelsize(30);
               buttons[i][j]->color(FL_RED);
               buttons[i][j]->labelcolor(fl_rgb_color(255,215,0));
             }
             else
             {
               buttons[i][j]=new Button(this,x+(j*w),y+(i*h),i,j, w, h,(char*)"");
               buttons[i][j]->labelsize(30);
               buttons[i][j]->color(FL_WHITE);
               buttons[i][j]->labelcolor(fl_rgb_color(255,215,0));
             }
         }
      }

      superbutton=new Fl_Button(200,50,200,100,"SOLVED");
      superbutton->labelsize(30);
      superbutton->color(fl_rgb_color(85,170,255));
      superbutton->labelcolor(fl_rgb_color(255,215,0));
   }

   bool Board::safe(int r,int c)
   {
      return (r>=0&&c>=0&&r<=3&&c<=3);
   }
   bool Board::check(int br,int bc,int& nr,int& nc)
   {
      int x,y;
      for(int i=0;i<4;i++)
      {
         x=br+dir[i];
         y=bc+dir[i+4];
         if(safe(x,y) && (strlen(buttons[x][y]->l) == 0) )
         {
            nr = x;
            nc = y;
            return 1;
         }
      }
      return 0;
   }
   void Board::update(int br,int bc,char *l,int color)
   {
      buttons[br][bc]->label(l);
      buttons[br][bc]->l = l;
      buttons[br][bc]->color(color);
   }

   void Board::solved()
   {
      int p=0,u;const char* t;
      for(int i=0;i<4;i++)
      {
         for(int j=0;j<4;j++)
         {
            if(i!=3 || j!=3)
            {
                t=buttons[i][j]->label();u=atoi(t);
                if(u==4*i+j+1)
                  p++;
            }
         }
      }
      if(p!=15)
             superbutton->label("UNSOLVED");

         else
           superbutton->label("SOLVED");
   }

   int main(int argc, char *argv[])
   {

       Fl_Window *window= new Fl_Window(600,700,"15PUZZLE GAME");
       
       Board board(100,200,100,100);
       window->color(fl_rgb_color(162,60,62));
       FL_SCREEN_BOLD;
       window->end();
       window->show(argc,argv);
       Fl::run();
   }
