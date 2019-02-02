#include<iostream>
#include <cmath>
#include <cstring>
#include <string>
#include "/usr/local/dislin/discpp.h"

void     exa_1   (void);

double xray[300], yray[300], y1ray[300], y2ray[300], zmat[50][50];
std::string cl1, cl2;
int id_lis1, id_lis2, id_pbut;

Dislin g;

int main ()
{ 
  if (g.getlev () != 0){ 
      std::cout << "level must start at 0" << std::endl;
      std::exit(EXIT_FAILURE); 
  }

  g.metafl("XWIN");
  //g.setpag("HP4P");
  g.page(3000, 3000);
  exa_1 ();

  return 0;
}

/* >>>>>>>>>> EXA_1  <<<<<<<<<< */
void exa_1 (void)
{ int n = 100, i;
  double fpi = 3.1415926, step, x;

  step = 2*fpi / (n - 1);

  for (i = 0; i < n; i++)
  { xray[i] = i * step;
    x = xray[i];
    y1ray[i] = sin (x);
    y2ray[i] = cos (x);
  }

  g.disini ();

  //g.pagera ();
  //g.hwfont ();
  g.axspos (500, 2500);
  g.axslen (2000, 2000);

  //g.name   ("X-axis", "x");
  //g.name   ("Y-axis", "y");

  g.labdig (2, "x");
  g.ticks  (10, "xy");

  //g.titlin ("Demonstration of CURVE", 1);
  //g.titlin ("SIN(X), COS(X)", 3);

  g.graf   (0.0, 2*fpi, 0, fpi, -1.0, 1.0, -1.0, 0.5);
  //g.title  ();
  g.incmrk(-1);
  //g.color  ("red");
  g.curve  (xray, y1ray, n);
  //g.color  ("green");
  g.curve  (xray, y2ray, n);
  //g.color  ("fore");
  //g.dash   ();
  //g.xaxgit ();
  g.disfin ();
}

