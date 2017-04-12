import java.util.*;

public class Prod extends Thread {

  Vector buffer;
  int cap;
  int i;

public Prod (Vector buffer, int cap) {
  this.cap = cap;
  this.buffer = buffer;
  i = 11;
}

public void run() {
  while (true) {
    try {
       if (buffer.size()<=cap-1) {
          System.out.println("Produc "+i);
          buffer.addElement(i);
          i++;
          synchronized (buffer) {
              buffer.notify(); //wakes up the first thread that called wait( ) on the same object.
          }
       }
       synchronized (buffer) {
       buffer.wait(); // tells the calling thread to give up the monitor and go to sleep until some other
       //thread enters the same monitor and calls notify( ).
       }
       sleep(4);
    }
    catch (Exception e) {};
  }
 }
}
