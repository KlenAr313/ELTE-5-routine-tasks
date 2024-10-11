import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.TimeUnit;

public class LoanDif {
    private static int POOL_SIZE = 4;
    private static int clientCount = 10;
    private static int STEP_COUNT = 10000;
    private static int loanAccount = 0;
    private static int[] clinetLoans = new int[clientCount];
    private static Object lock = new Object();

    public static void main(String[] args) throws InterruptedException{
        ExecutorService pool = Executors.newFixedThreadPool(POOL_SIZE);
        for (int i = 0; i < clientCount; i++) {
            final int ind = i;
            for (int j = 0; j < STEP_COUNT; j++) {
                int loan = ThreadLocalRandom.current().nextInt(10, 10_000);
                synchronized (lock) {
                    loanAccount += loan;
                } 
                clinetLoans[ind] += loan;
            }
        }
        pool.shutdown();
        pool.awaitTermination(1, TimeUnit.MINUTES);
        System.out.println(loanAccount);
        int total = 0;
        for (int i : clinetLoans) {
            total += i;
        }
        System.out.println(total == loanAccount);
    }
}
