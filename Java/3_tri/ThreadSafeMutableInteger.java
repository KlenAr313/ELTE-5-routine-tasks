import java.util.function.IntUnaryOperator;

class ThreadSafeMutableInteger
{
    private int num;

    public void ThreadSafeMutableInteger(int a){
        num = a;
    }

    public void ThreadSafeMutableInteger()
    {
        num = 0;
    }

    public synchronized int get()
    {
        return num;
    }

    public synchronized void set(int a)
    {
        num = a;
    }

    public synchronized int getAndIncement()
    {
        return num++;
    }
    
    public synchronized int getAndDecrement()
    {
        return num--;
    }
    
    public synchronized int getAndAdd(int a)
    {
        int temp = num;
        num += a;
        return temp;
    }

    public synchronized int incrementAndGet()
    {
        return ++num;
    }
    
    public synchronized int decrementAndGet()
    {
        return --num;
    }

    public synchronized int addAndGet(int a)
    {
        num += a;
        return num;
    }

    public synchronized int getAndUpdate(IntUnaryOperator op)
    {
        int temp = num;
        num = op.applyAsInt(num);
        return temp;
    }

    public synchronized int updateAndGet(IntUnaryOperator op)
    {
        num = op.applyAsInt(num);
        return num;
    }
}