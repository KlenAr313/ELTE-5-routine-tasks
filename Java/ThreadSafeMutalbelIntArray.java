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

    public static void main(String[] args)
    {
        int[] a = {1,2,3,4};
        ThreadSafeMutalbelIntArray array = new ThreadSafeMutalbelIntArray(a);
        public static Thread[] threads = new Thread[5];
        for(int j = 0; j< 5; ++j)
        {
            final int ind = j;
            threads[i] = new Thread(()->{
                System.out.print(array.get(ind));
            });
        }
    }
}