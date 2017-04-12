import java.util.Vector;

class ProdCons {


public static void main (String args[]) {

  Vector buffer;
  int nmax;
  nmax = 10;
  buffer = new Vector(nmax);
  for(int i=0; i<=nmax-1; i++)
     buffer.addElement(i);

  Prod prod = new Prod(buffer, nmax);
  Cons cons = new Cons(buffer);

  cons.start();
  prod.start();
}