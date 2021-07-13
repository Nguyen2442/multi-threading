/*
 * BLOCKING QUEUE
 * Version B: Fast producer, slow consumer
 */

package demo22;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;



public class ProgramB {

    public static void main(String[] args) {
        BlockingQueue<String> queue;


        // queue = new LinkedBlockingQueue<>();
        queue = new ArrayBlockingQueue<>(2); // blocking queue with capacity = 2


        new Thread(() -> producer(queue)).start();
        new Thread(() -> consumer(queue)).start();
    }


    private static void producer(BlockingQueue<String> queue) {
        try {
            queue.put("lorem");
            queue.put("ipsum");

            /*
             * Due to reaching the maximum of capacity = 2, when executing queue.put("fooooooo"),
             * this thread is going to sleep until the queue removes an element.
             */

            queue.put("fooooooo");
        }
        catch (InterruptedException e) {
            e.printStackTrace();
        }
    }


    private static void consumer(BlockingQueue<String> queue) {
        String data = "";

        try {
            Thread.sleep(2000);

            for (int i = 0; i < 3; ++i) {
                System.out.println("\nWaiting for data...");

                data = queue.take();

                System.out.println("    " + data);
            }
        }
        catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

}