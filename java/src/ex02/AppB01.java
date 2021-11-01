/*
 * THE PRODUCER-CONSUMER PROBLEM
 *
 * SOLUTION TYPE B: USING SEMAPHORE
 *      Version B01: 1 slow producer, 1 fast consumer
*/

package ex02;

import java.util.LinkedList;
import java.util.Queue;
import java.util.concurrent.Semaphore;



public class AppB01 {

    public static void main(String[] args) {
        var semFill = new Semaphore(0);     // item produced
        var semEmpty = new Semaphore(1);    // remaining space in queue

        Queue<Integer> qProduct = new LinkedList<>();

        new Thread(() -> funcProducer(semFill, semEmpty, qProduct)).start();

        new Thread(() -> funcConsumer(semFill, semEmpty, qProduct)).start();
    }


    private static void funcProducer(Semaphore semFill, Semaphore semEmpty, Queue<Integer> qProduct) {
        int i = 1;

        for (;; ++i) {
            try {
                semEmpty.acquire();

                qProduct.add(i);
                Thread.sleep(1000);

                semFill.release();
            }
            catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }


    private static void funcConsumer(Semaphore semFill, Semaphore semEmpty, Queue<Integer> qProduct) {
        for (;;) {
            try {
                semFill.acquire();

                int data = qProduct.remove();
                System.out.println("Consumer " + data);

                semEmpty.release();
            }
            catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

}