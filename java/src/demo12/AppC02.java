/*
 * RACE CONDITION AND DATA RACE
*/

package demo12;



public class AppC02 {

    public static void main(String[] args) throws InterruptedException {
        var thA = new Thread(() -> {
            try { Thread.sleep(30); } catch (InterruptedException e) { }

            while (Global.counter < 10)
                ++Global.counter;

            System.out.println("A won !!!");
        });


        var thB = new Thread(() -> {
            try { Thread.sleep(30); } catch (InterruptedException e) { }

            while (Global.counter > -10)
                --Global.counter;

            System.out.println("B won !!!");
        });


        thA.start();
        thB.start();
    }

}



class Global {
    static int counter = 0;
}