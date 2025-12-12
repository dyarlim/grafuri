#include "graphics.h"
#include "winbgim.h"
#include <iostream>
#include <cmath>
#define M_PI 3.141592653897932
using namespace std;

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

void sageata(int x1, int y1, int x2, int y2)
{
    line(x1, y1, x2, y2);
    double angle = atan2(y2 - y1, x2 - x1);
    int lung = 30;
    double unghi = M_PI / 6;
    int x3 = x2 - lung * cos(angle - unghi);
    int y3 = y2 - lung * sin(angle - unghi);
    int x4 = x2 - lung * cos(angle + unghi);
    int y4 = y2 - lung * sin(angle + unghi);

    line(x2, y2, x3, y3);
    line(x2, y2, x4, y4);
}

int ecran = 0;
int page = 0;
int next_page = 1;
bool ok = true;

const int MAX = 100;
int X[MAX] = { 0 }, Y[MAX] = { 0 }, nrnoduri = 0;
int EA[MAX * 2] = { 0 }, EB[MAX * 2] = { 0 }, nrmuchii = 0, Costuri[MAX * 2] = { 0 };
bool afiscost[MAX * 2] = { 0 };

int highlighted = 0;
int mod = 0;          // 0 = normal, 1 = add node, 2 = add edge, 3 = set cost
int orientat = 0;     // 0 = neorientat, 1 = orientat

// stare clara pentru adaugare muchie
// edge_stage: 0 = idle, 1 = astept primul nod, 2 = astept al doilea nod
int edge_stage = 0;
int edge_from = -1;

void cost(int x1, int y1, int x2, int y2, int nr)
{
    char numar[10];
    sprintf(numar, "%d", nr);
    setbkcolor(LIGHTGRAY);
    setcolor(RED);
    outtextxy((x1 + x2) / 2 - textwidth(numar) / 2, (y1 + y2) / 2 - textheight(numar) / 2, numar);
}

void nodurimuchiin()
{
    // 1. Desenez muchiile (neorientate)
    for (int i = 0; i < nrmuchii; i++)
    {
        int a = EA[i], b = EB[i];
        if (a >= 0 && a < nrnoduri && b >= 0 && b < nrnoduri)
            line(X[a], Y[a], X[b], Y[b]);
        if (afiscost[i])
            cost(X[a], Y[a], X[b], Y[b], Costuri[i]);
    }

    // 2. Desenez nodurile
    for (int i = 0; i < nrnoduri; i++)
    {
        setcolor(BLACK);
        circle(X[i], Y[i], 20);
        char nr[20];
        sprintf(nr, "%d", i + 1);
        setbkcolor(LIGHTRED);
        outtextxy(X[i] - textwidth(nr) / 2, Y[i] - textheight(nr) / 2, nr);
    }

    if (edge_stage == 1 && edge_from >= 0 && edge_from < nrnoduri)
    {
        setcolor(RED);
        circle(X[edge_from], Y[edge_from], 25);
    }
}

void nodurimuchiio()
{
    // 1. Desenez muchiile orientate + costurile
    for (int i = 0; i < nrmuchii; i++)
    {
        int a = EA[i];
        int b = EB[i];
        if (a >= 0 && a < nrnoduri && b >= 0 && b < nrnoduri)
        {
            sageata(X[a], Y[a], X[b], Y[b]);
            if (afiscost[i])
                cost(X[a], Y[a], X[b], Y[b], Costuri[i]);
        }
    }

    // 2. Desenez nodurile
    for (int i = 0; i < nrnoduri; i++)
    {
        setcolor(BLACK);
        circle(X[i], Y[i], 20);
        char nr[20];
        sprintf(nr, "%d", i + 1);
        setbkcolor(LIGHTRED);
        outtextxy(X[i] - textwidth(nr) / 2, Y[i] - textheight(nr) / 2, nr);
    }

    // 3. 
    if (edge_stage == 1 && edge_from >= 0 && edge_from < nrnoduri)
    {
        setcolor(RED);
        circle(X[edge_from], Y[edge_from], 25);
    }
}

string scrietext(int x, int y, int max = 5)
{
    char v[100]; int lun = 0; v[0] = '\0';
    setbkcolor(LIGHTGRAY);
    setcolor(BLACK);
    bar(x - 5, y - 5, x + 150, y + 25);
    rectangle(x - 5, y - 5, x + 150, y + 25);
    while (true)
    {
        if (kbhit())
        {
            char c = getch();
            if (c == 13 && lun > 0) // 13 e enter
                return string(v);
            if (c == 8 && lun > 0) // 8 = backspace
            {
                lun--;
                v[lun] = '\0';
            }
            else if (c >= '0' && c <= '9' && lun < max)
            {
                v[lun] = c; lun++; v[lun] = '\0';
            }
            setfillstyle(SOLID_FILL, LIGHTGRAY);
            bar(x, y, x + 140, y + 20);
            setcolor(BLACK);
            outtextxy(x, y, v);
        }
    }
}

int main()
{
    initwindow(800, 600, "joc");
    setbkcolor(LIGHTGREEN);

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
        }
        else if (ecran == 1)
        {
            setbkcolor(LIGHTGRAY);
            cleardevice();
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
            setcolor(RED);
            outtextxy(240, 10, "Algoritmica Grafurilor");
            outtextxy(260, 35, "Grafuri neorientate");
            outtextxy(300, 558, "x pentru exit");

            int cx = getmaxx() / 2;
            int cy = getmaxy() / 2;
            int offset = 150;

            desen b1 = butonmic("Adauga un nod", 1, LIGHTGRAY, cx - offset, cy + 150, highlighted == 1);
            desen b2 = butonmic("Adauga o muchie", 1, LIGHTGRAY, cx + offset, cy + 150, highlighted == 2);
            desen b3 = butonmic("Adauga un cost", 1, LIGHTGRAY, cx - offset, cy + 215, highlighted == 3);
            desen b4 = butonmic("Ecranul anterior", 1, LIGHTGRAY, cx + offset, cy + 215, highlighted == 4);

            nodurimuchiin();

            if (clicked)
            {
                if (inauntru(mx, my, b1.st, b1.sus, b1.dr - b1.st, b1.jos - b1.sus))
                {
                    mod = 1;
                    highlighted = 1;
                    // pentru adaugare nod, reset stare muchie
                    edge_stage = 0;
                    edge_from = -1;
                    orientat = 0;
                }
                else if (inauntru(mx, my, b4.st, b4.sus, b4.dr - b4.st, b4.jos - b4.sus))
                {
                    highlighted = 2;
                    // reset stare muchie
                    edge_stage = 0;
                    edge_from = -1;
                    ecran = 0;
                }
                else if (inauntru(mx, my, b3.st, b3.sus, b3.dr - b3.st, b3.jos - b3.sus))
                {
                    mod = 3;
                    highlighted = 3;
                    // reset stare muchie
                    edge_stage = 0;
                    edge_from = -1;
                }
                else if (inauntru(mx, my, b2.st, b2.sus, b2.dr - b2.st, b2.jos - b2.sus))
                {
                    // pornim procedura clar pentru adaugare muchie (neorientat)
                    mod = 2;
                    highlighted = 2;
                    edge_stage = 1;
                    edge_from = -1;
                    orientat = 0;
                }
                else
                {
                    if (mod == 1) // adauga nod
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
                    else if (mod == 2) // adauga muchie (neorientat) fara cost
                    {
                        // caut nodul pe care ai dat click (raza detectie = 30)
                        int gasite = -1;
                        for (int i = 0; i < nrnoduri; i++)
                        {
                            if (distanta(mx, my, X[i], Y[i]) <= 30)
                            {
                                gasite = i;
                                break;
                            }
                        }

                        if (gasite != -1)
                        {
                            if (edge_stage == 1) // primul nod
                            {
                                edge_from = gasite;
                                edge_stage = 2; // astept al doilea nod
                            }
                            else if (edge_stage == 2) // al doilea nod
                            {
                                if (edge_from != -1 && nrmuchii < MAX * 2)
                                {
                                    EA[nrmuchii] = edge_from;
                                    EB[nrmuchii] = gasite;
                                    Costuri[nrmuchii] = 0;
                                    afiscost[nrmuchii] = false;
                                    nrmuchii++;
                                }
                                // reset stare
                                mod = 0;
                                highlighted = 0;
                                edge_stage = 0;
                                edge_from = -1;
                            }
                        }
                       
                    }
                    else if (mod == 3) // seteaza cost muchie
                    {
                        bool muchiegasita = false;

                        for (int i = 0; i < nrmuchii; i++)
                        {
                            int a = EA[i], b = EB[i];  // muchia i
                            if (!(a >= 0 && a < nrnoduri && b >= 0 && b < nrnoduri)) continue;

                            double dx = X[b] - X[a];
                            double dy = Y[b] - Y[a];

                            double t = ((mx - X[a]) * dx + (my - Y[a]) * dy);
                            double denom = (dx * dx + dy * dy);
                            if (denom == 0) continue;
                            t = t / denom;
                            if (t < 0) t = 0;
                            if (t > 1) t = 1;

                            double px = X[a] + t * dx;
                            double py = Y[a] + t * dy;

                            double dist = sqrt((mx - px) * (mx - px) + (my - py) * (my - py)); // dist mouse-muchie
                            if (dist <= 10) // distanta mica-> click pe muchie
                            {
                                string s = scrietext(50, 550);
                                if (s == "") s = "0";
                                Costuri[i] = stoi(s);
                                afiscost[i] = true;

                                mod = 0;
                                highlighted = 0;

                                muchiegasita = true;
                                break;
                            }
                        }

                        if (!muchiegasita)
                        {
                            // daca nu s-a gasit muchia, revenim la modul normal
                            mod = 0;
                            highlighted = 0;
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
            setbkcolor(LIGHTGREEN);
            cleardevice();
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
            setcolor(RED);
            outtextxy(240, 10, "Algoritmica Grafurilor");
            outtextxy(260, 35, "Grafuri orientate");
            outtextxy(300, 558, "x pentru exit");

            int cx = getmaxx() / 2;
            int cy = getmaxy() / 2;
            int offset = 150;

            desen b1 = butonmic("Adauga un nod", 1, LIGHTGRAY, cx - offset, cy + 150, highlighted == 1);
            desen b2 = butonmic("Adauga o muchie", 1, LIGHTGRAY, cx + offset, cy + 150, highlighted == 2);
            desen b3 = butonmic("Adauga un cost", 1, LIGHTGRAY, cx - offset, cy + 215, highlighted == 3);
            desen b4 = butonmic("Ecranul anterior", 1, LIGHTGRAY, cx + offset, cy + 215, highlighted == 4);

            nodurimuchiio();

            if (clicked)
            {
                if (inauntru(mx, my, b1.st, b1.sus, b1.dr - b1.st, b1.jos - b1.sus))
                {
                    mod = 1;
                    highlighted = 1;
                    edge_stage = 0;
                    edge_from = -1;
                    orientat = 1;
                }
                else if (inauntru(mx, my, b2.st, b2.sus, b2.dr - b2.st, b2.jos - b2.sus))
                {
                    mod = 2;
                    highlighted = 2;
                    edge_stage = 1; // pornim selectarea muchiei
                    edge_from = -1;
                    orientat = 1;
                }
                else if (inauntru(mx, my, b3.st, b3.sus, b3.dr - b3.st, b3.jos - b3.sus))
                {
                    mod = 3;
                    highlighted = 3;
                    edge_stage = 0;
                    edge_from = -1;
                }
                else if (inauntru(mx, my, b4.st, b4.sus, b4.dr - b4.st, b4.jos - b4.sus))
                {
                    highlighted = 2;
                    edge_stage = 0;
                    edge_from = -1;
                    ecran = 0;
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
                    else if (mod == 2) // adauga muchie orientata fara cost
                    {
                        // caut nodul pe care ai dat click (raza detectie = 30)
                        int gasit = -1;
                        for (int i = 0; i < nrnoduri; i++)
                        {
                            if (distanta(mx, my, X[i], Y[i]) <= 30)
                            {
                                gasit = i;
                                break;
                            }
                        }

                        if (gasit != -1)
                        {
                            if (edge_stage == 1) // selectez primul nod
                            {
                                edge_from = gasit;
                                edge_stage = 2; // next: selectez al doilea nod
                            }
                            else if (edge_stage == 2) // selectez al doilea nod
                            {
                                int edge_to = gasit;
                                if (edge_from != -1 && nrmuchii < MAX * 2)
                                {
                                    EA[nrmuchii] = edge_from;
                                    EB[nrmuchii] = edge_to;
                                    Costuri[nrmuchii] = 0;
                                    afiscost[nrmuchii] = false;
                                    nrmuchii++;
                                }

                                // reset stare
                                mod = 0;
                                edge_stage = 0;
                                edge_from = -1;
                                highlighted = 0;

                                cleardevice();
                                if (orientat == 0)
                                    nodurimuchiin();
                                else
                                    nodurimuchiio();
                            }
                        }
                        // NU resetam mod daca clickul nu a fost pe nod; permit utilizatorului sa incerce din nou
                    }
                    else if (mod == 3) // seteaza cost muchie
                    {
                        bool muchiegasita = false;

                        for (int i = 0; i < nrmuchii; i++)
                        {
                            int a = EA[i], b = EB[i];  // muchia i
                            if (!(a >= 0 && a < nrnoduri && b >= 0 && b < nrnoduri)) continue;

                            double dx = X[b] - X[a];
                            double dy = Y[b] - Y[a];

                            double t = ((mx - X[a]) * dx + (my - Y[a]) * dy);
                            double denom = (dx * dx + dy * dy);
                            if (denom == 0) continue;
                            t = t / denom;
                            if (t < 0) t = 0;
                            if (t > 1) t = 1;

                            double px = X[a] + t * dx;
                            double py = Y[a] + t * dy;

                            double dist = sqrt((mx - px) * (mx - px) + (my - py) * (my - py)); // dist mouse-muchie
                            if (dist <= 10) // distanta mica-> click pe muchie
                            {
                                string s = scrietext(50, 550);
                                if (s == "") s = "0";
                                Costuri[i] = stoi(s);
                                afiscost[i] = true;

                                mod = 0;
                                highlighted = 0;

                                muchiegasita = true;
                                break;
                            }
                        }

                        if (!muchiegasita)
                        {
                            mod = 0;
                            highlighted = 0;
                        }
                    }
                    else
                    {
                        highlighted = 0;
                    }
                }
            }
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
