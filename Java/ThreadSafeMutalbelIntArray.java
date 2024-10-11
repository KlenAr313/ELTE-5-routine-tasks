public class ThreadSafeMutalbelIntArray
{
    private int[] data;
    private Object[] locks;

    public ThreadSafeMutalbelIntArray(int[] data){
        this.data = java.util.Arrays.copyOf(data, data.length);
        locks = new Object[data.length];
        for (int i = 0; i < locks.length; ++i){
            locks[i] = new Object();
        }
    }

    public int get(int index)
    {
        synchronized (locks[index]){
            return data[index];
        }
    }

    public static Thread[] threads = new Thread[5];
    public static void main(String[] args)
    {
        int[] a = {1,2,3,4,5};
        ThreadSafeMutalbelIntArray array = new ThreadSafeMutalbelIntArray(a);
        for(int j = 0; j< 5; ++j)
        {
            final int ind = j;
            threads[j] = new Thread(()->{
                System.out.print(array.get(ind));
            });
            threads[j].start();
        }
    }
}