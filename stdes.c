#include <stdio.h>
#include <stdlib.h>


#define ETA 0.5
#define N 50

typedef struct {
  float x0 ;
  float y0 ;
  float x;
  float y;
}point;

point dx(point data){
  data.x = 2 * data.x0 + data.y0 -2;
  return data;
}

point dy(point data){
  data.y = 2 * data.y0 + data.x0;
  return data;
}

point direction(point data){
  data.x = -1 * data.x;
  data.y = -1 * data.y;

  return data;
}

point advance_point(point data){
  data.x0 = data.x0 + ETA * data.x;
  data.y0 = data.y0 + ETA * data.y;

  return data;
}

float get_z(point data){
  return data.x0 * data.x0 + data.y0 * data.y0 + data.x0 * data.y0 - 2 * data.x0;
}

int main(){
  FILE *gp;

  point data;
  data.x0 = -2;
  data.y0 = 0.2;

  int i;

  gp = popen("gnuplot -persist", "w");

  fprintf(gp, "set xrange [-5.0:5.0]\n");
  fprintf(gp, "set yrange [-5.0:5.0]\n");
  fprintf(gp, "splot x*x + y*y + x*y - 2*x\n");


  fprintf(gp, "replot '-', '-', '-' with points\n");
  fprintf(gp, "e\n");
  for (i = 0; i < N; i++) {
    data = dx(data);
    data = dy(data);
    data = direction(data);
    data = advance_point(data);
    fprintf(gp, "%f %f %f\n", data.x0, data.y0, get_z(data));
    //printf("%d: x:%f y:%f\n", i, data.x0, data.y0);
  }
  fprintf(gp, "e\n");

  fprintf(gp, "set nomultiplot\n"); // マルチプロットモード終了
  fprintf(gp, "exit\n"); // gnuplotの終了
  fflush(gp);
  pclose( gp); // パイプを閉じる

}
