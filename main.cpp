#include "graphics.h"
#include "winbgim.h"
#include <cmath>


int distanta(int x1, int y1, int x2, int y2)
{
    double dx = double(x1) - double(x2);
    double dy = double(y1) - double(y2);
    return int(std::sqrt(dx * dx + dy * dy) + 0.5);
}

struct desen {
    int st, sus, dr, jos;
};

desen buton1(const char* text, int cy, int textsize, int text_color, int bg_color)
{
    desen b;
    settextstyle(GOTHIC_FONT, HORIZ_DIR, textsize);
    int x = 400 - (textwidth((char*)text) / 2);
    int y = cy - (textheight((char*)text) / 2);
    b.st = x - 10;
    b.sus = y - 10;
    b.dr = x + textwidth((char*)text) + 10;
    b.jos = y + textheight((char*)text) + 10;
    setfillstyle(SOLID_FILL, bg_color);
    setcolor(BLACK);
    bar(b.st, b.sus, b.dr, b.jos);
    rectangle(b.st, b.sus, b.dr, b.jos);
    setcolor(text_color);
    outtextxy(x, y, (char*)text);
    return b;
}

desen butonmic(const char* text, int textsize, int bg_color, int cx, int cy, bool highlight)
{
    desen b;
    settextstyle(GOTHIC_FONT, HORIZ_DIR, textsize);
    int w = textwidth((char*)text);
    int h = textheight((char*)text);
    int left = cx - w / 2 - 10;
    int top = cy - h / 2 - 10;
    int right = cx + w / 2 + 10;
    int bottom = cy + h / 2 + 10;
    b.st = left;
    b.sus = top;
    b.dr = right;
    b.jos = bottom;
    if (highlight) setfillstyle(SOLID_FILL, DARKGRAY);
    else setfillstyle(SOLID_FILL, bg_color);
    bar(left, top, right, bottom);
    setcolor(BLACK);
    rectangle(left, top, right, bottom);
    if (highlight) setcolor(BLACK);
    else setcolor(RED);
    outtextxy(cx - w / 2, cy - h / 2, (char*)text);
    return b;
}

void MENIU()
{
    setbkcolor(LIGHTRED);
    cleardevice();
    setcolor(BLUE);
    SetBkMode(0, OPAQUE);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 4);
    int x = 400 - (textwidth("ALGORITMICA GRAFURILOR") / 2);
    outtextxy(x, 50, "ALGORITMICA GRAFURILOR");
    buton1("GRAFURI NEORIENTATE", 200, 3, BLUE, DARKGRAY);
    buton1("GRAFURI ORIENTATE", 300, 3, BLUE, DARKGRAY);
    buton1("EXIT", 450, 3, BLUE, DARKGRAY);
}

bool inauntru(int mx, int my, int x, int y, int w, int h)
{
    return mx >= x && mx <= x + w && my >= y && my <= y + h;
}

int main()
{
    initwindow(800, 600, "exemplu");
    setbkcolor(LIGHTRED);
    int ecran = 0;
    int page = 0;
    int next_page = 1;
    bool ok = true;

    const int MAX = 100;
    int X[MAX] = { 0 }, Y[MAX] = { 0 }, nrnoduri = 0;
    int EA[MAX * 2] = { 0 }, EB[MAX * 2] = { 0 }, nrmuchii = 0;

    int highlighted = 0;
    int mod = 0;
    int nod = -1;

    while (ok)
    {
        setactivepage(next_page);
        cleardevice();

        int mx = -1, my = -1;
        bool clicked = false;
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN, mx, my);
            clicked = true;
        }

        if (ecran == 0)
        {
            if (clicked)
            {
                if (inauntru(mx, my, 300, 150, 200, 60)) ecran = 1;
                else if (inauntru(mx, my, 300, 250, 200, 60)) ecran = 2;
                else if (inauntru(mx, my, 300, 425, 200, 40)) ok = false;
            }
            MENIU();
            mod = 0;
            nod = -1;
            highlighted = 0;
        }
        else if (ecran == 1)
        {
            setbkcolor(LIGHTRED);
            cleardevice();
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
            setcolor(RED);
            outtextxy(240, 10, "Algoritmica Grafurilor");
            outtextxy(260, 35, "Grafuri neorientate");
            outtextxy(300, 558, "x pentru exit");

            int cx = getmaxx() / 2;
            int cy = getmaxy() / 2 + 200;
            int offset = 150;

            desen b1 = butonmic("Adauga un nod", 1, LIGHTRED, cx - offset, cy, highlighted == 1);
            desen b2 = butonmic("Adauga o muchie", 1, LIGHTRED, cx + offset, cy, highlighted == 2);

            for (int i = 0; i < nrmuchii; i++)
            {
                int a = EA[i], b = EB[i];
                if (a >= 0 && a < nrnoduri && b >= 0 && b < nrnoduri)
                    line(X[a], Y[a], X[b], Y[b]);
            }

            for (int i = 0; i < nrnoduri; i++)
            {
                setcolor(BLACK);
                circle(X[i], Y[i], 10);
            }

            if (nod != -1 && nod < nrnoduri)
            {
                setcolor(RED);
                circle(X[nod], Y[nod], 14);
            }

            if (clicked)
            {
                if (inauntru(mx, my, b1.st, b1.sus, b1.dr - b1.st, b1.jos - b1.sus))
                {
                    mod = 1;
                    highlighted = 1;
                    nod = -1;
                }
                else if (inauntru(mx, my, b2.st, b2.sus, b2.dr - b2.st, b2.jos - b2.sus))
                {
                    mod = 2;
                    highlighted = 2;
                    nod = -1;
                }
                else
                {
                    if (mod == 1)
                    {
                        if (nrnoduri < MAX)
                        {
                            X[nrnoduri] = mx;
                            Y[nrnoduri] = my;
                            nrnoduri++;
                        }
                        mod = 0;
                        highlighted = 0;
                    }
                    else if (mod == 2)
                    {
                        int gasite = -1;
                        for (int i = 0; i < nrnoduri; i++)
                        {
                            if (distanta(mx, my, X[i], Y[i]) <= 12)
                            {
                                gasite = i;
                                break;
                            }
                        }
                        if (gasite != -1)
                        {
                            if (nod == -1) nod = gasite;
                            else
                            {
                                if (nrmuchii < MAX * 2)
                                {
                                    EA[nrmuchii] = nod;
                                    EB[nrmuchii] = gasite;
                                    nrmuchii++;
                                }
                                nod = -1;
                                mod = 0;
                                highlighted = 0;
                            }
                        }
                    }
                    else
                    {
                        highlighted = 0;
                    }
                }
            }
        }
        else if (ecran == 2)
        {
            setbkcolor(LIGHTRED);
            cleardevice();
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
            setcolor(RED);
            outtextxy(240, 10, "Algoritmica Grafurilor");
            outtextxy(260, 35, "Grafuri orientate");
            outtextxy(300, 558, "x pentru exit");

            int cx = getmaxx() / 2;
            int cy = getmaxy() / 2 + 200;
            int offset = 150;

            butonmic("Adauga un nod", 1, LIGHTRED, cx - offset, cy, false);
            butonmic("Adauga o muchie", 1, LIGHTRED, cx + offset, cy, false);
        }

        if (kbhit())
        {
            if (tolower(getch()) == 'x') ok = false;
        }

        setvisualpage(next_page);
        page = next_page;
        next_page = 1 - page;

        if (kbhit() && getch() == 27) break;
        delay(15);
    }

    cleardevice();
    setcolor(WHITE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
    outtextxy(280, 250, "Ai iesit din joc. La revedere!");
    delay(1500);

    closegraph();
    return 0;
}
