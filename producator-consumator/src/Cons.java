import java.util.*;

public class Cons extends Thread {

   Vector buffer;

public Cons (Vector buffer) {
	this.buffer = buffer;
}

public void run() {
	while(true) {
		try {
			if(buffer.size()>=1) {
				System.out.println("Consum "+buffer.firstElement());
				buffer.remove(buffer.firstElement());
				System.out.println("buffer.size in Cons="+buffer.size());
				synchronized (buffer) {
					buffer.notify();
				}
			}
			synchronized (buffer) {
				buffer.wait();
			}
			sleep(10);
		}
		catch(Exception e) {};
	}
}
}