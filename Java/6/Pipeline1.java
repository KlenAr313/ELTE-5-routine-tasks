import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class Pipeline1 {
    public static void main(String[] args) throws Exception {
        var NO_FURTHER_INPUT1 = "";
        var NO_FURTHER_INPUT2 = -1;

        var bq1 = new ArrayBlockingQueue<String>(1024);
        var bq2 = new ArrayBlockingQueue<String>(1024);

        var pool = Executors.newCachedThreadPool();

        pool.submit(() -> {
            bq1.addAll(List.of("a", "bb", "ccccccc", "ddd", "eeee", NO_FURTHER_INPUT1));
        });

        pool.submit(() -> {
            try {
                while (true) {
                    String element = bq1.take();
                    if(element.equals(NO_FURTHER_INPUT1)){
                        bq2.put(NO_FURTHER_INPUT2);
                        break;
                    }
                    else{
                        bq2.put(element.len);
                    }
                    // TODO queue #1 ====> txt  len ===> queue #2
                    // TODO also handle NO_FURTHER_INPUTs
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        });

        pool.submit(() -> {
            try {
                while (true) {
                if(element.equals(NO_FURTHER_INPUT2) && element.equals(NO_FURTHER_INPUT2)){
                    break;
                }
                else{
                    System.out.println(bq2.len);
                }
                    // TODO queue #2 ====> len ====> print it
                    // TODO also handle NO_FURTHER_INPUTs
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        });

        pool.shutdown();
        pool.awaitTermination(10, TimeUnit.SECONDS);
    }
}
