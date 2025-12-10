#include "graphics.h"
#include "winbgim.h"

struct desen {
    int st, sus, dr, jos;
}buton;

desen Buton(char* text, int c, int textsize, int text_color, int bg_color, int V)
{
    settextstyle(GOTHIC_FONT, HORIZ_DIR, textsize);

    int x = 400 - (textwidth(text) / 2);
    int y = c - (textheight(text) / 2);

    buton.st = x - 10;
    buton.sus = y - 10;
    buton.dr = x + textwidth(text) + 10;
    buton.jos = y + textheight(text) + 10;

    setfillstyle(SOLID_FILL, DARKGRAY);
    setcolor(BLACK); //contur
    bar(buton.st, buton.sus, buton.dr, buton.jos);
    rectangle(buton.st, buton.sus, buton.dr, buton.jos); // test

    SetBkMode(0, OPAQUE);
    setbkcolor(DARKGRAY);
    setcolor(BLUE);
    outtextxy(x, y, text);

    return buton;
}

bool inauntru(int mx, int my, int x, int y, int w, int h)
{
    return mx > x && mx < x + w && my > y && my < y + h; // x+w=dreapasus; x=stangasus; y+h=dreaptajos; y=
}

desen butoanejos(char* text, int textsize, int bg_color, int cx, int cy)
{
    settextstyle(GOTHIC_FONT, HORIZ_DIR, textsize);

    int w = textwidth(text);
    int h = textheight(text);

    int left = cx - w / 2 - 10;
    int top = cy - h / 2 - 10;
    int right = cx + w / 2 + 10;
    int bottom = cy + h / 2 + 10;

    buton.st = left;
    buton.sus = top;
    buton.dr = right;
    buton.jos = bottom;

    setfillstyle(SOLID_FILL, bg_color);
    bar(left, top, right, bottom);

    setcolor(BLACK);
    rectangle(left, top, right, bottom);

    setbkcolor(bg_color);
    setcolor(RED);
    outtextxy(cx - w / 2, cy - h / 2, text);

    int mx, my;
    if (ismouseclick(WM_LBUTTONDOWN))
    {
        getmouseclick(WM_LBUTTONDOWN, mx, my);

        if (ecran == 1)
        {
            if (inauntru(mx, my, 300, 150, textwidth("Adauga o muchie"), textheight("Adauga un nod") adauganod();
            if (inauntru(mx, my, 300, 250, textwidth("Adauga un nod"), textheight("Adauga o muchie") adaugamuchie();

        }


    }

    return buton;
}



void MENIU()
{
    setbkcolor(LIGHTRED);
    cleardevice();
    setcolor(BLUE);
    SetBkMode(0, OPAQUE);

    cleardevice();
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 4);

    int x = 400 - (textwidth("ALGORITMICA GRAFURILOR") / 2);
    outtextxy(x, 50, "ALGORITMICA GRAFURILOR");

    Buton("GRAFURI NEORIENTATE", 200, 3, BLACK, DARKGRAY, 800);
    Buton("GRAFURI ORIENTATE", 300, 3, BLACK, DARKGRAY, 800);
    Buton("EXIT", 450, 3, BLACK, DARKGRAY, 800);
}



int main()
{
    initwindow(800, 600, "exemplu");
    setbkcolor(LIGHTRED);
    int ecran = 0;
    char key = ' ';
    int page = 0;
    int next_page = 1;
    bool ok = true;
    while (ok)
    {
        setactivepage(next_page); //pt glitch
        cleardevice();

        int mx, my;
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN, mx, my);

            if (ecran == 0)
            {
                if (inauntru(mx, my, 300, 150, 200, 60)) ecran = 1;
                if (inauntru(mx, my, 300, 250, 200, 60)) ecran = 2;
                if (inauntru(mx, my, 300, 425, 200, 40)) ok = false;
            }

            
        }

        

        if (ecran == 0) MENIU();
        else if (ecran == 1)
        {
            setbkcolor(LIGHTRED);
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
            setcolor(RED);
            outtextxy(240, 10, "Algoritmica Grafurilor");

            setcolor(RED);
            outtextxy(260, 35, "Grafuri neorientate");

            setcolor(RED);
            outtextxy(300, 558, "x pentru exit");

          

            int cx = getmaxx() / 2;
            int cy = getmaxy() / 2 + 200;

            int offset = 150;

            butoanejos("Adauga un nod", 1, LIGHTRED, cx - offset, cy);
            butoanejos("Adauga o muchie", 1, LIGHTRED, cx + offset, cy);


        }
        else if (ecran == 2)
        {
            setbkcolor(LIGHTRED);
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
            setcolor(RED);
            outtextxy(240, 10, "Algoritmica Grafurilor");

            setcolor(RED);
            outtextxy(260, 35, "Grafuri orientate");

            setcolor(RED);
            outtextxy(300, 558, "x pentru exit");


        }

        if (kbhit())
        {
            if (tolower(getch()) == 'x') ok = false;
        }
        setvisualpage(next_page);
        page = next_page;
        next_page = 1 - page;

        if (kbhit() && getch() == 27) break;
        delay(7);
    }
    cleardevice();

    setcolor(WHITE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
    outtextxy(280, 250, "Ai iesit din joc. La revedere!"); //ecran iesire
    delay(1500);

    closegraph();
    return 0;
}
