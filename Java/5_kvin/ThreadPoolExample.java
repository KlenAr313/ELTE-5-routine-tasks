import java.util.concurrent.Callable;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.TimeUnit;

public class ThreadPoolExample {
	public static void main(String[] args) throws Exception {
		int POOL_SIZE = 2;
//		int POOL_SIZE = 10;

		simpleSubmit(args, POOL_SIZE);
		submitWithReturn(args);
	}

	public static void simpleSubmit(String[] args, int POOL_SIZE) throws Exception {
		int THREAD_COUNT = 10;
		int STEP_COUNT = 10_000;

		var pool = Executors.newFixedThreadPool(POOL_SIZE);

		for (int i = 0; i < THREAD_COUNT; i++) {
			var threadIdx = i;
			pool.submit(() -> {
				for (int j = 0; j < STEP_COUNT; j++) {
					System.out.printf("Thread %d step %d%n", threadIdx, j);
				}
			});
		}

		pool.shutdown();
		// Note: see what happens if you remove the call to awaitTermination.
		pool.awaitTermination(1, TimeUnit.DAYS);

		System.out.println("Done");
	}

	private static void submitWithReturn(String[] args) throws Exception {
		int STEP_COUNT = 10_000;

		var pool = Executors.newFixedThreadPool(2);

		Callable<Integer> counter = () -> {
			int sum = 0;
			for (int j = 0; j < STEP_COUNT; j++) {
				sum += ThreadLocalRandom.current().nextInt(10);
			}
			return sum;
		};

		Future<Integer> future1 = pool.submit(counter);
		Future<Integer> future2 = pool.submit(counter);
		Future<Integer> future3 = pool.submit(counter);

		int result1 = future1.get();
		int result2 = future2.get();
		int result3 = future3.get();
		int result = result1 + result2 + result3;

		System.out.printf("Result: %d = %d + %d + %d%n", result, result1, result2, result3);

		pool.shutdown();
	}
}
