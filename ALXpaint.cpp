#include <X11/Xlib.h>   //   /$$$$$$  /$$       /$$   /$$
#include <X11/keysym.h> //  /$$__  $$| $$      | $$  / $$
#include <X11/Xutil.h>  // | $$  \ $$| $$      |  $$/ $$/
#include <unistd.h>     // | $$$$$$$$| $$       \  $$$$/ 
#include <iostream>     // | $$__  $$| $$        >$$  $$ 
#include <stdio.h>      // | $$  | $$| $$       /$$/\  $$
#include <stdlib.h>     // | $$  | $$| $$$$$$$$| $$  \ $$
#include <string.h>     // |__/  |__/|________/|__/  |__/                             
// Make sure to include -lX11 when compiling           

// Varabels:
Display *display;
Window window;
XEvent event;

//Declare functions:
void drawALX(Window window, Display *display, bool alx, int alxX, int alxY, int hexColor);
void printTo(Window window, int x, int y, const char* message);
void help(Window window, Display *display, bool help);
void setColor(Display *display, int color);
void clearWindow();

int main(){
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    // set the background color to white and the border color to black
    unsigned long white = WhitePixel(display, DefaultScreen(display));
    unsigned long black = BlackPixel(display, DefaultScreen(display));

    // create the window with the specified colors
    window = XCreateSimpleWindow(display, RootWindow(display, 0), 0, 0, 500, 500, 1, 0x000222, 0x000222);
    GC gc = XCreateGC(display, window, 0, nullptr);

    // The set up code:
    XSelectInput(display, window,  ExposureMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask);
    XSetWindowBackground(display, window, 0x000222);
    XStoreName(display, window, "ALX paint");   // This is where and how you can change/edit the name of the window
    XMapWindow(display, window);


    // Local variables:
        bool draw = false, helpV = true, alx = true;
        int rectX = 0, rectY = 0, radius = 10;
        int cursorColor = 0xffffff;

    // The program:

    while(true){    // On window startup
        XNextEvent(display, &event);
        if (event.type == Expose) {
            help(window, display, true);
            while(1){ XNextEvent(display, &event); 
            if(event.type == KeyPress){break;}
            if(event.type == ButtonPress){break;}
            }
            help(window, display, false);
            drawALX(window, display, alx, 10, 10, 0xDBA507);
            break;
        }
    }

        XEvent event;
            while(true){
                XNextEvent(display, &event);

                if (event.type == KeyPress){    // If you click a key on the keyboard

                    XKeyEvent key_event = event.xkey;
                    KeyCode pressedKey = key_event.keycode;
                
                         if (pressedKey == XKeysymToKeycode(display, XK_1)){ cursorColor = 0xff0000; }  // Sets the cursor to red
                    else if (pressedKey == XKeysymToKeycode(display, XK_2)){ cursorColor = 0x00ff00; }  // Sets the cursor to green
                    else if (pressedKey == XKeysymToKeycode(display, XK_3)){ cursorColor = 0x0000ff; }  // Sets the cursor to blue
                    else if (pressedKey == XKeysymToKeycode(display, XK_0)){ cursorColor = 0xffffff; }  // Sets the cursor to white
                    else if (pressedKey == XKeysymToKeycode(display, XK_c)) { clearWindow(); drawALX(window, display, alx, 10, 10, 0xDBA507);}
                    else if (pressedKey == XKeysymToKeycode(display, XK_BackSpace)) { if(alx){drawALX(window, display, alx, 10, 10, 0x000222);} alx = !alx; }   // Togles alx
                    else if (pressedKey == XKeysymToKeycode(display, XK_underscore)) { radius--; }
                    else if (pressedKey == XKeysymToKeycode(display, XK_Escape)) { break; }     // Exits the program
                    else if (pressedKey == XKeysymToKeycode(display, XK_equal)) { radius++; }
                    else if (pressedKey == XKeysymToKeycode(display, XK_d)) { draw = !draw; }   // Togles draw
                    else if (pressedKey == XKeysymToKeycode(display, XK_h)){ helpV = !helpV;}

                    drawALX(window, display, alx, 10, 10, 0xDBA507);
                    
                }
                else if(event.type == ButtonPress) {    // If you click the mouse
                    // check which button was pressed
                    if (event.xbutton.button == Button1) {  // Left click
                        XClearArea(display, window, rectX , rectY , radius +5, radius +5, true);
                        drawALX(window, display, alx, 10, 10, 0xDBA507);
                        draw = !draw;
                    }
                    else if (event.xbutton.button == Button3) {} // Right click

                    //Run afterwards:
                    drawALX(window, display, alx, 10, 10, 0xDBA507);
                    
                }
                else if (event.type == MotionNotify) {  // If the mouse moves
                    if(!draw){ 
                        XClearArea(display, window, rectX , rectY , radius +5, radius +5, true);
                        rectX = event.xmotion.x - radius/2; // center the square around the cursor
                        rectY = event.xmotion.y - radius/2;
                        setColor(display, 0xff0000);
                        XFillRectangle(display, window, DefaultGC(display, 0), rectX, rectY, radius, radius);
                        setColor(display, 0x000222);
                        XFillRectangle(display, window, DefaultGC(display, 0), rectX +1, rectY +1, radius -2, radius -2);  
                    }
                    else{
                        rectX = event.xmotion.x - radius/2; // center the circle around the cursor
                        rectY = event.xmotion.y - radius/2;
                        rectY * 0.5, rectX * 0.5;
                        XSetForeground(display, gc, cursorColor);
                        XFillArc(display, window, gc, rectX, rectY, radius, radius, 0, 360*64);
                    }
                        // Run when mouse is moved:
                        drawALX(window, display, alx, 10, 10, 0xDBA507);

                    }
                    else if (event.type == DestroyNotify) { // If you try to manualy close the display
                        break;
                    }
                XFlush(display);
                }
    
    // Ends the program:
    XCloseDisplay(display);
    return 0;
}

//Functions:

void printTo(Window window, int x, int y, const char* message) {
  XDrawString(display, window, DefaultGC(display, 0), x, y, message, strlen(message));
}

void setColor(Display *display, int color) {
    XSetForeground(display, DefaultGC(display, 0), color);
}

void clearWindow(){
    XWindowAttributes attributes;
    XGetWindowAttributes(display, window, &attributes);
    XClearArea(display, window, 0, 0, attributes.width, attributes.height, true);
}

void drawALX(Window window, Display *display, bool alx, int alxX, int alxY, int hexColor){
    if(alx){
        setColor(display, hexColor);
        printTo(window, alxX, alxY +10, "   /$$$$$$  /$$       /$$   /$$  ");
        printTo(window, alxX, alxY +20, "  /$$__  $$| $$      | $$  / $$  ");
        printTo(window, alxX, alxY +30, " | $$  \\ $$| $$      |  $$/ $$/ ");
        printTo(window, alxX, alxY +40, " | $$$$$$$$| $$       \\  $$$$/  ");
        printTo(window, alxX, alxY +50, " | $$__  $$| $$        >$$  $$   ");
        printTo(window, alxX, alxY +60, " | $$  | $$| $$       /$$/\\  $$ ");
        printTo(window, alxX, alxY +70, " | $$  | $$| $$$$$$$$| $$  \\ $$ ");
        printTo(window, alxX, alxY +80, " |__/  |__/|________/|__/  |__/  ");
        XFlush(display);
    }
    else if(!alx){
        setColor(display, 0x000222);
        printTo(window, alxX, alxY +10, "   /$$$$$$  /$$       /$$   /$$  ");
        printTo(window, alxX, alxY +20, "  /$$__  $$| $$      | $$  / $$  ");
        printTo(window, alxX, alxY +30, " | $$  \\ $$| $$      |  $$/ $$/ ");
        printTo(window, alxX, alxY +40, " | $$$$$$$$| $$       \\  $$$$/  ");
        printTo(window, alxX, alxY +50, " | $$__  $$| $$        >$$  $$   ");
        printTo(window, alxX, alxY +60, " | $$  | $$| $$       /$$/\\  $$ ");
        printTo(window, alxX, alxY +70, " | $$  | $$| $$$$$$$$| $$  \\ $$ ");
        printTo(window, alxX, alxY +80, " |__/  |__/|________/|__/  |__/  ");
        XFlush(display);
    }
}

void help(Window window, Display *display, bool check){
//local variables:
int x = 10, y = 20;
    if(check){
        setColor(display, 0xFFFFFF);
        printTo(window, x, y -10, " *READ THIS*");
        printTo(window, x, y  +1, "D: switches from drawing to erasing and vise versa");
        printTo(window, x, y +12, "Deft click: togles from drawing and erasing");
        printTo(window, x, y +24, "+/- : changes the size of cursor");
        printTo(window, x, y +36, "C: clears the WHOLE display");
        printTo(window, x, y +48, "Numbers: change the color");
        printTo(window, x, y +60, "Esc: exits the program");
        printTo(window, x, y +72, "Backspace: togles ALX");
        printTo(window, x, y +96, "");   
    }
    else if(!check){
        setColor(display, 0x000222);
        printTo(window, x, y -10, " *READ THIS*");
        printTo(window, x, y  +1, "D: switches from drawing to erasing and vise versa");
        printTo(window, x, y +12, "Deft click: togles from drawing and erasing");
        printTo(window, x, y +24, "+/- : changes the size of cursor");
        printTo(window, x, y +36, "C: clears the WHOLE display");
        printTo(window, x, y +48, "Numbers: change the color");
        printTo(window, x, y +60, "Esc: exits the program");
        printTo(window, x, y +72, "Backspace: togles ALX");
        printTo(window, x, y +96, ""); 
    }
}
