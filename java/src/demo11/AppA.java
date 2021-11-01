/*
 * THREAD POOL
 * Version A: Thread pool containing a single thread
 *
 * Note: The single thread executor is ideal for creating an event loop.
*/

package demo11;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;



public class AppA {

    public static void main(String[] args) {
        ExecutorService executor = Executors.newSingleThreadExecutor();

        executor.submit(() -> System.out.println("Hello World"));
        executor.submit(() -> System.out.println("Hello Multithreading"));

        executor.shutdown();
    }

}